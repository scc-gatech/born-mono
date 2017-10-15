#ifndef LLOYD_H
#define LLOYD_H 1
class lloyd{

public:
  lloyd(){};
  virtual ~lloyd(){lloyd_delete();}


 
  void do_iter(int iter);
  
  virtual void find_bound(){}
  virtual void partition_space(){}
 void solve_it();
 virtual void compute_center(){}
  virtual void compute_distort(){}
  virtual void change_regions(const int iter){ if(iter){}else{}}

   int iter,niter;
  private:
    void lloyd_delete(){}

   



};
#endif
