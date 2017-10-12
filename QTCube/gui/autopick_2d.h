#ifndef AUTOPICK_2D_H
#define AUTOPICK_2D_H 1
#include "path.h"
#include "float_2d.h"
namespace SEP{
class autopick_2d{
  public:
 autopick_2d(){};

 
  virtual std::vector<path> return_path(std::vector<int> pik,std::shared_ptr<SEP::float_2d>array);
  virtual ~autopick_2d(){};

};
}
#endif
