#include<sep_params.h> 
#include<sregf.h>
#include<hypercube_float.h>
#include<float_1d.h>
#include<float_2d.h>
#include<lloyd_1d.h>
 
 
 main(int argc, char **argv){
   
   sep_params pars=sep_params(argc,argv);
   sregf linef=sregf("in");
   hypercube_float lineh=hypercube_float(&linef);
   sreed("in",lineh.vals,(int)lineh.get_n123()*4);
   std::vector<float> line;
   for(int i=0; i< lineh.get_n123();i++) line.push_back(lineh.vals[i]);
   
   int nreg=pars.get_int("nreg",(int)line.size()/5);
   float err=pars.get_float("err",.01);
   lloyd_1d_p fit=lloyd_1d_p(line,nreg,err);
   fit.writeit=true;
      fprintf(stderr,"SIZE %d \n",fit.center.size());

   for(int i=0; i < 500; i++)  fit.do_iter(i);
   
   
   
   int eight=8;putch("esize","d",&eight);
   int n=fit.center.size(); putch("n1","d",&n);
   
   float o=linef.get_axis(1).o;
   float d=linef.get_axis(1).d;
   
   fprintf(stderr,"SIZE %d \n",n);
   for(int i=0; i < n; i++){
      lineh.vals[i*2]=o+d*fit.center[i];
      lineh.vals[i*2+1]=line[(int)(0.5+fit.center[i])];
   }
   float *backf=new float[lineh.get_n123()];
   for(int i=0; i < lineh.get_n123(); i++) backf[i]=fit.back[i];
   srite("out",lineh.vals,8*n);
   
   srite("error",backf,4*(int)lineh.get_n123());
 }
   
   
   
  