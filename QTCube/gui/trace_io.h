#ifndef TRACE_IO_H
#define TRACE_IO_H 1
#include "io_func.h"
#include "util.h"
#include "hypercube.h"

namespace SEP{
class trace_io: public io_func{


  public:
 
   trace_io(){ header_buf=0;}
   ~trace_io(){ if(header_buf!=0) delete [] header_buf;      
  }
  
   void read_block(int *nw, int *fw, unsigned char *buf,int nbytes);
   void set_trace_basics(QString fl, std::shared_ptr<SEP::hypercube>d, int reel_h,int head, bool sw, std::shared_ptr<SEP::util>pars,int in);
   void read_block_float(int *nw, int *fw, float *buf);
   void read_block_byte(int *nw, int *fw, unsigned char *buf);
   char file[1024];
   bool swap;
   int reel_head,header,inum;
   FILE *fd;
   std::shared_ptr<SEP::hypercube >des;
   std::shared_ptr<SEP::util>pars;
   char *header_buf;
   std::vector<QString> headers;
   std::vector<int> hoff;
   bool loaded;
  

};
}
#endif
