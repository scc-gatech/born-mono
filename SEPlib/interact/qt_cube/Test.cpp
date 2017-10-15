
#include<sregf.h>
#include<sep_params.h>
#include <iostream>
#include "nmo.h"


main(int argc, char **argv){

  sep_params pars=sep_params(argc,argv);
  sregf *off_file=new sregf("in");

  sregf *slow_file=new sregf("slow"); 

  hypercube_float *slow=new hypercube_float(slow_file);
  hypercube_float *sect=new hypercube_float(off_file);
  hypercube_float *sout=new hypercube_float(off_file);

  slow_file->read_all("slow",slow);
  off_file->read_all("in",sect);
  axis ax=off_file->get_axis(1);
  axis sx=slow_file->get_axis(1);

<<<<<<< .mine
fprintf(stderr,"AA \n");
  for(int i=0; i <sout->get_axis(2).n; i++) slow->vals[i]=slow->vals[i]*slow->vals[i];
fprintf(stderr,"AA0 \n");
=======
>>>>>>> .r336

  float *ss=new float[sx.n+1];
  for(int i=0; i < sx.n; i++) ss[i]=slow->vals[i]*slow->vals[i];
  ss[sx.n]=ss[sx.n-1];

  nmo *nm=new nmo(ax.n,ax.d,ax.o,sx.n+1,sx.d,sx.o);

  float hin=pars.get_float("hin");
  float hout=pars.get_float("hout");

  for(int i2=0; i2 < sout->get_axis(2).n; i2++){
    nm->dnmo3_it(ss,hin,hout,&sect->vals[i2*ax.n], &sout->vals[i2*ax.n]);

  }
  off_file->write_all("out",sout);
}
