#include "datasets.h"

datasets::datasets(param_func *p){
  par=p;
 
}

void datasets::clean_dats(){

  for(int i=0; i < (int)dats.size(); i++){
    delete dats[i];
  }
  dats.clear();
}

dataset *datasets::return_dat(int idat){
  if(idat < 0 || idat >= (int)dats.size()){
    fprintf(stderr,"Requested %d ndat=%d \n",idat,(int)dats.size());
    par->error("Invalid dataset \n");
  }
  return dats[idat];

}
dataset *datasets::return_dat(QString nm){
  for(int i=0; i < (int) dats.size(); i++){
    if(dats[i]->get_name()==nm) return dats[i];
  }
  //par->error("invalid dataset");
  return NULL;
}

void datasets::add_dat(dataset *d){

  dats.push_back(d);
}

