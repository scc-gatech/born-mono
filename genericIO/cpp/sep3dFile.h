#ifndef SEP_3D_FUNC_H
#define SEP_3D_FUNC_H 1
#include<string>
#include<stdbool.h>
#include "genericFile.h"
extern "C" {
#include "seplib.h"
}
namespace SEP{
class sep3dFile: public genericIrregFile{
  public:
  sep3dFile(){;}
  sep3dFile(const std::string & tag,const SEP::usage_code usage);
      virtual int getInt(const std::string &arg)const;
    virtual int getInt(const std::string &rg, const int def)const;
   
    virtual float getFloat(const std::string &, const float def)const;
    virtual float getFloat(const std::string&)const ;
   
    virtual std::string getString(const std::string &rg)const;
    virtual std::string getString(const std::string &rg, const std::string &def)const;
  
   
    virtual bool getBool(const std::string&, const bool def)const;
    virtual bool getBool(const std::string&)const;
   
   
    virtual std::vector<int> getInts(const std::string &rg,int num)const;
    virtual std::vector<int> getInts(const std::string &rg,std::vector<int> &defs)const;
     
    virtual std::vector<float> getFloats(const std::string &rg,int num)const;
    virtual std::vector<float> getFloats(const std::string &rg,std::vector<float> &defs)const;
       
    virtual void error(const std::string err) const;
    
    virtual void readFloatStream(float *array,const long long npts);
    virtual void readUCharStream(unsigned char *array,const long long npts);

    virtual void writeFloatStream( const float *array,const long long npts);
    virtual void readUCharWindow(const std::vector<int>& nw, const std::vector<int> &fw, 
      const std::vector<int>& jw,  unsigned char *array);
    virtual void readFloatWindow(const std::vector<int>& nw, const std::vector<int> &fw, 
      const std::vector<int>& jw,   float *array);

    virtual void writeFloatWindow(const std::vector<int>& nw, const std::vector<int>& fw, 
      const std::vector<int>& jw, const float *array);
     virtual void readDescription();
    virtual void writeDescription();
     virtual void putInt(const std::string par, const int val)const ;
    virtual void putFloat(const std::string par, const float val)const;
    virtual void putString(const std::string par, const std::string val)const;
    virtual void putBool(const std::string par, const bool val)const;
    virtual void putInts(const std::string par, const  std::vector<int> &val)const;
    virtual void putFloats(const std::string par, const std::vector<float>& val)const ;
    virtual int getHeaderIndex(std::string keyname)const;
    virtual std::vector<headerType> getTraceHeader(long long index)const;
    virtual std::vector<std::string> getHeaderTypes()const;
    virtual void close()const{
      auxclose(_tag.c_str());
    }
  private:
    std::string _tag;
};
}
#endif
