/******************************************************************
 * DrawWindow: window for drawing seismic data to screen
 ******************************************************************/

#ifndef PANEL_H
#define PANEL_H
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
#include "maps.h"
#include "datasets.h"
#include <map>
#include <vector>
#include "my_colors.h"
#include "pick_draw.h"
#include "my_fonts.h"
#include "anno.h"
#include<QWidget>
namespace SEP{
class panel: public QWidget{
    Q_OBJECT

  public:
     panel(int inum, std::shared_ptr<SEP::paramObj>pars, std::shared_ptr<position> pos, 
        std::shared_ptr<pick_draw>_pks, std::shared_ptr<datasets>ds, int idat, 
        std::shared_ptr<slice_types>ct, std::shared_ptr<mouse_func>f,
        std::shared_ptr<orientation_server>s, std::shared_ptr<orients>oo,std::shared_ptr<maps> mp);
     std::shared_ptr<panel>clone(int inum);
  signals:
        void actionDetected(std::vector< QString> text);
    
  public:
     /*Transparency*/
     void set_alpha(int a);
     int get_alpha(){return alpha;} 
     void change_map(int imap);
     //Three-way commands
     void moving_corner(int ix,int iy);
     void move_corner_beg(int ix,int iy);
     void move_corner_end(int ix,int iy);
     void get_view_pcts(float *pt, float *pf);
     void set_view_pcts(float pt, float pf);
     void set_proportions(float *props){for(int i=0; i < 8; i++) proportions[i]=props[i];}
     int get_orient_inst(){return pos->get_orient_inst();}

     int get_orient_num(){return pos->get_orient_num();}
     //Overlay functions
     void set_overlay(bool o){ overlay=o;}
     int get_idat_o(){return idat_o;}
     QString get_over_c(){return over_c;}
     void set_o_data(int id);
     void iter_o_data(int inc);
     void build_overlay_fact();
     bool get_overlay(){return overlay;}
     void set_overlay_color(QString o);
     void set_no_map(){
        pos->set_no_shift();
     }
	 void delete_overlay();
     
     //Fonts 
     QString get_font_name(){return font_name;}
     void update_font(QString font);

     
     //Mouse
     void set_mouse_func(std::shared_ptr<mouse_func> f){func=f;} //Set the mouse function for the panel
    
    //Color
    void set_colortable(QString newcol);
	void set_background_color(QString newc);
	void set_pen_color(QString newc);
	std::shared_ptr<slice_types>get_colortables(){ return colort;}
	QString get_ctable(){return wind_ctable;}
    QString get_background(){return wind_bg;}
    QString get_pen_c(){return wind_ov;}
     
     //Views
     std::shared_ptr<view> view_by_name(const std::string name){
        return view_by_name(QString(name.c_str()));
     }
     std::shared_ptr<view> view_by_name(const QString nm);
	 QString iter_views(int it);
	 QString get_view_name(){return view_name;}
	 void initial_view();
	 void viewit(QString update);
	 void update_view(QString name);
	 void redo_view(){update_view(view_name);}
	 void update_display();
	 int get_iview(){return iview;}
	 void pdf_save(QPainter *painter);
     void draw_base_pixmap(QPainter *painter);
     
     //View parameter functions
     bool get_draw_bar(){return drawit->draw_bar;}
     void set_draw_bar(bool b);
     void set_draw_region(bool b);
     int get_pos_transp(){ return drawit->draw_pos;}
     void set_pos_transp(int db);
     bool get_draw_picks(){return drawit->draw_picks;}
     void set_draw_picks(bool b);
     bool get_draw_title(){return drawit->draw_title;}
     bool get_draw_axis1(){return drawit->draw_axis1;}
     void set_draw_axis1(bool b);
          bool get_draw_axis2(){return drawit->draw_axis2;}
     void set_draw_axis2(bool b);
          bool get_draw_axis3(){return drawit->draw_axis3;}
     void set_draw_axis3(bool b);
     void set_draw_title(bool b);
     bool get_draw_grid(){return drawit->draw_grid;}
     void set_draw_grid(bool b);

     //Keypress events
     void keyPressEvent( QKeyEvent *e );
     void keyReleaseEvent( QKeyEvent *e );
     
     //Orient functions
     std::shared_ptr<orient_cube>return_orient(){return pos;}
     void sync_pos(std::shared_ptr<orient_cube>p){ pos->sync_pos(p);}

     //Mouse events
     void mousePressEvent(QMouseEvent *);
	 void mouseMoveEvent(QMouseEvent *);
	 void mouseReleaseEvent(QMouseEvent *);
     void mouseDoubleClickEvent ( QMouseEvent * event );

     //Orientation access functions
     void get_true_position(float *p){pos->get_true_pos(p,oversamp);}
     void return_grid_loc(float *p, int *iloc){ pos->return_grid_loc(p,iloc,oversamp);}
	 int get_order(int i){return this->pos->get_order(i);}
     
     //Panel size functions
     void get_panel_dimensions(int *bx, int *by, int *ex, int *ey){*bx=begx;*by=begy;*ex=endx;*ey=endy;}
     void set_panel_dimensions(int bx,int by,int ex, int ey){begx=bx;begy=by;endx=ex;endy=ey;}
            bool draw_it(){
         if(endx-begx<30 || endy-begy < 30) return false;
         return true;;
        }

     //Orient 
     std::shared_ptr<orient_cube> get_orient(){ return pos;}
     
     //Dataset/clips
     int add_clip_pt(QString str);
  	 void del_clip_pt(QString str);
     QString get_clip_pt(int ipt);
     std::shared_ptr<dataset>return_dat(){return dat;}
     void set_data(int id);
	 int  get_idat(){ return idat;}
     void iter_data(int inc);
      
     //Grid for multi-plot
     void set_jplane(int j){ jplane=j; update_view(view_name);}
     void set_grid1(int g){
       grid1=g; 
       update_view(view_name);
     }
     void set_grid2(int g){
       grid2=g;
       update_view(view_name);
       }
     int get_jplane(){ return jplane;}
     int get_grid1(){ return grid1;}
     int get_grid2(){ return grid2;}
     
     void delete_panel();
     ~panel(){
       delete_panel();
     }
     bool get_update_it(){return update_it;}
     void set_update_it(bool x){update_it=x;}
     std::shared_ptr<view> get_view(){return myv;}
protected:


private slots:
	
private:

 
   
   //Private variables
   bool update_it;
   QPainter *paint;

   std::shared_ptr<mouse_func>func; //what the mouse does
   std::shared_ptr<annotate>ano;    //Annotations on this window
   std::shared_ptr<dataset>dat,dat_o; //Current dataset as primary and overlaid
   int idat; //The dataset number currently being display
   bool overlay;  //Whether or not we are overlaying a dataset
   std::shared_ptr<datasets  >datas;  //The collections of datasets that exist
   std::shared_ptr<draw_other >draw_o;  //What do draw in a given view
   std::shared_ptr<orient_cube >pos;  //Returns info on what to display
   std::shared_ptr<SEP::paramObj>pars;  //Grab parameters from the command line
   std::vector<float> proportions; //The ratios of the various axes we will use 
   int jplane,grid1,grid2; //When displaying multiple slices 
   bool first; //First time through
   std::shared_ptr<slice>fact,fact_o;  //Current way to display a slice
   std::shared_ptr<pick_draw>pk;  //The picks
   std::shared_ptr<orientation_server>serv;
   QString view_name;  //Current view being display
   QString iview_s;  //View number in qstring
   QString over_c;  //Over c
   int alpha; //The amount of transparancy
   std::shared_ptr<draw_what >drawit,drawo;
   std::shared_ptr<my_colors >my_cmap;// Color generator
   std::shared_ptr<my_fonts>fonts;//Font Generator
   std::shared_ptr<QFont > myf; //Current font
   std::shared_ptr<QFontMetrics> fm; //Current font metric
   int *map; //Map used for distortions
   std::vector<QString> com; //Structure to send message
   std::map<QString,QColor>  color_map;  //Color map table
   std::vector<QString> allv;  //List of possible views
   QColor pen_c;  //Background and pen color
   std::shared_ptr<view>myv,myv_o;  //Main and overlay view
   int begx,begy,endx,endy; //Range to display a given panel
   std::shared_ptr<slice_types>colort;  //Different ways to view slices
   int idat_o; //Data to overlay
   QString wind_ctable,wind_ov,wind_bg;  //color info
   QString font_name; //FOnt name
   int iview;//View number
   int down_x,down_y,mouse_down; //Mouse status
   float oversamp;//Oversamp number
   QTime timeit; //Time variable for redraw
   std::shared_ptr<maps>my_maps;
   std::shared_ptr<orients> my_or;


};
}
#endif
