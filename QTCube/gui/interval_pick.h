#ifndef INTERVAL_PICK_H
#define INTERVAL_PICK_H 1
#include<string>
#include "pick_draw.h"
#include "orient_cube.h"
namespace SEP{
class interval_pick{

  public:

    static bool form_interval_vel(std::shared_ptr<SEP::orient_cube>pos,std::shared_ptr<SEP::hypercube>grid, float *ds, std::shared_ptr<SEP::pick_draw>pks, 
    QString col1, QString col2,
      int i_t,int i_off, int i_mes);

     
 };
 }
#endif
