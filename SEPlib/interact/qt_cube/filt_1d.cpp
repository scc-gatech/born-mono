#include "filt_1d.h"

deriv_filt::deriv_filt(int n){
  std::vector<int> l;
  std::vector<float> f;
  l.push_back(1);
  f.push_back(-1.);
  float_filter filt=float_filter(l,f);
  set_1d(filt,n);
}
deriv2_filt::deriv2_filt(int n){
  std::vector<int> l;
  std::vector<float> f;
  l.push_back(-1);
  f.push_back(-1.);
  float_filter filt=float_filter(l,f);
  set_1d(filt,n);
}

decay_filt::decay_filt(int n,float decay){
  std::vector<int> l;
  std::vector<float> f;
  l.push_back(1);
  f.push_back(-decay);
  float_filter filt=float_filter(l,f);
  set_1d(filt,n);
}