#include "auto_brown.h"
#include "fastmarch.h"
#include "viterbi.h"
#include "my_score.h"
using namespace SEP;
std::vector<path> auto_brown::return_path(std::vector<int> locs,float_2d *array){

  int imid=(array->getAxis(1).n-1)/2;
 n1=imid*2+1;
  float mymin=array->my_min();
  for(int i=0; i < array->getN123(); i++){
    array->vals[i]=1./(.3+2.5*(array->vals[i]-mymin));
  }
    n2=array->getAxis(2).n; n3=1; 

  slow=&array->vals[0];
  
  axis a1=array->getAxis(1),a2=array->getAxis(2);
  std::shared_ptr<float_2d> tot(new float_2d(a1,a2));
  float_2d times=float_2d(a1,a2);
  tot->zero();
  b1=1; b2=1; b3=1;
  d1=1.; d2=1.; d3=1.;
  n12=n1*n2;
  n=n12;
  order=1;


 // for(int isrc=0; isrc< (int)locs.size(); isrc++){
for(int isrc=0; isrc< (int)locs.size(); isrc++){
  alloc_local();
    slow0=slow[imid+n1*locs[isrc]];

   calc_time(imid,locs[isrc],0,&times.vals[0]);
   int i=0;
   for(int i2=0; i2 < n2; i2++){
     for(int i1=0; i1 < n1; i1++,i++){
       float dist=sqrtf(1.*((i2-locs[isrc])*(i2-locs[isrc])+(i1-imid)*(i1-imid)));
       tot->vals[i]+=dist/(times.vals[i]+.01);
     }
   }

  delete_fastmarch();
  }


  int ilast=imid;
  int imin;
  float val;
   for(int i=0; i < (int) locs.size(); i++)
       tot->vals[imid+locs[i]*(n1)]=nail;
       

   std::shared_ptr<my_score>  score (new my_score(pen_o,pen_e));
     viterbi myd=viterbi(score,tot,4,0);

      myd.calc_score();
      return myd.return_path(); 
  if(1==3){
  std::vector<path> myp;
  for(int i2=1; i2 < n2; i2++){
    mymin=tot->vals[n*i2+ilast];
    imin=ilast;
    if(ilast >0) {
      val=tot->vals[n*i2+ilast-1];
      if(val < mymin){ mymin=val; imin=ilast-1;}
    }
    if(ilast<n-1){
      val=tot->vals[n*i2+ilast+1];
      if(val <mymin) { mymin=val; imin=ilast+1;
     }
     ilast=imin;
     myp.push_back(path(ilast,i2));
   }
  }
  
  return myp;
}
}

