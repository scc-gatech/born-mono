#include "rsfRegFiles.h"
extern "C" {
#include "rsf.h"

}
#include "basicIO.h"
rsfRegFile::rsfRegFile(const std::string& tag,usage_code usage){
	_tag=tag;
	switch(usage) {
	case usageIn:
		_file=sf_input(_tag.c_str());
	case usageOut:
		_file=sf_output(_tag.c_str());
	case usageInOut:
		error("undefined usageInOut for rsf");
	case usageScr:
		error("undefined usageSrc for rsf");
	}

	_usage=usage;

}

int rsfRegFile::getInt(const std::string& arg) const {
	int x;
	if(!sf_histint(_file,arg.c_str(),&x))
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	return x;
}
int rsfRegFile::getInt(const sd::string &arg, const int def) const {
	int x=def;
	sf_histint(_file,arg.c_str(),&x);
	return x;
}

float rsfRegFile::getFloat(const sd::string &arg, const float def) const {
	float x;
	sf_histfloat(_file,arg.c_str(),&x);
	return x;

}
float rsfRegFile::getFloat(const sd::string &arg) const {
	float x;
	if(!sf_histfloat(_file,arg.c_str(),&x))
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	return x;
}

std::string rsfRegFile::getString(const sd::string &arg) const {
	char *buf;

	buf=sf_histstring(_file,arg.c_str());
	if(buf==NULL) ;
	error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	std::string y= std::string(buf);
	free(buf);
	return y;

}
std::string rsfRegFile::getString(const sd::string &arg, const std::string& def) const {

	char *buf=sf_histstring(_file,arg.c_str());
	if(buf==NULL) return def;

	std::string y=std::string(buf);
	free(buf);
	return y;
}

bool rsfRegFile::getBool(const sd::string &arg, const bool def) const {
	bool x=def;
	sf_histbool(_file,arg.c_str(),&x);
	return x;

}
bool rsfRegFile::getBool(const sd::string &arg) const {
	bool x;
	if(!sf_histbool(_file,arg.c_str(),&x)) {
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	}
	return x;

}

void rsfRegFile::seekTo(const long long iv,const int whence){
	sf_seek(_file,iv,whence)


}

std::vector<int> rsfRegFile::getInts(const sd::string &arg,int n) const {
	int tmp[10000];
	bool ierr=sf_histints(_file,arg.c_str(),tmp,n);
	if(!ierr)
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	std::vector<int> x;
	for(int i=0; i < n; i++) x.push_back(tmp[i]);
	return x;
}
std::vector<int> rsfRegFile::getInts(const sd::string &arg,std::vector<int> &defs) const {
	int tmp[10000];
	for(int i=0; i < defs.size(); i++) {
		tmp[i]=defs[i];
	}
	bool ierr=sf_histints(_file,arg.c_str(),tmp,defs.size());
	std::vector<int> x;
	if(ierr) {
		for(int i=0; i < defs.size(); i++) x.push_back(tmp[i]);
	}
	else{
		for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
	}
	return x;
}

std::vector<float> rsfRegFile::getFloats(const sd::string &arg,int n) const {
	float tmp[10000];
	bool ierr=sf_histfloats(_file,arg.c_str(),tmp,n);
	if(!ierr)
		error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
	std::vector<float> x;
	for(int i=0; i < n; i++) x.push_back(tmp[i]);
	return x;


}
std::vector<float> rsfRegFile::getFloats(const sd::string &arg,std::vector<float>& defs) const {

	float tmp[10000];
	for(int i=0; i < defs.size(); i++) {
		tmp[i]=defs[i];
	}
	bool ierr=sf_histfloats(_file,arg.c_str(),tmp,defs.size());

	std::vector<float> x;
	if(ierr) {
		for(int i=0; i < defs.size(); i++) x.push_back(tmp[i]);
	}
	else{
		for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
	}
	return x;
}


void rsfRegFile::error(const std::string &err) const {
	sf_error(err.c_str());
}

void rsfRegFile::putInt(const std::string &par, const int val){
	sf_putint(_file,par.c_str(),val);

}
void rsfRegFile::putFloat(const std::string &par, const float val){

	sf_putfloat(_file,par.c_str(),val);
}
void rsfRegFile::putString(const std::string &par, const std::string &val){
	sf_putstring(_file,par.c_str(),val.c_str());
}

void rsfRegFile::putBool(const std::string &par, const bool val){
	int i=1;
	if(!val) i=0;
	sf_putint(_file,par.c_str(),i);


}
void rsfRegFile::putInts(const std::string& par, const std::vector<int>& val){

/*
   int *tmp=new int[val.size()];
   for(int i=0; i < val.size(); i++) tmp[i]=val[i];
   sf_putints(_file,par.c_str(),tmp,val.size());
   delete [] tmp;
 */
}
void rsfRegFile::putFloats(const std::string &par, const std::vector<float>& val){

	/*
	   float *tmp=new float[val.size()];
	   for(int i=0; i < val.size(); i++) tmp[i]=val[i];
	   sf_putfloats(_file,par.c_str(),tmp,val.size());
	   delete [] tmp;
	 */

}
void rsfRegFile::readFloatStream( float *array,const long long npts){
	long long maxsize=10000000;
	long long nread=0;
	long long nptsT=npts*4;
	while(nptsT >nread) {
		long long toReadL=std::min(maxsize,nptsT-nread);
		sf_floatread(array,toReadL,_file);

		nread+=(long long)toReadL;
	}

}
void rsfRegFile::readUCharStream( unsigned char *array,const long long npts){
	long long maxsize=10000000;
	long long nread=0;
	while(npts >nread) {
		long long toReadL=std::min(maxsize,npts-nread);
		sf_ucharread(array,toReadL,_file);
		nread+=(long long)toReadL;

	}

}
void rsfRegFile::readComplexStream( float _Complex *array,const long long npts){
	long long maxsize=10000000;
	long long nread=0;
	long long nptsT=npts*8;
	setDataType(dataComplex);

	while(nptsT >nread) {
		long long toReadL=std::min(maxsize,nptsT-nread);
		sf_complexread((sf_complex*)array,toReadL,_file);

		nread+=(long long)toReadL;
	}

}
void rsfRegFile::writeComplexStream( const float _Complex *array,const long long npts){
	long long maxsize=10000000;
	long long nwrite=0;
	long long nptsT=npts*8;
	setDataType(dataComplex);

	while(nptsT >nwrite) {
		long long toWriteL=std::min(maxsize,nptsT-nwrite);
		sf_complexwrite((sf_complex*)array,toWriteL,_file);
		nwrite+=(long long)toWriteL;
	}

}

void rsfRegFile::writeFloatStream( const float *array,const long long npts){
	long long maxsize=10000000;
	long long nwrite=0;
	long long nptsT=npts*4;
	setDataType(dataFloat);

	while(nptsT >nwrite) {
		long long toWriteL=std::min(maxsize,nptsT-nwrite);
		sf_floatwrite((float*)array,toWriteL,_file);
		nwrite+=(long long)toWriteL;
	}

}
void rsfRegFile::readFloatWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int>& jw,  float *array){
	std::shared_ptr< hypercube>hyper=getHyper();
	std::vector<int> ng=hyper->returnNs();
	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<rsfBasic> i(new rsfBasic(_file));
		myio=i;
		myio->setFileParams(_tag,_usage,0,0,4,false,getHyper());
	}
	myio->readWindow(nw,fw,jw,array);


}
void rsfRegFile::readComplexWindow(const std::vector<int>& nw, const std::vector<int>& fw,
	const std::vector<int>& jw,  float _Complex *array){
	std::shared_ptr< hypercube>hyper=getHyper();
	std::vector<int> ng=hyper->returnNs();
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
		std::shared_ptr<rsfBasic> i(new rsfBasic(_file));
		myio=i;
		myio->setFileParams(_tag,_usage,0,0,8,false,getHyper());
	}
	myio->readWindow(nw,fw,jw,array);


}
void rsfRegFile::writeComplexWindow(const std::vector<int> &nw, const std::vector<int> &fw,
	const std::vector<int>& jw, float _Complex *array){
	setDataType(dataComplex);

	std::shared_ptr<hypercube>hyper=getHyper();
	std::vector<int> ng=hyper->returnNs();
	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<rsfBasic> i(new rsfBasic(_file));
		myio=i;
		myio->setFileParams(_tag,_usage,0,0,8,false,getHyper());
	}
	myio->writeWindow(nw,fw,jw,array);

}
void rsfRegFile::readUCharWindow(const std::vector<int>& nw, const std::vector<int> &fw,
	const std::vector<int>& jw,  unsigned char *array){
	std::shared_ptr<hypercube>hyper=getHyper();
	std::vector<int> ng=hyper->returnNs();
	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<rsfBasic> i(new rsfBasic(_file));
		myio=i;
		myio->setFileParams(_tag,_usage,0,0,1,false,getHyper());
	}
	myio->readWindow(nw,fw,jw,array);

}
void rsfRegFile::writeFloatWindow(const std::vector<int> &nw, const std::vector<int>& fw,
	const std::vector<int>& jw, float *array){
	setDataType(dataFloat);

	std::shared_ptr<hypercube>hyper=getHyper();
	std::vector<int> ng=hyper->returnNs();
	if(ng.size() >nw.size()) {
		for(int i=nw.size(); i < ng.size(); i++) {
			if(ng[i]>1) error("number of dimension does not equal data size");
		}
	}
	if(nw.size()< ng.size() || fw.size() < ng.size() || jw.size()< jw.size()) {
		error("number of dimensions does not equal data size");
	}
	if(!myio) {
		std::shared_ptr<rsfBasic> i(new rsfBasic(_file));
		myio=i;
		myio->setFileParams(_tag,_usage,0,0,4,false,getHyper());
	}
	myio->writeWindow(nw,fw,jw,array);

}
void rsfRegFile::readDescription(){
	int ndim;
	bool breakIt=false;
	int iax=9;
	while(iax >=1 && !breakIt) {
		std::string tmp;
		int n=1;
		tmp="n"+std::to_string(iax);
		sf_histint(_file,tmp.c_str(),&n);
		if(n>1) breakIt=true;
		iax--;
	}
	sf_datatype typ=sf_gettype(_file);
	switch(typ) {
	case SF_FLOAT:
		setDataType(dataFloat);
		break;
	case SF_UCHAR:
	case SF_CHAR:
		setDataType(dataByte);
		break;
	case SF_INT:
		setDataType(dataInt);
		break;
	default:
		error("Currently handle this datatype");
	}
	if(iax==0) error("couldn't find any axes");
	ndim=iax;
	std::vector<axis> axes;
	for(int i=1; i <= ndim; i++) {
		int n=1;
		float o=0.,d=1.;
		std::string tmp;
		tmp="n"+std::to_string(i); sf_histint(_file,tmp.c_str(),&n);
		tmp="o"+std::to_string(i);
		sf_histfloat(_file,tmp.c_str(),&o);
		tmp="d"+std::to_string(i); sf_histfloat(_file,tmp.c_str(),&d);
		tmp="label"+std::to_string(i);
		char *mm=sf_histstring(_file,tmp.c_str());
		std::string label;
		if(mm==NULL) label=std::string("none");
		else{
			label=std::string(mm);
			free(mm);
		}
		axes.push_back(axis(n,o,d,label));

	}
	std::shared_ptr< hypercube>hyper(new hypercube(axes));
	setHyper(hyper);
}
void rsfRegFile::writeDescription(){
	std::shared_ptr<hypercube>hyper=getHyper();
	std::vector<axis> axes=hyper->returnAxes(hyper->getNdim());
	for(int i=1; i <= axes.size(); i++) {
		int n=axes[i].n; float o=axes[i].o; float d=axes[i].d;
		char label[1024];  std::copy(axes[i].label.begin(), axes[i].label.end(), label);
		std::string tmp;
		tmp="n"+std::to_string(i); sf_putint(_file,tmp.c_str(),n);
		tmp="o"+std::to_string(i); sf_putfloat(_file,tmp.c_str(),o);
		tmp="d"+std::to_string(i); sf_putfloat(_file,tmp.c_str(),d);
		tmp="label"+std::to_string(i); sf_putstring(_file,tmp.c_str(),label);
	}
	dataType dat=getDataType();
	switch(dat) {
	case dataFloat:
		sf_setformat(_file,"native_float");
		break;
	case dataByte:
		sf_setformat(_file,"native_char");
		break;
	case dataInt:
		sf_setformat(_file,"native_int");
		break;
	default:
		error(std::string("format undefined"));
	}


}
