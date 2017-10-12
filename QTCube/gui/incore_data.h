#ifndef INCORE_DATA_H
#define INCORE_DATA_H 1
#include "dataset.h"
namespace SEP{
class incore_data:public dataset{
  
   public:
   
  
     incore_data(){;}
      ~incore_data(){ };
  
     std::shared_ptr<SEP::hypercube >datas;
     int mmap_it;

};

class incore_data_float: public incore_data{
  public:
  incore_data_float(){};
  incore_data_float(std::string title,QString nm,std::shared_ptr<SEP::hypercube>g,
  std::shared_ptr<io_func> i, std::shared_ptr<paramObj> p, int in,int mmap,int im=1);
   ~incore_data_float(){
      
      clean_bufs();
   }
    buffer *create_buffer(SEP::orient_cube *pos, int iax1, int iax2);
};

class incore_data_byte: public incore_data{
 public:
  incore_data_byte(){};
  incore_data_byte(std::string title,QString nm,std::shared_ptr<SEP::hypercube>g,std::shared_ptr<io_func>i, 
  std::shared_ptr<paramObj>p, int in,int im=1);
   ~incore_data_byte(){
      clean_bufs();
   }
  std::shared_ptr<buffer> create_buffer(std::shared_ptr<SEP::orient_cube >pos, int iax1, int iax2);
};
}
#endif
