#include "jsonGenericIO.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <exception>
using namespace SEP;
void jsonGenericIO::initJsonPars( const int argc,  char **argv){

	setValid(false);
	bool foundIn=false;
	std::string fileIn,fileOut;
	int i=0;
	/*Looking for jsonIn and jsonOut"*/
	while(i < argc && !foundIn ) {
		std::string arg=std::string(argv[i]);
		if(arg.length() > 8) {
			if(arg.substr(0,5)==std::string("json=")) {
				fileIn=arg.substr(5); foundIn=true;
			}

		}
		i++;
	}
	_init=false;
	if(!foundIn ) {
		return;

	}
	_init=true;
	inps.open(fileIn,std::ifstream::in);
	std::shared_ptr<Json::Value> v(new Json::Value());
	jsonArgs=v;
	if(!inps) {
		std::cerr<<std::string("Trouble openinga"+std::string(fileIn))<<std::endl;
		throw std::exception();
	}
	try{
		inps>>(*jsonArgs);
	}
	catch(int x) {
		std::cerr<<std::string("Trouble parsing JSON file "+std::string(fileIn))<<std::endl;
		throw std::exception();

	}

	std::shared_ptr<jsonParamObj> x(new jsonParamObj(jsonArgs));

	_type="JSON";
	_param=x;
	setValid(true);
}
std::shared_ptr<genericRegFile>  jsonGenericIO::getRegFileTag(const std::string &tag,const std::string &name,const usage_code usage){
	if(!_init) {
		std::cerr<<std::string("Expecting json=file on the command line")<<std::endl;
		throw std::exception();

	}
	/*
	   if((*jsonArgs)[name].isNull())  {
	   std::cerr<<name<<std::string("  does not exist in json file")<<std::endl;
	    throw std::exception();
	   }
	 */
	std::shared_ptr<jsonGenericFile> x(new jsonGenericFile(jsonArgs,usage,name,0,0));
	addRegFile(tag,x);
	return x;

}
std::shared_ptr<genericIrregFile>  jsonGenericIO::getIrregFileTag(const std::string &tag,const std::string &name, const usage_code usage){
	if(!_init) {
		std::cerr<<std::string("Expecting json=file  on the command line")<<std::endl;
		throw std::exception();

	}
	/*
	   if((*jsonArgs)[name].isNull())  {
	   std::cerr<<name<<std::string("  does not exist in json file")<<std::endl;
	    throw std::exception();
	   }
	 */
	std::shared_ptr<jsonGenericFile> x(new jsonGenericFile(jsonArgs,usage,name,0,0));
	addIrregFile(tag,x);
	return x;

}
std::shared_ptr<paramObj>  jsonGenericIO::getParamObj(){
	if(!_init) {
		std::cerr<<std::string("Expecting json=file  on the command line")<<std::endl;
		throw std::exception();

	}
	return _param;
}
void jsonGenericIO::close(){
	for(auto i=_irregFiles.begin(); i !=_irregFiles.end(); ++i) {
		std::shared_ptr <jsonGenericFile> x= std::static_pointer_cast<jsonGenericFile>(i->second);
		(*jsonArgs)[i->first]=x->getArgs();
	}
	for(auto i=_regFiles.begin(); i !=_regFiles.end(); ++i) {
		std::shared_ptr <jsonGenericFile> x= std::static_pointer_cast<jsonGenericFile>(i->second);
		(*jsonArgs)[i->first]=x->getArgs();
	}
	filesClose();

}

/*
   std::static_pointer_cast<DerivedClass>(ptr_to_base)->f(); // OK
   // (constructs a temporary shared_ptr, then calls operator->)

   static_cast<DerivedClass*>(ptr_to_base.get())->f(); // also OK
   // (direct cast, does not construct a temporary shared_ptr)

 */
