#include<multiple.h>
#include<ctime>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPolygon>

using namespace SEP;
int multiple::in_slice(int ix, int iy){
  for(int i=0;i < (int)slices.size(); i++){
    if (slices[i]->in_slice(ix,iy)) return i;
  }
  
  return -1;

}


std::vector<QString> multiple::key_release_multiple( QKeyEvent *e,std::shared_ptr<orient_cube>pos ){
  return key_press_view(e,pos);
}
std::vector<QString> multiple::key_press_multiple( QKeyEvent *e,std::shared_ptr<orient_cube>pos ){
  if(e==0);
 return key_press_view(e,pos);
}

