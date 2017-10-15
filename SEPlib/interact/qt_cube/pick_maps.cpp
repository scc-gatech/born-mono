
#include "pick_maps.h"
//#include "fstream.h"
#include "stdio.h"
#include "axis.h"
#include "my_colors.h"

pick_maps::pick_maps(){
  my_colors cols=my_colors();
  
  std::vector<QString> cvec=cols.return_cvec();

  for(int i=0; i < (int)cvec.size(); i++)
    groups[cvec[i]]=new pikset(cvec[i],cols.return_qcolor(cvec[i]),0,20,0);
   
  change_active("red");
}

void pick_maps::change_active(QString name){ 
  active_name=name;
  active=groups[name];
}

float *pick_maps::return_pick_line(int n, int *ipos,float *scale,float *os, float *ds,
  int ax,int single){

   return active->return_pick_line(n,ipos,scale,os,ds,ax,single);
  
}

