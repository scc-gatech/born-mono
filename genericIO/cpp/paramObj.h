#ifndef PARAM_OBJ_H
#define PARAM_OBJ_H 1
#include<string>
#include<stdbool.h>
#include<vector>

namespace SEP{
class paramObj {
  public:
  
    paramObj(){;}
    virtual int getInt(const std::string &arg) const =0;
    virtual int getInt(const std::string& arg, const int def) const=0;
   
    virtual float getFloat(const std::string& arg, const  float def)const =0;
    virtual float getFloat(const std::string& arg)const =0;
    virtual std::string getString(const std::string &arg)const =0;
    virtual std::string getString(const std::string &arg, const std::string &def)const=0;
    virtual bool getBool(const std::string &arg, const bool def) const =0;
    virtual bool getBool(const std::string& arg )const  =0;
    virtual std::vector<int> getInts(const std::string &arg,const int nvals)const =0;
    virtual std::vector<int> getInts(const std::string& arg,std::vector<int>& defs)const =0;     
     
    virtual std::vector<float> getFloats(const std::string &arg,int nvals)const =0;
    virtual std::vector<float> getFloats(const std::string& arg,std::vector<float>& defs)const =0;
    virtual void error(const std::string&err)const =0;
    virtual void message(const std::string&msg)const=0;
       
};
}

#endif

