#include<three_view.h>
#include<slice.h>
#include<dataset.h>
#include<vector>
#include<qfontmetrics.h>
#ifndef THREE_FACE_H
#define THREE_FACE_H 1
class three_face: public three_view
{

public:
  three_face(){}
  three_face(QString view,QFontMetrics *f_m,pick_draw *_pks,mode *mm,draw_other *dr);
  void viewit(QPainter *pm,QPen *pen, slice *fact,dataset *dat,orient_cube *pos,
  int bx, int ex, int by, int ey,draw_what *drawit,bool overlay);

  ~three_face(){  }
  
   private:



};
#endif
