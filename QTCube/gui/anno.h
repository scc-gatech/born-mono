#ifndef ANNO_NEW_H
#define ANNO_NEW_H 1
#include<QString>
#include <memory>
#include "my_fonts.h"
#include "my_colors.h"
#include "assert.h"
namespace SEP{
class anno{
  public:
    anno(){;}
    virtual float return_dist(float px, float py){
       assert(1==2);
       if(px==py){;};
    }
    virtual ~anno(){;}
    virtual void shift_anno(float bx, float by, float ex, float ey, float dx, float dy){ pctx=bx+dx; pcty=by+dy; endx=ex+dx; endy=ey+dy;}
    virtual void draw(QPainter *painter, std::shared_ptr<my_fonts> fonts, std::shared_ptr<my_colors> col, int bx, int ex, int by, int ey){
      if(bx==ex && by==ey && painter==0 && fonts==0  && col==0){;};}
    float pctx,pcty;
        float endx,endy;

    QString col;
    int thick;
};
class anno_text: public anno{
  public:
    anno_text(){;}
    anno_text(float px,float py,QString c,QString t,QString f,int th){ txt=t; font=f; pctx=px; pcty=py; col=c;thick=th;}
    virtual float return_dist(float px, float py);
    virtual void draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>col, int bx, int ex, int by, int ey);
    virtual ~anno_text(){;}
    QString txt;
    QString font;

};
class anno_line: public anno{
  public:
    anno_line(){;}
    anno_line(float px,float py,QString c,int t,float ex, float ey){ thick=t;pctx=px; pcty=py; col=c; endx=ex; endy=ey;}
    virtual float return_dist(float px, float py);
    virtual void draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>col, int bx, int ex, int by, int ey);
    virtual ~anno_line(){;}
};
class anno_arrow: public anno{
  public:
    anno_arrow(){;}
    anno_arrow(float px,float py,QString c,int t,float ex,float ey){ thick=t;pctx=px; pcty=py; col=c; endx=ex; endy=ey;}
    virtual float return_dist(float px, float py);
    virtual void draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>col,int bx, int ex, int by, int ey);
    virtual ~anno_arrow(){;}
};
class anno_ellipse: public anno{
  public:
    anno_ellipse(){;}
    anno_ellipse(float px,float py,QString c, int t,float ex,float ey){ thick=t;pctx=px; pcty=py; col=c; endx=ex; endy=ey;}
    virtual void shift_anno(float bx, float by, float ex, float ey, float dx, float dy){ 
         if(ex==ey) {;};pctx=bx+dx; pcty=by+dy;}
    virtual float return_dist(float px, float py);
    virtual void draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>col,int bx, int ex, int by, int ey);
    virtual ~anno_ellipse(){;}
};
class anno_box: public anno{
  public:
    anno_box(){;}
    anno_box(float px,float py,QString c,int t,float ex,float ey){ thick=t;pctx=px; pcty=py; col=c; endx=ex; endy=ey;}
    virtual float return_dist(float px, float py);
    virtual void draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>col,int bx, int ex, int by, int ey);

};
class annotate{
  public:
    annotate();
    QString return_cur_shape(){ return cur_shape;}
    QString return_cur_color(){ return cur_col;}
    QString return_cur_font(){ return cur_font;}
    QString return_cur_text(){ return cur_text;}
    QString return_cur_thick(){ return QString::number(cur_thick);}
    void set_cur_color(QString col){ cur_col=col;}
    void set_cur_font(QString col){ cur_font=col;}
    void set_cur_text(QString col){ cur_text=col;}
    void set_cur_shape(QString col){ cur_shape=col;}

    void set_cur_thick(QString col){ cur_thick=col.toInt(); fprintf(stderr,"setting thickness %d %s \n",cur_thick,col.toAscii().constData());}
    void delete_nearest(float px, float py);
    void move_nearest(float px, float py);
    int find_nearest(float px, float py);
    void add_annotate(float px, float py, float ox, float oy);
    void update_annotate(float px, float py, float ox, float oy);
        void shift_annotate(float px, float py, float ox, float oy);
        void shift_finish_annotate(float px, float py, float ox, float oy);

    void finish_annotate(float px, float py, float ox, float oy);

    //void draw_progress(int bx, int ex, int by, int ey);
    void draw(QPainter *painter,int bx, int by, int ex, int ey);
    void clear();
    



  private:
    std::vector<std::shared_ptr<SEP::anno>> annos;
    QString cur_col,cur_font,cur_text,cur_shape;
    int cur_thick;
    bool in_progress;
    std::shared_ptr<SEP::my_fonts> fonts;
    std::shared_ptr<SEP::my_colors> colors;
    int active_anno;
    float bxh,exh,byh,eyh;

};
}
#endif
