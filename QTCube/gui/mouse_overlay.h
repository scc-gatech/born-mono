#ifndef MOUSE_OVERLAY_H
#define MOUSE_OVERLAY_H 1
#include "mouse_func.h"
namespace SEP{
class mouse_overlay: public SEP::mouse_func{

 public:
 mouse_overlay();
 virtual void r_mouse_d(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<SEP::orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 virtual void r_mouse_m(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<SEP::orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 virtual void r_mouse_r(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<SEP::orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 virtual void m_mouse_r(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<SEP::orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 virtual void l_mouse_d(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<SEP::orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 virtual void l_mouse_m(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<SEP::orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 virtual void l_mouse_r(std::vector<std::shared_ptr<SEP::slice>> slices, int islice, int ix, int iy, std::shared_ptr<SEP::orient_cube>pos, std::vector<QString> *com, std::shared_ptr<SEP::draw_other>draw_o);
 QString get_info() {return label;}
 
 
  private:
    QString label;
};
}
#endif
