
#include "sepIO.h"
using namespace SEP;
sepIO::sepIO( int argc,  char **argv){
 std::shared_ptr<sepParam> x(new sepParam(argc,argv));
  _param=x;
  setValid(true);
}


std::shared_ptr<genericRegFile> sepIO::getRegFileTag(const std::string &tag, const std::string & name,const SEP::usage_code usage){
    std::shared_ptr<sepRegFile> x(new sepRegFile(name,usage));
   addRegFile(tag,x);
    return x;
    
}
std::shared_ptr<genericIrregFile> sepIO::getIrregFileTag(const std::string &tag, const std::string &name, const SEP::usage_code usage){
  
  return 0;
 //   std::shared_ptr<sep3dFile> x(new sep3dFile(name,usage));
  //  addIrregFile(name,x);
   // return x;
}
 
