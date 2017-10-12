
#include "dynamic.h"
using namespace SEP;
dynamic::dynamic(float_2d *s, float jump_pen){


 std::shared_ptr<float_2d> sc(new float_2d(s));
 sim=sc;
 std::shared_ptr<float_2d> sc2(new float_2d(s));
 score=sc2;
 jump=jump_pen;
 for(int i=0; i < score->getN123();i++) score->vals[i]=0.;
}
void dynamic::add_to_sim(float_2d *s){
 for(int i=0; i< sim->getN123(); i++) sim->vals[i]+=s->vals[i];
 
}
void dynamic::calc_score()
{
  int n1=sim->getAxis(1).n;
  int n2=sim->getAxis(2).n;
  //First fill the bottom edges
  for(int i=0; i < n2; i++) score->vals[n1-1+i*n2]=sim->vals[n1-1+i*n2];
  for(int i=0; i < n1; i++) score->vals[n1*(n2-1)+i]=sim->vals[n1*(n2-1)+i];
  float mymax2=-100.,mymax,sc;
  for(int i2=n2-2; i2 >=0 ; i2--){
    for(int i1=n1-2; i1>=0; i1--){
      mymax=-9;
      for(int i=i1+1; i < n1-1; i++){
        sc=score->vals[(i2+1)*n1+i]-jump*(i-i1);
        if(sc>mymax) sc=mymax;
      }
      for(int j=i2+1; j < n2-1; j++){
        sc=score->vals[(i1+1)+n1*j]-jump*(j-i2);
        if(sc>mymax) sc=mymax;
      }
      score->vals[i1+i2*n1]=mymax+sim->vals[i1+i2*n1];
      if(score->vals[i1+i2*n1] > mymax2){
        mymax2=score->vals[i1+i2*n1];
        maxloc[0]=i1; maxloc[1]=i2;
      }
    }
  }
 
}
std::vector<path> dynamic::return_path(){
  std::vector<path> myp;
  
  int i1=maxloc[0], i2=maxloc[1];
  int n1=sim->getAxis(1).n;
  int n2=sim->getAxis(2).n;
  float down,right,diagonal;
  myp.push_back(path(i1,i2));
  while(i1 !=n1-1 && i2 !=n2-1){
    down=score->vals[i1+1+i2*n1];
    right=score->vals[i1+i2*n1+n1];
    diagonal=score->vals[i1+1+i2*n1+n1];
    if(down > right && down > diagonal)i1++;
    else if(right >diagonal) i2++;
    else{ i1++; i2++;}
    myp.push_back(path(i1,i2));
  }
  return myp;
}
