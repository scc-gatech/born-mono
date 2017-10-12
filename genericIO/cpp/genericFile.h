#ifndef GENERIC_FILE
#define GENERIC_FILE 1
#include <vector>
#include <assert.h>
#include "paramObj.h"
#include "hypercube.h"
#include <memory>
#include <complex.h>


namespace SEP {
enum usage_code {
	usageIn,
	usageOut,
	usageInOut,
	usageScr
};
enum dataType {
	dataFloat,
	dataByte,
	dataShort,
	dataInt,
	dataComplex,
	dataUndefined
};


class genericRegFile : public paramObj {
public:
genericRegFile(){
	_type=dataUndefined;
}

virtual void putInt(const std::string &par, const int val);
virtual void putFloat(const std::string &par, const float val){
	if(par=="" && val==0) {;}
}
virtual void putString(const std::string &par, const std::string val){
	if(par=="" && val=="") {;}
}
virtual void putBool(const std::string &par, const bool val){
	if(par=="" && val==0) {;}
}
virtual void putInts(const std::string &par, const std::vector<int> val){
	if(par=="" && val[0]==0) {;}
}
virtual void putFloats(const std::string &par, const std::vector<float> val) {
	if(par=="" && val[0]==0) {;}
}
virtual void readUCharStream(unsigned char *array,const long long npts){
	if(npts==0 && array==0) {;}
}
virtual void readFloatStream(float *array,const long long npts){
	if(npts==0 && array==0) {;}
}
virtual void writeFloatStream( const float *array,const long long npts){
	fprintf(stderr,"in write float stream default \n");

	if(npts==0 && array==0) {;}
}
virtual void readUCharWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int>& jw, unsigned char *array){
	if(nw[0]==0 && fw[0]==0 && jw[0]==0 && array==0) {;}
}
virtual void seekTo(const long long iv,const int whence){
	if(whence==iv) {;}
}
virtual void readComplexStream(float _Complex *array,const long long npts){
	if(npts==0 && array==0) {;}
}
virtual void writeComplexStream( const float _Complex *array,const long long npts){
	if(npts==0 && array==0) {;}
}
virtual void readComplexWindow(const std::vector<int> &nw, const std::vector<int> &fw,
	const std::vector<int> jw,  float _Complex *array){
	if(nw[0]==0 && fw[0]==0 && jw[0]==0 && array==0) {;}
}
virtual void writeComplexWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int>& jw, const float _Complex *array){
	if(nw[0]==0 && fw[0]==0 && jw[0]==0 && array==0) {;}
}
virtual long long getDataSize(){
	assert(1==2);
}
virtual void readFloatWindow(const std::vector<int> &nw, const std::vector<int>& fw,
	const std::vector<int> &jw,  float *array){
	if(nw[0]==0 && fw[0]==0 && jw[0]==0 && array==0) {;}
}
virtual void writeFloatWindow(const std::vector<int> &nw, const std::vector<int> &fw,
	const std::vector<int>& jw, const float *array){
	if(nw[0]==0 && fw[0]==0 && jw[0]==0 && array==0) {;}
}
virtual void readDescription(){
	;
}
virtual void writeDescription(){
	;
}
virtual void close(){
	;
}
void setHyper(std::shared_ptr<SEP::hypercube> hyp){
	_hyper=hyp;
}
dataType getDataType(){
	return _type;
}
int getDataEsize();
void setDataType(dataType typ){
	_type=typ;
}
std::string getDataTypeString();
const std::shared_ptr<SEP::hypercube> getHyper(){
	if(!_hyper) error(std::string("Hypercube has not been setup"));
	return _hyper;
}
protected:
std::shared_ptr<SEP::hypercube> _hyper;
dataType _type;
};

union headerType {
	int i;
	short s;
	long long l;
	double d;
	float f;
};
class genericIrregFile : public genericRegFile {
public:
genericIrregFile(){
}
virtual int getHeaderIndex(std::string keyname){
	if(keyname==std::string("")) {;}
	return 0;
}
virtual std::vector<headerType> getTraceHeader(long long index){
	if(index==0) {;}
	std::vector<headerType> x; return x;
}
virtual std::vector<std::string> getHeaderTypes(){
	std::vector<std::string> a; return a;
}


};

}

#endif
