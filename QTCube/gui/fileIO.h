#ifndef FILE_IO_H
#define FILE_IO_H 1
#include "io_func.h"
#include "ioModes.h"
namespace SEP{
class fileIO: public io_func{


  public:
     fileIO(std::string fileName,std::shared_ptr< SEP::ioModes>io,std::string fileType);


    virtual void read_block_float(std::vector<int>&nw, std::vector<int>&fw, float *buf);
    virtual void read_block_byte(std::vector<int>&nw, std::vector<int>&fw, unsigned char *buf);
  private:
    std::shared_ptr<genericRegFile> _file;
    std::shared_ptr<genericIO> _io;
    std::shared_ptr<paramObj> _par;
    std::vector<int> _ns;    
    SEP::dataType _dataType;
    std::shared_ptr<util> _util;
};
}

#endif
