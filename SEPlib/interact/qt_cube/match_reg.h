#ifndef MATCH_REG
#define MATCH_REG 1
#include "reg_tree.h"
class match_reg{
 public:
  match_reg(){}
  match_reg(std::vector<int> split_axis, hypercube_float *v,int ntr);
  hypercube_float *compute_back();
  void break_up(float max_err);
  void split_big_errors(std::vector<reg_tree*>branches,  std::vector<float> err,
  float emax);
  float calc_errors(std::vector<reg_tree*> branches, std::vector<int> map,
  hypercube_float *back, std::vector<float> *err);
  float max_break_error(float max_err);
  void construct_map_back(std::vector<reg_tree*> branches, std::vector<int> *map,
  hypercube_float *back);
  reg_tree *my_tree;
  hypercube_float *tr;
  int niter;
  float abs_avg;

  


};
#endif