#include <qpainter.h>
#include "panel.h"
#include "single.h"
#include "multiface.h"
#include "cube.h"
#include "cube_cut.h"
#include "threeface.h"
#include "view_types.h"
using namespace SEP;

panel::panel(int inum, std::shared_ptr<paramObj>pr,std::shared_ptr<position>p,
std::shared_ptr<pick_draw>_pks, std::shared_ptr<datasets>ds, int idata,  std::shared_ptr<slice_types>ct,  
std::shared_ptr<mouse_func>f,  std::shared_ptr<orientation_server> server, std::shared_ptr<orients>oo, 
std::shared_ptr<maps> mym) {

    set_mouse_func(f);  //Set how the mouse function works for this panel
    idat=idata;  //Setup which dataset we will display by default
    datas=ds;   //Hold the list of all datasets
    dat=datas->return_dat(idat);  //Primary dataset to display
    dat_o=0;  //No overlaid dataset to begin with
    serv=server;
    std::shared_ptr<draw_other> mydo(new draw_other());
    draw_o=mydo;  //What we are going to display in a view
    update_it=false;
    std::shared_ptr<annotate> myao(new annotate());
    ano=myao;
    pars=pr;  //Parameter reading class
    pk=_pks;  //Picks 
    my_maps=mym;
    this->colort=ct; //All the different ways to display a slice
    iview=inum;  //Which view number I am
    my_or=oo;

    //What order to display the axes
    std::vector<int> ord;
    for(int i=0; i<8; i++) ord.push_back(i+1);
    std::vector<int> order=pars->getInts(std::string("order")+std::to_string(iview),ord);
    
    //Setup the orientation cube class
    std::shared_ptr<orient_cube> op(new orient_cube(p,order,server));
    pos=op;
    my_or->add_orient(pos);
   
    std::shared_ptr<my_fonts> myfff(new my_fonts());

    fonts=myfff;   //Create font server object
    font_name="Arial-12-Normal";  //Set the default font name
    
    myf=fonts->return_font(font_name); //Get that font info
    
    fm=fonts->return_font_metric(font_name);

   //Set the proportions -1 when means they haven't been specified
   for(int i=0; i < 8; i++) proportions.push_back(-1);

   //Set the parameter for the grid for multi panel displays
   jplane=1; grid1=1;grid2=1;
    
   first=true;  //Whether or not this is the initial creation

   //Sets the initial view to THREE and create it
   this->myv=view_by_name(pars->getString(std::string("view")+std::to_string(iview),std::string("THREE")));
   //Sets the initial data color to grey.
   std::string col=pars->getString(std::string("color")+std::to_string(iview),"grey");
   this->fact=ct->return_color_table(col);

   oversamp=1.;
   map=0;


  iview_s=QString::number(inum); //View in QString

  overlay=false; //We start by not overlaying anything
  fact_o=0;//No overlay factory to begin with
    over_c="rainbow";// Default overlay style will be rainbow
  idat_o=0;  //Set the default overlay to the first dataset
  myv_o=0; //No initial overlay view

  build_overlay_fact(); //Build overlay factor
    set_alpha(128) ; //Start with 50/50 transparency

  std::shared_ptr<draw_what> dw(new draw_what());
  std::shared_ptr<my_colors> my(new my_colors());
  drawit=dw;  //what to draw
  my_cmap=my; //What colors to use
  

  
   set_pen_color("black");

  com.push_back(iview_s); for(int i=0; i<7;i++) com.push_back("");

  //Setup the possible views
  view_types x=view_types();
  std::map<QString,bool>  *myl=x.return_views();
  for(std::map<QString,bool>::iterator i=myl->begin(); i!=myl->end(); i++) allv.push_back(i->first);

  myv_o=0;
  first=false;
}

std::shared_ptr<panel>panel::clone(int in){
  std::shared_ptr<panel>x( new panel(in,pars,pos,pk,datas,idat,colort,func,serv,my_or,my_maps));
  return x;
}
void panel::set_alpha(int a){
  alpha=a;
  build_overlay_fact();
}
void panel::update_font(QString fst){
  fm=fonts->return_font_metric(fst);
  myf=fonts->return_font(fst);
  font_name=fst;
  myv->set_new_font(fm);
  if(overlay) myv_o->set_new_font(fm);

}
void panel::moving_corner(int ix,int iy){
   myv->resize_view(ix,iy);

   if(overlay) myv_o->resize_view(ix,iy);
}
void panel::move_corner_beg(int ix,int iy){
 //  int i_x,i_y;

   myv->resize_view_beg(ix,iy);
   if(overlay) myv_o->resize_view_beg(ix,iy);
}
void panel::move_corner_end(int ix,int iy){
   myv->resize_view_end(ix,iy);
   if(overlay) myv_o->resize_view_end(ix,iy);
}
QString panel::iter_views(int iview){
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
void panel::set_o_data(int id){ 
  if(id==-1){
     return;
  }
  dat_o=datas->return_dat(id);idat_o=id;
  myv=view_by_name(view_name);
  myv_o=view_by_name(view_name);

  fact_o->set_bcolor(dat_o->get_bcolor());
 fact_o->set_ecolor(dat_o->get_ecolor());
}
void panel::set_draw_bar(bool db){
std::cerr<<db<<std::endl;
  drawit->draw_bar=db;
  if(myv_o!=0) {
    myv_o=view_by_name(view_name);
  }
  }

void panel::set_pos_transp(int db){
  drawit->draw_pos=db;

  if(myv_o!=0) {
    myv_o=view_by_name(view_name);
  }
  myv=view_by_name(view_name);
}
void panel::set_draw_grid(bool db){
  drawit->draw_grid=db;
  if(myv_o!=0) {
    myv_o=view_by_name(view_name);
  }
  myv=view_by_name(view_name);
}
void panel::set_draw_title(bool db){
  drawit->draw_title=db;
  if(myv_o!=0) {
    myv_o=view_by_name(view_name);
  }
  myv=view_by_name(view_name);
}
void panel::set_draw_axis1(bool db){
  drawit->draw_axis1=db;
  if(myv_o!=0) {
    myv_o=view_by_name(view_name);
  }
  myv=view_by_name(view_name);
}
void panel::set_draw_axis2(bool db){
  drawit->draw_axis2=db;
  if(myv_o!=0) {
    myv_o=view_by_name(view_name);
  }
  myv=view_by_name(view_name);
}
void panel::set_draw_axis3(bool db){
  drawit->draw_axis3=db;
  if(myv_o!=0) {
    myv_o=view_by_name(view_name);
  }
  myv=view_by_name(view_name);
}
void panel::set_draw_picks(bool db){
  drawit->draw_picks=db;
  if(myv_o!=0) {
    myv_o=view_by_name(view_name);
  }
  myv=view_by_name(view_name);
}
void panel::set_overlay_color(QString o){ 
  over_c=o;
  build_overlay_fact();
}
void panel::set_data(int id){ 
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

  std::vector<axis> ax=dat->return_io_hyper()->getAxes(8);
  int ns[8];for(int i=0; i < 8; i++) ns[i]=ax[i].n;
  com2[3]=util::string_from_int_array(8,ns);

  emit actionDetected(com2);
  
    QString valid= dat->validate_view(view_name,pos);
  //if(valid!=view_name
  update_view(valid);
  
 }
void panel::initial_view(){
  set_data(idat);
  update_display();
 }
void panel::build_overlay_fact(){

      fact_o=colort->return_overlay_color_table(over_c,alpha);
 if(dat_o!=0){
 fact_o->set_bcolor(dat_o->get_bcolor());
 fact_o->set_ecolor(dat_o->get_ecolor());
 }
}

void panel::viewit(QString up){
 if(up==QString("")){;}
}
/*
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
    for(int i=0; i < panels->; i++) {
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
void panels::set_windows(int n, std::vector<DrawWindow *>wids){
         this->nwindows=n;
         this->windows=wids;
}
*/
void panel::update_view(QString name){

float ix,iy;
 
 QString old=get_view_name();
 bool set=false;
 name=dat->validate_view(name,pos);

 if(old==name) set=true;
 this->myv->get_pcts(&ix,&iy);
 int c=myv->get_corner_sel();
 
 this->myv=view_by_name(name);
 if(set) this->myv->set_pcts(ix,iy);
 myv->set_corner_sel(c);
 if(overlay){
   build_overlay_fact();
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
std::shared_ptr<view> panel::view_by_name(QString name){

 std::shared_ptr<view> my;

 iview_s=QString::number(iview);
//ADD NEW VIEWS HERE

 if(!this->dat->restrict.contains("NONE")){
    name=this->dat->restrict;
 }

  name=dat->validate_view(name,pos);
  view_name=name;
//The FRONT, SIDE and TOP views all display one single face so the single class is called
 if(name=="FRONT") {
   std::shared_ptr<single> myfront(new single(iview_s,0,1,fm,pk,draw_o,proportions));
   my=myfront;
 }
 else if(name=="SIDE") {
   std::shared_ptr<single> myside(new single(iview_s,0,2,fm,pk,draw_o,proportions));
   my=myside;
  }
 else if(name=="TOP") {
   std::shared_ptr<single> mytop(new single(iview_s,1,2,fm,pk,draw_o,proportions));
   my=mytop;
  }
//CUT, CUBE and THREE all call their own individual classes since each is a unique type of view. 
  else if(name=="CUT") {
    std::shared_ptr<cube_cut> mycut(new cube_cut(iview_s,fm,pk,draw_o,proportions));
    my=mycut;
  }
  else if(name=="CUBE") {
    std::shared_ptr<cube> mycube(new cube(iview_s,fm,pk,draw_o,proportions));
    my=mycube;
  }
  else if(name=="MULTI")  {
      std::shared_ptr<multi_face> mymf(new multi_face(iview_s,fm,pk,draw_o,grid1,grid2,jplane));
      my=mymf;
  }
 else              {
   std::shared_ptr<three_face> myt(new three_face(iview_s,fm,pk,draw_o,proportions));
   my=myt;
 }

 return my;
}
void panel::set_colortable(QString ctable){
     fact=colort->return_color_table(ctable);
     wind_ctable=ctable;
       fact->set_bcolor(dat->get_bcolor());
 fact->set_ecolor(dat->get_ecolor());
     this->viewit("current");
}
void panel::iter_data(int inc){
   idat=idat+inc;
   overlay=false;
   if(idat==(int)datas->return_ndat()) idat=0;
   if(idat ==-1) idat=datas->return_ndat()-1;
   set_data(idat);
   //name=dat->validate_view(name,pos); may be necessary

}
void panel::iter_o_data(int inc){
   idat_o=idat_o+inc;
   overlay=true;
   if(idat_o==(int)datas->return_ndat()) idat_o=0;
   if(idat_o ==-1) idat_o=datas->return_ndat()-1;
   set_o_data(idat_o);
   //name=dat->validate_view(name,pos); may be necessary

}
void panel::del_clip_pt(QString str){
  dat->del_pt(str.toInt());
  std::vector<QString> com; com.push_back(iview_s); com.push_back("menu");
  com.push_back("Clip");
  com.push_back("set_pts"); com.push_back(dat->return_clip());
  emit actionDetected(com);
}
int panel::add_clip_pt(QString str){
  int iret=dat->add_pt(str.section(":",0,0).toFloat(),str.section(":",1,1).toFloat());
  std::vector<QString> com; com.push_back(iview_s); com.push_back("menu");
  com.push_back("Clip");
  com.push_back("set_pts"); com.push_back(dat->return_clip());
  emit actionDetected(com);
  return iret;
}
QString panel::get_clip_pt(int ipt){
  float pct,clp;
  dat->get_pt(ipt,&pct,&clp);
  return QString::number(pct)+":"+QString::number(clp);
}
void panel::set_pen_color(QString newc){
  this->wind_ov=newc;
  this->pen_c=my_cmap->return_qcolor(newc);
  if(!first)this->viewit("current");
}	
void panel::get_view_pcts(float *pt, float *pf){
  myv->get_pcts(pt,pf);
}
void panel::set_view_pcts(float pt, float pf){
 myv->set_pcts(pt,pf);
  if(overlay) myv_o->set_pcts(pt,pf);
}



void panel::update_display(){
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
 
	  this->draw_base_pixmap(paint);
}	
void panel::draw_base_pixmap(QPainter *painter){
	if(!fact->is_raster) drawit->draw_bar=false;

  fact->set_bcolor(dat->get_bcolor());
 fact->set_ecolor(dat->get_ecolor());

    QPen pen;
  pen.setWidth(2);

	pen.setColor(this->pen_c); 

	painter->eraseRect(begx,begy,endx,endy);

	painter->setFont(*myf);

    if(overlay){
      myv->viewit(painter,&pen,fact,dat,pos,begx,endx,begy,endy,drawit,false);
      myv_o->viewit(painter,&pen,fact_o,dat_o,pos,begx,endx,begy,endy,drawit,true);
    }
    else{

       this->myv->viewit(painter,&pen,fact,dat,pos,begx,endx,begy,endy,drawit,false);

    }

    painter->setPen(pen);
}
void panel::keyPressEvent( QKeyEvent *e ){
  switch( e->key() )
  {
  default:
    emit actionDetected(this->myv->keyPressEvent(e,pos));
  }
}
void panel::keyReleaseEvent( QKeyEvent *e ){
  std::vector<QString> com;
  com.push_back(iview_s); com.push_back("none");
  com.push_back(""); com.push_back("");com.push_back("");
  int locs[8],order[8];
  com[4]="1";
  pos->get_orders(order);
   QString myc;
  switch( e->key() )
  {
  case Qt::Key_C:
   
    myc=this->colort->cycle_color();
    com[1]="view"; com[2]="colortable";com[3]=myc;
    emit actionDetected(com);
    break;
  
  case Qt::Key_V:
    com[1]="view"; com[2]="type";com[3]=iter_views(1);
    emit actionDetected(com);
    break;
   case Qt::Key_Z:
     if ( e->modifiers() & Qt::ShiftModifier) com[3]="Z";
     else com[3]="z";
      pos->get_locs(locs);
     com[1]="navigate"; com[2]="direction"; 
    emit actionDetected(com);
    break;
  case Qt::Key_Y:
     if ( e->modifiers() & Qt::ShiftModifier) com[3]="Y";
     else com[3]="y";
      pos->get_locs(locs);
     com[1]="navigate"; com[2]="direction";
    emit actionDetected(com);
    break;
  case Qt::Key_X:
     if ( e->modifiers() & Qt::ShiftModifier) com[3]="X";
     else com[3]="x";
      pos->get_locs(locs);
     com[1]="navigate"; com[2]="direction"; 
    emit actionDetected(com);
    break;
  case Qt::Key_Less:
    com[1]="view"; com[2]="roundrobin"; com[3]="back";
    emit actionDetected(com);
    break;
  case Qt::Key_Greater:
      com[1]="view"; com[2]="roundrobin"; com[3]="forward";
    emit actionDetected(com);
    break;
  default:
    std::vector<QString > a; a.push_back(iview_s); a.push_back("none");
   	
  }
  
}
void panel::pdf_save(QPainter *painter){

  QPen pen;
  pen.setWidth(1);
  pen.setColor(this->pen_c);
  painter->setFont(*myf);
  QFont font=painter->font();
  QFontMetrics fm2=painter->fontMetrics();
  std::shared_ptr<QFontMetrics> fm3(new QFontMetrics(fm2));
  myv->set_new_font(fm3);

	if(overlay){
    myv_o->set_new_font(fm3);
    myv->viewit(painter,&pen,fact,dat,pos,begx,endx,begy,endy,drawit,false);

    myv_o->viewit(painter,&pen,fact_o,dat_o,pos,begx,endx,begy,endy,drawit,true);
    myv_o->set_new_font(fm);
  }
  else{
   
    this->myv->viewit(painter,&pen,fact,dat,pos,begx,endx,begy,endy,drawit,false);
    myv->set_new_font(fm);
  }

  myv->set_new_font(fm);

}
void panel::mousePressEvent(QMouseEvent *e){


  down_x = e->x();
	 down_y = e->y();
	   float pctx=((float)(down_x-begx))/((float)(endx-begx));
  float pcty=((float)(down_y-begy))/((float)(endy-begy));
  if (e->button() == Qt::MidButton  || ( e->modifiers() & Qt::ControlModifier && e->button() == Qt::LeftButton)) {
     mouse_down=2;
     actionDetected(myv->mid_mouse_down(pctx,pcty,pos,func));

  }
  else if (e->button() == Qt::LeftButton) {
     mouse_down=1;
     actionDetected(myv->left_mouse_down(pctx,pcty,pos,func));
  }
  else if (e->button() == Qt::RightButton) {
  actionDetected(myv->right_mouse_down(pctx,pcty,pos,func));
    mouse_down=3;
  }

}
void panel::mouseDoubleClickEvent(QMouseEvent *e){
  int x = e->x();
	int y = e->y();
  float pctx=((float)(x-begx))/((float)(endx-begx));
  float pcty=((float)(y-begy))/((float)(endy-begy));
  if (e->button() == Qt::MidButton  || ( e->modifiers() & Qt::ControlModifier && e->button() == Qt::LeftButton)){
    actionDetected(myv->mid_mouse_double(pctx,pcty,pos,func));

 }
 else if (e->button() == Qt::LeftButton) {
   actionDetected(myv->left_mouse_double(pctx,pcty,pos,func));
  }
  else if (e->button() == Qt::RightButton) {
   actionDetected(myv->right_mouse_double(pctx,pcty,pos,func));
  }

   timeit.start();

}
void panel::mouseMoveEvent(QMouseEvent *e){
  int x = e->x();
	int y = e->y();
  float pctx=((float)(x-begx))/((float)(endx-begx));
  float pcty=((float)(y-begy))/((float)(endy-begy));

   if(timeit.elapsed() >200){
  if (mouse_down==2) {
     actionDetected(myv->mid_mouse_move(pctx,pcty,pos,func));

  }
  else if (mouse_down==1 ) {
     QString ab;
     actionDetected(myv->left_mouse_move(pctx,pcty,pos,func));
  }
  else actionDetected(myv->right_mouse_move(pctx,pcty,pos,func));
  timeit.restart();
  }

  
}
void panel::mouseReleaseEvent(QMouseEvent *e){

  int x = e->x();
	int y = e->y();

  float pctx=((float)(x-begx))/((float)(endx-begx));
  float pcty=((float)(y-begy))/((float)(endy-begy));
if (e->button() == Qt::MidButton  || ( e->modifiers() & Qt::ControlModifier && e->button() == Qt::LeftButton)){
     actionDetected(myv->mid_mouse_release(pctx,pcty,pos,func));

  }
  else if (e->button() == Qt::LeftButton) {
 // 	if(abs(x-down_x) < 4 || (y-down_y)<4) return;
   actionDetected(myv->left_mouse_release(pctx,pcty,pos,func));
  }
  else if (e->button() == Qt::RightButton) {

  actionDetected(myv->right_mouse_release(pctx,pcty,pos,func));
  }
  this->myv->clear_mouse();
}
void panel::delete_panel(){


}
#ifdef CHEVRON
void panel::check_set_distort(){
	QString tag=this->get_string("map1_distort","NONE");
	if(tag!="NONE"){
      sregf fl=sregf(tag.toAscii().constData());
     int ns[8]; pos->get_ns(ns);
     long long sz=1;
     for(int i=0; i < 8; i++) sz=sz*(long long)ns[i];
     assert(sz==fl.get_n123()) ;
     oversamp=pars->get_float("oversamp",1.);
     hypercube_float buf=hypercube_float(&fl);
     map=new int[sz];
     fl.read_all(tag.toAscii().constData(),&buf);
     long long i=0;
     for(long long i2=0; i2 < sz/(long long)ns[0]; i2++){
       for(int i1=0 ;   i1 < ns[0]; i1++,i++){
           map[i]=(int)(oversamp*(buf.vals[i]-1.));
           if(map[i] < 0) map[i]=0;
           if(map[i]>= ns[0]) map[i]=ns[0]-1;   
       }
     }
     pos->set_one_shift(0,oversamp,map);
	}
}
#endif

void panel::change_map(int imap){
   std::shared_ptr<map_1> mp=my_maps->return_map(imap);
   pos->set_one_shift(0,mp->return_oversamp(),mp->return_index());
}

