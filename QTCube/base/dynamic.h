#ifndef DYNAMIC_H
#define DYNAMIC_H 1
#include<float_2d.h>
#include <path.h>
#include<memory>
namespace SEP{
class dynamic{

  public:
    dynamic(){sim=0; score=0;};
    dynamic(SEP::float_2d *buf, float jump_pen);
    void add_to_sim(SEP::float_2d *s);
    virtual void calc_score();
    virtual std::vector<SEP::path> return_path();
 
    virtual ~dynamic(){delete_ar();}

    void delete_ar(){
    
    }
   std::shared_ptr<SEP::float_2d>sim,score;
   float jump;
   int maxloc[2];
  
};
}

#endif
