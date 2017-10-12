#ifndef QTTABBASIC 
#define QTTABBASIC 1
#include <QTabWidget>
#include "position.h"
#include "panels.h"
#include "windows.h"
#include "datasets.h"
#include "pick_draw.h"
#include "slice_types.h"
#include "my_fonts.h"
#include "my_colors.h"
#include "view_types.h"
#include "qcube_panel.h"
#include <map>
#include <vector>
namespace SEP{
class qcubeTabBasic: public QTabWidget, public qcube_panel{
  Q_OBJECT
  public: 
    qcubeTabBasic(){;}
  
     ~qcubeTabBasic(){ clean_basic();}

     std::map<QString,QWidget*> my_tabs;
    private:

     void clean_basic();

};

}

#endif
