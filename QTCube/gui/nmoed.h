#include "moved_out.h"
#ifndef NMOED_H
#define NMOED_H 1
#include<string>
#include "datasets.h"
#include "pick_draw.h"
namespace SEP{
class nmoed: public moved_out{

  public:
     nmoed(){};
     nmoed(std::shared_ptr<SEP::hypercube>grid, std::shared_ptr<SEP::dataset>dat, int it,int ioff, int imes,
       std::shared_ptr<SEP::paramObj>pars, std::shared_ptr<SEP::pick_draw>pk,QString col1, QString col2);
     virtual void move_it(std::vector<int>&f,float *nmo);

     virtual void afterInitialization();
     float smute;
     bool done;
    
   private:
     
   std::shared_ptr<SEP::hypercube >_grid;
   std::shared_ptr<SEP::dataset>_dat;
  int _it,_ioff,_imes;
   std::shared_ptr<SEP::paramObj>_pars; 
 std::shared_ptr<SEP::pick_draw>_pk;
 QString _col1,_col2;
 
 };
 }
#endif
