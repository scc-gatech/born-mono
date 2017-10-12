#include <nmo_semblance.h>
#include <math.h>
#include "interval_pick.h"
using namespace SEP;

nmo_semblance::nmo_semblance(std::shared_ptr<hypercube>grid, std::shared_ptr<dataset>dat, int it,int ioff, int imes,
       std::shared_ptr<paramObj>pars,std::shared_ptr<pick_draw>pk1, QString col1, QString col2){
 fprintf(stderr,"before moveout basics \n");
 
 
 this->set_moveout_basics(grid,dat,it,ioff,imes,pars,pk1,col1,col2); 
 no_sem=pars->getInt("no_sem",0);
  nsmooth=pars->getInt("nsmooth",8);

 smute=pars->getFloat("smute",1.5);
       
}

void nmo_semblance::semblance_it(int *f, float *semb){

 int iv,it,itmute,i2,iti;
 float offset,v,offovs,temp,tnmute;
 float tn,ti,frac;
 
 axis taxis=grid->getAxis(i_t+1);
 axis oaxis=grid->getAxis(i_off+1);
 axis maxis=grid->getAxis(i_mes+1);
 int   nt=taxis.n, nv=maxis.n, nh=oaxis.n;
 float dt=taxis.d, dv=maxis.d, dh=oaxis.d;
 float ot=taxis.o, ov=maxis.o, oh=oaxis.o;
 for(int i=0; i < 8; i++) dumb->set_loc(i,f[i]);
 float *dat=data->get_float_data(dumb, i_t, 0, nt, i_off, 0, nh);

  if(no_sem==1){
    for(iv=0; iv < nv; iv++){
      for(it=0; it < nt; it++){
  		semb[it+iv*nt]=iv;  
  	  }
    }
    return;
  }
  for (iv=0; iv<nv*nt; ++iv) {
     num[iv] = 0.0;
     den[iv] = 0.0;
     nnz[iv] = 0.0;
 }


for(i2=0; i2 < nh; i2++){
  offset=oh+dh*i2;
  for (iv=0,v=ov; iv<nv; ++iv,v+=dv) {
 
    /* compute offset/velocity squared */
    offovs = (offset*offset)/(v*v);
    /* determine mute time after nmo */
    temp =offovs/(smute*smute-1.0);
    tnmute =sqrt(temp);
    if (tnmute > ot) {
      itmute =(int)( (tnmute-ot)/dt);
    } else {
        itmute = 0 ;
    }

      /* do nmo via quick and dirty linear interpolation
         (accurate enough for velocity analysis) and
         accumulate semblance numerator and denominator
      */

   for (it=itmute,tn=ot+itmute*dt; it<nt; ++it,tn+=dt) {
     ti = (sqrt(tn*tn+offovs)-ot)/dt;
        iti =(int) ti;
        if (iti<nt-1) {
          frac = ti-iti;
          temp = (1.0-frac)*dat[iti+nt*i2]+
            frac*dat[iti+1+nt*i2];
           // \(stderr,"in if conditional %d %d %d %f \n",it,iv,i2,dat[iti+nt*i2]);
          if (temp!=0.0) {
            num[iv*nt+it] += temp;
            den[iv*nt+it] += temp*temp;
            nnz[iv*nt+it] += 1.0;
          }
        }
			}
		}

 }
 float nsum,dsum;
 int ismin,ismax,itout;
 for (iv=0; iv<nv; ++iv) {

        /* compute semblance quotients */
        for (itout=0; itout<nt; ++itout) {
          it = itout;
          ismin = it-nsmooth/2;
          ismax = it+nsmooth/2;
          if (ismin<0) ismin = 0;
          if (ismax>nt-1) ismax = nt-1;
          nsum = dsum = 0.0;
  
          for (int is=ismin; is<ismax; ++is) {
            nsum += num[iv*nt+is]*
              num[iv*nt+is];
            dsum += nnz[iv*nt+is]*
              den[iv*nt+is];
          }
          semb[itout+iv*nt] = (dsum!=0.0?nsum/dsum:0.0);
              //   fprintf(stderr,"SETTING DAT %f %f %d %d %f \n",nsum,dsum,itout,iv,
                //      semb[itout+iv*nt]);
        }
			

	}

 delete [] dat;


}
 void nmo_semblance::update_vel(int *iloc){
   for(int i=0; i < 8; i++) dumb->set_loc(i,iloc[i]);
   interval_pick::form_interval_vel(dumb,grid,ds,pks,col1,col2,i_mes,i_t,i_off);
}
