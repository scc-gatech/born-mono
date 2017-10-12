#include "panels.h"
#include "wiggle.h"
#include "contour.h"
#include "autopick.h"
using namespace SEP;
panels::panels(std::shared_ptr<genericIO> io,std::shared_ptr<pick_draw>p,std::shared_ptr<maps>mym,std::shared_ptr<orients>oo){
pan_num=0;locked=false; pk=p; my_maps=mym;
   my_or=oo;
  _io=io;
  movie_timer = new QTimer( this);
    connect( movie_timer, SIGNAL(timeout()), this,  SLOT(update_movie()) ); 
   movie_delay=10;
   std::shared_ptr<autopick> ap(new autopick(pk));
  autop=ap;
}
void panels::add_panel(std::shared_ptr<panel> um){
  my_pan[um->get_iview()]=um;
  connect_it(um->get_iview());

}
void panels::set_active(int iact){
  active_num=iact;


  my_or->set_active(my_pan[active_num]->get_orient_inst());
}
void panels::sync_panel(std::shared_ptr<panel>um){
  
  std::shared_ptr<orient_cube> pos=um->get_orient();
  for(std::map<int,std::shared_ptr<panel>>::iterator i=my_pan.begin(); i!=my_pan.end(); i++){
    i->second->sync_pos(pos);
 }

}
void panels::update_mouse(std::shared_ptr<mouse_func> f){

  for(std::map<int,std::shared_ptr<panel> >::iterator i=my_pan.begin(); i!=my_pan.end(); i++){
    i->second->set_mouse_func(f);
  }
}
void panels::actionRespond(std::vector<QString> coms){
  emit actionDetected(coms);
}
void panels::set_range(int iv, int *ibeg, int *iend){
   if(locked){ *ibeg=0; *iend=my_pan.size()-1;}
   else {*ibeg=iv; *iend=iv;}
}
void panels::perform_orient(std::vector<QString> command){
  std::shared_ptr<panel>active=my_pan[command[0].toInt()];
  set_active(command[0].toInt());
  std::shared_ptr<orient_cube> pos=active->return_orient();
  int ibeg,iend;
  set_range(command[0].toInt(),&ibeg,&iend);
  if(command[2].contains("flip1")>0){
      for(int i=ibeg; i <= iend; i++) my_pan[i]->return_orient()->flip_axis(0);   
     update_it(active);
  }
  else if(command[2].contains("flip2")>0){
     for(int i=ibeg; i <=iend; i++) my_pan[i]->return_orient()->flip_axis(1);   
     update_it(active);
  }
  else if(command[2].contains("flip3")>0){
  
     for(int i=ibeg; i <=iend; i++) my_pan[i]->return_orient()->flip_axis(2);   
     update_it(active);
  }
    else if(command[2].contains("reset")>0){
  
     for(int i=ibeg; i <=iend; i++) {
        my_pan[i]->return_orient()->reset_orientation();
    } 
     update_it(active);
  }
  else if(command[2].contains("center")>0){
     for(int i=0; i < 8; i++){
       axis a=pos->getAxis(i);
       pos->set_loc(i,(int)(a.n/2));
     }
     sync_panel(active);
     update_all();
  
  }
  else if(command[2].contains("edge")>0){
     for(int i=0; i < 8; i++){
       pos->set_loc(i,0);
     }
     sync_panel(active);
     update_all();
  }
  else if(command[2].contains("transpose")>0){
     int order[8],new_order[8];
    for(int i=0; i < 8; i++) new_order[i]=order[i]=pos->get_order(i);
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
    for(int i=0; i<8; i++) new_order[i]++;
    for(int i=ibeg; i <=iend; i++) my_pan[i]->return_orient()->set_orders(new_order);   
    update_it(active);
  
  }
  else if(command[2].contains("angle")>0){
  
      for(int i=ibeg; i <=iend; i++) my_pan[i]->return_orient()->set_rot_angle(command[3].toFloat());    
    update_it(active);
  }
  else if(command[2].contains("rotate_axes")>0){
    for(int i1=1; i1 <9; i1++){
        for(int i2=i1+1; i2<9; i2++){
          QString test=QString::number(i1)+QString::number(i2);
          if(command[3].contains(test)){
            for(int i=ibeg; i <= iend; i++) my_pan[i]->return_orient()->set_rot_axes(i1-1,i2-1);
          }
        }
        }
    update_it(active);
  }
  else if(command[2].contains("reset")>0){
        for(int i=ibeg; i <= iend; i++) my_pan[i]->return_orient()->set_rot_angle(0);    
    update_it(active);
  
  }
  else if(command[2].contains("rotation_pt")>0){
     int iloc[8]; pos->get_locs(iloc);
        for(int i=ibeg; i <= iend; i++) my_pan[i]->return_orient()->set_rot_pt(iloc);    
  update_it(active);

  
  }
  else if(command[2].contains("true_prop")>0){
    float len[8];
    for(int i=0; i < 8; i++) len[i]=command[3+i].toFloat();
    for(int i=ibeg; i<= iend; i++) my_pan[i]->set_proportions(len);
      update_it(active);

  }

  else if(command[2].contains("false_prop")>0){
  
   float len[8];
    for(int i=0; i < 8; i++) len[i]=-1.;
    for(int i=ibeg; i<= iend; i++) my_pan[i]->set_proportions(len);
  update_it(active);

 }
   else if(command[2].contains("vsize_beg")){
    for(int i=ibeg; i <= iend; i++) my_pan[i]->move_corner_beg(command[3].toInt(),command[4].toInt());
      update_it(active);
  }
  else if(command[2].contains("vsize_end")){

    for(int i=ibeg; i <= iend; i++) my_pan[i]->move_corner_end(command[3].toInt(),command[4].toInt());
      update_it(active);
  } 
  else if(command[2].contains("vsize")){
    for(int i=ibeg; i <= iend; i++) my_pan[i]->moving_corner(command[3].toInt(),command[4].toInt());
      update_it(active);

  }
 
}
void panels::perform_display(std::vector<QString> command){
  std::shared_ptr<panel>active=my_pan[command[0].toInt()];
  set_active(command[0].toInt());
  std::shared_ptr<orient_cube> pos=active->return_orient();
  int ibeg,iend;
  set_range(command[0].toInt(),&ibeg,&iend);
  if(command[2].contains("font")>0){
      for(int i=ibeg; i<= iend; i++) my_pan[i]->update_font(command[3]);
     update_it(active);
  }
  else if(command[2].contains("colorbar")){

     bool val=false;
     if(command[3].contains("yes")) val=true;
     for(int i=ibeg; i<= iend; i++) my_pan[i]->set_draw_bar(val);
     update_it(active);
  }
 else if(command[2].contains("color")>0){
      for(int i=ibeg; i<= iend; i++) my_pan[i]->set_pen_color(command[3]);
     update_it(active);
  }

    else if(command[2].contains("picks")>0){
     bool val=false;
     if(command[3].contains("yes")) val=true;
     for(int i=ibeg; i<= iend; i++) my_pan[i]->set_draw_picks(val);
     update_it(active);
  }
     else if(command[2].contains("grid")>0){
     bool val=false;
     if(command[3].contains("yes")) val=true;
     for(int i=ibeg; i<= iend; i++) my_pan[i]->set_draw_grid(val);
     update_it(active);
  }
       else if(command[2].contains("axis1")>0){
     bool val=false;
     if(command[3].contains("yes")) val=true;
     for(int i=ibeg; i<= iend; i++) my_pan[i]->set_draw_axis1(val);
     update_it(active);
  }
         else if(command[2].contains("axis2")>0){
     bool val=false;
     if(command[3].contains("yes")) val=true;
     for(int i=ibeg; i<= iend; i++) my_pan[i]->set_draw_axis2(val);
     update_it(active);
  }
    else if(command[2].contains("axis3")>0){
     bool val=false;
     if(command[3].contains("yes")) val=true;
     for(int i=ibeg; i<= iend; i++) my_pan[i]->set_draw_axis3(val);
     update_it(active);
  }
      else if(command[2].contains("title")>0){
     bool val=false;
     if(command[3].contains("yes")) val=true;
     for(int i=ibeg; i<= iend; i++) my_pan[i]->set_draw_title(val);
     update_it(active);
  }
        else if(command[2].contains("pos_alpha")>0){
     int val=command[3].toInt();
     for(int i=ibeg; i<= iend; i++) my_pan[i]->set_pos_transp(val);
     update_it(active);
  }
  
}
void panels::run_movie(){
	  movie_timer->start(movie_delay);
}
void panels::update_movie(){
  my_pan[0]->return_orient()->update_movie_pos(my_pan[0]->return_dat()->io->return_hyper());
  sync_panel(my_pan[0]);
   update_all();
   emit windows_update();
}
void panels::stop_movie(){
	movie_timer->stop();
}
void panels::perform_navigate(std::vector<QString> command){
  std::shared_ptr<panel>active=my_pan[command[0].toInt()];

  set_active(command[0].toInt());

  std::shared_ptr<orient_cube> pos=active->return_orient();
  int ibeg,iend;
  set_range(command[0].toInt(),&ibeg,&iend);
  if(command[2].contains("direction")>0){
    int delta=command[4].toInt();
    int nax,ndir;

     if(command[3].contains("x")>0){ nax=1; ndir=delta;}
     else if(command[3].contains("X")>0){ nax=1; ndir=-1;}
     else if(command[3].contains("y")>0){ nax=2; ndir=1;}
     else if(command[3].contains("Y")>0){ nax=2; ndir=-1;}
     else if(command[3].contains("z")>0){ nax=0; ndir=1;}
     else if(command[3].contains("Z")>0){ nax=0; ndir=-1;}
     int move_axis=pos->get_order(nax);
     for(int i=0; i< (int)my_pan.size(); i++) my_pan[i]->return_orient()->set_movie_dir(move_axis,ndir);
     pos->update_movie_pos(active->return_dat()->io->return_hyper(),delta);
     sync_panel(active);
     update_all();
  }
  else if(command[2].contains("update")>0){
    int iloc[8];
    float *poz=util::float_array_from_string(command[3]);
    for(int i=0; i < 8; i++){
       axis a=pos->getAxis(i);
       iloc[i]=std::min(a.n-1,std::max(0,(int)((poz[i]-a.o)/a.d+.5)));
    }
    pos->set_locs(iloc);
    delete [] poz;
    sync_panel(active);
    update_all();
  }
  else if(command[2].contains("step")>0){
      int delta=command[3].toInt();

       pos->update_movie_pos(active->return_dat()->io->return_hyper(),delta);
     sync_panel(active);
     update_all();
     }
       else if(command[2].contains("go")){
    run_movie();
  }
   else if(command[2].contains("speed")){
     movie_delay=command[3].toInt();  
  }
  else if(command[2].contains("stop")){
  
    stop_movie();
  }
  else  if(command[2].contains("move")){
    std::vector<int> myp=util::int_vector_from_string(command[3]);
    for(int i=0; i < 8; i++) active->get_orient()->set_loc(i,myp[i]);
    sync_panel(active);
    update_all();
  }
  else if(command[2].contains("range")){
    int beg[8],end[8];
    pos->get_ends(end);
    pos->get_begs(beg);
    
    std::vector<int> myp=util::int_vector_from_string(command[3]);
    for(int i=0;i<8;i++){ beg[i]=myp[i]; end[i]=myp[i+8];}

    pos->set_begs(beg); pos->set_ends(end);
    pos->set_loc(myp[16],(int)((beg[myp[16]]+end[myp[16]])/2.));
    pos->set_loc(myp[17],(int)((beg[myp[17]]+end[myp[17]])/2.));

    sync_panel(active);
    update_all();
  }
}
void panels::perform_view(std::vector<QString> command){
  set_active(command[0].toInt());
 std::shared_ptr<panel>active=my_pan[command[0].toInt()];
  std::shared_ptr<orient_cube>pos=active->return_orient();
  int ibeg,iend;
    std::shared_ptr<wiggle >wigit;
    std::shared_ptr<contour >cont;
  set_range(command[0].toInt(),&ibeg,&iend);
  
  if(command[2].contains("map1")){
    if(command[3].contains("load")){
       std::shared_ptr<map_1>x(new map_1(_io,command[5],active->return_dat()->return_grid(),command[4].toFloat()));
       my_maps->add_map(x);
       active->change_map(my_maps->size()-1);
        update_it(command[0].toInt());
    }
    else if(command[3].contains("change")){
       if(command[4].contains("None")) active->set_no_map();
       else{
         active->change_map(my_maps->return_num(command[4]));
       }
        update_it(command[0].toInt());
    }
  }
  
  else if(command[2].contains("type")){
    for(int i=ibeg; i<=iend; i++) my_pan[i]->update_view(command[3]);  
    update_it(command[0].toInt());
  }
  else if(command[2].contains("roundrobin")){
    std::vector<QString> moc; moc.push_back("");
    moc.push_back("menu"); moc.push_back("view");
    moc.push_back("data"); moc.push_back(" ");
    int iter;
    if(command[3].contains("forward")) iter=1;
    else iter=-1;
    for(int i=ibeg; i<=iend; i++) {
       my_pan[i]->iter_data(iter);
       int idat=my_pan[i]->get_idat();
       moc[0]=QString::number(i);
       moc[4]=QString::number(idat);
       emit actionDetected(moc);
    }
    update_it(command[0].toInt());
  }
  else if(command[2].contains("roundorobin")){
      std::vector<QString> moc; moc.push_back("");
    moc.push_back("menu"); moc.push_back("view");
    moc.push_back("over"); moc.push_back(" ");
    int iter;
    if(command[3].contains("forward")) iter=1;
    else iter=1;

    for(int i=ibeg; i<=iend; i++) {
      my_pan[i]->set_overlay(true);
      my_pan[i]->iter_o_data(iter);  
      int idat=my_pan[i]->get_idat_o();
       moc[0]=QString::number(i);
       moc[4]=QString::number(idat);
       emit actionDetected(moc);
    }
    update_it(command[0].toInt());
  }
   else if(command[2].contains("odata")){
    for(int i=ibeg; i<=iend; i++) {
      if(command[3].toInt()==-1) my_pan[i]->set_overlay(false);
      else{
          my_pan[i]->set_overlay(true);
       my_pan[i]->set_o_data(command[3].toInt());  
      }
    }
    update_it(command[0].toInt());
  }
  else if(command[2].contains("data")){
    for(int i=ibeg; i<=iend; i++) my_pan[i]->set_data(command[3].toInt());  
    update_it(command[0].toInt());
  }
  else if(command[2].contains("ocolortable")){
    for(int i=ibeg; i<=iend; i++) my_pan[i]->set_overlay_color(command[3]);  
    update_it(command[0].toInt());
        update_it(command[0].toInt());

  }
  else if(command[2].contains("colortable")){
    for(int i=ibeg; i<=iend; i++) my_pan[i]->set_colortable(command[3]);  
    update_it(command[0].toInt());
        update_it(command[0].toInt());

  }
  else if(command[2].contains("opacity")){
      for(int i=ibeg; i<=iend; i++) my_pan[i]->set_alpha(command[3].toInt());  
    update_it(command[0].toInt());
  }
  else if(command[2].contains("wiggle_color")>0){
    std::shared_ptr<slice> x=active->get_colortables()->return_color_table(std::string("wiggle"));
    wigit=std::static_pointer_cast<wiggle>(x);
     wigit->set_wiggle_color(command[3]);
         update_it(command[0].toInt());

  }
  else if(command[2].contains("wiggle_pos")>0){
    wigit=std::static_pointer_cast<wiggle>( active->get_colortables()->return_color_table(std::string("wiggle")));
    wigit->set_pos_color(command[3]);
        update_it(command[0].toInt());

  }
  else if(command[2].contains("wiggle_neg")>0){
    wigit=std::static_pointer_cast<wiggle>( active->get_colortables()->return_color_table(std::string("wiggle")));
   wigit->set_neg_color(command[3]);
       update_it(command[0].toInt());

 }
 else if (command[2].contains("contour_color")>0){
	cont=std::static_pointer_cast<contour>( active->get_colortables()->return_color_table(std::string("contour")));
	cont->set_contour_color(command[3]);
	    update_it(command[0].toInt());

  }
  else if (command[2].contains("contour_number")>0){
  	cont=std::static_pointer_cast<contour>(  active->get_colortables()->return_color_table(std::string("contour")));
    cont->set_number_lines(command[3].toInt());
        update_it(command[0].toInt());

  }
  else if (command[2].contains("contour_dc")>0){
  	cont=std::static_pointer_cast<contour>( active->get_colortables()->return_color_table(std::string("contour")));
    cont->set_dc(command[3].toFloat());
        update_it(command[0].toInt());

  }
  else if (command[2].contains("contour_c0")>0){
  	cont=std::static_pointer_cast<contour>(  active->get_colortables()->return_color_table(std::string("contour")));
    cont->set_c0(command[3].toFloat());
        update_it(command[0].toInt());

  }
  else if(command[2].contains("grid_jplane")){
      for(int i=ibeg; i<=iend; i++) my_pan[i]->set_jplane(command[3].toInt());  
    update_it(command[0].toInt());
  }
    else if(command[2].contains("grid_grid1")){
      for(int i=ibeg; i<=iend; i++) my_pan[i]->set_grid1(command[3].toInt());  
    update_it(command[0].toInt());
  }
    else if(command[2].contains("grid_grid2")){
      for(int i=ibeg; i<=iend; i++) my_pan[i]->set_grid2(command[3].toInt());  
    update_it(command[0].toInt());
  }

  
  
}
void panels::perform_auto(std::vector<QString> command){
  set_active(command[0].toInt());
  std::shared_ptr<panel> active;
  int ipa=command[0].toInt();
  if(ipa>=0) active=my_pan[command[0].toInt()];
  std::shared_ptr<orient_cube>pos=active->return_orient();
    if(command[2].contains("method")>0){

    autop->set_method(command[3]);
  }
  else if(command[2].contains("plane")>0){

    QString old=autop->get_plane();
    autop->set_plane(command[3]);
  }
  else if(command[3].contains("accept")>0){

      autop->accept_picks();
    update_all();
  }
    else if(command[3].contains("decline")>0){

      autop->decline_picks();
    update_all();
  }
  else if(command[3].contains("extend")>0){

    QString old;
    if(command[4].contains("do")){
      autop->extend_picks(active->get_view(),pos);
    }
    update_all();
  }
  else if(command[3].contains("run2d")){
      autop->pick_2d(active->get_view(),pos);
      update_all();
    }
   

  else if(command[2].contains("type")>0){
    if(command[3].contains("amplitude")) autop->set_amplitude();
    else autop->set_correlate();
  }
  else if(command[2].contains("cor")>0){
    autop->set_ncor(command[3].toInt());
  }
  else if(command[2].contains("dist")>0){
    autop->set_nsearch(command[3].toInt());
  }
    else if(command[2].contains("error")>0){
    autop->set_error(command[3].toInt());
  }

}
void panels::perform_pick(std::vector<QString> command){
  std::shared_ptr<panel> active=0;
  int ipa=command[0].toInt();
   std::shared_ptr<orient_cube> pos=0;
  if(ipa>=0) {
    active=my_pan[command[0].toInt()];
    pos=active->return_orient();
     set_active(command[0].toInt());

    }
    if(command[2].contains("multi_add")>0){
      std::vector<long long> locs;
      int nlen=command[3].toInt();
      for(int i=0; i < nlen; i++)  locs.push_back(command[4].section(":",i,i).toLongLong());
      pk->add_multi_picks(locs);
      update_all();
    }
    else if(command[2].contains("multi_delete")>0){
      std::vector<long long> locs;
      int nlen=command[3].toInt();
      for(int i=0; i < nlen; i++)  locs.push_back(command[4].section(":",i,i).toLongLong());
      pk->delete_multi_picks(locs);
      update_all();
    }
   else if(command[2].contains("add")>0 ||
   (command[2].contains("move")>0 && pk->get_move()==0)){
   QString news; news=command[3];
   int *iloc=util::int_array_from_string(command[3]);

    if(command[2].contains("move")>0){
       pk->del_nearest(pos,&iloc[9],iloc[17],iloc[18]);
       active->return_dat()->del_pt_action(&iloc[9]);
     
     }
     pk->add_pt(iloc,iloc[8]);
     active->return_dat()->add_pt_action(iloc);
     news=util::string_from_int_array(11,iloc);
     iloc[9]=-iloc[9];
     update_all();
   }
   else if(command[2].contains("del")>0){
      int *iloc=util::int_array_from_string(command[3]);
       float ds[8];
       pos->get_ds(ds);
       pk->del_nearest(pos,iloc,iloc[9],iloc[10]);
       active->return_dat()->del_pt_action(iloc);
       update_all();
   }
   else if(command[2].contains("size")){
     if(command[3].contains("increase")){
        pk->increase_size();
        update_all();
    }
  
     else if(command[3].contains("decrease")){
       pk->decrease_size();
       update_all();
    }
  }
     else if(command[2].contains("text")){
        pk->set_pick_text(command[3]);
    }
  else if(command[2].contains("distance")){
    pk->set_dist(command[3].toInt());
    update_all();
  }
  else if(command[2].contains("display")){
     int new_display;
     if(command[3].contains("line")) new_display=DISPLAY_LINE;
     else if(command[3].contains("points")) new_display=DISPLAY_POINTS;
     else if(command[3].contains("text")) new_display=DISPLAY_TEXT;
     pk->set_display(new_display);
     update_all();
  }
  else if(command[2].contains("extra")>0){
     pk->set_extra(command[3].toInt());
     update_all();
  }
  else if(command[2].contains("single")>0){ 
     pk->set_single(command[3].toInt()-1);
     update_all();
 }
 else if(command[2].contains("load")>0){
    pk->read_file(command[3]);
     update_all();
 }
 else if(command[2].contains("save")>0){
   pk->write_file(command[3]);
 }
 else if(command[2].contains("group")>0){
   pk->change_active_col(command[3]);
 }
 else if(command[2].contains("view")>0){
   pk->set_draw_what(command[3]);
   update_all();
 }
 
 }


void panels::update_what(int iv, QString what){

  if(what.contains("all")) update_all();
  else if(what.contains("only")) update_only(iv);
  else if(what.contains("limited")) update_it(iv);
  

}
void panels::update_all(){

    for(std::map<int,std::shared_ptr<panel>>::iterator i=my_pan.begin(); i!=my_pan.end(); i++)
      i->second->set_update_it(true);

}



void panels::delete_panels(){



}/*
void panels::connect_all(){
  for(std::map<int,panel*>::iterator i=my_pan.begin(); i!=my_pan.end(); i++){
    connect_it(i->first);
  }

}
*/
void panels::connect_it(int i){

connect(my_pan[i].get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

}
void panels::disconnect_it(int i){

  disconnect(my_pan[i].get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

}
