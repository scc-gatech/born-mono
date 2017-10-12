#include<contour.h>
#include<math.h>
#include<qnamespace.h>
#include <QPolygon>
#include <QPixmap>
#include "my_colors.h"

using namespace SEP;

void contour::draw_slice(QPainter *painter, std::shared_ptr<dataset>dat,QPen *pen, 
 std::shared_ptr<orient_cube>pos,bool ov,bool draw_grid){


  my_dat=dat;
  axes[0]=dat->get_grid_axis(iax1+1);
  axes[1]=dat->get_grid_axis(iax2+1);


   f1=pos->get_beg(iax1);
   f2=pos->get_beg(iax2);

  int n1=pos->get_end(iax1)-pos->get_beg(iax1);
  int n2=pos->get_end(iax2)-pos->get_beg(iax2);
  
  ox=axes[1].o;
  oy=axes[0].o;
  dx=axes[1].d;
  dy=axes[0].d;

   int bb1,bb2,ee1,ee2;
   if((rev_1 && !pos->get_reverse(iax1)) || (!rev_1 && pos->get_reverse(iax1))){
     ee1=f1; bb1=f1+n1; 
   }
   else { bb1=f1; ee1=f1+n1;}
   if((rev_2 && !pos->get_reverse(iax2)) || (!rev_2 && pos->get_reverse(iax2))){
      ee2=f2; bb2=f2+n2; 
 
   }
   else { bb2=f2; ee2=f2+n2;}
   int b1u=f1, b2u=f2, e1u=f1+n1,e2u=f2+n2;
   float *buf=     dat->get_float_data(pos,iax1,b1u,e1u,iax2,b2u,e2u);



  b_1=f1; 
  b_2=f2;
  e_1=b_1+n1;
  e_2=b_2+n2;
  int it;
  //if(rev_1){ it=e_1;e_1=b_1; b_1=it;}
 // if(rev_2){ it=e_2;e_2=b_2; b_2=it;}

 //fprintf(stderr,"CHECK SIZE OF GET %d %d \n",n1,n2);
 Paintcontours(n2, n1, painter, buf,dat->get_minval(),dat->get_maxval());

   painter->setPen(*pen);
   if(axis_top||axis_bot||axis_left||axis_right) this->draw_axes(painter,pos,draw_grid);
    this->draw_pos(painter,pos);
  


}

int contour::cconnect (float *z, int n1, int n2,float  c, int *ix, int *iy, bool *south_mark,
  bool *west_mark,QPolygon *pts)
{
register int    jx, jy;
float           x, y;
int i_x,i_y;
    jx = (*ix);
    jy = (*iy);

    if (south_mark[(jy+1)*n1+jx])  /* if exiting north */
    {
  jy++;
  x = jx + delta (c, z[jy*n1+jx], z[jy*n1+jx + 1]);
  y = jy;
         get_shifted_image_pos_fract(x,y,&i_x,&i_y);
 pts->append(QPoint(i_x,i_y));
  south_mark[jy*n1+jx]=false;
  if (++(*iy) >= n2 - 1)
      return (0);
    }
    else
    if (west_mark[(jy)*n1+jx+1])  /* if exiting east */
    {
  jx++;
  x = jx;
  y = jy + delta (c, z[jy*n1+jx], z[(jy + 1)*n1+jx]);
         get_shifted_image_pos_fract(x,y,&i_x,&i_y);
   pts->append(QPoint(i_x,i_y));
  west_mark[jy*n1+jx]=false;
  if (++(*ix) >= n1 - 1)
      return (0);
    }
    else
    if (south_mark[(jy)*n1+jx])   /* if exiting south */
    {
  x = jx + delta (c, z[jy*n1+jx], z[jy*n1+jx + 1]);
  y = jy;
         get_shifted_image_pos_fract(x,y,&i_x,&i_y);
        // fprintf(stderr,"writing at %f %f (%d %d) \n", x,y,i_x,i_y);
         pts->append(QPoint(i_x,i_y));
  south_mark[jy*n1+jx]=false;
  if (--(*iy) < 0)
      return (0);
    }
    else
    if (west_mark[(jy)*n1+jx])  /* if exiting west */
    {
  x = jx;
  y = jy + delta (c, z[jy*n1+jx], z[(jy + 1)*n1+jx]);
         get_shifted_image_pos_fract(x,y,&i_x,&i_y);
  pts->append(QPoint(i_x,i_y));
  west_mark[jy*n1+jx]=false;
  if (--(*ix) < 0)
      return (0);
    }
    else
  return (0);    /* no exit found */
    return (1);
}
int contour::contour_val ( QPainter *painter,float *z, bool *west_mark, bool *south_mark, int n1, int n2,float c)
{
register int    ix, iy, non;
int             jx, jy;
register float  zxymc, zemc, znmc;
float           x, y;
float pzxy;
 int i_x,i_y;
    /*
     * find all the intersections 
     */
    non = 0;      /* clear intersection counter */
    for (iy = 0; iy < n2 - 1; iy++){
  for (ix = 0; ix < n1 - 1; ix++){
      pzxy = z[iy*n1+ix];
      zxymc = pzxy - c;/* z(x,y) - c */
      zemc = z[iy*n1+ix + 1] - c;  /* (z to the east) - c */
      //fprintf(stderr,"ASKING FOR %d %d %d %d \n",n1,ix,iy,(iy+1)*n1+ix);
      znmc = z[(iy+1)*n1+ix] - c;  /* (z to the north) - c */
#define OPPSIGN(A,B) (1==(((A)<0.)+((B)<0.)))
      if (OPPSIGN (zxymc, znmc))  /* if west edge intersected */
      {
      west_mark[iy*n1+ix]=true;  /* set the west bit */
    non++;    /* and increment counter */
      }
      else    /* else */
        west_mark[iy*n1+ix]=false;;  /* clear the west bit */
      if (OPPSIGN (zxymc, zemc))  /* if south edge intersected */
      {
       south_mark[iy*n1+ix]=true;/* set the south bit */
    non++;    /* and increment counter */
      }
      else    /* else */
    south_mark[iy*n1+ix]=false; /* clear the south bit */
  }
    }
    for (ix = 0, iy = n2 - 1; ix < n1 - 1; ix++)  /* northern boundary */
    {
  pzxy = z[iy*n1+ix];
  zxymc = pzxy - c;  /* z(x,y) - c */
  zemc = z[iy*n1+ix + 1] - c;  /* (z to the east) - c */
  if (OPPSIGN (zxymc, zemc))  /* if south edge intersected */
  {
      south_mark[iy*n1+ix+1]=true;  /* set the south bit */
      non++;    /* and increment counter */
  }
  else      /* else */
      south_mark[iy*n1+ix+1]=false;  /* clear the south bit */
      west_mark[iy*n1+ix+1]=false;    /* clear the west bit */
    }
    for (iy = 0, ix = n1 - 1; iy < n2 - 1; iy++)  /* eastern boundary */
    {
  pzxy = z[iy*n1+ix];
  zxymc = pzxy - c;  /* z(x,y) - c */
  znmc = z[(iy + 1)*n1+ix] - c;  /* (z to the north) - c */
  if (OPPSIGN (zxymc, znmc))  /* if west edge intersected */
  {
      west_mark[(iy + 1)*n1+ix]=true;  /* set the west bit */
      non++;    /* and increment counter */
  }
  else      /* else */
      west_mark[(iy + 1)*n1+ix]=false;  /* clear the west bit */
      south_mark[(iy + 1)*n1+ix]=false;    /* clear the south bit */
    }

    /*
     * draw contours intersecting a boundary 
     */
    for (ix = 0, iy = n2 - 1; ix < n1 - 1 && non > 0; ix++)
  /* north boundary */
    {
  if (south_mark[iy*n1+ix])
  {
      x = ix + delta (c, z[iy*n1+ix], z[iy*n1+ix + 1]);
      y = iy;
        QPolygon polygon;
       get_shifted_image_pos_fract(x,y,&i_x,&i_y);
       polygon.append(QPoint(i_x,i_y));

    //  moveu (x, y, coordinatec);
      south_mark[iy*n1+ix]=false;
      non--;
      jx = ix;
      jy = iy - 1;
     
      while (cconnect (z, n1, n2, c, &jx, &jy,south_mark,west_mark,&polygon))
    non--;
    draw_contour(c,painter,&polygon);
  }
    }
    for (ix = n1 - 1, iy = 0; iy < n2 - 1 && non > 0; iy++)  /* east boundary */
    {
  if (west_mark[iy*n1+ix])
  {
      x = ix;
      y = iy + delta (c, z[iy*n1+ix], z[(iy + 1)*n1+ix]);
      QPolygon polygon;
             get_shifted_image_pos_fract(x,y,&i_x,&i_y);
       polygon.append(QPoint(i_x,i_y));
      west_mark[iy*n1+ix]=false;
      non--;
      jx = ix - 1;
      jy = iy;
      while (cconnect (z, n1, n2, c, &jx, &jy,south_mark,west_mark,&polygon))
    non--;
     draw_contour(c,painter,&polygon);
  }
    }
    for (ix = 0, iy = 0; ix < n1 - 1 && non > 0; ix++)  /* south boundary */
    {
  if (south_mark[iy*n1+ix])
  {
      x = ix + delta (c, z[iy*n1+ix], z[iy*n1+ix + 1]);
      y = iy;
    QPolygon polygon;
           get_shifted_image_pos_fract(x,y,&i_x,&i_y);
       polygon.append(QPoint(i_x,i_y));
      south_mark[iy*n1+ix]=false;
      non--;
      jx = ix;
      jy = iy;
      while (cconnect (z, n1, n2, c, &jx, &jy,south_mark,west_mark,&polygon))
    non--;
     draw_contour(c,painter,&polygon);
  }
    }
    for (ix = 0, iy = 0; iy < n2 - 1 && non > 0; iy++)  /* west boundary */
    {
  if (west_mark[iy*n1+ix])
  {
      x = ix;
      y = iy + delta (c, z[iy*n1+ix], z[(iy + 1)*n1+ix]);
    QPolygon polygon;
           get_shifted_image_pos_fract(x,y,&i_x,&i_y);
       polygon.append(QPoint(i_x,i_y));
      west_mark[iy*n1+ix]=false;
      non--;
      jx = ix;
      jy = iy;
      while (cconnect (z, n1, n2, c, &jx, &jy,south_mark,west_mark,&polygon))
    non--;
     draw_contour(c,painter,&polygon);
  }
    }

    /*
     * draw interior contours 
     */
    for (iy = 0; iy < n2 - 1 && non > 0; iy++)
    {
  for (ix = 0; ix < n1 - 1 && non > 0; ix++)
  {
      if  (south_mark[iy*n1+ix])  /* check south edge of cell */
      {
    x = ix + delta (c, z[iy*n1+ix], z[iy*n1+ix + 1]);
    y = iy;
     QPolygon polygon;
            get_shifted_image_pos_fract(x,y,&i_x,&i_y);
       polygon.append(QPoint(i_x,i_y));
    south_mark[iy*n1+ix]=false;
    non--;    /* clear start */
    jx = ix;
    jy = iy;
    if (cconnect (z, n1, n2, c, &jx, &jy,south_mark,west_mark,&polygon))
        south_mark[iy*n1+ix]=true;/* finish = start */
         draw_contour(c,painter,&polygon);
    while (cconnect (z, n1, n2, c, &jx, &jy,south_mark,west_mark,&polygon))
        non--;
         draw_contour(c,painter,&polygon);
      }
  }
    }
  return(0);
}
float contour::delta (float a, float b, float c){
float           t;

    t = (float)(c - b);      /* avoids pathological comparison */
    if (t != 0.0)
  return ((a - (float) b) / t);
    else
  return ((float) 0.5);
}


void contour::Paintcontours(int n2, int n1, QPainter *painter, float *buf,
  float minv, float maxv){
	 painter->save();

	   painter->translate(bx,by);
	   //Space given per contour
	   
	   bool *west_mark=new bool[n1*n2];
	   bool *south_mark=new bool[n1*n2];
     
     float beg,samp;
     if(c0_set) beg=c0;
     else beg=minv;
     if(dc_set) samp=dc;
     else{
       optimal_scale_linear(num_lines,beg,maxv,&beg,&samp);
     }
 
	   for(int i=0; i < num_lines; i++) {
	      float val=beg+samp*i;
	     // fprintf(stderr,"check samp %f (%f %f) %d\n",val,minv,maxv,num_lines);
	      painter->setPen(contourColor);
	      contour_val(painter,buf,west_mark,south_mark,n1,n2,val);
	   }
	   delete [] west_mark;
	   delete [] south_mark;
	   
	
	
  painter->restore();
}

std::shared_ptr<slice>contour::clone(){

   std::shared_ptr<contour>x(new contour());
   x->contourColor=contourColor;
  x->is_raster=false;
  x->c0_set=c0_set;
  x->dc_set=dc_set;
  x->c0=c0;
  x->dc=dc;
  x->num_lines=num_lines;
  x->is_overlay=is_overlay;
  return x;
}
void contour::draw_contour(float c,QPainter *painter, QPolygon *poly){

 // painter->drawPolyline(*poly);
  int ix,iy;
  QPolygon poly1,poly2;
  QString val=QString::number(c);
  bool found=false;
  int i=0;
  int pw=  fm->width(val);
  float rad;
  int iye,ixe;
  int cen=(poly->size())/2;
  while(!found && i <poly->size()-1){
    //Scramble location
    int half=i/2;
    int dir;
    if(i%2==0) dir=1;
    else dir=-1;
    int ic=cen-half*dir;
  
  
     poly->point(ic,&ix,&iy);
     //Now attempt to find an acceptable end point
     bool valid=false;
     int j=ic+dir;
     int ix_b=0,ix_e=0,iy_b=0,iy_e=0;
     while(!valid && j< poly->size() && j>=0){
       poly->point(j,&ixe,&iye);
       if(ixe!=ix){
         if(ix < ixe) { ix_b=ix; ix_e=ixe; iy_b=iy; iy_e=iye;}
         else { ix_b=ixe; ix_e=ix; iy_b=iye; iy_e=iy;}
         float dist=sqrtf((float)((iy-iye)*(iy-iye)+(ix-ixe)*(ix-ixe)));
         if(dist > pw*1.1) valid=true;
       }
       j+=dir;
     }
     if(valid){
         rad=atan2((float)(iy_e-iy_b),(float)(ix_e-ix_b));
         box test=box(ix_b,iy_b,val,rad,fm);
         if(test.bx >xb && test.ex < xe && test.by > yb && test.ey < ye){
          bool check=true;
         for(int ib=0; ib< (int)boxes.size(); ib++){
           if(boxes[ib].boxes_overlap(&test)) check=false;
         }
         if(check){
          // int ph=  fm->height()/2;
           painter->translate(ix_b-test.shiftx,iy_b-test.shifty);
           painter->rotate(rad*45./atan(1.));
           painter->drawText(0,0,val);
           painter->rotate(-rad*45./atan(1.));
           painter->translate(-ix_b+test.shiftx,-iy_b+test.shifty);
           boxes.push_back(test);
           found=true;
           int div1,div2;
           if(j>ic) { div1=ic; div2=j;}
           else { div1=j; div2=ic;}
           for(int k=0; k<div1; k++) poly1.append(QPoint(poly->point(k)));
           for(int k=div2; k < poly->size();k++) poly2.append(QPoint(poly->point(k)));
           painter->drawPolyline(poly1);
           painter->drawPolyline(poly2);

         }
       }
     }
     i++;
   }
   if(!found)  painter->drawPolyline(*poly);
     

}

void contour::get_shifted_image_pos_fract(float fx, float fy,int *id1, int *id2){

  float pctx=((float)(fy+f2-b_2)/((float)(e_2-b_2)));
  float pcty=((float)(fx+f1-b_1)/((float)(e_1-b_1)));
  //Now pixels in x,w
  int pix_x=(int)(pctx*((float)(xe-xb)));
  int pix_y=(int)(pcty*((float)(ye-yb)));
  //Now deal with torsion in coordinate system
  int i2=pix_y+yb;
  int i1=pix_x+xb;
  *id1=(int)(i1+i2*sy+bx);
  *id2=(int)(i2+i1*sx+by);
  *id1=i1+(int)i2*sy;
  *id2=i2+(int)i1*sx;
  
}
  
  void contour::set_contour_color(QString color){
    my_colors x=my_colors();
    contourColor=x.return_qcolor(color);
    my_color=color;
  }
  QString contour::return_contour_color(){ return my_color;}
  
  bool box::boxes_overlap(box *other){
    if(by > other->ey) return false;
    if(other->by > ey) return false;
    if(other->bx > ex) return false;
    if(bx >other->ex)  return false;
    return true;

  }
 box::box(int ix, int iy, QString txt,  float ang,std::shared_ptr<QFontMetrics >fm){
    
   int pw = fm->width( txt);
     int ph=  fm->height()/2;
     shiftx=(int)(ph*fabs((sin(ang)))/2.);
     shifty=(int)(ph*fabs((cos(ang)))/2.);
     shiftx=shifty=0;
     int len=pw*fabs(cos(ang))+ph*abs((sin(ang)))*1.2+ph*1.5;
     int het=pw*fabs(sin(ang))+ph*abs((cos(ang)))*1.2+ph*1.5;
  //  fprintf(stderr,"sammy %s %f %d %d \n",txt.ascii(),ang*45./atan(1.),len,het);
    bx=ix-shifty;
    ex=ix+shifty+len;
    by=iy-shifty;
    ey=iy+shifty+het;
  }
  
