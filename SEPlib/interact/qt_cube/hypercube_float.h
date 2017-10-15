#ifndef HYPERCUBE_FLOAT_H
#define HYPERCUBE_FLOAT_H 1
//#include<cg.h>
#include<axis.h>
#include<hypercube.h>
#include"percentile.h"
#include "norm_vector.h"

class hypercube_float: public hypercube, public norm_vector{

  public:
    hypercube_float()
       {this->vals=0; _norm=0;}//Default
       

    virtual my_vector *clone_vec();
     virtual my_vector *clone_space();
    void check_same(my_vector *other){ if(other==0){}}
    double dot(my_vector *other);
    virtual void scale_add(const double mes,  my_vector *vec, const double other);
    void add(my_vector *other);
    hypercube_float(std::vector<axis> axes,bool alloc=true);
    hypercube_float(std::vector<axis> axes, float *vals);
    hypercube_float(hypercube *d);
    virtual void random();
         virtual void scale(double r){for(int i=0; i< get_n123(); i++) vals[i]=vals[i]*r;}
    void add(hypercube_float *vec);
    virtual void take_min(my_vector*vec,my_vector *other=0);
    virtual void take_max(my_vector *vec,my_vector *other=0);
        virtual void mult(my_vector *vec);
    virtual double sum();
    virtual void scale_add(const double sc1, my_vector *v1,double sc2, my_vector *v2);
    virtual void invert();
    virtual void power(float val);

        virtual double my_min();
    virtual double my_max();
    hypercube_float *clone(bool alloc=true);
    void set(float *vals);
    void set_val(double val);
    void normalize(float val);
    virtual void allocate(){
      if(this->vals!=0) deallocate();
      this->vals=new float[this->get_n123()];
     }
     void init_ndf(std::vector<axis> ax){ init_nd(ax); allocate();}
     void  resample(hypercube_float *in);
     
    
     void deallocate(){
         if(this->vals!=0) delete []this->vals;
         vals=0;
     }
     virtual ~hypercube_float(){
       this->deallocate();
      }
    virtual norm_vector *fill_percentile();
     void norm_scale(my_vector *norm_out);
    virtual bool check_match(const my_vector *v2){
      if(-1==(int) v2->name.find("hypercube_float")){
        fprintf(stderr,"vector not hypercube_float");
        return false;
      }
      return check_hypercube_match(v2);
    }
    bool check_hypercube_match(const my_vector *v2);
        virtual void info(char *str,int level=0);
    void hyper_info(int level);
    float *vals;
    
    
    
    private:
      void axis_resample(axis ain,axis aout, int *i, float *f);
 };
 
 #endif
 

