#ifndef SEP_REGFILE_FUNC_H
#define SEP_REGFILE_FUNC_H 1
#include <string>
#include <stdbool.h>
#include "genericFile.h"
namespace SEP {
class sepRegFile : public SEP::genericRegFile {
public:

// sepRegFile::sepRegFile(const std::string tag,usage_code usage){

sepRegFile(const std::string & tg, const SEP::usage_code usage);
virtual int getInt(const std::string &arg) const;
virtual int getInt(const std::string &arg, const int def) const;

virtual float getFloat(const std::string&, const float def) const;
virtual float getFloat(const std::string&) const;

virtual std::string getString(const std::string &arg) const;
virtual std::string getString(const std::string &arg, const std::string &def) const;

virtual void seekTo(const long long iv,const int whence);
virtual bool getBool(const std::string&, const bool def) const;
virtual bool getBool(const std::string&) const;


virtual std::vector<int> getInts(const std::string &arg,int nvals) const;
virtual std::vector<int> getInts(const std::string &arg,std::vector<int> &defs) const;

virtual std::vector<float> getFloats(const std::string &arg,int nvals) const;
virtual std::vector<float> getFloats(const std::string &arg,std::vector<float>& defs) const;

virtual void message(const std::string &err) const;
virtual void error(const std::string &err) const;

virtual void readUCharStream(unsigned char *array,const long long npts);

virtual void readComplexStream(float _Complex *array,const long long npts);
virtual void writeComplexStream( const float _Complex *array,const long long npts);

virtual void readFloatStream(float *array,const long long npts);
virtual void writeFloatStream( const float *array,const long long npts);

virtual void readUCharWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int> &jw,  unsigned char *array);

virtual void readFloatWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int> &jw,  float *array);

virtual void writeFloatWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int> &jw,  float *array);
virtual void readComplexWindow(const std::vector<int> &nw, const std::vector<int> &fw,
	const std::vector<int>& jw,  float _Complex*array);

virtual void writeComplexWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int> &jw,  float _Complex *array);

virtual void readDescription();
virtual void writeDescription();
virtual void putInt(const std::string &par, const int val);
virtual void putFloat(const std::string& par, const float val);
virtual void putString(const std::string& par, const std::string &val);
virtual void putBool(const std::string &par, const bool val);
virtual void putInts(const std::string &par, const std::vector<int>& val);
virtual void putFloats(const std::string& par, const std::vector<float>& val);
private:
std::string _tag;
};
}

#endif
