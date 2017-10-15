#include "io_func.h"
#ifndef MOVED_OUT_H
#define MOVED_OUT_H 1
#include<string>
#include "datasets.h"
#include "pick_draw.h"
class moved_out: public io_func{

  public:
     moved_out(){};
     void set_moveout_basics(hypercube *grid, dataset *dat, int it, int ioff,
       int imes, util *pars,pick_draw *_pk, QString col1, QString col2);
     virtual void read_block_float(int *nw, int *fw, float *buf);
     virtual void read_block_byte(int *nw, int *fw, unsigned char *buf);
     virtual void move_it(int *f,float *nmo){ if(f==0 && nmo==0){}}
     dataset *data;
     int i_off,i_mes,i_t;
     pick_draw *pks;
     QString col1,col2;
     float os[8],ds[8];
     util *par;
     orient_cube *dumb;
     float *num,*den,*sum;
     hypercube *grid;
     
 };
#endif
