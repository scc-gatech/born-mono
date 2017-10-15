#ifndef INTERVAL_PICK_H
#define INTERVAL_PICK_H 1
#include<string>
#include "pick_draw.h"
#include "orient_cube.h"
class interval_pick{

  public:

    static bool form_interval_vel(orient_cube *pos,hypercube *grid, float *ds, pick_draw *pks, 
    QString col1, QString col2,
      int i_t,int i_off, int i_mes);

     
 };
#endif
