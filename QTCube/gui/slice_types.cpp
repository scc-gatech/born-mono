#include<slice_types.h>
#include "wiggle.h"
#include <assert.h>
#include "contour.h"
#include "util.h"
using namespace SEP;
slice_types::slice_types()
{
   
 std::shared_ptr<grey> x1(new grey(255)); slices["grey"]=x1; names.push_back("grey");
 std::shared_ptr<flag> x2(new flag(255)); slices["flag"]=x2; names.push_back("flag");
 std::shared_ptr<cgsi> x3(new cgsi(255)); slices["cbl"]=x3; names.push_back("cbl");
 std::shared_ptr<cgsi> x4(new cgsi(255)); slices["cgsi"]=x4; names.push_back("cgsi");
 std::shared_ptr<rainbow> x5(new rainbow(255)); slices["rainbow"]=x5; names.push_back("rainbow");
 std::shared_ptr<jet> x6(new jet(255)); slices["jet"]=x6; names.push_back("jet");
 std::shared_ptr<patriotic> x7(new patriotic(255)); slices["patriotic"]=x7; names.push_back("patriotic");
 std::shared_ptr<color_cycle> x8(new color_cycle(255)); slices["ava"]=x8; names.push_back("ava");
 std::shared_ptr<color_cycle> x9(new color_cycle(255)); slices["cycle"]=x9; names.push_back("cycle");
 std::shared_ptr<user_color> xa(new user_color(255)); slices["user"]=xa; names.push_back("user");
 std::shared_ptr<wiggle> xb(new wiggle()); slices["wiggle"]=xb; names.push_back("wiggle");
 std::shared_ptr<contour> xc(new contour()); slices["contour"]=xc; names.push_back("contour");
}


std::shared_ptr<slice>slice_types::return_color_table(const QString val){
  
  assert(slices.count(val)!=0);
  
  for(int i=0; i < (int)names.size(); i++){
    if(names[i].contains(val)>0) icol=i;
  }
  return slices[val];

}
std::shared_ptr<slice>slice_types::return_overlay_color_table(QString val,int alpha){
  assert(slices.count(val)!=0);
  assert(slices[val]->is_raster);
  std::shared_ptr<raster>rast=std::static_pointer_cast<raster>(slices[val]);
  std::shared_ptr<raster> rast2=std::static_pointer_cast<raster>(rast->clone_alpha(alpha));
  rast2->set_overlay();

  return rast2;


}
void slice_types::update_colortable(std::vector<QString> command){
  std::shared_ptr<user_color>col=std::static_pointer_cast<user_color>( slices["user"]);
  if(command[2].contains("set_green")>0){
    int *vv=util::int_array_from_string(command[3]);
    col->set_green(vv);
    delete [] vv;
  }
  else if(command[2].contains("set_blue")>0){
    int *vv=util::int_array_from_string(command[3]);
    col->set_blue(vv);
    delete [] vv;
  }
  else if(command[2].contains("set_red")>0){
    int *vv=util::int_array_from_string(command[3]);
    col->set_red(vv);
    delete [] vv;
  }
}
void slice_types::set_bcolor(QString tab,QString col){
   assert(slices.count(tab)!=0);
   slices[tab]->set_bcolor(col);

}
void slice_types::set_ecolor(QString tab,QString col){
   assert(slices.count(tab)!=0);
   slices[tab]->set_ecolor(col);
}
QString slice_types::cycle_color(){
  icol=icol+1;
  if(icol ==(int) names.size()) icol=0;
  return names[icol];
}

