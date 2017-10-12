#include "my_fonts.h"

using namespace SEP;
my_font::my_font(QString name){
    QString nm=name.section("-",0,0);
    int sz=name.section("-",1,1).toInt();
    QString typ=name.section("-",2,2);
    if(typ.contains("Normal")){
      std::shared_ptr<QFont> x(new QFont(nm,sz,QFont::Normal));
      myf=x;
    }
    else if (typ.contains("Bold")){
      
     std::shared_ptr<QFont> y(new QFont(nm,sz,QFont::Bold));
     myf=y;
    }
    std::shared_ptr<QFontMetrics> z(new QFontMetrics(*myf));
    fm=z;
}

my_fonts::my_fonts(){

  font_list.push_back("Arial-12-Normal");

  font_list.push_back("Arial-12-Bold");
  font_list.push_back("Arial-14-Bold");
  font_list.push_back("Arial-16-Bold");
  font_list.push_back("Arial-17-Bold");
  for(int i=0; i < (int)font_list.size(); i++) {
    std::shared_ptr<my_font> y(new my_font(font_list[i]));
    fonts[font_list[i]]=y;
  }
}
void my_fonts::delete_it(){


}

