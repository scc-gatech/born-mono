#include "i_op.h"

 
i_op::i_op(my_vector *mod, my_vector *dat){
  if(!mod->check_match(dat)){
    fprintf(stderr,"ERROR domain and range not the same space\n");
  }
  set_domain(mod);
  set_range(dat);
}
 bool i_op::forward(bool add, my_vector *model, my_vector *data){
   if(!check_domain_range(model,data)) return false;
   double me=0;
   if(add)  me=1;
   data->scale_add(me,model,scale);
   return true;
   
 }
bool i_op::adjoint(bool add, my_vector *model, my_vector *data){
   if(!check_domain_range(model,data)) return false;
   double me=0;
   if(add)  me=1;
   model->scale_add(me,data,scale);
   return true;
}
