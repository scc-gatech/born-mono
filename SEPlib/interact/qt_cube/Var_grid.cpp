
#include<sregf.h>
#include<sep_params.h>
#include <iostream>
#include "var_trace_grid.h"


main(int argc, char **argv){

  sep_params *pars=new sep_params(argc,argv);
  sregf *off_file=new sregf("in");


  hypercube_float *headers=new hypercube_float(off_file);
  int icmpx=pars->get_int("cmp_x");
  int icmpy=pars->get_int("cmp_y");
  int ioff=pars->get_int("offset");
  int nh=headers->get_axis(1).n;
  int ntr=headers->get_axis(2).n;
  float *cmpx=new float[ntr];
  float *cmpy=new float[ntr];
  float *off=new float[ntr];
  
  
  off_file->read_all("in",headers);
  
  for(int i=0; i < ntr; i++){
    cmpx[i]=headers->vals[i*nh+icmpx];
    cmpy[i]=headers->vals[i*nh+icmpy];
    off[i]=headers->vals[i*nh+ioff];
  }
 
 
 int nmax=pars->get_int("nmax",20);
 int   noff=pars->get_int("noff");
 float ooff=pars->get_float("ooff");
 float doff=pars->get_float("doff");
 int   ncmpx=pars->get_int("ncmpx");
 float ocmpx=pars->get_float("ocmpx");
 float dcmpx=pars->get_float("dcmpx"); 
 int   ncmpy=pars->get_int("ncmpy");
 float ocmpy=pars->get_float("ocmpy");
 float dcmpy=pars->get_float("dcmpy");
 var_trace_grid *grid=new var_trace_grid(noff,ooff,doff,
                                        ncmpx,ocmpx,dcmpx,
                                        ncmpy,ocmpy,dcmpy,
                                        nmax);
 
 grid->build_grid_expand(ntr,cmpx,cmpy,off);
 
 
 float loc[3];
 loc[0]=pars->get_float("loc_o");
 loc[1]=pars->get_float("loc_x");
 loc[2]=pars->get_float("loc_y"); 
 
 int iloc=grid->get_trace_num(loc);
 fprintf(stderr,"trace number %d %f %f %f\n",iloc,
   off[iloc],cmpx[iloc],cmpy[iloc]);
 float dist,dmin;
 dmin=9999999;
 int imin;
 for(int i=0; i < ntr; i++){
   float d1=(cmpx[i]-loc[1]); d1=d1*d1;
      float d2=(cmpy[i]-loc[2]); d2=d2*d2;
   float d3=(off[i]-loc[0]); d3=d3*d3;
    dist=d1+d2+d3;
    if(dmin> dist){
       dmin=dist;
       imin=i;
    }
   }
   fprintf(stderr,"CLOSEST IS %d %f %f %f =%f\n",imin,off[imin],cmpx[imin],cmpy[imin],dmin);
 
 
 
}
