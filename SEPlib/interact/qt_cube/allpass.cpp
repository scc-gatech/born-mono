#include<seplib.h>
#include<allpass.h>

allpass::allpass(hypercube *hyper, int n_w, float *pp){

  int ndim=hyper->get_ndim();
  if(ndim==1 || ndim >3) seperr("allpass data must be >1 and <4");
    n1=hyper->get_axis(1).n;
  if(ndim==2) n3=1;
  else n2=hyper->get_axis(3).n;
  n2=hyper->get_axis(2).n;
  nw=n_w;
  dat=pp;
}
void allpass::deriv(hypercube_float *in, hypercube_float *out,int j){
 

  out->set_val(0.);
  int i,ibig;
  for(int i3=0,ibig=0; i3 < n3; i3++){
    for(int i2=0; i2 < n2; i2++,ibig+=n1){
      for(int i1=nw,i=ibig; i1 < n1-nw; i1++,ibig++){
        if(nw==1)//Compiler please move this loop
          do_deriv_1(in->vals,out->vals,i,j);
        else if(nw==2)
          do_deriv_2(in->vals,out->vals,i,j);
        else if(nw==3)
          do_deriv_3(in->vals,out->vals,i,j);
       }
     }
   }
}
void allpass::pass(hypercube_float *in, hypercube_float *out,int j){
 

  out->set_val(0.);
  int i,ibig;
  for(int i3=0,ibig=0; i3 < n3; i3++){
    for(int i2=0; i2 < n2; i2++,ibig+=n1){
      for(int i1=nw,i=ibig; i1 < n1-nw; i1++,ibig++){
        if(nw==1)//Compiler please move this loop
          do_pass_1(in->vals,out->vals,i,j);
        else if(nw==2)
          do_pass_2(in->vals,out->vals,i,j);
        else if(nw==3)
          do_pass_3(in->vals,out->vals,i,j);
       }
     }
   }
}