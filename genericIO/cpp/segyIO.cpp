#include "segyIO.h"
using namespace SEP;
  

std::shared_ptr<genericRegFile> segyIO::getRegFileTag(const std::string &tag,const std::string &name,const usage_code usage){
  
  std::shared_ptr<jsonGenericFile> x(new jsonGenericFile(getArgs(),usage,name,3600,240));
  return x;
  
}

