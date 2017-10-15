#include "nfilter.h"

basic_float_filter::basic_float_filter(float_filter *f){
  filt=float_filter(f);
}
basic_float_filter::basic_float_filter(std::vector<int> l, std::vector<float>f){

   filt=float_filter(l,f);
}

variable_float_filter::variable_float_filter(std::vector<float_filter> fs, std::vector<int> map){

  for(int i=0; i < (int) fs.size(); i++){
     filts.push_back(new float_filter(fs[i]));
  
}
  for(long long i; i < map.size(); i++){
    if(map[i] < 0 || map[i] >= (int)filts.size()){
      fprintf(stderr,"ERROR map[%d]=%d out of range of filter bank 0,%d \n",
         (int)i,map[i],(int)filts.size());
    }
    filt_map.push_back(map[i]);
 }


}
void float_filt_1d::set_1d(float_filter f, int n){
  real=float_filter(&f);
  std::vector<int> lag;
  std::vector<float> flt;
  dummy=float_filter(lag,flt);
  n1=n;
}
