#ifndef view_panel_H
#define view_panel_H 1
#include <QVBoxLayout>
#include "qcubewidgetbasics.h"
#include "widget_tools.h"
#include <QTabWidget>

#include <vector>
namespace SEP{
class view_panel: public qcubeWidgetBasic{
  Q_OBJECT
  public: 
    view_panel(std::shared_ptr<position> p,std::shared_ptr<windows>my_w, 
    std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk, 
    std::shared_ptr<slice_types>c,std::shared_ptr<maps> mym);
  
     void update_menu(std::vector<QString>);
     virtual void actionRespond(std::vector<QString>){;}
     ~view_panel(){ delete_row_1();delete_row_2(); delete_row_3(); delete layMain;}
      
      signals:
       void actionDetected(std::vector<QString> text);
 private:
     void delete_row_1();
     void delete_row_2();
     void delete_row_3();

     QWidget  *wiggle_tab();
     void delete_wiggle_tab();

     QWidget *map1_tab();
     void delete_map1_tab();

     QWidget *grid_tab();
     void delete_grid_tab();
     QWidget *contour_tab();
     void delete_contour_tab();
     QWidget *build_row_1();
     QWidget * build_row_2();
     QTabWidget * build_row_3();

    basicButtonBox *cycleData,*cycleOver;
    basicComboBox *data,*perspective,*mainType,*overType,*over;
    basicSliderBox *opacity;
    basicComboBox *lineColor,*posFill,*negFill;
    basicComboBox *wiggleColor,*numLines,*contourColor;
    basicLineEditBox *firstContour,*sampleContour,*sampleGrid;
    basicComboBox *grid1,*grid2;
    basicButtonBox  *loadMap1;
    basicLineEditBox *overSamp;
    basicComboBox *usingMap1;
    
    private slots:
      void  display_change(const QString str);
      void  odisplay_change(const QString str);
      void  data_change(const QString str);
      void  odata_change(const QString str);
      void  perspective_change(const QString str);
            void  wiggle_color_change(const QString str);
            void  wiggle_pos_change(const QString str);
            void  wiggle_neg_change(const QString str);
  void change_map1(const QString str);
  void load_map();
  void contour_color_change(const QString str);
  void num_lines_change(const QString str);
  void first_contour_change(const QString str);
  void sample_contour_change(const QString str);
  void change_grid1(const QString str);
  void change_grid2(const QString str);
  void change_jplane(const QString str);

  

      void update_opactiy(int);
      void  roundRobinForward();
      void  roundRobinBack();
      void  oRoundRobinForward();
      void oRoundRobinBack();



    private:
    QVBoxLayout *layMain;
     QHBoxLayout *lay1,*lay2,*layContour,*layGrid,*layWiggle,*layMap1;
     QWidget *row1,*row2;
     QWidget *contour,*grid,*wiggle,*map1;
     QTabWidget *row3;

     bool validax[8];

     
};
}


#endif
