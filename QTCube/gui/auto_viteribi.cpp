#include "auto_viteribi.h"
#include "viterbi.h"
#include "my_score.h"
using namespace SEP;
std::vector<path> auto_viteribi::return_path(std::vector<int> locs,std::shared_ptr<float_2d> sim){

 //Add in nails
      int nlen=(sim->getAxis(1).n-1)/2;
      for(int i=0; i < (int) locs.size(); i++)
        sim->vals[nlen+locs[i]*(nlen*2+1)]=nail;
       //Find picked line
     std::shared_ptr<my_score> score(new my_score(pen_o,pen_e));
     viterbi myd=viterbi(score,sim,4,0);

      myd.calc_score();
      return myd.return_path(); 

}