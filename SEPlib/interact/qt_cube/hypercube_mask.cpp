#include "hypercube_mask.h"
#include "hypercube_float.h"

hypercube_mask_pts::hypercube_mask_pts(my_vector *sp, std::vector<long long> m){
 fprintf(stderr,"SET MASK1 \n");
  set_space(sp);
   fprintf(stderr,"SET MASK2 \n");

  hypercube_float *v=(hypercube_float *)sp;
  long long n123=v->get_n123();
  for(long long i=0; i < m.size(); i++){
    if(m[i] <0 || m[i] >= n123) fprintf(stderr,"ERROR: Mask out of vector range %d (%d) \n",
       (int)i,(int)n123);
    map.push_back(m[i]);
  }
  
}
void hypercube_mask_pts::apply(my_vector *v){
    hypercube_float *vec=(hypercube_float *)v;
    for(int i=0; i < (int) map.size(); i++) vec->vals[map[i]]=0.;

}
hypercube_mask_vec::hypercube_mask_vec(my_vector *sp, std::vector<bool> m){
  set_space(sp);
  hypercube_float *v=(hypercube_float *)sp;
  long long n123=v->get_n123();
  if(n123!=m.size())
    fprintf(stderr,"ERROR: Mask and vector not the same size \n");
   
  for(long long i=0; i < m.size(); i++){
    map.push_back(m[i]);
  }
}
void hypercube_mask_vec::apply(my_vector *v){
 hypercube_float *vec=(hypercube_float *)v;
    for(int i=0; i < (int) map.size(); i++) {
      if(map[i]) vec->vals[i]=0.;
    }
}

