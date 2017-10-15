#ifndef FILTER_H
#define FILTER_H 1
#include<vector>
class float_filter{
  public:
    float_filter(){};
    float_filter(std::vector<int> l, std::vector<float> f);
    ~float_filter(){ lag.clear(); filt.clear();}
    float_filter(float_filter *f);
    
    std::vector<int> lag;
    std::vector<float> filt;

};

#endif
