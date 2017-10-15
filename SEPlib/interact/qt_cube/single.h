#include<slice.h>
#include<view.h>
#include<slice.h>
#include<dataset.h>
#include<qfontmetrics.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>
#ifndef SINGLE1_H
#define SINGLE1_H 1
class single: public view
{

public:
  single(){zero_bufs();}
  single(QString view,int iv1, int iv2,QFontMetrics *f_m,pick_draw *_pks, mode *mm,draw_other *dr);
  void viewit(QPainter *pm,QPen *pen, slice *fact,dataset *dat,orient_cube *pos,
  int bx, int ex, int by, int ey,draw_what *draws, bool overlay);
  

  
 std::vector<QString> keyPressEvent( QKeyEvent *e , orient_cube *pos);
  std::vector<QString> keyReleaseEvent( QKeyEvent *e, orient_cube *pos );
   ~single(){
     delete_slices();
   }
  
   
  private:
  
    void zero_bufs();
    int i_v1,i_v2;
    /* int bound,bsmall,baxes */;





};
#endif
