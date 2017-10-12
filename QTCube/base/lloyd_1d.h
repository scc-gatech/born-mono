#ifndef LLOYD_1D_H
#define LLOYD_1D_H 1
#include<vector>
#include<lloyd.h>
namespace SEP{
class lloyd_1d: public lloyd{

  public:
  lloyd_1d(){};
  ~lloyd_1d(){delete_lloyd_1d();}
  

  virtual  void find_bound();
  virtual void change_regions(int iter);


  void delete_lloyd_1d(){

  }
      void del_val(int i);
    void add_val(int i);
    void split_region();
   void set_basics(std::vector<float>tr, int nreg);
   
  std::vector<float> bound;
  std::vector<float> center;
  std::vector<int> region;
  std::vector<float> trace;
  std::vector<float> distort,wt,sum;
  float max_v,split_criteria,del_criteria;


};

class lloyd_1d_fit: public lloyd_1d{
  public:
    lloyd_1d_fit(){}
    lloyd_1d_fit(std::vector<float> tr, int nreg=40,float delc=.01,float splitc=.1);
    
    virtual void partition_space();
    virtual 
    void compute_center();
    virtual 
    void compute_distort();
    void compute_back();
     
    
  
    float ret_val(float loc){
      int iloc=(int)loc;
      float f=(loc-1.0*iloc);
      float v;
      v=(1.-f)*trace[iloc]+f*trace[iloc];
      return v;
    }
  
    std::vector<int> sign;
    std::vector<float> back;
    float range;
    
};
class lloyd_1d_p: public lloyd_1d{
  public:
    lloyd_1d_p(){}
    lloyd_1d_p(std::vector<float> tr, int nreg=40,float delc=.01,float splitc=.1);
    
    virtual void partition_space();
    virtual 
    void compute_center();
    virtual 
    void compute_distort();
    void compute_back();
     
    
  
    float ret_val(float loc){
      int iloc=(int)loc;
      float f=(loc-1.0*iloc);
      float v;
      v=(1.-f)*trace[iloc]+f*trace[iloc];
      return v;
    }
  
    std::vector<int> sign;
    std::vector<float> back;
    std::vector<float> mymin;
    std::vector<float> mymax;
    float range;
    bool writeit;
    
};
}
#endif
