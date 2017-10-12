#include<multiface.h>
#include<ctime>
#include "util.h"

using namespace SEP;

multi_face::multi_face(QString view,std::shared_ptr<QFontMetrics >f_m,std::shared_ptr<pick_draw> _pks,std::shared_ptr<draw_other> dr,
int g1, int g2, int j ){ 
 ;fm=f_m;
  grid1=g1;
  grid2=g2;
  j3=j;
   set_basics(_pks,dr);
   iview=view;
   corner_sel=0;
   com.push_back(iview); com.push_back("");com.push_back("");com.push_back("");
com.push_back("");
} 

void multi_face::viewit(QPainter *painter,QPen *pen, std::shared_ptr<slice>fact, 
  std::shared_ptr<dataset>dat,std::shared_ptr<orient_cube> pos,int b_x, int e_x, int b_y, int e_y,std::shared_ptr<draw_what>draws,
  bool overlay){
  
    this->set_size(b_x,e_x,b_y,e_y);
    trspace=true;
     cur_dat=dat;
    baxes=25;
     bound_l=bound_r=bound_b=bound_t=fm->height()/2;
     if(draws->draw_title) set_title_pos(painter,dat);
      if(draws->draw_bar) bound_b+=(fm->height())*5;
      
        if(draws->draw_title && title_bot) bound_b+=(int)(fm->height()*1.5);
    bsmall=1;
      

      int i3_cur=pos->get_loc(pos->get_order(2));
      axis a3=pos->getAxis(pos->get_order(2));
      int i3_cent=(int)((grid1*grid2)/2.);
      int fdraw=-i3_cent*j3;
      int deltax=((ex-bound_r)-(bx+bound_l))/grid1;
      int deltay=((ey-bound_b)-(by+bound_t))/grid2;
      
    
      for(int ig2=0; ig2< grid2; ig2++){
        for(int ig1=0; ig1< grid1; ig1++){
           if(fdraw+i3_cur>0 && fdraw+i3_cur<a3.n){
            int draw_p=0;
             if(fdraw==0) draw_p=draws->draw_pos;
             slices.push_back(fact->clone());
             slices[slices.size()-1]->set_draw_params(fm,pos,0,1,fdraw,
               bound_l+bx+ig1*deltax,bound_l+bx+(ig1+1)*deltax,
               bound_t+by+ig2*deltay,bound_t+by+(ig2+1)*deltay,
               draw_p, false,draws->draw_axis1,draws->draw_axis2,false,0.,0.,false,false);
        }
            fdraw+=j3;
        }}
             

 
  render_it(painter,pen, fact, dat, pos,  overlay, draws);
  //draw_line line=draw_line(cx,cy,cx+4,cy-4);
//  line.draw(painter);
}
