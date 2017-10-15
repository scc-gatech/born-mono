#ifndef MOUSE_REGION_H
#define MOUSE_REGION_H 1
#include "mouse_func.h"
class mouse_region: public mouse_func{

 public:
 mouse_region();
 virtual void r_mouse_d(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
 virtual void r_mouse_m(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
 virtual void r_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
 virtual void l_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
 virtual void l_mouse_double(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
 virtual void m_mouse_double(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
 virtual void m_mouse_r(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
 virtual void m_mouse_d(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);
 virtual void m_mouse_m(std::vector<slice*> slices, int islice, int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o);

};
#endif