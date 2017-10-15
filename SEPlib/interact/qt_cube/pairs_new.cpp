#include "pairs_new.h"
#include <algorithm>
#include <vector>
#include <stdio.h>


bool compx(pair_new x1, pair_new x2){
  return x1.x < x2.x;
}
bool compy(pair_new x1, pair_new x2){
  return x1.y < x2.y;
}

pair_new::pair_new(int _x, int _y){
  x=_x;
  y=_y;

}
void pairs_new::info(char *str,int i){

 fprintf(stderr,"%s size=%d \n",str,size());
 if(i >0){
   for(int j=0; j < size(); j++){
     fprintf(stderr,"VAL %d %d %d  \n",
        j,vals[j].x,vals[j].y);
     }
   }

}
void pairs_new::sort1(){
  std::sort(vals.begin(),vals.end(),compx);
}
void pairs_new::sort2(){
  std::sort(vals.begin(),vals.end(),compy);
}
void pairs_new::build_int_line(int f, int n, float *ar){



  int i2,i;
  //isort-->y
  //isingle-->x


  int ifirst=9999,ilast=-1,i1;
  for(i=0; i< n; i++) ar[i]=-1;

  for( i=0; i <size(); i++){

    i1=vals[i].y-f; 
    i2=vals[i].x;
   fprintf(stderr,"filling points %d %d \n",i1,i2);
    ar[i1]=i2;

    if(i1 < ifirst) ifirst=i1;

    if(i1 > ilast) ilast=i1;
    
  }
  if(ilast!=-1) { 
     for(i=0; i <  ifirst; i++) ar[i]=ar[ifirst];
     for(i=ilast ; i<n; i++) ar[i]=ar[ilast];
     i=0;
     float f;
     int beg=ifirst;
     while(i < n-1){
       i=beg+1;
       while(ar[i] < 0.) i++;
       for(int j=beg+1; j < i; j++){
         f=(float)(j-beg)/((float)(i-beg));
         ar[j]=(1.-f)*ar[beg]+f*ar[i];
         fprintf(stderr,"making my list %d %f \n",j,ar[j]);
       }
       beg=i; 
       if(beg==ilast)  i=n;
    }
  }
}
