#include "trace_io.h"
using namespace SEP;
void trace_io::set_trace_basics( QString fl, std::shared_ptr<hypercube>d, int reel_h,int head, bool sw, std::shared_ptr<util>p,int i){

  set_basics(p,d);
  strcpy(file,fl.toAscii().constData());
  fd=fopen(file,"r");

  loaded=false;
  reel_head=reel_h;
  header=head;
  swap=sw;
  inum=i;
  long long n=1;
  for(int i=1; i < d->getNdim(); i++)  {
  	n=n*(long long)d->getAxis(i+1).n;
    }
  header_buf=new char[header*n];
  

}
void trace_io::read_block(int *nw, int *fw, unsigned char *buf, int nbytes){
  //We are going to either trace by trace or slice by slice
  
  
  bool slice=false;
  char *tbuf;
  std::vector<axis> axes=hyper->getAxes();
  for(int i=axes.size(); i< 8; i++) axes.push_back(axis(1));
  bool first=false;
  long long  block[8];
  block[0]=nbytes;
  block[1]=nbytes*axes[0].n+header;
  
  long long nblock=1,ntot=nw[0]*nw[1];
  for(int i=2
  ; i < 8; i++) {
    block[i]=block[i-1]*axes[i-1].n;
    ntot=ntot*(long long)nw[i];
  }
  if(nw[1]==axes[1].n){
    slice=true;
    nblock=nw[1]*(axes[0].n*nbytes+header);
    tbuf=new char[nblock];
  }
  else{
    nblock=axes[0].n*nbytes+header;
    tbuf=new char[nblock];
  }
  if(nw[0]==axes[0].n ) first=true;
  int trlen=(axes[0].n*nbytes+header);
  loaded=true;
    long long to=0;
    for(int i7=0; i7 < nw[7]; i7++){              long long s7=block[7]*(i7+fw[7]);
     for(int i6=0; i6 < nw[6]; i6++){             long long s6=s7+block[6]*(i6+fw[6]);
       for(int i5=0; i5 < nw[5]; i5++){           long long s5=s6+block[5]*(i5+fw[5]);
         for(int i4=0; i4 < nw[4]; i4++){         long long s4=s5+block[4]*(i4+fw[4]);
           for(int i3=0; i3 < nw[3]; i3++){       long long s3=s4+(long long)(block[3]*(i3+fw[3]));
             for(int i2=0; i2 < nw[2]; i2++){     long long s2=s3+(long long)(block[2]*(long long)((i2+fw[2])));
               if(slice){
                  long long itr=s2/(long long)block[1];
                  if(0!=fseek(fd,s2+(long long)reel_head,0)) par->error("trouble seeking");
                  int ierr=fread((void*) tbuf,1,nblock,fd);

              if(ierr!=nblock){ fprintf(stderr,"ERR %d %d\n",ierr,(int)nblock); par->error("trouble reading");}
        
          for(int i1=0; i1 < nw[1]; i1++){
                    if(first) {
                      memcpy((void*)(buf+to),(const void*)(tbuf+trlen*i1+header),nw[0]*nbytes);
                      memcpy((void*)(header_buf+(itr+i1)*header), (const void*)(tbuf+trlen*i1),header);
                      to+=nw[0]*nbytes;
                    }
                    else{
                   
                      for(int i0=0; i0 < nw[0] ;i0++){
                    
                        memcpy((void*)(buf+to),
                         (const void*)(tbuf+trlen*i2+header+(i0+fw[0])*nbytes), nbytes);
                       memcpy((void*)(header_buf+(itr+i1)*header), (const void*)(tbuf+trlen*i1),header);


                         to+=nbytes*nw[0];
                      }
                   }
                 }
               }
               else{
                 for(int i1=0; i1 < nw[1]; i1++){     long long s1=s2+block[1]*(i1+fw[1]);

                   if(0!=fseek(fd,s1+(long long)reel_head,0)) par->error("trouble seeking");
                   int ierr=(int)fread((void*) tbuf,1,nblock,fd);
                    long long itr=s1/(long long)block[1];

                   if(ierr!=nblock)par->error("trouble reading");
                   if(first){
                     memcpy((void*)(buf+to),(const void*)(tbuf+header),nw[0]*nbytes);
                     memcpy((void*)(header_buf+(itr)*header), (const void*)(tbuf),header);

                     to+=nw[0]*nbytes;
                   }
                   else{
                     for(int i0=0; i0< nw[0];i0++){
                       memcpy((void*)(buf+to),(const void*)(tbuf+header+(i0+fw[0])*nbytes),
                         nbytes);
                      memcpy((void*)(header_buf+(itr)*header), (const void*)(tbuf),header);

                      to+=nbytes;
                   }
                  }
                }
               }
             }
           }
         }
       }
     }
   }
   

   delete [] tbuf;
   if(swap){
      pars->swap_float_bytes(ntot*nbytes/4,(float*)buf);
  }
}

void trace_io::read_block_float(int *nw, int *fw, float *buf){
  read_block(nw,fw,(unsigned char *)buf,sizeof(float));
}
void trace_io::read_block_byte(int *nw, int *fw, unsigned char *buf)
{
  long long n123=1;
  for(int i=0; i <8 ;i++) n123=n123*(long long)nw[i];
  float *tbuf=new float[n123];
  read_block_float(nw,fw,tbuf);
  par->convert_to_byte(tbuf,0,buf,0,n123,bclip,eclip);
  delete [] tbuf;
}

