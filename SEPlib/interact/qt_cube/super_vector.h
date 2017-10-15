#ifndef SUPER_VECTOR_H
#define SUPER_VECTOR_H 1
#include "norm_vector.h"
#include <vector>
class super_vector: public norm_vector{
  public:
    super_vector(){};
    super_vector(norm_vector *v1, norm_vector *v2,bool sp=false);
    super_vector(norm_vector *v1, norm_vector *v2, norm_vector *v3,bool sp=false);
    super_vector(std::vector<norm_vector*> vs,bool sp=false);
    void add_vector(norm_vector *v1);
    norm_vector *return_vector(int ivec);
    virtual void scale(const double num);
    virtual void scale_add(const double mes,  my_vector *vec, const double other) ;
    virtual my_vector *return_norm_param();

    virtual void set_val(double  val);
    virtual void add(my_vector *vec);
        virtual void mult(my_vector *vec);

    virtual double dot(my_vector *vec);
    virtual void info(char *str,int level=0);
    virtual void random();
    virtual void power(float val);
    virtual norm_vector *fill_percentile();
    void norm_scale(my_vector *norm_out);
    virtual void calc_norm_deriv(my_vector *rw,my_vector *c0, my_vector *c1,my_vector *c2);
    virtual void invert();
    virtual void inverse_hessian(my_vector *vec);
    virtual double sum();
    my_vector *clone_vec(){return clone_it(false);}
    my_vector *clone_space(){return clone_it(true);}
    my_vector *clone_it(bool alloc);
  
    virtual bool check_match(const my_vector *v2);
    void super_init(std::string n){ name=n;}
    
    ~super_vector(){
      for(int i=0; i < (int)vecs.size(); i++) delete vecs[i];
      vecs.clear();
    }
    
    
    bool just_space;
    std::vector<norm_vector*> vecs;



};

#endif
