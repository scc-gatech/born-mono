#include <unistd.h>
#include  "updater.h"
//Added by qt3to4:
#include <QCloseEvent>
#include "util.h"
#include "partial_data.h"
#include "outcore_data.h"
#include "incore_data.h"
#include "sreg.h"
#include "sep_reg_io.h"
#include "slice.h"
#include "wiggle.h"
#include "contour.h"
#include "surface_data.h"
#ifdef CHEVRON
#include "chev_strat_data.h"
#endif
#include <iostream>
updater::updater(QObject *parent,int nv,std::vector<DrawWindow*>ws, 
 datasets *d,param_func *pr,pick_draw *pks,slice_types *st,const char* name):QObject(parent,name){

  windows=ws;
  active=windows[0];
  locked=0;
  iactive=0;
  nviews=nv*2-1;

  pars=pr;
  ibeg=iactive;
  iend=iactive+1;
  movie_delay= 0;
  pk=pks;
  write=true;
  autop=new autopick(pk);
  datas=d;
  stype=st;

  snap="no";
  for(int i=0; i< 6; i++) com.push_back("");
  movie_timer = new QTimer( this, "movie" );
    connect( movie_timer, SIGNAL(timeout()), this,  SLOT(update_movie()) ); 
}
void updater::actionRespond(std::vector<QString> command){
  QString up="none";

  int it=command[0].toInt();
  if(it>=0 && it <nviews) iactive=it;

  active=windows[iactive];
  if(locked==0) {ibeg=iactive; iend=iactive+1;}
  com[0]=command[0];
//for(int i=0; i< (int)command.size(); i++) fprintf(stderr,"%s&",command[i].ascii());
 //fprintf(stderr,"\n");
  up="none";
  if(command[1].contains("color")>0) up=color_respond(command);

  else if(command[1].contains(tr("main"))>0) up=main_respond(command);
  else if(command[1].contains(tr("orient"))>0) up=orient_respond(command);
  else if(command[1].contains(tr("navigate"))>0) up=navigate_respond(command);
  else if(command[1].contains(tr("pick"))>0) up=pick_respond(command);
  else if(command[1].contains(tr("view"))>0) up=view_respond(command);
  else if(command[1].contains(tr("clip"))>0) up=clip_respond(command);
  else if(command[1].contains(tr("redraw"))>0) up=command[2];
  else if(command[1].contains(tr("history"))>0) up=history_respond(command);
  else if(command[1].contains(tr("auto"))>0) up=auto_respond(command);
  else if(command[1].contains(tr("ctable"))>0) up=ctable_respond(command);
  #ifdef CHEVRON
    else if(command[1].contains(tr("chev"))>0) up=chev_respond(command);
  #endif
  else if(command[1].contains(tr("sleep")) >0){
     sleep(command[2].toInt());
  }
  
  
  if(up=="none") active->update_display();
  if(up=="all"){ sync_orient();
    for(int i=0; i< nviews;i++) windows[i]->update_display();
  }
  if(up=="limited") for(int i=ibeg; i< iend;i++) windows[i]->update_display();
  if(up=="only") active->update_display();
   if(it>0 && it <nviews) iactive=it;
}
QString updater::history_respond(std::vector<QString>command){
  QString c;
  write=false;
  for(int i=0; i< 1+command[2].toInt(); i++){
    c=hist[hist.size()-1].output_back();
    int nelem=c.count("-");
    std::vector<QString> command;
    for(int i=0; i < nelem+1; i++) command.push_back(c.section("-",i,i));
    emit actionDetected(command);
    hist.pop_back();
  }
  write=true;
  return "all";
}
/*
	Responds to the command vector sent by the autopick menu. Takes the appropriate actions.
*/
QString updater::auto_respond(std::vector<QString>command){
  if(command[2].contains("method")>0){
    QString old=autop->get_method();
    if(write)hist.push_back(change(3,command,old,command[3]));
    autop->set_method(command[3]);
    return "none";
  }
  else if(command[2].contains("plane")>0){
    QString old=autop->get_plane();
    if(write)hist.push_back(change(3,command,old,command[3]));
    autop->set_plane(command[3]);
    return "none";
  }
 
  else if(command[3].contains("accept")>0){
    QString old;
    if(command[4].contains("do")){
      old=QString::number(autop->get_accept());
      autop->accept_picks();
    }
    else{
       old="do";
       autop->set_accept(command[4].toInt());
    }
    if(write)hist.push_back(change(4,command,old,command[3]));
    return "none";
  }
    else if(command[3].contains("decline")>0){
    QString old;
    if(command[4].contains("do")){
      old=QString::number(autop->get_accept());
      autop->decline_picks();
    }

    if(write)hist.push_back(change(4,command,old,command[3]));
    return "all";
  }
  else if(command[3].contains("extend")>0){
    QString old;
    if(command[4].contains("do")){
      old=QString::number(autop->get_auto());
      autop->extend_picks(active->myv,active->pos);
    }
    else{
       old="do";
       pk->delete_picks(active->pos,command[4].toInt());
       autop->decrease_auto();
    }
    if(write)hist.push_back(change(4,command,old,command[3]));
    return "all";
  }
  else if(command[3].contains("run2d")>0){
    QString old;
    if(command[4].contains("do")){
      old=QString::number(autop->get_auto());
      
      autop->pick_2d(active->myv,active->pos);
    }
    else{
       old="do";
       pk->delete_picks(active->pos,command[4].toInt());
              autop->decrease_auto();

    }
    if(write)hist.push_back(change(4,command,old,command[3]));
    return "all";
  }
  else if(command[2].contains("type")>0){
    bool doit=autop->get_correlate();
    QString old;
    if(doit) old="correlation";
    else old="amplitude";
    if(command[3].contains("amplitude")) autop->set_amplitude();
    else autop->set_correlate();
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "none";
  }
  else if(command[2].contains("cor")>0){
    QString old=QString::number(autop->get_ncor());
    autop->set_ncor(command[3].toInt());
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "none";
  }
  else if(command[2].contains("dist")>0){
    QString old=QString::number(autop->get_nsearch());
    autop->set_nsearch(command[3].toInt());
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "none";
  }
    else if(command[2].contains("error")>0){
    QString old=QString::number(autop->get_error());
    autop->set_error(command[3].toInt());
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "none";
  }
  return "none";
 }
/*
	Responds to the command vector sent by any method in the view menu.
*/
QString updater::view_respond(std::vector<QString>command){
  QString old;
 
   if(command[2].contains("save")>0){
    if(command[3].contains("jpeg")>0){

      if(command[4]!="NONE") active->jpeg_save(command[4]);

      if(write)hist.push_back(change(4,command,"NONE",command[4]));
    }
    else if(command[3].contains("pdf")>0){
      if(command[5]!="NONE") active->ps_save("pdf",command[5]);
      if(write)hist.push_back(change(5,command,"NONE",command[5]));
    }
    else if(command[3].contains("ps")>0){
      if(command[5]!="NONE") active->ps_save(command[4],command[5]);
      if(write)hist.push_back(change(5,command,"NONE",command[5]));
    }

    return "none";
  }
  else if(command[2].contains("data")>0){
    int idat=active->get_idat();
     for(int i=0; i< 8; i++) {
        if(command[3].contains(QString::number(i)+":")){
           active->set_data(i);
        }
      }
    if(write) hist.push_back(change(3,command,QString::number(idat)+":",command[3]));
        return "only";

  }
  else if(command[2].contains("pos_transp")>0){
    int old_transp=active->get_pos_transp();
    active->set_pos_transp(command[3].toInt());
    if(write) hist.push_back(change(3,command,QString::number(old_transp),command[3]));
        return "only";

  }
  else if(command[2].contains("overlay")>0){
    bool over=active->get_overlay();
    if(!over) old="none";
    else old=QString::number(active->get_idat_o())+":";
    if(command[3].contains("none")){
      active->set_overlay(false);
      
    }
    else{
          active->set_overlay(true);

      for(int i=0; i< 8; i++) {
        if(command[3].contains(QString::number(i)+":")){
           active->set_o_data(i);
        }
      }
     
    }
      if(write) hist.push_back(change(3,command,old,command[3]));

    return "only";
  }
  else if(command[2].contains("display")>0){
  

    old=active->get_view_name();
    //active->update_view(command[3]);
    for(int i=ibeg; i < iend; i++) windows[i]->update_view(command[3]);

    if(write) hist.push_back(change(3,command,old,command[3]));
    return "limited";
  }
  
  
  else if(command[2].contains("bar")>0){ 
    bool oldb=active->get_draw_bar(), newb=false;
    if(command[3].contains("yes") >0) newb=true;
    if(oldb==newb) return "none";
    old="yes";if(!oldb) old="no";
    //active->set_draw_bar(newb);
    for(int i=ibeg; i < iend; i++) windows[i]->set_draw_bar(newb);
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "limited";
  }
    else if(command[2].contains("title")>0){ 
    bool oldb=active->get_draw_title(), newb=false;
    if(command[3].contains("yes") >0) newb=true;
    if(oldb==newb) return "none";
    old="yes";if(!oldb) old="no";
    //active->set_draw_title(newb);
      for(int i=ibeg; i < iend; i++) windows[i]->set_draw_title(newb);
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "limited";
  }
      else if(command[2].contains("grid")>0){ 
    bool oldb=active->get_draw_grid(), newb=false;
    if(command[3].contains("yes") >0) newb=true;
    if(oldb==newb) return "none";
    old="yes";if(!oldb) old="no";
   // active->set_draw_grid(newb);
     for(int i=ibeg; i < iend; i++) windows[i]->set_draw_grid(newb);
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "limited";
  }
  
   else if(command[2].contains("region")>0){ 
      bool oldb=active->get_draw_region(), newb=false;
      if(command[3].contains("yes") >0) newb=true;
      if(oldb==newb) return "none";
      old="yes";if(!oldb) old="no";
      active->set_draw_region(newb);
      if(write) hist.push_back(change(3,command,old,command[3]));
      return "limited";
     }
    else if(command[2].contains("picks")>0){ 
      bool oldb=active->get_draw_picks(), newb=false;
      if(command[3].contains("yes") >0) newb=true;
      if(oldb==newb) return "none";
      old="yes";if(!oldb) old="no";
     // active->set_draw_picks(newb);
       for(int i=ibeg; i < iend; i++) windows[i]->set_draw_picks(newb);
      if(write) hist.push_back(change(3,command,old,command[3]));
      return "limited";
     }
  
  
   
  
  
  else if(command[2].contains("font")>0){
    old=active->get_font_name();
//    QString a=command[3]+"-"+command[4]+"-"+command[5];
   // active->update_font(a);
    
    for(int i=ibeg; i < iend; i++) windows[i]->update_font(command[3]);
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "only";
  }
  
  else if(command[2].contains("roundrobin")>0){
    int iter;
    if(command[3]=="forward") {old="back"; iter=1;}
    else {old="forward"; iter=-1;}
    active->iter_data(iter);
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "only";
  }
    else if(command[2].contains("roundorobin")>0){
    int iter;
              active->set_overlay(true);

    if(command[3]=="forward") {old="back"; iter=1;}
    else {old="forward"; iter=-1;}

    active->iter_o_data(iter);
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "only";
  }


  
  return "none";
}
/*
	Responds to the command vector sent by any method in the navigate menu.  
*/
QString updater::navigate_respond(std::vector<QString> command){
  int loc[8],new_loc[8];
  
  active->pos->get_locs(loc);
  QString old_loc=util::string_from_int_array(8,loc);
     int ndir=0,odir=active->pos->get_movie_dir();
    int nax=1,oax=active->pos->get_movie_axis();
    

  if(command[2].contains("move")>0){
    int *new_loc=util::int_array_from_string(command[3]);
    for(int i=ibeg; i < iend; i++) windows[i]->pos->set_locs(new_loc);
    delete [] new_loc;
    if(write) hist.push_back(change(3,command,old_loc,command[3]));
    return "all";
  }
  else if(command[2].contains("movie")>0){
    if(command[3].contains("ax_dir")>0){
      int *axd=util::int_array_from_string(command[4]);
      nax=axd[0]; ndir=axd[1]; 
      for(int i=0; i < nviews; i++) windows[i]->pos->set_movie_dir(nax,ndir);
      axd[0]=oax; axd[1]=odir;
      if(write) hist.push_back(change(4,command,util::string_from_int_array(2,axd),command[4]));
      return "all";
    }
    else if(command[3].contains("go")>0) {
		movie_delay = command[4].toInt();
		run_movie(); return "none";}
    else if(command[3].contains("stop")>0){stop_movie(); return "none";}
  
  }
  else if(command[2].contains("range")>0){
    int beg[8],end[8];
    active->pos->get_ends(end);
    active->pos->get_begs(beg);

    QString old=util::string_from_int_array(8,beg)+":"+util::string_from_int_array(8,end);
    int *a=util::int_array_from_string(command[3]);
    for(int i=0;i<8;i++){ beg[i]=a[i]; end[i]=a[i+8];}
    active->pos->set_begs(beg); active->pos->set_ends(end);

 //   for(int i=0; i< 8; i++) loc[i]=(int)((beg[i]+end[i])/2.);
    active->pos->set_loc(a[16],(int)((beg[a[16]]+end[a[16]])/2.));
    active->pos->set_loc(a[17],(int)((beg[a[17]]+end[a[17]])/2.));

    delete[]a;
  // active->pos->set_locs(loc);
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "all";
  }
  else if(command[2].contains("step")>0){
      active->pos->update_movie_pos(active->return_dat()->io->return_hyper());
    active->pos->get_locs(new_loc);
 
    com[1]="navigate";
    com[2]="move"; 
    if(write) hist.push_back(change(3,com,old_loc,util::string_from_int_array(8,new_loc)));
    for(int i=0; i < nviews; i++) windows[i]->pos->set_locs(new_loc);
    return "all";
  }
  else if(command[2].contains("direction")>0){
    QString olds,news;
    com[1]="navigate";

     if(command[3].contains("x")>0){ nax=1; ndir=1;}
     else if(command[3].contains("X")>0){ nax=1; ndir=-1;}
     else if(command[3].contains("y")>0){ nax=2; ndir=1;}
     else if(command[3].contains("Y")>0){ nax=2; ndir=-1;}
     else if(command[3].contains("z")>0){ nax=0; ndir=1;}
     else if(command[3].contains("Z")>0){ nax=0; ndir=-1;}
     int move_axis=active->pos->get_order(nax);
     com[2]="movie";  com[3]="ax_dir";
     olds=QString::number(oax)+":"+QString::number(odir);
     news=QString::number(move_axis)+":"+QString::number(ndir);
     if(write) hist.push_back(change(4,com,olds,news));
     for(int i=0; i< nviews; i++) windows[i]->pos->set_movie_dir(move_axis,ndir);
     active->pos->update_movie_pos(active->return_dat()->io->return_hyper());
     active->pos->get_locs(new_loc);
     for(int i=0; i < nviews; i++) windows[i]->pos->set_locs(new_loc);
     com[2]="move"; 
     if(write) hist.push_back(change(3,com,old_loc,util::string_from_int_array(8,new_loc)));
     return "all";
  }
  return "none";
}
/*
	Responds to the command vector sent by any method in the color menu.
*/
QString updater::color_respond(std::vector<QString>command){
  QString old;
  wiggle *wigit;
  contour *cont;
  if(command[2].contains("colortable")>0){
    old=active->get_ctable();
    for(int i=ibeg; i < iend; i++) windows[i]->set_colortable(command[3]);
  }
  else if(command[2].contains("line_color")>0){
    old=active->get_pen_c();
    for(int i=ibeg; i< iend;i++) windows[i]->set_pen_color(command[3]);
  }
  else if(command[2].contains("background")>0){
    old=active->get_background(); 
    for(int i=ibeg; i< iend;i++) windows[i]->set_background_color(command[3]);
  }
  else if(command[2].contains("wiggle_color")>0){

    wigit=(wiggle*) active->colort->return_color_table("wiggle");
    old=wigit->return_wiggle_color();
    wigit->set_wiggle_color(command[3]);
  }
  else if(command[2].contains("wiggle_pos")>0){

    wigit=(wiggle*) active->colort->return_color_table("wiggle");
    old=wigit->return_pos_color();
    wigit->set_pos_color(command[3]);
  }
  else if(command[2].contains("wiggle_neg")>0){

    wigit=(wiggle*) active->colort->return_color_table("wiggle");
    old=wigit->return_neg_color();
    wigit->set_neg_color(command[3]);
  }
  else if (command[2].contains("contour_color")>0){
	cont=(contour*) active->colort->return_color_table("contour");

	old=cont->return_contour_color();
	cont->set_contour_color(command[3]);


  }
  else if (command[2].contains("number")>0){
  	cont=(contour*) active->colort->return_color_table("contour");

    old=QString(cont->get_number_lines());
    cont->set_number_lines(command[3].toInt());


  }
    else if(command[2].contains("alpha")>0){
    old=QString::number(active->get_alpha());
    active->set_alpha(command[3].toInt());
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "only";
  }
  else if (command[2].contains("contour_dc")>0){
  	cont=(contour*) active->colort->return_color_table("contour");

    old=QString::number(cont->get_c0());
    cont->set_dc(command[3].toFloat());

  }
  else if (command[2].contains("contour_c0")>0){
  	cont=(contour*) active->colort->return_color_table("contour");
    old=QString::number(cont->get_c0());
    cont->set_c0(command[3].toFloat());

  }
 else if(command[2].contains("overlay_color")>0){
    old=active->get_over_c();
    active->set_overlay_color(command[3]);
    if(write) hist.push_back(change(3,command,old,command[3]));
    return "only";
  }

  if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
  return "limited";
}

QString updater::clip_respond(std::vector<QString>command){
 //Need to store these changes
  QString old;
  write=true;
  int idat=command[0].toInt();
  dataset *dat=datas->return_dat(idat);
  if(command[2].contains("add")>0){
    if (command[3].startsWith("XXX")){
      write=false;
       command[3].remove(0,3);
       dat->del_pt(command[3].toInt());
    }
    else{
      int iret=dat->add_pt(command[3].section(":",0,0).toFloat(),
        command[3].section(":",1,1).toFloat());

      old="XXX";//+QString::number(command[3].section(":",0,0).toFloat(),
        //command[3].section(":",1,1).toFloat());
              old="XXX"+QString::number(iret);
    }
  }
  else if(command[2].contains("set")>0){
    if (command[3].startsWith("XXX")){
       write=false;
      
    }
    else{
      float *pct=util::float_array_from_string(command[3]);
      QString tmp=dat->set_pts(pct);
      
      int iret=dat->add_pt(command[3].section(":",0,0).toFloat(),
        command[3].section(":",1,1).toFloat());
        if(iret==0){}
        old="XXX"+tmp;
      delete [] pct;
       // std::cout<<command[3]<<std::endl;
    }
  }
  else if(command[2].contains("clear")>0){
       if (command[3].startsWith("XXX")){
       write=false;
      
    }
    else{
       dat->clear_pts();
 
       // std::cout<<command[3]<<std::endl;
    }
  
  
  }
  else if(command[2].contains("delete")>0){
    if(command[3].startsWith("XXX")){
       command[3].remove(0,3);
       old=dat->add_pt(command[3].section(":",0,0).toFloat(),command[3].section(":",1,1).toFloat());
       write=false;
    }
    else{
        float pct,clp;
       dat->get_pt(command[3].toInt(),&pct,&clp);
      old="XXX"+QString::number(pct)+":"+QString::number(clp);
             dat->del_pt(command[3].toInt());

    }
  }
  else if(command[2].contains("bcolor")>0){
    old=dat->get_bcolor();
    dat->set_bcolor(command[3]);
   
  }
  else if(command[2].contains("ecolor")>0){
    old=dat->get_ecolor();
    dat->set_ecolor(command[3]);
  }
  if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
  return "all";
}
QString updater::ctable_respond(std::vector<QString>command){
 //Need to store these changes

  user_color *col=(user_color*) stype->slices["user"];
  if(command[2].contains("set_green")>0){
    int *vv=util::int_array_from_string(command[3]);
    col->set_green(vv);
    delete [] vv;
  }
  else if(command[2].contains("set_blue")>0){
    int *vv=util::int_array_from_string(command[3]);
    col->set_blue(vv);
    delete [] vv;
  }
  else if(command[2].contains("set_red")>0){
    int *vv=util::int_array_from_string(command[3]);
    col->set_red(vv);
    delete [] vv;
  }

  return "all";
}
QString updater::pick_respond(std::vector<QString>command){
  QString old;
  if(command[2].contains("surface")>0){
    surface_data *surf=(surface_data*) datas->return_dat("Surface");
    if(command[3].contains("data")>0){
       old=surf->get_dataset();
       surf->set_dataset(active->pos,command[4],datas->return_dat(command[4]));
       if(write) hist.push_back(change(4,command,old,command[command.size()-1]));
        return "all";
    }
    else if(command[3].contains("color")>0){
      old=surf->get_color();
      surf->set_color(active->pos,command[4]);
      if(write) hist.push_back(change(4,command,old,command[command.size()-1]));
      return "all";
    }
    else if(command[3].contains("update")>0){
      surf->form_histo(active->pos);
      return "all";
    }
    else if(command[3].contains("display")>0){
       old=surf->get_display();
      surf->set_display(active->pos,command[4]);
      if(write) hist.push_back(change(4,command,old,command[command.size()-1]));
      return "all";
    }
}
  else if(command[2].contains("bulk")>0){
       int move=pk->get_move();
       old="on";
       if(move==0) old="off";
       move=0;
       
       
       
       
       if(command[3].contains("on")>0) move=1;
       pk->set_move(move);
      if(write) hist.push_back(change(3,command,old,command[command.size()-1]));
      return "none";
    }
  else if(command[2].contains("range")>0){
    old=QString::number(pk->get_dist());
      pk->set_dist(command[3].toInt());
    if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
    return "all";
  }
  else if(command[2].contains("size")>0){
    if(command[3].contains("increase")>0){
      pk->increase_size();
      old="decrease";
    }
    else{ 
      pk->decrease_size();
      old="increase";
    }
    if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
    return "all";

  }
  else if(command[2].contains("snap")>0){
    old=snap;
    snap=command[3];   
    if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
    return "all";

  }
  else if(command[2].contains("draw")>0){
    int old_display=pk->get_display();
    int new_display;
    if(command[3].contains("line")) new_display=DISPLAY_LINE;
    else if(command[3].contains("point")) new_display=DISPLAY_POINTS;
    else new_display=DISPLAY_EXTRA;
    pk->set_display(new_display);

    if(write) hist.push_back(change(3,command,QString::number(old_display),
      QString::number(new_display)));
    return "all";
 }
   else if(command[2].contains("extra")>0){
    old=QString::number(pk->get_extra()); 
  


   pk->set_extra(command[3].toInt());
   if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
   return "all";
 }
  else if(command[2].contains("single")>0){
    old=QString::number(pk->get_single()+1); 
  
   surface_data *surf=(surface_data*) datas->return_dat("Surface");
   if(surf!=NULL) surf->set_single(active->pos,command[3].toInt()-1);
   pk->set_single(command[3].toInt()-1);
   if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
   return "all";
 }
 else if(command[2].contains("load")>0){
    if(command[3].contains("NONE")) return "none";
    pk->read_file(command[3]);
    if(write) hist.push_back(change(3,command,"none",command[command.size()-1]));
    return "all";
 }
 else if(command[2].contains("save")>0){
   if(command[3].contains("NONE")) return "None";
   pk->write_file(command[3]);
    if(write) hist.push_back(change(3,command,"none",command[command.size()-1]));
    return "none";
 }
 else if(command[2].contains("active")>0){
   old=pk->get_active_name();
   if(old==command[3]) return "none";
   pk->change_active_col(command[3]);
   if(write) hist.push_back(change(3,command,old,command[3]));
   return "none";
 }
 else if(command[2].contains("view")>0){
   old=pk->get_draw_what();
   if(old==command[3]) return"none";
   pk->set_draw_what(command[3]);
   if(write) hist.push_back(change(3,command,old,command[3]));
   return "all";
 }

 else if(command[2].contains("add")>0 ||
   (command[2].contains("move")>0 && pk->get_move()==0)){
   QString news; news=command[3];
   int *iloc=util::int_array_from_string(command[3]);
   if(iloc[9]<0) {
      iloc[9]=-iloc[9];
      float ds[8];
      active->pos->get_ds(ds);
       pk->del_pt(active->pos,iloc,iloc[9],iloc[10]);
       active->return_dat()->del_pt_action(iloc);
   }
   else{
     //active->return_dat()->snap_location(iloc,pk->get_single(),snap);
     if(command[2].contains("move")>0){
       pk->del_pt(active->pos,&iloc[9],iloc[17],iloc[18]);
       active->return_dat()->del_pt_action(&iloc[9]);
     
     }
     pk->add_pt(iloc,iloc[8]);
     active->return_dat()->add_pt_action(iloc);
     news=util::string_from_int_array(11,iloc);
     iloc[9]=-iloc[9];
   }
   old=util::string_from_int_array(11,iloc);
   if(write) hist.push_back(change(3,command,old,news));
     delete [] iloc;
      for(int i=0; i < nviews; i++) windows[i]->redo_view();
   return "all";
 }
 else if(command[2].contains("del")>0){
   int *iloc=util::int_array_from_string(command[3]);
   if(iloc[9]<0) {
     pk->add_pt(iloc,1);
     active->return_dat()->add_pt_action(iloc);

   }
   else {
     float ds[8];
     active->pos->get_ds(ds);
      bool doit=true;
      pk->del_pt(active->pos,iloc,iloc[9],iloc[10]);
    
          active->return_dat()->del_pt_action(iloc);

     old=util::string_from_int_array(11,iloc);
     if(doit) if(write) hist.push_back(change(3,command,old,command[3]));
   }
   delete[] iloc;
    for(int i=0; i < nviews; i++) windows[i]->redo_view();
   return "all";
 }
 return "none";
}
/*
	Responds to the command vector sent by any method in the orient menu.
*/
QString updater::orient_respond(std::vector<QString>command){

  int order[8],new_order[8];
  
  if(command[2].contains("vsize_beg")){
    for(int i=ibeg; i < iend; i++) windows[i]->resize_view_beg(command[3].toInt(),command[4].toInt());
    return "limited";
  }
  else if(command[2].contains("vsize_end")){
    for(int i=ibeg; i < iend; i++) windows[i]->resize_view_end(command[3].toInt(),command[4].toInt());
      return "limited";
  } 
  else if(command[2].contains("vsize")){
    for(int i=ibeg; i < iend; i++) windows[i]->resize_view(command[3].toInt(),command[4].toInt());
      return "limited";

  }
  else if(command[2].contains("transpose")>0){
    QString old;
    for(int i=0; i < 8; i++) new_order[i]=order[i]=active->pos->get_order(i);
    if(command[3].contains("12")>0){ new_order[1]=order[0]; new_order[0]=order[1];}
    else if(command[3].contains("13")>0){ new_order[2]=order[0]; new_order[0]=order[2];}
    else if(command[3].contains("14")>0){ new_order[3]=order[0]; new_order[0]=order[3];}
    else if(command[3].contains("15")>0){ new_order[4]=order[0]; new_order[0]=order[4];}
    else if(command[3].contains("16")>0){ new_order[5]=order[0]; new_order[0]=order[5];}
    else if(command[3].contains("17")>0){ new_order[6]=order[0]; new_order[0]=order[6];}
    else if(command[3].contains("18")>0){ new_order[7]=order[0]; new_order[0]=order[7];}
    else if(command[3].contains("23")>0){ new_order[2]=order[1]; new_order[1]=order[2];}
    else if(command[3].contains("24")>0){ new_order[3]=order[1]; new_order[1]=order[3];}
    else if(command[3].contains("25")>0){ new_order[4]=order[1]; new_order[1]=order[4];}
    else if(command[3].contains("26")>0){ new_order[5]=order[1]; new_order[1]=order[5];}
    else if(command[3].contains("27")>0){ new_order[6]=order[1]; new_order[1]=order[6];}
    else if(command[3].contains("28")>0){ new_order[7]=order[1]; new_order[1]=order[7];}
    else if(command[3].contains("34")>0){ new_order[3]=order[2]; new_order[2]=order[3];}
    else if(command[3].contains("35")>0){ new_order[4]=order[2]; new_order[2]=order[4];}
    else if(command[3].contains("36")>0){ new_order[5]=order[2]; new_order[2]=order[5];}
    else if(command[3].contains("37")>0){ new_order[6]=order[2]; new_order[2]=order[6];}
    else if(command[3].contains("38")>0){ new_order[7]=order[2]; new_order[2]=order[7];}
    else if(command[3].contains("45")>0){ new_order[4]=order[3]; new_order[3]=order[4];}
    else if(command[3].contains("46")>0){ new_order[5]=order[3]; new_order[3]=order[5];}
    else if(command[3].contains("47")>0){ new_order[6]=order[3]; new_order[3]=order[6];}
    else if(command[3].contains("48")>0){ new_order[7]=order[3]; new_order[3]=order[7];}
    else if(command[3].contains("56")>0){ new_order[5]=order[4]; new_order[4]=order[5];}
    else if(command[3].contains("57")>0){ new_order[6]=order[4]; new_order[4]=order[6];}
    else if(command[3].contains("58")>0){ new_order[7]=order[4]; new_order[4]=order[7];}
    else if(command[3].contains("67")>0){ new_order[6]=order[5]; new_order[5]=order[6];}
    else if(command[3].contains("68")>0){ new_order[7]=order[5]; new_order[5]=order[7];}
    else if(command[3].contains("78")>0){ new_order[7]=order[6]; new_order[6]=order[7];}
    old=command[3];
    for(int i=0; i<8; i++) new_order[i]++;
    for(int i=ibeg; i < iend; i++) windows[i]->pos->set_orders(new_order);
     if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
     return "limited";
  }
  else if(command[2].contains("flip")>0){
    QString old;
    int it=-1;
    old=command[3];
    if(command[3].contains("1")>0) it=0;
    else if(command[3].contains("2")>0) it=1;
    else if(command[3].contains("3")>0) it=2;
    for(int i=ibeg; i < iend; i++) windows[i]->pos->flip_axis(it);   
    if(write) hist.push_back(change(3,command,old, command[command.size()-1])); 
    return "limited";
  }
  else if(command[2].contains("move")>0){
    int pos[8],new_pos[8];
    active->pos->get_orders(order);
    for(int i=0; i< 8; i++) new_pos[i]=pos[i]=active->pos->get_loc(i);
    if(command[3].contains("corner")>0){
      for(int i=0; i<3; i++) new_pos[order[i]]=active->pos->get_end(order[i])-1;;
    }
    else if(command[3].contains("center")>0){
      for(int i=0; i<3;i++) new_pos[order[i]]=(int)(0.5*(active->pos->get_end(order[i])+
        active->pos->get_beg(order[i])));
    }
    for(int i=0; i< nviews;i++) windows[i]->pos->set_locs(new_pos);
    com[1]="navigate"; com[2]="move";
    QString oldp=util::string_from_int_array(8,pos),newp=util::string_from_int_array(8,new_pos);
    if(write) hist.push_back(change(3,com,oldp,newp));    
    return "all";
  }
  else if(command[2].contains("reset")>0){
  
    //get current info
    int loc[8],new_loc[8],beg[8],new_beg[8],end[8],new_end[8];
    axis ax[8];
    active->pos->get_orders(order);
    active->pos->get_begs(beg);
    active->pos->get_ends(end);
    active->pos->get_locs(loc);
    active->pos->get_axes(ax);
    
    if(locked==0){
      com[1]="main"; com[2]="locked"; com[3]="yes";
      if(write) hist.push_back(change(3,com,"no","yes"));
    
    }
    
    
    //Reset order
    for(int i=0; i<8; i++){ new_order[i]=i+1; order[i]=order[i]+1;}
    com[1]="orient"; com[2]="order";
    if(write) hist.push_back(change(3,com,util::string_from_int_array(8,order),
      util::string_from_int_array(8,new_order)));
    
    //Next reset range
    for(int i=0; i< 8; i++){
      new_beg[i]=0; new_end[i]=ax[i].n; new_loc[i]=new_end[i]/2;
    }
    com[1]="navigate"; com[2]="range";
    QString oldr=util::string_from_int_array(8,beg)+":"+util::string_from_int_array(8,end);
    QString newr=util::string_from_int_array(8,new_beg)+":"+util::string_from_int_array(8,new_end);
    if(write) hist.push_back(change(3,com,oldr,newr));
    
    //Straighten axes
    com[1]="orient"; com[2]="reverse";
    com[3]="false";
    bool rev;
    QString old;
    for(int i=0; i<8; i++){
      rev=active->pos->get_reverse(i);
      if(rev) old="true";
      else old="false";
      for(int j=0; j< nviews; j++) windows[j]->pos->set_reverse(i,false);
      com[2]=QString::number(i);
      if(write) hist.push_back(change(3,com,old,"false"));
    }
    
    //Finally change position
   
    for(int i=0; i< nviews;i++){
      windows[i]->pos->set_orders(new_order);
      windows[i]->pos->set_locs(new_loc);

      windows[i]->pos->set_begs(new_beg);
      windows[i]->pos->set_ends(new_end);
    }
    return "all";
  }
  else if(command[2].contains("order")>0){
    int order[8];
    active->pos->get_orders(order);
    for(int i=0; i<8 ;i++) order[i]+=1;
    int *new_order=util::int_array_from_string(command[3]);
    QString old=util::string_from_int_array(8,order);
    if(write) hist.push_back(change(3,command,old,command[3]));
    for(int i=ibeg; i < iend; i++) windows[i]->pos->set_orders(new_order);
    delete [] new_order;
    return "limited";
  }
  else if(command[2].contains("reverse")>0){
    int iax=command[3].toInt();
    bool old=active->pos->get_reverse(iax);
    QString olds="false";
    if(old) olds="true";
    bool newb=false;
    if(command[4]=="true") newb=true;
    for(int i=ibeg; i< iend; i++)windows[i]->pos->set_reverse(iax,newb);
    return "limited";
  }
  else if(command[2].contains("true")>0){
    float pt,pb;
    float npt,npb;
    active->get_view_pcts(&pt,&pb);
    QString olds=QString::number(pt*100.)+":"+QString::number(pb*100.);
    axis a1,a2,a3;
    a1=active->pos->get_axis(active->pos->get_order(0));
    a2=active->pos->get_axis(active->pos->get_order(1));
    a3=active->pos->get_axis(active->pos->get_order(2));
    float l1=a1.d*a1.n,l2=a2.d*a2.n,l3=a3.d*a3.n;
    npt=l3/(l1+l3); npb=l2/(l3+l2);
    for(int i=ibeg; i< iend; i++) windows[i]->set_view_pcts(npt,npb);
    com[1]="orient"; com[2]="ratio";
    if(write) hist.push_back(change(3,com,olds,QString::number(npt*100.)+":"+QString::number(npb*100.)));
    return "limited";
  }
  else if(command[2].contains("ratio")>0){
   float pt,pb;
    active->get_view_pcts(&pt,&pb);
    QString olds=QString::number(pt)+":"+QString::number(pb);
    int *pcts=util::int_array_from_string(command[3]);
    float npt=(float)pcts[0]/100., npb=(float)pcts[1]/100.;
    delete [] pcts;
    for(int i=ibeg; i< iend; i++) windows[i]->set_view_pcts(npt,npb);
    com[1]="orient"; com[2]="ratio";
    if(write) hist.push_back(change(3,command,olds,command[3]));
    return "limited";
  }
  else if(command[2].contains("rotate")){
    QString old;
    //Haven't handled back yet
    if(command[3].contains("set_pt")){
      int oa[8],pt[8];
      active->pos->get_rot_pt(oa);
      active->pos->get_rot_pt(pt);
      old=util::string_from_int_array(8,oa);
      for(int i=ibeg; i < iend; i++) windows[i]->pos->set_rot_pt(pt);
      

      
      if(write) hist.push_back(change(4,command,old,command[3]));
      return "limited";
    }
    else if(command[3].contains("reset")){
      for(int i=ibeg; i< iend;i++) windows[i]->pos->set_no_rotate();

     // if(write) hist.push_back(change(4,command,old,command[3]));
      return "limited";
    }
    else if(command[3].contains("accept")){
    
    
    }
    else if(command[3].contains("axes")){
      int ora[2];active->pos->get_rot_axes(&ora[0],&ora[1]);
      old=QString::number(ora[0]+1)+QString::number(ora[1]+1);
      for(int i1=1; i1 <9; i1++){
        for(int i2=i1+1; i2<9; i2++){
          QString test=QString::number(i1)+QString::number(i2);
          if(command[4].contains(test)){
            for(int i=ibeg; i < iend; i++) windows[i]->pos->set_rot_axes(i1-1,i2-1);
          }
        }
      }
       return "limited";
     
      if(write) hist.push_back(change(4,command,old,command[4]));
    
    }
    else if(command[3].contains("angle")){
      float ang=active->pos->get_rot_angle();
      old=QString::number(ang);
      ang=command[4].toFloat();
      for(int i=ibeg; i < iend; i++) windows[i]->pos->set_rot_angle(ang);
      if(write) hist.push_back(change(4,command,old,command[4]));  
    }
  }
  return "none";
}
/*
	Responds to the command vector sent by a method in the main menu.
*/
QString updater::main_respond(std::vector<QString>command){
  QString old;
  if(locked==0) old="no";
  else old="yes";
  if(command[2].contains("locked")>0){
    if(command[3].contains("no")) locked=0;
    else locked=1;
    if(locked==0) {ibeg=iactive; iend=iactive+1;}
    else {ibeg=0; iend=nviews;}

   if(write) hist.push_back(change(3,command,old, command[command.size()-1]));
   return "none";
  }
  else if(command[2].contains("save")>0){
    write_history(command[3]);
    if(write) hist.push_back(change(3,command,"none", command[command.size()-1]));
    return "none";
  }
    else if(command[2].contains("open")>0){
      this->load_history(command[3]);
    return "none";
  }
  else if(command[2].contains("load")>0){
	this->load_dataset(command);
	return "none";
  }
  else if(command[2].contains("resize") >0){
    int *szs=util::int_array_from_string(command[3]);
    QString newc;
    if(szs[0]==0){ //From mouse action
      old="1:"+QString::number(szs[3])+":"+QString::number(szs[4])+":"+
        QString::number(szs[1])+":"+QString::number(szs[2]);
       newc="2:"+QString::number(szs[1])+":"+QString::number(szs[2])+":"+
        QString::number(szs[3])+":"+QString::number(szs[4]);
        if(write) hist.push_back(change(3,command,old,newc));
     }
    
    delete [] szs;
    }
  else if(command[2].contains("attach")){
     if(command[3].contains("true")) old="false";
     else old="true";
     if(write) hist.push_back(change(4,command,old,command[3]));
  }
 return "none";

}
void updater::write_history(QString text){
  FILE *fd;
  fd=fopen(text,"w");
  QString com;
  for(int i=0;  i<(int)hist.size();i++) fprintf(fd,"%s\n",hist[i].output_for().ascii());
  fclose(fd);
}
void updater::load_history(QString text){
    FILE *fd;
  char line[2048];
  QString ln;
  QString col;
  std::vector<QString> com2;
  com2.push_back("50");
  com2.push_back(" ");
 if ((fd=fopen(text.ascii(),"r")) == NULL) fprintf(stderr,"PROBLEM OPENING \n");
 std::vector<QString> lines;
  while (!feof(fd)){
    if(NULL == fgets(line, 1000, fd)) break;
    if (0!=strncmp (line,"#",1)) {
      ln=line;
      ln.stripWhiteSpace();
      ln.remove(ln.length()-1,ln.length());
      lines.push_back(ln);
    }
  }
  for(int i=0; i< (int)lines.size(); i++){
      ln=lines[i];
      int nelem=ln.count("-");
      std::vector<QString> command;
      for(int i=0; i < nelem+1; i++) command.push_back(ln.section("-",i,i));
      emit actionDetected(command);
  }
  fclose (fd);
}
/*
	Loads a dataset into memory.
*/
void updater::load_dataset(std::vector<QString> coms){
     io_func *io=0;
	util *pp=active->return_dat()->io->par;
	//hypercube *grid=active->return_dat()->return_grid();
	//int idat=datas->return_ndat();
   int esize=1;
	axis axes[8];
     	 if(coms[4].contains("SEP")>0){
     	   char temp[1024]; strcpy(temp,coms[3].ascii());
		   sreg *d=new sreg(temp);
		   esize=d->get_int("esize",4);
		   if(esize==4) io=new sep_reg_io_float(temp,pp);
		   else if(esize==1) io=new sep_reg_io_byte(temp,pp);
		   std::vector<axis> ju=d->return_axes(8);
		 
		   for(int i=0; i < 8; i++) axes[i]=ju[i];

		 }
		 else {
                   if((io+1) != 0) /* pretent io is used after being set */
                   pars->error("Only support reading SEP datasets for now \n");	
                 }
/*
		if(coms[5] == "incore") 
		//	datas->add_dat(new incore_data_float(coms[3],grid,io,pars,idat,1));
	//	else if(coms[5] == "incore") 
			datas->add_dat(new incore_data_byte(coms[3],coms[3],grid,io,pars,idat,1));
		else if(coms[5] == "out-core") 
		//	datas->add_dat(new outcore_data_float(coms[3],grid,io,pars,idat,3));
		//else if(coms[5] == "out-core") 
			datas->add_dat(new outcore_data_byte(coms[3],coms[3],grid,io,pars,idat,3));
		else if(coms[5] == "partial") 
		//	datas->add_dat(new partial_data_float(coms[3],grid,io,pars,idat,1));
	//	else if(coms[5] == "Bytepartial") 
			datas->add_dat(new partial_data_byte(coms[3],coms[3],grid,io,pars,idat,1));
		else{
			fprintf(stderr,"store%d=%s \n",idat,coms[3].ascii());
			pars->error("Undefined storage\n");
		}
		*/
		if(datas->return_ndat()==2){ 
		  std::vector<QString> com2;
		  com2.push_back("-2");
		  com2.push_back("menu");
		  com2.push_back("View");
		  com2.push_back("multi");
		      emit actionDetected(com2);

		}
	
}	
void updater::closeEvent(QCloseEvent* event) {
	//writeSettings();
	event->accept();
}
void updater::run_movie(){
     
	  movie_timer->start(movie_delay);
}
void updater::update_movie(){
  active->pos->update_movie_pos(active->return_dat()->io->return_hyper());
  sync_orient();
  for(int i=0; i < nviews; i++) windows[i]->update_display();
}
void updater::stop_movie(){
	movie_timer->stop();
}
void updater::sync_orient(){
 int beg[8],end[8],loc[8];
 int bt[8],et[8],lt[8];
 active->pos->get_begs(beg);
 active->pos->get_ends(end);
 active->pos->get_locs(loc);
 int ia=active->pos->get_movie_axis(),id=active->pos->get_movie_dir();
 for(int i=0;i <nviews; i++){
   windows[i]->pos->set_movie_dir(ia,id);
   windows[i]->pos->get_begs(bt);
   windows[i]->pos->get_ends(et);
   windows[i]->pos->get_locs(lt);
   if(bt[0]!=beg[0] || bt[1]!=beg[1] || bt[2]!=beg[2] || bt[3]!=beg[3] ||
      bt[4]!=beg[4] || bt[5]!=beg[5] || bt[6]!=beg[6] || bt[7]!=beg[7])
        windows[i]->pos->set_begs(beg);
     if(et[0]!=end[0] || et[1]!=end[1] || et[2]!=end[2] || et[3]!=end[3] ||
        et[4]!=end[4] || et[5]!=end[5] || et[6]!=end[6] || et[7]!=end[7])
         windows[i]->pos->set_ends(end);
     if(lt[0]!=loc[0] || lt[1]!=loc[1] || lt[2]!=loc[2] || lt[3]!=loc[3] ||
        lt[4]!=loc[4] || lt[5]!=loc[5] || lt[6]!=loc[6] || lt[7]!=loc[7])
       windows[i]->pos->set_locs(loc);
 }
}

#ifdef CHEVRON
QString updater::chev_respond(std::vector<QString>command){
  chev_strat_data *cd=(chev_strat_data*)datas->return_dat("CHEV_STRAT");
  if(command[2].contains("layer")>0){
    QString old="none";
    if(write)hist.push_back(change(3,command,old,command[3]));
    cd->set_layer(active->pos,command[3].toInt());
    return "all";
  }
  else if(command[2].contains("color")>0){
    QString old=cd->get_color();
    if(write)hist.push_back(change(3,command,old,command[3]));
    cd->set_color(command[3]);
    return "all";
  }
 
 }
 #endif
