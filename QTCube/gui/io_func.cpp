#include "io_func.h"
#include "iostream"
using namespace SEP;
void io_func::set_basics(std::shared_ptr<util>pars, std::shared_ptr<hypercube>h){
     par=pars;
   
     std::shared_ptr<hypercube> hh(new hypercube());
     hyper=hh;
     std::vector<axis> axes;
     for(int i=0; i < h->getNdim(); i++) axes.push_back(h->getAxis(i+1));
     for(int i=h->getNdim(); i<8; i++) axes.push_back(axis(1));
     hyper->setAxes(axes);
     non_byte=true;
     changed=false;
     isregion=false;
     data_type="";
}

    
   
    
void io_func::read_block_float(std::vector<int>&nw, std::vector<int>&fw, float *buf){
  if(nw[0]==0 && fw[0]==0 && buf==0);
      par->error("Undefined  read_block_float for this type");
 }
void io_func::read_block_byte(std::vector<int>&nw, std::vector<int>&fw, unsigned char *buf){
  if(nw[0]==0 && fw[0]==0 && buf==0);
      par->error("Undefined read_block_byte for this type");
   }
void io_func::convert_to_local(std::vector<int>&nwin, std::vector<int>&fwin, std::vector<int>&nwout, std::vector<int>&fwout){
     for(int i=0; (int)nwout.size(); i++){
       nwout[i]=nwin[i];
       fwout[i]=fwin[i];
     }
}
std::shared_ptr<hypercube>io_func::return_hyper(){
  if(hyper==0) par->error("Hypercube not set");
  return hyper;
}
