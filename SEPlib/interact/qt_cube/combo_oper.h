#ifndef COMBO_OPER_H
#define COMBO_OPER_H 1
#include "my_operator.h"
#include "super_vector.h"

class col_op: public my_operator{
  public:
    col_op(){basic_init_op();}  
    col_op(my_operator *op);
    col_op(my_operator *op1,my_operator *op2);
    col_op(std::vector<my_operator *>op);
    void add_op(my_operator *op);
    void add_ops(std::vector<my_operator *>os);
    void create_super();
    
    virtual bool forward(bool add, my_vector *model, my_vector *data);
    virtual bool adjoint(bool add, my_vector *model, my_vector *data);
    virtual norm_vector *range_vec(norm_vector *v1, norm_vector *v2){
       return (norm_vector*)new super_vector(v1,v2);
    
    }
    ~col_op(){

      ops.clear();
    }
   
     virtual void set_scale(double sc){
      ops[0]->scale=ops[0]->scale*sc;
    }
   std::vector<my_operator*> ops;
  
};

class diag_op: public my_operator{
  public:
    diag_op(){basic_init_op();}  
    diag_op(my_operator *op);
    diag_op(my_operator *op1,my_operator *op2);
    diag_op(std::vector<my_operator *>op);
  
    void add_op(my_operator *op);
    void add_ops(std::vector<my_operator *>os);
    void create_super();
    virtual bool forward(bool add, my_vector *model, my_vector *data);
    virtual bool adjoint(bool add, my_vector *model, my_vector *data);
    ~diag_op(){
   //   ops.clear();
    }
   
      virtual void set_scale(double sc){
      ops[0]->scale=ops[0]->scale*sc;
    }
   std::vector<my_operator*> ops;
  
};

class row_op: public my_operator{
  public:
    row_op(){basic_init_op();}
    row_op(my_operator *op){
      std::vector<my_operator *> os; os.push_back(op);
      chain_init(os);
    }
    row_op(my_operator *op1, my_operator *op2){
       std::vector<my_operator *> os; os.push_back(op1);
       os.push_back(op2); chain_init(os);
         strcpy(name,"ROW_OP");

    }
        row_op(my_operator *op1, my_operator *op2, my_operator *op3){
       std::vector<my_operator *> os; os.push_back(op1);
       os.push_back(op2); os.push_back(op3) ; chain_init(os);
    }
    row_op(std::vector<my_operator *> os){
      chain_init(os);
    }
    bool check_valid_chain();
    virtual bool forward(bool add, my_vector *model, my_vector *data);
    virtual bool adjoint(bool add, my_vector *model, my_vector *data);
    void chain_init(std::vector<my_operator*> os);
    void add_op(my_operator *op);
    virtual void set_scale(double sc){
      ops[0]->scale=ops[0]->scale*sc;
    }
    ~row_op(){
     // ops.clear();
    }
   
  
   
   std::vector<my_operator*> ops;

};
  
#endif 
