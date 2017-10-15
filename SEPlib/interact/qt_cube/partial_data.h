#ifndef partial_data_H
#define partial_data_H 1
#include "dataset.h"
class partial_data:public dataset{
  
   public:
   
  
     partial_data(){};
      ~partial_data(){};
     virtual void set_contains();
     hypercube *datas;


};

class partial_data_float: public partial_data{
  public:
  partial_data_float(){};
  partial_data_float(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im=1);
   ~partial_data_float(){
      
      clean_bufs();
   }
    buffer *create_buffer(orient_cube *pos, int iax1, int iax2);
};

class partial_data_byte: public partial_data{
 public:
  partial_data_byte(){};
  partial_data_byte(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im=1);
   ~partial_data_byte(){
      clean_bufs();
   }
  buffer *create_buffer(orient_cube *pos, int iax1, int iax2);
};

#endif
