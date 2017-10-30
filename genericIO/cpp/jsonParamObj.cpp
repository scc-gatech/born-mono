#include "jsonParamObj.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <exception>
using namespace SEP;


jsonParamObj::jsonParamObj(std::shared_ptr<Json::Value> inp){
  jsonArgs=inp;

}

int jsonParamObj::getInt(const std::string &arg)const {
    int x;
    if((*jsonArgs)[arg].isNull())  
      error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
     x=jsonArgs->get(arg,1).asInt();
    return x;
}
int jsonParamObj::getInt(const std::string &arg, const int def)const {
   int x=jsonArgs->get(arg,def).asInt();
   return x;
}
   
float jsonParamObj::getFloat(const std::string &arg, const float def)const {
  float x;
  x= jsonArgs->get(arg,def).asFloat();
  return x;

}
float jsonParamObj::getFloat(const std::string &arg)const {
  float x;
 if((*jsonArgs)[arg].isNull())  
      error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  x=jsonArgs->get(arg,1.).asFloat();
  return x;
}
   
std::string jsonParamObj::getString(const std::string &arg)const {
   if((*jsonArgs)[arg].isNull())  
      error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
 
  return jsonArgs->get(arg,"").asString();


}
std::string jsonParamObj::getString(const std::string &arg, const std::string &def)const {

  return  jsonArgs->get(arg,def).asString();

}
 
bool jsonParamObj::getBool(const std::string &arg, const  bool def)const {
 return  jsonArgs->get(arg,def).asBool();
}
bool jsonParamObj::getBool(const std::string &arg)const {
  if((*jsonArgs)[arg].isNull())  
      error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  jsonArgs->get(arg,false).asBool();
}
   
   
std::vector<int> jsonParamObj::getInts(const std::string &arg,const int nvals)const {
   if((*jsonArgs)[arg].isNull())  
    error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  const Json::Value vals = (*jsonArgs)[arg];
  
  std::vector<int> x;
  for(int i=0; i < nvals; i++) x.push_back(vals[i].asInt());
  return x;
}
std::vector<int> jsonParamObj::getInts(const std::string &arg,std::vector<int>& defs)const {
  std::vector<int> x;
   if((*jsonArgs)[arg].isNull())  {
     for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
  }
  else{
     const Json::Value vals = (*jsonArgs)[arg];
      for(int i=0; i < defs.size(); i++) x.push_back(vals[i].asInt());
  }
  return x;
}
     
std::vector<float> jsonParamObj::getFloats(const std::string &arg,const int nvals)const {
  if((*jsonArgs)[arg].isNull())    
    error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  const Json::Value vals = (*jsonArgs)[arg];
  
  std::vector<float> x;
  for(int i=0; i < nvals; i++) x.push_back(vals[i].asFloat());
  return x;

}
std::vector<float> jsonParamObj::getFloats(const std::string &arg,std::vector<float>& defs)const {
  std::vector<float> x;
   if((*jsonArgs)[arg].isNull())   {
     for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
  }
  else{
     const Json::Value vals = (*jsonArgs)[arg];
      for(int i=0; i < defs.size(); i++) x.push_back(vals[i].asFloat());
  }
  return x;

}

void jsonParamObj::message(const std::string &errm)const {
   std::cerr<<errm<<std::endl;
}
void jsonParamObj::error(const std::string &errm)const {
   std::cerr<<errm<<std::endl;
   throw std::exception();
}
    

    
