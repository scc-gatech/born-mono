#ifndef INCORE_SEMB_DATA_H
#define INCORE_SEMB_DATA_H 1
#include "partial_data.h"
#include "pick_draw.h"
class part_semb_data_byte:public partial_data_byte{
  
   public:
   
  
     part_semb_data_byte(){};
     part_semb_data_byte(std::string title,QString nm,hypercube *g,io_func *i, int i_t,int i_off, int i_mes,
       pick_draw *pk,QString s1, QString s2,param_func *p, int in,int im=1);
      ~part_semb_data_byte(){};
    void add_pt_action(int *iloc){update_vel(iloc);}
    void del_pt_action(int *iloc){update_vel(iloc);}
    void update_vel(int *iloc);
    int it, im, ioff;
    orient_cube *dumb;
    pick_draw *pks;
    QString col1,col2;
    float ds[8];

};



#endif
