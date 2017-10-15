#include "sep_params.h"
#include "match_reg.h"
#include "sregf.h"

int main(int argc, char **argv){

  sep_params pars=sep_params(argc,argv);
  
  float emax=pars.get_float("max_err",.03);
  int niter=pars.get_int("niter",10000);
  sregf infile=sregf("in");
  hypercube_float input=hypercube_float(&infile);
  
  infile.read_all("in",&input);
  std::vector<int> split_axis;
  for(int i=0; i < infile.get_ndim_g1();i++) split_axis.push_back(i);
  //fprintf(stderr,"SIZE IS %d \n",(int)split_axis.size());
  //assert(1==0);
  fprintf(stderr,"WHAT HTE HELL %d %d %d\n",(int)input.get_n123(),input.get_axis(1).n,input.get_axis(2).n);
  
  match_reg mr=match_reg(split_axis,&input,niter);
  mr.break_up(emax);


  hypercube_float *estimate=mr.compute_back();
  
  infile.write_all("out",estimate);

  delete estimate;
 
 
 
 
 }
 