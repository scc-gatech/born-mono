#ifndef LLOYD_MAP_H
#define LLOYD_MAP_H 1
#include "lloyd.h"
#include "hypercube_float.h"
class region{
  public:
    region(){};
    std::vector<float> return_center(){
      std::vector<float> cent;
      for(int i=0; i <(int)center.size(); i++) cent.push_back(cent);
      return cent;
    }
   std::vector<float> center;
  float var;

};

class region_1d: public region{
  public:
  region_1d(int ib, int ie, hypercube_float *tr);
  int i_b, int i_e;
  
}

class lloyd_map: public lloyd{
  public:
    lloyd_map(){};
    virtual void irreg_to_hyper(hypercube_float *hyper){assert(1==0);}
  
  
  
  
};

class lloyd_map_1d: public lloyd_map{
  public:
    lloyd_map_1d(float_1d *vec);
  
    void find_bound();
    void partition_space();
    void compute_center();

  
    
  
  
};
#endif