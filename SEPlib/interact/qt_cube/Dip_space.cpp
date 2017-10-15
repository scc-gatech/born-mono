°¢#include<flatten_op.h>
#include<sregf.h>
#include<sep_params.h>
#include<basic_solver.h>
#include<flatten_fourier.h>
hypercube_float *create_data( hypercube_float *dips, hypercube_float *tau, 
 int iloc2, int iloc3);
void classic_cg(my_operator *op, hypercube_float *g,  hypercube_float *a,hypercube_float *mod, int niter);
class delta_mod: public hypercube_float{
  public:
    delta_mod(){}
    delta_mod(axis *ax, float *m,int imaster,std::vector<pt3> p, int it){ 
      init_nd(3,ax);
       im=imaster;
       iter=it;
             fprintf(stderr,"ICHECK %d \n",im);

      vals=new float[get_n123()];
      int zero=0;
        n1=ax[0].n; n2=ax[1].n; n3=ax[2].n;
       iloc=p[im].i2*n1+p[im].i3*n1*n2;
       fprintf(stderr,"ICHECK %d \n",im);
      fftw_test2_(&n1,&n2,&n3,&zero,this->vals,(long long*)&planF,(long long*)&planI);
      mult=m;
      pp=p;
      fprintf(stderr,"CHECK ILOC %d \n",iloc);
    
    }
  void inverse_hessian(my_vector *vec){
    if(iter>99){
      scale_add(0.,vec,1.);
    return;
    }
    hypercube_float *v=(hypercube_float*) vec;
    memcpy(vals,v->vals,get_n123()*sizeof(float));
    fftwf_execute(planF);
    for(int i=0; i < get_n123(); i++) vals[i]*=
      mult[i];
    fftwf_execute(planI);
    float *base=new float [n1];
    memcpy(base,&vals[iloc],n1*4);
    for(int i2=0; i2 <n2*n3; i2++) 
      for(int i1=0; i1 < n1; i1++) vals[i1+i2*n1]-=base[i1];
    int ii=pp[im].i1;
//    for(int i=0; i < pp.size(); i++){
  //    vals[ii+pp[i].i2*n1+pp[i].i3*n1*n2]=0.;
          

   // }
    delete [] base;
  }
   ~delta_mod(){   
        fftwf_destroy_plan( planF);
        fftwf_destroy_plan( planI);
     }
     float *mult;
     int iloc,n1,n2,n3,im,iter;
    fftwf_plan planF,planI;
    std::vector<pt3> pp;

};
main(int argc, char **argv){


  sep_params *pars=new sep_params(argc,argv);
  sregf *in=new sregf("in"); 
  axis *ax=in->return_axes(4);
  hypercube_float *buf=new hypercube_float(4,ax);
  in->read_next("in",buf);
  int one=1;putch("n4","d",&one);
  int *n=in->return_ns();
  

  

  
  ax[3].n=3;
  hypercube_float *data=new hypercube_float(4,ax,false);
  

  hypercube_float *tau=new hypercube_float(3,ax);
  tau->set_val(0.);
  hypercube_float *delta=tau->clone();

 int n1=ax[0].n,n2=ax[1].n,n3=ax[2].n;
 long long n123=n1*n2*n3;
 std::vector<pt3> pts;

 
 int jsmall[3];
 hypercube_float *prec;
 int niter=pars->get_int("niter",20);
 int nliter=pars->get_int("nliter",50);
 float eps_t=pars->get_float("eps_t",.03);
 prec=delta->clone();
   flatten_op *fop=new flatten_op(tau,data,eps_t,pts,10);
  // if(!fop->dot_test(true)) seperr("Dot product test failed \n");
   
 
   
     pts.push_back(pt3(72,4,28)); pts.push_back(pt3(91,21,28)); pts.push_back(pt3(112,49,28));
  pts.push_back(pt3(118,87,28)) ; pts.push_back(pt3(110,112,28)); pts.push_back(pt3(104,143,28));
  pts.push_back(pt3(108,180,28)); pts.push_back(pt3(104,180,27)); pts.push_back(pt3(108,180,54));
  pts.push_back(pt3(109,180,54)); pts.push_back(pt3(111,86,54)); pts.push_back(pt3(104,52,54));
  pts.push_back(pt3(77,14,54))  ; pts.push_back(pt3(106,135,3)); pts.push_back(pt3(125,168,3));
  pts.push_back(pt3(100,19,3))  ;

    int imaster=3;
  int iloc_2=pts[imaster].i2, iloc_3=pts[imaster].i3;
    int iloc=iloc_2*n1+iloc_3*n1*n2;

   flatten_fourier *inv=new flatten_fourier(prec,fop,n1,n2,n3,eps_t,iloc_2+iloc_3*n2);
 
  delete fop;
  fop=new flatten_op(tau,data,eps_t,pts,imaster);
     fop->set_scale(true);

 // Ï if(!fop->dot_test(true)) seperr("Dot product test failed \n");
   tau->set_val(0.);
    //  if(!fs_op->dot_test(true)) seperr("Dot product test failed \n");

 for(int i=0; i< pts.size(); i++){
   tau->vals[pts[imaster].i1+pts[i].i2*n1+pts[i].i3*n1*n2]=
  ( pts[i].i1-pts[imaster].i1);

 }
 tau->info("TAU");
 niter=20;
 for(int non=0; non<nliter; non++){
  
   hypercube_float *dbuf=create_data(buf,tau,iloc_2,iloc_3);
 
   hypercube_float *grad=tau->clone();
   
   
   dbuf->scale(-1.);
    fop->set_scale(false); 
 
   fop->forward(true,tau,dbuf);
    
   fop->set_scale(true);
   fop->scale_it(dbuf->vals);
   fop->adjoint(false,grad,dbuf);
   srite("xx.H",dbuf->vals,n123*12);
   delete dbuf;
   
   delta_mod *a=new delta_mod(ax,inv->return_mult(),imaster,pts,non);
   hypercube_float *delta=grad->clone();
   
   
   classic_cg(fop, grad,delta,a,niter);
   //if(non==2) niter*=10;
   delete  a;
  
   tau->add(delta);
   
   delete delta;
    for(int i=0; i< pts.size(); i++){
 
  
   tau->vals[pts[imaster].i1+pts[i].i2*n1+pts[i].i3*n1*n2]=
  ( pts[i].i1-pts[imaster].i1);

 }
   srite("bb.H",tau->vals,2*(int)in->get_n123());


 }
  delete fop;
  delete inv;
  delete data;



  srite("out",tau->vals,4*(int)in->get_n123());
    
}


void classic_cg( my_operator *op, hypercube_float *g, hypercube_float *mod, hypercube_float *a,int niter){

  mod->zero();
  hypercube_float *p=(hypercube_float*)mod->clone_vec();
  hypercube_float *u=(hypercube_float*)mod->clone_vec();

  hypercube_float *q=(hypercube_float*) mod->clone_vec();
  p->zero();
  u->zero();
  

  double beta=0.;
  double alpha;

  a->inverse_hessian(g);
  
  op->hessian(a,q);
  double pu;
  
  for(int iter=0; iter < niter; iter++){
    p->scale_add(0.,a,-1.);
    p->scale_add(1.,p,beta);

    u->scale_add(beta,q,-1.);
    pu=p->dot(u);
    alpha=-p->dot(g)/pu;
    mod->scale_add(1.,p,alpha);
    g->scale_add(1.,u,alpha);
    a->inverse_hessian(g);
    op->hessian(a,q);
    beta=p->dot(q)/pu;
    if(beta < -20.) beta=-20.;
    if(beta >20.) beta=20.;
    fprintf(stderr,"ITER=%d %g \n",iter,q->dot(q));
  }
  delete p; delete u;  delete q;

}
hypercube_float *create_data( hypercube_float *dips, hypercube_float *tau, 
  int iloc2, int iloc3){
  
 
   axis *axes=dips->return_axes(4);
   int n1=axes[0].n, n2=axes[1].n, n3=axes[2].n;
   axes[3].n=3;
  hypercube_float *d=new hypercube_float(4,axes);
  
  
   int iloc=n1*iloc2+n1*n2*iloc3,ib,it;
   long long n123=n1*n2*n3,i=0;
   float f;
   for(int i3=0; i3 < n3; i3++){
    for(int i2=0; i2 < n2; i2++){
       ib=n1*i2+i3*n1*n2;
       for(int i1=0; i1 < n1; i1++,i++){
          f=tau->vals[i]-tau->vals[i1+iloc]+i1;
          it=f; f-=it;
          d->vals[i]=0.;
     //   fprintf(stderr,"CHECK i1=%d it=%d %f %f \n",i1,it,tau->vals[i],dips->vals[it+ib]);
          if(it >0 && it < n1-1){
          d->vals[i+n123]=(1.-f)*dips->vals[it+ib]+f*dips->vals[it+ib+1];
          d->vals[i+n123*2]=(1.-f)*dips->vals[it+ib+n123]+f*dips->vals[it+n123+ib+1];
         }
         else {
           d->vals[i+n123]=d->vals[i+n123*2]=0.;}
       }
    }
  }
  //srite("d.H",&d->vals[n123],n3*n1*n2*4);
  delete [] axes;
  return d;
  
}

