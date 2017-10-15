#include<cube.h>
#include<ctime>
cube::cube(QString view,QFontMetrics *f_m, pick_draw *_pks,mode *mm,draw_other *dr){ 
 pct_top=.4; pct_front=.6;fm=f_m;
 mym=mm;
 iview=view;
    corner_sel=0;

 set_basics(_pks,dr);
   com.push_back(iview); com.push_back("");com.push_back("");com.push_back("");
com.push_back("");
} 
void cube::viewit(QPainter *painter,QPen *pen, slice *fact, 
  dataset *dat,orient_cube *pos,int b_x, int e_x, int b_y, int e_y, draw_what *draws,
  bool overlay){

    cur_dat=dat;
    this->set_size(b_x,e_x,b_y,e_y);
    


        if(draws->draw_title) set_title_pos(painter,dat);

     bound_l=bound_r=bound_b=bound_t=(int)(fm->height()/2.);
      if(draws->draw_bar) bound_b+=(fm->height())*5;
           if(draws->draw_title) set_title_pos(painter,dat);

        if(draws->draw_title) bound_b+=(int)(fm->height()*1.5);
    bsmall=0;
 

    int cx,cy;
    get_corner_loc(&cx,&cy);
    int front_x_b=bound_l;
    int front_x_e=cx;
    int front_y_b=cy;
    int front_y_e=ey-bound_b;
    pos->fix_min(1);
  

    
    int side_x_b=cx;
    int side_x_e=ex-bound_r;
    int side_y_b=cy;
    int side_y_e=ey-bound_b;
    
    int top_x_b=bx+bound_l;
    int top_x_e=cx;
    int top_y_b=by+bound_t;
    int top_y_e=cy;

 
 
    float slope=((float)(cx-ex+bound_r))/((float)(cy-(by+bound_t)));
   // slope=-1.;

    //Correct for shift
    int shift=(int)(slope*(top_y_e-top_y_b));
    top_x_b=top_x_b-shift;
    top_x_e=top_x_e-shift;
    
    shift=(int)(slope*(side_x_e-side_x_b));
    side_y_b=side_y_b /*stew-->*/ - shift;
    side_y_e=side_y_e /*stew-->*/ - shift;
    this->delete_slices();
 
     for(int i=0; i < 3; i++) slices.push_back(fact->clone());

 slices[0]->set_draw_params(fm,pos,0,1,front_x_b,front_x_e,front_y_b,front_y_e,draws->draw_pos,
   false,draws->draw_axes,draws->draw_axes,false,0.,0.,false,false);
  


//  shift=top_y_e;
slices[1]->set_draw_params(fm,pos,2,1,top_x_b,top_x_e,top_y_b,top_y_e,draws->draw_pos,
    false,false,draws->draw_axes,false,0.,slope,true,false);
    

slope=1./slope;

 slices[2]->set_draw_params(fm,pos,0,2,side_x_b,side_x_e,side_y_b,
    side_y_e,draws->draw_pos,false,true,false,false,slope,0.,false,false);

    

     render_it(painter,pen, fact, dat, pos,  overlay, draws);
//  draw_line line=draw_line(cx,cy,cx+4,cy-4);
 // line.draw(painter);
}
