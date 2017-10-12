#ifndef BASIC_IOH
#define BASIC_IOH 1
#include <stdio.h>
#include <memory>
#include "genericFile.h"
#include <assert.h>
/*this is very rudimentary class for file io */
namespace SEP {
class basicIO {
public:
basicIO(){
	_swapData=false;
}
void readWindow(const std::vector<int> &nw, const std::vector<int> &fw,const std::vector<int>& jw,void *data, void *head=0);
void writeWindow(const std::vector<int>& nw, const std::vector<int>& fw,const std::vector<int>& jw,const void *data, void *head=0);
virtual void readStream(const long long sz,  void *data){
	assert(1==2);
}
virtual void writeStream(const long long sz, const void *dat){
	assert(1==2);
}
virtual void readTraceStream(const long long sz, void *dat, void *head=0);
virtual void writeTraceStream(const long long sz, const void *dat, const void *head=0);
virtual void writeReelHead(const void *reelH);
void readBlocks(const int naxes,const std::vector<int>& nwo,const std::vector<int>& fwo, const std::vector<int>& jwo,
	const std::vector<int> &nwi, const std::vector<int>& fwi, const std::vector<int> &jwi,
	const long long buf, void *data,void *head);
void writeBlocks(const int naxes,const std::vector<int> &nwo,const std::vector<int> &fwo,const std::vector<int> &jwo,
	const std::vector<int>& nwi, const std::vector<int> &fwi,const std::vector<int> &jwi,
	const long long buf,const void *data,const void *head);
virtual void partsToBlock(const std::vector<int> &nw, const std::vector<int>& fw, const std::vector<int> &jw,  void *in, const void *out,const void *head);
virtual void blockToParts(const std::vector<int> &nw,const std::vector<int> &fw, const std::vector<int> &jw, const void *in, void *out, void *head);
virtual long long getCurrentPos() const {
	assert(1==2);
}
virtual long long getSize(){
	assert(1==2);
}
void swap_float_bytes(const int n, float *buf);
virtual inline void seekTo(const long long off, const int whence){
	assert(1==2);
}
virtual inline void seekToPos(const long long pos){
	assert(1==2);

}
void setFileParams(const std::string nm, const usage_code usage,const int reelH, const int traceH,
	const int esize, const bool swapData,std::shared_ptr<hypercube> hyper);
protected:
usage_code _usage;
FILE *_myf;
bool _swapData=false;
long long _reelH,_traceH;
long long _esize;
std::string _nm;
std::shared_ptr<hypercube> _hyper;
};
class myFileIO : public basicIO {
public:
myFileIO(const std::string& nm,const usage_code usage,const int reelH, const int traceH,const int esize,const bool swapData,std::shared_ptr<hypercube> hyper);
virtual inline void seekToPos(const long long pos){
	long long ft=getCurrentPos();
	long long bg=1024*1024*1024;
	long long diff=pos-ft;
	while(diff!=0) {
		long long dst;
		if(diff>0) dst=std::min(bg,diff);
		else dst=-std::min(-diff,bg);
		fseek(_myf,dst,SEEK_CUR);
		diff-=dst;
	}
}
virtual inline void seekTo(const long long pos, const int whence){

	fseek(_myf,pos,whence);

}
virtual long long getCurrentPos() const {
	return ftell(_myf);
}
virtual long long getSize(){
	long long cp=getCurrentPos();
	fseek(_myf,0L,SEEK_END);
	long long end=getCurrentPos();
	fseek(_myf,cp,0);
	return end;
}
virtual void readStream(const long long sz,  void *data);
virtual void writeStream(const long long sz,const void *data);
virtual void close(){
	if(myf!=0) fclose(myf);
}
~myFileIO(){
	close();
}
FILE *myf=0;
};

}

#endif
