#include<lloyd_1d.h>
#include<math.h>
#include<sep_pars_external.h>

#include<sep_main_external.h>
void lloyd_1d::find_bound(){

  for(int i=0; i < (((int)(center.size()))-1); i++) bound[i]=(center[i]+center[i+1])/2.;
  bound[center.size()-1]=max_v+5;
 // for(int i=0; i <center.size();i++) fprintf(stderr,"BOUNDS %d %f %f \n",
  //  i,center[i],bound[i]);
}

void lloyd_1d_fit::partition_space(){
 int iloc=0;
 float end=bound[iloc];
 for(int i=0; i < ((int)(region.size()));i++){
   if( i >end) {iloc++;end=bound[iloc];}
   region[i]=iloc;
 //  fprintf(stderr,"SETTING REGION %d %d \n",i,region[i]);
 }
}
void lloyd_1d_fit::compute_center(){
//fprintf(stderr,"BEFORE COMPUTE BAXK %d \n",region.size());
  this->compute_back();
  for(int i=0; i < ((int)(wt.size())); i++){ wt[i]=sum[i]=0.;}
  //for(int i=0; i < ((int)(center.size())); i++){
    for(int id=0; id < ((int)(region.size())); id++){
   // fprintf(stderr,"CHECK REGION %d %d \n",id,region[id]);
      wt[region[id]]+=(range+back[id]);
      sum[region[id]]+=(float)id*(range+back[id]);
     //fprintf(stderr,"CENTERCALC %d %d %f %f %f\n",id,region[id],wt[region[id]],
       //sum[region[id]],sum[region[id]]/wt[region[id]]);
   
  }
  for(int i=0; i < ((int)(wt.size())); i++){
    if(wt[i]==0.) center[i]=0.;
    else center[i]=sum[i]/wt[i];
    //fprintf(stderr,"CHECK CENTERS %d %f %f %f %f \n",i,sum[i],wt[i],center[i]);
  }

}


void lloyd_1d_fit::compute_back(){
  float beg=0;
  float end=bound[0];
  float bval,eval,f;
  // int i2;
  int iloc=0;
  bool endl=false;
 
  bval=eval=ret_val(center[0]);
//fprintf(stderr,"IN COMPUTE BAXK %d \n",region.size());
  for(int i=0; i < ((int)(region.size())); i++){
//  fprintf(stderr,"CJECK COND %d %d %f \n",i,iloc,center[iloc]);
    if(i > center[iloc] && !endl){
       bval=eval;iloc++;  end=beg;
       if(iloc==((int)(center.size()))){ eval=bval;endl=true;end=back.size();}
       else{ 
     //  fprintf(stderr,"IN ELSE CONDITIONAL %d %f %d \n",iloc,center[iloc],center.size());
         eval=ret_val(center[iloc]);end=center[iloc];}
     }
     if(fabs(end-beg) < .00001) f=0.;
     else f=((float)i-beg)/(end-beg);
  //   fprintf(stderr,"CHECK BEG END %f %f \n",beg,end);
    //  fprintf(stderr,"CHECK BEG END %f %f \n",back[i],trace[i]);
     back[i]=trace[i]-((1.-f)*bval+f*eval);
   //  fprintf(stderr,"SETTING BAACK %f %f %f %f %f %d \n",trace[i],beg,end,back[i],f,i);
   }
}

void lloyd_1d_fit::compute_distort(){
  this->compute_back();
  for(int i=0; i < ((int)(distort.size())); i++) {wt[i]=0.;distort[i]=0.;sum[i]=0.;}
  for(int i=0; i < ((int)(region.size()));i++){
//Is this wrong?
    sum[region[i]]+=fabs(back[i]);
    wt[region[i]]+=1.;
   // fprintf(stderr,"IN COMPUTE %d %d %f %f \n",i,region[i],sum[region[i]],wt[region[i]]);
  }
  for(int i=0; i < ((int)(distort.size()));i++){
    
    if(wt[i]>0.) distort[i]=sum[i]/wt[i]/range;
    else distort[i]=0.;
   //     fprintf(stderr,"CHECK DISTORTS %d %f %f %f %f \n",i,sum[i],wt[i],distort[i],range);
  }
  
}
/*ARGSUSED*/
void lloyd_1d::change_regions(int iter){
(void) iter; // silence unused argument complaint.
  std::vector<bool> del;
  float dmin=99999.;
  int imin=0;
  for(int i=0; i < ((int)(center.size())); i++){
    
     if(wt[i] <.001 ) del.push_back(true);
     else {
       del.push_back(false);
       if(distort[i] < dmin) { dmin=distort[i]; imin=i;}
     }
  }
  if(dmin< del_criteria) del[imin]=true;
  for(int i=center.size()-1; i >=0 ; i--){
    
    if(del[i]){ del_val(i);
     //fprintf(stderr,"DELETING %d \n",i);
    }
  }
  del.clear();
}


void lloyd_1d::del_val(int iloc){
  bound.erase(bound.begin()+iloc);
  center.erase(center.begin()+iloc);
  wt.erase(wt.begin()+iloc);
  sum.erase(sum.begin()+iloc);
}
/*ARGSUSED*/
void lloyd_1d::add_val(int iloc){
(void) iloc; // quiet unused argument complaint
  std::vector<float>::iterator it;
  bound.insert(it,0.);
  center.insert(it,0.);
  wt.insert(it,0.);
  sum.insert(it,0.);
}

void lloyd_1d::split_region(){
  float maxd=0.; int imax=-1;
  for(int i=0; i < ((int)(center.size())); i++){
    if(distort[i] > maxd) {maxd=distort[i]; imax=i;}
  }
  float beg,end;
  if(maxd > split_criteria){
    add_val(imax);
    if(imax >0) beg=center[imax-1];
    else beg=0;
    center[imax]=0.5*(beg+center[imax+1]);
    if(imax == (((int)(center.size()))-2)) end=max_v;
    else end=center[imax+2];
    center[imax+1]=0.5*(center[imax+1]+end);
    }
  
}
lloyd_1d_fit::lloyd_1d_fit(std::vector<float> tr, int nreg, float delc,float splitc){

  del_criteria=delc;
  split_criteria=splitc;
  max_v=tr.size();
  set_basics(tr,nreg);
  float mmin,mmax;
  mmin=mmax=trace[0];
  for(int i=0; i< ((int)(trace.size()));i++)    {
    back.push_back(0.);
    if(trace[i] < mmin) mmin=trace[i];
    if(trace[i] >mmax) mmax=trace[i];
 }
 range=mmax-mmin;
}

void lloyd_1d::set_basics(std::vector<float> tr,int nreg){

  for(int i=0; i < ((int)(tr.size())); i++){
    region.push_back(1);
    trace.push_back(tr[i]);

  }
  float cen=(float)trace.size()/((float)(nreg+1));
  for(int i=0; i < nreg; i++){
    bound.push_back(0.);
    distort.push_back(0.);
    wt.push_back(0.);
    sum.push_back(0.);
    center.push_back((float)(i+1)*cen);
  }

}


lloyd_1d_p::lloyd_1d_p(std::vector<float> tr, int nreg, float delc,float splitc){

  del_criteria=delc;
  split_criteria=splitc;
  max_v=tr.size();
  set_basics(tr,nreg);
  float mmin,mmax;
  mmin=mmax=trace[0];
  for(int i=0; i< ((int)(trace.size()));i++)    {
    back.push_back(0.);

    if(trace[i] < mmin) mmin=trace[i];
    if(trace[i] >mmax) mmax=trace[i];
 
 }
 for(int i=0;i < nreg; i++){
     sign.push_back(1);
    mymin.push_back(99999);
    mymax.push_back(99999);
    }
 range=mmax-mmin;
 writeit=false;
}

void lloyd_1d_p::partition_space(){
 int iloc=0;
 float end=bound[iloc];
 for(int i=0; i < ((int)(region.size()));i++){
   if( i >end) {iloc++;end=bound[iloc];}
   region[i]=iloc;
 // fprintf(stderr,"CHECK REGION %d %d \n",i,iloc);
 }
 //fprintf(stderr,"REGION 1 CHECK %d \n",region[774]);
}
void lloyd_1d_p::compute_center(){
//fprintf(stderr,"BEFORE COMPUTE BAXK %d \n",region.size());
  this->compute_back();
  for(int i=0; i < ((int)(wt.size())); i++){ wt[i]=sum[i]=0.; mymin[i]=range*10; mymax[i]=-range*10;}
  int id;
  for(int i=0;  i< ((int)(back.size())); i++){
    id=region[i];
    if(mymin[id] > back[i]) mymin[id]=back[i];
    if(mymax[id] < back[i]) mymax[id]=back[i];
    sum[id]+=back[i];
  }
  for(int i=0; i < ((int)(wt.size())); i++){
    if(sum[i] >0) sign[i]=1;
    else sign[i]=-1;
    sum[i]=0.;
  }
  for(int i=0; i < ((int)(back.size())); i++){
    id=region[i];
    if(sign[id]>0){
       sum[id]+=(back[i]-mymin[id])*i;
       wt[id]+=back[i]-mymin[id];
    }
    else {
   //   fprintf(stderr,"MY REGION %d %d %d \n",id,i,mymax.size());
      sum[id]+=(mymax[id]-back[i])*i;
      wt[id]+=mymax[id]-back[i];
    }
  }
  for(int i=0; i < ((int)(wt.size())); i++){
    if(wt[i] >0){
      center[i]=sum[i]/wt[i];
    }
    else center[i]=0.;
 }
      
}

void lloyd_1d_p::compute_back(){
  // float beg=0;
  // float end=bound[0];
  float bval,eval,f;
  // int i2;
  int iloc=0;
  // bool endl=false;
  int ib,ie;
 

 
  bval=eval=ret_val(center[0]);
//fprintf(stderr,"IN COMPUTE BAXK %d \n",region.size());
//   fprintf(stderr,"REGION 3 CHECK %d \n",region[774]);

  for(int i=0; i < ((int)(region.size())); i++){
    if(i > center[iloc]) iloc++;
    if(iloc==0) { f=0; ib=0; ie=0;}
    else if(iloc>=((int)(center.size()))) { f=0; ib=center.size()-1; ie=center.size()-1;}
    else{
      f=((float)i-center[iloc-1])/(center[iloc]-center[iloc-1]);
      ib=iloc-1;
      ie=iloc;
      
    }
    bval=trace[(int)(center[ib]+.5)];
    eval=trace[(int)(center[ie]+.5)];
    back[i]=trace[i]-((1.-f)*bval+f*eval);
 
  //  fprintf(stderr,"CHECK %d %d %f  %f %f \n",i,ib,val,back[i],trace[ib]);
  }

//seperr("");
/*
  for(int i=0; i < region.size(); i++){
// fprintf(stderr,"CJECK COND %d %d %d %f \n",bound.size(),i,iloc,center[iloc]);
    if(i > center[iloc] && !endl){
       bval=eval;iloc++;  end=beg;
       if(iloc==center.size()){ eval=bval;endl=true;end=back.size(); iloc--;}
       else{ 
     //  fprintf(stderr,"IN ELSE CONDITIONAL %d %f %d \n",iloc,center[iloc],center.size());
         eval=ret_val(center[iloc]);end=center[iloc];}
     }
     if(fabs(end-beg) < .00001) f=0.;
     else f=((float)i-beg)/(end-beg);
  //   fprintf(stderr,"CHECK BEG END %f %f \n",beg,end);
    //  fprintf(stderr,"CHECK BEG END %f %f \n",back[i],trace[i]);
     back[i]=trace[i]-((1.-f)*bval+f*eval);
   //  fprintf(stderr,"SETTING BAACK %f %f %f %f %f %d \n",trace[i],beg,end,back[i],f,i);
   }
   */
   if(iter==1 && writeit){
     float *backf=new float[back.size()];
     float *pos=new float[center.size()*2];
     int n=back.size(); auxputch("n1","d",&n,"back");
          n=2; auxputch("n2","d",&n,"back");
     n=center.size(); auxputch("n1","d",&n,"pts");
     n=8; auxputch("esize","d",&n,"pts");
      for(int i=0; i< ((int)(back.size())); i++) backf[i]=trace[i]-back[i];
     srite("back",backf,4*(int)back.size());
     for(int i=0; i< ((int)(back.size())); i++) backf[i]=back[i];
     srite("back",backf,4*(int)back.size());
     
     for(int i=0; i< ((int)(back.size())); i++) backf[i]=back[i];
     for(int i=0; i< ((int)(center.size()));i++){
       pos[i*2]=center[i];
       pos[i*2+1]=trace[(int)(center[i]+.5)];
     }
     srite("pts",pos,8*(int)center.size());
     delete [] pos; delete [] backf;
   
   }
}

void lloyd_1d_p::compute_distort(){
  this->compute_back();
  for(int i=0; i < ((int)(distort.size())); i++) {wt[i]=0.;distort[i]=0.;sum[i]=0.;}
  for(int i=0; i < ((int)(region.size()));i++){
//Is this wrong?
    sum[region[i]]+=fabs(back[i]);
    wt[region[i]]+=1.;
   // fprintf(stderr,"IN COMPUTE %d %d %f %f \n",i,region[i],sum[region[i]],wt[region[i]]);
  }
  for(int i=0; i < ((int)(distort.size()));i++){
    
    if(wt[i]>0.) distort[i]=sum[i]/wt[i]/range;
    else distort[i]=0.;
   //     fprintf(stderr,"CHECK DISTORTS %d %f %f %f %f \n",i,sum[i],wt[i],distort[i],range);
  }
  
}
