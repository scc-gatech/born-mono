#ifndef MY_COLORS
#define MY_COLORS 1

#include <qcolor.h>
#include <qstring.h>
#include  <map>
#include <vector>
class my_colors{
  public:
    my_colors();
   ~my_colors(){
      cvec.clear(); 
      cmap.clear();
    }
    std::vector<QString> return_cvec(){ return cvec;}
    QColor return_qcolor(QString col){ return cmap[col];}
     std::vector<QString> return_cvec_big(){ return cvec_big;}
    QColor return_qcolor_big(QString col){ return cmap_big[col];}
    std::vector<QString> cvec,cvec_big;
    std::map<QString, QColor> cmap,cmap_big;
    std::map<QString,int> cnum,loc;


};

#endif
