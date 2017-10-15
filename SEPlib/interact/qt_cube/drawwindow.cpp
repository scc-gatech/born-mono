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
#include <q3paintdevicemetrics.h>


DrawWindow::DrawWindow(int inum, param_func *pr,position *p,mode *mmm,
  pick_draw *_pks, datasets *ds,
 int idata, slice_types *ct,  mouse_func *f,orientation_server *server) {
	this->setBackgroundColor(QColor(0,0,0));
	//this->setMinimumSize(DRAW_WINDOW_WIDTH_MIN, DRAW_WINDOW_HEIGHT_MIN);
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    fact=0; dat=0;
    set_mouse_func(f);
    idat=idata;
    datas=ds;
    dat=datas->return_dat(idat);
    draw_o=new draw_other();
    

     dat_o=0;
    
    pars=pr;
    pk=_pks;
    mym=mmm;
    this->colort=ct;
    	iview=inum;
    int ord[]={1,2,3,4,5,6,7,8};
    char var1[] = "order";
    int *order=this->get_ints(var1,8,ord);

    this->pos=new orient_cube(p,order,server);
    delete [] order;
    first=true;
      myf=new QFont("Arial", 12, QFont::Normal);
  QFont font("Arial", 12, QFont::Normal);
  fm=new QFontMetrics(font );
//Sets the initial view to THREE.
    char tagname1[] = "view";
    char default1[] = "THREE";
    this->myv=view_by_name(this->get_string(tagname1,default1));
//Sets the initial data color to grey.
        char tagname2[] = "color";
        char default2[] = "grey";
	this->fact=ct->return_color_table(this->get_string(tagname2,default2));
//Sets the font to Arial 12, bold.
	font_name="arial-12-bold";


	
	border=20;
  char tt[24];
  sprintf(tt,"%d",inum);
  iview_s=tt;
  overlay=false;
  alpha=128;
  fact_o=0;
  over_c="rainbow";
  drawit=new draw_what();
  build_overlay_fact();
  my_cmap=new my_colors();
  

  idat_o=0;
  set_background_color("white");

  set_pen_color("black");

  com.push_back(iview_s); for(int i=0; i<4;i++) com.push_back("");

  allv.push_back("THREE"); allv.push_back("FRONT");
  allv.push_back("SIDE");  allv.push_back("TOP");
  //allv.push_back("CUT"); 
  allv.push_back("CUBE");
  myv_o=0;
  first=false;
}
void DrawWindow::jpeg_save(QString file){
 QFileInfo qfileinfo (file);
  pmScaled.save (file, "PPM");
}
/*
	Updates the font.
*/
void DrawWindow::update_font(QString fst)
{
  delete fm; delete myf;
  if(fst.contains("12-bold")>0){ 
    myf=new QFont("Arial", 12, QFont::Bold);
    font_name=fst;  fm=new QFontMetrics( *myf );

  }
  else if(fst.contains("14-bold")>0){ 
    myf=new QFont("Arial", 14, QFont::Bold);
    font_name=fst;  fm=new QFontMetrics( *myf );

  }
    else if(fst.contains("16-bold")>0){ 
    myf=new QFont("Arial", 16, QFont::Bold);
    font_name=fst;  fm=new QFontMetrics( *myf );

  }
    else if(fst.contains("18-bold")>0){ 
    myf= new QFont("Arial", 18, QFont::Bold);
    font_name=fst;  fm=new QFontMetrics( *myf );

  }
  else{
   myf=new QFont("Arial", 12, QFont::Normal);
   font_name="arial-12-normal";  fm=new QFontMetrics( *myf);

  }
      myv->set_new_font(fm);
    if(overlay) myv_o->set_new_font(fm);

}
void DrawWindow::resize_view(int ix,int iy){
   myv->resize_view(ix,iy);
   if(overlay) myv_o->resize_view(ix,iy);
}
void DrawWindow::resize_view_beg(int ix,int iy){
 //  int i_x,i_y;
   myv->resize_view_beg(ix,iy);
   if(overlay) myv_o->resize_view_beg(ix,iy);
}
void DrawWindow::resize_view_end(int ix,int iy){
   myv->resize_view_end(ix,iy);
   if(overlay) myv_o->resize_view_end(ix,iy);
}

void DrawWindow::bound_ps(QString file,int bx, int ex, int by, int ey){

  QFile fin("/tmp/jUnK.eps");
  QFile fout(file);
  // Overwrite the bounding box
  fin.open(QIODevice::ReadOnly);
  fout.open(QIODevice::WriteOnly);

   Q_LONG size;

   QString s;
   char buf[101];
   while (!fin.atEnd()){
     size = fin.readLine(buf, 100);
     s = QString(buf);
     if (s.find("%%BoundingBox:") == 0){ 
       s.sprintf("%%%%BoundingBox: %d %d %d %d\n",bx,by,ex,ey);
       size=s.length();
       sprintf(buf,"%s",s.toAscii().constData());
     }
     fout.writeBlock(buf,size);
    }
    fout.close();
    fin.close();
   
}
void DrawWindow::ps_save(QString sm,QString file){
QPrinter *printer;
  if(sm.contains("small")>0){
  printer = new QPrinter( QPrinter::ScreenResolution );
  printer->setOutputFormat ( QPrinter::PostScriptFormat );
  }
  else{
   if(sm.contains("pdf")>0){
   
   }
    printer = new QPrinter( QPrinter::HighResolution );
    if(sm.contains("pdf")>0) {
    printer->setOutputFormat ( QPrinter::PdfFormat);
    }
   else{
   //printer->setOutputFormat ( QPrinter::PostScriptFormat );
   }
  }

  Q3PaintDeviceMetrics metrics( printer );
  printer->setColorMode(QPrinter::Color);
  printer->setOutputToFile ( true);
  printer->setOutputFormat(QPrinter::PostScriptFormat);
  printer->setOutputFileName ( "/tmp/jUnK.eps");
  printer->setOrientation(QPrinter::Portrait);
  printer->setFullPage (false );
  QPainter painter(printer);
  QPen pen;
  pen.setWidth(2);
  pen.setColor(this->pen_c); 
  painter.setFont(*myf);
  
  
 // painter.setBackgroundColor(this->back_c);
	//painter.eraseRect(0,0,pm.width(), pm.height());
//	int shift=100;
    
     
    int bx,ex,by,ey;
	int rangex=printer->pageRect().width(),rangey=printer->pageRect().height();
	float ratio_draw=(float)width()/(float)height();
	float ratio_page=(float)rangex/(float)rangey;
  int Wt,Ht;
 if(ratio_draw >ratio_page){
    Wt=rangex,Ht=(int)(rangex/ratio_draw);
    bx=24; ex=588;
    ey=768; by=768-(int)(564/ratio_draw);
 }
  else{
    ex=588; ey=768;
    by=24; bx=588-(int)(ratio_draw*768.);
    Ht=rangey; Wt=(int)(rangey*width()/height());
  }
	

	QFont font=painter.font();
  QFontMetrics fm2= painter.fontMetrics();
  myv->set_new_font(&fm2);
	if(overlay){
    myv_o->set_new_font(&fm2);
    myv->viewit(&painter,&pen,fact,dat,pos,0,Wt,0,Ht,drawit,false);

    myv_o->viewit(&painter,&pen,fact_o,dat_o,pos,0,Wt,0,Ht,drawit,true);
    myv_o->set_new_font(fm);
  }
  else{
   

    this->myv->viewit(&painter,&pen,fact,dat,pos,0,Wt,0,Ht,drawit,false);
    myv->set_new_font(fm);
  }
  painter.setPen(pen);
  painter.end();
 

  bound_ps(file,bx,ex,by,ey);
  myv->set_new_font(fm);
}
QString DrawWindow::iter_views(int iview){
  int i=0;

  for(int iv=0; iv < (int)allv.size(); iv++){
    if(allv[iv]==view_name){
       i=iv+iview;
     }
  
    }
  if(i==(int)allv.size()) i=0;
  if(i==-1) i=allv.size()-1;
  return allv[i];
  
}
void DrawWindow::set_o_data(int id){ 
  dat_o=datas->return_dat(id);idat_o=id;
  if(myv_o!=0) delete myv_o;
  delete myv;
  myv=view_by_name(view_name);
  myv_o=view_by_name(view_name);

  fact_o->set_bcolor(dat_o->get_bcolor());
 fact_o->set_ecolor(dat_o->get_ecolor());
}
void DrawWindow::set_draw_bar(bool db){
  drawit->draw_bar=db;
  if(myv_o!=0) {
    delete myv_o;
    myv_o=view_by_name(view_name);
  }
  delete myv;
  myv=view_by_name(view_name);
}
void DrawWindow::set_pos_transp(int db){
  drawit->draw_pos=db;

  if(myv_o!=0) {
    delete myv_o;
    myv_o=view_by_name(view_name);
  }
  delete myv;
  myv=view_by_name(view_name);
}
void DrawWindow::set_draw_grid(bool db){
  drawit->draw_grid=db;
  if(myv_o!=0) {
    delete myv_o;
    myv_o=view_by_name(view_name);
  }
  delete myv;
  myv=view_by_name(view_name);
}
void DrawWindow::set_draw_title(bool db){
  drawit->draw_title=db;
  if(myv_o!=0) {
    delete myv_o;
    myv_o=view_by_name(view_name);
  }
  delete myv;
  myv=view_by_name(view_name);
}
void DrawWindow::set_draw_picks(bool db){
  drawit->draw_picks=db;
  if(myv_o!=0) {
    delete myv_o;
    myv_o=view_by_name(view_name);
  }
  delete myv;
  myv=view_by_name(view_name);
}
void DrawWindow::set_draw_region(bool db){
  drawit->draw_region=db;
  if(myv_o!=0) {
    delete myv_o;
    myv_o=view_by_name(view_name);
  }
  delete myv;
  myv=view_by_name(view_name);
}

void DrawWindow::set_alpha(int a){
  alpha=a;
  build_overlay_fact();
}
void DrawWindow::set_overlay_color(QString o){ 
  over_c=o;
  build_overlay_fact();
}
void DrawWindow::set_data(int id){ 
  dat=datas->return_dat(id);idat=id;

  std::vector<QString> com; com.push_back(QString::number(idat)); com.push_back("menu");

  com.push_back("Clip");

  com.push_back("set_pts"); com.push_back(dat->return_clip());

  emit actionDetected(com);

  com[4]="set_histo";

  com.pop_back();

  QString junk=dat->return_histogram();
    std::vector<QString> com2; com2.push_back(QString::number(idat)); com2.push_back("menu");
  com2.push_back("Clip");
  com2.push_back("set_histo");
  com2.push_back(junk);
  emit actionDetected(com2);

   fact->set_bcolor(dat->get_bcolor());
 fact->set_ecolor(dat->get_ecolor());
  com2[2]="Orient";

  com2.push_back("new_data");

  std::vector<axis> ax=dat->return_io_hyper()->return_axes(8);
  int ns[8];for(int i=0; i < 8; i++) ns[i]=ax[i].n;
  com2[3]=util::string_from_int_array(8,ns);

  emit actionDetected(com2);
 }
void DrawWindow::initial_view(){
  set_data(idat);
  update_display();
 }

void DrawWindow::build_overlay_fact(){
  if(fact_o!=0) delete fact_o;
 
      fact_o=colort->return_overlay_color_table(over_c,alpha);
 if(1==3){
 
  if(over_c=="flag")      fact_o=(slice*)new flag(alpha);
  else if(over_c=="gray")    fact_o=(slice*)new grey(alpha);
  else if(over_c=="cgsi")    fact_o=(slice*)new cgsi(alpha);
  else if(over_c=="cbl")     fact_o=(slice*)new cbl(alpha);
  else if(over_c=="rainbow") fact_o=(slice*)new rainbow(alpha);
   else if(over_c=="cycle") fact_o=(slice*)new color_cycle(alpha);
 else {
    fact_o=(slice*) colort->return_color_table(over_c)->clone();
 }
 fact_o->set_overlay();
 }
 if(dat_o!=0){
 fact_o->set_bcolor(dat_o->get_bcolor());
 fact_o->set_ecolor(dat_o->get_ecolor());
 }
}
void DrawWindow::update_view(QString name){

float ix,iy;
 
 QString old=get_view_name();
 bool set=false;
 name=dat->validate_view(name,pos);

 if(old==name) set=true;
 this->myv->get_pcts(&ix,&iy);
 int c=myv->get_corner_sel();
  delete myv;
 
 this->myv=view_by_name(name);
 if(set) this->myv->set_pcts(ix,iy);
 myv->set_corner_sel(c);
 if(overlay){
   build_overlay_fact();
   if(myv_o!=0) delete myv_o;
   myv_o=view_by_name(name); 
   if(set) myv_o->set_pcts(ix,iy);
   myv_o->set_corner_sel(c);
 }
 if(fact!=0 && dat!=0) {
  fact->set_bcolor(dat->get_bcolor());
 fact->set_ecolor(dat->get_ecolor());
 }
// this->viewit("current");
}
view *DrawWindow::view_by_name(QString name){

 view *my;

 iview_s=QString::number(iview);
//ADD NEW VIEWS HERE

 if(!this->dat->restrict.contains("NONE")){
    name=this->dat->restrict;
 }

 
  name=dat->validate_view(name,pos);
  view_name=name;
//The FRONT, SIDE and TOP views all display one single face so the single class is called
 if(name=="FRONT") {my=new single(iview_s,0,1,fm,pk,mym,draw_o);}
 else if(name=="SIDE") my= new single(iview_s,0,2,fm,pk,mym,draw_o);
 else if(name=="TOP")  my=new single(iview_s,1,2,fm,pk,mym,draw_o);
//CUT, CUBE and THREE all call their own individual classes since each is a unique type of view. 
  else if(name=="CUT")  my=new cube_cut(iview_s,fm,pk,mym,draw_o);
  else if(name=="CUBE")  my=new cube(iview_s,fm,pk,mym,draw_o);
 else              {
   my=new three_face(iview_s,fm,pk,mym,draw_o);
   }

 return my;
}

void DrawWindow::set_colortable(QString ctable){
     fact=colort->return_color_table(ctable);
     wind_ctable=ctable;
       fact->set_bcolor(dat->get_bcolor());
 fact->set_ecolor(dat->get_ecolor());
     this->viewit("current");
}
void DrawWindow::set_windows(int n, std::vector<DrawWindow *>wids){
	 this->nwindows=n;
	 this->windows=wids;
}
	
	
void DrawWindow::draw_base_pixmap(){
	W = width();
	H = height();

    pm.resize(W,H);
	QPainter painter(&pm);
	QPen pen;
	pen.setWidth(2);
  if(!fact->is_raster) drawit->draw_bar=false;

	pen.setColor(this->pen_c); 
    painter.setBackgroundColor(this->back_c);
	painter.eraseRect(0,0,pm.width(), pm.height());
	painter.setFont(*myf);
    if(overlay){
      myv->viewit(&painter,&pen,fact,dat,pos,0,W,0,H,drawit,false);

      myv_o->viewit(&painter,&pen,fact_o,dat_o,pos,0,W,0,H,drawit,true);

    }
    else{
    
        this->myv->viewit(&painter,&pen,fact,dat,pos,0,W,0,H,drawit,false);
    }
    painter.setPen(pen);
    scale();
}
void DrawWindow::update_display(){
  /*
  std::vector<QString> com2; com2.push_back(iview_s); com2.push_back("menu");
  com2.push_back("Clip");
  com2.push_back("set_histo");
  com2.push_back(dat->return_histogram());

  emit actionDetected(com2);
  */
  QString valid= dat->validate_view(view_name,pos);
  //if(valid!=view_name
  update_view(valid);

	  this->draw_base_pixmap();
    update();
}
void DrawWindow::viewit(QString up){
  QString up2="none",up3;
  for(int i=0; i < (int)datas->return_ndat(); i++){
    up3=datas->return_dat(i)->data_updated();
    if(up3!="none"){
      if(up2!="none" || up3=="all") up2="all";
      up2=up3;
    }
  }
  if(up=="all" || up2=="all") up2="all";
  else if(up=="current" || up2=="current") up2="current";
  else if(up=="update") up2="update";
     



  if(up2=="current"){ 
	  this->draw_base_pixmap();
    update();
  }
  else if(up2=="update"){
    delete myv;
    myv=view_by_name(view_name);
    if(overlay){
      if(myv_o!=0) delete myv_o; 
      myv_o=view_by_name(view_name);
    }
  }
  else if(up2=="all"){
    for(int i=0; i < nwindows; i++) {
      if(i!=iview) {
         windows[i]->pos->sync_pos(pos);
         windows[i]->viewit("update");
      }
      windows[i]->viewit("current");
    }
  }
  std::vector<QString> com;
  com.push_back(tr("Window")); com.push_back(tr("set_active")) ;
  com.push_back(tr(iview_s.toAscii().constData()));
  emit actionDetected(com);
}
void DrawWindow::iter_data(int inc){
   idat=idat+inc;
   overlay=false;
   if(idat==(int)datas->return_ndat()) idat=0;
   if(idat ==-1) idat=datas->return_ndat()-1;
   set_data(idat);
   //name=dat->validate_view(name,pos); may be necessary

}
void DrawWindow::iter_o_data(int inc){
   idat_o=idat_o+inc;
   overlay=true;
   if(idat_o==(int)datas->return_ndat()) idat_o=0;
   if(idat_o ==-1) idat_o=datas->return_ndat()-1;
   set_o_data(idat_o);
   //name=dat->validate_view(name,pos); may be necessary

}
void DrawWindow::del_clip_pt(QString str){
  dat->del_pt(str.toInt());
  std::vector<QString> com; com.push_back(iview_s); com.push_back("menu");
  com.push_back("Clip");
  com.push_back("set_pts"); com.push_back(dat->return_clip());
  emit actionDetected(com);
}
int DrawWindow::add_clip_pt(QString str){
  int iret=dat->add_pt(str.section(":",0,0).toFloat(),str.section(":",1,1).toFloat());
  std::vector<QString> com; com.push_back(iview_s); com.push_back("menu");
  com.push_back("Clip");
  com.push_back("set_pts"); com.push_back(dat->return_clip());
  emit actionDetected(com);
  return iret;
}
QString DrawWindow::get_clip_pt(int ipt){
  float pct,clp;
  dat->get_pt(ipt,&pct,&clp);
  return QString::number(pct)+":"+QString::number(clp);
}

void DrawWindow::set_background_color(QString col){
  this->wind_bg=col;
  this->back_c=my_cmap->return_qcolor(col);
 if(!first) this->viewit("current");
	}
void DrawWindow::set_pen_color(QString newc){
  this->wind_ov=newc;
  this->pen_c=my_cmap->return_qcolor(newc);
  if(!first)this->viewit("current");
}

void DrawWindow::scale(){
    int h = height();

 //   if ( image->isNull() ) return;

    //QApplication::setOverrideCursor( waitCursor ); // this might take time
    if ( width() == pm.width() && h == pm.height() )
    {						// no need to scale if widget
	pmScaled = pm;				// size equals pixmap size
    } else {

	    QMatrix m;				// transformation matrix
	    m.scale(((double)width())/pm.width(),// define scale factors
		    ((double)h)/pm.height());
	    pmScaled = pm.xForm( m );		// create scaled pixmap
    }
	pmScaledCopy = pmScaled;
    QApplication::restoreOverrideCursor();	// restore original cursor
}
void DrawWindow::paintEvent( QPaintEvent *e ){

    if ( pm.size() != QSize( 0,  0 ) ) {		// is an image loaded?
	QPainter painter(this);
	painter.setClipRect(e->rect());
	painter.drawPixmap(0, 0, pmScaled);

 //   this->myv->viewit(&painter,&pen,fact,dat,pos,border,W-2*border,border,H-2*border);

    }
}
void DrawWindow::resizeEvent( QResizeEvent * ){
      if ( pm.size() == QSize( 0, 0 ) )		{// we couldn't load the image
     if(width()>20) update_display();
	return;
}

    if ( width() != pmScaled.width() || height() != pmScaled.height())
    {						// if new size,
	scale();				// scale pmScaled to window
	W = width(); H = height();
    }
    
}
/*
	Certain actions in Hyperview have shortcuts. The following two methods handle what must be done
	when a certain shortcut key is pressed and then released.
*/
void DrawWindow::keyPressEvent( QKeyEvent *e ){
  switch( e->key() )
  {
  default:
    emit actionDetected(this->myv->keyPressEvent(e,pos));
  }
}
void DrawWindow::keyReleaseEvent( QKeyEvent *e ){
  std::vector<QString> com;
  com.push_back(iview_s); com.push_back("none");
  com.push_back(""); com.push_back("");
  int locs[8],order[8];
 
  pos->get_orders(order);
  
   
  switch( e->key() )
  {
  case Qt::Key_C:
  
    this->fact=this->colort->cycle_color();
    update_display();
    break;
   case Qt::Key_A:
	  if ( e->modifiers() & Qt::ControlModifier) {
	      com[1]="main"; com[2]="attach"; com[3]="true";
	      actionDetected(com);
	  }
	  break;
  case Qt::Key_D:
	  if (e->modifiers() & Qt::ControlModifier) {	  
	      com[1]="main"; com[2]="attach"; com[3]="false";
	     actionDetected(com);
	  }
	  break;
  case Qt::Key_V:
    com[1]="view"; com[2]="display";com[3]=iter_views(1);
    actionDetected(com);
    break;
   case Qt::Key_Z:
     if ( e->state() & Qt::ShiftModifier) com[3]="Z";
     else com[3]="z";
      pos->get_locs(locs);
     com[1]="navigate"; com[2]="direction"; 
    actionDetected(com);
    break;
  case Qt::Key_Y:
     if ( e->state() & Qt::ShiftModifier) com[3]="Y";
     else com[3]="y";
      pos->get_locs(locs);
     com[1]="navigate"; com[2]="direction";
    actionDetected(com);
    break;
  case Qt::Key_X:
     if ( e->state() & Qt::ShiftModifier) com[3]="X";
     else com[3]="x";
      pos->get_locs(locs);
     com[1]="navigate"; com[2]="direction"; 
    actionDetected(com);
    break;
  case Qt::Key_Less:
    com[1]="view"; com[2]="robin"; com[3]="back";
    actionDetected(com);
    break;
  case Qt::Key_Greater:
      com[1]="view"; com[2]="robin"; com[3]="forward";
    actionDetected(com);
    break;
  default:
    std::vector<QString > a; a.push_back(iview_s); a.push_back("none");
   	
  }
  
}
/*
	Responds to the user pressing down a mouse button, and takes action according to which button was pressed.
*/
void DrawWindow::mousePressEvent(QMouseEvent *e){

  down_x = e->x();
	 down_y = e->y();
  float pctx=((float)down_x)/((float)width());
  float pcty=((float)down_y)/((float)height());
  //sprintf(tt,"MenuBar::%d",iview);;
  //emit actionDetected(tr(tt));
  if (e->button() == Qt::MidButton) {
     mouse_down=2;
     actionDetected(myv->m_mouse_d(pctx,pcty,pos,func));

  }
  else if (e->button() == Qt::LeftButton) {
     mouse_down=1;
     actionDetected(myv->l_mouse_d(pctx,pcty,pos,func));
  }
  else if (e->button() == Qt::RightButton) {
  actionDetected(myv->r_mouse_d(pctx,pcty,pos,func));
    mouse_down=3;
  }

}
/*
	Same as the above method except this one deals with the double clicking event and has a different response
	when the third (middle) button is pressed.
*/
void DrawWindow::mouseDoubleClickEvent(QMouseEvent *e){
  int x = e->x();
	int y = e->y();
	 	float pctx=((float)x)/((float)width());

  float pcty=((float)y)/((float)height());
 if (e->button() == Qt::LeftButton) {
   actionDetected(myv->l_mouse_double(pctx,pcty,pos,func));
  }
  else if (e->button() == Qt::RightButton) {
   actionDetected(myv->r_mouse_double(pctx,pcty,pos,func));
  }
  else{
   actionDetected(myv->m_mouse_double(pctx,pcty,pos,func));
   }
   timeit.start();

}

void DrawWindow::mouseMoveEvent(QMouseEvent *e){
  int x = e->x();
	int y = e->y();
	float pctx=((float)x)/((float)width());
  float pcty=((float)y)/((float)height());
  if (mouse_down==2) {
     actionDetected(myv->m_mouse_m(pctx,pcty,pos,func));

  }
  else if (mouse_down==1) {
     QString ab;
     actionDetected(myv->l_mouse_m(pctx,pcty,pos,func));
  }
  else actionDetected(myv->r_mouse_m(pctx,pcty,pos,func));
  

  
}
void DrawWindow::mouseReleaseEvent(QMouseEvent *e){

  int x = e->x();
	int y = e->y();
	
	if(timeit.elapsed() < 500) return;

  float pctx=((float)x)/((float)width());
  float pcty=((float)y)/((float)height());
  if (e->button() == Qt::MidButton) {
 //     actionDetected(myv->mid_mouse_release(pctx,pcty,pos,e));
       actionDetected(myv->m_mouse_r(pctx,pcty,pos,func));

  }
  else if (e->button() == Qt::LeftButton) {
 // 	if(abs(x-down_x) < 4 || (y-down_y)<4) return;
   //  actionDetected(myv->left_mouse_release(pctx,pcty,pos,e));
     actionDetected(myv->l_mouse_r(pctx,pcty,pos,func));
  }
  else if (e->button() == Qt::RightButton) {
  //   actionDetected(myv->right_mouse_release(pctx,pcty,pos,e));
     actionDetected(myv->r_mouse_r(pctx,pcty,pos,func));
  }
  this->myv->clear_mouse();
}
void DrawWindow::get_view_pcts(float *pt, float *pf){
  myv->get_pcts(pt,pf);
}
void DrawWindow::set_view_pcts(float pt, float pf){
 myv->set_pcts(pt,pf);
  if(overlay) myv_o->set_pcts(pt,pf);
}
QString DrawWindow::get_string(char *beg, QString def){
  char temp_ch[128];
  sprintf(temp_ch,"%s%d",beg,iview);
  std::string temp= this->pars->get_string(temp_ch,def.toAscii().constData());
  QString ret=temp.c_str();
  return ret;
}
int DrawWindow::get_int(char *beg, int def){
  char temp_ch[128];
  sprintf(temp_ch,"%s%d",beg,iview);
  QString v=temp_ch;
  return this->pars->get_int(v,def);

}
int *DrawWindow::get_ints(char *beg, int max, int *def){
  char temp_ch[128];
  sprintf(temp_ch,"%s%d",beg,iview);
  return this->pars->get_ints(temp_ch,max,def);
 

}
