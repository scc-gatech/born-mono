#include "fileIO.h"
using namespace SEP;
fileIO::fileIO(std::string tag, std::shared_ptr<ioModes>modes,std::string fileType){

  _io=modes->getIO(fileType);
  _file=_io->getRegFile(tag,usageIn);
  _dataType=_file->getDataType();
  _par=_io->getParamObj();
  hyper=_file->getHyper();
    std::shared_ptr<util> x(new util(_par));
  set_basics(x,hyper);
    switch(_dataType){
    case dataByte:
      non_byte=false;
      bclip=_file->getFloat("minval",0);
      eclip=_file->getFloat("maxval",255);
      break;
    case dataFloat:
      non_byte=true;
      break;
    default:
     _par->error("only can handle float and byte files for now");
  }




}

void fileIO::read_block_float(std::vector<int>&nw, std::vector<int>&fw, float *buf){
  std::vector<int> jw(8,1); 
  long long n123=1;
 
   if(_dataType==dataFloat){
  
      _file->readFloatWindow(nw,fw,jw,buf);
    }
    else if(_dataType==dataByte){
       for(auto i=nw.begin(); i!=nw.end(); ++i) n123*=(long long) *i;
       unsigned char *buf2=new unsigned char [n123];
       _file->readUCharWindow(nw,fw,jw,buf2);
       par->convert_to_float(buf,0,buf2,0,n123,bclip,eclip);
      delete [] buf2;
   }
   else
      _par->error("Only can handle float and byte");
  

}

void fileIO::read_block_byte(std::vector<int>&nw, std::vector<int>&fw, unsigned char *buf){
  std::vector<int> jw(8,1);  
  long long n123=1;
   if(_dataType==dataByte){
      _file->readUCharWindow(nw,fw,jw,buf);
      
    }
     else if(_dataType==dataFloat){
       for(auto i=nw.begin(); i!=nw.end(); ++i) n123*=(long long) *i;
       float *buf2=new float[n123];
       _file->readFloatWindow(nw,fw,jw,buf2);
       par->convert_to_byte(buf2,0,buf,0,n123,bclip,eclip);
      delete [] buf2;
   }
   else
      _par->error("Only can handle float and byte");
  
    
}



