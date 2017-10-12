#include "lloyd.h"
#include<stdio.h>
#include "assert.h"

using namespace SEP;
void lloyd::do_iter(int it){
 iter=it;
  this->find_bound();
  this->partition_space();

  this->compute_center();

  this->compute_distort();

  this->change_regions(it);

}
void lloyd::solve_it(){
   for(int i=0; i < niter; i++) do_iter(i);
}

