 #include "mask.h"
 bool mask::check_match(my_vector *vec){
    if(vec==0) {
      fprintf(stderr,"MASK space vector undefined\n");
      return false;
    }
    return space->check_match(vec);
 }