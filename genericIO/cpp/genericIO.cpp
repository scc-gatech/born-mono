#include "genericIO.h"
using namespace SEP;
void genericIO::filesClose(){
	for(auto i=_regFiles.begin(); i!=_regFiles.end(); ++i) {

		i->second->close();
	}
	for(auto i=_irregFiles.begin(); i!=_irregFiles.end(); ++i) {
		i->second->close();
	}

}
std::shared_ptr<SEP::genericRegFile> genericIO::getRegFile(const std::string &name,const SEP::usage_code usage){


	std::shared_ptr<paramObj> par=getParamObj();
	std::string filename=par->getString(name,name);
	return getRegFileTag(name,filename,usage);


}
std::shared_ptr<SEP::genericIrregFile> genericIO::getIrregFile(const std::string& name,const SEP::usage_code usage){

	std::shared_ptr<paramObj> par=getParamObj();
	std::string filename=par->getString(name,name);

	return getIrregFileTag(name,filename,usage);


}
