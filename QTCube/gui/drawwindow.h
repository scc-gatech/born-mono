/******************************************************************
 * DrawWindow: window for drawing seismic data to screen
 ******************************************************************/

#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H
#include <qpainter.h>
#include <qwidget.h>
#include <qimage.h>
#include <qmatrix.h>
#include <qfiledialog.h>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QPixmap>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include<orient_cube.h>
#include <qtimer.h>
#include "slice.h"
#include "view.h"
#include "slice_types.h"
#include "mode.h"
#include "datasets.h"
#include <map>
#include <vector>
#include "my_colors.h"
#include "pick_draw.h"
#include "my_fonts.h"
#include "anno.h"
#include "panel.h"

namespace SEP{


class DrawWindow : public QWidget
{
	Q_OBJECT

    public:
      DrawWindow(int ngrid1, int ngrid, std::vector<std::shared_ptr<SEP::panel>> panels,int in);
      
      void pdf_save(QString sz,QString name);
      void scale();
      void draw_base_pixmap();
    
      /*Annotation functions*/
      void del_anno(int ix,int iy);
      void set_ano_color(QString text){ ano->set_cur_color(text);}
      void set_ano_shape(QString text){ ano->set_cur_shape(text);}
      void set_ano_text(QString text){ ano->set_cur_text(text);}
      void set_ano_font(QString text){ ano->set_cur_font(text);}
      void set_ano_thick(QString text){ ano->set_cur_thick(text);}
      void add_anno(int bx, int by, int ex, int ey);
      void update_anno(int bx, int by, int ex, int ey);
      void finish_anno(int bx, int by, int ex, int ey);
      void move_anno(int bx, int by, int ex, int ey);
      void shift_anno(int bx, int by, int ex, int ey);
      void shift_finish_anno(int bx, int by, int ex, int ey);
      
      void set_background_color(QString x);
      /*Panel functions*/
      void update_panel_map();
      void set_boundary_thick(int thick){
         bound_thick=thick;
        }
      int return_boundary_thick(){return bound_thick;}
      int get_panel(int ix,int iy);
      void set_active_panel(int ipanel);
    
         void keyPressEvent( QKeyEvent *e );
     void keyReleaseEvent( QKeyEvent *e );
    
         //Mouse events
     void mousePressEvent(QMouseEvent *);
	 void mouseMoveEvent(QMouseEvent *);
	 void mouseReleaseEvent(QMouseEvent *);
     void mouseDoubleClickEvent ( QMouseEvent * event );
     void update_display(bool force=false);   
     
     
     //Grid
     void set_grid1(int n);
     void set_grid2(int n);
     int return_grid1(){return ngrid1;}
     int return_grid2(){return ngrid2;}
     void add_panel(std::shared_ptr<SEP::panel>p);
     int get_inum(){return inum;}
 
     std::shared_ptr<SEP::panel>return_panel(int i){ return panels[i];}
     std::shared_ptr<SEP::panel>remove_last_panel(){
        std::shared_ptr<SEP::panel>x=panels[panels.size()-1];
        panels.erase(panels.begin()+panels.size()-1);
        return x;
     }
    
    signals:
        void actionDetected(std::vector< QString> text);
        
    protected:
	  void paintEvent(QPaintEvent *);
	  void resizeEvent( QResizeEvent * );
	  
    private:
      void change_bound(int ix,int iy);
      
      int *beg1,*beg2,*end1,*end2;  //Table with the beg/ends of panels
      int ngrid1,ngrid2;  //Number of panels 
      int bound_thick;  //Thickness of boundary
      float *port1,*port2;  //Portions along each axis
      std::vector<std::shared_ptr<SEP::panel>> panels;  //Array of panels
      annotate *ano;  //Annotation
      QImage *image;  //Image
	  QPixmap	pm;
      QPixmap	pmScaled,pmScaledCopy;

	  QColor wind_bg;
      int W,H; //Width and Height
      int active_panel;  //What panel is currently active (needed for key press)
      int mouse_panel; //What panel we were on when the mouse was pressed down
      int mouse_boundx,mouse_boundy; //What boundary the mouse has grabbed
      QTime timeit; //Time variable for redraw
      QColor back_c; //background color
      int inum;
 };
}
#endif
