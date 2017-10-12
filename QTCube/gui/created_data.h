#ifndef created_data_H
#define created_data_H 1
#include "dataset.h"
namespace SEP{
class created_data:public dataset{
  
   public:
   
  
     created_data(){};
      ~created_data(){};
     virtual void set_contains();
     virtual bool match_grid();
     std::shared_ptr<hypercube>datas;


};

class created_data_float: public created_data{
  public:
  created_data_float(){};
  created_data_float(std::string title,QString nm,std::shared_ptr<SEP::hypercube>g,std::shared_ptr<SEP::io_func>i, std::shared_ptr<SEP::paramObj>p, int in,int im=1);
   ~created_data_float(){
      
      clean_bufs();
   }
    std::shared_ptr<buffer>create_buffer(std::shared_ptr<SEP::orient_cube>pos, int iax1, int iax2);
};

class created_data_byte: public created_data{
 public:
  created_data_byte(){};
  created_data_byte(std::string title, QString nm,std::shared_ptr<SEP::hypercube>g,std::shared_ptr<SEP::io_func>i, std::shared_ptr<SEP::paramObj>p, int in,int im=1);
   ~created_data_byte(){
      clean_bufs();
   }
  std::shared_ptr<buffer>create_buffer(std::shared_ptr<SEP::orient_cube>pos, int iax1, int iax2);
};
}
#endif
