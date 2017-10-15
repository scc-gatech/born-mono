#ifndef REGION_DATA_H
#define REGION_DATA_H 1
#include "incore_data.h"
#include "region.h"
class region_data: public incore_data_byte{
  
   public:
     region_data(hypercube *g, param_func *p, int in);
    
    region *reg;

};


#endif
