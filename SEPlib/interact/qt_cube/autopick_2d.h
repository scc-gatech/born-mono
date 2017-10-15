#ifndef AUTOPICK_2D_H
#define AUTOPICK_2D_H 1
#include "path.h"
#include "float_2d.h"
class autopick_2d{
  public:
 autopick_2d(){};

 
  virtual std::vector<path> return_path(std::vector<int> pik,float_2d *array);
  virtual ~autopick_2d(){};

};
#endif
