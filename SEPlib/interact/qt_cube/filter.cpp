#include "filter.h"
    
    
 float_filter::float_filter(std::vector<int> l, std::vector<float> f){
   if(l.size()!=f.size()) fprintf(stderr,"ERROR FILTER AND LAG NOT THE SAME SIZE\n");
   for(int i=0; i < (int)l.size(); i++) {
       filt.push_back(f[i]);
       lag.push_back(l[i]);
   }
 }
 
 float_filter::float_filter(float_filter *f){
 
   for(int i=0; i < f->filt.size(); i++){
       filt.push_back(f->filt[i]);
       lag.push_back(f->lag[i]);
  }
 
 
 }
 