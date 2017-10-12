#ifndef WINDOW_PANEL_H
#define WINDOW_PANEL_H 1
#include "qcubewidgetbasics.h"
#include "qcubetabbasics.h"

#include "widget_tools.h"

#include <vector>
namespace SEP{
class window_panel: public qcubeTabBasic{
  Q_OBJECT
  public: 
  window_panel(std::shared_ptr<position> p,std::shared_ptr<windows>my_w, std::shared_ptr<panels> my_p, 
  std::shared_ptr<datasets> my_d, std::shared_ptr<pick_draw> pk, std::shared_ptr<slice_types>c,std::shared_ptr<maps>my_m);

       void set_current_index(int ind){ setCurrentIndex(ind);}
     void update_menu(std::vector<QString>);
       signals:
       void actionDetected(std::vector<QString> text);
  private slots:
    void actionRespond(std::vector<QString> text){
       emit actionDetected(text);
    }
};
class sub_window_panel: public qcubeWidgetBasic{
  Q_OBJECT
  public: 
    sub_window_panel(std::shared_ptr<position>p,std::shared_ptr<windows> my_w, std::shared_ptr<panels>my_p, 
    std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk, std::shared_ptr<slice_types>c,
    std::shared_ptr<maps> m,QString w);
  
     void actionRespond(std::vector<QString>){;}
     ~sub_window_panel(){ delete_sub_window_panel();}
     void update_menu(std::vector<QString>);
       signals:
       void actionDetected(std::vector<QString> text);
   private:
     void delete_sub_window_panel();
     basicComboBox *splitXBox,*splitYBox,*boundaryBox;
     QHBoxLayout *mainLay,*saveLay;
     QGroupBox *saveBox;
     QComboBox  *boundary;
     QPushButton *saveHigh,*saveLow,*closeButton,*splitButton;
     QString myw;     
 private slots:
   void save_big();
   void save_small();
   void split_x(const QString com);
   void split_y(const QString com);
   void boundary_change(const QString com);
   void close_window();


 
};

}

#endif
