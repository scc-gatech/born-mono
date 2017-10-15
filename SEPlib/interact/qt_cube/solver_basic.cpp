#include "solver_basic.h"
#include "i_op.h"
#include "super_vector.h"
#include "combo_oper.h"
lin_solver_basic::lin_solver_basic( step *s, my_vector *m0){
   set_dummy();

  st=s;
 
  mod0=m0;

  
}
lin_solver_basic::lin_solver_basic( step *s,my_operator *p, my_vector *m0){
  set_dummy();

 st=s;
  mod0=m0;
  prec=p;
}
void lin_solver_basic::add_equation(my_vector *d, my_operator *o, my_operator *w){
  ds.push_back(d);
  ops.push_back(o);
  ws.push_back(w);
}
void lin_solver_basic::add_equation(my_operator *o, double sc){

  o->set_scale(sc);
  ds.push_back(o->range->clone_zero());
  my_operator *x=0;
  ws.push_back(x);
  ops.push_back(o);
}

void lin_solver_basic::create_ww_rr(){

 do_wt=false;
 for(int i=0 ; i < ws.size(); i++){
   if(ws[i]!=0) do_wt=true;
   wfake.push_back(false);
 }
 if(do_wt){//Residual space is wt range
   std::vector<my_vector*>wjunk;
   for(int i=0; i < ws.size(); i++){
     if(ws[i]==0){
       wjunk.push_back(ops[i]->range->clone_space());
        ws[i]=new i_op(wjunk[i],wjunk[i]);
        wfake[i]=true;
     }
     else wjunk.push_back(ws[i]->range->clone_space());
   }
   rr=new super_vector(wjunk);
   for(long long i=0; i < wjunk.size(); i++) delete wjunk[i];
   tw_op=new diag_op(ws);
 }
 else rr=new super_vector(ds);
}
my_vector *lin_solver_basic::get_m0(){

  if(prec!=0){
    if(p0!=0){
      my_vector *junk=prec->range->clone_zero();
      prec->forward(false,p0,junk);
     // junk->info("MODEL o",10);
      return junk;
    }
    else return prec->range->clone_zero();
  }
  else if(mod0!=0) return mod0->clone_vec();
  return mod->clone_zero();
}
void lin_solver_basic::create_b(){
  if(do_wt){
     super_vector *jdat=new super_vector(ds);
     tw_op->forward(false,jdat,rr);  
     rr->scale(-1.) ; //rr=-Wd
     my_vector *jmod=get_m0();
     t_op->forward(false,jmod,jdat);
     tw_op->forward(true,jdat,rr);
     delete jdat; delete jmod;
  }
  else{
    rr->scale(-1.) ; //rr=-d
    my_vector *jmod=get_m0();
    t_op->forward(true,jmod,rr);
    delete jmod;
  }
 // rr->info("check rr",10);
}
void lin_solver_basic::update_dat(int idat, my_vector *dat){
  ds[idat]=dat;
  
}

bool lin_solver_basic::build_normal(){
 if(ops.size()==0) {
    fprintf(stderr, "No equations have been set \n");
    return false;
 }
 create_ww_rr(); 
 t_op=new col_op(ops);
 if(prec!=0)
 if(!do_wt){    
   if(prec==0){
     op=new col_op(ops);
   }
   else{
     op=new row_op(prec,t_op);
   }
 }
 else{
   if(prec==0){
      op=new row_op(t_op,tw_op);
   }
   else{
     op=new row_op(prec,t_op,tw_op);
   }
 }
 mod=op->domain->clone_zero();
 
 g=mod->clone_zero();
 if(my_mask!=0){
   if(!my_mask->check_match(mod))
     fprintf(stderr,"ERROR: Mask space doesn't match model space\n");
 }
 
 gg=rr->clone_zero();
 st->alloc_step(g,rr);

 return true;
}


my_vector *lin_solver_basic::solve(int niter){

  if(mod==0) {
     build_normal();
     create_b();
  }

  for(int iter =0; iter < niter; iter++){
 
    op->adjoint(false,g,rr);


    if(my_mask!=0) my_mask->apply(g);

    op->forward(false,g,gg);
           

    bool valid=st->step_it(mod,g,rr,gg);
  
    fprintf(stderr,"FINISHED ITER %d \n",iter);
    if(!valid) return 0;
//    assert(1==2);
  }
  if(prec==0){
    if(mod0!=0) mod->scale_add(1.,mod0,1.);
    return mod->clone_vec();
  }
  else{

    if(p0!=0) mod->scale_add(1.,p0,1.);
    my_vector *mm=prec->range->clone_zero();
  

    prec->forward(false,mod,mm);

    if(mod0!=0) mm->scale_add(1.,mod0,1.);

   
    return mm;
  
  }
}
