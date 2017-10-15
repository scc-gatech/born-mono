
/*
****************************************************************************
*  Copyright (c) 1997 Stanford Exploration Project                         * 
*  All Rights Reserved                                                     *
****************************************************************************
Author: Sergey Fomel 

second-order accuracy added by James Rickett (April 1999)
C++ Bob
*/ 
#ifndef FASTMARCH_H
#define FASTMARCH_H 1
#include "pqueue.h"
class fastmarch{
  public:
    fastmarch(){mask=0;myq=0;};
    fastmarch(int order, 
	      int b1, int b2, int b3,
	      int nz, int ny, int nx, 
	      float dz, float dy, float dx, 
	      float slow0, float *slow);
    int grid(int i, int n);
    void alloc_local();
    void calc_time(float s1,float s2, float s3, float *ttimes);
    void update (int p1, int p2, int p3, float* tj, unsigned char* mj, float s);
    void cvupdate (double cvtime,float* tj,unsigned char* mj);
    ~fastmarch(){ delete_fastmarch();}
    void delete_fastmarch();
    void setup_run(float *ttimes);
    void init_source(float s1, float s2, float s3, float *ttimes);
     int n1,n2,n3;
     int b1,b2,b3;
     double d1,d2,d3;
     float slow0;
     float *slow;
     int n12,n;
     unsigned char *mask;
     int stillconstant;
     pqueue *myq;
     double a, tp,dd[8], dd1,dd2,dd3;
     int nm,order;
     
  };
  #endif
