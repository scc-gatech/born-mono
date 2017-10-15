#ifndef FLATTEN_OP
#define FLATTEN_OP 1
#include <my_operator.h>
#include <vector>
#include<hypercube_float.h>
   extern "C" {

#include<fftw3.h>
void fftw_test2_(int *n1, int *n2, int *n3, int *zero,
float *temp, long long *p1,long long *p2);
}
class pt3{
  public:
    pt3(int i,int j, int k){ i1=i; i2=j; i3=k;}
    int i1,i2,i3;
};

class flatten_op:public my_operator{
  public:
    flatten_op(){};
    flatten_op(hypercube_float *mod,hypercube_float *dat, float eps, 
      std::vector<pt3> msk,int imaster,std::vector <int> froze);
    virtual bool forward(bool add, my_vector *mod, my_vector *dat){ 
      hypercube_float *m=(hypercube_float*) mod, *d=(hypercube_float*)dat;
     both_op(false,add,m->vals,d->vals);
     return true; 
    }
    virtual bool adjoint(bool add, my_vector *mod, my_vector *dat){ 
     hypercube_float *m=(hypercube_float*) mod, *d=(hypercube_float*)dat;
      both_op(true,add,m->vals,d->vals);
      return true;
    }
    void set_scale(bool sc){ scale=sc;}
    void scale_it(float *dat);
    void both_op(bool adj, bool adj, float *mod, float *dat);
    ~flatten_op(){

      delete [] tbuf;

      for(int i3=0; i3 < n3;i3++) mask[i3].clear();

      delete [] mask;

     }
 
    
    float eps;
    int n1, n2,n3;
    long long n123;
    float *tbuf;
    bool scale;
    float mys;
    std::vector<int> *mask;
    std::vector<pt3> mskit;
    std::vector<int> frozen;
};

class flatten_prec_op:public flatten_op{
  public:
  
    flatten_prec_op(hypercube_float *mod, hypercube_float *dat, float eps, 
      std::vector<pt3> msk,int imaster);
    
    virtual bool forward(bool add, my_vector *mod, my_vector *dat){ 
      hypercube_float *m=(hypercube_float*) mod, *d=(hypercube_float*)dat;
      combo_op(false,add,m->vals,d->vals);
      return true;
    }
    void set_prec(float *m){ mult=m;}
    virtual bool adjoint(bool add, my_vector *mod, my_vector *dat){ 
     hypercube_float *m=(hypercube_float*) mod, *d=(hypercube_float*)dat;
      combo_op(true,add,m->vals,d->vals);
      return true;
    }
    

    void combo_op(bool adj, bool adj, float *mod, float *dat);
    ~flatten_prec_op(){
      delete [] mid;

        fftwf_destroy_plan( planF);
        fftwf_destroy_plan( planI);

     }
  private:

    float *mult,*mid;
    fftwf_plan planF,planI;
};

#endif
