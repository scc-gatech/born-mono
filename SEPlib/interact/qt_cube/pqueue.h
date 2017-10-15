
/* 
 * Copyright (c) 1997 Stanford Exploration Project
 * All Rights Reserved
 *
 * Author: Sergey Fomel
   C++ Bob
 */
#ifndef PQUEUE_H
#define PQUEUE_H 1
class pqueue{
  public:
    pqueue(){}
    pqueue(int n);
    ~pqueue(){ delete_pqueue();}
    void delete_pqueue();
    void pqueue_insert(float *v);
    float *pqueue_extract();
    float **x,**xn,**x1;
};
#endif
