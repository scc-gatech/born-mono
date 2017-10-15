#include<flatten_op.h>
#include<sregf.h>
#include<sep_params.h>
#include<solver_basic.h>
#include<flat_size.h>

class run_pars{
  public:
    run_pars(int j, int nl, int nn, float e){
      jsamp=j; niter=nl; nliter=nn; eps=e;
    }
    
    int jsamp, niter, nliter;
    float eps;
    };

main(int argc, char **argv){


  sep_params *pars=new sep_params(argc,argv);
  sregf *in=new sregf("in"); 
  
  std::vector<axis>ax=in->return_axes(4);
  hypercube_float *dips=new hypercube_float(ax);
  in->read_next("in",dips);
  int one=1;putch("n4","d",&one);
 
  
 int niter=pars->get_int("niter",20);
 int nliter=pars->get_int("nliter",50);
 float eps_t=pars->get_float("eps_t",.03);

  

  
  ax.pop_back();
  hypercube_float *tau=new hypercube_float(ax);
  tau->set_val(0.);

   std::vector<pt3> pts;


   
  pts.push_back(pt3(72,4,28)); pts.push_back(pt3(91,21,28)); pts.push_back(pt3(112,49,28));
  pts.push_back(pt3(118,87,28)) ; pts.push_back(pt3(110,112,28)); pts.push_back(pt3(104,143,28));
  pts.push_back(pt3(108,180,28)); pts.push_back(pt3(108,281,28)); pts.push_back(pt3(108,180,54));
  pts.push_back(pt3(109,180,54)); pts.push_back(pt3(111,86,54)); pts.push_back(pt3(104,52,54));
  pts.push_back(pt3(77,14,54))  ; pts.push_back(pt3(106,135,3)); pts.push_back(pt3(125,168,3));
  pts.push_back(pt3(100,19,3))  ;
  int imaster=3;

  std::vector<run_pars> run;
  
  //run.push_back(run_pars(8,20,10,eps_t*5.));
  run.push_back(run_pars(8,200,15,eps_t));
 run.push_back(run_pars(8,200,20,eps_t));
 run.push_back(run_pars(4,60,4,eps_t));
  run.push_back(run_pars(2,80,4,eps_t));
 // run.push_back(run_pars(1,10,10,eps_t));
 
  flat_size *sz;
  for(int i=0; i < run.size(); i++){

    fprintf(stderr,"Running tau estimate j=%d eps=%f nliter=%d niter=%d \n",
      run[i].jsamp,run[i].eps,run[i].nliter,run[i].niter);
  if(i==0){
  sz=new flat_size(dips,tau,run[i].jsamp,pts,imaster,true);
  }
  else 
   sz=new flat_size(dips,tau,run[i].jsamp,pts,imaster,false);
    sz->update_tau(run[i].eps,run[i].nliter,run[i].niter);
    
    
    srite("bb.H",tau->vals,(int)tau->get_n123()*4);
    delete sz;
  }
  srite("out",tau->vals,(int)tau->get_n123()*4);
  delete dips; delete tau;
}