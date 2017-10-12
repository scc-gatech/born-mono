#ifndef IOMODES
#define IOMODES 1
#include "genericIO.h"
#include "jsonGenericIO.h"
#include<mutex>
namespace SEP{
class ioModes{
  public:
    ioModes(){;}
    ioModes(const int argc, char **argv){ setup(argc,argv);}
    void setup(const int argc, char**argv);
   std::shared_ptr<genericIO> getDefaultIO();
   std::shared_ptr<genericIO> getIO(const std::string &def);
   std::shared_ptr<genericRegFile> getRegFileTag(const std::string &tag,const std::string &def,const std::string &name,const usage_code usage);
   std::shared_ptr<genericRegFile> getGenericRegFile(const std::string &name,const usage_code usage);
   std::string getDefaultType(){ return _defaultType;}
  private:
    std::shared_ptr<genericIO> _defaultIO;
    std::map<std::string,std::shared_ptr<genericIO>> _ios;

    std::shared_ptr<paramObj> _par;
    std::string _defaultType;

};

class ioModesFortran{
  private:

 
    ioModesFortran(){;}
    ioModesFortran(const ioModesFortran& rs);
    ioModesFortran& operator = (const ioModesFortran& rs);
        static std::shared_ptr<ioModesFortran> instance;

    std::shared_ptr<ioModes> _io;
  public:
      void setup( const int argc,char **argv);
    ~ioModesFortran(){;}
    std::shared_ptr<ioModes> getIOModes(){
       return _io;
    }

  static std::shared_ptr<ioModesFortran>& getInstance()
  {
 

 
      if (!instance) {
      instance.reset(new ioModesFortran());
      instance->_io.reset(new ioModes());
    }
 
  return instance;
  }
 
};
}

#endif