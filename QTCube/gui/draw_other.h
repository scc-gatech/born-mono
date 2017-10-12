#ifndef DRAW_OTHER 
#define DRAW_OTHER 1
#include <QPen>
#include <QPainter>
namespace SEP{
class draw_object{
  public:
  draw_object(){};
  void set_basic(){pen=0;}
  void set_pen(QPen *pn){ pen=pn;}
  virtual void test(){
  }
  virtual void draw(QPainter *painter){
    if(painter==0){;};}
  virtual ~draw_object(){};
  QPen *pen;
};
class draw_box: public draw_object{
public:
  draw_box(){set_basic();}
  draw_box(int bx, int by, int ex, int ey);
    virtual void test(){
  }
  virtual void draw(QPainter *painter){

    if(pen!=0) painter->setPen(*pen);

    painter->drawPolyline(poly);
  }
  void add_pt(int ix,int iy){ poly.append(QPoint(ix,iy));}
  QPolygon poly;
};
class draw_line: public draw_object{
public:
  draw_line(){set_basic();}
  draw_line(int bx, int by, int ex, int ey);
    virtual void test(){
  }
  virtual void draw(QPainter *painter){

    if(pen!=0) painter->setPen(*pen);

    painter->drawPolyline(poly);
  }
  void add_pt(int ix,int iy){ poly.append(QPoint(ix,iy));}
  QPolygon poly;
};
class draw_hyperbola: public draw_object{
public:
  draw_hyperbola(){set_basic();}
  draw_hyperbola(int bx, int by, int ex, int ey);
    virtual void test(){
  }
  virtual void draw(QPainter *painter){

    if(pen!=0) painter->setPen(*pen);

    painter->drawPolyline(poly);
  }
  void add_pt(int ix,int iy){ poly.append(QPoint(ix,iy));}
  QPolygon poly;
};

class draw_pts: public draw_object{
public:
  draw_pts(){sz=2; set_basic();}
  void draw(QPainter *painter);
  void add_pt(int ix,int iy){ poly.append(QPoint(ix,iy));}
  QPolygon poly;
  int sz;
};

class draw_other{
  public:
    draw_other(){
      objs.clear();
    }
    ~draw_other(){
      for(int i=0; i<(int)objs.size(); i++) delete objs[i];
      objs.clear();
    }
    void draw(QPainter *painter){
      for(int i=0; i < (int)objs.size(); i++) {
        objs[i]->draw(painter);
       }
    }
   void reset(){ objs.clear();}
  QPen *pen;
  std::vector<SEP::draw_object*> objs;
};
}
#endif

