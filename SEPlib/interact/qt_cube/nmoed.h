#include "moved_out.h"
#ifndef NMOED_H
#define NMOED_H 1
#include<string>
#include "datasets.h"
#include "pick_draw.h"
class nmoed: public moved_out{

  public:
     nmoed(){};
     nmoed(hypercube *grid, dataset *dat, int it,int ioff, int imes,
       util *pars, pick_draw *pk,QString col1, QString col2);
     virtual void move_it(int *f,float *nmo);
     float smute;
     bool done;
     
 };
#endif
