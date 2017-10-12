#include "datasets.h"
#include "surface_data.h"
#include "incore_data.h"
using namespace SEP;
datasets::datasets(std::shared_ptr<paramObj >p,std::shared_ptr<pick_draw >pk){
  par=p;
  std::shared_ptr<util> u(new util(par));
  my_util=u;
  my_pk=pk;
}
void datasets::clean_dats(){


  dats.clear();
}
std::shared_ptr<dataset> datasets::return_dat(int idat){
  if(idat < 0 || idat >= (int)dats.size()){
//    par->error("Invalid dataset \n");
  }
  return dats[idat];

}
void datasets::afterInitialization(){

   for(int i=0; i < (int)dats.size(); i++){  
      dats[i]->afterInitialization();
  }
}
void datasets::update_data(std::vector<QString> command){
  int idat=command[0].toInt();

  if(command[2].contains("add")>0){
      dats[idat]->add_pt(command[3].section(":",0,0).toFloat(),
        command[3].section(":",1,1).toFloat());
  }
  else if(command[2].contains("axis")>0){
    int iax=command[3].toInt();
    dats[idat]->set_display_axis(iax-1,command[4]);
  }
  else if(command[2].contains("set")>0){
      float *pct=util::float_array_from_string(command[3]);
      QString tmp=dats[idat]->set_pts(pct);      
      dats[idat]->add_pt(command[3].section(":",0,0).toFloat(),
              command[3].section(":",1,1).toFloat());

      delete [] pct;

  }
  else if(command[2].contains("clear")>0){
       dats[idat]->clear_pts();
  }
  else if(command[2].contains("delete")>0){
     float pct,clp;
     dats[idat]->get_pt(command[3].toInt(),&pct,&clp);
     dats[idat]->del_pt(command[3].toInt());
  }
  else if(command[2].contains("beg_clip")>0){
    dats[idat]->set_bcolor(command[3]);
   
  }
  else if(command[2].contains("surface")>0){
    if(command[3].contains("update")>0){
      std::shared_ptr<dataset> r=dats[idat];
      std::shared_ptr<surface_data> x=std::static_pointer_cast<surface_data>( r);
      x->update_surface();
     // ( (std::shared_ptr<surface_data>) dats[idat])->update_surface();
    }
}
  else if(command[2].contains("end_clip")>0){
    dats[idat]->set_ecolor(command[3]);
  }
  else if(command[2].contains("new")>0){
  
     par->error("Error no longer supported ");
    

   

  }

}
std::shared_ptr<dataset> datasets::return_dat(QString nm){
  for(int i=0; i < (int) dats.size(); i++){
    if(dats[i]->get_name()==nm) return dats[i];
  }
  //par->error("invalid dataset");
  return NULL;
}
void datasets::actionRespond(std::vector<QString> coms){

  emit actionDetected(coms);
}
void datasets::add_dat(std::shared_ptr<dataset> d){

     connect(d.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

  dats.push_back(d);
}
int datasets::dataNumFromName(QString name){
  for(int i=0; i < return_ndat(); i++){
    if(name==dats[i]->get_name()) return i;
  }
  return 0;

}

