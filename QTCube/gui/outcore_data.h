#ifndef OUTCORE_DATA_H
#define OUTCORE_DATA_H 1
#include "dataset.h"
namespace SEP{

class outcore_data:public dataset{
  
   public:
   
  
     outcore_data(){};
      ~outcore_data(){};
  
     void delete_dataset(std::shared_ptr<orient_cube >pos, int iax1,int iax2);
     void hold_pattern(std::shared_ptr<orient_cube >pos, int iax1, int iax2, bool *hold);

     std::shared_ptr<hypercube>datas;



};

class outcore_data_float: public outcore_data{
  public:
  outcore_data_float(){};
  outcore_data_float(std::string title,QString nm,std::shared_ptr<hypercube>g,std::shared_ptr<io_func>i, std::shared_ptr<paramObj >p, int in,int im=5);
   ~outcore_data_float(){
      
      clean_bufs();
   }
    std::shared_ptr<buffer>create_buffer(std::shared_ptr<orient_cube >pos, int iax1, int iax2);
};

class outcore_data_byte: public outcore_data{
 public:
  outcore_data_byte(){};
  outcore_data_byte(std::string title,QString nm,std::shared_ptr<hypercube>g,std::shared_ptr<io_func>i, std::shared_ptr<paramObj>p, int in,int im=5);
   ~outcore_data_byte(){
      clean_bufs();
   }
  std::shared_ptr<buffer>create_buffer(std::shared_ptr<orient_cube>pos, int iax1, int iax2);
};
}
#endif
