#include "panel_panel.h"
#include "display_panel.h"
#include "orient_panel.h"
#include "view_panel.h"
#include "navigate_panel.h"
using namespace SEP;
panel_panel::panel_panel(std::shared_ptr<position> p,std::shared_ptr<windows> my_w, std::shared_ptr<panels>my_p,
 std::shared_ptr<datasets> my_d, std::shared_ptr<pick_draw> pk, std::shared_ptr<slice_types>c,
  std::shared_ptr<maps> mym){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);
   for(int i=0; i < my_p->size(); i++){
      my_tabs[QString::number(i+1)]=new sub_panel_panel(p,my_w,my_p,my_d,pk,c,mym,QString::number(i));
      addTab(my_tabs[QString::number(i+1)],QString::number(i+1));
      connect(my_tabs[QString::number(i+1)], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
      tab_order.push_back(i+1);
  }
  
//  setMinimumHeight(500);



  
}
void panel_panel::set_current_index(int ind){

  int i=0;
  while(i < (int)tab_order.size()){
    if(tab_order[i]==ind+1){
      setCurrentIndex(i);
      return;
     }
     i++;
    }
}

void panel_panel::update_menu(std::vector<QString> coms){

 if(coms[1].contains("panels")){
   int inum=coms[3].toInt(), ip=inum+1;
    if(coms[2].contains("add")){
       my_tabs[QString::number(ip)]=
         new sub_panel_panel(my_pos,my_wind,my_pan,my_dat,my_pick,my_slices,my_maps,QString::number(inum));
      addTab(my_tabs[QString::number(ip)],QString::number(ip));
      tab_order.push_back(ip);
          connect(my_tabs[QString::number(ip)], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

    }
    else if(coms[2].contains("delete")){
      int i=0;
      bool found=false;
      while(!found &&  i < (int)tab_order.size()){
       if(tab_order[i]==ip){
         removeTab(i);
         tab_order.erase(tab_order.begin()+i);
       }
       i++;
    }
 }
 }
  else{
   for(std::map<QString,QWidget*>::iterator i=my_tabs.begin(); i!=my_tabs.end(); i++){
     sub_panel_panel *x=(sub_panel_panel*) i->second;
     x->update_menu(coms);
   }
}
}

void panel_panel::actionRespond(std::vector<QString> coms){
  emit actionDetected(coms);

}
void sub_panel_panel::update_menu(std::vector<QString>coms){
  if(coms[0].contains("-1") || coms[0].contains(vnum)){
     ((view_panel*)my_tabs["view"])->update_menu(coms);
     ((navigate_panel*)my_tabs["navigate"])->update_menu(coms);
     ((orient_panel*)my_tabs["orient"])->update_menu(coms);
     ((display_panel*)my_tabs["display"])->update_menu(coms);
  }
 }
sub_panel_panel::sub_panel_panel(std::shared_ptr<position>p,
  std::shared_ptr<windows>my_w, std::shared_ptr<panels> my_p, std::shared_ptr<datasets>
  my_d,std::shared_ptr< pick_draw> pk, std::shared_ptr<slice_types>c,
  std::shared_ptr<maps> mym,QString v){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  vnum=v;

  my_tabs["view"]=new view_panel(p,my_w,my_p,my_d,pk,c,mym);
  addTab(my_tabs["view"],"View");
  
    my_tabs["navigate"]=new navigate_panel(p,my_w,my_p,my_d,pk,c,mym);
  addTab(my_tabs["navigate"],"Navigate");
  
  
  my_tabs["orient"]=new orient_panel(p,my_w,my_p,my_d,pk,c,mym);
  addTab(my_tabs["orient"],"Orient");
 
  my_tabs["display"]=new display_panel(p,my_w,my_p,my_d,pk,c,mym);
  addTab(my_tabs["display"],"Display");
   
  connect(my_tabs["view"], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
  connect(my_tabs["navigate"], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
  connect(my_tabs["orient"], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
  connect(my_tabs["display"], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));


}
void sub_panel_panel::actionRespond(std::vector<QString> coms){
  coms.insert(coms.begin(),vnum);
  emit actionDetected(coms);

}


  


  
