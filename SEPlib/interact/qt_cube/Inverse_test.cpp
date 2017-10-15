
#include<sregf.h>
#include<sep_params.h>
#include "lin_solver.h"
#include "float_1d.h"
#include "cgstep.h"
#include "hybrid_norm.h"
#include "generic_norm.h"
#include "mat_mult.h"
#include "l2_norm.h"
#include "hypercube_mask.h"
#include "filt_1d.h"
#include "helicon.h"
#include "diagonal_wt.h"
#include "polydiv.h"
#include "i_op.h"
#include <iostream>
#include "combo_oper.h"
#include "mask_op.h"
void mat_mult_reg();
void mat_mult_prec();

void mat_mult_test();
void rms_interval();


class deriv_it: public my_operator{
  public:
    deriv_it(){}
    deriv_it(hypercube_float *mod){
     set_domain(mod);
  set_range(mod);
    
    }
    bool forward(bool add, my_vector *model, my_vector *data){
    
    hypercube_float *m=(hypercube_float*) model; 
     hypercube_float *d=(hypercube_float*) data;
     if(!add) d->set_val(0.);
     int n=(int)m->get_n123();
     for(int i=1; i <n; i++){
        d->vals[i]+=(m->vals[i]-m->vals[i-1])*scale;
    }
    return true;
  //  d->vals[0]+=scale*(m->vals[n-1]-m->vals[n-2]);
  }
    bool adjoint(bool add, my_vector *model, my_vector *data){
    
 hypercube_float *m=(hypercube_float*) model;
hypercube_float *d=(hypercube_float*) data;

   if(!add) m->set_val(0.);
          int n=(int)m->get_n123();

    for(int i=1; i <n; i++){
      m->vals[i]+=scale*d->vals[i];
      m->vals[i-1]-=scale*d->vals[i];
    }
  //  m->vals[n-1]+=scale*d->vals[n-1];
    //m->vals[n-2]-=scale*d->vals[n-2];
return true;
 
    }
    

};

main(int argc, char **argv){

  sep_params pars=sep_params(argc,argv);
  
  
//  mat_mult_test();
 // mat_mult_reg();
 // mat_mult_prec();
  rms_interval();

}
void mat_mult_reg(){
  float *a1=new float[30];
  float *a=new float[30];
  a[0]=-55; a[1]=-90; a[2]=-24;a[3]=-13; a[4]=-73; a[5]=61; a[6]=-27; a[7]=-19; a[8]=23;a[9]=-55;
  a[10]=8; a[11]=-86; a[12]=72;a[13]=87; a[14]=-41; a[15]=-3; a[16]=-29; a[17]=29; a[18]=-66;a[19]=50;
  a[20]=84; a[21]=-49; a[22]=80;a[23]=44; a[24]=-52; a[25]=-51; a[26]=8; a[27]=86; a[28]=77;a[29]=50;
  float *a2=new float[100];
    

  float_1d *dat=new float_1d(3);
  dat->vals[0]=41; dat->vals[1]=33; dat->vals[2]=-58;
  int i=0;
  for(int i2=0; i2 < 3; i2++){
    for(int i1=0; i1 < 10; i++,i1++){
       a1[i2+i1*3]=a[i1+i2*10];
     }
    }
    i=0;for(int i2=0; i2 < 10; i2++){
     for(int i1=0; i1 < 10; i1++,i++){
       if(i1==i2) a2[i]=1;
       else if(i1==i2+1) a2[i]=-1;
       else a2[i]=0.;
    }
  }

  matmult_float_op *mat=new matmult_float_op(10,3,a1);
  matmult_float_op *mat2=new matmult_float_op(10,10,a2);

  cgstep *st=new cgstep();
  prec_solver *solv=new prec_solver(st,dat,mat,mat2,.001);
  solv->set_verbose(2);
  solv->solve(750);
  float_1d *mod=(float_1d*) solv->return_model();
  
  float_1d *dat2=(float_1d*)dat->clone_zero();
  mat->forward(false,mod,dat);
  dat->info("data is ",3);
  
  
  //solv->add_equation(dat,mat);
  //solv->add_equation(dat2,mat2);
  //float_1d *mod=(float_1d*) solv->solve(13);
  
//  std::cout  << "model should be 1 6 5 "<< mod->vals[0]<<" "<<mod->vals[1]<< " "<<mod->vals[2]<<std::endl;
  
  
  delete st;
  //delete solv;
  delete mat; delete mat2;
  delete [] a; delete [] a2; delete [] a1;
  delete dat; delete dat2;
}
void mat_mult_prec(){
  float *a=new float[9];
  a[0]=1; a[1]=2; a[2]=3;
  a[3]=3; a[4]=1; a[5]=2;
  a[6]=2; a[7]=2; a[8]=2;
  
  float_1d *dat=new float_1d(3);
  dat->vals[0]=29; dat->vals[1]=18; dat->vals[2]=25;

  matmult_float_op *mat=new matmult_float_op(3,3,a);
  cgstep *st=new cgstep();
 // lin_solver_basic *solv=new lin_solver_basic(st);

 // solv->add_equation(dat,mat);
 // float_1d *mod=(float_1d*) solv->solve(3);
  
  //std::cout  << "model should be 1 6 5 "<< mod->vals[0]<<" "<<mod->vals[1]<< " "<<mod->vals[2]<<std::endl;
  
  
  delete st;
//  delete solv;
  delete mat;
  delete [] a;
  delete dat;
}
void mat_mult_test(){
  float *a=new float[9];
  a[0]=1; a[3]=2; a[6]=3;
  a[1]=3; a[4]=1; a[7]=2;
  a[2]=2; a[5]=2; a[8]=2;
  
  float_1d *dat=new float_1d(3);
  dat->vals[0]=29; 
  dat->vals[1]=18; 
  dat->vals[2]=25;

  matmult_float_op *mat=new matmult_float_op(3,3,a);
  cgstep *st2=new cgstep();
  hybrid_norm nrm;
  generic_norm *st=new generic_norm(10,&nrm,10);
  simple_solver *solv=new simple_solver(st,dat,mat);

//  solv->add_equation(dat,mat);
  solv->solve(3);
  float_1d *mod=(float_1d*) solv->return_model();
  
  std::cout  << "model should be 1 6 5 "<< mod->vals[0]<<" "<<mod->vals[1]<< " "<<mod->vals[2]<<std::endl;
assert(1==2);  
  
  delete st;
  delete solv;
  delete mat;
  delete [] a;
  delete dat;
}
void rms_interval(){

  sregf *rfile=new sregf("rms");
  sregf *wfile=new sregf("wt");
  
  
  hypercube_float *rms=new hypercube_float(rfile);
  rfile->read_all("rms",rms);
  
  hypercube_float *wt=new hypercube_float(wfile);
  wfile->read_all("wt",wt);
  deriv_it *der=new deriv_it(rms);
  deriv_filt *dfilt=new deriv_filt((int)rms->get_n123());
  deriv2_filt *dfilt2=new deriv2_filt((int)rms->get_n123());
  for(long long i=0; i < wt->get_n123(); i++) wt->vals[i]=wt->vals[i]/(1.+(float)i);
   // for(long long i=0; i < wt->get_n123(); i++) wt->vals[i]=1./(1.+(float)i);

  diagonal_wt_float *wop=new diagonal_wt_float(rms,wt);
  /*
  if(!wop->dot_test(true)){
    fprintf(stderr,"WEIGHT FAILED\n");
    assert(1==2);
  }
  */
  helicon_float *deriv=new helicon_float(rms,dfilt2);
  /*
    if(!deriv->dot_test(true)){
    fprintf(stderr,"DERIV FAILED\n");
    assert(1==2);
  }
  */
  polydiv_float *causint= new polydiv_float(rms,dfilt);
  /*
    if(!causint->dot_test(true)){
    fprintf(stderr,"POLYDIV FAILED\n");
    assert(1==2);
  }
  */
  polydiv_float *causint2= new polydiv_float(rms,dfilt);
  /*
    if(!causint->dot_test(true)){
    fprintf(stderr,"POLYDIV FAILED\n");
    assert(1==2);
  }
  */
    i_op *iop = new i_op(rms,rms);
    /*
    if(!causint->dot_test(true)){
    fprintf(stderr,"POLYDIV FAILED\n");
    assert(1==2);
  }
  */
  
 for(int i=0; i< (int) rms->get_n123(); i++) rms->vals[i]=rms->vals[i]*rms->vals[i]*(i+1);
 hypercube_float *mod0=(hypercube_float*)rms->clone_vec();
 for(int i=1; i<(int) rms->get_n123();i++) mod0->vals[i]=0.;

 std::vector<bool> mask; mask.push_back(false);
 for(int i=1; i < (int)mod0->get_n123(); i++) mask.push_back(true);
 mask_float_op *msk=new mask_float_op(mod0,&mask);
 
 
 float eps=.1;

  hypercube_float *reg0=(hypercube_float*) deriv->range->clone_zero();
  reg0->set_percent(80.);

  
 
 
  cgstep *st=new cgstep();
  hybrid_norm nrm;
  //l2_norm nrm;
  generic_norm *st2=new generic_norm(10,&nrm,10);

  wop->range->set_percent(100.);
  reg_solver *solv=new reg_solver(st2,rms,causint,der,eps);
  //simple_solver *solv=new simple_solver(st,rms,causint);

  solv->set_wt_op(wop);
  solv->set_reg0(reg0);
  solv->set_mask_op(msk);
  solv->set_m0(mod0);
  solv->setup_solver();
  solv->verb=true;
  
    solv->solve(700);
  float_1d *mod=(float_1d*) solv->return_model();
  
//reg_solver::reg_solver(step *s, my_vector *data, my_operator *op, my_operator *reg, float eps,
 // my_operator *wt, my_vector *m0)
 // lin_solver_basic *solv=new lin_solver_basic(st,causint2);
 // solv->add_p0(mod0);
 // solv->add_mask(msk);
 // solv->add_equation(rms,causint);
//  solv->add_equation(rms,causint,wop);
  //solv->add_equation(deriv,eps);
  //solv->add_equation(iop,eps);
 // hypercube_float *mod=(hypercube_float*) solv->solve(20);
hypercube_float *rms2=(hypercube_float*)rms->clone_vec();
causint->forward(false,mod,rms2);
for(int i=0; i < (int) mod->get_n123();i++) {
  //fprintf(stderr,"MATCH %d %f %f %f %f \n",i,sqrtf(mod->vals[i]),rms->vals[i],rms2->vals[i],
   //   rms->vals[i]/((float)(i+1))-rms2->vals[i]/((float)(i+1)));
 // rms2->vals[i]=sqrt(rms2->vals[i]/(float)(i+1));
}
 //   rfile->write_all("interval",rms2);
 
 //for(int i=0; i < (int) mod->get_n123(); i++)
 // if(mod->vals[i] >0.) mod->vals[i]=sqrtf(mod->vals[i]);
 //else mod->vals[i]=0.;
  
  rfile->write_description("interval");
  rfile->write_all("interval",mod);

  delete wt;
  delete dfilt2;
  delete iop;
  delete mod0;
  delete st; 
  delete rms; 
 // delete mod; 
  delete msk;
  delete rfile; delete wfile; delete dfilt;
  delete causint; delete deriv;
}
