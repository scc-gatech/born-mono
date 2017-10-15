#include "sep_reg_io.h"

void sep_reg_io::set_sep_basics(QString tagit, util *pars){


   strcpy(tag,tagit.ascii());
   fdes=new sreg(tag);
   set_basics(pars,fdes);

}

sep_reg_io_byte::sep_reg_io_byte(QString tagit,util *pars){
   set_sep_basics(tagit,pars);
   bclip=fdes->get_float("minval",0);
   eclip=fdes->get_float("maxval",255);
   non_byte=false;
}
sep_reg_io_float::sep_reg_io_float(QString tagit,util *pars){

   set_sep_basics(tagit,pars);
   non_byte=true;
}
void sep_reg_io_float::read_block_float(int *nw, int *fw, float *buf){
  int ng[8], jw[8];
  
  for(int i=0; i < 8;i++){ ng[i]=jw[i]=1;}
  std::vector<int> ns=fdes->return_ns();
  for(int i=0; i< fdes->get_ndim(); i++) ng[i]=ns[i];

  int ierr=sreed_window_new(tag,8,ng,nw,fw,jw,4,buf);
  if(ierr!=0) {
    char errmsg[] = "Trouble reading buffer \n";
    par->error(errmsg);
  }
}
void sep_reg_io_float::read_block_byte(int *nw, int *fw, unsigned char *buf)
{
  long long n123=1;
  for(int i=0; i <8 ;i++) n123=n123*(long long)nw[i];
  float *tbuf=new float[n123];
  read_block_float(nw,fw,tbuf);
  par->convert_to_byte(tbuf,0,buf,0,n123,bclip,eclip);
  delete [] tbuf;
}
void sep_reg_io_byte::read_block_byte(int *nw, int *fw, unsigned char *buf){
  int ng[8], jw[8];
  
  for(int i=0; i < 8;i++){ ng[i]=jw[i]=1;}
  std::vector<int> ns=fdes->return_ns();
  for(int i=0; i< fdes->get_ndim(); i++) ng[i]=ns[i];

  int ierr=sreed_window_new(tag,8,ng,nw,fw,jw,1,buf);

  if(ierr!=0) {
    char errmsg[] = "Trouble reading buffer \n";
    par->error(errmsg);
  }
}
void sep_reg_io_byte::read_block_float(int *nw, int *fw, float *buf)
{
  long long n123=1;
  for(int i=0; i <8 ;i++) n123=n123*(long long)nw[i];
  unsigned char *tbuf=new unsigned char[n123];
  read_block_byte(nw,fw,tbuf);
  par->convert_to_float(buf,0,tbuf,0,n123,bclip,eclip);
  delete [] tbuf;
}
