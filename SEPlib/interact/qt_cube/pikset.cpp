#include<pikset.h>
#include<algorithm>
#include<math.h>
int isort;
    bool comp2(pik p1, pik p2){
     return p1.floc[isort] < p2.floc[isort]; 
    }



std::vector<pik> pikset::get_pts(float *fbeg, float *fend,int ilim){
  int j;
  bool good;
  std::vector <pik> pout;
 

  for(int i=0; i < (int) pts.size(); i++){
    j=0; good=true;
    while(j < 8 && good){
      if(pts[i].floc[j] < fbeg[j] || pts[i].floc[j] > fend[j]){
    //  fprintf(stderr,"KILLING %d axis=%d %f %f %f\n",i,j,
     //    fbeg[j],pts[i].floc[j],fend[j]);
          good=false;
        }
      if(ilim !=-1 && ilim!= pts[i].type) good=false;
      j++;
    }
    if(good) pout.push_back(pts[i]);
  }
  return pout;
}


std::vector<pik> pikset::get_pts_sort(float *beg, float *end, int sort, float *ds,int ilim){
  isort=sort;
  if(ds==0);
  std::vector< pik> pout=get_pts(beg,end,ilim);
  std::sort(pout.begin(),pout.end(),comp2);
  return pout;
}
void pikset::add_pt(float *floc,int type,int ex){
  pts.push_back(pik(floc,type,ex));

}
void pikset::find_closest(float *loc,float *scale,int *iclose, float *closest,float *ds){

  
  *closest=1e30;
  *iclose=-1;
  float dist;
  for(int i=0 ; i < (int)pts.size(); i++){
  
    dist=scale[0]/ds[0]/ds[0]*(loc[0]-pts[i].floc[0])*(loc[0]-pts[i].floc[0])+
     1./ds[1]/ds[1]*scale[1]*(loc[1]-pts[i].floc[1])*(loc[1]-pts[i].floc[1])+
     1./ds[2]/ds[2]*scale[2]*(loc[2]-pts[i].floc[2])*(loc[2]-pts[i].floc[2])+
     1./ds[3]/ds[3]*scale[3]*(loc[3]-pts[i].floc[3])*(loc[3]-pts[i].floc[3])+
     1./ds[4]/ds[4]*scale[1]*(loc[4]-pts[i].floc[4])*(loc[4]-pts[i].floc[4])+
     1./ds[5]/ds[5]*scale[2]*(loc[5]-pts[i].floc[5])*(loc[5]-pts[i].floc[5])+
     1./ds[6]/ds[6]*scale[3]*(loc[6]-pts[i].floc[6])*(loc[6]-pts[i].floc[6])+
     1./ds[7]/ds[7]*scale[4]*(loc[7]-pts[i].floc[7])*(loc[7]-pts[i].floc[7]);

     
     
     if(dist < *closest) {
      *iclose=i;*closest=dist;
    }
  }

}

bool pikset::del_pt(float *floc, int iax1, int iax2,float *ds){
  bool good;
  int iclose=-1;
  float closest;
  
  float scale[8];
  for(int i=0; i < 8; i++) scale[i]=999;
  scale[iax1]=scale[iax2]=1;
  find_closest(floc,scale,&iclose,&closest,ds);

  if(iclose!=-1){
    good=true;
    for(int i=0; i < 8; i++){
      if(i!=iax1 && i!=iax2){
       if(fabs(pts[iclose].floc[i] -floc[i])>fabs(ds[i])/3.) good=false;
      }
   //   if(good) fprintf(stderr,"close good %d \n",i);
    //  else fprintf(stderr,"close bad %d %f %f %f \n",i,pts[iclose].floc[i],floc[i],ds[i]);
    }
    if(good ) {
      pts.erase(pts.begin()+iclose);
      return true;
    }
  }
  return false;
}
void pikset::delete_picks(int ity){
 std::vector<pik>::iterator iter;
for(int i=pts.size()-1; i>=0 ; i--){
   if(pts[i].type ==ity){
      pts.erase(pts.begin()+i);
  
   }
 }
 }
 
 
 float *pikset::return_pick_line(int n, int *ipos,float *scale,float *os, float *ds,
  int ax,int single){

  float *ar=new float[n];
  for(int i=0; i< n; i++) ar[i]=-1;


  float dist;
  int iclose;
  float beg[8],end[8],fpos[8];
  for(int i=0; i < 8; i++) fpos[i]=ipos[i]*ds[i]+os[i];
  find_closest(fpos,scale,&iclose,&dist,ds);
  if(iclose==-1) return ar;
  for(int i=0; i < 8; i++) {
    if(ipos[i]!=-1) {
      beg[i]=pts[iclose].floc[i]-ds[iclose]/2.;
      end[i]=beg[i]+ds[iclose];
    }
    else{
      beg[i]=-1e29; end[i]=1e29;
    }
  }
 
   std::vector <pik> pts= get_pts(beg,end);
   build_int_line(pts,0,n,ar,single,os[single],ds[single],ax,os[ax],ds[ax]);
   return ar;
}


void pikset::delete_pt(float *floc,float *ds){
  for(int i=0;i < (int)pts.size(); i++){
    if(fabs(floc[0]-pts[i].floc[0])< ds[0]/10. &&
    fabs(floc[1]-pts[i].floc[1])< ds[0]/10. &&
    fabs(floc[2]-pts[i].floc[2])< ds[0]/10. &&
    fabs(floc[3]-pts[i].floc[3])< ds[0]/10. &&
    fabs(floc[4]-pts[i].floc[4])< ds[0]/10. &&
    fabs(floc[5]-pts[i].floc[5])< ds[0]/10. &&
    fabs(floc[6]-pts[i].floc[6])< ds[0]/10. &&
    fabs(floc[7]-pts[i].floc[7])< ds[0]/10. )
      pts.erase(pts.begin()+i);
  }
}
void pikset::build_int_line(std::vector<pik> pts, int f, int n, float *ar,int single,
 float osingle, float dsingle, int isort, float osort, float dsort){

  int i2,i;



  int ifirst=9999,ilast=-1,i1;
  for(i=0; i< n; i++) ar[i]=-1;

  for( i=0; i <(int)pts.size(); i++){

    i1=(int)((pts[i].floc[single]-osingle)/dsingle+.4999)-f; 
    i2=(int)((pts[i].floc[isort]-osort)/dsort+.4999);

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
