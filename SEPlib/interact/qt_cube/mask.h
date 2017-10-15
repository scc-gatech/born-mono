#ifndef MASK_H
#define MASK_H 1
#include "my_vector.h"
class mask{
 public:
 mask(){set_basic_mask();}
 void set_basic_mask(){space=0;}
 bool check_match(my_vector *vec);
 virtual void apply(my_vector *vec){
   fprintf(stderr,"APPLY UNDEFINED \n");
 }
 void set_space(my_vector *vec){
   fprintf(stderr,"IN SPACE \n");
   space=vec->clone_space();
   
 }
 virtual ~mask(){
   if(space!=0) delete space;
 }

 my_vector *space;
};
#endif
