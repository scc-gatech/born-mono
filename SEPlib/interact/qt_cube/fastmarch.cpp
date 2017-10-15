#define INFINITY 1.e+20
#define FMM_OUT '\0'
#define FMM_IN 'i'
#define FMM_FRONT 'f'
#define CVTIME(p1,p2,p3) (slow0*hypot(hypot((p1)*dd1-s1,(p2)*dd2-s2),(p3)*dd3-s3))
#include "fastmarch.h"
#include <math.h>
#include <stdio.h>
int fastmarch::grid (int i, int n)
{
  if (i >=0 && i < n) return i;
  if (i < 0)          return 0;
  return (n-1);
}

void fastmarch::delete_fastmarch(){
  if(mask!=0) delete [] mask;
  mask=0;
  if(myq!=0) delete myq;
  myq=0;
}
void fastmarch::alloc_local(){

 /* should probably check the return value of malloc */
  mask  =new unsigned char[n];

  /* nh is an estimate of the maximum front size */
  int nh = 0;
  if (n1 > 1) nh += 2*n2*n3;
  if (n2 > 1) nh += 2*n1*n3;
  if (n3 > 1) nh += 2*n1*n2;
  stillconstant = 1;
  myq=new pqueue (nh);
    /* precompute some of the finite-difference coefficients */
  dd[0] = 0.;
  dd[1] = d1;
  dd[2] = d2;
  dd[4] = d3;

  dd1=d1;
  dd2=d2;
  dd3=d3;

  d1 = 1./(d1*d1);
  d2 = 1./(d2*d2);
  d3 = 1./(d3*d3);  

  dd[3] = 1./sqrt(d1+d2);
  dd[5] = 1./sqrt(d1+d3);
  dd[6] = 1./sqrt(d2+d3);

  dd[7] = 1./sqrt(d1+d2+d3);
}
void fastmarch::setup_run(float *ttime){

  for (int i=0; i<n; i++) {
    ttime[i] = INFINITY;
    mask[i] = FMM_OUT;
  }

}
void fastmarch::init_source(float s1, float s2, float s3,float *ttime){

  int i1,i2,i3;
  int start1,start2,start3,end1,end2,end3;
  double delta1,delta2,delta3;
  setup_run(ttime);
 /* initialize source */
  i1 = (int) (0.5 + s1/d1); start1 = grid(i1-b1,n1); end1 = grid(i1+b1,n1);
  i2 = (int) (0.5 + s2/d2); start2 = grid(i2-b2,n2); end2 = grid(i2+b2,n2);
  i3 = (int) (0.5 + s3/d3); start3 = grid(i3-b3,n3); end3 = grid(i3+b3,n3);
  int i;
  nm = n;
  for (i3 = start3; i3 <= end3; i3++) {
    for (i2 = start2; i2 <= end2; i2++) {
      for (i1 = start1; i1 <= end1; i1++) {
	nm--;
	delta1 = s1-i1*d1; 
	delta2 = s2-i2*d2;
	delta3 = s3-i3*d3; 
 i = i1+n1*i2+n12*i3;
	  float *pt;

	pt=ttime+i;  
	*pt = slow0 * sqrt(delta1*delta1+delta2*delta2+delta3*delta3);
	if ((n3 > 1 && (i3 == start3 || i3 == end3)) ||
	    (n2 > 1 && (i2 == start2 || i2 == end2)) ||
	    (n1 > 1 && (i1 == start1 || i1 == end1))) {
	  mask[i] = FMM_IN;
	  myq->pqueue_insert (pt);
	} else {
	  mask[i] = FMM_IN;
	}
	
	if (stillconstant && fabs(slow[i]-slow0)>1.0e-6) { 
	  stillconstant=0;
/*	  fprintf(stderr,"Non-constant velocity detected inside initialbox\n");*/
/*	  fprintf(stderr,"Switching to fast-marching at box edge\n");*/
	}
      }
    }
  }

}
fastmarch::fastmarch(int o, 
	      int b_1, int b_2, int b_3,
	      int nz, int ny, int nx, 
	      float dz, float dy, float dx, 
	      float slow_0, float *s){

  order=o;
  slow=s;
  /* save to static parameters */
  n1 = nz; n2 = ny; n3 = nx;
  d1 = dz; d2 = dy; d3 = dx;
  n12 = n1*n2;
  n = n12*n3;
  b1=b_1; b2=b_2;b3=b_3;
  slow0=slow_0;
  alloc_local();
}
void fastmarch::calc_time(float s1, float s2, float s3, float *ttime){
  init_source(s1,s2,s3,ttime);

  float *pt;
  unsigned char *pm;
  int i1,i2,i3,i;

  /* start marching */
  while (nm > 0) {  /* "far away" points */
    pt = myq->pqueue_extract ();
    i = (int) (pt-ttime);
    i1 = i%n1;
    i2 = (i/n1)%n2;
    i3 = i/n12;
   // fprintf(stderr,"this is annoying %d %d %d \n",i1,i2,i3);
    *(pm = mask+i) = FMM_IN;
    if (stillconstant && fabs((slow[i]-slow0)/slow0) > 1.0e-6) { 
      stillconstant=0;
/*      fprintf(stderr,"Computed %d points with const. vel. ray-tracing\n",n-nm);*/
/*      fprintf(stderr,"Now switching to fast marching\n");*/
    } 
    if (stillconstant) { /* traveltimes by analytic ray-tracing */
      if (i1 < n1-1 && *(pm+  1) != FMM_IN) 
	cvupdate (CVTIME(i1+1,i2,i3), pt+  1, pm+  1); 
      if (i1 > 0    && *(pm-  1) != FMM_IN) 
	cvupdate (CVTIME(i1-1,i2,i3), pt-  1, pm-  1);
      if (i2 < n2-1 && *(pm+ n1) != FMM_IN) 
	cvupdate (CVTIME(i1,i2+1,i3), pt+ n1, pm+ n1); 
      if (i2 > 0    && *(pm- n1) != FMM_IN) 
	cvupdate (CVTIME(i1,i2-1,i3), pt- n1, pm- n1);
      if (i3 < n3-1 && *(pm+n12) != FMM_IN) 
	cvupdate (CVTIME(i1,i2,i3+1), pt+n12, pm+n12); 
      if (i3 > 0    && *(pm-n12) != FMM_IN) 
	cvupdate (CVTIME(i1,i2,i3-1), pt-n12, pm-n12);
    } else {  /*  upwind finite-difference stencil */
      if (i1 < n1-1 && *(pm+  1) != FMM_IN) 
	update (i1+1,i2,i3, pt+  1, pm+  1, slow[i+  1]); 
      if (i1 > 0    && *(pm-  1) != FMM_IN) 
	update (i1-1,i2,i3, pt-  1, pm-  1, slow[i-  1]);
      if (i2 < n2-1 && *(pm+ n1) != FMM_IN) 
	update (i1,i2+1,i3, pt+ n1, pm+ n1, slow[i+ n1]); 
      if (i2 > 0    && *(pm- n1) != FMM_IN) 
	update (i1,i2-1,i3, pt- n1, pm- n1, slow[i- n1]);

     if (i3 < n3-1 && *(pm+n12) != FMM_IN) 
	update (i1,i2,i3+1, pt+n12, pm+n12, slow[i+n12]); 
      if (i3 > 0    && *(pm-n12) != FMM_IN) 
	update (i1,i2,i3-1, pt-n12, pm-n12, slow[i-n12]);
	
    }
  }
 }
void fastmarch::update (int p1, int p2, int p3, float* tj, unsigned char* mj, float s)
{
  double  /*b, c, */ t1=0.0, t2=0.0;
  unsigned int k, i;
  double aaa,bbb,ccc,ddd;
  unsigned int jjj;
// fprintf(stderr,"CHECK IT %d %d %d \n",p1,n1,order);   
  /*b = c = 0.;*/ i = k = 0;
  ddd=1.; jjj = 0;
  aaa = bbb = ccc = 0.;

  if ((p1 > 0   ) && *(mj-1) && ((t1 = *(tj-1)) < *tj)) i |= 0x01;
  if ((p1 < n1-1) && *(mj+1) && ((t2 = *(tj+1)) < *tj) && 
      ((i ^ 0x01) || t2 < t1)) {i |= 0x01; t1 = t2; jjj ^= 0x01; } 
  if (i & 0x01) {
    ddd=d1;
    if (jjj & 0x01) {
      if   (
 
      (p1 < n1-2) && 
         (order>=2) &&
      *(mj+2)
      ) {
	if ((order>=3) && (p1 < n1-3) && *(mj+3)) {
	  ddd*=(121./36.); t1*=18.; t1-=*(tj+2)*9.; t1+=*(tj+3)*2.; t1/=11.;
	} else {
	  ddd *= 2.25; t1 *= 4.; t1 -= *(tj+2); t1 /= 3.;
	}
      }
    } else {
      if   ((order>=2) && (p1 > 1) && *(mj-2)) {
	if ((order>=3) && (p1 > 2) && *(mj-3)) {
	  ddd*=(121./36.); t1*=18.; t1-=*(tj-2)*9.; t1+=*(tj-3)*2.; t1/=11.;
	} else {
	  ddd *= 2.25; t1 *= 4.; t1 -= *(tj-2); t1 /= 3.;
	}
      }
    }
    aaa += ddd; bbb += -2.*ddd*t1; ccc += ddd*t1*t1;
    i ^= 0x01; k |= 0x01;
  }
  jjj =0;

  if ((p2 > 0   ) && *(mj-n1) && ((t1 = *(tj-n1)) < *tj)) i |= 0x01;
  if ((p2 < n2-1) && *(mj+n1) && ((t2 = *(tj+n1)) < *tj) && 
      ((i ^ 0x01) || t2 < t1)) {i |= 0x01; t1 = t2; jjj ^= 0x01; } 
  if (i & 0x01) {
    ddd=d2;
    if (jjj & 0x01) {
      if   ((order>=2) && (p2 < n2-2) && *(mj+2*n1)) {
	if (
	(order>=3) &&
	(p2 < n2-3) && 
	*(mj+3*n1)) {
	  ddd*=(121./36.);t1*=18.;t1-=*(tj+2*n1)*9.;t1+=*(tj+3*n1)*2.;t1/=11.;
	} else {
	  ddd *= 2.25; t1 *= 4.; t1 -= *(tj+2*n1); t1 /= 3.;
	}
      }
    } else {

      if   (
      (p2 > 1) &&
      (order>=2) && 
      *(mj-2*n1)) {
	if ((order>=3) && (p2 > 2) && *(mj-3*n1)) {
	  ddd*=(121./36.);t1*=18.;t1-=*(tj-2*n1)*9.;t1+=*(tj-3*n1)*2.;t1/=11.;
	} else {
	  ddd *= 2.25; t1 *= 4.; t1 -= *(tj-2*n1); t1 /= 3.;
	}
      }
    }
    aaa += ddd; bbb += -2.*ddd*t1; ccc += ddd*t1*t1;
    i ^= 0x01; k |= 0x02;
  }
  jjj =0;

  if ((p3 > 0   ) && *(mj-n12) && ((t1 = *(tj-n12)) < *tj)) i |= 0x01;
  if ((p3 < n3-1) && *(mj+n12) && ((t2 = *(tj+n12)) < *tj) && 
      ((i ^ 0x01) || t2 < t1)) {i |= 0x01; t1 = t2; jjj ^= 0x01; }  
  if (i & 0x01) {
    if ((i & 0x02) && (!(i & 0x01) || t2 > t1)) t1 = t2; 
    ddd=d3;
    if (jjj & 0x01) {
      if   ((order>=2) && (p3 < n3-2) && *(mj+2*n12)) {
	if ((order>=3) && (p3 < n3-3) && *(mj+3*n12)) {
	  ddd*=(121./36.);t1*=18.;t1-=*(tj+2*n12)*9.;
	  t1+=*(tj+3*n12)*2.;t1/=11.;
	} else {
	  ddd *= 2.25; t1 *= 4.; t1 -= *(tj+2*n12); t1 /= 3.;
	}
      }
    } else {
      if   ((order>=2) && (p3 > 1) && *(mj-2*n12)) {
	if ((order>=3) && (p3 > 2) && *(mj-3*n12)) {
	  ddd*=(121./36.);t1*=18.;t1-=*(tj-2*n12)*9.;
	  t1+=*(tj-3*n12)*2.;t1/=11.;
	} else {
	  ddd *= 2.25; t1 *= 4.; t1 -= *(tj-2*n12); t1 /= 3.;
	}
      }
    }
    aaa += ddd; bbb += -2.*ddd*t1; ccc += ddd*t1*t1;
    i ^= 0x01; k |= 0x04;
  }
  jjj =0;

  if (!k) return;
  ccc -= ((double) s)*((double) s);
  ddd = (bbb*bbb-4.*aaa*ccc);
  if (ddd <= 0.) { ddd=0.; }
  tp = (-bbb +sqrt (ddd))/(2.*aaa);
  if (tp < *tj) {
    *tj = tp;
    if (*mj == FMM_OUT) {
      nm--; 
      *mj = FMM_FRONT; 
      myq->pqueue_insert (tj);
    } 
  }
}

void fastmarch::cvupdate (double cvtime,float* tj,unsigned char* mj)

{
  if (cvtime < *tj) {
    *tj = cvtime;
    if (*mj == FMM_OUT) {
      nm--; 
      *mj = FMM_FRONT; 
      myq->pqueue_insert (tj);
    } 
  }
}


