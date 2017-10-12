#include "jsonGenericFile.h"
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <exception>
#include <cstdlib>
using namespace SEP;
jsonGenericFile::jsonGenericFile(std::shared_ptr<Json::Value> arg,const usage_code usage,const std::string &tag, const int reelH,const int traceH){
	_usage=usage;
	setupJson(arg,tag);
	_reelH=reelH;
	_traceH=traceH;

	if(!_newFile) {
		readDescription();
		std::shared_ptr<myFileIO> x(new myFileIO(getDataFileName(),usage,reelH,traceH,
				jsonArgs.get("esize",4).asInt(),jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=x;
	}
	else{
		std::string datapath=std::string("./");
		if(const char* env_p = std::getenv("DATAPATH")) datapath=std::string(env_p);
		_dataFile=datapath+std::string("/")+getJSONFileName()+std::string(".dat");
		jsonArgs["filename"]=_dataFile;
	}

}
void jsonGenericFile::setupJson(std::shared_ptr<Json::Value> arg,const std::string &tag){
	_tag=tag;

	if((*arg)[tag].isNull()) {
		_jsonFile=_tag;
	}
	else{
		_jsonFile=(*arg)[tag].asString();
	}
	_newFile=true;
	if(_usage==usageIn) _newFile=false;
	else if(_usage==usageInOut) {
		std::ifstream f(getJSONFileName());
		_newFile=!f.good();
		f.close();
	}

	if(_usage==usageIn || !_newFile) {
		std::ifstream inps;
		inps.open(getJSONFileName(),std::ifstream::in);
		if(!inps) {
			std::cerr<<std::string("Trouble opening1 "+getJSONFileName())<<std::endl;
			throw std::exception();
		}
		try{
			inps>>jsonArgs;
		}
		catch(int x) {
			std::cerr<<std::string("Trouble parsing JSON file "+getJSONFileName())<<std::endl;
			throw std::exception();
		}
		_dataFile=jsonArgs[std::string("filename")].asString();
	}

}
std::string jsonGenericFile::getJSONFileName() const {

	return _jsonFile;
}
std::string jsonGenericFile::getDataFileName() const {

	return _dataFile;
}
int jsonGenericFile::getInt(const std::string& arg) const {
	int x;
	if(jsonArgs[arg].isNull())
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	x=jsonArgs.get(arg,1).asInt();
	return x;
}
int jsonGenericFile::getInt(const std::string &arg, const int def) const {

	int x=jsonArgs.get(arg,def).asInt();
	return x;
}
float jsonGenericFile::getFloat(const std::string &arg, const float def) const {
	float x;
	x= jsonArgs.get(arg,def).asFloat();
	return x;

}
float jsonGenericFile::getFloat(const std::string &arg) const {
	float x;
	if(jsonArgs[arg].isNull())
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	x=jsonArgs.get(arg,1.).asFloat();
	return x;
}
std::string jsonGenericFile::getString(const std::string &arg) const {
	if(jsonArgs[arg].isNull())
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));

	return jsonArgs.get(arg,"").asString();


}
std::string jsonGenericFile::getString(const std::string &arg, const std::string &def) const {

	return jsonArgs.get(arg,def).asString();

}
bool jsonGenericFile::getBool(const std::string &arg,  bool def) const {
	return jsonArgs.get(arg,def).asBool();
}
bool jsonGenericFile::getBool(const std::string& arg) const {
	if(jsonArgs[arg].isNull())
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	jsonArgs.get(arg,false).asBool();
}
std::vector<int> jsonGenericFile::getInts(const std::string &arg,int nvals) const {
	if(jsonArgs[arg].isNull())
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	const Json::Value vals = jsonArgs[arg];

	std::vector<int> x;
	for(int i=0; i < nvals; i++) x.push_back(vals[i].asInt());
	return x;
}
std::vector<int> jsonGenericFile::getInts(const std::string &arg,std::vector<int> &defs) const {
	std::vector<int> x;
	if(jsonArgs[arg].isNull())  {
		for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
	}
	else{
		const Json::Value vals = jsonArgs[arg];
		for(int i=0; i < defs.size(); i++) x.push_back(vals[i].asInt());
	}
	return x;
}
std::vector<float> jsonGenericFile::getFloats(const std::string &arg,int nvals) const {
	if(jsonArgs[arg].isNull())
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	const Json::Value vals = jsonArgs[arg];

	std::vector<float> x;
	for(int i=0; i < nvals; i++) x.push_back(vals[i].asFloat());
	return x;

}
std::vector<float> jsonGenericFile::getFloats(const std::string &arg,std::vector<float>& defs) const {
	std::vector<float> x;
	if(jsonArgs[arg].isNull())   {
		for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
	}
	else{
		const Json::Value vals = jsonArgs[arg];
		for(int i=0; i < defs.size(); i++) x.push_back(vals[i].asFloat());
	}
	return x;

}

void jsonGenericFile::putInt(const std::string &par, const int val){
	jsonArgs[par]=val;
}
void jsonGenericFile::putFloat(const std::string& par, const float val){
	jsonArgs[par]=val;

}
void jsonGenericFile::putString(const std::string& par, const std::string &val){
	jsonArgs[par]=val;

}
void jsonGenericFile::putBool(const std::string &par, const bool val){
	jsonArgs[par]=val;


}
void jsonGenericFile::putInts(const std::string& par, const std::vector<int>& val){
	Json::Value vals;
	for(int i =0; i < val.size(); i++) vals.append(val[i]);
	jsonArgs[par]=vals;

}
void jsonGenericFile::putFloats(const std::string &par, const std::vector<float>& val){
	Json::Value vals;
	for(int i =0; i < val.size(); i++) vals.append(val[i]);
	jsonArgs[par]=vals;
}
void jsonGenericFile::readDescription(){
	int ndim;
	bool breakIt=false;
	int iax=9;
	while(iax >=1 && !breakIt) {
		std::string tmp;
		int n=getInt("n"+std::to_string(iax),1);
		float o=getFloat("o"+std::to_string(iax),0.);

		if(n>1 || fabsf(o) >1e-4) breakIt=true;
		else iax--;
	}
	if(iax==0) error("couldn't find any axes");
	ndim=iax;

	std::vector<axis> axes;
	for(int i=1; i <= ndim; i++) {
		int n=getInt(std::string("n")+std::to_string(i),1);
		float o=getFloat(std::string("o")+std::to_string(i),0.);
		float d=getFloat(std::string("d")+std::to_string(i),1.);
		std::string label=getString(std::string("label")+std::to_string(i),"");
		axes.push_back(axis(n,o,d,label));

	}
	std::string dtyp=getString(std::string("dataType"),std::string("FLOAT"));
	if(dtyp==std::string("FLOAT")) setDataType(dataFloat);
	else if(dtyp==std::string("COMPLEX")) setDataType(dataComplex);
	else if(dtyp==std::string("INTEGER")) setDataType(dataInt);
	else if(dtyp==std::string("BYTE")) setDataType(dataByte);

	std::shared_ptr<hypercube> hyper(new hypercube(axes));
	setHyper(hyper);
}
void jsonGenericFile::writeDescription(){
	std::shared_ptr<hypercube> hyper=getHyper();
	std::vector<axis> axes=hyper->returnAxes(hyper->getNdim());
	for(int i=1; i <= axes.size(); i++) {
		putInt(std::string("n")+std::to_string(i),axes[i-1].n);
		putFloat(std::string("o")+std::to_string(i),axes[i-1].o);
		putFloat(std::string("d")+std::to_string(i),axes[i-1].d);
		putString(std::string("label")+std::to_string(i),axes[i-1].label);
	}

}
void jsonGenericFile::close(){
	myio->close();
	if(_usage==usageOut || _usage==usageInOut) {
		std::ofstream outps;
		outps.open(getJSONFileName(),std::ofstream::out);
		if(!outps) {
			std::cerr<<std::string("Trouble opening for write")+getJSONFileName()<<std::endl;
			throw std::exception();
		}
		try{
			outps<<jsonArgs;
		}
		catch(int x) {
			std::cerr<<std::string("Trouble writing JSON file ")+getJSONFileName()<<std::endl;
			throw std::exception();
		}


	}
}
void jsonGenericFile::readFloatStream( float *array,const long long npts){
	long long maxsize=10000000;
	long long nread=0;
	long long nptsT=npts*4;
	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,4,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}

	myio->readTraceStream(npts,array);

}
void jsonGenericFile::seekTo(const long long iv,const int whence){
	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,4,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}
	myio->seekTo(iv,whence);

}

void jsonGenericFile::readUCharStream( unsigned char *array,const long long npts){
	long long maxsize=10000000;
	long long nread=0;
	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,1,false,getHyper()));
		myio=iox;
	}
	myio->readTraceStream(npts,array);


}

void jsonGenericFile::writeFloatStream( const float *array,const long long npts){
	long long maxsize=10000000;
	long long nwrite=0;
	long long nptsT=npts*4;
	setDataType(dataFloat);

	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,4,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}
	myio->writeTraceStream(npts,array);
}
void jsonGenericFile::readFloatWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int> &jw,  float *array){
	std::shared_ptr<hypercube> hyper=getHyper();
	std::vector<int> ng=hyper->getNs();
	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,4,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}
	myio->readWindow(nw,fw,jw,array);


}
long long jsonGenericFile::getDataSize(){

	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,1,false,getHyper()));
		myio=iox;
	}
	return myio->getSize();
}
void jsonGenericFile::readUCharWindow(const std::vector<int>& nw, const std::vector<int> &fw,
	const std::vector<int>& jw,  unsigned char *array){
	std::shared_ptr<hypercube> hyper=getHyper();
	std::vector<int> ng=hyper->getNs();
	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,1,false,getHyper()));
		myio=iox;
	}
	myio->readWindow(nw,fw,jw,array);

}
void jsonGenericFile::writeFloatWindow(const std::vector<int>& nw, const std::vector<int> &fw,
	const std::vector<int> &jw, const float *array){
	setDataType(dataFloat);

	std::shared_ptr<hypercube> hyper=getHyper();
	std::vector<int> ng=hyper->getNs();
	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,4,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}
	myio->writeWindow(nw,fw,jw,array);

}



void jsonGenericFile::readComplexStream( float _Complex *array,const long long npts){
	long long maxsize=10000000;
	long long nread=0;
	long long nptsT=npts*4;
	setDataType(dataComplex);

	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,8,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}
	myio->readTraceStream(npts,array);


}
void jsonGenericFile::writeComplexStream( const float _Complex *array,const long long npts){
	long long maxsize=10000000;
	long long nwrite=0;
	long long nptsT=npts*4;
	setDataType(dataComplex);

	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,8,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}
	myio->writeTraceStream(npts,array);
}
void jsonGenericFile::readComplexWindow(const std::vector<int> &nw, const std::vector<int> &fw,
	const std::vector<int>& jw,  float _Complex  *array){
	std::shared_ptr<hypercube> hyper=getHyper();
	std::vector<int> ng=hyper->getNs();
	setDataType(dataComplex);

	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,8,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}
	myio->readWindow(nw,fw,jw,array);


}
void jsonGenericFile::writeComplexWindow(const std::vector<int>& nw, const std::vector<int> &fw,
	const std::vector<int>& jw, const float _Complex *array){
	setDataType(dataComplex);

	std::shared_ptr<hypercube> hyper=getHyper();
	std::vector<int> ng=hyper->getNs();
	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<myFileIO> iox(new myFileIO(getDataFileName(),_usage,_reelH,_traceH,8,jsonArgs.get("swapData",false).asBool(),getHyper()));
		myio=iox;
	}
	myio->writeWindow(nw,fw,jw,array);

}

void jsonGenericFile::message(const std::string &errm) const {
	std::cerr<<errm<<std::endl;
}
void jsonGenericFile::error(const std::string &errm) const {
	std::cerr<<errm<<std::endl;
	throw std::exception();
}
