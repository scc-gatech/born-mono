#ifndef BASIC_SOLVER_H
#include<my_vector.h>
#include<my_operator.h>
#include<cgstep.h>
#include <vector>
#include "mask.h"
class lin_solver_basic{
  public:
    lin_solver_basic(){set_dummy();}
    
    void set_dummy(){rr=0; gg=0; g=0; prec=0,mod0=0;my_mask=0;mod=0;t_op=0;tw_op=0;p0=0;}
    lin_solver_basic( step *s,my_vector *m0=0);
    lin_solver_basic(step *s, my_operator *prec, my_vector *m0=0);
    void add_equation(my_vector *d, my_operator *o, my_operator *w=0);
    void add_equation(my_operator *o, double sc=1.);
    bool add_mask(mask *m){ my_mask=m;}
    void add_p0(my_vector *pz){p0=pz;}
    bool build_normal();
   


    my_vector *solve(int niter);
    void set_step(step *s){ st=s;}
   ~lin_solver_basic(){
      if(rr!=0){
      delete rr; delete g; delete gg; delete mod;
      if(t_op!=0) delete t_op;
      if(tw_op!=0) delete tw_op;
      for(int i=0; i < wfake.size(); i++) {
        if(wfake[i]) delete ws[i];
      }
     }
    }
    my_vector *get_m0();
    void create_b();
    void create_ww_rr();
    void update_m0(my_vector *m){ mod0=m;}
    void update_dat(int idat, my_vector *dat);
  protected:
    my_vector *rr,*g,*gg,*mod0,*mod,*p0;
    step *st;
    my_operator *op,*t_op,*tw_op;
    my_operator *prec;
    mask *my_mask;
    std::vector <my_operator*> ops,ws;
    std::vector <my_vector*> ds;
    std::vector < bool> wfake;
    bool do_wt;

};
#endif
