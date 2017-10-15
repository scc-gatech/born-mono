
#ifndef MOVE_SEMBLANCE_H
#define MOVE_SEMBLANCE_H 1
#include "dataset.h"
#include<string>
#include "io_func.h"
#include "pick_draw.h"
class  move_semblance: public io_func{

  public:
   move_semblance(){};
   void set_moveout_basics(hypercube *grid, dataset *dat,int it, int ioff, int imes,
     util *pars,pick_draw *pck,QString col1, QString col2);



   virtual void read_block_float(int *nw, int *fw, float *buf);
   virtual void read_block_byte(int *nw, int *fw, unsigned char *buf);
   virtual void semblance_it(int *f,float *nmo){if(f==0 && nmo==0){}}
     dataset *data;
     int i_off,i_mes,i_t;
     float os[8],ds[8];
     util *par;
     orient_cube *dumb;
     float *num,*den,*nnz;
     hypercube *grid;
     pick_draw *pks;
     QString col1,col2;


 };
#endif
