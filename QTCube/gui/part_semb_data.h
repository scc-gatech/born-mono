#ifndef INCORE_SEMB_DATA_H
#define INCORE_SEMB_DATA_H 1
#include "partial_data.h"
#include "pick_draw.h"
namespace SEP{
class part_semb_data_byte:public partial_data_byte{
  
   public:
   
  
     part_semb_data_byte(){};
     part_semb_data_byte(std::string title,QString nm,std::shared_ptr<SEP::hypercube>g,std::shared_ptr<SEP::io_func>i, int i_t,int i_off, int i_mes,
       std::shared_ptr<SEP::pick_draw>pk,QString s1, QString s2,std::shared_ptr<SEP::paramObj>p, int in,int im=1);
      ~part_semb_data_byte(){};
    void add_pt_action(int *iloc){update_vel(iloc);}
    void del_pt_action(int *iloc){update_vel(iloc);}
    void update_vel(int *iloc);
    int it, im, ioff;
    std::shared_ptr<SEP::orient_cube>dumb;
    std::shared_ptr<SEP::pick_draw> pks;
    QString col1,col2;
    float ds[8];

};

}

#endif
