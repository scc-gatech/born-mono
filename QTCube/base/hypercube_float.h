#ifndef HYPERCUBE_FLOAT_H
#define HYPERCUBE_FLOAT_H 1
#include<iostream>
#include<memory>
//#include<cg.h>
#include<axis.h>
#include<hypercube.h>
#include"percentile.h"
#include "norm_vector.h"

namespace SEP{
class hypercube_float: public SEP::hypercube, public norm_vector{

  public:
    hypercube_float()
       {}//Default
       

    virtual my_vector *clone_vec() const;
     virtual my_vector *clone_space() const;
    void check_same( const my_vector *other)const { if(other==0){;};}
    double dot( const my_vector *other) const;
    virtual void scale_add(const double mes,  const my_vector *vec, const double other);
    void add( const my_vector *other);
    hypercube_float( const std::vector<SEP::axis> axes, const bool alloc=true);
    hypercube_float( const std::vector<SEP::axis> axes,  const float *vals);
    hypercube_float( const SEP::hypercube *d);
    hypercube_float( const std::shared_ptr<SEP::hypercube>d);

    virtual void random();
         virtual void scale( const double r){for(long long i=0; i< getN123(); i++) vals[i]=vals[i]*r;}
    void add( const hypercube_float *vec);
    virtual void take_min( const my_vector*vec, const my_vector *other=0);
    virtual void take_max( const my_vector *vec, const my_vector *other=0);
        virtual void mult( const my_vector *vec);
    virtual double sum()  ;
    virtual void scale_add(const double sc1, const  my_vector *v1, const double sc2,  const my_vector *v2);
    virtual void invert() ;
    virtual void power( const float val);

        virtual double my_min() const ;
    virtual double my_max() const ;
    hypercube_float *clone( const bool alloc=true) const;
    void set( const float *vals);
    void set_val( const double val);
    void normalize( const float val);
    virtual void allocate(){
      vals.resize(this->getN123());
     }
     void init_ndf( const std::vector<SEP::axis> ax){ initNd(ax); allocate();}
     void  resample( const hypercube_float *in);
     
    
     void deallocate(){
         vals.resize(0);
   
     }
     virtual ~hypercube_float(){
       this->deallocate();
      }
    virtual norm_vector *fill_percentile();
     void norm_scale(  my_vector *norm_out);
    virtual bool check_match(const my_vector *v2){
      if(-1==(int) v2->name.find("hypercube_float")){
        fprintf(stderr,"vector not hypercube_float");
        return false;
      }
      return check_hypercube_match(v2) ;
    }
    bool check_hypercube_match(const my_vector *v2) const ;
        virtual void info( const char *str, const int level=0) const ;
    void hyper_info(int level) const ;
    std::vector<float> vals;
    
    
    
    private:
      void axis_resample( const SEP::axis ain, const SEP::axis aout,  int *i,   float *f);
 };
 }
 #endif
 

