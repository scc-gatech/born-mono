#include "flat_nonlin.h"

flat_nonlin::flat_nonlin(hypercube_float *dp, hypercube_float *tau, float eps,
  std::vector<long long>fixed, int ir){
  flat_op *f_op=new flat_op(tau,dp,eps);
  hypercube_mask_pts *m_op=new hypercube_mask_pts(fixed);
  dips=dp;
  iref=ir;
  st=cgstep();
  solv=new lin_solver_basic(st);
  solv->add_equation(d,f_op);
  solv->add_mask(m_op);
  
}
void flat_nonlin::update_tau(int non_lin, int niter_lin, hypercube_float *tau){
  
  for(int in_iter=0; in_iter< non_lin; in_iter++){
    hypercube_float *dat=create_data(dips,tau,iref);
    solv->update_d(0,dat);
    solv->update_m0(tau);
    solv->create_b();
    hypercube_float *tnew=solv->solve(niter_lin);
    tau->add_scale(0.,tnew,1.);
    delete tnew; delete dat;
  }
  

}
hypercube_float *flat_nonlin::create_data( hypercube_float *dips, hypercube_float *tau, 
  int iloc2){
  
 
   std::vector<axis> axes=dips->return_axes(4);
   int n1=axes[0].n, n2=axes[1].n, n3=axes[2].n;
   axes[3].n=3;
  hypercube_float *d=new hypercube_float(axes);
  
  
   int iloc=n1*iloc2,ib,it;
   long long n123=n1*n2*n3,i=0;
   float f;
   for(int i3=0; i3 < n3; i3++){
    for(int i2=0; i2 < n2; i2++){
       ib=n1*i2+i3*n1*n2;
       for(int i1=0; i1 < n1; i1++,i++){
          f=tau->vals[i]-tau->vals[i1+iloc]+i1;
          it=(int)f; f-=it;
          d->vals[i]=0.;
          if(it >0 && it < n1-1){
          d->vals[i+n123]=(1.-f)*dips->vals[it+ib]+f*dips->vals[it+ib+1];
          d->vals[i+n123*2]=(1.-f)*dips->vals[it+ib+n123]+f*dips->vals[it+n123+ib+1];
         }
         else {
           d->vals[i+n123]=d->vals[i+n123*2]=0.;}
       }
    }
  }
  return d;
  
}

    
  