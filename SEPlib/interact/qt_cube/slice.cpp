#include<slice.h>
#include<math.h>
#include<qnamespace.h>
#include <QPolygon>
#include <QPixmap>
/*
	Sets the parameters for drawing the slice.
*/
/*ARGSUSED*/
void slice::set_draw_params(QFontMetrics *f_m,orient_cube *pos, int i1, int i2, int b_x, int e_x, int b_y, int e_y,  int dopos,bool at,bool ab, bool al, bool ar,float s1,float s2, bool rev1, bool rev2, bool verb){

(void) verb; // silence unused argument compiler warning
  sx=s1;  //Skew parameter
  sy=s2; //Skew parameter
  fm=f_m;
  bx=b_x;
  ex=e_x;
  rev_1=rev1;
  rev_2=rev2;
  by=b_y;
  ey=e_y;
  xb=0;
  xe=ex-bx;
  yb=0;
  ye=ey-by;
  n1_pixel=ye;
int f1,e1;

  int idat1=pos->get_order(i1), idat2=pos->get_order(i2);
  axis_left=al;
  axis_right=ar;
  axis_top=at;
  axis_bot=ab;
  boundary=fm->height()*2;
   if(axis_top) {
     by+=boundary;
     ye-=boundary;
   }
   if(axis_bot) ye-=boundary;
   if(axis_left){
     bx+=boundary;
     xe-=boundary;
   }
   if(axis_right)   xe-=boundary;
   

  my_pos=pos;

  iax1=idat1;
  iax2=idat2;
  int f2,e2;
 pos->get_axis_range(iax1,&f1,&e1); n1_slice=e1-f1;
 pos->get_axis_range(iax2,&f2,&e2); n2_slice=e2-f2;

  ps=pos;
  do_pos =dopos;
  
  reverse1=((pos->get_reverse(iax1) || rev1)&&(rev1!=pos->get_reverse(iax1)));
  reverse2=((pos->get_reverse(iax2) || rev2)&&(rev2!=pos->get_reverse(iax2)));
 
  ps->form_index_map(iax1,iax2,reverse1,reverse2);
  temp_map=form_pixel_slice_map();
  int ilocs[8];
  ps->get_locs(ilocs);
  slice_to_grid_map=ps->get_index_map_ptr(iax1,iax2,f1,e1,f2,e2,0);
  int order[8];
  ps->get_orders(order);
  if((iax1==order[0]  && iax2==order[1]) || (iax2==order[0] && iax1==order[1])) i3_axis=order[2];
  if((iax1==order[0]  && iax2==order[2]) || (iax2==order[0] && iax1==order[2])) i3_axis=order[1];
  if((iax1==order[2]  && iax2==order[1]) || (iax2==order[2] && iax1==order[1])) i3_axis=order[0];
  
}
int  *slice::form_pixel_slice_map(){

/*
       
   int i_x=(int)(-(iy-by)*sy+ix-bx);
   int i_y=(int)(-(ix-bx)*sx+iy-by);
   */

int n1 /*,n2*/;
  my_pos->get_axis_range(iax1,&b_1,&e_1); n1=e_1-b_1;
/*
  my_pos->get_axis_range(iax2,&b_2,&e_2); n2=e_2-b_2;
*/
int *map=new int[xe*ye*2];
for(int i=0; i < 2*xe*ye; i++) map[i]=-1;
//float den=1./(1-sx*sy);
int i=0;

//
// Don't forget that axis 1 goes with y dimension
//
//
pixel_to_slice_map=new int [xe*ye];
for(int i2=0; i2 < xe; i2++){ 
  for(int i1=0; i1 < ye; i1++,i++){
     float pctx=(float)(i2-xb)/((float)(xe-xb));
     float pcty=(float)(i1-yb)/((float)(ye-yb));
     int ia=(int)(pcty*(e_1-b_1)+.5);
     int ib=(int)(pctx*(e_2-b_2)+.5);
     pixel_to_slice_map[i]=ia+ib*n1;
     if(reverse1) pcty=1.-pcty;
     if(reverse2) pctx=1.-pctx;
     int iy=((int)(pcty*((float)(e_1-b_1))))+b_1;
     int ix=((int)(pctx*((float)(e_2-b_2))))+b_2;
     map[i]=iy;
     map[i+xe*ye]=ix;
     // fprintf(stderr,"writing map %f %f %d %d %d %d \n",pctx,pcty,ix,iy,map[i],map[i+xe*ye]);
    }
}


return map;
}
int *slice::get_pixel_to_slice_map(int *n1, int *n2){
   *n1=ye;
   *n2=xe;
   return pixel_to_slice_map;
}
long long *slice::get_slice_to_grid_map(){

   return slice_to_grid_map;

}
pairs_new *slice::return_pick_locs(QString col,picks_vec *pk ){
  pairs_new *pout=new pairs_new();
  for(int i=0; i < pk->return_size(); i++){
    int ix,iy;
    pick_new *p=pk->return_pick(i);
    if(col==p->col){
      bool inside=get_shifted_image_pos_from_sample(p->iloc[iax2],p->iloc[iax1],&ix,&iy);
      if(inside) pout->add_point(ix,iy);
    }
  }
  return pout;

}
long long *slice::get_index_map_ptr(int idelta){
  int f2,e2,f1,e1;
  ps->get_axis_range(iax1,&f1,&e1); 
  ps->get_axis_range(iax2,&f2,&e2); 
  return ps->get_index_map_ptr(iax1,iax2,f1,e1,f2,e2,idelta);
}
void slice::draw_pos(QPainter *painter, orient_cube *pos){
  int p_1=0,p_2=0,ix1,ix2,iy1,iy2;
 // int ix1_1,ix1_2,iy1_1,iy1_2;
 // int ix2_1,ix2_2,iy2_1,iy2_2;


   QPen pen=painter->pen();
   QColor col=pen.color();
   QColor col2=col;
   
   col.setAlphaF((float)do_pos/100.);
	     pen.setColor(col); 

	     painter->setPen(pen);
	     painter->setBrush(col);
	    
 int locs[8]; pos->get_locs(locs);
    if(do_pos){
      int x1,y1;
      QPolygon pointarray;
      int ic=0;
      float f=b_2-.5;
         p_1=pos->get_loc(iax1); 
    p_2=pos->get_loc(iax2);
      while(f<=e_2){
        get_shifted_image_pos_from_sam_fract(f,p_1*1.,&x1,&y1);

        pointarray.putPoints(ic, 1,x1,y1);
        ic++;
        f+=1;
     }
     //painter->drawPolyline(pointarray);
     pointarray.clear();
     ic=0;
     f=b_1-.5;
     while(f<=e_1){
        get_shifted_image_pos_from_sam_fract(p_2*1.,f,&x1,&y1);
        pointarray.putPoints(ic, 1,x1,y1);
        ic++;
        f+=1;
     }
      painter->drawPolyline(pointarray);
    }
     get_shifted_image_pos_from_sam_fract(b_2-.5,p_1,&ix1,&iy1);
 get_shifted_image_pos_from_sam_fract(e_2-.5,p_1,&ix2,&iy2);
    painter->drawLine(ix1,iy1,ix2,iy2);

 
   get_shifted_image_pos_from_sam_fract(p_2,b_1-.5,&ix1,&iy1);
    get_shifted_image_pos_from_sam_fract(p_2,e_1-.5,&ix2,&iy2);
    if(1==2){
    p_1=pos->get_loc(iax1); 
    p_2=pos->get_loc(iax2);
  // void slice::get_image_pos_pct(float pctx,float pcty,int *id1, int *id2){

 get_shifted_image_pos_from_sam_fract(b_2-.5,p_1,&ix1,&iy1);
 get_shifted_image_pos_from_sam_fract(e_2-.5,p_1,&ix2,&iy2);
    
    
//   painter->drawLine(ix1,iy1,ix2,iy2);
  // fprintf(stderr,"IMAGE POS FROM SAMPLE\n");
  get_shifted_image_pos_from_sam_fract(p_2,b_1-.5,&ix1,&iy1);
    get_shifted_image_pos_from_sam_fract(p_2,e_1-.5,&ix2,&iy2);

//   painter->drawLine(ix1,iy1,ix2,iy2);
}
return;
  int i,j;
  get_image_pos(p_2,p_1,&i,&j);
  ix1=(int)(xb+j*sy+bx);
  ix2=(int)(xe+j*sy+bx);
  iy1=(int)(j+xb*sx+by);
  iy2=(int)(j+xe*sx+by);
  

	
  ix1=(int)(i+yb*sy+bx);
  ix2=(int)(i+ye*sy+bx);
  iy1=(int)(yb+i*sx+by);
  iy2=(int)(ye+i*sx+by);
 painter->drawLine(ix1,iy1,ix2,iy2);
  


  
  
  
    
    QPolygon pointarray;
    bool t;
    t=get_shifted_image_pos_from_sample(b_2,b_1,&ix1,&iy1);
    pointarray.putPoints(0, 1,ix1,iy1);
    t=get_shifted_image_pos_from_sample(b_2,e_1,&ix2,&iy2);
    pointarray.putPoints(1, 1,ix2,iy2);
    t=get_shifted_image_pos_from_sample(e_2,e_1,&ix2,&iy2);
    pointarray.putPoints(2, 1,ix2,iy2);
    t=get_shifted_image_pos_from_sample(e_2,b_1,&ix2,&iy2);
    pointarray.putPoints(3, 1,ix2,iy2);
    pointarray.putPoints(4, 1,ix1,iy1);

    t=!(!t); /* quiet unused var complaint */

	//	painter->drawPolyline(pointarray);
		
		     pen.setColor(col2); 
	     painter->setPen(pen);
	     painter->setBrush(col2);
}  

/*
	Draws the axes for the graph.
*/

void slice::draw_axes(QPainter *painter,orient_cube *pos,bool draw_grid){
  int ntic1,ntic2=0;
  int tic_len=fm->height()/3;
  float otic1,otic2,dtic1,dtic2;
  // int npixels=20; //Distance between tics (approx)
  axis ax1=pos->get_axis(iax1);
  axis ax2=pos->get_axis(iax2);
  int xs,ys;
   int f,e;
   pos->get_axis_range(iax1,&f,&e); 

  
  float mn=f*ax1.d+ax1.o;
  float mx=(e-1)*ax1.d+ax1.o;
/*
  float o1,d1,o2,d2;
  o1=ax1.o;
  d1=ax1.d;
*/
  if(is_overlay) { 
    return;
  }
/*
  o2=ax2.o;
  d2=ax2.d;
*/
  float ffpos;
  int ipos;
  int ix,iy,ix2,iy2;
  ntic1=this->optimal_with_font(ye-yb,mn-ax1.d/2.,mx+ax1.d/2.,&otic1,&dtic1);
 // fprintf(stderr,"I GET BACK min=%f max=%f otic=%f dtic=%f \n",mn,mx,otic1,dtic1);
  
//HERE WE GO
  QString aa;
  int ph,pw;
  
     int eleft=e_2,bleft=b_2;
      if(reverse2){
      eleft=b_2;
      bleft=e_2;
    }

  if(reverse1) {
     otic1=otic1+dtic1*(ntic1-1);
     dtic1=-dtic1;
  }

  float rad=atan(1.0*sy);
  float ang=atan(sy)*180./3.1415926536;
  
  int pos_old=-9999;
  int delta= 10; 
/*
  int dir=1;
  if(dtic1<0){ dir=-1;}
*/

  for(int i=0; i < ntic1; i++){
         ffpos=(otic1+dtic1*i-ax1.o)/ax1.d;
       ipos=(int)(ffpos+.5f);
//       ipos=(int)((otic1+dtic1*i-o1)/d1);
       double val=otic1+dtic1*i;
     aa.sprintf("%1.5g", val);
       pw = fm->width( aa );
       ph=  fm->height();

  if(draw_grid){
     
     get_shifted_image_pos_from_sam_fract((float)bleft,ffpos,&ix,&iy);
     get_shifted_image_pos_from_sam_fract((float)eleft,ffpos,&ix2,&iy2);
     painter->drawLine(ix,iy,ix2,iy2);
    
    }

       if(axis_left) {

       //fprintf(stderr,"LOOP %d %f %f \n",i,otic1+dtic1*i,ffpos);
         get_shifted_image_pos_from_sam_fract((float)bleft,ffpos,&ix,&iy);
         painter->drawLine(ix,iy,ix-(int)(tic_len*cos(rad)),iy+(int)(tic_len*sin(rad)));

         if(ang>= -.0001){
         ys=(int)(cos(rad)*pw/2.-sin(rad)*(ph/2+2*tic_len));
         xs=(int)(cos(rad)*(2*tic_len+ph/2)+sin(rad)*pw/2.);
         
         painter->translate(ix-xs,iy-ys);
         painter->rotate(90-ang);
//        if((dir==1&&pos_old<iy-pw/2) || pos_old > iy+pw/2 ){ /*}*/

        if(pos_old<iy-pw/2){
          painter->drawText(0,0,aa);
            pos_old=iy+pw/2+delta;
        }

         painter->rotate(-90+ang);
         painter->translate(-ix+xs,-iy+ys);
        // fprintf(stderr,"IN HERE 0 %d %d %s \n",iax1,iax2,aa.ascii());


         }
         
         else{

         ys=(int)(cos(rad)*pw/2.-sin(rad)*(-2*tic_len));
         xs=(int)(cos(rad)*(-2*tic_len)+sin(rad)*pw/2.);
         painter->translate(ix+xs,iy+ys);
         painter->rotate(-90-ang);
        if(pos_old<iy-pw/2){
          painter->drawText(0,0,aa);
           pos_old=iy+pw/2+delta;
        }

        painter->rotate(90+ang);
         painter->translate(-ix-xs,-iy-ys);
         }
         }

       if(axis_right){

              this->get_shifted_image_pos_from_sam_fract((float)eleft,ipos+.5,&ix,&iy);

         painter->drawLine(ix,iy,ix+tic_len,iy);
         painter->translate(ix+tic_len+tic_len/2,iy-pw/2);
         painter->rotate(90);
      if(pos_old<iy-pw/2){
          painter->drawText(0,0,aa);
           pos_old=iy+pw/2+delta;
        }

         painter->rotate(-90);
         painter->translate(-ix-tic_len-tic_len/2,-iy+pw/2);
        }

    }
   
    aa=ax1.label.c_str();
    pw = fm->width( aa ); 
    ph=  fm->height();
    if(reverse1) {
      otic2=otic2+dtic2*(ntic2-1);
      dtic2=-dtic2;
    }

    if(axis_left){   
         get_shifted_image_pos_from_sam_fract((float)bleft,(b_1+e_1)/2.,&ix,&iy);
         if(ang>= -.0001){
     //   fprintf(stderr,"INa HERE 1 %d %d \n",iax1,iax2);

         ys=(int)(cos(rad)*pw/2.-sin(rad)*(ph+2.3*tic_len));
         xs=(int)(cos(rad)*(2.3*tic_len+ph)+sin(rad)*pw/2.);
         painter->translate(ix-xs,iy-ys);
         painter->rotate(90-ang);
          painter->drawText(0,0,aa);

         painter->rotate(-90+ang);
         painter->translate(-ix+xs,-iy+ys);
         }
         else{
     //   fprintf(stderr,"INb HERE 1 %d %d \n",iax1,iax2);
         
         ys=(int)(cos(rad)*pw/2.-sin(rad)*(-ph-2.3*tic_len));
         xs=(int)(cos(rad)*(-2.3*tic_len-ph)+sin(rad)*pw/2.);
         painter->translate(ix+xs,iy+ys);
         painter->rotate(270-ang);
          painter->drawText(0,0,aa);

         painter->rotate(-270+ang);
         painter->translate(-ix-xs,-iy-ys);
         
         }
         
   }

    if(axis_right){
      painter->translate(xe+tic_len+ph/2+tic_len/2,(yb+ye)/2-pw/2);
      painter->rotate(90);
          painter->drawText(0,0,aa);

      painter->rotate(-90);
      painter->translate(-xe-tic_len-ph/2-tic_len/2,-(yb+ye)/2+pw/2);
    
    }

  ang=atan(sx)*180./3.1415926536;
  rad=atan(sx);
   int ff,ee;
   pos->get_axis_range(iax2,&ff,&ee); 
  mn=ff*ax2.d+ax2.o;
  mx=(ee-1)*ax2.d+ax2.o;

  ntic2=this->optimal_with_font(xe-xb,mn-ax2.d/2.,mx+ax2.d/2.,&otic2,&dtic2);
   // fprintf(stderr,"I GET BACK2 min=%f max=%f otic=%f dtic=%f \n",mn,mx,otic1,dtic1);
    int btop=b_1;
    int etop=e_1;
/*
    float btop_f=mx;
    float etop_f=mn;
*/

    if((rev_1 && !pos->get_reverse(iax1)) || (!rev_1 && pos->get_reverse(iax1))){
      etop=b_1;
      btop=e_1;
/*
      btop_f=mn;
      etop_f=mx;
*/
    }
    pos_old=-9999;
    for(int i=0; i < ntic2; i++){
       ffpos=(otic2+dtic2*i-ax2.o)/ax2.d;
       //ipos=(otic2+dtic2*i-ax2.o)/ax2.d;
    ipos=(int)(ffpos+.5f);

    //AAAAAA
      // fprintf(stderr,"I think %f is at %d %f sample \n",
       //   otic2+dtic2*i,ipos,ffpos);
     double val=otic2+dtic2*i;
     aa.sprintf("%1.5g", val);
        pw = fm->width( aa );
        ph=  fm->height()/2;

         if(draw_grid){
   get_shifted_image_pos_from_sam_fract(ipos+.5,(float)btop,&ix,&iy);
         get_shifted_image_pos_from_sam_fract(ipos+.5,(float)etop,&ix2,&iy2);
    painter->drawLine(ix,iy,ix2,iy2);
    
    }

       if(axis_top==true) {
       this->get_shifted_image_pos_from_sam_fract(ipos+.5,(float)btop, &ix,&iy);

         painter->drawLine(ix,iy,ix,iy-tic_len);
         
            if(pos_old<ix-pw/2){
         painter->drawText(ix-pw/2,iy-tic_len-tic_len/2,aa);
           pos_old=ix+pw/2+delta;
        }

       }

      if(axis_bot==true){      
            this->get_shifted_image_pos_from_sam_fract(ffpos,etop*1., &ix,&iy);
  //fprintf(stderr,"COMPAFRE %d,%d to %d,%d \n",ix,iy,ix,iy);
         painter->drawLine(ix,iy,(int)(ix-1.0*tic_len*sin(rad)),(int)(iy+1.0*tic_len*cos(rad)));
         xs=(int)(cos(rad)*pw/2.+sin(rad)*(ph+2*tic_len));
         ys=(int)(cos(rad)*(2*tic_len+ph)-sin(rad)*pw/2.);
         painter->translate(ix-xs,iy+ys);
         painter->rotate(ang);
      if(pos_old<ix-pw/2){
      
          painter->drawText(0,0,aa);
          
           pos_old=ix+pw/2+delta;
        }

         painter->rotate(-ang);
         painter->translate(-ix+xs,-iy-ys);
       }
    }

        aa=ax2.label.c_str();
    pw = fm->width( aa ); 
    ph=  fm->height();

    if(axis_top){ 
    
      get_shifted_image_pos_from_sam_fract((b_2+e_2)/2.,btop*1., &ix,&iy);
      xs=(int)(cos(rad)*(pw/2)+sin(rad)*(tic_len*2+ph));
      ys=(int)(cos(rad)*(2*tic_len+ph)-sin(rad)*pw/2.);
      painter->translate(ix-xs,iy+ys);
      painter->rotate(ang);
          painter->drawText(0,0,aa);

      painter->rotate(-ang);
       painter->translate(-ix+xs,-iy-ys);
     // painter->drawText((xb+xe)/2-pw/2,yb-tic_len-ph-tic_len/2,aa);
    }

    if(axis_bot){

      get_shifted_image_pos_from_sam_fract((b_2+e_2)/2.,etop, &ix,&iy);

      xs=(int)(cos(rad)*pw/2.+sin(rad)*(ph+2.5*tic_len));
      ys=(int)(cos(rad)*(2.5*tic_len+ph)-sin(rad)*pw/2.);
      painter->translate(ix-xs,iy+ys);painter->rotate(ang);
          painter->drawText(0,0,aa);

      painter->rotate(-ang);
       painter->translate(-ix+xs,-iy-ys);
      //painter->drawText((xb+xe)/2-pw/2,ye+tic_len+ph+tic_len/2,aa);

    }

}
bool slice::in_slice(int ix, int iy){
  bool ret=false;
     int i_x=-((int)((iy-by)*sy+ix-bx));
   int i_y=-((int)((ix-bx)*sx+iy-by));

   if(i_x >= xb && i_x <= xe && i_y >= yb && i_y <=ye) ret=true;
   return ret;
}
void slice::update_pos(std::vector<QString> *com,int ix,int iy, orient_cube *pos){
   int loc[8];
   std::vector<QString> com2;
  com->at(1)="none";
  if(this->in_slice(ix,iy)==true){
     //int id1,id2;
     pos->get_locs(loc);

     this->get_data_loc(ix,iy,loc);
     com->at(1)="navigate";
     com->at(2)="move";
     com->at(3)=string_from_array(8,loc);

     
  }
}
void slice::update_range(std::vector<QString> *com, int ix_old,int iy_old, int ix, int iy, orient_cube *pos){
  
 com->at(1)="none";
  if(this->in_slice(ix,iy)==true){
    int beg[8],end[8];
    int id1_b,id2_b,id1_e,id2_e,i;
    int loc[8];
    pos->get_locs(loc); //RGC-This maybe should be differently when in a rotated/stretched coordinate
    this->get_data_loc(ix_old,iy_old,loc);
    id2_b=loc[iax2]  ; id1_b=loc[iax1];
    pos->get_locs(loc);
    this->get_data_loc(ix    ,iy    ,loc);
        id2_e=loc[iax2]  ; id1_e=loc[iax1];

    if(id1_b > id1_e) { i=id1_e;id1_e=id1_b; id1_b=i;}
    if(id2_b > id2_e) { i=id2_e;id2_e=id2_b; id2_b=i;}
       
    pos->get_begs(beg); pos->get_ends(end);
    beg[iax2]=id2_b; beg[iax1]=id1_b;
    end[iax2]=id2_e; end[iax1]=id1_e;
    if(end[iax2]-beg[iax2]<=1){
       if(end[iax2]==1) {
          beg[iax2]=0; end[iax2]=2;
       }
       else{
          beg[iax2]=end[iax2]-2;
       }
    }
    if(end[iax1]-beg[iax1]<=1){
          if(end[iax1]==1) {
          beg[iax1]=0; end[iax1]=2;
       }
       else{
          beg[iax1]=end[iax1]-2;
       } 
    
    }
    
    com->at(1)="navigate";com->at(2)="range";
    com->at(3)=string_from_array(8,beg)+":"+string_from_array(8,end)+":"+
      QString::number(iax1)+":"+QString::number(iax2);
  }
  
}

void slice::get_data_loc(int ix, int iy,int *loc_new){
 
  
   int i_x=(int)(-(iy-by)*sy+ix-bx);
   int i_y=(int)(-(ix-bx)*sx+iy-by);

  //Return the point we cliccked on 
 
  ps->index_to_loc(slice_to_grid_map[pixel_to_slice_map[i_y+i_x*ye]],loc_new);
 ps->rotation_to_grid_loc(iax1,iax2,loc_new);

}
void slice::get_slice_axes(orient_cube *pos, int *i1, int *i2, int *i3){
  *i1=iax1;
  *i2=iax2;
  int d1=pos->get_order(0), d2=pos->get_order(1), d3=pos->get_order(2);
  if(d1!=iax1 && d1!=iax2) *i3=d1;
  if(d2!=iax1 && d2!=iax2) *i3=d2;
  if(d3!=iax1 && d3!=iax2) *i3=d3;
 }

void slice::get_shifted_image_pos_from_sample(int ipos, int *id1, int *id2){
  int i,j;
  j=ipos/n1_slice; i=ipos-j*n1_slice;
  get_shifted_image_pos_from_sample(j,i,id1,id2);
}
bool slice::get_shifted_image_pos_from_sample(int iax2, int iax1, int *id1, int *id2){
  int i,j;
  bool t=this->get_image_pos_sample((float)iax2,(float)iax1,&i,&j);
  *id1=(int)(i+j*sy+bx);
  *id2=(int)(j+i*sx+by);
  return t;

}
void slice::get_shifted_image_pos_from_sam_fract(float iax2, float iax1, int *id1, int *id2){
  int i,j;

  assert(this->get_image_pos_sample(iax2,iax1,&i,&j));
  *id1=(int)(i+j*sy+bx);
  *id2=(int)(j+i*sx+by);

}
void slice::get_shifted_image_pos_from_loc(float iax2, float iax1, int *id1, int *id2,bool ignore_rev){
  int i,j;
  this->get_image_pos(iax2,iax1,&i,&j,ignore_rev);
  *id1=(int)(i+j*sy+bx);
  *id2=(int)(j+i*sx+by);

}
bool slice::get_image_pos_sample(float ix, float iy,int *id1, int *id2){

  int loc[8];
  memcpy(loc,my_pos->loc,sizeof(int)*8);
  //IO_loc assumes an integer so we are going to round to the nearest sample and then
  //redo the fraction
  int iix=(int)(ix+.5), iiy=(int)(iy+.5);
  float fx=ix-iix, fy=iy-iiy;
  loc[iax2]=iix; loc[iax1]=iiy;
  
  my_dat->get_io_loc(loc);

  float pctx=((float)(loc[iax2]+fx-b_2+.5)/((float)(e_2-b_2)));
  float pcty=((float)(loc[iax1]+fy-b_1+.5)/((float)(e_1-b_1)));
  if(reverse1) pcty=1.-pcty;
  if(reverse2) pctx=1.-pctx;

  float pct[2]; pct[0]=pcty; pct[1]=pctx;
 if(pct[0]<-.01 || pct[1]<-.01 || pct[0]>1.01 || pct[1]>1.01) return false;
 pct[0]=std::max(0.f,std::min(.99999f,pct[0]));
  pct[1]=std::max(0.f,std::min(.99999f,pct[1]));

 ps->shift_data_image(iax1,iax2,pct);
  int pix_x=(int)(pct[1]*((float)(xe-xb)));
  int pix_y=(int)(pct[0]*((float)(ye-yb)));
  //Now deal with torsion in coordinate system

  *id2=pix_y+yb;
  *id1=pix_x+xb;
  return true;
}
void slice::get_image_pos(float x,float y,int *id1, int *id2,bool ignore_rev){

  float xx=(x-ox)/dx,yy=(y-oy)/dy;
  xx=(x-axes[1].o)/axes[1].d;
  yy=(y-axes[0].o)/axes[0].d;
  
  int loc[8];
  memcpy(loc,my_pos->loc,sizeof(int)*8);
  float f2=xx-(int)xx, f1=yy-(int)yy;
  loc[iax2]=(int)xx; loc[iax1]=(int)yy;
  my_dat->get_io_loc(loc);
  
  float a2=loc[iax2], a1=loc[iax1];
  loc[iax2]=(int)xx+1; loc[iax1]=(int)yy+1;
  my_dat->get_io_loc(loc);
  ps->orient_data_loc(iax1,iax2,loc);

  xx=(1.-f2)*a2+f2*loc[iax2];
  yy=(1.-f1)*a1+f1*loc[iax1];
  float pctx=((float)(xx-b_2+.5)/((float)(e_2-b_2+1.)));
  float pcty=((float)(yy-b_1+.5)/((float)(e_1-b_1+1.)));


  if(!ignore_rev &&reverse1) {pcty=1.-pcty;}
  if(!ignore_rev &&reverse2) {pctx=1.-pctx;}
 // std::cout<<"reverse is"<<reverse1<<reverse2<<" "<<pctx<<" "<<pcty<<std::endl;

    /*if((rev_1 && !ps->get_reverse(iax1)) || (!rev_1 && ps->get_reverse(iax1))){
    pcty=1.-pcty;
  }
  if((rev_2 && !ps->get_reverse(iax2)) || (!rev_2 && ps->get_reverse(iax2))){
     pctx=1.-pctx;
  }
  */
  // fprintf(stderr,"IN GET IMAGE POS2 %d %d %f %f \n",
   // iax1,iax2,pctx,pcty);
  //Now pixels in x,w
  //fprintf(stderr,"pct %f %f \n",pctx,pcty);
  int pix_x=(int)(pctx*((float)(xe-xb)));
  int pix_y=(int)(pcty*((float)(ye-yb)));

  *id2=pix_y;//+yb;
  *id1=pix_x;//+xb;
}
void slice::get_image_pos_pct(float pctx,float pcty,int *id1, int *id2){


  //Now pixels in x,w
  int pix_x=(int)(pctx*((float)(xe-xb)));
  int pix_y=(int)(pcty*((float)(ye-yb)));
  //Now deal with torsion in coordinate system

  int i=pix_y;//+yb;
  int j=pix_x;//+xb;
  
  *id2=(int)(i+j*sx+yb);
  *id1=(int)(j+i*sy+xb);
  //*id1=j;
  //*id2=i;
}
void slice::delete_slice(){
 if(temp_map!=0) delete[] temp_map;
 if(pixel_to_slice_map!=0) delete[] pixel_to_slice_map;
  zero_slice();
}
void slice::zero_slice(){
    is_overlay=false;
    slice_to_grid_map=0;
    pixel_to_slice_map=0;
    temp_map=0;
}
int slice::optimal_with_font(int axis_size,float min, float max, float *onum, float *dnum){
   
  
  int nf,nc;
  float axis_f=(float) axis_size;
  
  QString aa;
  double val;

  float ot,dt;
  int ifirst,ilast;
  for(int i=15; i >2 ; i--){
    nf=this->optimal_scale(i,min,max,&ot,&dt);
    
    ifirst=-1; ilast=-1;
    bool good=false;
    for(int j=0; j < nf; j++){
      val=ot+dt*j;
      int over = 0;
      float loc=(val-min)/(max-min);
      aa.sprintf("%1.5g", val);
      nc = fm->width( aa )/2;
   //   fprintf(stderr,"NC IS %s %d \n",aa.ascii(),nc);
      if(ifirst<0){
        if(loc*axis_f - nc > -axis_size/50.) { //We are less than 2% outside axis frame
           ifirst=j; ilast=j;
           over=(int)(loc*axis_f+nc);
           good=true;
        }
      }
      else{ //We have already found one acceptable point
        if(loc*axis_f-nc > over+axis_f/33.){
          if( loc*axis_f+nc < axis_f) ilast=j;
          over=(int)(loc*axis_f+nc);
        }
        else{
          good=false;
       }
      }
    }
    if(good){
      *onum=ot+dt*ifirst;
      *dnum=dt;
      nf=ilast-ifirst+1;
      return nf;
    }
  }

  return 1;
}

/* Algorithm for internal labeling from Tom Steppe, "Composing
 * well-tempered linear scales", Computer Language, September 1989,
 * 49-65. */
int slice::optimal_scale(int n, float min, float max,float *onum,float *dnum){
    int lo, hi;
    float d, nice;

    d = fabsf(max-min)/n;

    nice = nice_number(d);
    float range=fabs(max-min);
    float delta=.02;

    if (min <= max) {
        lo = (int) ceilf ((min+delta*range)/nice);
        if ((lo-1)*nice+FLT_EPSILON >= min+delta*range) lo--;

        hi = (int) floorf ((max-delta*range)/nice);
        if ((hi+1)*nice <= (max-delta*range)+FLT_EPSILON) hi++;
        *onum = lo * nice;
        *dnum = nice;
    } else {
        lo = (int) ceilf ((max+delta*range)/nice);
        if ((lo-1)*nice+FLT_EPSILON >= max+delta*range) lo--;

        hi = (int) floorf ((min-delta*range)/nice);
        if ((hi+1)*nice <= min+FLT_EPSILON+delta*range) hi++;

        *onum = hi * nice;
        *dnum = -nice;
    }
   

    return (hi-lo+1);
}
/* smallest nice number >= d */
float slice::nice_number (float d){
    float p, nice;
    int i, ie;
    const float set[] = {1.0, 2.0, 5.0};

    ie = (int) floor (log10f(d));
    nice = p = power (10.,ie);

    for (i=1; nice < d; i++) {
        if (i >= 3) {
            i=0;
            p *= 10.;
        }

        nice = set[i]*p;
    }

    return nice;
}
/*
	Finds what power of 10 would best serve for labeling the axes.
*/
float slice::power (float f, int ie){
    float p;

    if (ie < 0) {
        f = 1./f;
        ie = -ie;
    }

    p = (ie & 1)? f: 1.;

    for (ie >>= 1; ie > 0; ie >>=1) {
        f *= f;
        if (ie & 1) p *= f;
    }

    return p;
}
QString slice::string_from_array(int n,int *ar){
   QString st=QString::number(ar[0]);
   for(int i=1; i<n;i++) st=st+":"+QString::number(ar[i]);
  return st;
}
int *slice::array_from_string(QString str){
    int nelem=str.count(":");
    int *buf=new int[nelem+1];
    for(int i=0; i < nelem+1; i++) buf[i]=str.section(":",i,i).toInt();

  return buf;
}
