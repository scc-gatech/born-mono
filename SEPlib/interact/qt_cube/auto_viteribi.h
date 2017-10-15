#ifndef AUTO_VITERIBI
#define AUTO_VITERIBI 1
#include "autopick_2d.h"
class auto_viteribi:public autopick_2d{
  public:
  auto_viteribi(){};
  auto_viteribi(float peno,float pene, float nl){ pen_o=peno; pen_e=pene;nail=nl;}
  std::vector<path> return_path(std::vector<int> pik,float_2d *vals);

  float pen_o,pen_e,nail; 

};
#endif
