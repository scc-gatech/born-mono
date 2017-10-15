#include "sep_reg_mmap_io.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h> /* mmap() is defined in this header */
#include <fcntl.h>

void sep_reg_mmap_io::set_sep_basics(QString tagit, util *pars){
 struct stat statbuf;
   strcpy(tag,tagit.ascii());
   fdes=new sreg(tag);
   set_basics(pars,fdes);
   std::string str=fdes->get_string("in");
   char temp_ch[1024];
  
   memcpy((void*)temp_ch,(const void*)str.c_str(),(1+str.size()));
   int fd = open(temp_ch, O_RDONLY);
    if (fd == -1) {
	     perror("Error opening file for reading");
     	exit(EXIT_FAILURE);
    }
   if (fstat (fd,&statbuf) < 0)
     perror ("fstat error");

   map = (char*)mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fd, 0);
      /*
      if (map == MAP_FAILED) {
   	   close(fd);
	     perror("Error mmapping the file");
	     exit(EXIT_FAILURE);
    }
    */

}

sep_reg_mmap_io_byte::sep_reg_mmap_io_byte(QString tagit,util *pars){
   set_sep_basics(tagit,pars);
   bclip=fdes->get_float("minval",0);
   eclip=fdes->get_float("maxval",255);
   non_byte=false;
}
sep_reg_mmap_io_float::sep_reg_mmap_io_float(QString tagit,util *pars){
   set_sep_basics(tagit,pars);
   non_byte=true;
}
void sep_reg_mmap_io_float::read_block_float(int *nw, int *fw, float *buf){
  int ng[8], jw[8];
  
  for(int i=0; i < 8;i++){ ng[i]=jw[i]=1;}
  std::vector<int> ns=fdes->return_ns();
  for(int i=0; i< fdes->get_ndim(); i++) {
    ng[i]=ns[i];
    if(ng[i]!=nw[i]) perror("In mmap mode entire dataset needs to be buffered");
  }
  int ierr=sreed_window_new(tag,8,ng,nw,fw,jw,4,buf);
  if(ierr!=0) {
    char errmsg[] = "Trouble reading buffer \n";
    par->error(errmsg);
  }
}
void sep_reg_mmap_io_float::read_block_byte(int *nw, int *fw, unsigned char *buf)
{
  long long n123=1;
  for(int i=0; i <8 ;i++) n123=n123*(long long)nw[i];
  float *tbuf=new float[n123];
  read_block_float(nw,fw,tbuf);
  par->convert_to_byte(tbuf,0,buf,0,n123,bclip,eclip);
  delete [] tbuf;
}
void sep_reg_mmap_io_byte::read_block_byte(int *nw, int *fw, unsigned char *buf){
  int ng[8], jw[8];
  
  for(int i=0; i < 8;i++){ ng[i]=jw[i]=1;}
  std::vector<int> ns=fdes->return_ns();
  for(int i=0; i< fdes->get_ndim(); i++) {
    ng[i]=ns[i];
    if(ng[i]!=nw[i]) perror("In mmap mode entire dataset needs to be buffered");
  }
  int ierr=sreed_window_new(tag,8,ng,nw,fw,jw,1,buf);

  if(ierr!=0) {
    char errmsg[] ="Trouble reading buffer \n";
    par->error(errmsg);
  }
}
void sep_reg_mmap_io_byte::read_block_float(int *nw, int *fw, float *buf)
{
  long long n123=1;
  for(int i=0; i <8 ;i++) n123=n123*(long long)nw[i];
  unsigned char *tbuf=new unsigned char[n123];
  read_block_byte(nw,fw,tbuf);
  par->convert_to_float(buf,0,tbuf,0,n123,bclip,eclip);
  delete [] tbuf;
}

