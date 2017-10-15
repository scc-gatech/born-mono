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
#include<param_func.h>
#include "view.h"
#include "slice_types.h"
#include "mode.h"
#include "datasets.h"
#include <map>
#include <vector>
#include "my_colors.h"
#include "pick_draw.h"


// class definition
class DrawWindow : public QWidget
{
	Q_OBJECT

public:
	DrawWindow(int inum,param_func *pars,position *pos,mode *mmm,
	  pick_draw *_pks,datasets *ds,int idat,
   slice_types *ct, mouse_func *f,orientation_server *s);

	void viewit(QString update);
	view *view_by_name(QString name);
	QString iter_views(int it);
	void set_colortable(QString newcol);
	void keyPressEvent( QKeyEvent *e );
	void keyReleaseEvent( QKeyEvent *e );
	void set_background_color(QString newc);
	void set_pen_color(QString newc);
	void update_display();
	void redo_view(){update_view(view_name);}
	void update_view(QString name);	
	void jpeg_save(QString name);
	void ps_save(QString sz,QString name);
	void sync(position *p);
	void set_data(int id);
	int  get_idat(){return idat;}
	int add_clip_pt(QString str);
	void del_clip_pt(QString str);
	void set_o_data(int id);
	void set_mouse_func(mouse_func *f){
	 func=f;
	}
  void iter_data(int inc);
    void iter_o_data(int inc);

  void build_overlay_fact();
  bool get_overlay(){return overlay;}
  void set_alpha(int a);
  int get_alpha(){return alpha;} 
  void set_overlay(bool o){ overlay=o;}
  int get_idat_o(){return idat_o;}
  QString get_ctable(){return wind_ctable;}
  QString get_background(){return wind_bg;}
  QString get_pen_c(){return wind_ov;}
  QString get_view_name(){return view_name;}
  QString get_over_c(){return over_c;}
  QString get_font_name(){return font_name;}
  void update_font(QString font);
  void get_view_pcts(float *pt, float *pf);
  void set_view_pcts(float pt, float pf);
  void bound_ps(QString file, int bx, int by, int ex, int ey);
 
	int get_order(int i){return this->pos->get_order(i);}
  void set_overlay_color(QString o);
	void delete_overlay();
	void resetValues();
	void set_windows(int nw, std::vector<DrawWindow *>wid);
    orient_cube *pos;
  void initial_view();
  void resize_view(int ix,int iy);
    void resize_view_beg(int ix,int iy);
  void resize_view_end(int ix,int iy);

	view *myv,*myv_o;
	  bool get_draw_bar(){return drawit->draw_bar;}
	  	  bool get_draw_region(){return drawit->draw_region;}
  void set_draw_bar(bool b);
    void set_draw_region(bool b);
   int get_pos_transp(){ return drawit->draw_pos;}
 void set_pos_transp(int db);

  
     bool get_draw_picks(){return drawit->draw_picks;}
  void set_draw_picks(bool b);
     bool get_draw_title(){return drawit->draw_title;}
  void set_draw_title(bool b);
     bool get_draw_grid(){return drawit->draw_grid;}
  void set_draw_grid(bool b);


  QString get_clip_pt(int ipt);

     dataset *return_dat(){return dat;}
slice_types *colort;
signals:
	void actionDetected(std::vector< QString> text);

	



protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent( QResizeEvent * );

private slots:
	
private:



    QString get_string(char *par, QString def);
    int get_int(char *par, int def);
    int *get_ints(char *par, int nmax, int *def);
	QImage *image;
	QPixmap	pm;
    QPixmap	pmScaled,pmScaledCopy;
	bool reconvertImage();
	void scale();
	void set_windows(int n, DrawWindow *wind);

	int xpos;
	int ypos;
	int x_t, y_t;
	int interval,idat_o;



	int moviemode;
	int moviespeed;


	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
  void mouseDoubleClickEvent ( QMouseEvent * event );


   


    void draw_base_pixmap();


	int time;
	double last;
	bool first;

    QFontMetrics *fm;
	QTime TIME;
	int iview;
	int W,H;
	param_func *pars;
	int border;
	int down_x,down_y;
	int mouse_down;
	dataset *dat,*dat_o;
	int idat;
	bool overlay;
	datasets  *datas;
	pick_draw *pk;
	slice *fact,*fact_o;

	std::vector<DrawWindow *>windows;
	QColor back_c,pen_c;
	QString view_name;
	QString iview_s;
	QString over_c;
	int nwindows;
	mode *mym;
	int alpha;
	QString wind_ctable,wind_ov,wind_bg;
	QString font_name;
	QFont *myf;
	draw_other *draw_o;
	std::map<QString,QColor>  color_map;
	std::vector<QString> com;
	std::vector<QString> allv;
    draw_what *drawit,*drawo;
    mouse_func *func;
    QTime timeit;
    my_colors *my_cmap;
  




};


#endif
