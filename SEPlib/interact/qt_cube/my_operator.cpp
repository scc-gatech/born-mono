#include<my_operator.h>
#include<math.h>
#include<stdio.h>

 void my_operator::set_domain(my_vector *dom){
     scale=1;
     domain=dom->clone_space();
 }
 void my_operator::set_range(my_vector *ran){
      scale=1.;
      range=ran->clone_space();
 }
 bool my_operator::dot_test(bool verb){
   my_vector *mod1,*mod2,*dat1,*dat2;
   mod1=domain->clone_vec();
   mod2=mod1->clone_vec();
   dat1=range->clone_vec();
   dat2=dat1->clone_vec();
   mod1->random();
   dat2->random();
   forward(false,mod1,dat1);
   double dot1=dat1->dot(dat2);
   adjoint(false,mod2,dat2);
   double dot2=mod2->dot(mod1);
  /*
   dat1->info("dat1");
   mod1->info("mod1");
   dat2->info("dat2");
   mod2->info("mod2");
   */
      if(verb) fprintf(stderr,"Dot(add=false) %g %g \n",dot1,dot2);
    
   forward(true,mod1,dat1);
   adjoint(true,mod2,dat2);
   double dot3=mod1->dot(mod2);
   double dot4=dat1->dot(dat2);
   if(verb) fprintf(stderr,"Dot(add=true) %g %g \n",dot3,dot4);
   
   delete dat1; delete dat2; delete mod1; delete mod2;
   
   if(fabs(dot1) < 1e-12 || fabs(dot3) < 1e-12) {
     fprintf(stderr,"Dot product suspiciously small\n");
     
     return false;
   }
   if(fabs( (dot1-dot2)/dot1) >.0001) {
     fprintf(stderr,"Failed add=false dot product %g %g\n",dot1,dot2);
     return false;
  }
  
  if(fabs( (dot3-dot4)/dot3) >.0001) {
     fprintf(stderr,"Failed add=true dot product  %g (%g %g) \n",fabs( (dot3-dot4)/dot3),dot3,dot4);
     return false;
  }
  return true;
 
 }
 void my_operator::hessian(my_vector *in, my_vector *out){
  
   my_vector *d=range->clone_vec();
   forward(false,in,d);
   adjoint(false,out,d);
   delete d;
}
 bool my_operator::check_domain_range(my_vector *dom, my_vector *rang){
   if(!dom->check_match(domain)){
     fprintf(stderr,"domains don't match\n");
     return false;
   }
   if(!rang->check_match(range)){
     fprintf(stderr,"ranges don't match\n");
     return false;
   }
   return true;
 }
