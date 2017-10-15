#include "simple_color.h"
#include<string>
#include<vector>
#include<map>
#include<simple_color.h>
#ifndef SLICE_TYPES_H
#define SLICE_TYPES_H 1
class slice_types
{
  public:
  
    slice_types();
 
    slice *return_color_table(QString val);
    slice *return_overlay_color_table(QString val,int alpha);
    slice *cycle_color(); 

    void set_ecolor(QString tab,QString col);
    void set_bcolor(QString tab,QString col);
    


    ~slice_types(){
      //for(int i=0; i < (int)slices.size(); i++) delete slices[i];
      slices.clear();
      names.clear();
    }
    std::map<QString,slice*> slices;
    //std::vector<slice*> slices;
    std::vector<QString> names;
    int icol;


};

#endif
