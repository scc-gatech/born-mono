#include "anno.h"
#include<qpainter.h>
#include "qfontmetrics.h"
#include "math.h"
using namespace SEP;
annotate::annotate(){
  cur_thick=2;
  cur_font="Arial-12-Normal";
  cur_col="red";
  cur_text=" ";
  cur_shape="box";
  fonts=std::shared_ptr<my_fonts> (new my_fonts());
  colors= std::shared_ptr<my_colors> (new my_colors());
  active_anno=-1;
}


float anno_text::return_dist(float px, float py){

return sqrtf((pctx-px)*(pctx-px)+(pcty-py)*(pcty-py));

}
float anno_line::return_dist(float px, float py){
return sqrtf((pctx-px)*(pctx-px)+(pcty-py)*(pcty-py));

}
float anno_arrow::return_dist(float px, float py){
return sqrtf((pctx-px)*(pctx-px)+(pcty-py)*(pcty-py));

}
float anno_box::return_dist(float px, float py){
return sqrtf((pctx-px)*(pctx-px)+(pcty-py)*(pcty-py));

}
float anno_ellipse::return_dist(float px, float py){
return sqrtf((pctx-px)*(pctx-px)+(pcty-py)*(pcty-py));

}
void  anno_text::draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>colors, int bx, int by, int ex, int ey){
    assert(!fonts);
    std::shared_ptr<QFontMetrics> fm=fonts->return_font_metric(font);
    painter->setFont(*fonts->return_font(font));

    int pw=fm->width(txt);
    int ph=fm->height();

    QPen pen;
    pen.setWidth(thick);
    pen.setColor(colors->return_qcolor(col));
    painter->setPen(pen);
        int xlt=bx+(int)(pctx*(ex-bx)),ylt=by+(int)(pcty*(ey-by));

if(thick!=0){
    QPolygon tmp;
    tmp<<QPoint(xlt-pw/2-ph/4,ylt-ph/2)<<QPoint(xlt+pw/2+ph/4,ylt-ph/2)<<QPoint(xlt+pw/2+ph/4,ylt+ph/2)<<
      QPoint(xlt-pw/2-ph/4,ylt+ph/2)<<QPoint(xlt-pw/2-ph/4,ylt);
              QBrush brush=QBrush(Qt::SolidPattern);
    brush.setColor(colors->return_qcolor("white"));
    painter->setBrush(brush);
   painter->drawPolygon(tmp);
   }
        painter->drawText(xlt-pw/2,ylt+ph/4,txt);
     

    
}
void anno_line::draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>colors,int bx, int by, int ex, int ey){
   
     assert(!fonts);
 int xlt=bx+(int)(pctx*(ex-bx)),ylt=by+(int)(pcty*(ey-by));
  int xet=bx+(int)(endx*(ex-bx)),yet=by+(int)(endy*(ey-by));
    QPen pen;
    pen.setWidth(thick);
    pen.setColor(colors->return_qcolor(col));
    painter->setPen(pen);
    
    QPolygon tmp;
    tmp<<QPoint(xlt,ylt)<<QPoint(xet,yet);
    painter->drawPolyline(tmp);
}
void  anno_arrow::draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>colors, int bx, int by, int ex, int ey){

  assert(!fonts);
 int xlt=bx+(int)(pctx*(ex-bx)),ylt=by+(int)(pcty*(ey-by));
  int xet=bx+(int)(endx*(ex-bx)),yet=by+(int)(endy*(ey-by));
    QPen pen;
    pen.setWidth(thick);
    pen.setColor(colors->return_qcolor(col));
    QBrush brush=QBrush(Qt::SolidPattern);
    brush.setColor(colors->return_qcolor(col));
    painter->setBrush(brush);
    painter->setPen(pen);
    QPolygon tmp;
    tmp<<QPoint(xlt,ylt)<<QPoint(xet,yet);
    painter->drawPolyline(tmp);
    tmp.clear();
   float  Pi=3.1415926536;
    float arrowSize=(float)(thick*8);
    // double angle = ::acos((float)(yet-ylt)/sqrt(1.*((ylt-yet)*(ylt-yet)+(xlt-xet)*(xlt-xet))));
    float angle=-atan2((float)(yet-ylt),(float)(xet-xlt));
   //  angle=(3.1415926536*2)-angle;
    // angle = (Pi * 2) - angle;
    QPoint arrowP1 =QPoint(xlt,ylt)+ QPoint((int)(sin(angle + Pi / 3) * arrowSize),
                                     (int)(    cos(angle + Pi / 3) * arrowSize));
         QPoint arrowP2 = QPoint(xlt,ylt)+
            QPoint((int)(sin(angle + Pi - Pi / 3) * arrowSize),
                                     (int)(    cos(angle + Pi - Pi / 3) * arrowSize));

         
         tmp <<QPoint(xlt,ylt)<< arrowP1 << arrowP2;
     painter->drawPolygon(tmp);

}
void  anno_box::draw(QPainter *painter, std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>colors,int bx, int by, int ex, int ey){
 assert(!fonts);
 int xlt=bx+(int)(pctx*(ex-bx)),ylt=by+(int)(pcty*(ey-by));
  int xet=bx+(int)(endx*(ex-bx)),yet=by+(int)(endy*(ey-by));
    QPen pen;
    pen.setWidth(thick);
    pen.setColor(colors->return_qcolor(col));
    QBrush empty=QBrush(Qt::NoBrush);
    painter->setBrush(empty);
    painter->setPen(pen);
    QPolygon tmp;
    tmp<<QPoint(xlt,ylt)<<QPoint(xlt,yet)<<QPoint(xet,yet)<<QPoint(xet,ylt)<<QPoint(xlt,ylt);
    painter->drawPolyline(tmp);
}
void  anno_ellipse::draw(QPainter *painter,std::shared_ptr<my_fonts>fonts,  std::shared_ptr<my_colors>colors, int bx, int by, int ex, int ey){
  assert(!fonts);
 int xlt=bx+(int)(pctx*(ex-bx)),ylt=by+(int)(pcty*(ey-by));
  int xet=bx+(int)(endx*(ex-bx)),yet=by+(int)(endy*(ey-by));
      QPen pen;
    QBrush empty=QBrush(Qt::NoBrush);
    painter->setBrush(empty);
     pen.setWidth(thick);
    pen.setColor(colors->return_qcolor(col));
    painter->setPen(pen);
      painter->drawEllipse(xlt,ylt,abs(xlt-xet),abs(ylt-yet));


}
int  annotate::find_nearest(float px, float py){

if((int)annos.size()>0){
  float dmin=annos[0]->return_dist(px,py);
  int iclose=0;
  for(int i=1; i < (int)annos.size(); i++){
    float d=annos[i]->return_dist(px,py);
    if(d< dmin){
       dmin=d;
       iclose=i;
    }
  } 
  return iclose;
}
else return -1;
}
void annotate::delete_nearest(float px, float py){
  int iclose=find_nearest(px,py);
  if(iclose!=-1){
    annos.erase(annos.begin()+iclose);
  }
}
void annotate::move_nearest(float px, float py){

active_anno=find_nearest(px,py);
if(active_anno>=0){
 std::shared_ptr<anno> a=annos[active_anno];
 bxh=a->pctx;
 byh=a->pcty;
 exh=a->endx;
 eyh=a->endy;
}
}
void annotate::add_annotate(float px, float py, float ox, float oy){
   if(cur_shape.contains("ellipse")>0){
      annos.push_back(std::shared_ptr<anno_ellipse> (new anno_ellipse(px,py,cur_col,cur_thick,ox,oy)));
   }
   else if(cur_shape.contains("text")>0){
     annos.push_back(std::shared_ptr<anno_text> (new anno_text(px,py,cur_col,cur_text,cur_font,cur_thick)));
   }
   else if(cur_shape.contains("line")>0){
     annos.push_back(std::shared_ptr<anno_line> (new  anno_line(px,py,cur_col,cur_thick,ox,oy)));
   }
   else if(cur_shape.contains("arrow")>0){
     annos.push_back(std::shared_ptr<anno_arrow> (new  anno_arrow(px,py,cur_col,cur_thick,ox,oy)));
   }
   else if(cur_shape.contains("box")>0){
     annos.push_back(std::shared_ptr<anno_box> (new anno_box(px,py,cur_col,cur_thick,ox,oy)));
   }
   active_anno=annos.size()-1;
}
void annotate::update_annotate(float px, float py, float ox, float oy){

  if(active_anno>=0){
  annos[active_anno]->endx=ox;
  annos[active_anno]->endy=oy;
  annos[active_anno]->pctx=px;
  annos[active_anno]->pcty=py;
  }
}
void annotate::shift_annotate(float px, float py, float ox, float oy){

  if(active_anno>=0){
     annos[active_anno]->shift_anno(bxh,byh,exh,eyh,ox-px,oy-py);
  }
}

void annotate::shift_finish_annotate(float px, float py, float ox, float oy){
  if(active_anno>=0){
     annos[active_anno]->shift_anno(bxh,byh,exh,eyh,ox-px,oy-py);
   active_anno=-1;
   }

}
void annotate::finish_annotate(float px, float py, float ox, float oy){

   update_annotate(px,py,ox,oy);
   active_anno=-1;

}



void annotate::draw(QPainter *painter,int bx, int by, int ex, int ey){
  for(int i=0; i< (int) annos.size(); i++) {
    annos[i]->draw(painter,fonts,colors,bx,by,ex,ey);
  }
}
void annotate::clear(){

  annos.clear();
}