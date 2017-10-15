#ifndef SOLVER_PG
#define SOLVER_PG 1
#include "my_vector.h"
#include "my_operator.h"

class solver_pg{
  public:

 solver_pg(){};
 solver_pg(my_operator *o, my_vector *minv, my_vector *maxv, double mintol);


  void kk_proj(my_vector *x,my_vector *on_b=0);
  void proja(my_vector *vec, my_vector *change);
  void proja(my_vector *vec, my_vector *change);
  my_vector *min_vec, my_vector *max_vec;
  double mint;
  my_operator *op;
  double min_range;

};


