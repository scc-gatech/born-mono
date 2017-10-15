#ifndef FLAT_SIZE_H
#define FLAT_SIZE_H 1
#include<flatten_op.h>
#include<flatten_fourier.h>
class delta_mod: public hypercube_float{
  public:
    delta_mod(){}
    delta_mod(std::vector<axis> ax, float *m,int imaster,std::vector<pt3> p, int it){ 
      init_nd(ax);
       im=imaster;
       iter=it;

      vals=new float[get_n123()];
      int zero=0;
      fprintf(stderr,"AXIS SIZE %d \n",(int)ax.size());
        n1=ax[0].n; n2=ax[1].n; n3=ax[2].n;
       iloc=p[im].i2*n1+p[im].i3*n1*n2;
      fftw_test2_(&n1,&n2,&n3,&zero,this->vals,(long long*)&planF,(long long*)&planI);
      mult=m;
      pp=p;
    
    }
  void inverse_hessian(my_vector *vec){
    this->scale_add(0.,vec,1.);
    return;
    hypercube_float *v=(hypercube_float*) vec;
    memcpy(vals,v->vals,get_n123()*sizeof(float));
    fftwf_execute(planF);
    for(int i=0; i < get_n123(); i++) vals[i]*=-
      mult[i];
    fftwf_execute(planI);
    float *base=new float [n1];
    
    fprintf(stderr,"in inverswe mult \n");
    memcpy(base,&vals[iloc],n1*4);
    for(int i2=0; i2 <n2*n3; i2++) 
      for(int i1=0; i1 < n1; i1++) vals[i1+i2*n1]-=base[i1];
    int ii=pp[im].i1;

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
class flat_size{

  public:
    flat_size(){}
    flat_size(hypercube_float *dips, hypercube_float *tau, int j, std::vector<pt3> fix,
     int im,bool four);

    ~flat_size(){ if(sub){
      delete dips_small; delete tau_small;}
      delete data; fix.clear();
      aout.clear();
    }
    int jfact(int n,int fact);
    void update_tau(float eps_t,int nliter, int niter);
    void resample_down();
    void classic_cg( my_operator *op, hypercube_float *g, hypercube_float *mod, hypercube_float *a,int niter);
    hypercube_float *create_data( hypercube_float *dips, hypercube_float *tau, 
  int iloc2, int iloc3);
     
 
    void resample_up();

  void freeze(int i){ frozen.push_back(i/jsamp[0]);}


  private:
    hypercube_float *tau_big,*dips_big;
    int jsamp[3];
    int imaster;
    int n1,n2,n3;
    hypercube_float *tau_small,*dips_small,*data;
    std::vector<pt3> fix;
    std::vector<axis> aout;
    bool sub;
    bool fourier;
    bool view;
    
    std::vector<int> frozen;


};

#endif
