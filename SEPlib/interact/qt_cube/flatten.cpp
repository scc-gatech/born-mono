#include<flatten.h>
#include<math.h>
#include<seplib.h>


flatten::flatten(int *n, float e,int iloc_2,int iloc_3,int n_iter){

  n1=n[0]; n2=n[1]; n3=n[2];
  eps_t=e;
  //eps_t=1.;
  alloc=true;
  //temp=new float[n1*n2*n3];
  //temp2=new double[n1*n2*n3];

  int flag=FFTW_PATIENT;
  flag=FFTW_ESTIMATE;
  temp=new float[n1*n2*n3];
  
  //temp[0]=1.2345;
  //temp[1]=2.3456;
 // fftw_init_threads();
  //fftw_plan_with_nthreads(4);
  fftw_test2_(&n1,&n2,&n3,temp,(long long*)&planF,(long long*)&planI);
  //fftwf_r2r_kind kinds[3];
    mult=new float[n1*n2*n3];
  resid=new float[n1*n2*n3*3];
    create_mult();
    create_bank(1001,-3.,3.);
    gmap=new float [n1*n2*n3];
  
 // for(int i=0; i < 3; i++) kinds[i]=FFTW_REDFT10;
  
//  planF=fftwf_plan_r2r(3,n,temp ,temp,kinds,FFTW_ESTIMATE);
  //   for(int i=0; i < 3; i++) kinds[i]=FFTW_REDFT01;
 // planI=fftwf_plan_r2r(3,n,temp ,temp,kinds,FFTW_ESTIMATE);
  iloc2=iloc_2; iloc3=iloc_3;
  iloc=iloc2*n1+iloc3*n1*n2;
  niter=n_iter;
   fprintf(stderr,"TYP AGAIN %f %f %d \n",ofilt,delta,bank.size());
  n123=n1*n2*n3;
  fprintf(stderr,"CHECK %d %d %d %d \n",n1,n2,n3,n123);

} 
void flatten::set_fix(std::vector<ploc> pts){
  
  for(int i=0; i < pts.size(); i++)
    fix.push_back(ploc(pts[i].i1,pts[i].i2,pts[i].i3));
}
void flatten::calc_tau(float *tau, float *dip){

   double res,rold;
   for(int i=0;i < n123;i++) tau[i]=0.;
   for(int iter=0; iter < niter; iter++){
 //    set_fixed(tau);
iii=iter;
   res=nonlinear_iter(tau,dip);
      fprintf(stderr,"TY2 AGAIN %f %f %d \n",ofilt,delta,(int)n123);

  // res=nonlinear_iter3(tau,dip);
//   fprintf(stderr,"SSS %f %f %f \n",dip[0],dip[2],dip[4]);
     fprintf(stderr,"ITER %d  - %g \n",iter,res);
     //if(iter >0 && rold < res) return;
     rold=res;
     
     
   }

}
void flatten::set_fixed(float *tau){
   int del,ii,ival;
   float f;
   for(int i=0; i < fix.size(); i++){
     ii=fix[i].i2*n1+fix[i].i3*n1*n2;
     f=tau[ii+fix[0].i1];
      tau[ii+fix[0].i1]=
     fix[i].i1-fix[0].i1;
     
     fprintf(stderr,"ITAU %d=%f(%f) - %d %d %d \n",i,tau[ii+fix[0].i1],f,fix[i].i1, fix[i].i2,fix[i].i3);
   }
}
void flatten::add_map_tau(float *tau, float *dtau, float o ,float d){

  

  int j=0,iloc,iold;
  float f,delta;
  for(int i2=0; i2 < n2*n3; i2++,j+=n1){
    for(int i1=0; i1 < n1; i1++){
      f=(-tau[i1+j]+i1-o)/d;iloc=f; f-=iloc;
            if(iloc<0) {iloc=0; f=0;}
      else if(iloc>=n1-1){iloc=n1-2; f=1.;}
     // fprintf(stderr,"CHECK %d %d %d \n",i1+j,iloc,iloc+1+j);
      
      
      delta=(1.-f)*dtau[iloc+j]+f*dtau[iloc+1+j];
      
      if(i1 >0){
        if(tau[i1-1+j]-1.<tau[i1+j]+delta) tau[i1+j]-=delta;
        else{
         // fprintf(stderr,"NOT ADDING %d %d %f %f %f %d %d %f %f \n",i1,i2,tau[i1+j-1],tau[i1+j],delta,
           // iloc,iold,dtau[iloc+j],dtau[iloc+j-1]);
           
          tau[i1+j]=tau[i1+j-1]-1.;
        }
      }
      else tau[i1+j]-=delta;
    }
    iold=iloc;
    //tau[i1+j]-=delta;
    
  }
}
void flatten::calc_grad(float *tau, float *grad1, float *grad2,float *grad3){
   int i=0,ibig;
  return;
   for(int i3=1; i3 < n3; i3++){
     for(int i2=1; i2 < n2; i2++){
       ibig=i2*n1+i3*n1*n2;
       for(int i1=1,i=ibig+1; i1 < n1; i1++,i++){
         
         grad1[i]=-eps_t*(tau[i]-tau[i-1]);
         grad2[i]=-tau[i]+tau[i-n1];
         grad3[i]=-tau[i]+tau[i-n1*n2];
       } 
     }
   }
}
void flatten::map_grad_tau(float *tau, float *dip,float *grad,float *oo, float *dd){
   float *tmap=new float[n1];
   float *junk=new float[n1];
    //   fprintf(stderr,"WORKIN1 ON %f %f %f\n",dip[30],dip[32],dip[34]);

   float mymin=tau[0],mymax=tau[0];
   int ii=0;
  n123=n1*n2*n3;
   for(int i=0; i < n2*n3; i++) {
     for(int j=0; j < n1; ii++,j++){
     mymin=MIN(-tau[ii]+j,mymin);
     mymax=MAX(-tau[ii]+j,mymax);
  //   temp[ii]=-tau[ii]+j;
    // fprintf(stderr,"%d %f %f %f \n",i,mymin,mymax,tau[i]);
     }
   }
  int ibig,k;
fprintf(stderr,"MY MIN MAX %f %f %d\n",mymin,mymax,n123);
     srite("check6.H",&grad[n123],n123*4);

 fprintf(stderr,"XXX \n");
   float deltat=(mymax-mymin)/(n1-1);
   otau=mymin; dtau=deltat;
     //  fprintf(stderr,"WORKING3 ON %f %f %f\n",dip[30],dip[32],dip[34]);
//
fprintf(stderr,"MY DELTA TAU IS %f %f %f \n",deltat,mymin,mymax);
 //   srite("check.H",grad,n1*n2*n3*12);
   for(int i=0; i < n2*n3; i++){
   
     //fprintf(stderr,"WORKING ON %d %f %f %f\n",i,dip[0],dip[2],dip[4]);
     //fprintf(stderr,"BEFORE Z T AU \n");
     find_z_tau_map(&tau[i*n1],tmap,mymin,deltat);
      for(int i1=0; i1< n1; i1++){
//        fprintf(stderr,"CHECK IT %d %d \n",i1,i);
         if(tmap[i1] < 0.) gmap[i*n1+i1]=-1.;
         else gmap[i*n1+i1]=tmap[i1];
       }
     srite("check5.H",tmap,n1*4);
     //   fprintf(stderr,"W3ORKING ON %d %f %f %f\n",i,dip[30],dip[32],dip[34]);
     // apply_map(tmap,&grad[i*n1],junk,true);
          jjj=i;
 //find_z_tau_map(&tau[i*n1],tmap,mymin,deltat);
       apply_map(tmap,&grad[n123+i*n1],junk);
       
       srite("check3.H",junk,n1*4);
        //if(iii==3 && jjj==5) seperr("iter limit");
       apply_map(tmap,&grad[i*n1+2*n123],junk);
        srite("check1.H",&temp[i*n1],n1*4);
        apply_map(tmap,&temp[i*n1],junk);
         srite("check2.H",junk,n1*4);
        // if(iii==2) seperr("ddd");
    //    fprintf(stderr,"WO4RKING ON %d %f %f %f\n",i,dip[30],dip[32],dip[34]);
     // apply_map(tmap,&grad[i*n1+n123],junk,true);
   //     fprintf(stderr,"WO5RKING ON %d %f %f %f\n",i,dip[30],dip[32],dip[34]);
     // apply_map(tmap,&grad[i*n1+2*n123],junk,true);
  // fprintf(stderr,"WORKyING ON %d %f %f %f\n",i,dip[30],dip[32],dip[34]);
      apply_map(tmap,&dip[i*n1],junk,false);
       //   fprintf(stderr,"WORKING ON %d %f %f %f\n",i,junk[0],junk[2],junk[4]);
    //  srite("check.H",junk,n1*4);
      
  //    if(i==2) seperr("");
      for(int j=1; j < n1; j++) grad[i*n1+n123+j]=-junk[j];
        srite("check.H",junk,n1*4);

      apply_map(tmap,&dip[i*n1+n123],junk,false);
      for(int j=0; j < n1-1; j++) grad[i*n1+2*n123+j]=-junk[j];
                      srite("check4.H",junk,n1*4);

   
   
  }
  fprintf(stderr,"GMAP  \n");
// for(int i=0;i <n123; i++) temp[i]=tau[i];
//   srite("check.H",grad,n1*n2*n3*12);
   //fprintf(stderr,"CHECK %d %d %d \n",n1,n2,n3);
 //  seperr("");
 /*  for(int i=0; i < n1*n2*n3; i++)
    fprintf(stderr,"GMAP %d %f \n",i,gmap[i]);
   */
   delete [] tmap; delete [] junk;
   *oo=mymin; *dd=deltat;
 }
void flatten::apply_map(float *tmap,float *in, float *out,bool cp){
   int ib;
   float f;
   for(int i=0; i < n1; i++){
     // fprintf(stderr,"MAP %d %f \n",i,tmap[i]);
      if(tmap[i]<-0.0001) out[i]=0;
      else{
        ib=tmap[i]; f=tmap[i]-ib;
   //     fprintf(stderr,"APPLUIN %d %d %f %f \n",i,ib,f,in[ib]);
        out[i]=in[ib]*(1.-f)+f*in[ib+1];
        if(jjj==5 && iii==3 && i >75 && i < 120) fprintf(stderr,"APPLY MAP %d %f %f %f %f \n",
            i,tmap[i],in[ib],in[ib+1],out[i]);
 //      fprintf(stderr,"OUT IS %d %f \n",i,out[i]);
      }
    }
   // seperr("");
    if(cp) memcpy((void*)in,(const void*) out,sizeof(float)*n1);
 }
void flatten::find_z_tau_map(float *tau, float *map, float o, float d){
 
 bool found;
 float look_for;
 int ilast=0,icheat;

 look_for=o-d;
 //fprintf(stderr,"CHECK LOOK FOR %f \n",look_for);
 for(int i1=0; i1 < n1; i1++){
 look_for+=d;
 found=false;
//    if(iii==1) fprintf(stderr,"CHECK %f %f \n",look_for,-tau[ilast]+ilast);
      while(!found){
        if(ilast == n1-1) {
          found=true;
          map[i1]=-1;
          icheat=n1-1;
        } 
        else if(look_for >-tau[ilast]+ilast){
           ilast+=1;
       //    if(iii==1) fprintf(stderr,"GOING UP %f %f \n",look_for,tau[ilast]);
          
           }
           else{
          found=true;
          if(ilast==0) {
             icheat=0;
            if(fabs(look_for-tau[0]) < .00001) map[i1]=0.0;
            else   map[i1]=-1;
          }
          else{
           icheat=ilast-1;
            map[i1]=ilast-1+(look_for+tau[ilast-1]-ilast+1.)/(-tau[ilast]+tau[ilast-1]+1.);
 icheat=ilast-1;
     //      fprintf(stderr,"MAPP %d %f \n",i1,map[i1]);
          }
        }
      }
  //  if(iii==1)  fprintf(stderr,"CHECK ASSIGN %d-i look_for=%f map=%f tau=%f icheat=%d\n",
  //        i1,look_for,map[i1],tau[icheat],icheat);
  }
// if(iii==1) seperr("");
 }
double flatten::nonlinear_iter(float *tau, float *dip){
  float f;

  long long ib,it,ishift=n1*n2*n3;
  int i1,i2,i3,i;
  for(i3=0; i3 < n3-1; i3++){
    for(i2=0; i2 < n2-1; i2++){
       ib=n1*i2+i3*n1*n2;
       for( i1=0; i1 < n1-1; i1++){
       //fprintf(stderr,"BEFORE ERROR %d %d %d\n",iloc,iloc2,iloc3);
          f=tau[i1+i2*n1+i3*n1*n2]-tau[i1+iloc]+i1;
          it=f; f-=it;
        //  fprintf(stderr,"XXX %d %d %d -%d \n",i1,i2,i3,it);
          if(it >=0 && it < n1-1){
            resid[ib+i1         ]=-eps_t*(tau[i1+ib+1]-tau[i1+ib]);
            resid[ib+i1+ishift*1]       =(1.-f)*dip[it+ib]       +f*dip[it+ib+1]+
                (tau[i1+ib]-tau[i1+ib+n1]);
            resid[ib+i1+ishift*2]=(1.-f)*dip[it+ib+ishift]+f*dip[ishift+it+ib+1]+
                (tau[i1+ib]-tau[i1+ib+n1*n2]);
          }
       }
     }
   }
   

  double dprod;
  
  
   for( i=0; i< n1*n2*n3; i++) temp[i]=0.;
   
   
   for( i3=0; i3 < n3-1; i3++){
     for( i2=0; i2 < n2-1; i2++){
       ib=n1*i2+i3*n1*n2;
       for(int i1=0; i1 < n1-1; i1++){
         temp[i1+ib+n1]+=resid[i1+ib+ishift];
         temp[i1+ib]+=-eps_t*resid[ib+i1]-resid[ib+i1+ishift]-resid[ib+i1+ishift*2];
         temp[i1+ib+1]+=eps_t*resid[ib+i1];
         temp[i1+ib+n1*n2]+=resid[i1+ib+ishift*2];
       }
     }
   }
   int ii;



  srite("uu.H",resid,n123*12);
  srite("uu.H",temp,n123*4);
 fftwf_execute(planF);
 
 for( i=0; i <ishift; i++) temp[i]=temp[i]*mult[i];
  fftwf_execute(planI);

//  srite("uu.H",temp,n123*4);
// seperr("");
   srite("uu.H",temp,n123*4);

   for( i=0; i< ishift; i++) tau[i]+=temp[i];   
   for( i=0; i< ishift*3;i++) dprod+=resid[i]*resid[i];
 srite("uu.H",tau,n123*4);
   return dprod;
}
double flatten::nonlinear_iter2(float *tau, float *dip){
  float f;


  calc_grad(tau,resid,&resid[n123],&resid[n123*2]);
  float dtau,otau;
  map_grad_tau(tau,dip,resid,&otau,&dtau);
 // fprintf(stderr,"DTAU CALC %f % f\n",otau,dtau);


   //Apply adjoint
   int ib;
   for( int i=0; i< n1*n2*n3; i++) temp[i]=0.;
   for( int i3=0 ;i3 < n3; i3++){
     for( int i2=0; i2 < n2; i2++){
       ib=n1*i2+i3*n1*n2;
       for(int i1=0;i1 < n1; i1++){
           //fprintf(stderr,"THE ERROR %d %d %d \n",i1,i2,i3);
         /*  if(i2>0){
             temp[i1+ib]+=resid[ib+i1+n123];
             temp[i1+ib-n1]-=resid[i1+ib+n123];
           }
           else{
             temp[i1+ib+n1]+=resid[ib+i1+n123];
             temp[i1+ib]-=resid[ib+i1+n123];
          }*/
                //     fprintf(stderr,"THE ERROR %d %d %d \n",i1,i2,i3);

          if(i1>0){
             temp[i1+ib]+=eps_t*resid[ib+i1];
             temp[i1+ib-1]-=eps_t*resid[i1+ib];
           }
           else{
             temp[i1+ib+1]+=eps_t*resid[ib+i1];
             temp[i1+ib]-=eps_t*resid[ib+i1];
          }
                   //  fprintf(stderr,"THE ERROR %d %d %d \n",i1,i2,i3);

          if(i3>0){
             temp[i1+ib]+=resid[ib+i1+n123*2];
             temp[i1+ib-n1*n2]-=resid[i1+ib+n123*2];
           }
           else{
             temp[i1+ib+n1*n2]+=resid[ib+i1+n123*2];
             temp[i1+ib]-=resid[ib+i1+n123*2];
          }
                 //    fprintf(stderr,"THE ERROR %d %d %d \n",i1,i2,i3);

  
       }
     }
   }
  // seperr("");
 //   srite("tt.H",resid,3*n1*n2*n3*4);
//    srite("tt.H",temp,n1*n2*n3*4);

  fftwf_execute(planF);
  double  dprod=0;
 //srite("tt.H",resid,n1*n2*n3*12);
  //srite("tt.H",mult,n1*n2*n3*4);

  for( int i=0; i <n123; i++) temp[i]=temp[i]*mult[i];
 // srite("tt.H",temp,n1*n2*n3*4);

 fftwf_execute(planI);
 srite("tt.H",temp,n1*n2*n3*4);
 add_map_tau(tau,temp,otau,dtau);
 srite("tt.H",tau,n1*n2*n3*4);
 //seperr("");
 double dp[3];
 dp[0]=dp[1]=dp[2]=0.;
  for( int i=0; i< n123;i++) {
    dp[0]+=resid[i]*resid[i];
    dp[1]+=resid[i+n123]*resid[i+n123];
    dp[2]+=resid[i+n123*2]*resid[i+n123*2];
  }
   fprintf(stderr,"CHECK DP %f %f %f \n",dp[0],dp[1],dp[2]);
   return dp[0]+dp[1]+dp[2];
}
double flatten::nonlinear_iter3(float *tau, float *dip){
  float f;


  
  float dtau,otau;
  fprintf(stderr,"SSS %d \n",n123);
  map_grad_tau(tau,dip,resid,&otau,&dtau);
 fprintf(stderr,"DTAU CALC %f % f\n",otau,dtau);
 int o=n1*n2*(n3-1);
 for(int i=0; i < n2*n1; i++) resid[i+o]=resid[i+n123+o]=resid[i+n123*2+o]=0.;
  for(int i=0; i < n2*n1; i++) resid[i]=resid[i+n123]=resid[i+n123*2]=0.;
 for(int j=0; j < n3; j++){
   o=j*n1*n2+(n2-1)*n1;
   for(int i=0; i < n1; i++){
     resid[i+o]=resid[i+o+n123]=resid[i+o+n123*2]=0.;
     resid[i+n1*n2*j]=resid[i+n1*n2*j+n123]=resid[i+n1*n2*j+n123*2]=0.;
   }
 }
 int k=0;
 for(int i3=0; i3 < n3; i3++){
  for(int i2=0; i2 <n2; i2++){
    for(int i1=0;i1<n1; i1++,k++){
      temp[k]=-(otau+dtau*i1);
    }
   }
  }
  fprintf(stderr,"XXX \n");
  
 srite("aa.H",temp,n123*4);
 apply_filt2(false,true,tau,resid);  
 // srite("tt.H",resid,3*n123*4);
 apply_filt2(true,false,temp,resid);
 //Now go back to tau space
   srite("tt.H",temp,n123*4);
  
//if(iii==0) {auxinout("bb.H");srite("bb.H",temp,n123*4);}
//else { sseek("bb.H",0,0); sreed("bb.H",temp,n123*4);}
/*
 for(int i=0;i < n123; i++) resid[i]=temp[i];
  int j=0,iloc;
  float  delta;
  for(int i2=0; i2 < n2*n3; i2++,j+=n1){
    for(int i1=0; i1 < n1; i1++){
      f=(-tau[i1+j]+i1-otau)/dtau;iloc=f; f-=iloc;
            if(iloc<0) {iloc=0; f=0;}
      else if(iloc>=n1-1){iloc=n1-2; f=1.;}
      delta=(1.-f)*resid[iloc+j]+f*resid[iloc+1+j];
    temp[i1+j]=delta;
    }
  }
  */
  /*
 if(iii==0) {auxinout("bb.H");srite("bb.H",temp,n123*4);}
else { sseek("bb.H",0,0); sreed("bb.H",temp,n123*4);}
 */
  //srite("tt.H",temp,n123*4);
  fftwf_execute(planF);
  double  dprod=0;
 //srite("tt.H",resid,n1*n2*n3*12);
  //srite("tt.H",mult,n1*n2*n3*4);

  for( int i=0; i <n123; i++) temp[i]=temp[i]*mult[i];
 fftwf_execute(planI);
 srite("tt.H",temp,n1*n2*n3*4);
 //for(int i=0; i < n123; i++) tau[i]=tau[i]-temp[i];
 add_map_tau(tau,temp,otau,dtau);
 srite("tt.H",tau,n1*n2*n3*4);
 //seperr("");
 double dp[3];
 dp[0]=dp[1]=dp[2]=0.;
  for( int i=0; i< n123;i++) {
    dp[0]+=resid[i]*resid[i];
    dp[1]+=resid[i+n123]*resid[i+n123];
    dp[2]+=resid[i+n123*2]*resid[i+n123*2];
  }
   fprintf(stderr,"CHECK DP %f %f %f \n",dp[0],dp[1],dp[2]);
   return dp[0]+dp[1]+dp[2];
}
double flatten::nonlinear_iter4(float *tau, float *dip, float *tdip){
  
  
   int ii=0;
   for(int i2=0; i2 < n2*n3; i2++){
     for(int i1=0; i1< n1; i1++,ii++) tau[ii]+=temp[ii]+i1;   
   }
   
  for(int i=0;i < 2*n123; i++){
    resid[i+n123]=dip[i]-tdip[i];
  }
  for(int i=0; i < n123; i++) resid[i]=0.;
    srite("junk.H",dip,n123*8);
     srite("junk.H",tdip,n123*8);
   srite("junk.H",resid,n123*12);
   for(int i=0; i <n123; i++) temp[i]=0.;
  float *bb=new float[n123*3];
  for(int i=0; i < n123*3; i++) bb[i]=0.;
  apply_filt(false,false,temp ,bb,tdip);  
    srite("junk.H",bb,n123*12);
for(int i=0; i < n123*3; i++) resid[i]+=bb[i];
srite("junk.H",resid,n123*12);
delete [] bb;
  apply_filt(true,false,temp,resid,tdip);
    srite("junk.H",temp,n123*4);

  fftwf_execute(planF);
      srite("junk.H",temp,n123*4);

  for(int i=0; i <n123; i++) temp[i]=temp[i]*mult[i];
  fftwf_execute(planI);
   srite("junk.H",temp,n123*4);
  
   ii=0;
   for(int i2=0; i2 < n2*n3; i2++){
     for(int i1=0; i1< n1; i1++,ii++) tau[ii]-=temp[ii]+i1;   
   }

  
  int j;

}
void flatten::create_bank(int nbank, float min, float max){
  float p,c1,c2,c3;
  delta=(max-min)/(nbank-1);
  for(int i=0; i< nbank; i++){
     p=min+delta*i;
     c1=(1.-p)*(2.-p)/12.;
     c2=(2.+p)*(2.-p)/6.;
     c3=(1.+p)*(2.+p)/12.;
     if(i==250) fprintf(stderr,"AT %f %f %f %f \n",c1,c2,c3,p);
     bank.push_back(filt(c1,c2,c3));
   }
   ofilt=min;
   fprintf(stderr,"SETTING UP %f %f \n",ofilt,delta);
}
void flatten::apply_filt(bool adj, bool add, float *mod, float *dat,float *tdip){

  int i,if1,if2;
  float der1,der2;
  if(!add){
    if(adj){ 
      for( i=0; i< n123; i++) mod[i]=0.;
    }
    else{
      for(i=0; i < n123*3;i++) dat[i]=0.;
   }
 }

  for(int i3=1; i3 < n3-1; i3++){
    for(int i2=1; i2 <n2-1; i2++){
      i=i2*n1+i3*n1*n2+1;

      for(int i1=1; i1 < n1-1; i1++,i++){
        
      
        if2=(tdip[i]-ofilt)/delta;
        if1=MAX(0,MIN(bank.size()-1,if2));
        der2=tdip[i+n123]; if2=(der2-ofilt)/delta;
        if2=MAX(0,MIN(bank.size()-1,if2)); 
     
        if(!adj){
        
          dat[i]+=eps_t*(mod[i]-mod[i-1]);

          dat[i+n123*2]+=(mod[i+1+n1*n2]-mod[i-1])*bank[if2].f[0]+
                        (mod[i  +n1*n2]-mod[i]  )*bank[if2].f[1]+
                        (mod[i-1+n1*n2]-mod[i+1]  )*bank[if2].f[2];
                    
                    dat[i+n123]+=(mod[i+1+n1]-mod[i-1])*bank[if1].f[0]+
                      (mod[i+n1]  -mod[i]  )*bank[if1].f[1]+
                      (mod[i-1+n1]-mod[i+1])*bank[if1].f[2];
  
        }
        else{
        //  mod[i-1]-=eps_t*dat[i];
        //  mod[i]+=eps_t*dat[i];
          
          mod[i+1+n1]+=dat[i+n123]*bank[if1].f[0];
          mod[i-1]   -=dat[i+n123]*bank[if1].f[0];
          mod[i+n1]  +=dat[i+n123]*bank[if1].f[1];
          mod[i]     -=dat[i+n123]*bank[if1].f[1];
          mod[i-1+n1]+=dat[i+n123]*bank[if1].f[2];
          mod[i+1]   -=dat[i+n123]*bank[if1].f[2];
          
          
          mod[i+1+n1*n2]+=dat[i+n123*2]*bank[if2].f[0];
          mod[i-1]      -=dat[i+n123*2]*bank[if2].f[0];
          mod[i+n1*n2]  +=dat[i+n123*2]*bank[if2].f[1];
          mod[i]        -=dat[i+n123*2]*bank[if2].f[1];
          mod[i-1+n1*n2]+=dat[i+n123*2]*bank[if2].f[2];
          mod[i+1]      -=dat[i+n123*2]*bank[if2].f[2];       
        }
    }
  }
  }
}

void flatten::apply_filt2(bool adj, bool add, float *mod, float *res){

  if(!add){
    if(adj){ 
      for( int i=0; i< n123; i++) mod[i]=0.;
    }
    else{
      for(int i=0; i < n123*3;i++) res[i]=0.;
   }
 }
 if(!adj){
   //take the gradient of the tau field in z 
  
   float *temp2=new float[n123*2];
   memcpy(temp2,&res[n123],n123*8);
   grad3(false,false,mod,res);
   //map gradient into tau
     srite("mm.H",temp2,n123*8);
          srite("mm.H",&res[n123],n123*8);

  convert_tau_lin(false,false,res,temp); memcpy(res,temp,n123*4);
   convert_tau_lin(false,false,&res[n123],temp); memcpy(&res[n123],temp,n123*4);
   convert_tau_lin(false,false,&res[n123*2],temp); memcpy(&res[n123*2],temp,n123*4);
      srite("junk.H",&res[n123],n123*8);
  
  for(int i=0; i < n123*2;i++){
      res[n123+i]+=temp2[i];
   }
   srite("mm.H",&res[n123],n123*8);
   delete [] temp2;
 }
 else{
   convert_tau_lin(true,false,temp,res); memcpy(res,temp,n123*4);
   convert_tau_lin(true,false,temp,&res[n123]); memcpy(&res[n123],temp,n123*4);
   convert_tau_lin(true,false,temp,&res[n123*2]); memcpy(&res[n123*2],temp,n123*4);
   grad3(true,false,mod,res);
 }


}
void flatten::grad3(bool adj, bool add, float *mod, float *dat){

  int i,if1,if2;
  int n123=n1*n2*n3;
  float der1,der2;
  if(!add){
    if(adj){ 
      for( i=0; i< n123; i++) mod[i]=0.;
    }
    else{
      for(i=0; i < n123*3;i++) dat[i]=0.;
   }
 }
  if(!adj) srite("tc.H",mod,n123*4);
  if(!adj) {
  srite("ff.H",&dat[n123],n123*8);
  }
 if(!adj) srite("tb.H",dat,n123*12);
 float *bb=new float[n123*2];
 for(int i=0; i <n123*2; i++) bb[i]=0.;
;
      
  for(int i3=1; i3 < n3; i3++){
    for(int i2=1; i2 <n2; i2++){
      i=i2*n1+i3*n1*n2+1;
      for(int i1=1; i1 < n1; i1++,i++){
        if(!adj){    
          dat[i]+=eps_t*(mod[i]-mod[i-1]);
      /*
          dat[i+n123*1] +=
          mod[i]-mod[i-n1];
          
          dat[i+n123*2]+=
          mod[i]-mod[i-n1*n2];
        */
                    
        }
        else{
          mod[i-1]-=eps_t*dat[i];
          mod[i]+=eps_t*dat[i];
          mod[i]+=dat[i+n123];
          mod[i-n1]-=dat[i+n123];
          mod[i]+=dat[i+n123*2];
          mod[i-n1*n2]-=dat[i+n123*2];
        }
    }
  }
}


  if(!adj) {
 
  srite("ff.H",bb,n123*8);
    srite("ff.H",&resid[n123],n123*8);
  }
 delete [] bb; 
  if(!adj) srite("tb.H",dat,n123*12);

}
void flatten::convert_tau_lin(bool adj, bool add, float *mod, float*dat){
  if(!add){
    if(adj){ 
      for( int i=0; i< n123; i++) mod[i]=0.;
    }
    else{
      for(int i=0; i < n123;i++) dat[i]=0.;
   }
 }
 int i;
 float f;
 int j;
 for(int i2=0; i2 <n3*n2;i2++){
  for(int i1=0; i1 < n1; i1++){
     f=gmap[i1+i2*n1]; 
     if(f>-.5){
       i=f; f-=i;
       if(!adj){
         dat[i1+i2*n1]=(1.-f)*mod[i+i2*n1]+f*mod[1+i+i2*n1];
       }
       else{
         mod[i+i2*n1]+=(1.-f)*dat[i+i2*n1];
         mod[i+i2*n1+1]+=f*dat[i+i2*n1+1];
       }
       
     }
    }
   }


}
void flatten::create_mult(){
  float *f1=new float[n1];
  float *f2=new float[n2];
  float *f3=new float[n3];
  double pi=acos(-1.)*2.;
  float et2=eps_t*eps_t;


    float in1=1./(n1*2.),in2=1./(n2*2.), in3=1./(n3*2.);
    
    for(int i=0; i <n1; i++) f1[i]=in1*i;
    for(int i=0; i <n2; i++) f2[i]=in2*i;
    for(int i=0; i <n3; i++) f3[i]=in3*i;
 
    //fprintf(stderr,"scale %d \n",8*n1*n2*n3);
    int ib=0;
    float k1,k2,k3,ct1,ct2,ct3,ct4,ct5,ct6,ct7,tmp;
    for(int i3=0; i3 < n3; i3++){
      ct3=-cos(f3[i3]*pi)-cos(-f3[i3]*pi)+4 +2.*et2;
      for(int i2=0; i2 < n2; i2++){
        ct2=-cos(f2[i2]*pi)-cos(f2[i2]*pi);    
        for(int i1=0; i1 < n1; i1++,ib++){
          tmp=ct3+ct2-et2*(cos(-f1[i1]*pi)+cos(f1[i1]*pi));
          if(i1==1 && i2==1 && i3==1) fprintf(stderr,"CHECK MULT %f %g \n",tmp,in1*in2*in3);
          if(fabs(tmp)>.000001) mult[ib]=in1*in2*in3*tmp/(tmp*tmp+.000000001);
          else mult[ib]=0.;
          
         //temp[ib]=.125*in1*in2*in3*tmp;
        }
      }
    }
//   srite("tt.H",mult,n1*n2*n3*4);
    /*
     fftwf_execute(planF);
     for(int i1=0; i1 < fix.size(); i1++){
       temp[fix[i1].i1+fix[i1].i2*n1+fix[i1].i3*n1*n2]=0.;
     }
     fftwf_execute(planI); 
     */
//     for(int i1=0; i1 < n1*n2*n3; i1++) muli=//.125*in1*in2*in3*
 //      (temp[i1]/(temp[i1]*temp[i1]+.000001));

    //srite("aa.H",mult,n1*n2*n3*4);
    delete [] f1; delete []f2; delete [] f3;
  }
