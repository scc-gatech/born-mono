#include<nmo_semblance.h>
#include "move_semblance.h"
using namespace SEP;
void move_semblance::set_moveout_basics(std::shared_ptr<hypercube>g, std::shared_ptr<dataset>dat, int it,int ioff, 
int imes, std::shared_ptr<paramObj>pars, std::shared_ptr<pick_draw>_pk, QString a, QString b){
  
  data=dat;
  std::shared_ptr<util> pp(new util(pars));
  par=pp;

  grid=g;
  i_t=it;
  i_off=ioff;
  i_mes=imes;
  std::vector<axis> axes;
  for(int i =0; i < 8; i++) axes.push_back(axis(1));
  axes[i_t]=axis(grid->getAxis(i_t+1));
    axes[i_mes]=axis(grid->getAxis(i_mes+1));
  std::shared_ptr<hypercube>myh(new hypercube());
   myh->setAxes(axes);
  set_basics(par,myh);
  //changed=true;
  for(int i=0; i < 8; i++) {
    axis a=grid->getAxis(i+1);
    os[i]=a.o;
    ds[i]=a.d;
  }
  std::shared_ptr<orient_cube> oo(new orient_cube(grid));
  dumb=oo;
  nnz=new float[axes[i_t].n*axes[i_mes].n];
  num=new float[axes[i_t].n*axes[i_mes].n];
  den=new float[axes[i_t].n*axes[i_mes].n];
 col1=a; col2=b;
 
 

 pks=_pk;
}
void move_semblance::moveSemblanceAfterInitialization(){
 pks->ignore_axis(i_off);
}

  
  



void move_semblance::read_block_float(std::vector<int>&nw, std::vector<int>&fw, float *buf){
  //For now we always redo the NMO
  if(nw[i_t]!=grid->getAxis(i_t+1).n ||
    nw[i_mes]!=grid->getAxis(i_mes+1).n)
     par->error("Internal error in measure");
    semblance_it(fw,buf);
 }  

void move_semblance::read_block_byte(std::vector<int>&nw, std::vector<int>&fw, unsigned char *buf){
    if(nw[i_t]!=grid->getAxis(i_t+1).n ||
    nw[i_mes]!=grid->getAxis(i_mes+1).n)
     par->error("Internal error in measure");
     float *tbuf=new float[nw[i_t]*nw[i_off]];
     read_block_float(nw,fw,tbuf);
     set_clip(tbuf,0,nw[i_t]*nw[i_mes]);
     convert_to_byte(tbuf,0,buf,0,nw[i_t]*nw[i_mes]);
     delete [] tbuf;
}


