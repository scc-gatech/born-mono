#include<single.h>
#include "util.h"
#include<threeface.h>
#include<ctime>
#include <QPolygon>
#include <QMouseEvent>
#include <QKeyEvent>


single::single(QString view,int iv1, int iv2, QFontMetrics *f_m,pick_draw *_pks,mode *mm, draw_other *dr){ 
  fm=f_m; i_v1=iv1; i_v2=iv2; this->zero_bufs();
 mym=mm;
  iview=view;
  set_basics(_pks,dr);
    com.push_back(iview); com.push_back("");com.push_back("");com.push_back("");
    com.push_back("");
    
    

    
} 
void single::zero_bufs(){


}

std::vector<QString> single::keyReleaseEvent( QKeyEvent *e,orient_cube *pos ){
  if(pos==0){}
  com[1]="none";
  switch( e->key() )
  {
      return com;  
  }
  return com;
}
std::vector<QString> single::keyPressEvent( QKeyEvent *e,orient_cube *pos ){
 if(e==0){} if(pos==0){}
 com[1]="none";
 return com;
}
/*
	The following five methods deal with the pressing, releasing, and moveing of the mouse and its buttons.
*/

void single::viewit(QPainter *painter,QPen *pen, slice *fact, 
  dataset *dat,orient_cube *pos,int bx, int ex, int by, int ey,draw_what *draws,
  bool overlay){

    this->set_size(bx,ex,by,ey);
    cur_dat=dat;
     bound_l=bound_r=bound_b=bound_t=fm->height()/2;
          if(draws->draw_title) set_title_pos(painter,dat);

    if(draws->draw_bar) bound_b+=(int)(fm->height())*5;
    if(draws->draw_title) bound_b+=(int)(fm->height()*1.5);
    int b_x=bx+bound_l, e_x=ex-bound_r;
    int b_y=by+bound_t, e_y=ey-bound_b;
     pos->fix_min(1);

    this->delete_slices();

    slices.push_back(fact->clone());

    slices[0]->set_draw_params(fm,pos,i_v1,i_v2,b_x,e_x,b_y,e_y,draws->draw_pos,
     false,draws->draw_axes,draws->draw_axes,false,0.,0.,false,false);
   // slices[0]->draw_slice(painter,dat,fact,pen,pos,overlay,draws->draw_grid);
/*
    if(draws->draw_pos){
      if(boxes.size() >0){
        for(int i=0; i < (int)boxes.size();i++)
          painter->drawPolyline(boxes[i]);
        boxes.clear();
      }
     }

      if(cur_dat->pickable&&draws->draw_picks) this->pg->draw(painter,slices[0]);

      if(draws->draw_bar) draw_bar(4*bound_l,painter,pen,fact,dat->conv,draws->draw_title);
      if(draws->draw_title) draw_title(0,painter,pen,dat->get_name());
     */
       render_it(painter,pen, fact, dat, pos,  overlay, draws);

}
