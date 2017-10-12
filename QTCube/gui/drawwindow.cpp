#include <qpainter.h>
#include <qfiledialog.h>

#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPaintEvent>
#include <stdlib.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qclipboard.h>
#include <qmenubar.h>
#include <qimage.h>
#include <qmatrix.h>
#include "qfileinfo.h"
#include "multiface.h"
#include "util.h"
#include "drawwindow.h"
#include "centerwindow.h"
#include <ctime>
#include<assert.h>
#include "threeface.h"
#include "single.h"
#include "simple_color.h"
#include "cube_cut.h"
#include "cube.h"
#include "qprinter.h"
using namespace SEP;

DrawWindow::DrawWindow(int ng1, int ng2, std::vector<std::shared_ptr<panel>> pan,int in){
  ngrid1=ng1; ngrid2=ng2; //Grid size of panels to display
  inum=in;
  for(int i=0; i < (int)pan.size(); i++){ panels.push_back(pan[i]);}
  //Setup basic display for panels
  QPalette palette;
  palette.setColor(this->backgroundRole(), QColor(0,0,0));
  setPalette(palette);
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  set_boundary_thick(4); //How thick boundary between panels
  ano=new annotate(); //Annotate window

  //Time to setup panel boundaries
  ngrid1=ng1; ngrid2=ng2;

  beg1=new int [ngrid1]; end1=new int [ngrid1];

  beg2=new int[ngrid2]; end2=new int [ngrid2];

  port1=new float[ngrid1]; port2=new float[ngrid2];
  for(int i=0; i < ngrid1; i++) port1[i]=1./(float)ngrid1;
  for(int i=0; i< ngrid2; i++) port2[i]=1./(float)ngrid2;
  update_panel_map();
  this->wind_bg="white";
 active_panel=0;

  set_active_panel(0);

}
void DrawWindow::add_panel(std::shared_ptr<panel>p){
      panels.push_back(p);

       }
void DrawWindow::pdf_save(QString sm,QString file){
QPrinter *printer;

 
  if(sm.contains("small")>0) printer = new QPrinter( QPrinter::ScreenResolution );
  else {
    printer = new QPrinter( QPrinter::HighResolution );
  }
  printer->setOutputFormat ( QPrinter::PdfFormat );
  


  printer->setColorMode(QPrinter::Color);
  printer->setOutputFileName (file);
  printer->setOrientation(QPrinter::Portrait);
  printer->setFullPage (false );
  qreal q=0.;
  printer->setPageMargins(q,q,q,q,QPrinter::Point);
  printer->setPaperSize(QSizeF(width(),height()),QPrinter::Point);
  QPainter painter(printer);
  painter.setClipRegion(rect());
int rangex=printer->pageRect().width(),rangey=printer->pageRect().height();
   long long sz=(long long)rangex *(long long)rangey;

  if(sz>2*1000*1000*1000){
    fprintf(stderr,"To large a figure, shrink your window or resolution \n");
    return;
 }  

 W=rangex; H=rangey; 
 update_panel_map();
 for(int i=0; i<(int)panels.size(); i++) panels[i]->pdf_save(&painter);
   ano->draw(&painter,0,0,W,H);

 W=width(); H=height();
 
 update_panel_map();
  painter.end();
 delete printer;
}
void DrawWindow::scale(){
    int h = height();

 //   if ( image->isNull() ) return;

    //QApplication::setOverrideCursor( waitCursor ); // this might take time
    if ( width() == pm.width() && h == pm.height() )
    {                                           // no need to scale if widget
        pmScaled = pm;                          // size equals pixmap size
    } else {

            QMatrix m;                          // transformation matrix
            m.scale(((double)width())/pm.width(),// define scale factors
                    ((double)h)/pm.height());

                 pmScaled = pm.transformed( m );                // create scaled pixmap

         // pmScaled = pm.xForm( m );           // create scaled pixmap
    }
        pmScaledCopy = pmScaled;
    QApplication::restoreOverrideCursor();      // restore original cursor
}
void DrawWindow::update_display(bool force){
  bool update_it=false;
  
  for(int i=0; i < panels.size(); i++){
     if(panels[i]->get_update_it()) update_it=true;
  }
  /*XXXXXX*/
  if(update_it|| force){
    this->draw_base_pixmap();
    update();
  }
  for(int i=0; i < panels.size(); i++){
     panels[i]->set_update_it(false);
  }
}
void DrawWindow::add_anno(int bx, int by, int ex, int ey){

  ano->add_annotate((float)bx/(float)width(),(float)by/(float)height(),
    (float)ex/(float)width(),(float)ey/(float)height());
}
void DrawWindow::finish_anno(int bx, int by, int ex, int ey){

  ano->finish_annotate((float)bx/(float)width(),(float)by/(float)height(),
    (float)ex/(float)width(),(float)ey/(float)height());
}
void DrawWindow::update_anno(int bx, int by, int ex, int ey){

  ano->update_annotate((float)bx/(float)width(),(float)by/(float)height(),
    (float)ex/(float)width(),(float)ey/(float)height());
}
void DrawWindow::shift_finish_anno(int bx, int by, int ex, int ey){

  ano->shift_finish_annotate((float)bx/(float)width(),(float)by/(float)height(),
    (float)ex/(float)width(),(float)ey/(float)height());
}
void DrawWindow::shift_anno(int bx, int by, int ex, int ey){

  ano->shift_annotate((float)bx/(float)width(),(float)by/(float)height(),
    (float)ex/(float)width(),(float)ey/(float)height());
}
void DrawWindow::move_anno(int bx, int by, int ex, int ey){
  if(ex==ey);
  ano->move_nearest((float)bx/(float)width(),(float)by/(float)height());
}
void DrawWindow::del_anno(int bx, int by){

  ano->delete_nearest((float)bx/(float)width(),(float)by/(float)height());

}
void DrawWindow::resizeEvent( QResizeEvent *x ){
      if ( pm.size() == QSize( 0, 0 ) )		{// we couldn't load the image
     if(width()>20) draw_base_pixmap();
	return;
}

    if ( width() != pmScaled.width() || height() != pmScaled.height())
    {						// if new size,
	scale();				// scale pmScaled to window
	W = width(); H = height();
	update_panel_map();
    }
    
}
void DrawWindow::set_grid2(int ng2){

  ngrid2=ng2;
 delete []port2;
  delete []beg2;  delete []end2;
  beg2=new int [ngrid2]; end2=new int [ngrid2];
  port2=new float[ngrid2];
  for(int i=0; i< ngrid2; i++) port2[i]=1./(float)ngrid2;
  update_panel_map();

}
void DrawWindow::set_grid1(int ng1){

  ngrid1=ng1;
  delete []port1;
  delete []beg1;  delete []end1; 
  beg1=new int [ngrid1]; end1=new int [ngrid1];
  port1=new float[ngrid1]; 
  for(int i=0; i < ngrid1; i++) port1[i]=1./(float)ngrid1;
  update_panel_map();

}
void DrawWindow::update_panel_map(){
    W=width(); H=height();
  
  int Wd=W-(ngrid1-1)*bound_thick,Ht=H-(ngrid2-1)*bound_thick;

  if(Wd<30) return;
  if(Wd<0 || Ht<0) return;
  //First figure out if there any panels that are to small to both displaying
  for(int i=0; i < ngrid1;i++){
  
      if(port1[i]*Ht<60) port1[i]=0;

  }
  for(int i=0; i< ngrid2;i++){
    if(port2[i]*Ht<60) port2[i]=0;

  }
  /*Correct if we have collapsed a section*/
  float sm=0; for(int i=0; i< ngrid1; i++) sm+=port1[i];
  for(int i=0; i< ngrid1; i++) {
    port1[i]/=sm;
  }
  sm=0; for(int i=0; i< ngrid2; i++) sm+=port2[i];
  for(int i=0; i< ngrid2; i++) port2[i]/=sm;

  beg1[0]=0; end1[0]=(int)(port1[0]*W); 
  for(int i=1; i< ngrid1; i++) {
    beg1[i]=end1[i-1]+bound_thick; end1[i]=beg1[i]+(int)(Wd*port1[i]);

  }

  beg2[0]=0; end2[0]=(int)(port2[0]*H); 
  for(int i=1; i< ngrid2; i++) {
    beg2[i]=end2[i-1]+bound_thick; end2[i]=beg2[i]+(int)(Ht*port2[i]);
    
  }

  int i=0;
  for(int i2=0; i2 < ngrid2; i2++){
    for(int i1=0; i1< ngrid1;i1++,i++){
      panels[i]->set_panel_dimensions(beg1[i1],beg2[i2],end1[i1],end2[i2]);
    }
  }

}
void DrawWindow::paintEvent( QPaintEvent *e ){

    if ( pm.size() != QSize( 0,  0 ) ) {		// is an image loaded?
	QPainter painter(this);
	painter.setClipRect(e->rect());
	painter.drawPixmap(0, 0, pmScaled);

    }
}
void DrawWindow::draw_base_pixmap(){
	W = width();
	H = height();

	
   if(pm.isNull()){
     pm=QPixmap(100,100);
   }
   else {
     pm = pm.scaled(W,H);
   }

   assert(!pm.isNull());

   QPainter painter(&pm);

        QPen pen;
        pen.setWidth(2);

          QPalette palette;
 palette.setColor(this->backgroundRole(), this->back_c);
 setPalette(palette);
        painter.eraseRect(0,0,pm.width(), pm.height());

   for(int i=0; i< panels.size(); i++) {
    
   if(panels[i]->draw_it()) panels[i]->draw_base_pixmap(&painter);
 }

  if(bound_thick>0 && (ngrid1>1 || ngrid2>1)){
     QPen pen;
     pen.setWidth(bound_thick);
     pen.setColor(Qt::black);
     painter.setPen(pen);
     QPolygon pts;
     
     if(ngrid1>1){
       for(int i=0; i < ngrid1-1; i++){
         pts<<QPoint(end1[i],0)<<QPoint(end1[i],H);
         painter.drawPolyline(pts);
         pts.clear();
       }
      }
      if(ngrid2>1){
        for(int i=0; i < ngrid2-1; i++){
          pts<<QPoint(0,end2[i])<<QPoint(W,end2[i]);
          painter.drawPolyline(pts);
          pts.clear();
        }
       }
     }

     ano->draw(&painter,0,0,W,H);

    scale();
    update();
}
void DrawWindow::set_active_panel(int act){
//fprintf(stderr,"in active window %d  \n",act);

 int i=0;
 bool found=false;
 while(i< (int)panels.size() && !found){
   if(panels[i]->get_iview()==act){
     active_panel=i;
     found=true;
   }
   i++;
  }
}
void DrawWindow::set_background_color(QString col){
  this->wind_bg=col;
 // this->back_c=my_cmap->return_qcolor(col); For now don't support
  draw_base_pixmap();
}
void DrawWindow::keyPressEvent( QKeyEvent *e ){
  switch( e->key() )
  {
  default:
    panels[active_panel]->keyPressEvent(e);
  }
}
void DrawWindow::keyReleaseEvent( QKeyEvent *e ){

   switch( e->key() )
  {
  default:
    panels[active_panel]->keyReleaseEvent(e);
  }
  
  
}
int DrawWindow::get_panel(int ix,int iy){
   mouse_boundx=mouse_boundy=-1;
   //First the x directions
   bool found=false;
   int i1,i2,i=1;
   if(ix< end1[0]) { i1=0; found=true;}
   while(i< ngrid1 && !found){
     if(ix< beg1[i]) { mouse_boundx=i-1; i1=-1; found=true;}
     else if(ix< end1[i]) { i1=i; found=true;}
     i++;
   }
   i=1;
   found=false;

   if(iy< end2[0]) { i2=0; found=true;}
   while(i< ngrid2 && !found){
     if(iy< beg2[i]) { mouse_boundy=i-1; i2=-1; found=true;}
     else if(iy< end2[i]) { i2=i; found=true;}
     i++;
   }

   if(i1<0 || i2<0) return -1;
   return i1+i2*ngrid1;
}
void DrawWindow::change_bound(int ix,int iy){
   float pctx=(float)ix/(float)W;
   float pcty=(float)iy/(float)H;
   if(mouse_boundx!=-1){
     float *bariers=new float[ngrid1];
     bariers[0]=port1[0];
     for(int i=1; i < ngrid1; i++) bariers[i]=bariers[i-1]+port1[i];
     bariers[mouse_boundx]=pctx;
     for(int i=0; i< ngrid1; i++){
      if(i<mouse_boundx){
        if(bariers[i] >=bariers[mouse_boundx]){
          if(i==0) bariers[0]=0.;
          else bariers[i]=bariers[i-1];
        }
      } 
      else if(i >mouse_boundx){
        if(bariers[i] <= bariers[mouse_boundx]) bariers[i]=bariers[mouse_boundx];
      }
   }
   port1[0]=bariers[0]; for(int i=1; i < ngrid1; i++) port1[i]=bariers[i]-bariers[i-1];
   delete [] bariers;
  }
    if(mouse_boundy!=-1){
     float *bariers=new float[ngrid2];
     bariers[0]=port2[0];
     for(int i=1; i < ngrid2; i++) bariers[i]=bariers[i-1]+port2[i];
     bariers[mouse_boundy]=pcty;
     for(int i=0; i< ngrid2; i++){
      if(i<mouse_boundy){
        if(bariers[i] >=bariers[mouse_boundy]){
          if(i==0) bariers[0]=0.;
          else bariers[i]=bariers[i-1];
        }
      } 
      else if(i >mouse_boundy){
        if(bariers[i] <= bariers[mouse_boundy]) bariers[i]=bariers[mouse_boundy];
      }
   }
   port2[0]=bariers[0]; for(int i=1; i < ngrid2; i++) port2[i]=bariers[i]-bariers[i-1];
   delete [] bariers;
  }
  update_panel_map();
  draw_base_pixmap();
  

}
void DrawWindow::mousePressEvent(QMouseEvent *e){

  mouse_panel=get_panel(e->x(),e->y());
  if(mouse_panel>=0) panels[mouse_panel]->mousePressEvent(e);
  else{
       timeit.start();
   }

}
void DrawWindow::mouseDoubleClickEvent(QMouseEvent *e){
  
   mouse_panel=get_panel(e->x(),e->y());
   if(mouse_panel>=0) panels[mouse_panel]->mouseDoubleClickEvent(e);
   else if (e->button() == Qt::LeftButton){
       for(int i=0; i < ngrid1; i++){ port1[i]=1./(float)ngrid1;}
       for(int i=0; i < ngrid2; i++){ port2[i]=1./(float)ngrid2;}
     }
     draw_base_pixmap();
   }
void DrawWindow::mouseMoveEvent(QMouseEvent *e){

  if(mouse_panel>=0){
    int ip=get_panel(e->x(),e->y());
    if(ip==mouse_panel) panels[mouse_panel]->mouseMoveEvent(e);
  }
  else if(timeit.elapsed() >60){ 
    change_bound(e->x(),e->y());
    timeit.restart();
  }
  
  
}
void DrawWindow::mouseReleaseEvent(QMouseEvent *e){


  if(mouse_panel>=0){
    int ip=get_panel(e->x(),e->y());
    if(ip==mouse_panel) {
       panels[mouse_panel]->mouseReleaseEvent(e);
    }
  }
  else change_bound(e->x(),e->y());

}

 
