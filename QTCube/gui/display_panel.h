#ifndef display_panel_H
#define display_panel_H 1
#include <QVBoxLayout>
#include "qcubewidgetbasics.h"
#include "widget_tools.h"

#include <vector>
namespace SEP{
class display_panel: public qcubeWidgetBasic{
  Q_OBJECT
  public: 
    display_panel(std::shared_ptr<SEP::position>p,std::shared_ptr<SEP::windows>my_w, 
    std::shared_ptr<SEP::panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<SEP::pick_draw>pk, 
    std::shared_ptr<SEP::slice_types>c,std::shared_ptr<SEP::maps> mym);

     virtual void actionRespond(std::vector<QString>){;}
     ~display_panel(){ delete_row_1();delete_row_2(); delete_row_3(); delete layMain;}
     void update_menu(std::vector<QString> coms);
        signals:
       void actionDetected(std::vector<QString> text);
 private:
     void delete_row_1();
     void delete_row_2();
     void delete_row_3();
     QWidget *build_row_1();
     QWidget * build_row_2();
     QWidget * build_row_3();
     
     QVBoxLayout *layMain;
     QHBoxLayout *lay1,*lay2,*lay3;
     QWidget *row1,*row2,*row3;
     basicComboBox *fontBox,*colorBox;
     basicRadioBox *colorbar,*title,*axes1,*axes2,*axes3,*grid,*picks;
     basicSliderBox *posSlide;
  private slots:
      void change_font(const QString font);
      void change_color(const QString col);
      void yes_colorbar();
      void no_colorbar();
      void yes_title();
      void no_title();
      void draw_pos(int alpha);
      void yes_axis1();
      void no_axis1();
      void yes_axis2();
      void no_axis2();
      void yes_axis3();
      void no_axis3();
      void yes_picks();
      void no_picks();
      void yes_grid();
      void no_grid();

};

}

#endif
