
#ifndef MOVE_SEMBLANCE_H
#define MOVE_SEMBLANCE_H 1
#include "dataset.h"
#include<string>
#include "io_func.h"
#include "pick_draw.h"
namespace SEP{
class  move_semblance: public io_func{

  public:
   move_semblance(){};
   void set_moveout_basics(std::shared_ptr<SEP::hypercube>grid, std::shared_ptr<SEP::dataset>dat,int it, int ioff, int imes,
     std::shared_ptr<paramObj>pars,std::shared_ptr<SEP::pick_draw>pck,QString col1, QString col2);

   void moveSemblanceAfterInitialization();

   virtual void read_block_float(std::vector<int>&nw, std::vector<int>&fw, float *buf);
   virtual void read_block_byte(std::vector<int>&nw, std::vector<int>&fw, unsigned char *buf);
   virtual void semblance_it(std::vector<int>&f,float *nmo){if(f[0]==0 && nmo==0){;}}
     std::shared_ptr<dataset>data;
     int i_off,i_mes,i_t;
     float os[8],ds[8];
     std::shared_ptr<SEP::util>par;
     std::shared_ptr<SEP::orient_cube>dumb;
     float *num,*den,*nnz;
     std::shared_ptr<SEP::hypercube>grid;
     std::shared_ptr<SEP::pick_draw>pks;
     QString col1,col2;


 };
 
 }
#endif
