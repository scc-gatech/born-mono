#include<slice.h>
#include<math.h>
#include<qnamespace.h>
#include <QPolygon>
#include <QPixmap>
using namespace SEP;

/*
	Sets the parameters for drawing the slice.
*/
void slice::set_draw_params(std::shared_ptr<QFontMetrics >f_m,std::shared_ptr<orient_cube>pos, int i1, int i2, int off,int b_x, int e_x, int b_y, int e_y,
int dopos,bool at,bool ab, bool al, bool ar,float s1,float s2, bool rev1, bool rev2, bool verb){

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
 int ia[8];
offset=off;
  int idat1=pos->get_order(i1), idat2=pos->get_order(i2);
   pos->get_begs(ia);
   pos->get_ends(ia);
   pos->getAxis_range(idat1,&b1_grid,&e1_grid);
      pos->getAxis_range(idat2,&b2_grid,&e2_grid);





  axis_left=al;
  axis_right=ar;
  axis_top=at;
  axis_bot=ab;
  boundary=fm->height()*2;
   if(axis_top) {
     by+=boundary;
     ye-=boundary;
   }
   if(axis_bot) {
     ye-=boundary;
          ey-=boundary;
   }
   if(axis_left){
     bx+=boundary;
     xe-=boundary;
   }
   if(axis_right)   xe-=boundary;
   

  my_pos=pos;

  iax1=idat1;
  iax2=idat2;
  int f2,e2;
 pos->getAxis_range(iax1,&f1,&e1); n1_slice=e1-f1;
 pos->getAxis_range(iax2,&f2,&e2); n2_slice=e2-f2;
 

  ps=pos;
  do_pos =dopos;
  
  reverse1=((pos->get_reverse(iax1) || rev1)&&(rev1!=pos->get_reverse(iax1)));
  reverse2=((pos->get_reverse(iax2) || rev2)&&(rev2!=pos->get_reverse(iax2)));
 
  ps->form_index_map(iax1,iax2,reverse1,reverse2);
  form_pixel_slice_map();
  int ilocs[8];
  ps->get_locs(ilocs);
  slice_to_grid_map=ps->get_index_map_ptr(iax1,iax2,f1,e1,f2,e2,offset);
  int order[8];
  ps->get_orders(order);

  
  if((iax1==order[0]  && iax2==order[1]) || (iax2==order[0] && iax1==order[1])) i3_axis=order[2];
  if((iax1==order[0]  && iax2==order[2]) || (iax2==order[0] && iax1==order[2])) i3_axis=order[1];
  if((iax1==order[2]  && iax2==order[1]) || (iax2==order[2] && iax1==order[1])) i3_axis=order[0];





  
  
}
void slice::form_pixel_slice_map(){

/*
       
   int i_x=(int)(-(iy-by)*sy+ix-bx);
   int i_y=(int)(-(ix-bx)*sx+iy-by);
   */

int n1,n2;
  my_pos->getAxis_range(iax1,&b_1,&e_1); n1=e_1-b_1;
  my_pos->getAxis_range(iax2,&b_2,&e_2); n2=e_2-b_2;
//int *map=new int[xe*ye*2];
//for(int i=0; i < 2*xe*ye; i++) map[i]=-1;
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
   
   //  map[i]=iy;
    // map[i+xe*ye]=ix;
     // fprintf(stderr,"writing map %f %f %d %d %d %d \n",pctx,pcty,ix,iy,map[i],map[i+xe*ye]);
    }
}


//return map;
}
int *slice::get_pixel_to_slice_map(int *n1, int *n2){
   *n1=ye;
   *n2=xe;
   return pixel_to_slice_map;
}
long long *slice::get_slice_to_grid_map(){

   return slice_to_grid_map;

}
std::shared_ptr<pairs_new>slice::return_pick_locs(QString col,std::shared_ptr<picks_vec>pk ){
  std::shared_ptr<pairs_new>pout( new pairs_new());
  for(int i=0; i < pk->return_size(); i++){
    int ix,iy;
    std::shared_ptr<pick_new >p=pk->return_pick(i);
    if(col==p->col){
      bool inside=get_shifted_image_pos_from_sample(p->iloc[iax2],p->iloc[iax1],&ix,&iy);
      if(inside) pout->add_point(ix,iy,p->txt);
    }
  }
  return pout;

}
long long *slice::get_index_map_ptr(int idelta){
  int f2,e2,f1,e1;
  ps->getAxis_range(iax1,&f1,&e1); 
  ps->getAxis_range(iax2,&f2,&e2); 
  return ps->get_index_map_ptr(iax1,iax2,f1,e1,f2,e2,idelta);
}
void slice::draw_pos(QPainter *painter, std::shared_ptr<orient_cube>pos){
  int p_1,p_2,ix1,ix2,iy1,iy2;
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
      float f=b2_grid-.49;
      p_1=pos->get_loc(iax1); 
      p_2=pos->get_loc(iax2);
      
      
      while(f<=e2_grid){
        get_shifted_image_pos_from_sam_fract(f,p_1*1.,&x1,&y1);
        pointarray.putPoints(ic, 1,x1,y1);
        ic++;
        f+=1;
     }

             get_shifted_image_pos_from_sam_fract(e2_grid,p_1*1.,&x1,&y1);
        pointarray.putPoints(ic, 1,x1,y1);

    painter->drawPolyline(pointarray);
     pointarray.clear();
     ic=0;
     f=b1_grid-.49;
     while(f<=e1_grid){

        get_shifted_image_pos_from_sam_fract(p_2*1.,f,&x1,&y1);
        pointarray.putPoints(ic, 1,x1,y1);
        ic++;
        f+=1;
     }

     get_shifted_image_pos_from_sam_fract(p_2*1.,e1_grid,&x1,&y1);
        pointarray.putPoints(ic, 1,x1,y1);
      painter->drawPolyline(pointarray);
    
     //Now lets just draw two lines that go through the center point to the edges
     get_shifted_image_pos_from_sam_fract(p_2*1.,p_1*1.,&x1,&y1);
   if(fabs(sx)<0.001 && fabs(sy)<0.0001){
     painter->drawLine(bx,y1,ex,y1);
     painter->drawLine(x1,by,x1,ey);
    }
    
}

}  

/*
	Draws the axes for the graph.
*/
void slice::get_shifted_axis_pos1( bool left, float pct,int *ix, int *iy){
  if(left){
   *iy=by+(int)(ye*pct);
   *ix=bx+(int)((float)ye*pct*(float)sy);
   return;
  }
  *iy=by+(int)(ye*pct)+xe*sx;
  *ix=ex+(int)((float)ye*pct*(float)sy);
}
//Axis 2 is x
void slice::get_shifted_axis_pos2( bool top, float pct,int *ix, int *iy){
  if(top){
    *ix=bx+(int)(pct*xe);
    *iy=by+(int)((float)xe*pct*(float)sx);
    return;
  }
    *ix=bx+(int)(pct*xe)+ye*sy;
    *iy=ey+(int)((float)xe*pct*(float)sx);
}

void slice::draw_axes(QPainter *painter,std::shared_ptr<orient_cube>pos,bool draw_grid){
  int tic_len=fm->height()/3;
  int npixels=20; //Distance between tics (approx)
  axis ax1=pos->getAxis(iax1);
  axis ax2=pos->getAxis(iax2);
  int xs,ys;
   int f,e;
   pos->getAxis_range(iax1,&f,&e); 
   float ov1=pos->get_oversamp(iax1),ov2=pos->get_oversamp(iax2);

  float mn=f*ax1.d/ov1+ax1.o;
  float mx=(e-1)*ax1.d/ov1+ax1.o;
  float o1,d1,o2,d2;
  o1=ax1.o;
  d1=ax1.d;
  if(is_overlay) { 
    return;
  }
  int display1=my_dat->return_axes_display(iax1),display2=my_dat->return_axes_display(iax2);
  std::vector<float> ax_val1,ax_val2;
  if(display1>=0){
    std::vector<long long> locs;
    for(int i=0; i< n1_slice; i++){
      locs.push_back(slice_to_grid_map[i]);
      }
    ax_val1=my_dat->return_axis_value(iax1,locs);
  }
  if(display2>=0){
    std::vector<long long> locs;
    for(int i=0; i < n2_slice; i++){
      locs.push_back(slice_to_grid_map[i*n1_slice]);

      }
    ax_val2=my_dat->return_axis_value(iax2,locs);
  }

  o2=ax2.o;
  d2=ax2.d;

 // float fpos;
  int ipos;
  int ix,iy,ix2,iy2;
  std::vector<float> tic_loc1,tic_loc2;
  std::vector<float> tic_val1,tic_val2;
  if(display1 <0)
    tic_loc1=this->optimal_with_font(ye-yb,mn-ax1.d/2.,mx+ax1.d/2.);
  else
    tic_loc1=this->optimal_with_font(ye-yb,mn,mx,ax_val1,&tic_val1);
    
  
//HERE WE GO
  QString aa;
  int ph,pw;
  
     int eleft=e_2,bleft=b_2;
      if(reverse2){
      eleft=b_2;
      bleft=e_2;
    }
  if(reverse1 && display1>=0) {
     std::vector<float> junk; for(int i=0; i < (int) tic_loc1.size(); i++) junk.push_back(tic_loc1[i]);
     for(int i=0;i<(int)junk.size();i++) tic_loc1[tic_loc1.size()-1-i]=junk[i];
     junk.clear(); for(int i=0; i < (int) tic_val1.size(); i++) junk.push_back(tic_val1[i]);
     for(int i=0;i<(int)junk.size();i++) tic_val1[tic_val1.size()-1-i]=junk[i];
  }

  float rad=atan(1.0*sy);
  float ang=atan(sy)*180./3.1415926536;
  
  int pos_old=-9999;
  if(reverse1) pos_old=9999;
  int delta= 10; 
  int dir=1;
  if(tic_loc1.size()>1){
    if(tic_loc1[1]<tic_loc1[0]){ dir=-1;}
  }
  for(int i=0; i < (int)tic_loc1.size(); i++){
         float ffpos=(tic_loc1[i]-mn)/(mx-mn);
         if(reverse1) ffpos=1.-ffpos;
       ipos=(int)(ffpos+.5);
//       ipos=(int)((tic_loc1[i]-o1)/d1);
       double val=tic_loc1[i];

       if(display1>=0)    aa.sprintf("%1.5g", tic_val1[i]);
       else aa.sprintf("%1.5g",val);
       pw = fm->width( aa );
       ph=  fm->height();

  if(draw_grid){
     get_shifted_axis_pos1(true,ffpos,&ix,&iy);
     get_shifted_axis_pos1(false,ffpos,&ix2,&iy2);
//     get_shifted_image_pos_from_sam_fract((float)bleft,ffpos,&ix,&iy);
 //    get_shifted_image_pos_from_sam_fract((float)eleft,ffpos,&ix2,&iy2);
     painter->drawLine(ix,iy,ix2,iy2);
    
    }

       if(axis_left) {
       int space=std::max(2,(int)(tic_len*cos(rad)));
       get_shifted_axis_pos1(true,ffpos,&ix,&iy);

  //       get_shifted_image_pos_from_sam_fract((float)bleft,ffpos,&ix,&iy);
         
         
         painter->drawLine(ix,iy,ix-(int)(tic_len*cos(rad)),iy+(int)(tic_len*sin(rad)));
         if(ang>= -.0001){
         ys=(int)(cos(rad)*pw/2.-sin(rad)*(ph/2+2*tic_len));
         xs=(int)(cos(rad)*(2*tic_len+ph/2)+sin(rad)*pw/2.);
         
         painter->translate(ix-(int)(tic_len*cos(rad))-space,iy+ys);
         painter->rotate(-90-ang);
//        if((dir==1&&pos_old<iy-pw/2) || pos_old > iy+pw/2 ){

        if(pos_old<iy-pw/2 && !reverse1){
          painter->drawText(0,0,aa);
            pos_old=iy+pw/2+delta;
        }
        else if(pos_old >iy+pw/2 && reverse1){
                 painter->drawText(0,0,aa);
            pos_old=iy-pw/2+delta;
        
        }
         painter->rotate(90+ang);
         painter->translate(-(ix-(int)(tic_len*cos(rad))-space),-iy-ys);


         }
         
         else{

           ys=(int)(cos(rad)*pw/2.-sin(rad)*(space+tic_len));
           xs=(int)(cos(rad)*(tic_len+space)-sin(rad)*pw/2.);
          // xs=0;
           ys=0;
        
           painter->translate(ix-xs,iy-ys);
           painter->rotate(-90-ang);
           if(pos_old<iy-pw/2&& !reverse1){
             painter->drawText(0,0,aa);
             pos_old=iy+pw/2+delta;
           }
          else if(pos_old >iy+pw/2 && reverse1){
            painter->drawText(0,0,aa);
            pos_old=iy-pw/2+delta;
          }

        painter->rotate(90+ang);
         painter->translate(-ix+xs,-iy+ys);
         }
         }

  

    }
   
   
    //aa=ax1.label.c_str();
    aa=ax1.label.c_str();
    if(display1 >=0) aa=my_dat->return_axis_name(iax1);
    pw = fm->width( aa ); 
    ph=  fm->height();
    
    


    if(axis_left){   
         get_shifted_axis_pos1(true,.5,&ix,&iy);
         if(ang>= -.0001){

         ys=(int)(sin(rad)*(ph+2.3*tic_len));
         xs=(int)(cos(rad)*(2.*tic_len+ph));
         painter->translate(ix-xs,iy-ys);
         painter->rotate(-90-ang);
          painter->drawText(0,0,aa);
         painter->rotate(90+ang);
         painter->translate(-ix+xs,-iy+ys);
         }
         else{
         ys=cos(rad)*pw/2.-sin(rad)*(-ph-2.3*tic_len);
         xs=cos(rad)*(-2.3*tic_len-ph)+sin(rad)*pw/2.;
         painter->translate(ix+xs,iy+ys);
         painter->rotate(270-ang);
          painter->drawText(0,0,aa);

         painter->rotate(-270+ang);
         painter->translate(-ix-xs,-iy-ys);
         
         }
         
   }



  ang=atan(sx)*180./3.1415926536;
  rad=atan(sx);
   int ff,ee;
   pos->getAxis_range(iax2,&ff,&ee); 
   mn=ff*ax2.d/ov2+ax2.o;
   mx=(ee-1)*ax2.d/ov2+ax2.o;


  if(display2<0) tic_loc2=this->optimal_with_font(xe-xb,mn-ax2.d/2.,mx+ax2.d/2.);
  else tic_loc2=this->optimal_with_font(xe-xb,mn,mx,ax_val2,&tic_val2);

   // fprintf(stderr,"I GET BACK2 min=%f max=%f otic=%f dtic=%f \n",mn,mx,otic1,dtic1);
    float ffpos;

    
    if(reverse2 && display2 >=0 ) {
     std::vector<float> junk; for(int i=0; i < (int) tic_loc2.size(); i++) junk.push_back(tic_loc2[i]);
     for(int i=0;i<(int)junk.size();i++) tic_loc2[tic_loc2.size()-1-i]=junk[i];
     junk.clear(); for(int i=0; i < (int) tic_val2.size(); i++) junk.push_back(tic_val2[i]);
     for(int i=0;i<(int)junk.size();i++) tic_val2[tic_val2.size()-1-i]=junk[i];
    }

    pos_old=-9999;
    if(reverse2) pos_old=9999;
    for(int i=0; i < (int)tic_loc2.size(); i++){
     //ffpos=(tic_loc2[i]-ax2.o)/ax2.d;
       //ipos=(otic2+dtic2*i-ax2.o)/ax2.d;
       ffpos=(tic_loc2[i]-mn)/(mx-mn);
       if(reverse2) ffpos=1.-ffpos;
       ipos=(int)(ffpos+.5);

       double val=tic_loc2[i];

       if(display2>=0) aa.sprintf("%1.5g", tic_val2[i]);
       else aa.sprintf("%1.5g",val);
       pw = fm->width( aa );
       ph=  fm->height()/2;

       if(draw_grid){
          get_shifted_axis_pos2(true,ffpos,&ix,&iy);
          get_shifted_axis_pos2(false,ffpos,&ix2,&iy2);
          painter->drawLine(ix,iy,ix2,iy2);  
       }

      if(axis_bot==true){  
         get_shifted_axis_pos2(false,ffpos,&ix,&iy);
         painter->drawLine(ix,iy,(int)((float)ix-1.0*tic_len*sin(rad)),(int)((float)iy+1.0*tic_len*cos(rad)));
         xs=(int)(cos(rad)*pw/2.+sin(rad)*(ph+2*tic_len));
         ys=(int)(cos(rad)*(2*tic_len+ph)-sin(rad)*pw/2.);
         painter->translate(ix-xs,iy+ys);
         painter->rotate(ang);
        if(pos_old<ix-pw/2 && !reverse2){
          painter->drawText(0,0,aa);
          pos_old=ix+pw/2+delta;
        }
        else if(pos_old>ix+pw/2 &&reverse2){
          painter->drawText(0,0,aa);
          pos_old=ix-pw/2+delta;
        }
              //  else fprintf(stderr,"Didn't plot because if conditional2 \n");

        painter->rotate(-ang);
        painter->translate(-ix+xs,-iy-ys);
      }
    }

    aa=ax2.label.c_str();
    if(display2>=0) aa=my_dat->return_axis_name(iax2);

    pw = fm->width( aa ); 
    ph=  fm->height();

    if(axis_bot){
      get_shifted_axis_pos2(false,.5,&ix,&iy);
      xs=(int)(cos(rad)*pw/2.+sin(rad)*(ph+2.5*tic_len));
      ys=(int)(cos(rad)*(3*tic_len+ph)-sin(rad)*pw/2.);
      painter->translate(ix-xs,iy+ys);painter->rotate(ang);
      painter->drawText(0,0,aa);
      painter->rotate(-ang);
      painter->translate(-ix+xs,-iy-ys);

    }

}
bool slice::in_slice(int ix, int iy){
  bool ret=false;
     int i_x=-(int)((iy-by)*sy)+ix-bx;
   int i_y=-(int)((ix-bx)*sx)+iy-by;

   if(i_x >= xb && i_x <= xe && i_y >= yb && i_y <=ye) ret=true;
   return ret;
}
void slice::update_pos(std::vector<QString> *com,int ix,int iy, std::shared_ptr<orient_cube>pos){
   int loc[8];
   std::vector<QString> com2;
  com->at(1)="none";
  if(this->in_slice(ix,iy)==true){
     int id1,id2;
     pos->get_locs(loc);

     this->get_data_loc(ix,iy,loc);
     //This is tricky. This is my current position in the unrotated coordinate system. 
    //If I am rotated I really want my rotated position?
    pos->rotate_loc(loc);

     com->at(1)="navigate";
     com->at(2)="move";
     com->at(3)=string_from_array(8,loc);

     
  }
}
std::vector<long long> slice::get_multi_locs(int ix_old,int iy_old, int ix, int iy){
  

  //Return the point we cliccked on 
 
  int  n1;
  //CHECK CHECK CHECK
  my_pos->getAxis_range(iax1,&b_1,&e_1); n1=e_1-b_1;
  std::vector<long long > locs;
  if(this->in_slice(ix,iy)==true){
     int i_x=(int)(-(iy-by)*sy+ix-bx);
     int i_y=(int)(-(ix-bx)*sx+iy-by);
     int i_x_old=(int)(-(iy_old-by)*sy+ix_old-bx);
     int i_y_old=(int)(-(ix_old-bx)*sx+iy_old-by);
     int ip1=pixel_to_slice_map[i_y+i_x*ye];
     int ip2=pixel_to_slice_map[i_y_old+i_x_old*ye];
     int ip1_1=ip1%n1;
     int ip1_2=ip1/n1;
     int ip2_1=ip2%n1, ip2_2=ip2/n1;
     int  beg_1=std::min(ip1_1,ip2_1), end_1=std::max(ip1_1,ip2_1);
     int  beg_2=std::min(ip1_2,ip2_2), end_2=std::max(ip1_2,ip2_2);
     for(int i2=beg_2; i2 < end_2; i2++){
       for(int i1=beg_1; i1 < end_1; i1++){
         locs.push_back(slice_to_grid_map[i1+i2*n1]);
       }
    }
  }
      return locs;  
}
void slice::update_range(std::vector<QString> *com, int ix_old,int iy_old, int ix, int iy, std::shared_ptr<orient_cube>pos){
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

void slice::get_data_pos(int ix,int iy,float *pos){
 int iloc[8];
  get_data_loc(ix,iy,iloc);
  axis a1=my_pos->getAxis(iax1),a2=my_pos->getAxis(iax2);
  pos[0]=a1.o+a1.d*iloc[iax1]; 
  pos[1]=a2.o+a2.d*iloc[iax2];

}
void slice::get_slice_axes(std::shared_ptr<orient_cube>pos, int *i1, int *i2, int *i3){
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

bool slice::get_image_pos_sample(float ix, float iy,int *id1, int *id2){

  int loc[8];
  memcpy(loc,my_pos->loc,sizeof(int)*8);
  //IO_loc assumes an integer so we are going to round to the nearest sample and then
  //redo the fraction
  int iix=(int)(ix+.5), iiy=(int)(iy+.5);
  float fx=ix-iix, fy=iy-iiy;
  //fprintf(stderr,"before loc chainge %f %d\n",ix,iix);
  loc[iax2]=iix; loc[iax1]=iiy;
  // my_pos->rotate_loc(loc);
//  fprintf(stderr,"after loc change %d %d \n",loc[iax1],loc[iax2]);
  //fprintf(stderr,"lock int %d %d \n",loc[iax1],loc[iax2]);  
  my_dat->get_io_loc(loc);
 // fprintf(stderr,"l2ock int %d %d %f %f %d %d %d %d\n",loc[iax1],loc[iax2],fx,fy,
  //b1_grid,e1_grid,b2_grid,e2_grid);  

  float pctx=((float)(loc[iax2]+fx-b2_grid)/((float)(e2_grid-b2_grid)));
  float pcty=((float)(loc[iax1]+fy-b1_grid)/((float)(e1_grid-b1_grid)));
  
 // fprintf(stderr,"pct x %d %d %d %d \n",b2_grid,e2_grid,fx,loc[iax2]);
  if(reverse1) pcty=1.-pcty;
  if(reverse2) pctx=1.-pctx;

  float pct[2]; pct[0]=pcty; pct[1]=pctx;
  //fprintf(stderr,"the pcts %f %f \n",pct[0],pct[1]);
  float delta1=1./(float)abs(e1_grid-b1_grid);
  float delta2=1./(float)abs(e2_grid-b2_grid);
//  fprintf(stderr,"pct in %f %f %f %f \n",pct[0],pct[1],delta1,delta2);
    if(pct[1]<-delta2){
   //   fprintf(stderr,"failed two? %f %f \n",pct[1],-delta2);
    }
 if(pct[0]<-delta1 || pct[1]<-delta2 || pct[0]>delta1+1. || pct[1]>delta2+1.) {
 return false;
 }
 pct[0]=std::max(0.f,std::min(.99999f,pct[0]));
  pct[1]=std::max(0.f,std::min(.99999f,pct[1]));
  ps->shift_data_image(iax1,iax2,pct);

 // fprintf(stderr,"pct out %f %f \n",pct[0],pct[1]);
  //We want to get rid of this
 // pct[0]=std::max(std::min(1.0f,pct[0]),0.f);
 // pct[1]=std::max(std::min(1.0f,pct[1]),0.f);

  int pix_x=(int)(pct[1]*((float)(xe-xb)));
  int pix_y=(int)(pct[0]*((float)(ye-yb)));
  
  *id2=pix_y+yb;
  *id1=pix_x+xb;
 // fprintf(stderr,"x %d %d %d y %d %d %d \n",xb,*id1,xe,yb,*id2,ye);
  assert(*id1 >= xb && *id1 <=xe && *id2>=yb && *id2<=ye);
  return true;
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
 //if(temp_map!=0) delete[] temp_map;
 if(pixel_to_slice_map!=0) delete[] pixel_to_slice_map;
  zero_slice();
}
void slice::zero_slice(){
    is_overlay=false;
    slice_to_grid_map=0;
    pixel_to_slice_map=0;
   // temp_map=0;
}

std::vector<float> slice::optimal_with_font(int axis_size, int mn,int mx,std::vector<float> vals,
   std::vector<float> *pvals){
  float delta=(float) axis_size/(float) vals.size();
  
  std::vector<float> pout;
  QString aa;
  aa.sprintf("%1.5g",vals[vals.size()/2]); 
  int nc=fm->width(aa); //Width of a typical?? value
  int nguess=axis_size/(nc+5);//Approximate number of samples we could display
  int j=std::max(1,(int)(vals.size()/nguess));
  
  int cur_pos=(int)(delta/2.); //Current position on axis
  int isamp=0;  //Current sample testing
  int left_check=0;
  int bound=5; //Minimum pixel count between samples displayed
  while(isamp<(int)vals.size()){
    aa.sprintf("%1.5g",vals[isamp]); 
    int n=fm->width(aa);
    if(cur_pos-n/2 <left_check){
      isamp++;
      cur_pos=(int)((float)cur_pos+delta);
    }
    else if(cur_pos + n/2 > axis_size){
      return pout;
    }
    else{
      pout.push_back(1.*isamp/vals.size()*(mx-mn)+mn);
      pvals->push_back(vals[isamp]);
      isamp+=j;
      cur_pos+=(int)(j*delta);
      left_check=cur_pos+n/2+bound;
    }
  }
  return pout; 


}
//}
std::vector<float> slice::optimal_with_font(int axis_size,float min, float max){
   
  
  if(min==max) max=min+1;
    int nf,nc;
  float axis_f=(float) axis_size;
 
  QString aa;
  double val;

  float ot,dt;
  int ifirst,ilast;
  for(int i=15; i >2 ; i--){
    nf=this->optimal_scale_linear(i,min,max,&ot,&dt);
  
    ifirst=-1; ilast=-1;
    bool good=false;
    for(int j=0; j < nf; j++){
      val=ot+dt*j;
      int over;
      float loc=(val-min)/(max-min);
      aa.sprintf("%1.5g", val);
      nc = fm->width( aa )/2;
   //   fprintf(stderr,"NC IS %s %d \n",aa.ascii(),nc);
      if(ifirst<0){
        if(loc*axis_f - nc > -axis_size/50.) { //We are less than 2% outside axis frame
           ifirst=j; ilast=j;
           over=loc*axis_f+nc;
           good=true;
        }
      }
      else{ //We have already found one acceptable point
        if(loc*axis_f-nc > over+axis_f/33.){
          if( loc*axis_f+nc < axis_f) ilast=j;
          over=loc*axis_f+nc;
        }
        else{
          good=false;
       }
      }
    }
    if(good){
      std::vector<float> x;
      for(int i=0; i < ilast-ifirst+1;i++){
        x.push_back(ot+dt*ifirst+dt*i);
      }
   //   onum=ot+dt*ifirst;
   //   dnum=dt;
    //  nf=ilast-ifirst+1;
      return x;
    }
  }
  std::vector<float> x;
  return x;
}

/* Algorithm for internal labeling from Tom Steppe, "Composing
 * well-tempered linear scales", Computer Language, September 1989,
 * 49-65. */
int slice::optimal_scale_linear(int n, float min, float max,float *onum,float *dnum){
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
