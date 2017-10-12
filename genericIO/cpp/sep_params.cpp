#include "sep_params.h"
using namespace SEP;
extern "C" {
#include "seplib.h"
}
#include<string.h>
sepParam::sepParam(const int argc,  char **argv) {
 initpar(argc,argv);
}

int sepParam::getInt(const std::string &arg)const {
    int x;
    if(0==getch(arg.c_str(),"d",&x))
      error(std::string("1trouble grabbing parameter ")+arg+std::string(" from parameters"));
    return x;
}
int sepParam::getInt(const std::string &arg, const int def)const {
   int x=def;
   int i=getch(arg.c_str(),"d",&x);
   return x;
}
   
float sepParam::getFloat(const std::string & arg, const float def)const {
  float x=def;
  int i=getch(arg.c_str(),"f",&x);
  return x;

}
float sepParam::getFloat(const std::string  &arg)const {
  float x;
  if(0==getch(arg.c_str(),"f",&x))
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  return x;
}
void sepParam::message(const std::string& arg)const{

  sepwarn(0,arg.c_str());

}

std::string sepParam::getString(const std::string & arg)const {
  char buf[10000];
  if(0==getch(arg.c_str(),"s",buf))
      error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  return std::string(buf);


}
std::string sepParam::getString(const std::string &arg, const std::string &def)const {

  char buf[10000];
  strcpy(buf,def.c_str());
 // std::copy(def.begin(), def.end(), buf);
  int i=getch(arg.c_str(),"s",buf);

  return std::string(buf);

}
 
bool sepParam::getBool(const std::string &arg,  bool def)const {
  bool x=def;
  int i=getch(arg.c_str(),"l",&x);
  return x;

}
bool sepParam::getBool(const std::string &arg)const {
 bool x;
  if(0==getch(arg.c_str(),"l",&x)){
    error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  }
  return x;

}
   
   
std::vector<int> sepParam::getInts(const std::string &arg,const int nvals )const {
  int tmp[10000];
  int ierr=getch(arg.c_str(),"d",tmp);
  if(ierr==0) 
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::vector<int> x;
  for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  return x;
}
std::vector<int> sepParam::getInts(const std::string &arg,std::vector<int>& defs)const {
  int tmp[10000];
  for(int i=0; i < defs.size(); i++){
    tmp[i]=defs[i];
  }
  int ierr=getch(arg.c_str(),"d",tmp);
 
  std::vector<int> x;
  if(ierr>0){  
    for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  }
  else{
    for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
  }
  return x;
}
     
std::vector<float> sepParam::getFloats(const std::string &arg,const int nvals)const {
  float tmp[10000];
  int ierr=getch(arg.c_str(),"f",tmp);
  if(ierr==0) 
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::vector<float> x;
  for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  return x;


}
std::vector<float> sepParam::getFloats(const std::string  &arg,std::vector<float> &defs)const {

  float tmp[10000];
  for(int i=0; i < defs.size(); i++){
    tmp[i]=defs[i];
  }
  int ierr=getch(arg.c_str(),"f",tmp);
  std::vector<float> x;
  if(ierr>0){  
    for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  }
  else{
    for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
  }
  return x;
}


void sepParam::error(const std::string &errm)const {;
   seperr(errm.c_str());
}
    

    


