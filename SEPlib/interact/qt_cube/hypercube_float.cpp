#include<hypercube_float.h>
#include<math.h>
#include <cstdlib>
#include "percentile.h"
#include "seplib.h"
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
extern "C"{
 extern void vec_scale_add(int *,float *,float *,float *,float*);
 };
hypercube_float::hypercube_float(std::vector<axis> axes,bool alloc){
  this->init_nd(axes);
  if(alloc) {
    this->vals=new float[this->get_n123()];
    }
  else this->vals=0;
   name="hypercube_float";

}
hypercube_float::hypercube_float(hypercube *hyper){
  int n=hyper->get_ndim();
  std::vector<axis> axes=hyper->return_axes(n);
  this->init_nd(axes);
  this->vals=new float [this->get_n123()];
   name="hypercube_float";
}
hypercube_float::hypercube_float(std::vector<axis> axes, float *vals){

  this->init_nd(axes);
  this->vals=new float[this->get_n123()];
  this->set(vals);
   name="hypercube_float";
}


hypercube_float *hypercube_float::clone(bool alloc){
 int ndims=this->get_ndim();
std::vector<axis> axes;
 for(int i=0; i< ndims; i++) axes.push_back(get_axis(i+1));

 hypercube_float *tmp=new hypercube_float(axes,alloc);
 
 if(alloc && this->vals!=0) {
   //int n=(int)get_n123(),one=1;
   //scopy(&n,this->vals,&one,tmp->vals,&one);
   for(int i=0; i< get_n123(); i++) tmp->vals[i]=this->vals[i];
 }
 else if(alloc){
    for(int i=0; i< get_n123(); i++) tmp->vals[i]=0;
 }

 tmp->perc=perc;
 tmp->set_norm(_norm);

 //tmp->_norm=_norm;
 return tmp;
}
void hypercube_float::add(hypercube_float *vec){
  //#pragma omp parallel for
  #pragma parallel
  for(int i=0; i < get_n123(); i++) vals[i]+=vec->vals[i];
}
void hypercube_float::set(float *array){

       memcpy((void*)this->vals,(const void*) array, 
         sizeof(float)*this->get_n123());

}
double hypercube_float::sum(){
  double sm=0;
   //#pragma omp parallel for
 

  for(int i=0; i < this->get_n123(); i++) sm+=(double)this->vals[i];
//   fprintf(stderr,"SUM IS %g\n",sm);
  return sm;
}
void hypercube_float::set_val(double val){
  //#pragma omp parallel for

  #pragma parallel
  for(int i=0; i < this->get_n123(); i++) this->vals[i]=val;
}
void hypercube_float::normalize(float val){
 float mymax=vals[0];
  //#pragma omp parallel for

 for(int i=1; i< get_n123(); i++){
 
   if(mymax < vals[i]) mymax=vals[i];
 }
 float sc;
 sc=val/mymax;
  //#pragma omp parallel for

 #pragma parallel
 for(int i=0; i< get_n123(); i++){
   vals[i]=vals[i]*sc;
   
  } 

}
void hypercube_float::info(char *str,int level){
 fprintf(stderr,"**********************\n%s\n***********************\n TYPE=%s \n",str,name.c_str());
 hyper_info(level);
}
void hypercube_float::hyper_info(int level){
 double sm=0.,mymin=0.,mymax=0.;
 if(vals!=0) mymax=mymin=vals[0];
 int imin=0,imax=0;

 if(vals!=0){
   mymin=vals[0];imin=0;
   mymax=vals[0];imax=0;
   for(int i=0; i < get_n123(); i++){
     if(mymin > vals[i]) {mymin=vals[i]; imin=i;}
     if(mymax < vals[i]) {mymax=vals[i]; imax=i;}
     sm+=vals[i]*vals[i];
    }
}
  for(int i=0; i < get_ndim(); i++) fprintf(stderr,"    n%d=%d",i+1,get_axis(i+1).n);
  fprintf(stderr,"\n       N=%d min(%d)=%g max(%d)=%g RMS=%g   \n",
      (int)get_n123(), imin,
mymin,
imax,
mymax,
sqrt(sm)/(1.0*get_n123()));

  long long print;
  if(level!=0 && vals!=0){
  if(level<1)  print=get_n123();
  else print=MIN(level,(int)get_n123());
   for(long long i=0; i < print; i++){
     fprintf(stderr,"val %d %f \n",(int)i,vals[i]);
  } 
  }

 

}
void hypercube_float::power(float val){

  //#pragma omp parallel for
  #pragma parallel
for(long long i=0; i < get_n123(); i++) vals[i]=powf(vals[i],val);

}
void hypercube_float::invert(){
  //#pragma omp parallel for
#pragma parallel
for(long long i=0; i < get_n123(); i++) vals[i]=1./vals[i];

}
my_vector *hypercube_float::clone_vec(){
  //my_vector *m=(my_vector*) this->clone();
  hypercube_float *m=this->clone();
  return (my_vector*)m;
}
void hypercube_float::norm_scale(my_vector *norm_out){
  _norm->norm_scale(this,norm_out);
}
norm_vector *hypercube_float::fill_percentile(){

  
  hypercube_float *m=this->clone();
  percentile p;
  long long samp=std::min((long long)m->get_n123()-1, (long long)(perc*m->get_n123()));
 float val=p.find_abs(m->vals, m->get_n123(),samp);
  //val=1e6;
  if(fabs(val)<1e-6) {
    float def=1;
    fprintf(stderr,"WARNING: val[%d(%f)]=0. reset to %f\n",(int)samp,perc,def);
    val=def;
  }
  m->set_val(val);
  return (norm_vector*) m;
}
my_vector *hypercube_float::clone_space(){
  //my_vector *m=(my_vector*) this->clone();
  hypercube_float *m=this->clone(false);

  return (my_vector*)m;
}
void hypercube_float::random(){
    //#pragma omp parallel for

  #pragma parallel
  for(int i=0; i < get_n123(); i++){
    vals[i]=(float)rand()/(float)RAND_MAX-.5;
  }
}
double hypercube_float::dot(my_vector *other){
  double ret=0;
  check_same(other);
  hypercube_float *o=(hypercube_float*) other;
   #pragma omp parallel for reduction(+: ret) 
  for(long long i=0; i < get_n123(); i++){
    ret+=(double)this->vals[i]*(double)o->vals[i];
  }
//  fprintf(stderr,"IN DOT %g %d \n",ret,get_n123());
  return ret;
}
void  hypercube_float::mult(my_vector *other){
 
  check_same(other);
  hypercube_float *o=(hypercube_float*) other;
   #pragma parallel
  for(long long i=0; i < get_n123(); i++){
   vals[i]=vals[i]*o->vals[i];
  }
 
}
void  hypercube_float::scale_add(double sc1, my_vector *v1, double sc2, my_vector *v2){
 
  check_same(v1);
    check_same(v2);
  hypercube_float *v_1=(hypercube_float*) v1;
    hypercube_float *v_2=(hypercube_float*) v2;
  //  #pragma omp parallel for
  // float s1=(float)sc1,s2=(float)sc2;
  int n=(int)get_n123();
  if(1==3){
   if(n==(int)get_n123()){
    #pragma omp parallel for
    #pragma ivdep
    #pragma vector always
    for(int i=0; i < get_n123(); i++){
      vals[i]=v_1->vals[i]*sc1+v_2->vals[i]*sc2;
    }
  }
  else{
    #pragma omp parallel for
    for(long long i=0; i < get_n123(); i++){
     vals[i]=v_1->vals[i]*sc1+v_2->vals[i]*sc2;
    }
  } 
  
  }
  
  //vec_scale_add(&n,v_1->vals,&s1,v_2->vals,&s2);
 
  for(long long i=0; i < get_n123(); i++){
   vals[i]=v_1->vals[i]*sc1+v_2->vals[i]*sc2;
  }
  
 
}
void hypercube_float::take_min(my_vector*other,my_vector *change){
 check_same(other);
       
 hypercube_float *c = (hypercube_float *) NULL;
  hypercube_float *o=(hypercube_float*) other;
   if(change!=0){
       check_same(change);
       c=(hypercube_float*) change;
     }
    //#pragma omp parallel for

  for(long long i=0; i < get_n123(); i++){
    if(o->vals[i] < vals[i]){
      vals[i]=o->vals[i];
       if(change!=0) c->vals[i]=1;
    }
  }
 
 }
double hypercube_float::my_min(){
  double my_min=vals[0];
    //#pragma omp parallel for

  for(long long i=1; i < get_n123(); i++){
    if(vals[i]< my_min) my_min=vals[i];
  }
  return my_min;
}
 double hypercube_float::my_max(){
  double my_max=vals[0];
  //#pragma omp parallel for
  for(long long i=1; i < get_n123(); i++){
    if(vals[i]>my_max) my_max=vals[i];
  }
  return my_max;
}
 void hypercube_float::take_max(my_vector *other,my_vector *change){
     check_same(other);
hypercube_float *c = (hypercube_float *) NULL;
     if(change!=0){
       check_same(change);
       c=(hypercube_float*) change;
     }
  hypercube_float *o=(hypercube_float*) other;
  for(long long i=0; i < get_n123(); i++){
    if(o->vals[i] > vals[i]) {
      vals[i]=o->vals[i];
      if(change!=0) c->vals[i]=1;
    }
  }
    
    }
 void hypercube_float::scale_add(const double scale_me, my_vector *other, const double scale_other){
  check_same(other);
  hypercube_float *o=(hypercube_float*) other;
  int n=(int)get_n123();
  float sc1=(float)scale_me,sc2=(float)scale_other;
   if(1==1){
   
   if(n==(int)get_n123()){
    #pragma parallel
    for(int i=0; i < get_n123(); i++){
      vals[i]=vals[i]*sc1+o->vals[i]*sc2;
    }
  }
  else{
    #pragma parallel
    for(long long i=0; i < get_n123(); i++){
     vals[i]=vals[i]*sc1+o->vals[i]*sc2;
    }
  } 
  
  }
 // vec_scale_add(&n,vals,&sc1,o->vals,&sc2);
if(1==4){  
 #pragma parallel	
for(int i=0; i< get_n123();i++){ this->vals[i]=this->vals[i]*scale_me+
  scale_other*o->vals[i];
 }
 }
}
void hypercube_float::add(my_vector *other){
  check_same(other);
  hypercube_float *o=(hypercube_float*) other;
  //#pragma omp parallel for

  #pragma parallel
  for(int i=0; i < get_n123(); i++) this->vals[i]+=o->vals[i];
}
bool hypercube_float::check_hypercube_match(const my_vector *v2){

   hypercube_float *h2=(hypercube_float*) v2;

   if(get_ndim_g1()!=h2->get_ndim_g1()){
     fprintf(stderr,"vectors not the same number of dimensions\n");
     return false;
   }

   for(int i=0; i < get_ndim_g1(); i++){
     axis a1=get_axis(i+1),a2=h2->get_axis(i+1);
     if(a1.n!=a2.n){
       fprintf(stderr,"vectors axis=%d not the same number of samples %d,%d \n",
         i+1,a1.n,a2.n);
       return false;
     }

  if(fabs((a1.o-a2.o)/a1.d) > .01){
       fprintf(stderr,"vectors axis=%d not the same origin %f,%f \n",
         i+1,a1.o,a2.o);
       return false;
     }  if(a1.n!=a2.n){
       fprintf(stderr,"vectors axis=%d not the same sampling %f,%f \n",
         i+1,a1.d,a2.d);
       return false;
     }     
   }

   return true;
}
void hypercube_float::axis_resample(axis ain, axis aout,int *iloc, float *floc){
   for(int i=0; i < aout.n; i++){
     floc[i]=(aout.o+aout.d*i-ain.o)/ain.d;
     if(floc[i] < 0.) {
        iloc[i]=0; floc[0]=0.;
     }
     else if(floc[i] >=ain.n-1){
        iloc[i]=ain.n-2; floc[i]=1.;
     }
     else{
       iloc[i]=(int)floc[i]; floc[i]-=iloc[i];
   }
 //  fprintf(stderr,"FLOC %d %d %f \n",i,iloc[i],floc[i]);

 }
}
void  hypercube_float::resample(hypercube_float *in){
  
  assert(in->get_ndim() == get_ndim() && get_ndim()<4);
  int ndim=in->get_ndim();
  int **iloc=new int*[ndim];
  float **floc=new float*[ndim];
  for(int i=0; i < ndim; i++){
    iloc[i]=new int[get_axis(i+1).n];
    floc[i]=new float[get_axis(i+1).n];
    axis_resample(in->get_axis(i+1),get_axis(i+1), iloc[i],floc[i]);
  }
  if(ndim==1){
    for(int i1=0; i1 < get_axis(1).n; i1++){
      vals[i1]=(1.-floc[0][i1])*in->vals[iloc[0][i1]]+floc[0][i1]*in->vals[iloc[0][i1+1]];
    }
  }
  else if(ndim==2){
    int n1=in->get_axis(1).n;
    int i=0;
    for(int i2=0; i2 < get_axis(2).n; i2++){
      for(int i1=0; i1 < get_axis(1).n; i1++,i++){
       int ib=iloc[0][i1]+n1*iloc[1][i2];
        vals[i]=
          in->vals[ib]*     (1.-floc[0][i1])*(1.-floc[1][i2])+
          in->vals[ib+1]*   (floc[0][i1])*(1.-floc[1][i2])+
          in->vals[ib+n1]*  (1.-floc[0][i1])*(floc[1][i2])+
          in->vals[ib+1+n1]*(floc[0][i1])*(floc[1][i2]);
      }
    }
  
  }
  else{
    int n1=in->get_axis(1).n;
    int n2=in->get_axis(2).n;
    int n12=n1*n2;
    int i=0;
    for(int i3=0; i3 < get_axis(3).n; i3++){
      for(int i2=0; i2 < get_axis(2).n; i2++){
        for(int i1=0; i1 < get_axis(1).n; i1++,i++){
          int ib=iloc[0][i1]+iloc[1][i2]*n1+iloc[2][i3]*n12;
 

          vals[i]=
            in->vals[ib]*     (1.-floc[0][i1])*(1.-floc[1][i2])*(1.-floc[2][i3])+
            in->vals[ib+1]*   (floc[0][i1])*(1.-floc[1][i2])*(1.-floc[2][i3])+
            in->vals[ib+n1]*  (1.-floc[0][i1])*(floc[1][i2])*(1.-floc[2][i3])+
            in->vals[ib+1+n1]*(floc[0][i1])*(floc[1][i2])*(1.-floc[2][i3])+
            in->vals[ib+n12]*     (1.-floc[0][i1])*(1.-floc[1][i2])*(floc[2][i3])+
            in->vals[ib+1+n12]*   (floc[0][i1])*(1.-floc[1][i2])*(floc[2][i3])+
            in->vals[ib+n1+n12]*  (1.-floc[0][i1])*(floc[1][i2])*(floc[2][i3])+
            in->vals[ib+1+n1+n12]*(floc[0][i1])*(floc[1][i2])*(floc[2][i3]);
  //  if(ib+1+n1+n12>=in->get_n123())      
    //   fprintf(stderr,"%d [%d] %d [%d] %d [%d] -%d  \n",i1,in->get_axis(1).n,i2,
      //   in->get_axis(2).n,i3,in->get_axis(3).n,ib+1+n1+n12);
        }
      }
    }
  
  }
  for(int i=0; i < ndim; i++){
    delete [] iloc[i];
    delete [] floc[i];
  }
  delete [] iloc; delete [] floc;

}
