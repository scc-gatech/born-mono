#ifndef RSF_IO_H
#define RSF_IO_H 1
#include "genericIO.h"
#include "rsfRegFiles.h"
#include "rsfParams.h"

class rsfIO: public genericIO{
  public:
    rsfIO(int argc, char **argv);
    virtual std::shared_ptr<genericRegFile> getRegFileTag(const std::string &tag,const std::string& name,const usage_code usage);
    virtual std::shared_ptr<genericIrregFile> getIrregFileTag(const std::string &tag,const std::string& name, const usage_code usage){
       assert(1==2);
    }
  


};
#endif