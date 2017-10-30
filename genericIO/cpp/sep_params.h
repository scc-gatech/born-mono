#ifndef SEP_PARAM_FUNC_H
#define SEP_PARAM_FUNC_H 1
#include "paramObj.h"
#include<string>
#include<stdbool.h>
namespace SEP{
class sepParam: public SEP::paramObj{
  public:
    sepParam(const int argc, char **argv);
    virtual int getInt(const std::string &arg)const ;
    virtual int getInt(const std::string &arg, const int def)const ;
   
    virtual float getFloat(const std::string&, const float def)const ;
    virtual float getFloat(const std::string&)const  ;
   
    virtual std::string getString(const std::string &arg)const ;
    virtual std::string getString(const std::string& arg, const std::string &def)const ;
  
   
    virtual bool getBool(const std::string&, const bool def)const ;
    virtual bool getBool(const std::string&)const ;
   
   
    virtual std::vector<int> getInts(const std::string& arg,const int nvals)const ;
    virtual std::vector<int> getInts(const std::string& arg,std::vector<int>& defs)const ;
     
    virtual std::vector<float> getFloats(const std::string &arg,const int nvals)const ;
    virtual std::vector<float> getFloats(const std::string &arg,std::vector<float> &defs)const ;
       
    virtual void error(const std::string &errm)const  ;
    virtual void message(const std::string&msg)const;



    






};
}

#endif
