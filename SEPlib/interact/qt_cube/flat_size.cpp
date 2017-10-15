#include<flat_size.h>    
#include<sepmath.h>
#include<seplib.h>
flat_size::flat_size(hypercube_float *dps, hypercube_float *tau, int j, 
 std::vector<pt3> pbig, int im,bool four){

  fourier=four;
  std::vector<axis> ax=dps->return_axes(4);  
  view=false;
 // if(j==4) view=true;
 // view=true;
// std::vector<axis> aout;
 
  for(int i=0; i < 3; i++){
    jsamp[i]=jfact(ax[i].n,j);
    aout.push_back(axis(ax[i].n/jsamp[i]));
  }
  
  aout.push_back(axis(2));
  aout.push_back(axis(1));
  n1=aout[0].n; n2=aout[1].n; n3=aout[2].n;

  tau_big=tau;
  dips_big=dps;
      aout[3].n=3;

    data=new hypercube_float(aout,false);
  if(j>1){
    std::vector<axis> ab;
    ab.push_back(aout[0]); ab.push_back(aout[1]); ab.push_back(aout[2]);
    tau_small=new hypercube_float(ab);
    dips_small=new hypercube_float(aout);
    
    resample_down();
   
    sub=true;
  }
  else{
    sub=false;
    tau_small=tau_big;
    dips_small=dips_big;
  }
    fix.clear();
   for(int i=0; i < pbig.size(); i++){
     fix.push_back(pt3(
     pbig[i].i1/jsamp[0],
     pbig[i].i2/jsamp[1],
     pbig[i].i3/jsamp[2]));
   }

  imaster=im;

}


void flat_size::resample_down(){
 
  std::vector<int> n=tau_big->return_ns();
  std::vector<int> ns=tau_small->return_ns();
  
  float *tbig=tau_big->vals, *tsmall=tau_small->vals;
  float *dbig=dips_big->vals,*dsmall=dips_small->vals;
  
  int i,ibig;
  int *ax1=new int[2*jsamp[0]-1];
  int *ax2=new int[2*jsamp[1]-1];
  int *ax3=new int[2*jsamp[2]-1];
  int nsz=(2*jsamp[0]-1)*(2*jsamp[1]-1)*(2*jsamp[2]-1);
  float *fmap=new float[nsz];
  float scale=1.;
  float tot=0.;
  //for(int i=0; i < 3; i++) scale=scale*0.5*(jsamp[i]*(jsamp[i]-1)+jsamp[i]*jsamp[i]+1);
  for(int i3=-jsamp[2]+1,i=0; i3< jsamp[2]; i3++){
    for(int i2=-jsamp[1]+1; i2< jsamp[1]; i2++){
      for(int i1=-jsamp[0]+1; i1 < jsamp[0]; i1++,i++){
        fmap[i]=(1.0*(jsamp[0]-abs(i1))*(jsamp[1]-abs(i2))*(jsamp[2]-abs(i3)));
        tot+=fmap[i];
        
      }
    }
  }
  scale=tot;tot=0;
  for(int i=0; i< (jsamp[0]*2-1)*(jsamp[1]*2-1)*(jsamp[2]*2-1); i++){
    fmap[i]=fmap[i]/scale;
    tot+=fmap[i];
  }

   int jj=0,l,k;
   float t;
   float rat01=((float)jsamp[1])/((float)jsamp[0]);
   float rat02=((float)jsamp[2])/((float)jsamp[0]);
   for(int i3=0,i=0; i3 < ns[2]; i3++){
     for(int j3=i3*jsamp[2]-jsamp[2]+1,k=0; j3 < i3*jsamp[2]+jsamp[2]; j3++,k++) 
        ax3[k]=MIN(MAX(j3,0),n[2]-1)*n[0]*n[1];
     
     for(int i2=0; i2 < ns[1]; i2++){
       for(int j2=i2*jsamp[1]-jsamp[1]+1,k=0; j2 < i2*jsamp[1]+jsamp[1]; j2++,k++) {
         ax2[k]=MIN(MAX(j2,0),n[1]-1)*n[0];
         
         }
       for(int i1=0; i1 < ns[0]; i1++,i++){
         for(int j1=i1*jsamp[0]-jsamp[0]+1,k=0; j1 < i1*jsamp[0]+jsamp[0]; j1++,k++) 
           ax1[k]=MIN(MAX(j1,0),n[0]-1);
         dsmall[i]=dsmall[i+ns[0]*ns[1]*ns[2]]=tsmall[i]=0.;
         for(int j3=0,jj=0; j3 < 2*jsamp[2]-1; j3++){
           for(int j2=0; j2< 2*jsamp[1]-1; j2++){
             for(int j1=0; j1< 2*jsamp[0]-1; j1++,jj++){
               l=ax3[j3]+ax2[j2]+ax1[j1];
              // fprintf(stderr,"IN LOOP %d %d %d (%d) %d %d %d (%d) %d %d \n",
               // i1,i2,i3,i,j1,j2,j3,l,jj,ax2[j2]);
               tsmall[i]+=tbig[l]*fmap[jj];
               dsmall[i]+=dbig[l]*fmap[jj];
               dsmall[i+ns[0]*ns[1]*ns[2]]+=
               dbig[l+n[0]*n[1]*n[2]]*fmap[jj];
             }
           }
         }
         tsmall[i]=tsmall[i]/((float)jsamp[0]);
           dsmall[i]=dbig[i1*jsamp[0]+i2*jsamp[1]*n[0]+i3*jsamp[2]*n[0]*n[1]]*rat01;
           dsmall[i+ns[0]*ns[1]*ns[2]]=
             dbig[i1*jsamp[0]+i2*jsamp[1]*n[0]+i3*jsamp[2]*n[0]*n[1]+n[0]*n[1]*n[2]]*rat02;
           // dsmall[i]=dsmall[i]*rat01;
            //dsmall[i+ns[0]*ns[1]*ns[2]]*=rat02;
     //  fprintf(stderr,"CH ECK D %d %f %f \n",i,dsmall[i],dsmall[i+ns[0]*ns[1]*ns[2]]);
       }
     }
  }
  delete[]ax2; delete []ax3;  
  delete []fmap;delete [] ax1;

  
  

}

void flat_size::resample_up(){
//For now do linear probably should think about sinc

  std::vector<int> n=tau_big->return_ns();
  std::vector<int> ns=tau_small->return_ns();
  
  float *tbig=tau_big->vals, *tsmall=tau_small->vals;
  
  
  int i,b1,e1,b2,e2,b3,e3;
  float f1,f2,f3;
  for(int i3=0,i=0; i3 < n[2]; i3++){
    f3=(i3*1.)/(jsamp[2]*1.);
    b3=(int)f3;
    f3-=b3;
    e3=MIN(ns[2]-1,b3+1);
                 if(b3>=ns[2]-1) { b3=ns[2]-2; e3=b3+1; f3=1.;}
             //   if(i3==28) fprintf(stderr,
              //     " b3=%d e3=%d f3=%f \n",
              //       b3,e3,f3);
    b3=b3*ns[0]*ns[1]; 
    e3=e3*ns[1]*ns[0];
    for(int i2=0; i2 < n[1]; i2++){
      f2=(i2*1.)/(jsamp[1]*1.);
      b2=(int)f2; f2-=b2;
      e2=MIN(ns[1]-1,b2+1);
       if(b2>=ns[1]-1) { b2=ns[1]-2; e2=b2+1; f2=1.;}
                // if(i3==28 && i2==291 ) fprintf(stderr,
                //   " b2=%d e2=%d b3=%d e3=%d  f2=%f f3=%f \n",
                 //     b2,e2,b3,e3,f2,f3);
     b2=b2*ns[0]; e2=e2*ns[0];
      for(int i1=0; i1 < n[0];i1++,i++){
        f1=(i1*1.)/(jsamp[0]*1.);
        b1=(int)f1; 
        f1-=b1;
        e1=MIN(ns[0]-1,b1+1);
         if(b1>=ns[0]-1) { b1=ns[0]-2; e1=b1+1; f1=1.;}
        tbig[i]=((1.-f3)*(1.-f2)*(1.-f1)*tsmall[b1+b2+b3]+
                (1.-f3)*(1.-f2)*(   f1)*tsmall [e1+b2+b3]+
                (1.-f3)*(   f2)*(1.-f1)*tsmall [b1+e2+b3]+
                (1.-f3)*(   f2)*(   f1)*tsmall [e1+e2+b3]+
                (   f3)*(1.-f2)*(1.-f1)*tsmall [b1+b2+e3]+
                (   f3)*(1.-f2)*(   f1)*tsmall [e1+b2+e3]+
                (   f3)*(   f2)*(1.-f1)*tsmall [b1+e2+e3]+
                (   f3)*(   f2)*(   f1)*tsmall [e1+e2+e3])*(1.*jsamp[0]);
               // if(i3==28 && i2==291 && i1==97) fprintf(stderr,
               //    "b1=%d e1=%d b2=%d e2=%d b3=%d e3=%d f1=%f f2=%f f3=%f \n",
                 //     b1,e1,b2,e2,b3,e3,f1,f2,f3);
      }
    }
  }

}
int flat_size::jfact(int n,int fact){
  int nt=n/fact;
  int j;
  if(nt<24) j=n/24;
  else j=fact;
  return j;
}
void flat_size::update_tau(float eps_t,int nliter, int niter){

   flatten_op *fop=new flatten_op(tau_small,data,eps_t,fix,10,frozen);

  int iloc_2=fix[imaster].i2, iloc_3=fix[imaster].i3;

  int iloc=iloc_2*n1+iloc_3*n1*n2;

  flatten_fourier *inv=new flatten_fourier(tau_small,fop,n1,n2,n3,eps_t,iloc_2+iloc_3*n2);

  delete fop;
  fop=new flatten_op(tau_small,data,eps_t,fix,imaster,frozen);
  fop->set_scale(true);
  fop->dot_test(true);
 fftwf_plan planF,planI;
 hypercube_float *delta,*grad;
 if(!fourier){
 if(frozen.size()==0){
 for(int i=0; i< fix.size(); i++){
    tau_small->vals[fix[imaster].i1+fix[i].i2*n1+fix[i].i3*n1*n2]=(fix[i].i1-fix[imaster].i1);
  ;
 }
 }
 }
 else{

 
  delta=tau_small->clone();
 int zero=0;
      fftw_test2_(&n1,&n2,&n3,&zero,delta->vals,(long long*)&planF,(long long*)&planI);
      hypercube_float *grad=tau_small->clone();
 }
 for(int non=0; non<nliter; non++){
   hypercube_float *dbuf=create_data(dips_small,tau_small,iloc_2,iloc_3);
   if(!fourier) grad=tau_small->clone();

  if(view) srite("nn.H",tau_small->vals,tau_small->get_n123()*4);
   dbuf->scale(-1.);
   fop->set_scale(false); 
   fop->forward(true,tau_small,dbuf);   
   if(!fourier)  fop->set_scale(true);
   fop->scale_it(dbuf->vals);
   if(!fourier){
   fop->adjoint(false,grad,dbuf);
   
  
  if(view) srite("mm.H",dbuf->vals,dbuf->get_n123()*4);
  if(view) srite("oo.H",grad->vals,grad->get_n123()*4);

   delete dbuf;
   
   delta_mod *a=new delta_mod(aout,inv->return_mult(),imaster,fix,non);
   delta=grad->clone();
   
   
   classic_cg(fop, grad,delta,a,niter);

   delete  a;
   if(frozen.size()>0){
     for(int i1=0; i1 < frozen.size(); i1++){
        for(int i2=0; i2 < n2*n3; i2++){
           delta->vals[frozen[i1]+i2*n1]=0.;
   
         }
       }
     fprintf(stderr,"CHECK %d \n",frozen[0]);
   }
  
   tau_small->add(delta);   
   fprintf(stderr,"I SEE IT %f %f %d\n",tau_small->vals[13],delta->vals[13],frozen.size());
  delete delta;
   delete grad;
  }
  else{
   //Fourier
 fop->adjoint(false,delta,dbuf);
    delete dbuf;
    float *mult=inv->return_mult();
  fftwf_execute(planF);
  for(int i=0;  i < delta->get_ndim(); i++){
    axis ax=delta->get_axis(i+1);
    fprintf(stderr,"CHECK NDIM SIZE %d \n",ax.n);
  }
    for(int i=0; i < (int)delta->get_n123(); i++) {
      delta->vals[i]*=mult[i];
    }
    fftwf_execute(planI);
    tau_small->scale_add(1.,delta,-1.);
  }
  if(!fourier){
  
    if(frozen.size()==0){
    for(int i=0; i< fix.size(); i++)
      tau_small->vals[fix[imaster].i1+fix[i].i2*n1+fix[i].i3*n1*n2]=
        (fix[i].i1-fix[imaster].i1);
   }
     
}

   if(view) srite("nn.H",tau_small->vals,tau_small->get_n123()*4);
   
 }
  if(fourier){
    delete delta;//Fourier
    fftwf_destroy_plan( planF);
    fftwf_destroy_plan( planI);
  }
 if(sub) resample_up();
  delete fop;
  delete inv;



}
void flat_size::classic_cg( my_operator *op, hypercube_float *g, hypercube_float *mod,
hypercube_float *a,int niter){

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
    fprintf(stderr,"ALPHA=%g BETA=%g iter=%d \n",alpha,beta,iter);
    if(beta < -20.) beta=-20.;
    if(beta >20.) beta=20.;
 //   fprintf(stderr,"ITER=%d %g \n",iter,q->dot(q));
  }
  delete p; delete u;  delete q;

}
hypercube_float *flat_size::create_data( hypercube_float *dips, hypercube_float *tau, 
  int iloc2, int iloc3){
  
 
   std::vector<axis> axes=dips->return_axes(4);
   int n1=axes[0].n, n2=axes[1].n, n3=axes[2].n;
   axes[3].n=3;
  hypercube_float *d=new hypercube_float(axes);
  
  
   int iloc=n1*iloc2+n1*n2*iloc3,ib,it;
   long long n123=n1*n2*n3,i=0;
   float f;
   for(int i3=0; i3 < n3; i3++){
    for(int i2=0; i2 < n2; i2++){
       ib=n1*i2+i3*n1*n2;
       for(int i1=0; i1 < n1; i1++,i++){
          f=tau->vals[i]-tau->vals[i1+iloc]+i1;
          it=(int)f; f-=it;
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
  return d;
  
}
