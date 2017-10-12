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
namespace SEP{
class single: public view
{

public:
  single(){zero_bufs();}
  single(QString view,int iv1, int iv2,std::shared_ptr<QFontMetrics >f_m,std::shared_ptr<SEP::pick_draw>_pks, std::shared_ptr<SEP::draw_other>dr,std::vector<float >proportions);
  void viewit(QPainter *pm,QPen *pen, std::shared_ptr<slice>fact,std::shared_ptr<SEP::dataset>dat,std::shared_ptr<SEP::orient_cube>pos,
  int bx, int ex, int by, int ey,std::shared_ptr<SEP::draw_what>draws, bool overlay);
  

  
 std::vector<QString> keyPressEvent( QKeyEvent *e , std::shared_ptr<SEP::orient_cube>pos);
  std::vector<QString> keyReleaseEvent( QKeyEvent *e, std::shared_ptr<SEP::orient_cube>pos );
   ~single(){
     delete_slices();
   }
  
   
  private:
  
    void zero_bufs();
    int i_v1,i_v2;
    int bound,bsmall,baxes;


};

}
#endif
