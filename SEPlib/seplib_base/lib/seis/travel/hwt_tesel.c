#include <math.h>
#include <sepmath.h>
#include <septravel.h>
#include <hwt.h>
#include <seplib.h>

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void group_rays(rays *pRR, triplets *ptrp);
void get_T4(rays *pRR, triplets *ptrp, tetrahedra *ptet);
void out_T4(triplets *ptrp, tetrahedra *ptet,int iT4,int iR3,int ia,int ib,int ic,char last,int i_last);
int ingrid(cube *pTT,point3D *pA,point3D *pB,point3D *pC,point3D *pD);
float volum(point3D* pP, point3D* pQ, point3D* pR, point3D* pS);
void get_R3(rays *pRR,triplets *ptrp, int iR3);
void slic3(cube *pTT,cube *pLL,point3D* pA, point3D* pB, point3D* pC, point3D* pD);
void slic2(cube *pTT,cube *pLL,point3D* pA, point3D* pB, point3D* pC, int ix);
void slic1(cube *pTT,cube *pLL, point3D* pA, point3D* pB, point3D* pC, int ix, int iy);
void getit2(cube *pTT,point3D *pA,point3D *pB,point3D *pC,point3D *pP);
void sort(point3D** pp, int n, char c);
point3D lineval(point3D* pA, point3D* pB, float vv,char c);
void median_filter(cube *pTT, cube *pLL);
void mean_filter(cube *pTT, cube *pLL);
void get_times_slow(rays *pRR, triplets *ptrp,tetrahedra *ptet,cube *pTT, cube *pLL);
int isinT4(point3D* pM, point3D* pA, point3D* pB, point3D* pC, point3D* pD);
int sameside(point3D* pM, point3D* pT,point3D* pX, point3D* pY, point3D* pZ);
int isT4(point3D* pT, point3D* pX, point3D* pY, point3D* pZ);
void getit(point3D* pM, point3D* pA, point3D* pB, point3D* pC, point3D* pD);
int integerup(float x, float ox, float dx);
int integerdn(float x, float ox, float dx);
void measure_rays(rays* pRR);
void tesselate_init(rays *pRR, triplets *ptrp, tetrahedra *ptet);
void tesselate(rays *pRR, triplets *ptrp, tetrahedra *ptet);
void get_times_fast(rays *pRR,triplets *ptrp,tetrahedra *ptet,cube *pTT,cube *pLL);
#else
void group_rays();
void get_T4();
void out_T4();
void get_times_fast();
float volum();
void get_R3();
void slic3();
void slic2();
void slic1();
void getit2();
void sort();
point3D lineval();
int ingrid();
void median_filter();
void mean_filter();
void get_times_slow();
int isinT4();
int sameside();
int isT4();
void getit();
int integerup();
int integerdn();
int intnn();
void measure_rays();
void tesselate_init();
void tesselate_close();
void tesselate();
void get_times_fast();
#endif

#define LENGTH(i1,i2,i3) pLL->c0[i1+i2*pLL->n1+i3*pLL->n1*pLL->n2]
#define TIME(i1,i2,i3) pTT->c0[i1+i2*pTT->n1+i3*pTT->n1*pTT->n2]


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void tesselate_rays(rays* pRR,cube *pVV,float *times,int fast)
_XFUNCPROTOEND
#else
void tesselate_rays(pRR,pVV,times,fast)
rays *pRR;float *times;
cube *pVV;int fast;
#endif
{
float *length;
triplets trp;
tetrahedra tet;
cube pTT,pLL;
int n[3];
float o[3],d[3];

n[0]=pVV->n1; n[1]=pVV->n2; n[2]=pVV->n3;
o[0]=pVV->o1; o[1]=pVV->o2; o[2]=pVV->o3;
d[0]=pVV->d1; d[1]=pVV->d2; d[2]=pVV->d3;


length=(float*) alloc(sizeof(float)*n[0]*n[1]*n[2]);
tesselate_init(pRR,&trp,&tet);
init_cube(&pTT,times,n,o,d);
init_cube(&pLL,length,n,o,d);
measure_rays(pRR);
set_cube(pTT,NOTIME);
set_cube(pLL,0.0);
tesselate(pRR,&trp,&tet);
if(fast==1){
	get_times_fast(pRR,&trp,&tet,&pTT,&pLL);
	median_filter(&pTT,&pLL); 
	mean_filter(&pTT,&pLL); 
}
else{
 get_times_slow(pRR,&trp,&tet,&pTT,&pLL); 
}
tesselate_close(&trp, &tet);
free(length);
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void measure_rays(rays* pRR)
_XFUNCPROTOEND
#else
void measure_rays(pRR)
rays* pRR;
#endif
{
  int it,ii,ia;
  float ddx,ddy,ddz,ddl;

  for(ii=0;ii<pRR->ni;ii++) {
    for(ia=0;ia<pRR->na;ia++) {
      for(it=1;it<pRR->nt;it++) {
  ddx=pRR->c0[ii][ia][it].x-pRR->c0[ii][ia][it-1].x;
  ddy=pRR->c0[ii][ia][it].y-pRR->c0[ii][ia][it-1].y;
  ddz=pRR->c0[ii][ia][it].z-pRR->c0[ii][ia][it-1].z;
  ddl=sqrt(ddx*ddx+ddy*ddy+ddz*ddz);
  pRR->c0[ii][ia][it].l=pRR->c0[ii][ia][it-1].l+ddl;
      }}}
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void tesselate_init(rays *pRR, triplets *ptrp, tetrahedra *ptet)
_XFUNCPROTOEND
#else
void tesselate_init(pRR,ptrp,ptet)
rays *pRR; triplets *ptrp; tetrahedra *ptet;
#endif
{
  int iT4;

  ptrp->nR3=2*(pRR->ni-1)*(pRR->na);
  ptrp->R3=(triplet*) alloc(ptrp->nR3*sizeof(triplet));

  ptrp->rayA.point=(point3D*) alloc(pRR->nt*sizeof(point3D));
  ptrp->rayB.point=(point3D*) alloc(pRR->nt*sizeof(point3D));
  ptrp->rayC.point=(point3D*) alloc(pRR->nt*sizeof(point3D));

  ptet->nT4=3*(pRR->nt-2)+1;
  ptet->T4=(tetrahedron**) alloc(ptet->nT4*sizeof(tetrahedron*));
  for(iT4=0;iT4<ptet->nT4;iT4++)
    ptet->T4[iT4]=(tetrahedron*) alloc(ptrp->nR3*sizeof(tetrahedron));

  /***********
    T4[i][j]
       ^  ^
      T4  R3
   ***********/
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void tesselate(rays *pRR, triplets *ptrp, tetrahedra *ptet)
_XFUNCPROTOEND
#else
void tesselate(pRR,ptrp,ptet)
rays *pRR; triplets *ptrp; tetrahedra *ptet;
#endif
{
  group_rays(pRR,ptrp);  
	get_T4(pRR,ptrp,ptet);
}


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void tesselate_close(triplets *ptrp, tetrahedra *ptet)
_XFUNCPROTOEND
#else
void tesselate_close(ptrp,ptet)
triplets *ptrp; tetrahedra *ptet;
#endif
{
  int iT4;

  free(ptrp->R3);
  free(ptrp->rayA.point);
  free(ptrp->rayB.point);
  free(ptrp->rayC.point);

  for(iT4=0;iT4<ptet->nT4;iT4++)
    free(ptet->T4[iT4]);
  free(ptet->T4);
}


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void group_rays(rays *pRR, triplets *ptrp)
_XFUNCPROTOEND
#else
void group_rays(pRR,ptrp)
rays *pRR; triplets *ptrp;
#endif
{
  int iR3=0;
  int ii,ia;

  for(ii=0;ii<pRR->ni-1;ii++) {
    for(ia=0;ia<pRR->na;ia++) {
      /* the loop will output 2 triplets at a time */
      /*  x  x       -> incl
    x  */
      ptrp->R3[iR3].A_i=ii  ;    ptrp->R3[iR3].A_a=ia  ;
      ptrp->R3[iR3].B_i=ii  ;    ptrp->R3[iR3].B_a=(ia+1==pRR->na)?0:ia+1;
      ptrp->R3[iR3].C_i=ii+1;  ptrp->R3[iR3].C_a=ia  ;
      iR3++;

      /*      x     -> incl
        x  x  */
      ptrp->R3[iR3].A_i=ii  ;  ptrp->R3[iR3].A_a=(ia+1==pRR->na)?0:ia+1;
      ptrp->R3[iR3].B_i=ii+1;  ptrp->R3[iR3].B_a=(ia+1==pRR->na)?0:ia+1;
      ptrp->R3[iR3].C_i=ii+1;  ptrp->R3[iR3].C_a=ia;
      iR3++;
    }}
}


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void get_T4(rays *pRR, triplets *ptrp, tetrahedra *ptet)
#else
void get_T4(pRR,ptrp,ptet)
rays *pRR; triplets *ptrp; tetrahedra *ptet;
#endif
{
  int iR3=0;
  int ia,ib,ic;
  char last='a';

  for(iR3=0;iR3<ptrp->nR3;iR3++) {
    int iT4=0;
    get_R3(pRR,ptrp,iR3);

    out_T4(ptrp,ptet,iT4,iR3,1,1,1,'a',0); iT4++;

    for(ia=1,ib=1,ic=1;ia<pRR->nt-1 || ib<pRR->nt-1 || ic<pRR->nt-1;) {
      ia++; last='a'; out_T4(ptrp,ptet,iT4,iR3,ia,ib,ic,last,ia-1); iT4++;
      ib++; last='b'; out_T4(ptrp,ptet,iT4,iR3,ia,ib,ic,last,ib-1); iT4++;
      ic++; last='c'; out_T4(ptrp,ptet,iT4,iR3,ia,ib,ic,last,ic-1); iT4++;
    }}
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void out_T4(triplets *ptrp, tetrahedra *ptet,
      int iT4,int iR3,int ia,int ib,int ic,char last,int i_last)
#else
void out_T4(ptrp,ptet,iT4,iR3,ia,ib,ic,last,i_last)
triplets *ptrp; tetrahedra *ptet;
      int iT4;int iR3;int ia;int ib;int ic;char last;int i_last;
#endif
{
  /* A */
  ptet->T4[iT4][iR3].A_i=ptrp->R3[iR3].A_i;
  ptet->T4[iT4][iR3].A_a=ptrp->R3[iR3].A_a;
  ptet->T4[iT4][iR3].ia=ia;
  /* B */
  ptet->T4[iT4][iR3].B_i=ptrp->R3[iR3].B_i;
  ptet->T4[iT4][iR3].B_a=ptrp->R3[iR3].B_a;
  ptet->T4[iT4][iR3].ib=ib;
  /* C */
  ptet->T4[iT4][iR3].C_i=ptrp->R3[iR3].C_i;
  ptet->T4[iT4][iR3].C_a=ptrp->R3[iR3].C_a;
  ptet->T4[iT4][iR3].ic=ic;
  /* D */
  if(last=='a') {
    ptet->T4[iT4][iR3].D_i=ptrp->R3[iR3].A_i;
    ptet->T4[iT4][iR3].D_a=ptrp->R3[iR3].A_a;
    ptet->T4[iT4][iR3].id=i_last;
  }
  else if(last=='b') {
    ptet->T4[iT4][iR3].D_i=ptrp->R3[iR3].B_i;
    ptet->T4[iT4][iR3].D_a=ptrp->R3[iR3].B_a;
    ptet->T4[iT4][iR3].id=i_last;
  }
  else {  /* (last=='c') */
    ptet->T4[iT4][iR3].D_i=ptrp->R3[iR3].C_i;
    ptet->T4[iT4][iR3].D_a=ptrp->R3[iR3].C_a;
    ptet->T4[iT4][iR3].id=i_last;
  }
}



#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void get_times_fast(rays *pRR,triplets *ptrp,tetrahedra *ptet,cube *pTT,cube *pLL)
_XFUNCPROTOEND
#else
void get_times_fast(pRR,ptrp,ptet,pTT,pLL)
rays *pRR;triplets *ptrp;tetrahedra *ptet;cube *pTT;cube *pLL;
#endif
{
  int iR3,iT4;
  point3D *pA,*pB,*pC,*pD;  int ai,aa,ia,bi,ba,ib,ci,ca,ic,di,da,id;

  for(iT4=0;iT4<ptet->nT4;iT4++) {
    for(iR3=0;iR3<ptrp->nR3;iR3++) {
      ai=ptet->T4[iT4][iR3].A_i;
      aa=ptet->T4[iT4][iR3].A_a;
      ia=ptet->T4[iT4][iR3].ia;

      bi=ptet->T4[iT4][iR3].B_i;
      ba=ptet->T4[iT4][iR3].B_a;
      ib=ptet->T4[iT4][iR3].ib;

      ci=ptet->T4[iT4][iR3].C_i;
      ca=ptet->T4[iT4][iR3].C_a;
      ic=ptet->T4[iT4][iR3].ic;

      di=ptet->T4[iT4][iR3].D_i;
      da=ptet->T4[iT4][iR3].D_a;
      id=ptet->T4[iT4][iR3].id;

      pA=&pRR->c0[ai][aa][ia];
      pB=&pRR->c0[bi][ba][ib];
      pC=&pRR->c0[ci][ca][ic];
      pD=&pRR->c0[di][da][id];

      pA->t=pRR->ot+ia*pRR->st;
      pB->t=pRR->ot+ib*pRR->st;
      pC->t=pRR->ot+ic*pRR->st;
      pD->t=pRR->ot+id*pRR->st;
     if(ingrid(pTT,pA,pB,pC,pD) && volum(pA,pB,pC,pD)>0 ) {
  			slic3(pTT,pLL,pA,pB,pC,pD);
      }
    }}
}
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int ingrid(cube *pTT,point3D *pA,point3D *pB,point3D *pC,point3D *pD)
_XFUNCPROTOEND
#else
int ingrid(pTT,pA,pB,pC,pD)
cube *pTT;point3D *pA;point3D *pB;point3D *pC;point3D *pD;
#endif
{
  int nAx,nAy,nAz,nBx,nBy,nBz,nCx,nCy,nCz,nDx,nDy,nDz;
  int xout,yout,zout;
  int mx,my,mz;

  mx=0;my=0;mz=0;

  nAx=integerup(pA->x,pTT->o2,pTT->d2);
  nBx=integerdn(pB->x,pTT->o2,pTT->d2);
  nCx=integerdn(pC->x,pTT->o2,pTT->d2);
  nDx=integerdn(pD->x,pTT->o2,pTT->d2);
  xout=(nAx>pTT->n2-1 && nBx>pTT->n2-1 &&  nCx>pTT->n2-1 && nDx>pTT->n2-1)||
    (   nAx<mx        && nBx<mx        &&  nCx<mx        && nDx<mx    );

  if (!xout) {
    nAy=integerup(pA->y,pTT->o3,pTT->d3);
    nBy=integerdn(pB->y,pTT->o3,pTT->d3);
    nCy=integerdn(pC->y,pTT->o3,pTT->d3);
    nDy=integerdn(pD->y,pTT->o3,pTT->d3);
    yout=(nAy>pTT->n3-1 && nBy>pTT->n3-1 &&  nCy>pTT->n3-1 && nDy>pTT->n3-1)||
      (   nAy<my        && nBy<my        &&  nCy<my        && nDy<my    );

    if (!yout) {
      nAz=integerup(pA->z,pTT->o1,pTT->d1);
      nBz=integerdn(pB->z,pTT->o1,pTT->d1);
      nCz=integerdn(pC->z,pTT->o1,pTT->d1);
      nDz=integerdn(pD->z,pTT->o1,pTT->d1);
      zout=(nAz>pTT->n1-1 && nBz>pTT->n1-1 && nCz>pTT->n1-1 && nDz>pTT->n1-1)||
  (   nAz<mz        && nBz<mz        &&  nCz<mz        && nDz<mz   );
    }
  }

  if( xout || yout || zout)
    return NO;
  else
    return YES;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float volum(point3D* pP, point3D* pQ, point3D* pR, point3D* pS)
_XFUNCPROTOEND
#else
float volum(pP,pQ,pR,pS)
point3D* pP; point3D* pQ; point3D* pR; point3D* pS;
#endif
{
  float m[9];
  float zRP,xRP,yRP, zQP,xQP,yQP, zSP,xSP,ySP;

  zRP=pR->z-pP->z; zQP=pQ->z-pP->z; zSP=pS->z-pP->z;
  xRP=pR->x-pP->x; xQP=pQ->x-pP->x; xSP=pS->x-pP->x;
  yRP=pR->y-pP->y; yQP=pQ->y-pP->y; ySP=pS->y-pP->y;

  m[0]=zRP; m[1]=zQP; m[2]=zSP;
  m[3]=xRP; m[4]=xQP; m[5]=xSP;
  m[6]=yRP; m[7]=yQP; m[8]=ySP;

  return ABS(det3(m));
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void get_R3(rays *pRR,triplets *ptrp, int iR3)
_XFUNCPROTOEND
#else
void get_R3(pRR,ptrp,iR3)
rays *pRR;triplets *ptrp; int iR3;
#endif
{
  int A_i,A_a;
  int B_i,B_a;
  int C_i,C_a;
  int it;
  
  A_i=ptrp->R3[iR3].A_i;  A_a=ptrp->R3[iR3].A_a;  
  B_i=ptrp->R3[iR3].B_i;  B_a=ptrp->R3[iR3].B_a;  
  C_i=ptrp->R3[iR3].C_i;  C_a=ptrp->R3[iR3].C_a;

  for(it=0;it<pRR->nt;it++) {
    ptrp->rayA.point[it]=pRR->c0[A_i][A_a][it];
    ptrp->rayB.point[it]=pRR->c0[B_i][B_a][it];
    ptrp->rayC.point[it]=pRR->c0[C_i][C_a][it];
  }
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void slic3(cube *pTT,cube *pLL,point3D* pA, point3D* pB, point3D* pC, point3D* pD)
_XFUNCPROTOEND
#else
void slic3(pTT,pLL,pA, pB,pC,pD)
cube *pTT;cube *pLL;point3D* pA; point3D* pB; point3D* pC; point3D* pD;
#endif
{
  /* find a vertical slice through the tetrahedron defined by A,B,C,D. */
  /* it finds either one or two triangles */
  /* the calls slic2 which continues */
  point3D *pp[4],K1,K2,K3;
  int nAx,nBx,nCx,nDx,ix;
  float xx,eps=pTT->d2/10000;

  /* order the points A-B-C-D, so that A has the smallest inline coordinate (X) */
  pp[0]=pA; pp[1]=pB; pp[2]=pC; pp[3]=pD;
  sort(pp,4,'x');
  pA=pp[0]; pB=pp[1]; pC=pp[2]; pD=pp[3];

  nAx=integerup(pA->x,pTT->o2,pTT->d2);
  nBx=integerdn(pB->x,pTT->o2,pTT->d2);
  nCx=integerdn(pC->x,pTT->o2,pTT->d2);
  nDx=integerdn(pD->x,pTT->o2,pTT->d2);

  /* it the T4 is not in the grid, skip it */
  if(ingrid(pTT,pA,pB,pC,pD)) {
    /* special case - ABCD are in the same plane, i.e. have the same X */
    if (nAx==nBx && nAx==nCx && nAx==nDx) {
      float distbc;
      
      xx=pTT->o2+nAx*pTT->d2;

      pp[0]=pA; pp[1]=pB; pp[2]=pC; pp[3]=pD;
      sort(pp,4,'y');
      pA=pp[0]; pB=pp[1]; pC=pp[2]; pD=pp[3];
      
      distbc=sqrt((pB->y-pC->y)*(pB->y-pC->y)+(pB->z-pC->z)*(pB->z-pC->z));
      
      if (!distbc) { slic2(pTT,pLL,pA,pB,pC,nAx);	slic2(pTT,pLL,pB,pC,pD,nAx); }
      else {         slic2(pTT,pLL,pB,pA,pD,nAx);	slic2(pTT,pLL,pC,pA,pD,nAx); }
    }
    else {
      
      /* from A to B*/
      for(ix=MAX(nAx,0);ix<=MIN(nBx,pTT->n2-1);ix++) {
	xx=pTT->o2+ix*pTT->d2;
	
	/* other special cases - pathological cases */
	/* when the points have the same coordinates */
	if (ABS((float)(pA->x-pB->x))<eps && ABS((float)(pA->x-pC->x))>eps) {
	  K1=lineval(pB,pC,xx,'x');  K2=lineval(pA,pC,xx,'x'); 
	  K3=lineval(pA,pD,xx,'x');
	}
	else if (ABS((float)(pA->x-pB->x))<eps && ABS((float)(pA->x-pC->x))<eps) {
	  K1=lineval(pB,pD,xx,'x');  K2=lineval(pC,pD,xx,'x'); 
	  K3=lineval(pA,pD,xx,'x');
	}
	else { /* finally a decent case */
	  K1=lineval(pA,pB,xx,'x');  K2=lineval(pA,pC,xx,'x'); 
	  K3=lineval(pA,pD,xx,'x');
	}
	slic2(pTT,pLL,&K1,&K2,&K3,ix);
      }
      
      /* from B to C*/
      for(ix=MAX(nBx+1,0);ix<=MIN(nCx,pTT->n2-1);ix++) {
	xx=pTT->o2+ix*pTT->d2;

	K1=lineval(pA,pD,xx,'x');  K2=lineval(pB,pC,xx,'x');
	
	K3=lineval(pA,pC,xx,'x');  slic2(pTT,pLL,&K1,&K2,&K3,ix);
	K3=lineval(pB,pD,xx,'x');  slic2(pTT,pLL,&K1,&K2,&K3,ix);
      }
      
      /* from C to D*/
      for(ix=MAX(nCx+1,0);ix<=MIN(nDx,pTT->n2-1);ix++) {
	xx=pTT->o2+ix*pTT->d2;

	K1=lineval(pD,pA,xx,'x');  K2=lineval(pD,pB,xx,'x'); 
	K3=lineval(pD,pC,xx,'x');  slic2(pTT,pLL,&K1,&K2,&K3,ix);
      }

    } /* end if-else for the special case */
  } /* end ingrid */
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void slic2(cube *pTT,cube *pLL,point3D* pA, point3D* pB, point3D* pC, int ix)
_XFUNCPROTOEND
#else
void slic2(pTT,pLL,pA,pB,pC,ix)
cube *pTT;cube *pLL;point3D* pA; point3D* pB; point3D* pC; int ix;
#endif
{
  point3D *pp[3];
  int nAy,nBy,nCy,iy;

  /* order K1,K2,K3 from slic3 by the xline coordinate */
  pp[0]=pA; pp[1]=pB; pp[2]=pC;
  sort(pp,3,'y');
  pA=pp[0]; pB=pp[1]; pC=pp[2];

  nAy=integerup(pA->y,pTT->o3,pTT->d3);
  nBy=integerdn(pB->y,pTT->o3,pTT->d3);
  nCy=integerdn(pC->y,pTT->o3,pTT->d3);

  for(iy=MAX(nAy,1);iy<=MIN(nBy,pTT->n3-1);iy++)
    slic1(pTT,pLL,pA,pB,pC,ix,iy);
  
  for(iy=MAX(nBy+1,1);iy<=MIN(nCy,pTT->n3-1);iy++)
    slic1(pTT,pLL,pC,pB,pA,ix,iy);
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void slic1(cube *pTT,cube *pLL, point3D* pA, point3D* pB, point3D* pC, int ix, int iy)
_XFUNCPROTOEND
#else
void slic1(pTT,pLL,pA,pB,pC,ix,iy)
cube *pTT;cube *pLL;point3D* pA; point3D* pB; point3D* pC; int ix; int iy;
#endif
{
  point3D *pp[3];
  int izl,izh,iz;
  point3D M;
  float z1,z2,zl,zh,yy,zz;
  float tl,th,ti, ll,lh,li;
  int ggg;
  ggg=0;


  M.x=pTT->o2+ix*pTT->d2;
  M.y=pTT->o3+iy*pTT->d3; yy=M.y;

  if (pA->y==pB->y && pA->y!=pC->y) {
    z1=(lineval(pC,pB,yy,'y')).z;
    z2=(lineval(pC,pA,yy,'y')).z;
    /*    if(ix==40 && iy==0) wr(".1.");*/
  }
  else if (pA->y==pB->y && pA->y==pC->y) {
    pp[0]=pA; pp[1]=pB; pp[2]=pC;
    sort(pp,3,'z');
    pA=pp[0]; pB=pp[1]; pC=pp[2];
    z1=pA->z;
    z2=pC->z;
    /*    if(ix==40 && iy==0) wr(".2.");*/
  }
  else {
    z1=(lineval(pA,pB,yy,'y')).z;
    z2=(lineval(pA,pC,yy,'y')).z;
    /*    if(ix==40 && iy==0) wr(".3.");*/
  }

  izl=integerup(MIN(z1,z2),pTT->o1,pTT->d1);
  zl=pTT->o1+izl*pTT->d1;

  izh=integerdn(MAX(z1,z2),pTT->o1,pTT->d1);
  zh=pTT->o1+izh*pTT->d1;



  if(izl<=izh) {
    /* smallest z - Zlow */
    M.z=zl;                    
    getit2(pTT,pA,pB,pC,&M); tl=M.t; ll=M.l;

/*    if (izl>=0 && izl<=pTT->n1) {*/
		/*possible bug*/
    if (izl>=0 && izl<pTT->n1) {
			if(LENGTH(izl,ix,iy)==0.0){
			TIME(izl,ix,iy)=tl;
	if(ggg)  TIME(izl,ix,iy)=1;
		LENGTH(izl,ix,iy)=ll;
      }
  else if(ll<LENGTH(izl,ix,iy)){
		TIME(izl,ix,iy)=tl;
	  if(ggg) TIME(izl,ix,iy)=1;
		LENGTH(izl,ix,iy)=ll;
	}
    }
    
    /* biggest z - Zhigh */
    M.z=zh;                    
    getit2(pTT,pA,pB,pC,&M); th=M.t; lh=M.l;

    if(izh>=0 && izh<pTT->n1) {
      if(LENGTH(izh,ix,iy)==0.0){
			TIME(izh,ix,iy)=th;
	if(ggg) TIME(izh,ix,iy)=10;
	LENGTH(izh,ix,iy)=lh;
      }
      else
	if(lh<LENGTH(izh,ix,iy)){
		TIME(izh,ix,iy)=th;
	  if(ggg) TIME(izh,ix,iy)=10;
		LENGTH(izh,ix,iy)=lh;
	}
    }

    for(iz=MAX(izl+1,0);iz<=MIN(izh-1,pTT->n1-1);iz++) {
      zz=pTT->o1+iz*pTT->d1;
      li=ll+(lh-ll)*(zz-zl)/(zh-zl);
      ti=tl+(th-tl)*(zz-zl)/(zh-zl);
      if(lh< LENGTH(iz,ix,iy)){
			TIME(iz,ix,iy)=ti;
	if(ggg) TIME(iz,ix,iy)=5;
	LENGTH(iz,ix,iy)=lh;
      }
      else
	if(li<LENGTH(iz,ix,iy)){
		TIME(iz,ix,iy)=ti;
	  if(ggg)TIME(iz,ix,iy)=5;
		LENGTH(iz,ix,iy)=li;
	}
    }
  } /* end if izl<izh */
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void getit2(cube *pTT,point3D *pA,point3D *pB,point3D *pC,point3D *pP)
_XFUNCPROTOEND
#else
void getit2(pTT,pA,pB,pC,pP)
cube *pTT;point3D *pA;point3D *pB;point3D *pC;point3D *pP;
#endif
{
  float ap[5],ab[5],ac[5];
  float detyz,detyt,detzt,detyl,detzl;
  float d, dmax;
  point3D *pp[3];
  point3D *pL,*pH;
  float eps=pTT->d2/100000;

  ab[1]=pB->y-pA->y; ac[1]=pC->y-pA->y; ap[1]=pP->y-pA->y; /*y*/
  ab[2]=pB->z-pA->z; ac[2]=pC->z-pA->z; ap[2]=pP->z-pA->z; /*z*/
  ab[3]=pB->t-pA->t; ac[3]=pC->t-pA->t;                    /*t*/
  ab[4]=pB->l-pA->l; ac[4]=pC->l-pA->l;                    /*l*/

  detyz=ab[1]*ac[2]-ab[2]*ac[1]; /*1-2 = y-z*/
  detyt=ab[1]*ac[3]-ab[3]*ac[1]; /*1-3 = y-t*/
  detzt=ab[2]*ac[3]-ab[3]*ac[2]; /*2-3 = z-t*/

  detyl=ab[1]*ac[4]-ab[4]*ac[1]; /*1-3 = y-l*/
  detzl=ab[2]*ac[4]-ab[4]*ac[2]; /*2-3 = z-l*/

  if(ABS(detyz)>eps) {
    pP->t=pA->t+ap[2]*detyt/detyz-ap[1]*detzt/detyz;
    pP->l=pA->l+ap[2]*detyl/detyz-ap[1]*detzl/detyz;
  }
  else {
    pp[0]=pA; pp[1]=pB; pp[2]=pC;
    sort(pp,3,'z');
    pL=pp[0]; pH=pp[2];

    dmax=sqrt((pH->z-pL->z)*(pH->z-pL->z)+(pH->y-pL->y)*(pH->y-pL->y));
    if (ABS(dmax)<eps) {
      pP->t=pL->t;
      pP->l=pL->l;
    }
    else {
      d=sqrt((pP->z-pL->z)*(pP->z-pL->z)+(pP->y-pL->y)*(pP->y-pL->y));
      pP->t=pL->t+(pH->t-pL->t)*d/dmax;
      pP->l=pL->l+(pH->l-pL->l)*d/dmax;
    }
  }
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void sort(point3D** pp, int n, char c)
_XFUNCPROTOEND
#else
void sort(pp,n,c)
point3D** pp; int n; char c;
#endif
{
  int i,j,min;
  point3D *pt;

  for(i=0;i<n-1;i++) {
    min=i;
    for(j=i+1;j<n;j++)
      if (c=='x'){       if(pp[j]->x < pp[min]->x) min=j;}
      else if (c=='y') { if(pp[j]->y < pp[min]->y) min=j;}
      else {             if(pp[j]->z < pp[min]->z) min=j;}  
    pt=pp[min];pp[min]=pp[i];pp[i]=pt;
  }
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
point3D lineval(point3D* pA, point3D* pB, float vv,char c)
_XFUNCPROTOEND
#else
point3D lineval(pA,pB, vv,c)
point3D* pA; point3D* pB; float vv;char c;
#endif
{
  point3D M;
  float ratio;

  if (c=='x') {
    ratio=(vv-pA->x)/(pB->x-pA->x);
    M.x=vv;
    M.y=pA->y+(pB->y-pA->y)*ratio;
  }
  else {
    ratio=(vv-pA->y)/(pB->y-pA->y);
    M.x=pA->x+(pB->x-pA->x)*ratio;
    M.y=vv;
  }
  M.z=pA->z+(pB->z-pA->z)*ratio;
  M.t=pA->t+(pB->t-pA->t)*ratio;
  M.l=pA->l+(pB->l-pA->l)*ratio;

  return M;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void median_filter(cube *pTT, cube *pLL)
_XFUNCPROTOEND
#else
void median_filter(pTT, pLL)
cube *pTT; cube *pLL;
#endif
{
  int i1,i2,i3;
  int j1,j2,j3;
  float pm[27];
  float pmm;
  int i,j,k;
  int min;

  for(i3=0;i3<pTT->n3;i3++) {
    for(i2=0;i2<pTT->n2;i2++) {
      for(i1=0;i1<pTT->n1;i1++) {
	LENGTH(i1,i2,i3)=TIME(i1,i2,i3);
      }}}
  
  for(i3=0;i3<pTT->n3;i3++) {
    for(i2=0;i2<pTT->n2;i2++) {
      for(i1=0;i1<pTT->n1;i1++) {
	if (TIME(i1,i2,i3)==NOTIME) {
	  k=0;
	  for(j3=MAX(i3-1,0);j3<=MIN(i3+1,pTT->n3-1);j3++) {
	    for(j2=MAX(i2-1,0);j2<=MIN(i2+1,pTT->n2-1);j2++) {
	      for(j1=MAX(i1-1,0);j1<=MIN(i1+1,pTT->n1-1);j1++) {
		if (LENGTH(j1,j2,j3)!=NOTIME){
		  pm[k]=LENGTH(j1,j2,j3);
		  k++;
		}
	      }}}
	  if (k!=0) {
	    /* sort by magnitude*/
	    for(i=0;i<k-1;i++) {
	      min=i;
	      for(j=i+1;j<k;j++) {
		if(pm[j] < pm[min]) min=j;
	      }
	      pmm=pm[min]; pm[min]=pm[i]; pm[i]=pmm;
	    }
			TIME(i1,i2,i3)=pm[k/2];
	  } /* end sort */
	} /* end if NOTIME */
    }}}
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void mean_filter(cube *pTT, cube *pLL)
_XFUNCPROTOEND
#else
void mean_filter(pTT,pLL)
cube *pTT; cube *pLL;
#endif
{
  int i1,i2,i3;
  int j1,j2,j3;
  int k;
  float sum;

  for(i3=0;i3<pTT->n3;i3++) {
    for(i2=0;i2<pTT->n2;i2++) {
      for(i1=0;i1<pTT->n1;i1++) {
		LENGTH(i1,i2,i3)=TIME(i1,i2,i3);	
      }}}

  for(i3=0;i3<pTT->n3;i3++) {
    for(i2=0;i2<pTT->n2;i2++) {
      for(i1=0;i1<pTT->n1;i1++) {
	k=0;
	sum=0;
	for(j3=MAX(i3-1,0);j3<=MIN(i3+1,pTT->n3-1);j3++) {
	  for(j2=MAX(i2-1,0);j2<=MIN(i2+1,pTT->n2-1);j2++) {
	    for(j1=MAX(i1-1,0);j1<=MIN(i1+1,pTT->n1-1);j1++) {
	      sum+=LENGTH(j1,j2,j3);
	      k++;
	    }}}
		TIME(i1,i2,i3)=sum/k;
      }}}
}


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void get_times_slow(rays *pRR, triplets *ptrp,tetrahedra *ptet,cube *pTT, cube *pLL)
_XFUNCPROTOEND
#else
void get_times_slow(pRR,ptrp,ptet,pTT,pLL)
rays *pRR; triplets *ptrp;tetrahedra *ptet;cube *pTT; cube *pLL;
#endif
{
  int iR3,iT4;
  point3D *pA,*pB,*pC,*pD;
  float b2min,b2max,b3min,b3max,b1min,b1max;
  int n2min,n2max,n3min,n3max,n1min,n1max;
  int bi2,bi3,bi1;
  point3D M;
  int ai,aa,ia,bi,ba,ib,ci,ca,ic,di,da,id;
  
  for(iT4=0;iT4<ptet->nT4;iT4++) {
    for(iR3=0;iR3<ptrp->nR3;iR3++) {

      ai=ptet->T4[iT4][iR3].A_i; 
      aa=ptet->T4[iT4][iR3].A_a;
      ia=ptet->T4[iT4][iR3].ia;

      bi=ptet->T4[iT4][iR3].B_i;
      ba=ptet->T4[iT4][iR3].B_a;
      ib=ptet->T4[iT4][iR3].ib;

      ci=ptet->T4[iT4][iR3].C_i;
      ca=ptet->T4[iT4][iR3].C_a;
      ic=ptet->T4[iT4][iR3].ic;

      di=ptet->T4[iT4][iR3].D_i;
      da=ptet->T4[iT4][iR3].D_a;
      id=ptet->T4[iT4][iR3].id;

      pA=&pRR->c0[ai][aa][ia];
      pB=&pRR->c0[bi][ba][ib];
      pC=&pRR->c0[ci][ca][ic];
      pD=&pRR->c0[di][da][id];

      if (isT4(pA,pB,pC,pD)) {
	
	b2min=pA->x; b2max=pA->x;
	b2min=MIN(pB->x,b2min); b2min=MIN(pC->x,b2min); b2min=MIN(pD->x,b2min);
	b2max=MAX(pB->x,b2max); b2max=MAX(pC->x,b2max); b2max=MAX(pD->x,b2max);
	n2min=(int)(b2min-pTT->o2)/pTT->d2; n2max=(int)(b2max-pTT->o2)/pTT->d2+1;
	
	b3min=pA->y; b3max=pA->y; 
	b3min=MIN(pB->y,b3min); b3min=MIN(pC->y,b3min); b3min=MIN(pD->y,b3min);
	b3max=MAX(pB->y,b3max); b3max=MAX(pC->y,b3max); b3max=MAX(pD->y,b3max);
	n3min=(int)(b3min-pTT->o3)/pTT->d3; n3max=(int)(b3max-pTT->o3)/pTT->d3+1;
	
	b1min=pA->z; b1max=pA->z;
	b1min=MIN(pB->z,b1min); b1min=MIN(pC->z,b1min); b1min=MIN(pD->z,b1min);
	b1max=MAX(pB->z,b1max); b1max=MAX(pC->z,b1max); b1max=MAX(pD->z,b1max);
	n1min=(int)(b1min-pTT->o1)/pTT->d1; n1max=(int)(b1max-pTT->o1)/pTT->d1+1;
	
	n2min=(n2min<0)?0:n2min; n2max=(n2max<=pTT->n2)?n2max:pTT->n2;
	n3min=(n3min<0)?0:n3min; n3max=(n3max<=pTT->n3)?n3max:pTT->n3;
	n1min=(n1min<0)?0:n1min; n1max=(n1max<=pTT->n1)?n1max:pTT->n1;
	
	/* loop over the box around the tetrahedron */
	for(bi2=n2min;bi2<n2max;bi2++) {
	  for(bi3=n3min;bi3<n3max;bi3++) {
	    for(bi1=n1min;bi1<n1max;bi1++) {
	      M.x=pTT->o2+bi2*pTT->d2;
	      M.y=pTT->o3+bi3*pTT->d3;
	      M.z=pTT->o1+bi1*pTT->d1;

	      
	      if(isinT4(&M,pA,pB,pC,pD)==YES) {
		
		pA->t=pRR->ot+ia*pRR->st;
		pB->t=pRR->ot+ib*pRR->st;
		pC->t=pRR->ot+ic*pRR->st;
		pD->t=pRR->ot+id*pRR->st;
		
		getit(&M,pA,pB,pC,pD);
		if(LENGTH(bi1,bi2,bi3)==0.0){
			TIME(bi1,bi2,bi3)=M.t;
		  LENGTH(bi1,bi2,bi3)=M.l;
		}
		else {
		  if (M.l<LENGTH(bi1,bi2,bi3)){
				TIME(bi1,bi2,bi3)=M.t;
				LENGTH(bi1,bi2,bi3)=M.l;
		  }
		}
	      }
	    }}}

      } /* end 'if isT4' */
    }}/* end loop iR3 and iT4 */
}
/* end 'if isinT4' */
/* end loop on the box around the T4 */

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int isinT4(point3D* pM, point3D* pA, point3D* pB, point3D* pC, point3D* pD)
_XFUNCPROTOEND
#else
int isinT4(pM,pA,pB,pC,pD)
point3D* pM; point3D* pA; point3D* pB; point3D* pC; point3D* pD;
#endif
{
  /* check if M is inside (ABCD) */
  int sMA,sMB,sMC,sMD;

  sMA=sameside(pM,pA,   pB,pC,pD);
  sMB=sameside(pM,pB,pA,   pC,pD);
  sMC=sameside(pM,pC,pA,pB,   pD);
  sMD=sameside(pM,pD,pA,pB,pC   );
  
  if(sMA==YES && sMB==YES && sMC==YES && sMD==YES)
    return YES;
  else
    return NO;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int sameside(point3D* pM, point3D* pT,point3D* pX, point3D* pY, point3D* pZ)
_XFUNCPROTOEND
#else
int sameside(pM,pT,pX,pY,pZ)
point3D* pM; point3D* pT;point3D* pX; point3D* pY; point3D* pZ;
#endif
{
  /* check if the apex and the interior point are on the same side of the base*/
  v3D XM,XY,XZ, XT;
  int sigM,sigT;

  XM.dx=pM->x-pX->x; XM.dy=pM->y-pX->y; XM.dz=pM->z-pX->z;
  XY.dx=pY->x-pX->x; XY.dy=pY->y-pX->y; XY.dz=pY->z-pX->z;
  XZ.dx=pZ->x-pX->x; XZ.dy=pZ->y-pX->y; XZ.dz=pZ->z-pX->z;
  XT.dx=pT->x-pX->x; XT.dy=pT->y-pX->y; XT.dz=pT->z-pX->z;

  sigM=sig(v_mp(&XM,&XY,&XZ));
  sigT=sig(v_mp(&XT,&XY,&XZ));

  if (sigM==sigT || sigM==0)  return YES;
  else                        return NO;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int isT4(point3D* pT, point3D* pX, point3D* pY, point3D* pZ)
_XFUNCPROTOEND
#else
int isT4(pT,pX,pY,pZ)
point3D* pT; point3D* pX; point3D* pY; point3D* pZ;
#endif
{
  /* check if a T4 really is a T4 or is actually a planar object */
  v3D XY,XZ,XT;
  int sigT;
  XY.dx=pY->x-pX->x; XY.dy=pY->y-pX->y; XY.dz=pY->z-pX->z;
  XZ.dx=pZ->x-pX->x; XZ.dy=pZ->y-pX->y; XZ.dz=pZ->z-pX->z;
  XT.dx=pT->x-pX->x; XT.dy=pT->y-pX->y; XT.dz=pT->z-pX->z;

  sigT=sig(v_mp(&XT,&XY,&XZ));

  if (sigT!=0)  return YES;
  else          return NO;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void getit(point3D* pM, point3D* pA, point3D* pB, point3D* pC, point3D* pD)
_XFUNCPROTOEND
#else
void getit(pM,pA,pB,pC,pD)
point3D* pM; point3D* pA; point3D* pB; point3D* pC; point3D* pD;
#endif
{
  /* get the time for M which should be inside the T4 (ABCD) */
  float xMA,yMA,zMA, xBA,yBA,zBA,tBA,lBA, xCA,yCA,zCA,tCA,lCA, xDA,yDA,zDA,tDA,lDA;
  float m[9];
  float detxyz,detyzt,detxzt,detxyt, detyzl,detxzl,detxyl;

  xMA=pM->x-pA->x; yMA=pM->y-pA->y; zMA=pM->z-pA->z;
  xBA=pB->x-pA->x; yBA=pB->y-pA->y; zBA=pB->z-pA->z; tBA=pB->t-pA->t; lBA=pB->l-pA->l;
  xCA=pC->x-pA->x; yCA=pC->y-pA->y; zCA=pC->z-pA->z; tCA=pC->t-pA->t; lCA=pC->l-pA->l;
  xDA=pD->x-pA->x; yDA=pD->y-pA->y; zDA=pD->z-pA->z; tDA=pD->t-pA->t; lDA=pD->l-pA->l;

  m[0]=xBA; m[1]=yBA; m[2]=zBA;
  m[3]=xCA; m[4]=yCA; m[5]=zCA;
  m[6]=xDA; m[7]=yDA; m[8]=zDA;  detxyz=det3(m);

  m[0]=yBA; m[1]=zBA; m[2]=tBA;
  m[3]=yCA; m[4]=zCA; m[5]=tCA;
  m[6]=yDA; m[7]=zDA; m[8]=tDA;  detyzt=det3(m);
  m[0]=yBA; m[1]=zBA; m[2]=lBA;
  m[3]=yCA; m[4]=zCA; m[5]=lCA;
  m[6]=yDA; m[7]=zDA; m[8]=lDA;  detyzl=det3(m);
  
  m[0]=xBA; m[1]=zBA; m[2]=tBA;
  m[3]=xCA; m[4]=zCA; m[5]=tCA;
  m[6]=xDA; m[7]=zDA; m[8]=tDA;  detxzt=det3(m);
  m[0]=xBA; m[1]=zBA; m[2]=lBA;
  m[3]=xCA; m[4]=zCA; m[5]=lCA;
  m[6]=xDA; m[7]=zDA; m[8]=lDA;  detxzl=det3(m);

  m[0]=xBA; m[1]=yBA; m[2]=tBA;
  m[3]=xCA; m[4]=yCA; m[5]=tCA;
  m[6]=xDA; m[7]=yDA; m[8]=tDA;  detxyt=det3(m);
  m[0]=xBA; m[1]=yBA; m[2]=lBA;
  m[3]=xCA; m[4]=yCA; m[5]=lCA;
  m[6]=xDA; m[7]=yDA; m[8]=lDA;  detxyl=det3(m);

  pM->t=pA->t + (detyzt/detxyz)*xMA - (detxzt/detxyz)*yMA + (detxyt/detxyz)*zMA;
  pM->l=pA->l + (detyzl/detxyz)*xMA - (detxzl/detxyz)*yMA + (detxyl/detxyz)*zMA;
}


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int integerup(float x, float ox, float dx)
_XFUNCPROTOEND
#else
int integerup(x,ox,dx)
float x; float ox; float dx;
#endif
{
  int n;
  if ( (int)((x-ox)/dx)==(x-ox)/dx )
    n=(x-ox)/dx;
  else
    n=(int)((x-ox)/dx)+1;
  return n;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int integerdn(float x, float ox, float dx)
_XFUNCPROTOEND
#else
int integerdn(x,ox,dx)
float x; float ox; float dx;
#endif
{
  int n;
  n=(int)((x-ox)/dx);
  return n;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int intnn(float x, float ox, float dx)
_XFUNCPROTOEND
#else
int intnn(x,ox,dx)
float x; float ox; float dx;
#endif
{
  int n;
  n=(int)((x-ox)/dx+0.5);
  return n;
}

















