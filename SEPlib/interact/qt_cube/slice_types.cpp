#include<slice_types.h>
#include "wiggle.h"
#include <assert.h>
#include "contour.h"
slice_types::slice_types()
{

  slices["grey"]=new grey(255); names.push_back("grey");
  slices["flag"]=new flag(255); names.push_back("flag");
  slices["cbl"]=new cbl(255); names.push_back("cbl");
  slices["cgsi"]=new cgsi(255); names.push_back("cgsi");
  slices["rainbow"]=new rainbow(255); names.push_back("rainbow");
  slices["jet"]=new jet(255); names.push_back("jet");
  slices["patriotic"]=new patriotic(255); names.push_back("patriotic");
  slices["ava"]=new ava(255); names.push_back("ava");
  slices["cycle"]=new color_cycle(255); names.push_back("cycle");
  slices["user"]=new user_color(255); names.push_back("user");
  slices["wiggle"]=new wiggle(); names.push_back("wiggle");
  slices["contour"]=new contour(); names.push_back("contour");

}
slice *slice_types::return_color_table(QString val){
  
  assert(slices.count(val)!=0);
  for(int i=0; i < (int)names.size(); i++){
    if(names[i].contains(val)>0) icol=i;
  }
  return slices[val];

}
slice *slice_types::return_overlay_color_table(QString val,int alpha){
  assert(slices.count(val)!=0);
  assert(slices[val]->is_raster);
  raster *rast=(raster*)slices[val];
  raster *rast2=(raster*)rast->clone_alpha(alpha);
  rast2->set_overlay();

  return rast2;


}

void slice_types::set_bcolor(QString tab,QString col){
   assert(slices.count(tab)!=0);
   slices[tab]->set_bcolor(col);

}
void slice_types::set_ecolor(QString tab,QString col){
   assert(slices.count(tab)!=0);
   slices[tab]->set_ecolor(col);
}
slice *slice_types::cycle_color(){
  icol=icol+1;
  if(icol ==(int) names.size()) icol=0;
  return slices[names[icol]];
}

