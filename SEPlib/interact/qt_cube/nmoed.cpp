#include "nmoed.h"
#include <math.h>
#include "seplib.h"
#include "interval_pick.h"
nmoed::nmoed(hypercube *grid, dataset *dat, int it,int ioff, int imes, util *pars, 
 pick_draw *_pk, QString col1, QString col2){
  
  set_moveout_basics(grid,dat,it,ioff,imes,pars,_pk,col1,col2);
  char tag[] = "smute";
  smute=par->get_float(tag,1.5);
  
}


void nmoed::move_it(int *f,float *nmo){
 int it,i2;
 float offset,offovs,tnmute;
 float tn,ti;

 axis taxis=grid->get_axis(i_t+1);
 axis oaxis=grid->get_axis(i_off+1);
 axis maxis=grid->get_axis(i_mes+1);
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


 pairs_new *myp=pks->get_pts_sort(dumb,iloc,i_t,i_mes,  col1);

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
