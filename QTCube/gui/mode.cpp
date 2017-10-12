#include "mode.h"
/*
 *	The different modes allow the user to interact with the data differently and give the three mouse buttons
 *	different uses.
 */
 using namespace SEP;
mode::mode(QString init_mode){
 if(init_mode=="");
  modes.push_back("navigate");
  modes.push_back("pick");
}
void mode::change_mode(QString val){
  cur_mode=val;

}
QString mode::return_mode(){
  return cur_mode;
}

