#include "nmoed.h"
#include <math.h>
#include "interval_pick.h"
using namespace SEP;
nmoed::nmoed(std::shared_ptr<hypercube >grid, std::shared_ptr<dataset>dat, int it,int ioff, int imes,
 std::shared_ptr<paramObj>pars, 
 std::shared_ptr<pick_draw>pk, QString col1, QString col2){
 fprintf(stderr,"FAIL 1 \n");
    std::shared_ptr<util> pp(new util(pars));
  par=pp;
  fprintf(stderr,"FAIL2 \n");
  _grid=grid;
  _dat=dat;
  _it=it;
  _ioff=ioff;
  _imes=imes;
  _pk=pk;
  _col1=col1;
  _col2=col2;
    this->set_moveout_basics(_grid,_dat,_it,_ioff,_imes,par,_pk,_col1,_col2);

  smute=pars->getFloat("smute",1.5);
  
}
void nmoed::afterInitialization(){
  this->moveoutAfterInitialization();



}


void nmoed::move_it(std::vector<int>&f,float *nmo){
 int it,i2;
 float offset,offovs,tnmute;
 float tn,ti;


fprintf(stderr,"IN MOVE IT\n");
 axis taxis=grid->getAxis(i_t+1);
 axis oaxis=grid->getAxis(i_off+1);
 axis maxis=grid->getAxis(i_mes+1);
 int   nt=taxis.n,  nh=oaxis.n;
 float dt=taxis.d, dh=oaxis.d,dv=maxis.d;
 float ot=taxis.o, oh=oaxis.o,ov=maxis.o;

 for(int i=0; i < 8; i++) dumb->set_loc(i,f[i]);
  
 float *dat=data->get_float_data(dumb, i_t, 0, nt, i_off, 0, nh);






 
 //CHANGE
int  iloc[8];
for(int i=0; i < 8; i++) iloc[i]=f[i];
iloc[i_off]=-1;
iloc[i_t]=-1;
iloc[1]=-1;


 std::shared_ptr<pairs_new> myp=pks->get_pts_sort(dumb,iloc,i_t,i_mes,  col1);

 if(myp->size()==0) myp->add_point(maxis.n/2,1);
 float *vel=new float[nt];
 myp->build_int_line(0,nt,vel);
 
 
for(int i=0; i < nt; i++) vel[i]=ov+dv*vel[i];
 int iti;
 float frac;
	for(i2=0; i2 < nh; i2++){
		offset=oh+dh*i2;
      for (it=0,tn=ot; it<nt; ++it,tn+=dt) {
				offovs=offset*offset/(vel[it]*vel[it]);
      	tnmute = sqrt(offovs/(smute*smute-1.0));
        ti = (sqrt(tn*tn+offovs)-ot)/dt;
        iti = (int)ti;
        if (iti<nt-1 && tnmute < tn) {
          frac = ti-iti;
			    nmo[it+i2*nt]=dat[iti+i2*nt]*(1.-frac)+dat[iti+1+i2*nt]*frac;
				}
				else nmo[it+i2*nt]=0;
			}
		}
//	interval_pick::form_interval_vel(dumb,grid,ds,pm1,pm2,i_mes,i_t,i_off);
	changed=false;
  delete [] dat;
  delete []vel;
 // srite("tmp.H",nmo,nt*nh*4);
  
 }
