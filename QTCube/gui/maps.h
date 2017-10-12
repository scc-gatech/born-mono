#ifndef MAPS_H
#define MAPS_H 1 
#include <vector>
#include "hypercube.h"
#include <genericIO.h>
#include <QString>
namespace SEP{
class map_1{
  public:
    map_1(){;}
    map_1(std::shared_ptr<SEP::genericIO> io,QString nm, const std::shared_ptr<SEP::hypercube>h, float oversamp);
    std::vector<int> return_index(){return ind;}
    QString return_name(){ return name;}
    float return_oversamp(){ return oversamp;}
  private:
    std::vector<int> ind;
    QString name;
    std::shared_ptr<SEP::genericIO> _io;
    float oversamp;

};

class maps{
  public:
  maps(std::shared_ptr<SEP::genericIO> io){_io=io;}
  void add_map(std::shared_ptr<map_1> x){ my_maps.push_back(x);}
  std::shared_ptr<map_1> return_map(int inum);
  std::shared_ptr<map_1>return_map(QString nm);
  std::vector<QString> return_names();
  int size(){ return (int)my_maps.size();}
  int return_num(QString nm);
  
  private:

  std::vector<std::shared_ptr<map_1>> my_maps;
  std::shared_ptr<SEP::genericIO> _io;

};

}
#endif
