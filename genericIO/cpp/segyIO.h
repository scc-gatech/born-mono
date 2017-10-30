#ifndef SEGY_H 
#define SEGY_H 1
#include "jsonGenericIO.h"
namespace SEP{
class segyIO:public jsonGenericIO{
  public:
  segyIO( const int argc, char **argv){initJsonPars(argc,argv);}
  virtual std::shared_ptr<genericRegFile> getRegFileTag(const std::string &tag,const std::string &name,const usage_code usage);
    virtual std::shared_ptr<genericIrregFile> getIrregFileTag(const std::string &tag,const std::string &name, const usage_code usage){
       assert(1==2);
    }
  




};

}
#endif
