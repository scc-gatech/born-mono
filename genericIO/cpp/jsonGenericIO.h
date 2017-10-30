#ifndef JSON_IO_H
#define JSON_IO_H 1
#include "genericIO.h"
#include "jsonGenericFile.h"
#include "jsonParamObj.h"
#include <fstream>      // std::ifstream
namespace SEP {
class jsonGenericIO : public genericIO {
public:
jsonGenericIO(){
	;
}
jsonGenericIO( const int argc,   char **argv){
	initJsonPars(argc,argv);
}
void initJsonPars( const int argc,  char **argv);
std::shared_ptr<Json::Value> getArgs(){
	return jsonArgs;
}
virtual std::shared_ptr<genericRegFile> getRegFileTag(const std::string &tag, const std::string& name,const usage_code usage);
virtual std::shared_ptr<genericIrregFile> getIrregFileTag(const std::string &tag,const std::string &name, const usage_code usage);
virtual void close();
virtual std::shared_ptr<paramObj>  getParamObj();


private:
std::ifstream inps;
std::shared_ptr<Json::Value> jsonArgs;
bool _init;



};
}
#endif
