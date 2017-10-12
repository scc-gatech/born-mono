#include<single.h>
#include "util.h"
#include<threeface.h>
#include<ctime>
#include <QPolygon>
#include <QMouseEvent>
#include <QKeyEvent>
using namespace SEP;

single::single(QString view,int iv1, int iv2, std::shared_ptr<QFontMetrics >f_m,std::shared_ptr<pick_draw>_pks, std::shared_ptr<draw_other>dr,std::vector<float >prop){ 
  fm=f_m; i_v1=iv1; i_v2=iv2; this->zero_bufs();

  iview=view;
  set_basics(_pks,dr);
    com.push_back(iview); com.push_back("");com.push_back("");com.push_back("");
    com.push_back("");
    proportions=prop;
    

    
} 
void single::zero_bufs(){


}

std::vector<QString> single::keyReleaseEvent( QKeyEvent *e,std::shared_ptr<orient_cube>pos ){
  if(pos==0);
  com[1]="none";
  switch( e->key() )
  {
      return com;  
  }
  return com;
}
std::vector<QString> single::keyPressEvent( QKeyEvent *e,std::shared_ptr<orient_cube>pos ){
 if(e==0); if(pos==0);
 com[1]="none";
 return com;
}
/*
	The following five methods deal with the pressing, releasing, and moveing of the mouse and its buttons.
*/

void single::viewit(QPainter *painter,QPen *pen, std::shared_ptr<slice>fact, 
  std::shared_ptr<dataset>dat,std::shared_ptr<orient_cube>pos,int bx, int ex, int by, int ey,std::shared_ptr<draw_what>draws,
  bool overlay){
   if(proportions[0]>0.){
   
      float rat1=(float)(ex-bx)/((float)(ey-by));
      float rat2=(float)(proportions[pos->get_order(i_v2)])/proportions[pos->get_order(i_v1)];
      if(rat1<rat2){
         int yover=(ey-by)-(ex-bx)/rat2;
          by+=yover/2; ey-=yover/2;
      
      }
      else{
         int xover=(ex-bx)-(ey-by)*rat2;
         bx+=xover/2; ex-=xover/2;
      }

   }
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

     bool d1,d2;
     if(i_v1==1) d1=draws->draw_axis1;
     else if(i_v1==2) d1=draws->draw_axis2;
     else d1=draws->draw_axis3;
     if(i_v2==1) d2=draws->draw_axis1;
     else if(i_v2==2) d2=draws->draw_axis2;
     else d2=draws->draw_axis3;
    slices[0]->set_draw_params(fm,pos,i_v1,i_v2,0,b_x,e_x,b_y,e_y,draws->draw_pos,
     false,d1,d2,false,0.,0.,false,false);
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
