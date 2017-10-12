#include "draw_other.h"
#include <math.h>
using namespace SEP;

draw_box::draw_box(int bx, int  by, int ex, int ey){
    set_basic();
   
	poly << QPoint (bx,by) << QPoint (bx,ey)
						 << QPoint (ex,ey)<< QPoint(ex,by)<<QPoint(bx,by);
						 

}
draw_line::draw_line(int bx, int  by, int ex, int ey){
    set_basic();
   
	poly << QPoint (bx,by) << QPoint (ex,ey);
						 

}
draw_hyperbola::draw_hyperbola(int bx, int  by, int ex, int ey){
    set_basic();
   
    float slow2=(float)(ey*ey-by*by)/(float)(ex*ex-bx*bx);
    float t02=((float)(ey*ey))-slow2*(float(ex*ex));
    int inc=-1;
    if(ex > bx) inc=1;
    int il=0;
    for(int i=bx; i!=ex+inc; i=i+inc,il++){
      int y=(int)(.5+sqrtf((float)(i*i)*slow2+t02));
      poly.putPoints(il, 1,i,y);
   }
						 

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