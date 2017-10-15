#include<wiggle.h>
#include<math.h>
#include<qnamespace.h>
#include <QPolygon>
#include <QPixmap>
#include "my_colors.h"

void wiggle::draw_slice(QPainter *painter, dataset *dat,QPen *pen, 
 orient_cube *pos,bool ov,bool draw_grid){

  if(ov){}
  my_dat=dat;
  axes[0]=dat->get_grid_axis(iax1+1);
  axes[1]=dat->get_grid_axis(iax2+1);


  int f1=pos->get_beg(iax1);
  int f2=pos->get_beg(iax2);

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

   unsigned char *buf=     dat->get_char_data(pos,iax1,bb1,ee1,iax2,bb2,ee2);



  b_1=f1; 
  b_2=f2;
  e_1=b_1+n1;
  e_2=b_2+n2;
//  int it;
//  if(rev_1){ it=e_1;e_1=b_1; b_1=it;}
//  if(rev_2){ it=e_2;e_2=b_2; b_2=it;}
   PaintWiggles(n2, n1, painter, buf);
 
   painter->setPen(*pen);
   if(axis_top||axis_bot||axis_left||axis_right) this->draw_axes(painter,pos,draw_grid);
    this->draw_pos(painter,pos);
  
  


}


void wiggle::PaintWiggles(int n2, int n1, QPainter *painter, unsigned char *buf){
	 painter->save();
  painter->translate(bx,by);
	   //Space given per wiggle
    int wiggle_pixel_space=8;
	int ndraw=(xe-xb)/wiggle_pixel_space;
	int iyzero,ixzero;
	if(ndraw<0) ndraw = 0;
	bool do_pos=true,do_neg=true;
	if(neg_color.contains("none")>0) do_neg=false;
	if(pos_color.contains("none")>0) do_pos=false;
	//int nmax=ndraw;
	int j2=1;
	if(ndraw > n2) ndraw=n2;
	else{
	  j2=n2/ndraw;
	  ndraw=(int)ceilf((float)n2/(float)j2);
	}
	if(j2<1) j2=1;
	QBrush solid=QBrush(Qt::SolidPattern);
	QBrush empty=QBrush(Qt::NoBrush);
	int itr=0;
						float prevX=0, prevY=0;
	for (int n2Counter = 0; n2Counter < n2; n2Counter+=j2,itr++){
		float xRange = (float)(xe - xb)/(float)ndraw;
	    xRange=1./(float)ndraw;
	//	int xBeg = xb + (xRange * itr);
		float xBeg=xRange*itr;
	//	int xEnd = xBeg + xRange;
		float xEnd=xBeg+xRange;
								//fprintf(stderr,"CHECK THIS %d %d %d %f \n",j2,n2,ndraw,xRange);

//Divide the range by the number of possible unsigned chars, yielding the change in x per char
	int ix,iy,ixold=0,iyold=0;
	float dxr = (float)xRange/256.;
		QPolygon pointArray;
		float yRange = (float)(ye - yb)/(float)(n1-1);
		yRange=1/(float)(n1-1.);
		for (int n1Counter = 0; n1Counter < n1; n1Counter++)
		{
		
			//int y = yb + n1Counter * yRange;
			
			float y=n1Counter*yRange;
			float x = buf[n1Counter + n2Counter * n1] * dxr + xBeg;
						get_image_pos_pct(x,y,&ix,&iy);

				if (n1Counter ==0)
					{
						prevX = x;
						prevY = y;
						ixold=ix;
						iyold=iy;
					}
			if (x > xEnd) x = xEnd;
			if (xEnd > 1.) x =1.;
			if (x < xBeg) x = xBeg;
			float xzero=xBeg+dxr*128;
			pointArray.putPoints(n1Counter, 1, ix,iy);
			painter->setPen(wiggleColor);
  if(do_pos){
//Regions of data to the right of the value 128 (ie positive in the unsigned char scale) are shaded in.			
			if (x > xzero || prevX > xzero){
//If the yRange us one or less, the regions are shaded in by horizontal lines.			
				if (yRange*(ye-yb) <= 1.0 && x > xzero)
				{
				  
				  //  get_get_image_pos_pct(xzero,y,&ixold,&iyold);
				    get_image_pos_pct(xzero,y,&ixzero,&iyzero);
					painter->drawLine(ixzero,iyzero,ix,iy);
				}
//Otherwise they are shaded in as polygons made up of combinations of triangles and quadrilaterals.				
				else
				{ 
					QPolygon polyFill;

				
					//else
				//	{
				//		pointArray.point(n1Counter - 1, &prevX, &prevY);
				//	}
					if (prevX <= xzero  && x >= xzero)
					{
						double slope;
						if (x - prevX != 0)
						{
						  
							slope = ((float)(y - prevY)/(float)(x - prevX));
						}
						else
						{
							slope = 0;
						}
					//	double difference = (float)x - (xBeg + (dxr *128));
					//	int vertexY = prevY + (int)(difference * slope);
						float yzero=slope*(xzero-prevX)+prevY;
						int ix2,iy2;
						get_image_pos_pct(xzero,yzero,&ixzero,&iyzero);
						get_image_pos_pct(xzero,y,&ix2,&iy2);

						polyFill << QPoint (ixzero,iyzero) << QPoint (ix, iy)
						 << QPoint (ix2, iy2);
						  		
					}
					
				
					else if (prevX >= xzero  && x <= xzero)
					{

						double slope;
						if (x - prevX != 0)
						{
						  
							slope = ((float)(y - prevY)/(float)(x - prevX));
						}
						else
						{
							slope = 0;
						}
					//	double difference = (float)x - (xBeg + (dxr *128));
					//	int vertexY = prevY + (int)(difference * slope);
						float yzero=slope*(xzero-prevX)+prevY;
						int ix2,iy2;
						get_image_pos_pct(xzero,yzero,&ixzero,&iyzero);
						get_image_pos_pct(xzero,prevY,&ix2,&iy2);

						polyFill << QPoint (ixzero, iyzero) << QPoint (ixold, iyold)
						 << QPoint (ix2, iy2);
						 
					}
		
					
					else
					{
                      
                        int ix2,iy2;				
                        get_image_pos_pct(xzero,prevY,&ixzero,&iyzero);
                        get_image_pos_pct(xzero,y,&ix2,&iy2);

			//			polyFill << QPoint (ixzero, iyzero) << QPoint (ixold,iyold)
				//		 << QPoint(ix,iy) << QPoint (ix2,iy2);
						 polyFill << QPoint (ixzero, iyzero) << QPoint(ixold,iyold)
						 << QPoint(ix,iy) << QPoint (ix2,iy2);
					}
					painter->setBrush(Qt::SolidPattern);
					painter->setBrush(posColor);
					painter->drawPolygon(polyFill);
					painter->setBrush(empty);

				}
			}
			}
			if(do_neg){
			
			
//Regions of data to the left of the value 128 (ie negative in the unsigned char scale) are shaded in.			
			if (x < xzero || prevX < xzero){
//If the yRange us one or less, the regions are shaded in by horizontal lines.			
				if (yRange*(ye-yb) <= 1.0 && x < xzero)
				{
				    
				    get_image_pos_pct(xzero,y,&ixzero,&iyzero);
					painter->drawLine(ixzero, iyzero, ix, iy);
				}
//Otherwise they are shaded in as polygons made up of combinations of triangles and quadrilaterals.				
				else
				{ 
					QPolygon polyFill;

				
					//else
				//	{
				//		pointArray.point(n1Counter - 1, &prevX, &prevY);
				//	}
					if (prevX >= xzero  && x <= xzero)
					{
						double slope;
						if (prevX - x!= 0)
						{
						  
							slope = ((float)(y - prevY)/(float)(x - prevX));
						}
						else
						{
							slope = 0;
						}
					//	double difference = (float)x - (xBeg + (dxr *128));
					//	int vertexY = prevY + (int)(difference * slope);
						float yzero=slope*(xzero-prevX)+prevY;
						 int ix2,iy2;
				        get_image_pos_pct(xzero,yzero,&ixzero,&iyzero);
				           get_image_pos_pct(xzero,yzero,&ix2,&iy2);
						polyFill << QPoint (ixzero,iyzero) << QPoint (ix,iy)
						 << QPoint (ix2,iy2);
						  		
					}
					
				
					else if (prevX <= xzero  && x >= xzero)
					{

						double slope;
						if (x - prevX != 0)
						{
						  
							slope = ((float)(y - prevY)/(float)(x - prevX));
						}
						else
						{
							slope = 0;
						}
					//	double difference = (float)x - (xBeg + (dxr *128));
					//	int vertexY = prevY + (int)(difference * slope);
						float yzero=slope*(xzero-prevX)+prevY;
						 int ix2,iy2;
				        get_image_pos_pct(xzero,yzero,&ixzero,&iyzero);
				           get_image_pos_pct(xzero,prevY,&ix2,&iy2);
						polyFill << QPoint (ixzero, iyzero) << QPoint (ixold,iyold)
						 << QPoint (ix2,iy2);
						 
					}
		
					
					else
					{

                      int ix2,iy2;
				        get_image_pos_pct(xzero,prevY,&ixzero,&iyzero);
				           get_image_pos_pct(xzero,y,&ix2,&iy2);
						polyFill << QPoint (ixzero,iyzero) << QPoint (ixold,iyold)
						 << QPoint(ix,iy) << QPoint (ix2,iy2);
					}
					painter->setBrush(Qt::SolidPattern);
					painter->setBrush(negColor);
					painter->drawPolygon(polyFill);
					painter->setBrush(empty);

				}
			}
			
			
			
			
			}
			prevX=x;
			prevY=y; ixold=ix; iyold=iy;
		}
		painter->drawPolyline(pointArray);
	}
	painter->restore();
}

slice *wiggle::clone(){
   wiggle *x=new wiggle();
   x->pos_color=pos_color;
   x->posColor=posColor;
   x->negColor=negColor;
   x->neg_color=neg_color;
   x->wiggle_color=wiggle_color;
   x->wiggleColor=wiggleColor;
  x->is_raster=false;
  x->is_overlay=is_overlay;
  return x;
}


  
  void wiggle::set_neg_color(QString color){
     if(color.contains("none") >0){}
    else{
    my_colors x=my_colors();
    negColor=x.return_qcolor(color);
    
    }
    neg_color=color;
  }
  QString wiggle::return_pos_color(){ return pos_color;}
 void wiggle::set_pos_color(QString color){
    if(color.contains("none") >0){}
    else{
    my_colors x=my_colors();
    posColor=x.return_qcolor(color);
    
    }
    pos_color=color;
  }
  QString wiggle::return_neg_color(){ return neg_color;}
   void wiggle::set_wiggle_color(QString color){
    my_colors x=my_colors();
    wiggleColor=x.return_qcolor(color);
    wiggle_color=color;
  }
  QString wiggle::return_wiggle_color(){ return wiggle_color;}
