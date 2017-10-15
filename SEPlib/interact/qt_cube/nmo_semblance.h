#include "move_semblance.h"
#ifndef NMO_SEMBLANCE_H
#define NMO_SEMBLANCE_H 1
#include<string>
#include "pick_draw.h"
#include "datasets.h"
class nmo_semblance: public move_semblance{

  public:
     nmo_semblance(){};
     nmo_semblance(hypercube *grid, dataset *dat, int it,int ioff, int imes,
       util *pars,pick_draw *pk,QString col1, QString col2);
       
     virtual void semblance_it(int *f,float *semb);

    int no_sem;
    float smute;
    int nsmooth;
    void add_pt_action(int *iloc){update_vel(iloc);}
    void del_pt_action(int *iloc){update_vel(iloc);}
    void update_vel(int *iloc);

     
 };
#endif
