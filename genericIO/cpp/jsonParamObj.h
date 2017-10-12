#ifndef JSON_PARAM_OBJ
#define JSON_PARAM_OBJ 1
#include "json.h"
#include "paramObj.h"
#include <memory>
namespace SEP{


class jsonParamObj: public paramObj{
  public:
    jsonParamObj(std::shared_ptr<Json::Value> inp) ;
    virtual int getInt(const std::string& arg)const ;
    virtual int getInt(const std::string& arg, const int def)const ;
   
    virtual float getFloat(const std::string&, const float def)const ;
    virtual float getFloat(const std::string&)const  ;
   
    virtual std::string getString(const std::string &arg)const ;
    virtual std::string getString(const std::string &arg, const std::string& def)const ;
  
   
    virtual bool getBool(const std::string&, const bool def)const ;
    virtual bool getBool(const std::string&)const ;
   
   
    virtual std::vector<int> getInts(const std::string &arg,const int nvals)const ;
    virtual std::vector<int> getInts(const std::string &arg,std::vector<int> &defs)const ;
     
    virtual std::vector<float> getFloats(const std::string &arg,const int nvals)const ;
    virtual std::vector<float> getFloats(const std::string &arg,std::vector<float>& defs)const ;
    virtual void message(const std::string& err)const  ;

    virtual void error(const std::string& err)const  ;

  protected:
   std::shared_ptr<Json::Value> jsonArgs;
};



}


#endif