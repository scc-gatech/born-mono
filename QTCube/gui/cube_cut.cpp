#include<cube_cut.h>
#include<ctime>
using namespace SEP;
cube_cut::cube_cut(QString view,std::shared_ptr<QFontMetrics >f_m,std::shared_ptr<pick_draw>_pks,std::shared_ptr<draw_other>dr,std::vector<float>prop){ 
 pct_top=.4; pct_front=.6;fm=f_m;
 set_basics(_pks,dr);
  ori_f=0; ori_s=0; ori_t=0; ori=0;
   iview=view;
     corner_sel=0;
  proportions=prop;
   com.push_back(iview); com.push_back("");com.push_back("");com.push_back("");
   com.push_back("");
} 
void cube_cut::delete_orient(){

}
void cube_cut::viewit(QPainter *painter,QPen *pen, std::shared_ptr<slice>fact, std::shared_ptr<dataset>dat,std::shared_ptr<orient_cube>pos,int b_x, int e_x, int b_y, int e_y, std::shared_ptr<draw_what>draws,bool overlay){
    cur_dat=dat;
    this->set_size(b_x,e_x,b_y,e_y);

    baxes=fm->height()*2;
      if(draws->draw_title) set_title_pos(painter,dat);

    bsmall=0;
    bound_l=bound_r=bound_b=bound_t=fm->height()/2;
    if(draws->draw_bar) bound_b+=(fm->height())*5;
    if(draws->draw_title) bound_b+=(int)(fm->height()*1.5);
    
    int cx,cy;
  if(proportions[0]<-1) get_corner_loc(&cx,&cy);
  else{
    get_corner_loc(&cx,&cy,proportions[pos->get_order(0)],proportions[pos->get_order(1)],
      proportions[pos->get_order(2)]);
  }
  int front_x_b=bound_l;
    int front_x_e=cx;
    int front_y_b=cy;
    int front_y_e=ey-bound_b;
    
  

    
    int side_x_b=cx;
    int side_x_e=ex-bound_r;
    int side_y_b=cy;
    int side_y_e=ey-bound_b;
    
    int top_x_b=bx+bound_l;
    int top_x_e=cx;
    int top_y_b=by+bound_t;
    int top_y_e=cy;

 
    pos->fix_min(3);
    float slope=((float)(cx-ex+bound_r))/((float)(cy-(by+bound_t)));
   // slope=-1.;

    //Correct for shift
    int shift=(int)(slope*(top_y_e-top_y_b));
    top_x_b=top_x_b-shift;
    top_x_e=top_x_e-shift;
    
    shift=(int)(slope*(side_x_e-side_x_b));
    side_y_b=side_y_b;
    side_y_e=side_y_e;
    this->delete_slices();
    this->delete_orient();
    
  int order[8];
  for(int i=0; i <8; i++) order[i]=pos->get_order(i);
  std::shared_ptr<orient_cube> o(new orient_cube(pos));
  ori=o;
  for(int i=0; i < 3; i++){
    ori->set_loc(order[i],pos->get_beg(order[i]));
  }

  for(int i=0; i < 6; i++) slices.push_back(fact->clone());
  
  
  slices[0]->set_draw_params(fm,ori,0,1,0,front_x_b,front_x_e,front_y_b,front_y_e,draws->draw_pos,
   false,draws->draw_axis1,draws->draw_axis2,false,0.,0.,false,false);
  
  slices[1]->set_draw_params(fm,ori,2,1,0,top_x_b,top_x_e,top_y_b,top_y_e,draws->draw_pos,
    false,false,draws->draw_axis3,false,0.,slope,true,false);
  slices[2]->set_draw_params(fm,ori,0,2,0,side_x_b,side_x_e,side_y_b,
    side_y_e,draws->draw_pos, false,draws->draw_axis3,false,false,1./slope,0.,false,false);

   for(int i=0; i < 3; i++)
     slices[i]->draw_slice(painter,dat,pen,ori,overlay,false);


  std::shared_ptr<orient_cube> x1(new orient_cube(pos));
    std::shared_ptr<orient_cube> x2(new orient_cube(pos));
  std::shared_ptr<orient_cube> x3(new orient_cube(pos));

  ori_f=x1;
  ori_s=x2;
  ori_t=x3;

  //Now shift into position
  int shiftx,shifty;
  
  float pct[3];
  int b,e;
  for(int i=0; i< 3; i++) {
    b=pos->get_beg(order[i]);
    e=pos->get_end(order[i]);
    pct[i]=((float)(pos->get_loc(order[i])-b))/((float)(e-b));
  }
  
   shift=(int)(slope*(float)(top_y_e-top_y_b)*(pct[2]));
top_x_b=(int)((pct[1]-.01)*((float)(top_x_e-top_x_b-baxes))+baxes-shift+baxes*-1./slope);
  
  top_x_e=cx-shift;
  
  top_y_b=(int)((1.-pct[2])*((float)(top_y_e-top_y_b))+top_y_b); 
  
  
  shifty=(int)(pct[0]*(ey-bound_b-baxes-cy));
  shiftx=0;
  

      //float slope=((float)(cx-ex+bound_r))/((float)(cy-(by+bound_t)));


  slices[3]->set_draw_params(fm,ori_t,2,1,0,top_x_b+shiftx,shiftx+top_x_e,
     shifty+top_y_b,shifty+top_y_e,false,
    false,false,false,false,0.,slope,true,false,true);



  int iloc[3];

  for(int i=0; i < 3; i++){
   iloc[i]=pos->get_loc(order[i]);
   b=pos->get_beg(order[i]);
   e=pos->get_end(order[i]);
   if(iloc[i]==b) iloc[i]+=1;
   if(iloc[i]==e) iloc[i]--;
  }
  
  ori_f->set_beg(order[1],iloc[1]);
  ori_f->set_end(order[0],iloc[0]+1);

  front_y_e=(int)(pct[0]*(float)(front_y_e-front_y_b-baxes)+front_y_b);
  front_x_b=(int)(pct[1]*(float)(front_x_e-front_x_b-baxes)+front_x_b+baxes);
  
  
  shifty=(int)((pct[2])*(float)(cy-by-bound_t));
  shiftx=(int)(shifty*slope);

  
    slices[4]->set_draw_params(fm,ori_f,0,1,0,front_x_b-shiftx,front_x_e-shiftx, front_y_b-shifty,
   front_y_e-shifty,false,
   false,false,false,false,0.,0.,false,false);
   
   
  
  ori_t->set_end(order[2],iloc[2]+1);
  ori_t->set_beg(order[1],iloc[1]);
 
 
  ori_s->set_end(order[2],iloc[2]+1);
  ori_s->set_end(order[0],iloc[0]);

  side_y_e=(int)(pct[0]*(float)(side_y_e-side_y_b-baxes))+side_y_b;
  side_x_e=(int)(pct[2]*(float)(side_x_e-side_x_b))+side_x_b;

  shiftx=-(int)((1.-pct[1])*(float)(cx-bx-bound_r-baxes));
  shifty=0;


  //fprintf(stderr,"SIDE IS %d %d %d %d \n",side_x_b,shiftx,side_x_e);
  slices[5]->set_draw_params(fm,ori_s,0,2,0,side_x_b+shiftx,shiftx+side_x_e,shifty+side_y_b,
    shifty+side_y_e,false,false,false,false,false,
     1./slope,0.,false,false);

/*


 // fprintf(stderr,"TOP IS %d %d %d \n",top_x_b,shiftx,top_x_e);
 slices[5]->set_draw_params(fm,ori_t,2,1,
   side_x_b+shiftx,shiftx+top_x_e,
     shifty+top_y_b,shifty+top_y_e,false,
    false,false,false,false,0.,slope,true,false,true);
*/
//    if(draws->draw_pos) for(int i=0; i < 3; i++) this->pg->draw(painter, pos, slices[i],dat);
 if(draws->draw_pos) for(int i=0; i < 3; i++) this->pk->draw(painter,  slices[i]);
     slices[3]->draw_slice(painter,dat,pen,ori_t,overlay,false);

    slices[4]->draw_slice(painter,dat,pen,ori_f,overlay,false);

    slices[5]->draw_slice(painter,dat,pen,ori_s,overlay,false);

  if(draws->draw_picks&& cur_dat->pickable){
    //for(int i=3; i < 6; i++) this->pg->draw(painter, pos, slices[i],dat);
      for(int i=3; i < 6; i++) this->pk->draw(painter,slices[i]);
    draw_o->draw(painter);
    draw_o->reset();
  }
  
      if(draws->draw_bar) draw_bar(4*bound_l,painter,pen,std::static_pointer_cast<raster>(fact),
        dat->conv,draws->draw_title);
     
     
    if(draws->draw_title) draw_title(0,painter,pen,dat->get_name());
  //draw_line line=draw_line(cx,cy,cx+4,cy-4);
//  line.draw(painter);
    
    //delete top; delete front; delete side;

   // this->clear_mouse();
}
