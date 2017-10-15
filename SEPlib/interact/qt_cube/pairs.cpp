#include "pairs.h"
#include <algorithm>
#include <vector>
#include <stdio.h>


bool compx(pair x1, pair x2){
  return x1.x < x2.x;
}
bool compy(pair x1, pair x2){
  return x1.y < x2.y;
}

pair::pair(float _x, float _y, int typ, int ex){
  x=_x;
  y=_y;
  type=typ;
  extra=ex;
}
void pairs::info(char *str,int i){

 fprintf(stderr,"%s size=%d \n",str,size());
 if(i >0){
   for(int j=0; j < size(); j++){
     fprintf(stderr,"VAL %d %f %f %d %d \n",
        j,vals[j].x,vals[j].y,vals[j].type,vals[j].extra);
     }
   }

}
void pairs::sort1(){
  std::sort(vals.begin(),vals.end(),compx);
}
void pairs::sort2(){
  std::sort(vals.begin(),vals.end(),compy);
}
void pairs::build_int_line(int f, int n, float *ar,
 float osingle, float dsingle,float osort, float dsort){

  int i2,i;
  //isort-->y
  //isingle-->x


  int ifirst=9999,ilast=-1,i1;
  for(i=0; i< n; i++) ar[i]=-1;

  for( i=0; i <size(); i++){

    i1=(int)((vals[i].y-osingle)/dsingle+.4999)-f; 
    i2=(int)((vals[i].x-osort)/dsort+.4999);
    //fprintf(stderr,"THE POINT IS %d %d \n",i1,i2);

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
       }
       beg=i; 
       if(beg==ilast)  i=n;
    }
  }
}

