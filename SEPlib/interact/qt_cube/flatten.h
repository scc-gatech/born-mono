#include<vector>
#ifndef FLATTEN_H
#define FLATTEN_H 1
   extern "C" {

#include<fftw3.h>
void fftw_test2_(int *n1, int *n2, int *n3, float *temp, long long *p1,long long *p2);
}
class ploc{
  public:
    ploc(int i,int j,int k){ i1=i; i2=j; i3=k;}
    int i1,i2,i3;
};
class filt{
  public:
    filt(float f1,float f2, float f3){ f[0]=f1; f[1]=f2; f[2]=f3;}
    float f[3];
   };
class flatten{

  public:
    flatten(){alloc=false;}
    flatten(int *n, float e,int lck,int aa,int niter);
    void create_mult();
    double nonlinear_iter(float*, float*);
    void calc_tau(float *tau, float *dips);
    void add_map_tau(float *tau, float *dtau, float o ,float d);
    void calc_grad(float *tau, float *grad1, float *grad2,float *grad3);
    void map_grad_tau(float *tau, float *dip, float *grad,float *oo, float *dd);
     void apply_map(float *tmap,float *in, float *out,bool cp=true);
    void find_z_tau_map(float *tau, float *map, float o, float d);
     void create_bank(int n, float min, float max);
    double nonlinear_iter2(float *tau, float *dip);
      double nonlinear_iter3(float *tau, float *dip);
            double nonlinear_iter4(float *tau, float *dip, float *tdip);

    void apply_filt(bool adj, bool add, float *mod, float *dat,float *tau);
    void apply_filt2(bool adj, bool add, float *mod, float *dat);
    void grad3(bool adj, bool add, float *mod, float *dat);
        void convert_tau_lin(bool adj, bool add, float *mod, float *dat);

    ~flatten(){
      if(alloc){
        delete[] temp;
       
        delete [] resid;
        alloc=false;
        fftwf_destroy_plan( planF);
        fftwf_destroy_plan( planI);
        delete[] mult;
        delete []gmap;
      }
    }
    void set_fix(std::vector<ploc> pv);
    void set_fixed(float *tau);
  private:
    bool alloc;
    float *temp,*mult,*resid;
    float *gmap;
    double *temp2;
    fftwf_plan planF,planI;
    int iloc,iloc2,iloc3,n1,n2,n3,niter,lock,iii,jjj;
    float eps_t;
    std::vector<ploc> fix;
    std::vector<filt> bank;
    float delta,ofilt;
    float otau,dtau;
    long long n123;
   
};
#endif

   
 
