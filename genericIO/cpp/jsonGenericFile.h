#ifndef JSON_REGFILE_FUNC_H
#define JSON_REGFILE_FUNC_H 1
#include <string>
#include "json.h"
#include <stdbool.h>
#include "genericFile.h"
#include "basicIO.h"
namespace SEP {
class jsonGenericFile : public genericIrregFile {
public:

// sepRegFile::sepRegFile(const std::string tag,usage_code usage){

jsonGenericFile(){
	;
}
jsonGenericFile(std::shared_ptr<Json::Value> arg,const SEP::usage_code usage,const std::string& tag,const int reelH,const int traceH);
void setupJson(std::shared_ptr<Json::Value> jsonArgs,const std::string& tag);
virtual int getInt(const std::string &arg) const;
virtual int getInt(const std::string &arg, const int def) const;

virtual float getFloat(const std::string&, const float def) const;
virtual float getFloat(const std::string&) const;

virtual std::string getString(const std::string &arg) const;
virtual std::string getString(const std::string &arg, const std::string &def) const;


virtual bool getBool(const std::string&, const bool def) const;
virtual bool getBool(const std::string&) const;


virtual std::vector<int> getInts(const std::string &arg,int nvals) const;
virtual std::vector<int> getInts(const std::string &arg,std::vector<int> &defs) const;

virtual std::vector<float> getFloats(const std::string &arg,int nvals) const;
virtual std::vector<float> getFloats(const std::string &arg,std::vector<float>& defs) const;
virtual void message(const std::string &err) const;

virtual void error(const std::string &err) const;
Json::Value getArgs(){
	return jsonArgs;
}
std::string getTag(){
	return _tag;
}
virtual void close();
std::string getJSONFileName() const;
std::string getDataFileName() const;
virtual void readDescription();
virtual void writeDescription();
virtual void putInt(const std::string &par, const int val);
virtual void putFloat(const std::string& par, const float val);
virtual void putString(const std::string& par, const std::string &val);
virtual void putBool(const std::string &par, const bool val);
virtual void putInts(const std::string& par, const std::vector<int>& val);
virtual void putFloats(const std::string& par, const std::vector<float>& val);
virtual void readFloatStream(float *array,const long long npts);
virtual void readUCharStream(unsigned char *array,const long long npts);
virtual void seekTo(const long long iv,const int whence);

virtual void writeFloatStream( const float *array,const long long npts);
virtual void readUCharWindow(const std::vector<int>& nw, const std::vector<int> &fw,
	const std::vector<int>& jw,  unsigned char *array);
virtual void readFloatWindow(const std::vector<int> &nw, const std::vector<int> &fw,
	const std::vector<int> &jw,  float *array);
virtual long long getDataSize();
virtual void writeFloatWindow(const std::vector<int> &nw, const std::vector<int> &fw,
	const std::vector<int>& jw, const float  *array);
virtual void readComplexWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int> &jw,  float _Complex*array);
virtual void writeComplexWindow(const std::vector<int>& nw, const std::vector<int> &fw,
	const std::vector<int> &jw, const float _Complex*array);
virtual void writeComplexStream( const float _Complex*array,const long long npts);
virtual void readComplexStream(float _Complex*array,const long long npts);

protected:
Json::Value jsonArgs;
private:
std::string _tag;
usage_code _usage;
std::string _jsonFile,_dataFile;
std::shared_ptr<myFileIO> myio;
int _reelH,_traceH;
bool _newFile;

};
}

#endif
