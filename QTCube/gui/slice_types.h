#include "simple_color.h"
#include<string>
#include<vector>
#include<memory>
#include "slice.h"
#include<map>
#include<simple_color.h>
#ifndef SLICE_TYPES_H
#define SLICE_TYPES_H 1
namespace SEP{
class slice_types
{
  public:
  
    slice_types();
 
    std::shared_ptr<slice>return_color_table(const QString val);
    std::shared_ptr<slice>return_color_table(std::string x){
       return return_color_table(QString(x.c_str()));
    }
    std::shared_ptr<slice>return_overlay_color_table(QString val,int alpha);
    QString cycle_color(); 

    void set_ecolor(QString tab,QString col);
    void set_bcolor(QString tab,QString col);
    void update_colortable(std::vector<QString> coms);


    ~slice_types(){
      //for(int i=0; i < (int)slices.size(); i++) delete slices[i];
      slices.clear();
      names.clear();
    }
    std::map<QString,std::shared_ptr<slice>> slices;
    //std::vector<slice*> slices;
    std::vector<QString> names;
    int icol;


};
}
#endif
