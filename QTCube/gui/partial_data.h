#ifndef partial_data_H
#define partial_data_H 1
#include "dataset.h"
namespace SEP{
class partial_data:public dataset{
  
   public:
   
  
     partial_data(){};
      ~partial_data(){};
     virtual void set_contains();
     std::shared_ptr<SEP::hypercube> datas;


};

class partial_data_float: public partial_data{
  public:
  partial_data_float(){};
  partial_data_float(std::string title,QString nm,std::shared_ptr<SEP::hypercube> g,std::shared_ptr<SEP::io_func> i, std::shared_ptr<SEP::paramObj>p, int in,int im=1);
   ~partial_data_float(){
      
      clean_bufs();
   }
    std::shared_ptr<buffer> create_buffer(std::shared_ptr<SEP::orient_cube>pos, int iax1, int iax2);
};

class partial_data_byte: public partial_data{
 public:
  partial_data_byte(){};
  partial_data_byte(std::string title,QString nm,std::shared_ptr<SEP::hypercube>g,std::shared_ptr<io_func>i, std::shared_ptr<SEP::paramObj>p, int in,int im=1);
   ~partial_data_byte(){
      clean_bufs();
   }
  std::shared_ptr<buffer>create_buffer(std::shared_ptr<SEP::orient_cube>pos, int iax1, int iax2);
};
}
#endif
