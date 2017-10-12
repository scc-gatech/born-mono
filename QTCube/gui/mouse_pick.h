#ifndef MOUSE_PICK
#define MOUSE_PICK 1
#include "mouse_func.h"
namespace SEP{
class mouse_pick: public SEP::mouse_func{
 public:
 mouse_pick();
 //virtual std::vector<QString> r_mouse_d(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o);
 //virtual std::vector<QString> r_mouse_m(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o);
 void l_mouse_r(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 void m_mouse_r(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 //void r_mouse_r(std::vector<std::shared_ptr<slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<draw_other>draw_o);
 void  r_mouse_d(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);

void  r_mouse_m(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
  void  r_mouse_r(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
  

};
}
#endif
