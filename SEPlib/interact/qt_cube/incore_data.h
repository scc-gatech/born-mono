#ifndef INCORE_DATA_H
#define INCORE_DATA_H 1
#include "dataset.h"
class incore_data:public dataset{
  
   public:
   
  
     incore_data(){map=0;};
      ~incore_data(){ if(map!=0) delete[] map;};
  
     hypercube *datas;
     int mmap_it;
     int *map;

};

class incore_data_float: public incore_data{
  public:
  incore_data_float(){};
  incore_data_float(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int mmap,int im=1);
   ~incore_data_float(){
      
      clean_bufs();
   }
    buffer *create_buffer(orient_cube *pos, int iax1, int iax2);
};

class incore_data_byte: public incore_data{
 public:
  incore_data_byte(){};
  incore_data_byte(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im=1);
   ~incore_data_byte(){
      clean_bufs();
   }
  buffer *create_buffer(orient_cube *pos, int iax1, int iax2);
};

#endif
