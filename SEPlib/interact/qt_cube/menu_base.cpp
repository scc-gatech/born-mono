#include "menu_base.h"


void menu_base::set_basics(datasets *dats, QString num, QString sub,
      int nd){
   
 datas=dats;
 vnum=num; sub_name=sub; ndims=nd;    
}

void menu_base::send_message(std::vector<QString> com){
  com.insert(com.begin(),vnum);
  emit action_detected(com);
}
