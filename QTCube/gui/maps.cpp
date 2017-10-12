#include "maps.h"
#include <QFileInfo>
#include "hypercube_float.h"
using namespace SEP;
map_1::map_1(std::shared_ptr<genericIO> io,QString nm, const std::shared_ptr<hypercube> h,float over){
  oversamp=over;
  _io=io;
  
  QString n=QFileInfo(nm).baseName();
  name=n;
  if(name.length()>20){
    name=name.remove(0,name.length()-20);
  }
  std::string nmS=std::string(nm.toAscii().constData());
 std::shared_ptr<genericRegFile> rfile=io->getRegFile(nmS,usageIn);
 
 int ns[8]; 
 long long sz=1;
 for(int i=0; i < 8; i++) {
   ns[i]=h->getAxis(i+1).n;
   sz=sz*(long long)ns[i];
}
     assert(sz==rfile->getHyper()->getN123()) ;
     hypercube_float buf=hypercube_float(h);
     ind.resize(sz);
     rfile->readFloatStream(&buf.vals[0],sz);
     long long i=0;
     for(long long i2=0; i2 < sz/(long long)ns[0]; i2++){
       for(int i1=0 ;   i1 < ns[0]; i1++,i++){
           ind[i]=(int)(oversamp*(buf.vals[i]-1.));
           if(ind[i] < 0) ind[i]=0;
           if(ind[i]>= ns[0]) ind[i]=ns[0]-1;   
       }
     }
}


std::shared_ptr<map_1> maps::return_map(int inum){
   return my_maps[inum];
}
std::shared_ptr<map_1> maps::return_map(QString nm){
  for(int i=0; i < (int) my_maps.size();i++){
    if(nm==my_maps[i]->return_name()) 
      return my_maps[i];
  }
  return my_maps[0];
}
std::vector<QString> maps::return_names(){

  std::vector<QString> nms;
  for(int i=0; i < (int) my_maps.size(); i++){
    nms.push_back(my_maps[i]->return_name());
  }
  return nms;
}
int maps::return_num(QString nm){

for(int i=0; i < (int) my_maps.size();i++){
    if(nm==my_maps[i]->return_name()) 
      return i;
  }
  return 0;
}
