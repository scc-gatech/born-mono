#include<lloyd_1d.h>
#include<vector>
#include<sregf.h>
#include<sep_params.h>
#include<hypercube_float.h>
int main(int argc, char **argv){

  sep_params *sep_pars=new sep_params(argc,argv);
  
  sregf *dat=new sregf("in");
  fprintf(stderr,"INIT SREG\n");
  
  std::vector<float> tr;
  
  axis *axes=dat->return_axes(3);
  fprintf(stderr,"GRAB AXES\n");
  axis aout[3];
  for(int i=0; i< axes[0].n; i++) tr.push_back(0.);
  
  hypercube_float *datf=new hypercube_float(3,axes);
  float_1d *out=new float_1d(2000);
  fprintf(stderr,"GRAB AXES\n");
  
  dat->read_all("in",datf);
fprintf(stderr,"DIE 1 \n");
  lloyd_1d_fit *fit;
  int niter=sep_pars->get_int("niter",50);
  float f;
  
  aout[0]=axis(40);
  aout[1]=axes[1];
  aout[2]=axes[2];
  
fprintf(stderr,"DIE 2 %d %d %d\n",aout[0].n,aout[1].n,aout[2].n);
  sreg *outs=new sreg(3,aout);
fprintf(stderr,"DIE 3 \n");
  outs->write_description("out");
  int e=8;
  putch("esize","d",&e);
  fprintf(stderr,"GRAB AXES\n");
  
  int ic;
  for(int i3=0; i3 < axes[2].n; i3++){
    for(int i2=0; i2 < axes[1].n; i2++){
      for(int i1=0; i1 < axes[0].n; i1++) 
        tr[i1]=
        datf->vals[i1+i2*axes[0].n+i3*axes[0].n*axes[1].n];
fprintf(stderr,"BEFORE FIT \n");
      fit=new lloyd_1d_fit(tr,40,.0001);
fprintf(stderr,"BEFORE FIT2\n");
      for(int iter=0; iter < niter; iter++){
fprintf(stderr,"BEFORE FIT3\n");
         fit->do_iter(iter);
         for(int i=0; i< fit->center.size(); i++){
        f=fit->center[i];
        out->vals[i*2]=axes[0].o+axes[0].d*f;
        ic=f; f-=ic;
        out->vals[i*2+1]=(1.-f)*tr[ic]+f*tr[ic+1];
      }
       out->vals[fit->center.size()*2]=axes[0].o+axes[0].d*(axes[0].n-1);
          for(int i=fit->center.size()*2+1; i < 2000; i++) out->vals[i]=out->vals[i-2];
         srite("out",out->vals,8000);
      for(int i=0; i < 1000; i++){
        out->vals[i*2]=axes[0].o+axes[0].d*i;
        out->vals[i*2+1]=tr[i];
      }
      srite("out",out->vals,8000);
      
fprintf(stderr,"BEFORE FIT4\n");
      }
      
      for(int i=0; i< fit->center.size(); i++){
        f=fit->center[i];
        out->vals[i*2]=axes[0].o+axes[0].d*f;
        ic=f; f-=ic;
        out->vals[i*2+1]=(1.-f)*tr[ic]+f*tr[ic+1];
      }
      for(int i=fit->center.size()*2; i < 80; i++) out->vals[i]=-1.;
    }
    delete fit;
    srite("out",out->vals,320);
   }
  
  
  
  }
