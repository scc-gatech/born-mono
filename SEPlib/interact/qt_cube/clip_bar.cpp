#include<clip_bar.h>
#include<qapplication.h>
//Added by qt3to4:
#include <QResizeEvent>
#include <QPolygon>
#include <QMouseEvent>
#include <QPaintEvent>
#include "util.h"

clip_bar::clip_bar(int id, QWidget* parent,  const char* name){
	pm=QPixmap(100,100);
	idat=QString::number(id);
	for(int i=0; i < 256; i++) histo.push_back(0.);
	if(parent==0 && name==0){}
	symmetric=false;
}
void clip_bar::set_pts(QString str) {
  pts.clear();
 // pts.push_back(bar_pt(0.,0.));
  this->to_bar_pts(str);
 //pts.push_back(bar_pt(1.,1.));
  if(pts.size()==0){
    pts.push_back(bar_pt(0.,0.005));
    pts.push_back(bar_pt(1.,.995));
  }
  else{
    if(pts[0].pct >.03){
      pts.insert(pts.begin(),bar_pt(0.,0.005));
    }
    if(pts[pts.size()-1].pct < .97){
      pts.push_back(bar_pt(1.,.995));
    }
  }
  draw_bar();
}
void clip_bar::set_histo(QString str){
 for(int i=0; i < 256;i++) {
   histo[i]=str.section(":",i,i).toFloat();
   }
 draw_bar();
}
void clip_bar::draw_bar(){
	W = width();
	H = height();
	if(W<=0 || H<=0) { W=150; H=40;}
  pm=QPixmap(W,H);

	QPainter painter(&pm);
	QPen pen;

	pen.setWidth(2);
	pen.setColor(Qt::green); 

  painter.setBackground(Qt::white);
  
  painter.setPen(pen);

	painter.eraseRect(0,0,pm.width(), pm.height());
	QPolygon array,pt,h;

  int ix,iy;
	for(int i=0; i < (int) pts.size(); i++) {
	  float c=pts[i].clip;
	  to_pt_small(pts[i].pct,c,&ix,&iy);
	  array.putPoints(i, 1,ix,iy);
	  pt.putPoints(0,5,ix-2,iy-2,ix+2,iy-2,ix+2,iy+2,ix-2,iy+2,ix-2,iy-2);
	  painter.drawPolyline(pt);
	}

	painter.drawPolyline(array);
	pen.setColor(Qt::blue); 
	painter.setPen(pen);
	float px,py;
	for(int i=0; i < (int)histo.size(); i++) {
	  px=(float)i/(float)255;
	  py=histo[i];
	//  fprintf(stderr,"in this loop %d %d %f %d \n",px,py,histo[i],i);
	  to_pt(px,py,&ix,&iy);
	  h.putPoints(i, 1,ix,iy);
	  
	}

	painter.drawPolyline(h);
	
	QPolygon *box1;
	

	
  box1=boxit(0.,.8,1.,1.0);
	pen.setColor(Qt::gray); 
	painter.setPen(pen);
	painter.drawPolyline(*box1);
	delete box1;

	
  box1=boxit(0.,0.,1.,.2);
	pen.setColor(Qt::gray); 
	painter.setPen(pen);
	painter.drawPolyline(*box1);
	delete box1;
	
	
	box1=boxit(0.,0.,1.,1.);
	pen.setColor(Qt::black); 
	painter.setPen(pen);
	painter.drawPolyline(*box1);
	delete box1;
	  
  
  scale();
  update();

 // QPainter painter2(this);
 // painter2.drawPixmap(0, 0, pmScaled);
}
QPolygon *clip_bar::boxit(float bx, float by, float ex, float ey){
  QPolygon *a=new QPolygon;
  int ix,iy;
  to_pt(bx,by,&ix,&iy);
	a->putPoints(0,1,ix,iy);
  to_pt(bx,ey,&ix,&iy);
	a->putPoints(1,1,ix,iy);
	to_pt(ex,ey,&ix,&iy);

	a->putPoints(2,1,ix,iy);
  to_pt(ex,by,&ix,&iy);
	a->putPoints(3,1,ix,iy);
	to_pt(bx,by,&ix,&iy);
	a->putPoints(4,1,ix,iy);

  return a;
}
void clip_bar::to_pt_small(float px, float py, int *ix, int *iy){
  *ix=(int)(W*px+.0*W);
   if(py <=.004){
     *iy=(int)(.9*H);
   }
   else if(py >=.996){
     *iy=(int)(.1*H);
   }
   else   *iy=(int)(.8*H-py*.6*H);
}
void clip_bar::to_pct_small(int ix, int iy, float *px, float *py){
  *px=(ix-.0*W)/1./(float)W;
  float t=(H-iy)/(float)H;
  if(t<=.199){
     *py=.0003;
  }
  else if(t >=.801){
   *py=.999;
  }
  else{
    *py=(.8*H-iy)/(H*.6);
  }  

}
void clip_bar::to_pt(float px, float py, int *ix, int *iy){
  *ix=(int)(W*1.*px);
   *iy=(int)(H-H*py);
}
void clip_bar::to_pct(int ix, int iy, float *px, float *py){
  *px=(ix)/1./W;
  *py=(H-iy)/H;
}
void clip_bar::to_bar_pts(QString str){
 int nelem=str.count(":"); 
 float *buf=new float[nelem+1];
 for(int i=0; i < nelem+1; i++) buf[i]=str.section(":",i,i).toFloat();

 for(int i=0; i< (nelem+1)/2; i++) pts.push_back(bar_pt(buf[i*2],buf[i*2+1]));

 delete [] buf;
}
void clip_bar::scale(){
    int h = height();

 //   if ( image->isNull() ) return;

 //   QApplication::setOverrideCursor( waitCursor ); // this might take time
    if ( width() == pm.width() && h == pm.height() )
    {						// no need to scale if widget
	pmScaled = pm;				// size equals pixmap size
    } else {

	    QMatrix m;				// transformation matrix
	    m.scale(((double)width())/pm.width(),// define scale factors
		    ((double)h)/pm.height());
	    pmScaled = pm.transformed( m );		// create scaled pixmap
    }
	pmScaledCopy = pmScaled;
//    QApplication::restoreOverrideCursor();	// restore original cursor
}
void clip_bar::paintEvent( QPaintEvent *e ){

    if ( pm.size() != QSize( 0,  0 ) ) {		// is an image loaded?
	QPainter painter(this);
	painter.setClipRect(e->rect());
	painter.drawPixmap(0, 0, pmScaled);
 // QPainter painter2(this);
 // painter2.drawPixmap(0, 0, pmScaled);
 //   this->myv->viewit(&painter,&pen,fact,dat,pos,border,W-2*border,border,H-2*border);

    }
}
void clip_bar::resizeEvent( QResizeEvent *e ){
 if(e==0){}
    if ( pm.size() == QSize( 0, 0 ) )		// we couldn't load the image
	   return;

    if ( width() != pmScaled.width() || height() != pmScaled.height())
    {						// if new size,
   	  scale();				// scale pmScaled to window
    	W = width(); H = height();
    }
  
  draw_bar();
}
void clip_bar::add_pt(int ix, int iy){
  float px,py;
    std::vector<QString> com;
      com.push_back(idat);
  com.push_back("clip"); com.push_back("add");
  to_pct_small(ix,iy,&px,&py);
  
  if(px < .02 && px>=0.) px=0.001;
  if(px > .98 && px<=1.) px=.999;
  if(py >1.000) py=.9999;
  if(py < 0.0) py=.00001;
  
//  com.push_back(idat);
  if(px >=0. && px <=1.0 && py>=0. && py<= 1.){
    com.push_back(QString::number(px)+":"+QString::number(py));
     emit emitit(com);
    if(symmetric){
      com[3]=QString::number(1.-px)+":"+QString::number(1.-py);
      emit emitit(com);
    }
    
  }
}
int clip_bar::find_pt(int ix, int iy, int dmin,int ib, int ie){

  std::vector<QString> com;
  com.push_back("clip"); com.push_back("delete");
  int dist=999,x,y,d,i_f=0;
  for(int i=ib; i<ie; i++){
    to_pt_small(pts[i].pct,pts[i].clip,&x,&y);
    d=(x-ix)*(x-ix)+(y-iy)*(y-iy);
    if(d < dist){ i_f=i; dist=d;}
  }
  if(dist > dmin) return -1;
  return i_f;
}
void clip_bar::del_pt(int ix, int iy){
  float px,py;
  to_pct(ix,iy,&px,&py);
 
  std::vector<QString> com;
  com.push_back(idat);
  com.push_back("clip"); com.push_back("delete");
  
  int i_f=find_pt(ix,iy,25,1,pts.size()-1);
  if(i_f >=0){
    com.push_back(QString::number(i_f-1));
    emit emitit(com);  
  }
}
void clip_bar::clear_picks(){

std::vector<QString> com;
  com.push_back(idat);
  com.push_back("clip"); com.push_back("clear");
  com.push_back("blah");
  emit emitit(com);

}
void clip_bar::set_pcts(float *ps){
  std::vector<QString> com;
  com.push_back(idat);
  com.push_back("clip"); com.push_back("set");
  com.push_back(util::string_from_float_array(256,ps));
  emit emitit(com);
}
void clip_bar::mousePressEvent(QMouseEvent *e){
	W = width();
	H = height();
 if(e->button() == Qt::RightButton){
    ipt_down=find_pt(e->x(),e->y(),25,0,pts.size());
  }

}
void clip_bar::mouseReleaseEvent(QMouseEvent *e){
	W = width();
	H = height();
 if (e->button() == Qt::RightButton) {
  // draw_bar();
 //  QPainter painter(this);
  // painter.drawPixmap(0, 0, pmScaled);
    if(ipt_down >=0){
      if(ipt_down==0 || ipt_down==(int)pts.size()-1){//Fake points
        add_pt(e->x(),e->y());
      }
      else{
        std::vector<QString> com;
        com.push_back(idat);
        com.push_back("clip"); com.push_back("delete");
        com.push_back(QString::number(ipt_down-1));
        emit emitit(com);
        add_pt(e->x(),e->y());
      }
    }
  }
  else if(e->button() == Qt::MidButton){
    del_pt(e->x(),e->y());
  }
  else if(e->button() == Qt::LeftButton){
    add_pt(e->x(),e->y());
  }

}
