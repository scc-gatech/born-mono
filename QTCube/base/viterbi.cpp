#include "viterbi.h"
using namespace SEP;
viterbi::viterbi(std::shared_ptr<my_score> s2,std::shared_ptr<float_2d> s, int ns,int floc){

 sc=s2;
 std::shared_ptr<float_2d> sc(new float_2d(s));
 sim=s;
 std::shared_ptr<float_2d> sc2(new float_2d(s));
 score=sc2;
 nsearch=ns;
 for(int i=0; i < score->getN123();i++) score->vals[i]=0.;
 f=floc;
}

void viterbi::calc_score()
{

  int n1=score->getAxis(1).n;
  int n2=score->getAxis(2).n;
 
  for(int i=0; i < n1; i++) score->vals[i+(n2-1)*n1]=sim->vals[i+(n2-1)*n1];
  int ib,ie;
  float mymax2=-100.,mymax,ss;
  for(int i2=n2-2; i2 >=0 ; i2--){
    for(int i1=0;i1< n1; i1++){
      ib=i1-nsearch; ie=i1+nsearch;
      if(ib < 0) ib=0;
      if(ie> n1-1) ie=n1-1;
      mymax=-9999;
       //
       
      for(int i=ib; i<=ie; i++){
        ss= score->vals[i+i2*n1+n1]+
        sim->vals[i1+i2*n1] +sc->penalty(i2+f,i,i1);
        if(mymax < ss) mymax=ss;
      }
 
      
      score->vals[i1+i2*n1]=mymax;//+sim->vals[i1+i2*n1];
           if(score->vals[i1+i2*n1] > mymax2){
       
        mymax2=score->vals[i1+i2*n1];

        maxloc[0]=i1; maxloc[1]=i2;
      }
    }
  }
  
}
std::vector<path> viterbi::return_path(){
  std::vector<path> myp;
  
  
  int n1=sim->getAxis(1).n;
  int n2=sim->getAxis(2).n;
  
  int i1;
  int imax;

  float mymax;
  //for( i2=0; i2 < n2; i2++){
    mymax=score->vals[0]; imax=0;

    for(i1=1; i1 < n1;i1++){
      if(mymax < score->vals[i1]){ mymax=score->vals[i1]; imax=i1;}
      }

          myp.push_back(path(imax,0));
    for(int i2=1; i2 < n2; i2++){
        int ibeg=imax-nsearch, iend=imax+nsearch;
        if(ibeg<0) ibeg=0; if(iend>=n1-1) iend=n1-1;
          mymax=score->vals[i2*n1]; imax=0;
        for(int i1=ibeg; i1 < iend; i1++){
      if(mymax < score->vals[i1+i2*n1]){ mymax=score->vals[i1+i2*n1]; imax=i1;}
      }
    myp.push_back(path(imax,i2));
  }
  return myp;
}
