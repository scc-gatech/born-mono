#include "nmo.h"
#include <math.h>
#include "stdio.h"
#include "assert.h"
#include "seplib.h"

//Initialize with trace info
nmo::nmo(int n_t, float d_t, float t_0, int n_s, float d_s, float o_s){
   nt=n_t; dt=d_t; ot=t_0;
   ns=n_s; ds=d_s; os=o_s;
   sinc=new sinc_bob(1000,8);
 }


//Doesn't do stretch mute....for now
 void nmo::nmo_it(float *s2, float h, float *in, float *out){
   float h2=h*h;
   float t=ot;
   float j=0;
   for(int it=0; it < nt; it++,t+=dt){
     float t0=(sqrtf(t*t+h2*s2[it])-ot)/dt;
     int i0=(int) t0; 
     if(it < nt-2){
      float f0=t0-i0;
      float *table=sinc->return_tab(f0);
      out[it]=0;
      int j;
      if(i0<3){
        for(int k=0; k < 8; k++){
          j=it+k-3;
          if(j<0) j=0;
          out[it]+=in[j]*table[k];
        }
      }
      else if(i0>nt-5){
       for(int k=0; k< 8; k++){
          j=it+k-3;
          if(j>nt-1) j=nt-1;
          out[it]+=in[j]*table[k];
        }
      }
      else{
        for(int k=0, j=it-3; k < 8;  k++,j++)
          out[it]+=in[j]*table[k];
      }
    }
   }
 }
 


 void nmo::dnmo_it(float *s2, float hin, float hout,float *in, float *out){
   
   float h2in=hin*hin;
   float h2out=hout*hout;
   int iz=0;
   
   //Loop over output nmo time
   float tn=ot;
   float tnew=sqrt((ot+dt*iz)*(ot+dt*iz)+h2out*s2[iz]);
   float told=tnew;
   float tz=ot;
   float valo=0;
   for(int it=0; it < nt; it++, tn+=dt){
     while(tnew < tn && iz < ns-1){
       iz+=1; told=tnew; tz+=ds;
       tnew=sqrt(tz*tz+h2out*s2[iz]);
     }
     if(iz==0){ //Edge condition 
       out[it]=in[0];
         float val=0;
       srite("map.H",&val,4);
     }
//     else if(iz==nt-1){//Edge condition maybe should do a check and zero?
 //     out[it]=0.;
  //      float val=nt-1;
   //  }
     else{
       float t_old=(sqrt((tz-ds)*(tz-ds)+h2in*s2[iz-1]));
       float t_new=(sqrt(tz*tz         +h2in*s2[iz]));
       float sold=((sqrt((tz-ds)*(tz-ds)+h2in*s2[iz-1])-ot)/dt);
       float snew=((sqrt(tz*tz         +h2in*s2[iz])-ot)/dt);
       
       //This is approximating with linear interpolation something
       //that isn't linear...probably okay? Taylor expansion?
       float f=(tn-told)/(tnew-told);
       if(f>=1.) f=.9999;
       float *table=sinc->return_tab(f);
       float val=(1.-f)*sold+f*snew;
       float junk=val-valo;
       srite("map.H",&junk,4);
       
       int isamp=(int)val; 
 
       
       f=val-isamp;
       int j;
       out[it]=0.;
fprintf(stderr,"CHECK ME %d %f %d %f %f %f\n",it,val,iz,sold,snew,tz);
   
       if(isamp<3){

          for(int k=0; k < 8; k++){
            j=isamp+k-3;
            if(j<0) j=0;
            out[it]+=in[j]*table[k];
          }
        }
        else if(isamp>nt-5 ){

          for(int k=0; k< 8; k++){
            j=isamp+k-3;
            if(j>nt-1) j=nt-1;

            float x=in[j];
            float y=table[k];
          
            out[it]+= x*y;
          }
        }
        else{
          for(int k=0, j=isamp-3; k < 8;  k++,j++){
            out[it]+=in[j]*table[k];
//fprintf(stderr,"CCC %d=%f %d=%f %d=%f \n",it,out[it],j,in[j],k,table[k]);
            }
        }
       
     //  out[it]=in[isamp];
     }
   }
   srite("xx.H",out,nt*4);
  assert(1==0);
  }
  
    
 void nmo::dnmo2_it(float *s2, float hin, float hout,float *in, float *out){
   
   float h2in=hin*hin;
   float h2out=hout*hout;   
   for(int it=0; it < nt; it++) out[it]=0;
   float ts_old=os;
   float tout_old=sqrt(ts_old*ts_old+s2[0]*h2out);
   float tin_old =sqrt(ts_old*ts_old+s2[0]*h2in);
   for(int is=1; is < ns; is++){
     float ts_new=ts_old+ds;
     float tout_new=sqrt(ts_new*ts_new+s2[is]*h2out);
     float tin_new =sqrt(ts_old*ts_old+s2[is]*h2in);
     int  ibeg=(int)ceilf((tout_old-ot)/dt);
     int  iend=(int)floor((tout_new-ot)/dt);
     fprintf(stderr,"CHECK IT %d %d %d \n",is,ibeg,iend);
     float f=(ibeg*dt+ot-tout_old)/(tout_new-tout_old);
     float df=dt/(tout_new-tout_old);
     for(int it=ibeg; it <=iend; it++,f+=df){
     float fract=((1.-f)*tin_old+f*tin_new-ot)/dt;
     int isamp=(int)fract; fract-=isamp;
     float *table=sinc->return_tab(fract);
     int j;
     if(isamp<nt){ 
       if(isamp<3){

          for(int k=0; k < 8; k++){
            j=isamp+k-3;
            if(j<0) j=0;
            out[it]+=in[j]*table[k];
          }
        }
        else if(isamp>nt-5){

          for(int k=0; k< 8; k++){
            j=isamp+k-3;
            if(j>nt-1) j=nt-1;
            out[it]+= in[j]*table[k];
          }
        }
        else{
          for(int k=0, j=isamp-3; k < 8;  k++,j++){

            out[it]+=in[j]*table[k];

            }
        }
       
     //  out[it]=in[isamp];
     }
    }
     tout_old=tout_new; tin_old=tin_new; ts_old=ts_new;
   }
   //assert(1==0);
  }
void nmo::dnmo3_it(float *s2, float hin, float hout,float *in, float *out){
   
	float h2in=hin*hin;
	float h2out=hout*hout;   
	for(int it=0; it < nt; it++) out[it]=0;

	float ts_old=os;
	float tout_old;
	float tin_old;

	float t_diff;
	float ts_new;
	float ts_new2, ts_old2;
	float f, df, f_start;
	int ibeg, iend;
	int ibegs[ns], iends[ns];
	float f_starts[ns], dfs[ns];
	float tin_news[ns];

	float tout_new, tin_new;
	int out_is = 0;


    //Loop over subsampled slowness (~100ms???)
	for(int is=0; is < ns; is++){	
		tout_old= tout_new; // prev
		ts_new = os + ds*is;
		ts_new2 = ts_new * ts_new; // ts_new squared
		tout_new = sqrt(ts_new2+s2[is]*h2out);        //Calculate travel time to new offset
		tin_news[out_is] =sqrt(ts_new2+s2[is]*h2in);  //Calculate travel time to old offset

        //Calculate range of output samples between last slowness and this slowness
		ibegs[out_is] = (int)ceilf((tout_old-ot)/dt);
		iends[out_is] = (int)floor((tout_new-ot)/dt);
		
		//Deal with the end condition (Expecting slowness padded by 1 to handle edge)
        if(iends[out_is]>= nt) iends[out_is]=nt-1;
	
	    //We will use linear interpolation to map input times to output times
	    //this isn't ideal and assuming a linear trend between samples would be
	    //better (and doable)
	    t_diff = tout_new - tout_old;
		f_starts[out_is] = (ibegs[out_is] *dt+ot-tout_old)/t_diff;
		dfs[out_is] = dt/t_diff;
	 
	    
		if (is>=1 /* explicit latency */ && ibegs[out_is] <= iends[out_is]) { // valid point, commit to output stream
			out_is++;
		}
	}

	int is = 0;
	ibeg = -1, iend=-1;
	
	//Loop over out put time
	for(int it=0; it < nt; it++){
			
			//If we have moved to the next slowness pair
			if (it > iend) {
				ibeg = ibegs[is];
				iend = iends[is];
				f_start = f_starts[is];
				df = dfs[is]; 
				tin_new = tin_news[is];
				tin_old = tin_news[is-1];
				is++;
			}

			//Needed for streaming rather than moving outside
			if (it >= ibeg && it <= iend) {
			
			    //Calculate the sample to grab
				f = f_start + df*(it-ibeg);
				float fract=((1.-f)*tin_old+f*tin_new-ot)/dt;
				int isamp=(int)fract; 
				fract = fract-isamp;
printf("getting sinc for %f (isamp=%d) at it=%d\n", fract, isamp, it);
				
				//Do sinc interpolation
				float *table=sinc->return_tab(fract);
                if(isamp<nt){ 
                int j;
                if(isamp<3){ 
       

                 for(int k=0; k < 8; k++){
                   j=isamp+k-3;
                   if(j<0) j=0;
                   out[it]+=in[j]*table[k];
                 }
               }
               else if(isamp>nt-5){
                for(int k=0; k< 8; k++){
                  j=isamp+k-3;
                  if(j>nt-1) j=nt-1;
                  out[it]+= in[j]*table[k];
                }
               }
              else{
                for(int k=0, j=isamp-3; k < 8;  k++,j++){
                  out[it]+=in[j]*table[k];
                }
             }
		   }
		}
	}
}
