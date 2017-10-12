#include<hypercube_float.h>
#include<math.h>
#include <cstdlib>
#include "percentile.h"
#include <tbb/tbb.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_for.h>
using namespace SEP;
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
extern "C"{
 extern void vec_scale_add(int *,float *,float *,float *,float*);
 };
hypercube_float::hypercube_float( const std::vector<axis> axes, const bool alloc){
  this->initNd(axes);
  if(alloc) {
    this->vals.resize(getN123());
    }
  else this->vals.resize(0);
   name="hypercube_float";

}
hypercube_float::hypercube_float(const hypercube *hyper){
  int n=hyper->getNdim();
  std::vector<axis> axes=hyper->returnAxes(n);
  this->initNd(axes);
  this->vals.resize(getN123());
   name="hypercube_float";
}
hypercube_float::hypercube_float(const std::shared_ptr<hypercube> hyper){
  int n=hyper->getNdim();
  std::vector<axis> axes=hyper->returnAxes(n);
  this->initNd(axes);
  this->vals.resize(getN123());
   name="hypercube_float";
}
hypercube_float::hypercube_float( const std::vector<axis> axes, const  float *vals){

  this->initNd(axes);
  this->vals.resize(getN123());
  this->set(vals);
   name="hypercube_float";
}
hypercube_float *hypercube_float::clone( const bool alloc) const{
 int ndims=this->getNdim();
 std::vector<axis> axes;
 for(int i=0; i< ndims; i++) axes.push_back(getAxis(i+1));

 hypercube_float *tmp=new hypercube_float(axes,alloc);
 
 if(alloc && this->vals.size()!=0) {
     tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) tmp->vals[i]=vals[i];
  });
 }
 else if(alloc){
      tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) tmp->vals[i]=0;
  });
 }

 tmp->perc=perc;
 tmp->set_norm(_norm);

 //tmp->_norm=_norm;
 return tmp;
}
void hypercube_float::add( const hypercube_float *vec){
 tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i)
      vals[i]+=vec->vals[i];
  });
  }
void hypercube_float::set( const float *array){

 tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i)
      vals[i]+=array[i];
  });
  

}
double hypercube_float::sum()  {
    return tbb::parallel_reduce(
        tbb::blocked_range<float*>( &vals[0], &vals[0]+getN123() ),
        0.f,
        [](const tbb::blocked_range<float*>& r, double init)->double {
            for( float* a=r.begin(); a!=r.end(); ++a )
                init += *a;
            return init;
        },
        []( double x, double y )->double {
            return x+y;
        }
    );

}
void hypercube_float::set_val( const double val){

  tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) this->vals[i]=val;
  });
}
void hypercube_float::normalize( const float val){
 float mymax=vals[0];

 for(int i=1; i< getN123(); i++){
 
   if(mymax < vals[i]) mymax=vals[i];
 }
 float sc;
 sc=val/mymax;


  tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) vals[i]*=sc;
  });

}
void hypercube_float::info( const char *str, const int level) const {
 fprintf(stderr,"**********************\n%s\n***********************\n TYPE=%s \n",str,name.c_str());
 hyper_info(level);
}
void hypercube_float::hyper_info( const int level) const {
 double sm=0.,mymin,mymax;
 if(vals.size()>0) mymax=mymin=vals[0];
 int imin=0,imax=0;

 if(vals.size()>0){
   mymin=vals[0];imin=0;
   mymax=vals[0];imax=0;
   for(int i=0; i < getN123(); i++){
     if(mymin > vals[i]) {mymin=vals[i]; imin=i;}
     if(mymax < vals[i]) {mymax=vals[i]; imax=i;}
     sm+=vals[i]*vals[i];
    }
}
  for(int i=0; i < getNdim(); i++) fprintf(stderr,"    n%d=%d",i+1,getAxis(i+1).n);
  fprintf(stderr,"\n       N=%d min(%d)=%g max(%d)=%g RMS=%g   \n",
      (int)getN123(), imin,
mymin,
imax,
mymax,
sqrt(sm)/(1.0*getN123()));

  long long print;
  if(level!=0 && vals.size()!=0){
  if(level<1)  print=getN123();
  else print=MIN(level,(int)getN123());
   for(long long i=0; i < print; i++){
     fprintf(stderr,"val %d %f \n",(int)i,vals[i]);
  } 
  }

 

}
void hypercube_float::power( const float val){

    tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) vals[i]=powf(vals[i],val);
  });

}
void hypercube_float::invert(){

  tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) vals[i]=1./vals[i];
  });
}
my_vector *hypercube_float::clone_vec() const {
  //my_vector *m=(my_vector*) this->clone();
  hypercube_float *m=this->clone();
  return (my_vector*)m;
}
void hypercube_float::norm_scale(  my_vector *norm_out){
  _norm->norm_scale(this,norm_out);
}
norm_vector *hypercube_float::fill_percentile(){

  
  hypercube_float *m=this->clone();
  percentile p;
  long long samp=std::min((long long)m->getN123()-1, (long long)(perc*m->getN123()));
 float val=p.find_abs(&m->vals[0], m->getN123(),samp);
  //val=1e6;
  if(fabs(val)<1e-6) {
    float def=1;
    fprintf(stderr,"WARNING: val[%d(%f)]=0. reset to %f\n",(int)samp,perc,def);
    val=def;
  }
  m->set_val(val);
  return (norm_vector*) m;
}
my_vector *hypercube_float::clone_space() const {
  //my_vector *m=(my_vector*) this->clone();
  hypercube_float *m=this->clone(false);

  return (my_vector*)m;
}
void hypercube_float::random(){
 
   tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) vals[i]=(float)rand()/(float)RAND_MAX-.5;
  });

}
double hypercube_float::dot( const my_vector *other) const {

  check_same(other);
  hypercube_float *o=(hypercube_float*) other;
 double sm= tbb::parallel_reduce(
        tbb::blocked_range<int>( 0,getN123() ),
        0.f,
        [this,o](const tbb::blocked_range<int>& r, double init)->double {
            for( int a=r.begin(); a!=r.end(); ++a )
                init += vals[a]*o->vals[a];
            return init;
        },
        []( double x, double y )->double {
            return x+y;
        }
    );


/*
   double sm=tbb::parallel_reduce(
   tbb::blocked_range<float*>(0,getN123()), 0.,
     [](const tbb::blocked_range<float*>&r, double smt)->double{
       for(float a=r.begin(); a != r.end(); ++a) smt+=*a*o->vals[i];
        return smt;},
        [] (double x, double y)->double{return x+y;}
     );
     */
  return sm;
}
void  hypercube_float::mult( const my_vector *other){
 
  check_same(other);
  hypercube_float *o=(hypercube_float*) other;
    tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) vals[i]*=o->vals[i];
  });

}
void  hypercube_float::scale_add( const double sc1,  const my_vector *v1,  const double sc2, const  my_vector *v2){
 
  check_same(v1);
    check_same(v2);
  hypercube_float *v_1=(hypercube_float*) v1;
    hypercube_float *v_2=(hypercube_float*) v2;
  tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) vals[i]=v_1->vals[i]*sc1+v_2->vals[i]*sc2;
  });
 

 
}
void hypercube_float::take_min( const my_vector*other, const my_vector *change){
 check_same(other);
       
 hypercube_float *c;
  hypercube_float *o=(hypercube_float*) other;
   if(change!=0){
       check_same(change);
       c=(hypercube_float*) change;
     }


  for(long long i=0; i < getN123(); i++){
    if(o->vals[i] < vals[i]){
      vals[i]=o->vals[i];
       if(change!=0) c->vals[i]=1;
    }
  }
 
 }
double hypercube_float::my_min() const {
  double my_min=vals[0];


  for(long long i=1; i < getN123(); i++){
    if(vals[i]< my_min) my_min=vals[i];
  }
  return my_min;
}
 double hypercube_float::my_max() const {
  double my_max=vals[0];
  for(long long i=1; i < getN123(); i++){
    if(vals[i]>my_max) my_max=vals[i];
  }
  return my_max;
}
 void hypercube_float::take_max( const my_vector *other, const my_vector *change){
     check_same(other);
hypercube_float *c;
     if(change!=0){
       check_same(change);
       c=(hypercube_float*) change;
     }
  hypercube_float *o=(hypercube_float*) other;
  for(long long i=0; i < getN123(); i++){
    if(o->vals[i] > vals[i]) {
      vals[i]=o->vals[i];
      if(change!=0) c->vals[i]=1;
    }
  }
    
    }
 void hypercube_float::scale_add(const double scale_me, const  my_vector *other, const double scale_other){
  check_same(other);
  hypercube_float *o=(hypercube_float*) other;
  float sc1=(float)scale_me,sc2=(float)scale_other;

     tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) vals[i]=vals[i]*sc1+o->vals[i]*sc2;
  });

  
}
void hypercube_float::add( const my_vector *other){
  check_same(other);
  hypercube_float *o=(hypercube_float*) other;

  tbb::parallel_for(tbb::blocked_range<long long>(0,getN123()),[&](
  const tbb::blocked_range<long long>&r){
  for(long long  i=r.begin(); i!=r.end(); ++i) vals[i]+=o->vals[i];
  });


}
bool hypercube_float::check_hypercube_match(const my_vector *v2) const {

   hypercube_float *h2=(hypercube_float*) v2;

   if(getNdimG1()!=h2->getNdimG1()){
     fprintf(stderr,"vectors not the same number of dimensions\n");
     return false;
   }

   for(int i=0; i < getNdimG1(); i++){
     axis a1=getAxis(i+1),a2=h2->getAxis(i+1);
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
void hypercube_float::axis_resample( const axis ain,  const axis aout,  int *iloc,   float *floc){
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
void  hypercube_float::resample( const hypercube_float *in){
  
  assert(in->getNdim() == getNdim() && getNdim()<4);
  int ndim=in->getNdim();
  int **iloc=new int*[ndim];
  float **floc=new float*[ndim];
  for(int i=0; i < ndim; i++){
    iloc[i]=new int[getAxis(i+1).n];
    floc[i]=new float[getAxis(i+1).n];
    axis_resample(in->getAxis(i+1),getAxis(i+1), iloc[i],floc[i]);
  }
  if(ndim==1){
    for(int i1=0; i1 < getAxis(1).n; i1++){
      vals[i1]=(1.-floc[0][i1])*in->vals[iloc[0][i1]]+floc[0][i1]*in->vals[iloc[0][i1+1]];
    }
  }
  else if(ndim==2){
    int n1=in->getAxis(1).n;
    int i=0;
    for(int i2=0; i2 < getAxis(2).n; i2++){
      for(int i1=0; i1 < getAxis(1).n; i1++,i++){
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
    int n1=in->getAxis(1).n;
    int n2=in->getAxis(2).n;
    int n12=n1*n2;
    int i=0;
    for(int i3=0; i3 < getAxis(3).n; i3++){
      for(int i2=0; i2 < getAxis(2).n; i2++){
        for(int i1=0; i1 < getAxis(1).n; i1++,i++){
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
