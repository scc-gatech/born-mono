#include "lloyd.h"
#include<stdio.h>
#include "assert.h"


void lloyd::do_iter(int it){
 iter=it;
  fprintf(stderr,"doing iter %d \n",it);
  this->find_bound();
  fprintf(stderr,"rint 1 \n");
  this->partition_space();
  fprintf(stderr,"rint 2c \n");

  this->compute_center();
  fprintf(stderr,"rint 3 \n");

  this->compute_distort();
    fprintf(stderr,"rint 4 \n");

  this->change_regions(it);
   fprintf(stderr,"rint 5 \n");

}
void lloyd::solve_it(){
   for(int i=0; i < niter; i++) do_iter(i);
}

