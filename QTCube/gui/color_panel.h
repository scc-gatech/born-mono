 #pragma once 
#ifndef COLOR_BAR
#define COLOR_BAR_H 1
#include "qcubewidgetbasics.h"
#include "widget_tools.h"
#include<memory>
#include <vector>


namespace SEP{
class color_bar2: public QWidget{
	Q_OBJECT
  signals:
    void emitit(std::vector<QString> com);
  public:
    color_bar2(  std::shared_ptr<SEP::user_color>col);
    
    void extract_draw(QString color, QPainter *painter);
    void draw_bar();
    void mouseReleaseEvent(QMouseEvent *);
    void create_map();
    void scale();
    void redo_line();
    QString create_maps();
    void paintEvent( QPaintEvent *e );
    void resizeEvent( QResizeEvent * );
    void set_pts(QString pts) ;
    QString find_col_panel(int ih);
    void to_bar_pts(QString com);
    void to_pct(QString col,int ix, int iy, float *px, float *py);
    void to_pt(QString col,float px, float py, int *ix, int *iy);
    void to_pct_small(int ix, int iy, float *px, float *py);
    void to_pt_small(float px, float py, int *ix, int *iy);
    void add_pt(int ix, int iy);
    void add_pt_pct(float px, float py);
    void set_pcts(float *pct);
    void clear_picks();
    void del_pt(int ix, int iy);
    void del_pt_pct(float px, float py);
    int find_pt(int ix, int iy);
    void set_sizes();
    void set_source(QString file);
    void load_cbar(QString file);
    void save_cbar(QString file);
    int *return_map(QString col);
    QPolygon *boxit(float bx,float by, float ex, float ey);
    signals:
      void actionDetected(std::vector <QString>);
    public:
    int ipt_down,nh_bar,fh_bar;
    std::shared_ptr<user_color>ctable;
    std::map<QString,float*> maps;  
    std::map<QString,int> bh,nh;
    QString color;
	  int W,H;
		QPixmap	pm;
		bool symmetric;
    QPixmap	pmScaled,pmScaledCopy;
};

class color_panel: public qcubeWidgetBasic{
  Q_OBJECT
  public: 
    color_panel(std::shared_ptr<position>p,
    std::shared_ptr<windows>my_w, 
    std::shared_ptr<panels>my_p, 
    std::shared_ptr<datasets>my_d,
     std::shared_ptr<pick_draw>pk, 
    std::shared_ptr<slice_types>c,std::shared_ptr<maps>mym);
       void update_menu(std::vector<QString>);
     ~color_panel(){;}
   
        signals:
       void actionDetected(std::vector<QString> text);
  private slots:
    void actionRespond(std::vector< QString> text);
   private:
     QVBoxLayout *mainLay;
     QHBoxLayout *barLay;
     basicButtonBox *loadsave;
     basicComboBox *color;
     QGroupBox *colorBox;
     std::shared_ptr<color_bar2> cbar;


 
};
}
#endif