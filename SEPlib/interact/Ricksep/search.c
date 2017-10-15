#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
#include <stdio.h>
#include "rick.h"
#include<math.h>

#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif
#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif




void wshsjmm(int *iout, int *iin,int imin,int imax,int n);
void wshszero(float *y, int n);


/* THIS IS A TRANSLATION OF BILL HARLAN'S pickit routine */
/*
C TRACE A COHERENT PATH THROUGH THE DATA, WITH FIXED POINTS
C R1PATH(N2) GIVES THE BEST TIMES FOR EACH COLUMN
C O1,D1,O2,D2 ARE SAMPLING RATES FOR DTA(N1,N2)
C NAILS FIX THE PATH AT COORDINATES (R1NAIL(NNAIL), N2NAIL(NNAIL))
C   R1PATH((R2NAIL(INAIL)-O2)/D2 + 1) = R1NAIL(INAIL) FOR INAIL=1,NNAIL
C TJUMP IS THE MAXIMUM DISTANCE IN TIME TO JUMP WHEN PASSING FROM
C   ONE COLUMN TO THE NEXT
C TWIDE IS THE DISTANCE OVER WHICH TO EVALUATE CORRELATIONS
C WRK(NWRK) IS A SCRATCH ARRAY; SET NWRK >= 2*N1*N2 + 3*N1
C NOTE: R1NAIL AND R2NAIL GET TURNED INTO GARBAGE, TO SAVE MEMORY
C YOU MUST GIVE AT LEAST ONE NAIL (TO SAVE TROUBLE IN DO LOOPS)
*/
 
/*
  TRACE A PATH THROUGH THE DATA FROM DTA(.,1) TO DTA(.,N2), PASSING
    THROUGH CERTAIN PICKED POINTS.
  J1PATH(N2) GIVES THE BEST TIME SAMPLES FOR EACH COLUMN
    J1PATH(I2NAIL(INAIL)) = I1NAIL(INAIL) FOR INAIL=1,NNAIL
  JJUMP IS THE MAXIMUM DISTANCE IN TIME (N1) TO JUMP WHEN PASSING FROM
    ONE COLUM TO THE NEXT
  JWIDE IS THE MAXIMUM DISTANCE IN TIME (N1) OVER WHICH TO EVALUATE
     CORRELATIONS
   I1PATH AND COST ARE SCRATCH ARRAYS FOR INTERNAL WRK
  I1PATH: VALUE IS STATE (TIME SAMPLE); 1ST ARG: STEPS TRACED (COLUMNS);
      2ND ARG: LAST STATE (TIME) IN SEQUENCE; 3RD, OLD OR NEW SEQUENCE.
  COST: VALUE IS TOTAL COST IN SEQUENCE (TO BE MAXIMIZED);
    ARGS: CORRESPOND TO 2ND AND 3RD OF I1PATH;
*/

		void wshspickr(int *j1path, float *dta,int n1,int n2,int *i1nail,int *i2nail,int nnail, int jwide,int jjump){

      int jold,jnew,jtemp,i1old,i1new,i2old,i2new;
      int i1dif,i1,i2,i1best,i,i1a,i1b,i2a,i2b,inail;
      float bad,good,scale,r;
			int lyes;
			float *cost;
			int *i1path;
			double d1,d2,d3;


			NEW(float*,cost,n1*3);
			NEW(int*,i1path,n1*n2*2);
			/*ADD BIG PENALTIES AND BONUSES FOR BIG JUMPS AND NAILS*/
      good = 2.*(float)(n2);
      bad = - good;
			/* initialize indices for old and new paths*/
      jnew = 0;jold = 1;jtemp = 2;

			for(i=0; i < n1*n2*2;i++) i1path[i]=1;

			/*INITIALIZE COST FUNCTION*/
      wshszero(cost,n1*3);

			/*ITERATIVELY FIND NEW PATHS FOR SUCCESSIVE COLUMNS (STEPS)*/
		for(i2new=0; i2new < n2; i2new++){
			/*I2OLD IS THE PREVIOUS COLUMN*/
      i2old = MAX(0, i2new - 1);

			/* swap addresses for old and new pathS */
      i = jnew;jnew = jold;jold = i;

			/*find which nails are closest on either side*/
      i2a = -1;i2b = -1;
			for(inail=0; inail < nnail; inail++){

				/*C FIND NAIL TO LEFT*/
    	  if(i2nail[inail] <= i2new && (i2nail[inail]>i2a || i2a== -1)){
     		 	i2a = i2nail[inail];
     		 	i1a = i1nail[inail];
				}
     		if(i2nail[inail] >= i2new && (i2nail[inail]<i2b || i2b== -1)){
     		 	i2b = i2nail[inail];i1b = i1nail[inail];
				}
			}
			/*C SEE IF ALL NAILS TO ONE SIDE*/
      if(i2a == -1) i2a = i2b;
      if(i2b== -1) i2b = i2a;

			/*C SCALE IS THE FRACTIONAL DISTANCE BETWEEN NAILS*/
      scale = 0.5;
      if(i2b!=i2a) scale = (float)(i2new - i2a)/(float)(i2b - i2a);

			/*C LOOK AT ALL NEW STATES (TIMES)*/
			for(i1new=0;i1new<n1; i1new++){	
				/*C LOOK AT ALL PREVIOUS STATES AND THE COST OF MOVING FROM THERE*/
				for(i1old=0;i1old<n1; i1old++){	
					/*C SEE HOW FAR NEW STATE (TIME) IS FROM OLD*/
      		i1dif = i1new - i1old;if(i1dif<0) i1dif=-i1dif;
					if(i1dif > jjump){
						/*C IF OUTSIDE REASONABLE DISTANCE, GIVE HUGE PENALTY*/
      			cost[i1old+n1*jtemp] = cost[i1old+n1*jtemp] + bad;
/*					  fprintf(stderr,"check this %d %d %f \n",i1old,jtemp, cost[i1old+n1*jtemp]);*/
					}
					else{
						/*WITHIN REASONABLE DISTANCE*/
					/* CALCULATE CROSS CORRELATION, 
              MAX/MIN = +/-1.,FOR INCREMENTAL COST*/
				
      			cost[i1old+n1*jtemp]=wshspickrc(&dta[n1*i2new],
              &dta[n1*i2old],i1new,i1old, n1,jwide);
/*					  fprintf(stderr,"check this %d %d %f \n",i1old,jtemp, cost[i1old+n1*jtemp]);*/
					/*ADD IN CORRELATION WITH FIRST NAIL: MAX,MIN = +/- (1. - SCALE)*/
    		   cost[i1old+n1*jtemp] +=wshspickrc(&dta[n1*i2new],
            &dta[n1*i2a],i1new,i1a,n1,jwide);
/*					  fprintf(stderr,"1heck this %d %d %f \n",i1old,jtemp, cost[i1old+n1*jtemp]);*/
		
					/*ADD IN CORRELATION WITH SECOND NAIL:  MAX,MIN = +/- SCALE*/
 		      cost[i1old+n1*jtemp]+=wshspickrc(&dta[n1*i2new],
            &dta[n1*i2b],i1new,i1b,n1,jwide);
/*					  fprintf(stderr,"2heck this  %d %d %d %f %f %d %d %d  %f \n",i2new,i2b,i1b,dta[n1*i2new+i1new],dta[n1*i2b+i1b],*/
/*i1old,jtemp, cost[i1old+n1*jtemp]);*/
					/*PENALIZE FOR MOVING AWAY FROM PREVIOUS STEP (MIN = -1., MAX = 0).*/

					/*PUTS A TAUTNESS ON THE PATH.*/
 		     cost[i1old+n1*jtemp]-=(float)(i1dif)/(float)(jjump);
/*					  fprintf(stderr,"3heck this %d %d %f \n",i1old,jtemp, cost[i1old+n1*jtemp]);*/
					/* ADD COST OF PREVIOUS PATH*/
 		     cost[i1old+n1*jtemp] = cost[i1old+n1*jtemp] + cost[i1old+n1*jold];
					  fprintf(stderr,"4heck this %d %d %f \n",i1old,jtemp, cost[i1old+n1*jtemp]);
				}
			}

	/*C NOW SEE WHICH PREVIOUS STATE COSTS MOST (WE LIKE TO SPEND)*/
   i1best = 0;
	 for(i1old=0; i1old < n1; i1old++){
      if(cost[i1old+n1*jtemp]>cost[i1best+n1*jtemp]) i1best = i1old;
					  fprintf(stderr,"5heck this %d %d %f %f\n",i1old,jtemp, cost[i1old+n1*jtemp],cost[i1best+n1*jtemp]);
		}

		 
		for(i2=0;i2< i2old;i2++) {
      i1path[i2+n2*i1new+n1*n2*jnew] = i1path[i2+n2*i1best+n2*n1*jold];
			fprintf(stderr,"ia1path %d %d %d %d \n",i2,i1new,jnew,i1path[i2+n2*i1new+n1*n2*jnew]);
		}

		/*ADD NEW STATE IN PATH, THIS NEW STEP*/
    i1path[i2new+n2*i1new+n1*n2*jnew] = i1new;
		fprintf(stderr,"up 1 %d \n",i1new);

		/*C UPDATE COST FUNCTION WITH BEST VALUE*/
     cost[i1new+n1*jnew] = cost[i1best+n1*jtemp];
	}
	/*PUT IN BONUS FOR NAILS*/
  if(i2new==i2a) cost[i1a+n1*jnew] += good;
  if(i2new==i2b) cost[i1b+n1*jnew] += good;
	/*	ENDDO (NEXT I2NEW, NEW STEP, COLUMN)*/
	}
	/*C SEE WHICH PATH COSTS MOST (WE LIKE TO SPEND)*/
  i1best = 0;
	for(i1=0; i1 < n1; i1++){
      if(cost[i1+n1*jnew]>cost[i1best+n1*jnew]) i1best = i1;
	}
	/*COPY BEST PATH TO OUTPUT ARRAY*/
	for(i2=0; i2 < n2; i2++){
		j1path[i2] = i1path[i2+i1best*n2+jnew*n1*n2];
		fprintf(stderr,"check path %d %d %d %d \n",i2,j1path[i2],i1best,jnew);
	}
	free(i1path); free(cost);
}





 
/*
  CROSS PRODUCT OF DA(N) WITH DB(N), SCALE LIKE SEMBLANCE
                    SUM  DA(IA + J) DB(IB + J)
  COEF =  ---------------------------------------
         SQRT(SUM DA(IA + J)**2) SQRT(SUM DB(IB + J)**2)
 
               SUMS ARE OVER ( -JWIDE <= J <= JWIDE)
      -1 <= COEF <= 1
   USE TRIANGLE WEIGHTS IN SUMS
*/

float wshspickrc(float *da,float *db,int ia,int ib,int n,int jwide){
	int jamin,jamax,jbmin,jbmax,ja,jb;
	float  scalea,scaleb,r,sqrtp,coef;
	double d1,d2,d3;
/*C DO NOT RUN OFF ENDS OF TRACE*/
      jamin = MAX(0,ia-jwide);jamax = MIN(n-1,ia+jwide);
      jbmin = MAX(0,ib-jwide);jbmax = MIN(n-1,ib+jwide);
			/*C GET POWER OF WINDOW IN DA*/
      scalea = 0.;

			for(ja=jamin; ja <= jamax; ja++){
				/*C USE TRIANGLE WEIGHT*/
				d1=ja - ia;d2=jwide + 1;d3=d1/d2;
      	r = 1. - fabs(d3);
      	scalea = scalea + r*da[ja]*da[ja];
			}
      scalea = sqrt(scalea);
			/*GET POWER OF WINDOW IN DB*/

      scaleb = 0.;
			for(jb=jbmin; jb <= jbmax; jb++){
				/*USE TRIANGLE WEIGHT*/
				d1=jb - ib;d2=jwide + 1;d3=d1/d2;
      	r = 1. - fabs(d3);
      	scaleb = scaleb + r*db[jb]*db[jb];
			}

      scaleb = sqrt(scaleb);
      coef = 0.;
			for(ja=jamin; ja <= jamax; ja++){
      	jb = ja - ia + ib;
      	if(jb>=jbmin && jb<=jbmax){
					/* USE TRIANGLE WEIGHT*/
					d1=ja - ia;d2=jwide + 1;d3=d1/d2;
     		 	r = 1. - fabs(d3);
     		 	coef = coef + r*da[ja]*db[jb];
				}
			}
    
  /*  FACTOR OF 1.01 IS NECESSARY IN CASE SCALEA*SCALEB=COEF, BUT
       NEITHER IS 0 (UNLIKELY) */
      r = 1.01*scalea*scaleb;

			/*  check for zero energy*/
      if(r< coef){
				return(0);
			}

			
			/*SCALE CROSS PRODUCT
			SCALE COEFFICIENT AND TAKE SIGNED SQUARE ROOT */
      r = coef/r;
      coef = sqrt(fabs(r));
			/* sign(x,y) = abs(x)sgn(y) */
			if(r>0) coef=coef;
			else coef=-coef;
			return(coef);
}
 
 
/*C CLIP AN INTEGER ARRAY WITH MIN AND MAX*/
void wshsjmm(int *iout, int *iin,int imin,int imax,int n){
			int i;
		for(i=0; i< n; i++) iout[i]=MIN(MAX(iout[i],imin),imax);
		return;
}

/*C ZERO AN ARRAY OF REAL NUMBERS Y(N).*/
 void wshszero(float *y, int n){
		int i;
		for(i=0; i< n; i++) y[i]=0.;
}

#define AR_2D(array,i1,i2) array[(i1-1)+(i2-1)*n1]
#define AR_1D(array,i1) array[(i1-1)]



/*from Lin Zhang's Paper SEP71??*/

 void vit_min(int n1,int n2, int npath,float *in, int *out){

/*
!
!  ------------------------------------------------------------------------
!   find the shortest-path with viterbi algorithm
!  ------------------------------------------------------------------------
!
! n1,n2       dimension of the array
! npath       number of allowed paths
! in        input array
! qqsum,qqmin     auxiliary array
! out       output array
!
!  ------------------------------------------------------------------------
!
*/
  int i1,i2;
  int ipath;
  int i2min,i2max,npathh,ipathmin,ipathmax;
  int i2pick,i2newpick,i;
  float minvalue;
  float  *qqsum,*qqmin;


  npathh=npath/2;

  NEW(float*,qqsum,n1*n2);
  NEW(float*,qqmin,n2);
  for(i=0; i < n1*n2;i++) qqsum[i]=0.;
  for(i=0; i < n2;i++) qqmin[i]=0.;

  for(i2=1; i2 <= n2; i2++) AR_2D(qqsum,1,i2)=AR_2D(in,1,i2);
  for(i1=2; i1 <= n1; i1++){
    for(i2=1; i2 <= n2; i2++) AR_1D(qqmin,i2)=10000000.;
    for(ipath=-npath; ipath <= npath; ipath++){
      i2min = MAX(1,1-ipath);
      i2max = MIN(n2,n2-ipath);
      for(i2=i2min; i2<= i2max; i2++){
        AR_1D(qqmin,i2)=MIN(AR_1D(qqmin,i2),AR_2D(qqsum,i1-1,i2+ipath));
       }
    }
    for(i2=1; i2 <= n2; i2++)
        AR_2D(qqsum,i1,i2)=AR_1D(qqmin,i2)+AR_2D(in,i1,i2);
  }

/*!    backward tracing*/
  i1 = n1;
  i2pick = (n2-1)/2;
  minvalue = AR_2D(qqsum,i1,i2pick);
  for(i2=1; i2 <= n2; i2++){
    if(AR_2D(qqsum,i1,i2) < minvalue){
        i2pick = i2;
        minvalue =  AR_2D(qqsum,i1,i2pick);
      }
  }
  AR_1D(out,i1)=i2pick;
  for(i1=n1-1; i1>=1; i1--){
    minvalue = 100000000;
    if(AR_2D(qqsum,i1,i2pick) <= minvalue){
      minvalue=AR_2D(qqsum,i1,i2pick);
      i2newpick = i2pick;
     }
     ipathmin = MAX(-npathh,1-i2pick);
     for(ipath=ipathmin;ipath<=-1;ipath++){
      if(AR_2D(qqsum,i1,i2pick+ipath) < minvalue){
        minvalue=AR_2D(qqsum,i1,i2pick+ipath);
   i2newpick = i2pick+ipath;
        }
      }
     i2pick = i2newpick;
      AR_1D(out,i1)=i2pick;
  }

  free(qqsum); free(qqmin);
}


 void vit_max(int n1,int n2, int npath,float *in, int *out){
/*
c
c  ------------------------------------------------------------------------
c   solve the nonlinear maximization by viterbi algorithm
c  ------------------------------------------------------------------------
c
c n1,n2       dimension of the array
c npath       number of allowed paths
c in        input array
c out       output array
c
c  ------------------------------------------------------------------------
c
*/
  int i,i1,i2;
  int ipath;
  int i1min,i1max,npathh,ipathmin,ipathmax;
  int i1pick,i1newpick;
  float maxvalue;
  float  *qqsum,*qqmax;


  npathh=npath/2;

  NEW(float*,qqsum,n1*n2);
  NEW(float*,qqmax,n1);
  for(i=0; i < n1*n2;i++) qqsum[i]=0.;
  for(i=0; i < n1;i++) qqmax[i]=0.;

  npathh = (npath-1)/2;
/*
c VITERBI ALGORITHM
c    forward accumulation
*/
	for(i1=1;i1 <= n1; i1++) AR_2D(qqsum,i1,1) = AR_2D(in,i1,1);
	for(i2=2;i2 <= n2; i2++){
		for(i1=1;i1 <= n1; i1++){
        AR_1D(qqmax,i1) = -1.e+30;
		}
		for(ipath=-npathh; ipath <=npathh; ipath++){
        i1min = MAX(1,1-ipath);
        i1max = MIN(n1,n1-ipath);
				for(i1=i1min; i1 <=i1max; i1++){
           AR_1D(qqmax,i1) = MAX(AR_1D(qqmax,i1),AR_2D(qqsum,i1+ipath,i2-1));
				}
		}
		for(i1=1;i1 <= n1; i1++){
        AR_2D(qqsum,i1,i2) = AR_1D(qqmax,i1)+AR_2D(in,i1,i2);
		}
	}

/*c    backward tracing*/
  i2 = n2;
  i1pick = (n1-1)/2;
  maxvalue = AR_2D(qqsum,i1pick,i2);
	for(i1=1;i1 <= n1; i1++){
     if(AR_2D(qqsum,i1,i2) > maxvalue){
        i1pick = i1;
        maxvalue = AR_2D(qqsum,i1pick,i2);
			}
	}
  AR_1D(out,i2) = (float)(i1pick);
	for(i2=n2-1; i2 >=1 ; i2--){
     maxvalue = -1.e+30;
     if(AR_2D(qqsum,i1pick,i2) >= maxvalue){
        maxvalue = AR_2D(qqsum,i1pick,i2);
        i1newpick = i1pick;
			}
     ipathmin = MAX(-npathh,1-i1pick);
		 for(ipath=ipathmin; ipath <= -1; ipath++){
        if(AR_2D(qqsum,i1pick+ipath,i2) > maxvalue){
           maxvalue = AR_2D(qqsum,i1pick+ipath,i2);
           i1newpick = i1pick+ipath;
				}
			}
     ipathmax = MIN(npathh,n1-i1pick);
		 for(ipath=1; ipath <= ipathmax; ipath++){
        if(AR_2D(qqsum,i1pick+ipath,i2) > maxvalue){
           maxvalue = AR_2D(qqsum,i1pick+ipath,i2);
           i1newpick = i1pick+ipath;
				}
			}
     i1pick = i1newpick;
     AR_1D(out,i2) = (float)(i1pick);
	}
  free(qqsum); free(qqmax);

}


#endif
