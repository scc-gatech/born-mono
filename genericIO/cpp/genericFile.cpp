#include "genericFile.h"
using namespace SEP;
 void genericRegFile::putInt(const std::string &par, const int val) {
   if(par=="" && val==0);
   }
   
std::string genericRegFile::getDataTypeString(){
  dataType x=getDataType();
  
  switch(x){
    case dataFloat:
      return std::string("dataFloat");
      break;
    case dataByte:
      return std::string("dataByte");
      break;
    case dataShort:
      return std::string("dataShort");
      break;
    case dataInt:
      return std::string("dataInt");
      break;
    case dataComplex:
      return std::string("dataComplex");
      break;
    default:
      return std::string("dataUndefined");
  }
}
int genericRegFile::getDataEsize(){
  dataType x=getDataType();
  
  switch(x){
    case dataFloat:
      return 4;
      break;
    case dataByte:
      return 1;
      break;
    case dataShort:
      return 2;
      break;
    case dataInt:
      return 4;
      break;
    case dataComplex:
      return 8;
      break;
    default:
      return -1;
  }
}    