#include<flatten_fourier.h>
#include<math.h>
#include<seplib.h>
flatten_fourier::flatten_fourier(hypercube_float *delta, flatten_op *oper,int n_1, 
int n_2, int n_3, float e,int i){
  op=oper;
  n1=n_1; n2=n_2; n3=n_3; eps_t=e;
  int zero=0;
  fftw_test2_(&n1,&n2,&n3,&zero,delta->vals,(long long*)&planF,(long long*)&planI);
  n123=n1*n2*n3;

  
  fprintf(stderr,"MULT ALLOC %d  %d %d %d\n",(int)n123,n1,n2,n3);
  mult=new float[n123];

  create_mult();
  iloc=i;

}

void flatten_fourier::inverse(hypercube_float *mod, hypercube_float *dat,
  hypercube_float *delta){
  dat->scale(-1.);
  op->forward(true,mod,dat);
  dat->info("dat");
  
  op->adjoint(false,delta,dat);
  delta->info("delta");
  srite("ff.H",dat->vals,n123*12);
  srite("x.H",delta->vals,n123*4);
  fftwf_execute(planF);
  
  for(int i=0; i < mod->get_n123(); i++) delta->vals[i]*=mult[i];
  
  fftwf_execute(planI);
  float *base=new float [n1];
  memcpy(base,&delta->vals[iloc*n1],n1*4);
  for(int i2=0; i2 <n2*n3; i2++) 
    for(int i1=0; i1 < n1; i1++) delta->vals[i1+i2*n1]-=base[i1];
  srite("xx.H",delta->vals,n123*4);
  delete [] base;

}
    
    
    
void flatten_fourier::create_mult(){
  float *f1=new float[n1];
  float *f2=new float[n2];
  float *f3=new float[n3];
  double pi=acos(-1.)*2.;
  float et2=eps_t*eps_t;
  


    float in1=1./(n1*2.),in2=1./(n2*2.), in3=1./(n3*2.);
    
    for(int i=0; i <n1; i++) f1[i]=in1*i;
    for(int i=0; i <n2; i++) f2[i]=in2*i;
    for(int i=0; i <n3; i++) f3[i]=in3*i;
 
    int ib=0;
    float k1,k2,k3,ct1,ct2,ct3,ct4,ct5,ct6,ct7,tmp;
    for(int i3=0; i3 < n3; i3++){
      ct3=-cos(f3[i3]*pi)-cos(-f3[i3]*pi)+4 +2.*et2;
      for(int i2=0; i2 < n2; i2++){
        ct2=-cos(f2[i2]*pi)-cos(f2[i2]*pi);    
        for(int i1=0; i1 < n1; i1++,ib++){
          tmp=ct3+ct2-et2*(cos(-f1[i1]*pi)+cos(f1[i1]*pi));
 //         fprintf(stderr,"WHAT HTE %d %d %d -%d \n",i1,i2,i3,ib);
          if(fabs(tmp)>.000001) mult[ib]=in1*in2*in3*tmp/(tmp*tmp+.000000001);
          else mult[ib]=0.;
          
        }
      }
    }

    delete [] f1; delete []f2; delete [] f3;
  }
