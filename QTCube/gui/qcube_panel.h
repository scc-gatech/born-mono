#ifndef QCUBE_PANEL_H
#define QCUBE_PANEL_H 1
#include "position.h"
#include "panels.h"
#include "windows.h"
#include "datasets.h"
#include "pick_draw.h"
#include "slice_types.h"
#include "my_fonts.h"
#include "my_colors.h"
#include "view_types.h"
#include "maps.h"
#include <QWidget>
#include <QHBoxLayout>
#include<QGroupBox>
#include<QComboBox>
#include <QPushButton>
#include <vector>
namespace SEP{
class qcube_panel{
 public:
   qcube_panel(){;}
   void set_basics(std::shared_ptr<SEP::position>p,std::shared_ptr<SEP::windows>my_w, 
   std::shared_ptr<SEP::panels>my_p, std::shared_ptr<SEP::datasets>my_d, 
   std::shared_ptr<SEP::pick_draw>pk, std::shared_ptr<SEP::slice_types>c, 
   std::shared_ptr<SEP::maps> m);
   ~qcube_panel(){ delete_qcube_panel();}
    virtual void actionRespond(std::vector<QString>){;}


     std::shared_ptr<position>pos,my_pos;
     std::shared_ptr<windows>my_wind;
     std::shared_ptr<panels>my_pan;
     std::shared_ptr<datasets>my_dat;
     std::shared_ptr<pick_draw>my_pick;
     std::shared_ptr<slice_types>my_slices;
     std::shared_ptr<my_colors>my_c;
     std::shared_ptr<my_fonts>my_f;
     std::shared_ptr<maps>my_maps;
     std::shared_ptr<view_types> my_v;


  private:
    void delete_qcube_panel();


};

}
#endif

