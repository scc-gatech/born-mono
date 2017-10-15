#include<nmo_semblance.h>
#include "move_semblance.h"
   
void move_semblance::set_moveout_basics(hypercube *g, dataset *dat, int it,int ioff, 
int imes, util *pars, pick_draw *_pk, QString a, QString b){
  par=pars;
  data=dat;


  grid=g;
  i_t=it;
  i_off=ioff;
  i_mes=imes;
  std::vector<axis> axes;
  for(int i =0; i < 8; i++) axes.push_back(axis(1));
  axes[i_t]=axis(grid->get_axis(i_t+1));
    axes[i_mes]=axis(grid->get_axis(i_mes+1));
  hypercube *myh=new hypercube();
   myh->set_axes(axes);
  set_basics(par,myh);
  //changed=true;
  for(int i=0; i < 8; i++) {
    axis a=grid->get_axis(i+1);
    os[i]=a.o;
    ds[i]=a.d;
  }
  dumb=new orient_cube(grid); 
  nnz=new float[axes[i_t].n*axes[i_mes].n];
  num=new float[axes[i_t].n*axes[i_mes].n];
  den=new float[axes[i_t].n*axes[i_mes].n];
 col1=a; col2=b;
 pks=_pk;
 pks->ignore_axis(ioff);
}

  
  



void move_semblance::read_block_float(int *nw, int *fw, float *buf){
  //For now we always redo the NMO
  if(nw[i_t]!=grid->get_axis(i_t+1).n ||
    nw[i_mes]!=grid->get_axis(i_mes+1).n) {
    char errmsg[] = "Internal error in measure";
     par->error(errmsg);
    }
    semblance_it(fw,buf);
 }  

void move_semblance::read_block_byte(int *nw, int *fw, unsigned char *buf){
    if(nw[i_t]!=grid->get_axis(i_t+1).n ||
    nw[i_mes]!=grid->get_axis(i_mes+1).n) {
     char errmsg[] = "Internal error in measure";
     par->error(errmsg);
    }
     float *tbuf=new float[nw[i_t]*nw[i_off]];
     read_block_float(nw,fw,tbuf);
     set_clip(tbuf,0,nw[i_t]*nw[i_mes]);
     convert_to_byte(tbuf,0,buf,0,nw[i_t]*nw[i_mes]);
     delete [] tbuf;
}


