#include "moved_out.h"
using namespace SEP;
void moved_out::set_moveout_basics(std::shared_ptr<hypercube>g, std::shared_ptr<dataset>dat, int it,int ioff, int imes, 
 std::shared_ptr<util>pars,   std::shared_ptr<pick_draw>_pk, QString c1, QString c2){
  par=pars;
  data=dat;
  pks=_pk;
  col1=c1;
  col2=c2;
  grid=g;
  
  i_t=it;
  i_off=ioff;
  i_mes=imes;
  std::vector<axis> axes;
  for(int i =0; i < 8; i++) axes.push_back(axis(1));
  axes[i_t]=axis(grid->getAxis(i_t+1));
  axes[i_off]=axis(grid->getAxis(i_off+1));
  std::shared_ptr<hypercube>myh (new hypercube());
  myh->setAxes(axes);
  
  set_basics(par,myh);
  changed=true;
  for(int i=0; i < 8; i++) {
    axis a=grid->getAxis(i+1);
    os[i]=a.o;
    ds[i]=a.d;
  }
  std::shared_ptr<orient_cube> oo(new orient_cube(grid));
  dumb=oo;

}
void moved_out::moveoutAfterInitialization(){


}
  
  



void moved_out::read_block_float(std::vector<int>&nw, std::vector<int>&fw, float *buf){
  //For now we always redo the NMO
  if(nw[i_t]!=grid->getAxis(i_t+1).n ||
    nw[i_off]!=grid->getAxis(i_off+1).n)
     par->error("Internal error in measure");
     
     fprintf(stderr,"in read block float \n");
    move_it(fw,buf);
    fprintf(stderr,"AFTER READ BLOCK FLOAT \n");
    changed=true;
 }  

void moved_out::read_block_byte(std::vector<int>&nw, std::vector<int>&fw, unsigned char *buf){
    if(nw[i_t]!=grid->getAxis(i_t+1).n ||
    nw[i_off]!=grid->getAxis(i_off+1).n)
     par->error("Internal error in measure");
     fprintf(stderr,"IN READ BLOCK BYTE \n");
     float *tbuf=new float[nw[i_t]*nw[i_off]];
     read_block_float(nw,fw,tbuf);
     fprintf(stderr,"CEHCK CLIP %d %d \n",nw[i_t]*nw[i_mes]);
     set_clip(tbuf,0,nw[i_t]*nw[i_mes]);
     convert_to_byte(tbuf,0,buf,0,nw[i_t]*nw[i_mes]);
     delete [] tbuf;
     
}


