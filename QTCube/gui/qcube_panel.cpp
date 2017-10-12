#include "qcube_panel.h"
using namespace SEP;
void qcube_panel:: set_basics(std::shared_ptr<SEP::position>p,std::shared_ptr<SEP::windows >my_w, 
std::shared_ptr<SEP::panels>my_p, std::shared_ptr<SEP::datasets >my_d, std::shared_ptr<SEP::pick_draw >pk, 
std::shared_ptr<SEP::slice_types>c,std::shared_ptr<SEP::maps> mym){

  pos=p;
  my_wind=my_w;
  my_pan=my_p;
  my_dat=my_d;
  my_maps=mym;
  my_pick=pk;
  my_slices=c;
  std::shared_ptr<SEP::my_fonts> myf(new my_fonts());
  std::shared_ptr<SEP::view_types> myv(new view_types());
  std::shared_ptr<SEP::my_colors> myc(new my_colors());

  my_f=myf;
  my_v= myv;
  my_c = myc;
  my_pos=p;
}
void qcube_panel::delete_qcube_panel(){

}
