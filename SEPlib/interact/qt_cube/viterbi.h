
#ifndef VITERBI_H
#define VITERBI_H 1
#include "dynamic.h"
#include<my_score.h>

class viterbi: public dynamic{

  public:
    viterbi();
    viterbi(my_score *s,float_2d *buf, int ns,int flac);
    virtual void calc_score();
    virtual std::vector<path> return_path();
    
    int nsearch,f;
    virtual  ~viterbi(){};
   my_score *sc;
    
};

#endif
