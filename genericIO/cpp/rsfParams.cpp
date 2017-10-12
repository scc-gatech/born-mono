#include "rsfParams.h"
extern "C" {
#include "rsf.h"
}

rsfParam::rsfParam(const int argc,const char **argv){
 sf_init(argc,argv);
}

int rsfParam::getInt(const std::string& arg)const {
    int x;
    if(!sf_getint(arg.c_str(),&x))
      error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
    return x;
}
int rsfParam::getInt(const std::string arg, const int def)const {
   int x=def;
  sf_getint(arg.c_str(),&x);
   return x;
}
   
float rsfParam::getFloat(const std::string& arg, const float def)const {
  float x;
 sf_getfloat(arg.c_str(),&x);  
 return x;

}
float rsfParam::getFloat(const std::string& arg)const {
  float x;
  if(!sf_getfloat(arg.c_str(),&x));
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  return x;
}
   
std::string rsfParam::getString(const std::string &arg)const {
  char buf[10000];
  char *tmp=sf_getstring(arg.c_str());
  if(tmp==NULL) error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::string y= std::string(tmp);
  free(tmp);
  return y;

}
std::string rsfParam::getString(const std::string& arg, const std::string &def)const {

  char *buf =sf_getstring(arg.c_str());
  if(buf==NULL) return def;
  std::string y= std::string(buf);
  free(buf);
  return y;
}
 
bool rsfParam::getBool(const std::string& arg,  bool def)const {
  bool x=def;
  sf_getbool(arg.c_str(),&x);
  return x;

}
bool rsfParam::getBool(const std::string& arg)const {
 bool x;
  if( !sf_getbool(arg.c_str(),&x)){
    error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  }
  return x;

}
   
   
std::vector<int> rsfParam::getInts(const std::string &arg,const int num)const {
  int tmp[10000];
  bool ierr=sf_getints(arg.c_str(),tmp,num);
  if(!ierr) 
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::vector<int> x;
  for(int i=0; i < num; i++) x.push_back(tmp[i]);
  return x;
}
std::vector<int> rsfParam::getInts(const std::string arg,std::vector<int> defs)const {
  int tmp[10000];
  for(int i=0; i < defs.size(); i++){
    tmp[i]=defs[i];
  }
  bool ierr=sf_getints(arg.c_str(),tmp,defs.size());
 
  std::vector<int> x;
  if(ierr){  
    for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  }
  else{
    for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
  }
  return x;
}
     
std::vector<float> rsfParam::getFloats(const std::string& arg,const int num)const {
  float tmp[10000];
  bool  ierr=sf_getfloats(arg.c_str(),tmp,num);
  if(!ierr) 
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::vector<float> x;
  for(int i=0; i < num; i++) x.push_back(tmp[i]);
  return x;


}
std::vector<float> rsfParam::getFloats(const std::string& arg,std::vector<float> &defs)const {

  float tmp[10000];
  for(int i=0; i < defs.size(); i++){
    tmp[i]=defs[i];
  }
  bool ierr=sf_getfloats(arg.c_str(),tmp,defs.size());

  std::vector<float> x;
  if(ierr){  
    for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  }
  else{
    for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
  }
  return x;
}


void rsfParam::error(const std::string& err)const {
   sf_error(err.c_str());
}
    

    


