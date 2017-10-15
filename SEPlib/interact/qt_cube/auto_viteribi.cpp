#include "auto_viteribi.h"
#include "viterbi.h"
#include "my_score.h"
std::vector<path> auto_viteribi::return_path(std::vector<int> locs,float_2d *sim){

 //Add in nails
      int nlen=(sim->get_axis(1).n-1)/2;
      for(int i=0; i < (int) locs.size(); i++)
        sim->vals[nlen+locs[i]*(nlen*2+1)]=nail;
       //Find picked line
     my_score score(pen_o,pen_e);
     viterbi myd=viterbi(&score,sim,4,0);

      myd.calc_score();
      return myd.return_path(); 

}
