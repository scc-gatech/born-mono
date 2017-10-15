#include <stdlib.h>
#include "seispak_reg_io.h"
#include "assert.h"

seispak_reg_io_float::seispak_reg_io_float(QString tagit,hypercube* d, bool sw, int in,util *pars){
   set_trace_basics(tagit,d,0,240,sw,pars,in);
  int ihdr[125];
  float fhdr[125];
  char bhdr[3000];
  int sz=2000; 
  if((size_t) sz != fread((void*)bhdr,1,sz,fd)) {
     perror("seispak_reg_io_float: ");
     exit(EXIT_FAILURE);
  }
  memcpy((void*)fhdr,(const void*)(bhdr+12),500);
  memcpy((void*)ihdr,(const void*)(bhdr+12),500);
  fseek(fd,0,0);
  
   
  header=ihdr[1]*4; reel_head=(ihdr[0]+2)*4;
  fclose(fd);  
     set_trace_basics(tagit,d,reel_head,header,sw,pars,in);
  
  if((size_t) reel_head != fread((void*)bhdr,1,reel_head,fd)) {
     perror("seispak_reg_io_float: ");
     exit(EXIT_FAILURE);
  }
  int ibeg=(ihdr[14]-1)*4+12;
  for(int i=0; i < ihdr[1]; i++){
    QString val;
    int j=0;
    while(j<4  ) {
     //fprintf(stderr,"CHECK IT %d %c \n",ibeg+i*8+j,bhdr[ibeg+i*8+j]);
      val=val+bhdr[ibeg+i*8+j];j++;
    }
    headers.push_back(val);
     float x; memcpy(&x,&bhdr[ibeg+i*8+4],4);
   int y=(int)(x+.5);
  y=std::max(0,std::min(y,ihdr[1]-1));
  
 hoff.push_back(y);
 

  }
  fflush(stderr);
//fprintf(stderr,"CHECK THIS %d %d  %d %d\n",ihdr[0],ihdr[1],header,reel_head);
//fprintf(stderr,"XX %d %d \n",ihdr[14],ihdr[15]);
   non_byte=true;
 
 /*
 int ihdr[40];
  float fhdr[40];
  char bhdr[3000];
  int sz=100; 
  set_trace_basics(tagit,d,0,240,sw,pars,in);

  if((size_t) sz != fread((void*)bhdr,1,sz,fd)) {
     perror("seispak_reg_io_float: ");
     exit(EXIT_FAILURE);
  }
  memcpy((void*)fhdr,(const void*)(bhdr+12),160);
  memcpy((void*)ihdr,(const void*)(bhdr+12),160);
  //axes[0].n=ihdr[2];  axes[0].d=fhdr[3]; axes[1].n=ihdr[4]; axes[2].n=ihdr[5];
    fseek(fd,0,0);
  header=ihdr[1]*4; reel_head=(ihdr[0]+2)*4;
  fclose(fd);
  
  if((size_t) reel_head != fread((void*)bhdr,1,reel_head,fd)) {
     perror("seispak_reg_io_float: ");
     exit(EXIT_FAILURE);
  }
  int ibeg=(ihdr[14]-1)*4+12;
  for(int i=0; i < ihdr[1]; i++){
    QString val;
    int j=0;
    while(j<4 && bhdr[ibeg+i*8+j]!=' ') {
      val=val+bhdr[ibeg+i*8+j];j++;
    }
    headers.push_back(val);
    float x; memcpy(&x,&bhdr[ibeg+i*8+4],4);
 hoff.push_back((int)(x+.5)-1);
  }
  //fread((void*)buf,1,nglobal-52,fd);
   non_byte=true;
   */
}
seispak_reg_io_short::seispak_reg_io_short(QString tagit,hypercube* d, bool sw, int in,util *pars){
  set_trace_basics(tagit,d,0,240,sw,pars,in);
  int ihdr[125];
  float fhdr[125];
  char bhdr[3000];
  int sz=2000; 
  if((size_t) sz != fread((void*)bhdr,1,sz,fd)) {
     perror("seispak_reg_io_short: ");
     exit(EXIT_FAILURE);
  }
  memcpy((void*)fhdr,(const void*)(bhdr+12),500);
  memcpy((void*)ihdr,(const void*)(bhdr+12),500);
  fseek(fd,0,0);
  
   
  header=ihdr[1]*4; reel_head=(ihdr[0]+2+ihdr[1]*2)*4;
  fclose(fd);
   set_trace_basics(tagit,d,reel_head,header,sw,pars,in);

  if((size_t) reel_head !=  fread((void*)bhdr,1,reel_head,fd)) {
     perror("seispak_reg_io_short: ");
     exit(EXIT_FAILURE);
  }
  int ibeg=(ihdr[14]-1)*4+12;
  for(int i=0; i < ihdr[1]; i++){
    QString val;
    int j=0;
    while(j<4  ) {
     // fprintf(stderr,"CHECK IT %d %c \n",ibeg+i*8+j,bhdr[ibeg+i*8+j]);
      val=val+bhdr[ibeg+i*8+j];j++;
    }
    headers.push_back(val);
     float x; memcpy(&x,&bhdr[ibeg+i*8+4],4);
   int y=(int)(x+.5);
   y=std::min(0,std::max(y,ihdr[1]-1));
 hoff.push_back(y);  }

//fprintf(stderr,"CHECK THIS %d %d  %d %d\n",ihdr[0],ihdr[1],header,reel_head);
//fprintf(stderr,"XX %d %d \n",ihdr[14],ihdr[15]);
   non_byte=true;
}
void seispak_reg_io_short::read_block_float(int *nw, int *fw, float *buf){
  int nwt[8]; memcpy(nwt,nw,8*sizeof(int));
  if(nw[0]==1){
    //Unimplimented two reads first the scaling parameter
    char errmsg[] = "Unimplemented seispak short read non-1st axis";
    par->error(errmsg);
  }
  else{
    int f=0;
    long long n123=1;
    if(((int)nwt[0]/2)*2!=nwt[0]){
      nwt[0]+=1;
      f=1;
    }
    n123=nwt[0]+2;
    for(int i=1; i <8 ;i++) n123=n123*(long long)nwt[i];

    short *tbuf=new  short[n123];
    nwt[0]=nwt[0]+2;
    axis a1=hyper->get_axis(1),a2=hyper->get_axis(1);
    a2.n=a2.n+2+f;

    hyper->set_axis(1,a2);
    float scale;
  

    read_block(nwt,fw,(unsigned char *)tbuf,sizeof(short));
    for(int j=0; j < n123/nwt[0]; j++){
     memcpy(&scale,&tbuf[j*nwt[0]],4);
     //scale=scale/32000.;
      for(int i=0; i < nw[0]; i++){
        buf[i+nw[0]*j]=(float)tbuf[(i+nwt[0]*j+1+f)]/scale;
        
  
      }
    }
  
    hyper->set_axis(1,a1);
    delete [] tbuf;
  }
}
void seispak_reg_io_short::read_block_byte(int *nw, int *fw, unsigned char *buf){
  long long n123=1;
  for(int i=0; i <8 ;i++) n123=n123*(long long)nw[i];
  float *tbuf=new float[n123];
  read_block_float(nw,fw,tbuf);
  par->convert_to_byte(tbuf,0,buf,0,n123,bclip,eclip);
  delete [] tbuf;
}
std::vector<QString> seispak_reg_io::get_header_val(long long inum){
  std::vector<QString> send;
for(int i=0; i< (int)headers.size(); i++){
    float *h=(float*)&header_buf[inum*header]; 
    send.push_back(QString::number(h[hoff[i]]));
  }
  return send;
}
std::vector<QStringList> seispak_reg_io::get_header(long long inum){
  std::vector<QStringList> send;
  for(int i=0; i< ((int)(headers.size())); i++){
    //float val;
    QStringList single;
    single.push_back(headers[i]);
    if(loaded){
    float *h=(float*)&header_buf[inum*header];


    single.push_back(QString::number(h[hoff[i]]));
    }
    else{
      single.push_back("0.0");
    }
 send.push_back(single);
  }
  return send;
}
