#include "draw_other.h"


draw_line::draw_line(int bx, int  by, int ex, int ey){
    set_basic();
   
	poly << QPoint (bx,by) << QPoint (bx,ey)
						 << QPoint (ex,ey)<< QPoint(ex,by)<<QPoint(bx,by);
						 

}
void draw_pts::draw(QPainter *painter){
 	 if(pen!=0) painter->setPen(*pen);

   for(int i=0; i < poly.size(); i++){
     QPolygon p;
     int ix=poly.at(i).x();
     int iy=poly.at(i).y();
     p << QPoint (ix-sz,iy-sz) << QPoint (ix-sz,iy+sz)
						 << QPoint (ix+sz,iy+sz)<< QPoint(ix+sz,iy-sz)<<QPoint(ix-sz,iy-sz);
   
     painter->drawPolyline(p);
   }
}
