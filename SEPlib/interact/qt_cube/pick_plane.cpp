#include "pick_plane.h"
#include "pairs.h"

pick_plane::pick_plane(std::map<QString, picks*> *all,int i1,int i2){

 
  std::map<QString, picks*>::const_iterator i;
  for ( i=all->begin();
  i != all->end(); 
  ++i){
    maps[i->first]=new pickset((i->second));
  }
  iax1=i1; iax2=i2;
}

void pick_plane::draw_line(pairs *points, QPainter *painter, orient_cube *pos, slice *slc, 
int sz, int idist){
  
  
  if(pos==0);
  QPolygon pointarray,pointarray2;
  for(int i=0; i < (int)points->size(); i++){
    int ix,iy,bx,by,ex,ey;
    slc->get_shifted_image_pos_from_loc(points->vals[i].y, points->vals[i].x,&ix,&iy,true);
    pointarray.putPoints(i,1,ix,iy);
    bx=ix-sz; by=iy-sz; ex=ix+sz; ey=iy+sz;
    if(points->vals[i].type!=1){
  	  pointarray2.putPoints(0, 5,bx,by,bx,ey,ex,ey,ex,by,bx,by);
		  if(idist==0) painter->drawPolyline(pointarray2);
		}
		else  if(idist==0)
	   painter->drawRect(ix,iy,sz*2,sz*2);
    }
 	painter->drawPolyline(pointarray);
}
void pick_plane::draw_points(pairs *points, QPainter *painter,orient_cube *pos, slice *slc, int sz){
    QPolygon pointarray;

 if(pos==0);
  QVector<QRectF> rects;
  for(int i=0; i < (int)points->size(); i++){
    int ix,iy,bx,ex,by,ey;
    slc->get_shifted_image_pos_from_loc(points->vals[i].y, points->vals[i].x,&ix,&iy,false);
   // fprintf(stderr,"TEST ME %d %d %d %d \n",ix,iy,slc->iax1,slc->iax2);
  //  fprintf(stderr,"in draw pos %d %d \n",ix,iy);
    if(points->vals[i].type!=1 ){
      bx=ix-sz; by=iy-sz; ex=ix+sz; ey=iy+sz;
  	  pointarray.putPoints(0, 5,bx,by,bx,ey,ex,ey,ex,by,bx,by);
		  painter->drawPolyline(pointarray);
		}
	
	  else {
	   // painter->drawRect(ix,iy,sz*2,sz*2);
	    rects.push_back(QRectF(ix,iy,sz*2,sz*2));
	   }
	}

 painter->drawRects(rects);
}
void pick_plane::draw_extra(pairs *points, QPainter *painter,orient_cube *pos, slice *slc, int sz){
    QPolygon pointarray;

 if(pos==0);

 
  QVector<QRectF> rects;
  for(int i=0; i < (int)points->size(); i++){
    int ix,iy,bx,ex,by,ey;
    slc->get_shifted_image_pos_from_loc(points->vals[i].y, points->vals[i].x,&ix,&iy,true);
  //  fprintf(stderr,"in draw pos %d %d \n",ix,iy);
  //  if(points->vals[i].type!=1){
       painter->drawText(ix,iy,QString::number(points->vals[i].extra));


	//	}
	
	
	}

 painter->drawRects(rects);
}
pairs pick_plane::get_pts_sort(int iax1, int iax2, float *o, float *d,int ival,int isort, 
QString col,orient_cube *pos){
  return maps[col]->get_pts_sort(pos,iax1,iax2,o,d,ival,isort);
}
pairs pick_plane::get_pts_sort_le(int iax1, int iax2, float *o, float *d,int ival,int isort, 
QString col,orient_cube *pos){
  return maps[col]->get_pts_sort_le(pos,iax1,iax2,o,d,ival,isort);
}
void pick_plane::draw(QPainter *painter, int ndist,std::vector<pick_plane*>*neighbors,
      std::vector<int> *dist,orient_cube *pos,slice *slc,QString draw_what){

  int iax1,iax2,iax3;
  slc->get_slice_axes(pos,&iax1,&iax2,&iax3);
 
  int beg1=pos->get_beg(iax1), beg2=pos->get_beg(iax2);
  int end1=pos->get_end(iax1), end2=pos->get_end(iax2);
  
  axis ax1=pos->get_axis(iax1);
  axis ax2=pos->get_axis(iax2);
  std::map<QString, pickset*>::const_iterator i;
  for ( i=maps.begin(); i != maps.end(); ++i){
    int  draw_type=i->second->get_display();

    if((draw_what=="all" || draw_what==i->first) ){
       if(iax1!=i->second->get_single() && iax2!=i->second->get_single()
          && draw_type==DISPLAY_LINE) draw_type=DISPLAY_POINTS;
    //   fprintf(stderr,"DRAW_TYPE IS %d \n",draw_type);
       QPen pen;
       int sz=i->second->get_sz();
       pen.setWidth(sz);
	     QColor c=i->second->get_color();
	     c.setAlphaF(1.);
	     pen.setColor(c); 
	     painter->setBrush(c);
	     painter->setPen(pen);
	     bool rot;
	     pairs points=i->second->subsample(slc,&rot);
	 //    fprintf(stderr,"POINTS ARE %d \n",(int)points.size());
	    // pairs points=i->second->subsample(pos,ax1,iax1,beg1,end1,ax2,iax2,beg2,end2,&rot);
	     if(draw_type==DISPLAY_LINE && points.size() >1) {

	       if(iax1==i->second->get_single()) points.sort2();
	       else points.sort1();
	       draw_line(&points, painter, pos, slc,sz,0);
	     }
	     else if(points.size() >0){
	       if(draw_type==DISPLAY_EXTRA) draw_extra(&points, painter, pos,slc,sz);
	        else    draw_points(&points, painter, pos, slc,sz);
	     }
	     points.clear();
	     if(!rot && draw_type!=DISPLAY_EXTRA){
	     for(int in=0; in < (int)neighbors->size(); in++){
	        float rat=(float)(ndist-dist->at(in))/(float)ndist;
	        if(rat >0.){
	          c.setAlphaF(rat);
	               pen.setColor(c); 
	               painter->setBrush(c);
	          painter->setPen(pen);
	          points=neighbors->at(in)->subsample(ax1,iax1,beg1,end1,ax2,iax2,beg2,end2,i->first);
	          if(draw_type=DISPLAY_LINE && points.size()>1) {
	          	if(iax1==i->second->get_single()) points.sort2();
	            else points.sort1();
	            draw_line(&points,painter,pos,slc,sz,dist->at(in));
	          }
	          else if(points.size() >0) draw_points(&points,painter,pos,slc,sz);
	          points.clear();
	        }
	      }
	      }
	    }
	  }
	}
	        
	     
