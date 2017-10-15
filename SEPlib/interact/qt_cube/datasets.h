#ifndef DATASETS_H 
#define DATASETS_H 1
#include "dataset.h"

class datasets{
  public:
   datasets(param_func *p);
   datasets(){}
   ~datasets(){ clean_dats();}
   void clean_dats();
   
   void add_dat(dataset *dat);
   dataset *return_dat(int i);
   dataset *return_dat(QString nm);
   int return_ndat(){ return (int)dats.size();}


  private:
    std::vector<dataset*> dats;
    param_func *par;
  
};
#endif
