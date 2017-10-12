#ifndef MY_FONTS
#define MY_FONTS 1

#include <qcolor.h>
#include <qstring.h>
#include  <map>
#include<stdio.h>
#include <vector>
#include <QFont>
#include<qfontmetrics.h>
#include<memory>
namespace SEP{
class my_font{
  public:
     my_font(){;}
     my_font(QString xx);
     std::shared_ptr<QFontMetrics>return_font_metric(){return fm;}
     std::shared_ptr<QFont>return_font(){ return myf;}
     ~my_font(){ 

       }
  private:
     std::shared_ptr<QFontMetrics> fm;
	 std::shared_ptr<QFont>myf;
};

class my_fonts{
  public:
    my_fonts();
	std::vector<QString> font_list; 
    std::shared_ptr<QFontMetrics> return_font_metric(QString nm){ return fonts[nm]->return_font_metric();}
    std::shared_ptr<QFont> return_font(QString nm){ return fonts[nm]->return_font();}
    ~my_fonts(){
       delete_it();
 
    }
    void delete_it();
  private:
    std::map<QString,std::shared_ptr<my_font>> fonts;
   

};
}


#endif
