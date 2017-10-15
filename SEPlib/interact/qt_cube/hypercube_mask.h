#ifndef HYPERCUBE_MASK_H
#define HYPERCUBE_MASK_H 1
#include "mask.h"
#include <vector>
class hypercube_mask_pts:public mask{
  public:
    hypercube_mask_pts(){ set_basic_mask();}
    hypercube_mask_pts(my_vector *sp, std::vector<long long> map);
    ~hypercube_mask_pts(){ map.clear();}
    virtual void apply(my_vector *v);
    std::vector<long long> map;
};
class hypercube_mask_vec:public mask{
  public:
    hypercube_mask_vec(){ set_basic_mask();}
    hypercube_mask_vec(my_vector *sp, std::vector<bool> map);
    ~hypercube_mask_vec(){ map.clear();}
    virtual void apply(my_vector *v);
    std::vector<bool> map;
};
#endif
