#ifndef OUTCORE_DATA_H
#define OUTCORE_DATA_H 1
#include "dataset.h"
class outcore_data:public dataset{
  
   public:
   
  
     outcore_data(){};
      ~outcore_data(){};
  
     void delete_dataset(orient_cube *pos, int iax1,int iax2);
     void hold_pattern(orient_cube *pos, int iax1, int iax2, bool *hold);

     hypercube *datas;



};

class outcore_data_float: public outcore_data{
  public:
  outcore_data_float(){};
  outcore_data_float(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im=5);
   ~outcore_data_float(){
      
      clean_bufs();
   }
    buffer *create_buffer(orient_cube *pos, int iax1, int iax2);
};

class outcore_data_byte: public outcore_data{
 public:
  outcore_data_byte(){};
  outcore_data_byte(std::string title,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im=5);
   ~outcore_data_byte(){
      clean_bufs();
   }
  buffer *create_buffer(orient_cube *pos, int iax1, int iax2);
};

#endif
