#ifndef navigate_panel_H
#define navigate_panel_H 1
#include <QVBoxLayout>
#include "qcubewidgetbasics.h"
#include "widget_tools.h"

#include <vector>
namespace SEP{
class navigate_panel: public qcubeWidgetBasic{
  Q_OBJECT
  public: 
    navigate_panel(std::shared_ptr<SEP::position>p,std::shared_ptr<SEP::windows>my_w, std::shared_ptr<SEP::panels>my_p, 
    std::shared_ptr<SEP::datasets>my_d, std::shared_ptr<SEP::pick_draw>pk, std::shared_ptr<SEP::slice_types>c,
    std::shared_ptr<SEP::maps> mym);
  
     virtual void actionRespond(std::vector<QString>){;}
     ~navigate_panel(){ delete_row_1();delete_row_2(); delete_row_3(); delete layMain;}
     void update_menu(std::vector<QString>);
  
  signals:
       void actionDetected(std::vector<QString> text);
  private slots:
      void x_neg_clicked();
      void x_pos_clicked();
      void y_neg_clicked();
      void y_pos_clicked();      
      void z_neg_clicked();
      void z_pos_clicked();
      void go_clicked();
      void stop_clicked();
      void update_clicked();
      void step_clicked();
      void inc_changed(const QString&txt);
      void speed_changed(int);

  private:
     void delete_row_1();
     void delete_row_2();
     void delete_row_3();
     QWidget *build_row_1();
     QWidget * build_row_2();
     QWidget * build_row_3();

    basicButtonBox *step;
    basicSliderBox *speed;
     basicRadioBox *direction,*movieGo;
     basicLineEditBox *increment;
    std::vector<basicLineEditBox*> pos;
    basicButtonBox *update;
          QVBoxLayout *layMain;
     QHBoxLayout *lay1,*lay2,*lay3;
     QWidget *row1,*row2,*row3;

     bool validax[8];
    
   
     
};

}

#endif
