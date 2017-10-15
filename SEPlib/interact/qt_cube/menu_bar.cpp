#include "menu_bar.h"

menu_bar::menu_bar(param_func *p, datasets *dat,int ndims,
    QString num)  {

 if(p==0){}
  vnum=num;
  if(num=="0"){
     parts["Main"]= new main_menu( dat,num,ndims);
    order.push_back("Main");
  }

  parts["Navigate"]=new navigate_menu(dat,num,ndims);
  order.push_back("Navigate");

  parts["View"]=new view_menu(dat,num,ndims);
  order.push_back("View");
  parts["Orient"]=new orient_menu( dat,num,ndims);
  order.push_back("Orient");

  //parts["Display"]=new color_menu(dat,num,ndims);
 // order.push_back("Display");
 //  parts["Clip"]=new clip_menu( dat,num,ndims);

 //order.push_back("Clip");
 
     parts["Auto"]=new autopick_menu( dat,num,ndims);
  order.push_back("Auto");



  if(num=="0"){
    parts["Picks"]=new pick_menu(dat,num,ndims);
    order.push_back("Picks");

    parts["Help"]= new help_menu( dat,num,ndims);
    order.push_back("Help");
  }

  for(int i=0; i < (int) order.size(); i++){
     parts[order[i]]->create_menu();

   connect(parts[order[i]], SIGNAL(action_detected(std::vector<QString>)), 
        this, SLOT(receive_menu(std::vector<QString>)));
    this->insertTab(parts[order[i]],order[i]);

  }

  
}
void menu_bar::receive_menu(std::vector<QString> com){
  emit actionDetected(com);
}
void menu_bar::actionRespond(std::vector<QString> com){
  parts[com[2]]->actionRespond(com);
}
