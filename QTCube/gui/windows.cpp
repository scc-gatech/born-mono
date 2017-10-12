#include "windows.h"
using namespace SEP;
void windows::add_window(std::shared_ptr<DrawWindow>um){
  my_winds.push_back(um);
  connect_it(my_winds.size()-1);
  active_num=0;
}

void windows::delete_window(std::shared_ptr<DrawWindow>wind){
  for(int i=0; i <(int) my_winds.size(); i++){
    if(my_winds[i]==wind){
      my_winds.erase(my_winds.begin()+i);
      return;
    }
  }
}
void windows::delete_windows(){
  for(int i=0; i < (int)  my_winds.size(); i++){
  

  }
  my_winds.clear();

}
void windows::annotate_respond(std::vector<QString> coms){

 std::shared_ptr<DrawWindow> active=my_winds[coms[0].toInt()];
  if(coms[2].contains("shape")){
    active->set_ano_shape(coms[3]);
  }
  else if(coms[2].contains("color")){
     active->set_ano_color(coms[3]);
  }
   else if(coms[2].contains("text")){
     active->set_ano_text(coms[3]);
  }
    else if(coms[2].contains("font")){
     active->set_ano_font(coms[3]);
  }
    else if(coms[2].contains("thick")){
     active->set_ano_thick(coms[3]);
  }
      else if(coms[2].contains("add")>0){
      int *vv=util::int_array_from_string(coms[3]);
    if(vv[0]>=0) active->add_anno(vv[0],vv[1],vv[2],vv[3]);
    delete [] vv;
      active->update_display(true);
  }  
    else if(coms[2].contains("update")>0){
    int *vv=util::int_array_from_string(coms[3]);
    if(vv[0]>=0) active->update_anno(vv[0],vv[1],vv[2],vv[3]);
    delete [] vv;
      active->update_display(true);
  }  
    else if(coms[2].contains("shift_finish")>0){
    int *vv=util::int_array_from_string(coms[3]);
    int ww[4]; ww[0]=-vv[0];ww[1]=vv[1]; ww[2]=vv[2];ww[3]=vv[3];
     active->shift_finish_anno(vv[0],vv[1],vv[2],vv[3]);
    delete [] vv;
      active->update_display(true);
  }  
  else if(coms[2].contains("shift")>0){
    int *vv=util::int_array_from_string(coms[3]);
    if(vv[0]>=0) active->shift_anno(vv[0],vv[1],vv[2],vv[3]);
    delete [] vv;
      active->update_display(true);
  }  
  else if(coms[2].contains("finish")>0){
    int *vv=util::int_array_from_string(coms[3]);
    active->finish_anno(vv[0],vv[1],vv[2],vv[3]);
    delete [] vv;
      active->update_display(true);
  }
  
  else if(coms[2].contains("remove")>0){
    int *vv=util::int_array_from_string(coms[3]);
    active->del_anno(vv[0],vv[1]);
    delete [] vv;
      active->update_display(true);
 }
   else if(coms[2].contains("move")>0){
    int *vv=util::int_array_from_string(coms[3]);
    active->move_anno(vv[0],vv[1],vv[2],vv[3]);
    delete [] vv;
      active->update_display(true);
 }
  
}


void windows::windows_respond(std::vector<QString> coms){

  std::shared_ptr<DrawWindow>active=my_winds[coms[0].toInt()];
  if(coms[2].contains("save_big")){
    active->pdf_save("big",coms[3]);
  }
  else  if(coms[2].contains("save_small")){
  
      active->pdf_save("small",coms[3]);
  }
  
  else  if(coms[2].contains("split_x")){
    update_grid(active,active->return_grid1(),active->return_grid2(),coms[3].toInt(),active->return_grid2());
      active->update_display(true);
  }
    else  if(coms[2].contains("split_y")){
    update_grid(active,active->return_grid1(),active->return_grid2(),active->return_grid1(),coms[3].toInt());
      active->update_display(true);
  }
    else  if(coms[2].contains("boundary")){
      active->set_boundary_thick(coms[3].toInt());
      active->update_panel_map();
      active->update_display(true);
 }
    else  if(coms[2].contains("close")){
      active->hide(); 
  }

}
void windows::update_grid(std::shared_ptr<DrawWindow>active,int noldx,int noldy, int nnewx, int nnewy){
   std::vector<QString> coms;
   coms.push_back("-1");coms.push_back("panels"); coms.push_back("delete");
    coms.push_back("xx"); 
  if(noldx*noldy>nnewx*nnewy){
    for(int i=0;i < noldx*noldy-nnewx*nnewy;i++){
      std::shared_ptr<panel>pan=active->remove_last_panel();
      int ival=pan->get_iview();
      my_pan->delete_panel(ival);
      coms[3]=QString::number(ival);
      emit actionDetected(coms);
    }
  }
  else if(noldx*noldy<nnewx*nnewy){
    /*we need to add some panels*/
    coms[2]="add";
    for(int i=0;i < -noldx*noldy+nnewx*nnewy;i++){
    int in=my_pan->get_next_panel_num();
    std::shared_ptr<panel> x=active->return_panel(0)->clone(in);
    my_pan->add_panel(x);
    my_pan->set_window(in,active->get_inum());
    active->add_panel(x);
    coms[3]=QString::number(in);
    emit actionDetected(coms);
    }
  }
  active->set_grid1(nnewx);
  active->set_grid2(nnewy);
  


}
void windows::update_all(){
 update_displays();
 }
void windows::actionRespond(std::vector<QString> coms){
  emit actionDetected(coms);
}

void windows::connect_all(){
  for(int i=0; i<(int)my_winds.size(); i++){
    connect_it(i);
  }

}
void windows::update_displays(bool force){
   for(int i=0; i < (int) my_winds.size(); i++){
     my_winds[i]->update_display(force);
     
     }
}
void windows::connect_it(int i){

connect(my_winds[i].get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

}
void windows::disconnect_it(int i){

  disconnect(my_winds[i].get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

}

