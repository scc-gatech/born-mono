#ifndef MY_SCORE_H
#define MY_SCORE_H 1
#include<stdlib.h>
#include<math.h>
class my_score{
  public:
  my_score(){};
  my_score(float i, float g){ init=i; growth=g;}
  virtual float penalty(int iloc, int iold,int inew){
     if(iloc==0){}
     if(iold==inew) return 0;
     else return -(init+growth*abs(iold-inew));
  }
  float init,growth;
  virtual ~my_score(){}
  
  };
class semb_score: public my_score{
 public:
  semb_score(){};
  semb_score(axis time, axis vel, float p, float mv=1.4, float mx=5.1){
    ot=time.o; dt=time.d; ov=vel.o; pen=p;dv=vel.d;
    minv=mv*mv; maxv=mx*mx;
   
   }
   virtual float penalty(int iloc,int iold, int inew){
     //float time=ot+dt*iloc+dt;
     //float velo=iold*dv+ov;
    // float veln=inew*dv+ov;
     //float dv=((velo*velo*time-(time-dt)*veln*veln)/dt);
     //fprintf(stderr,"DV IS %f %f %f %f =%f \n",velo,veln,velo*velo*time,(time-dt)*veln*veln,dv);
    // if(dv < minv || dv >maxv) return -8;
    // else 
     if(iloc==0){}
     return -abs(iold-inew)*pen;
   }
   virtual ~semb_score(){}
    float ov,dv,minv,maxv,dt,ot,pen;
    
  
  };
  
  
#endif
