#include<threeface.h>
#include<ctime>
#include "util.h"

using namespace SEP;

three_face::three_face(QString view,std::shared_ptr<QFontMetrics >f_m,std::shared_ptr<pick_draw>_pks,std::shared_ptr<draw_other>dr,std::vector<float> prop){ 
 ; pct_top=.4; pct_front=.6;fm=f_m;

   set_basics(_pks,dr);
   proportions=prop;
   iview=view;
   corner_sel=0;
   com.push_back(iview); com.push_back("");com.push_back("");com.push_back("");
com.push_back("");com.push_back("");

} 
void three_face::viewit(QPainter *painter,QPen *pen, std::shared_ptr<slice>fact,   std::shared_ptr<dataset>dat,std::shared_ptr<orient_cube>pos,int b_x, int e_x, int b_y, int e_y,std::shared_ptr<draw_what>draws,bool overlay){
  
    this->set_size(b_x,e_x,b_y,e_y);
    trspace=true;
     cur_dat=dat;
    baxes=25;
     bound_l=bound_r=bound_b=bound_t=fm->height()/2;
     if(draws->draw_title) set_title_pos(painter,dat);
      if(draws->draw_bar) bound_b+=(fm->height())*5;
      
        if(draws->draw_title && title_bot) bound_b+=(int)(fm->height()*1.5);
    bsmall=1;
        int cx,cy;

  if(proportions[0]<-.999) get_corner_loc(&cx,&cy);
  else{
    get_corner_loc(&cx,&cy,proportions[pos->get_order(0)],proportions[pos->get_order(1)],
      proportions[pos->get_order(2)]);
  }
    int front_x_b=bound_l+bx;
    int front_x_e=cx-bsmall;
    int front_y_b=cy+bsmall;
    int front_y_e=ey-bound_b;
    
  
    
    int side_x_b=cx+bsmall;
    int side_x_e=ex-bound_r;
    int side_y_b=cy+bsmall;
    int side_y_e=ey-bound_b;
    
    int top_x_b=bx+bound_t;
    int top_x_e=cx-bsmall;
    int top_y_b=by+bound_t;
    int top_y_e=cy-bsmall;


    this->delete_slices();
    pos->fix_min(1);

  for(int i=0; i < 3; i++) slices.push_back(fact->clone());
  slices[0]->set_draw_params(fm,pos,0,1,0,front_x_b,front_x_e,front_y_b,front_y_e,draws->draw_pos,
     false,draws->draw_axis1,draws->draw_axis2,false,0.,0.,false,false);


  slices[1]->set_draw_params(fm,pos,0,2,0,side_x_b,side_x_e,side_y_b,side_y_e,draws->draw_pos,
     false,draws->draw_axis3,false,false,0.,0.,false,false);
  slices[2]->set_draw_params(fm,pos,2,1,0,top_x_b,top_x_e,top_y_b,top_y_e,draws->draw_pos,
      false,false,draws->draw_axis3,false,0.,0.,true,false);

  render_it(painter,pen, fact, dat, pos,  overlay, draws);
  //draw_line line=draw_line(cx,cy,cx+4,cy-4);
//  line.draw(painter);
}
