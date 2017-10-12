#ifndef QTWIDGETBASIC 
#define QTWIDGETBASIC 1
#include <QWidget>
#include "position.h"
#include "panels.h"
#include "windows.h"
#include "datasets.h"
#include "pick_draw.h"
#include "slice_types.h"
#include "my_fonts.h"
#include "my_colors.h"
#include "view_types.h"
#include <QWidget>
#include <QHBoxLayout>
#include<QGroupBox>
#include<QComboBox>
#include <QPushButton>
#include "qcube_panel.h"
#include <vector>
namespace SEP{
class qcubeWidgetBasic: public QWidget,public qcube_panel{
  Q_OBJECT
  public: 
    qcubeWidgetBasic(){;}
  
     ~qcubeWidgetBasic(){ clean_basic();}

   private:
     void clean_basic();

};
}


#endif
