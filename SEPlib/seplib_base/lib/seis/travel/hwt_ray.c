#include <math.h>
#include <sepmath.h>
#include <septravel.h>
#include <hwt.h>
#include<seplib.h>

extern int sjacob(point3D *C,point3D *T,point3D *P, point3D *Q);

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void tracerays(rays* pRR,cube* pVV)
_XFUNCPROTOEND
#else
void tracerays(pRR,pVV)
rays* pRR;cube* pVV;
#endif
{
  int it,ii,ia;


  init_wf(pRR,pVV);

  for( it=1; it<pRR->nt-1; it++ ) {     /* loop over time */
    for( ii=1; ii<pRR->ni-1; ii++ ) {   /* loop over inclination angles */
      for( ia=0; ia<pRR->na; ia++ ) {   /* loop over azimuth angles */

        if(iscusp(pRR,it,ii,ia) || gottooclose(pRR,pVV,it,ii,ia) ) {
          pRR->c0[ii][ia][it+1]=raytr(pRR,pVV,it,ii,ia);
        } else {
          pRR->c0[ii][ia][it+1]=wfttr(pRR,pVV,it,ii,ia);
        }

      }
    }
  }


}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void init_wf(rays* pRR,cube* pVV)
_XFUNCPROTOEND
#else
void init_wf(pRR,pVV)
rays* pRR;
cube* pVV;
#endif
{
  float vtmp,vcur;
  float a,r,alpha,beta;
  float D,x0,y0,z0,x1,y1,z1,x2,y2,z2;
  int it,ii,ia;

  char* tmp;



  for( ii=0; ii<pRR->ni; ii++) {
    for( ia=0; ia<pRR->na; ia++) {
      for( it=0; it<pRR->nt; it++) {
        pRR->c0[ii][ia][it].x=ABSENT;
        pRR->c0[ii][ia][it].y=ABSENT;
        pRR->c0[ii][ia][it].z=ABSENT;
        pRR->c0[ii][ia][it].v=ABSENT;
      }
    }
  }

  
  it=0;
  pRR->c0[0][0][it].x = pRR->xshot;
  pRR->c0[0][0][it].y = pRR->yshot;
  pRR->c0[0][0][it].z = pRR->zshot;
  vtmp = get_vel_lin( pVV, &(pRR->c0[0][0][it]) );


  for(ii=0;ii<pRR->ni;ii++)
    for(ia=0;ia<pRR->na;ia++) {
      pRR->c0[ii][ia][it].x=pRR->xshot;
      pRR->c0[ii][ia][it].y=pRR->yshot;
      pRR->c0[ii][ia][it].z=pRR->zshot;
      pRR->c0[ii][ia][it].v=vtmp;
    }
  

  it=1;
  for(ii=0;ii<pRR->ni;ii++)
    for(ia=0;ia<pRR->na;ia++) {
      vcur=pRR->c0[ii][ia][it-1].v;
      D=pRR->st*vcur;
      
      a=PI*(pRR->oi+ii*pRR->si)/180;
      r=PI*(pRR->oa+ia*pRR->sa)/180;	
      alpha=PI*(pRR->aima)/180;
      beta =PI*(pRR->aimi)/180;
      
      x0=D*sin(a)*cos(r);
      y0=D*sin(a)*sin(r);
      z0=D*cos(a);
      
      x1=x0*cos(beta)+z0*sin(beta);
      y1=y0;
      z1=z0*cos(beta)-x0*sin(beta);
      
      x2=x1*cos(alpha)-y1*sin(alpha);
      y2=x1*sin(alpha)+y1*cos(alpha);
      z2=z1;
      
      pRR->c0[ii][ia][it].x=pRR->c0[ii][ia][it-1].x+x2;
      pRR->c0[ii][ia][it].y=pRR->c0[ii][ia][it-1].y+y2;
      pRR->c0[ii][ia][it].z=pRR->c0[ii][ia][it-1].z+z2;
      pRR->c0[ii][ia][it].v=get_vel_lin(pVV,&(pRR->c0[ii][ia][it]));
    }
  
  /*set (x,y,z) for boundaries */
  for(it=1;it<pRR->nt-1;it++) {
    for(ia=0;ia<pRR->na;ia++) {
      ii=0;	     pRR->c0[ii][ia][it+1]=raytr(pRR,pVV,it,ii,ia);
      ii=pRR->ni-1;  pRR->c0[ii][ia][it+1]=raytr(pRR,pVV,it,ii,ia);
    }}


}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
point3D raytr(rays* pRR,cube* pVV,int it, int ii, int ia)
_XFUNCPROTOBEGIN
#else
point3D raytr(pRR,pVV,it,ii,ia)
rays* pRR;cube* pVV;int it,ii,ia;
#endif
{
  float xn,yn,zn,rn,xm,ym,zm,rm;
  float denom,x1,y1,z1,x2,y2,z2;
  double ax,bx,ay,by,a,b,c,delta;
  point3D T,C,S1,S2,S,N1,N2,M1,M2;
  float a1,a2,LL,LL0;  
  v3D v1,v2,vT,vx,vz;
  float aa,bb;
  float deps;
  float wico=2;

  C=pRR->c0[ii][ia][it];  
  T=pRR->c0[ii][ia][it-1];

  /* unitary vectors */
  vz.dx=0;vz.dy=0;vz.dz=1;
  vx.dx=1;vx.dy=0;vx.dz=0;
  
  vT.dx=C.x-T.x; 
  vT.dy=C.y-T.y; 
  
  vT.dz=C.z-T.z;  LL =wico*v_l(&vT); /* the vector T->C */
  aa=v_a(&vT,&vz); /* angle between TC and the vertical axis */
  
  vT.dz=0;        LL0=wico*v_l(&vT); /* horizontal projection of TC */
  if (LL0!=0) 
    bb=signum(v_mp(&vT,&vx,&vz))*v_a(&vT,&vx);
  else   /* angle between the horizontal projection of TC and the x axis */
    bb=0;
  
  aa=PI*aa/180; bb=PI*bb/180; 
  
  /* points in the vertical plane containing the ray*/
  N1.x=C.x+LL*cos(aa)*cos(bb);           N2.x=C.x-LL*cos(aa)*cos(bb);
  N1.y=C.y-LL*cos(aa)*sin(bb);           N2.y=C.y+LL*cos(aa)*sin(bb);
  N1.z=C.z-LL*sin(aa);                   N2.z=C.z+LL*sin(aa);
  N1.v=get_vel_lin(pVV,&N1);             N2.v=get_vel_lin(pVV,&N2);
  
  /* points in a horizontal plane */
  M1.x=C.x-LL*sin(bb);                   M2.x=C.x+LL*sin(bb); 
  M1.y=C.y-LL*cos(bb);                   M2.y=C.y+LL*cos(bb);
  M1.z=C.z;                              M2.z=C.z;
  M1.v=get_vel_lin(pVV,&M1);             M2.v=get_vel_lin(pVV,&M2);
  
  xn= N2.x-N1.x;                         xm= M2.x-M1.x;
  yn= N2.y-N1.y;                         ym= M2.y-M1.y;
  zn= N2.z-N1.z;                         zm= M2.z-M1.z;
  rn=(N1.v-N2.v)*pRR->st;                rm=(M1.v-M2.v)*pRR->st;

  deps=0.01*v_l(&vT);
  denom=xn*ym-xm*yn;
  if (denom==0) denom=deps;
  
  ax=(rn*ym-rm*yn)/denom; bx=(zn*ym-zm*yn)/denom;
  ay=(xn*rm-xm*rn)/denom; by=(xn*zm-xm*zn)/denom;
  
  a=1+bx*bx+by*by; b=ax*bx+ay*by; c=ax*ax+ay*ay-1;
  delta=b*b-a*c;
  if(delta<0) {
    delta=0; 
    seperr("Trouble! Imaginary solutions...\n");
  }

  /* line intersects sphere = 1 */
  z1=(b+sqrt(delta))/a;	  S1.z=(C.z)+(C.v)*pRR->st*z1; 
  x1=ax-bx*z1;            S1.x=(C.x)+(C.v)*pRR->st*x1;
  y1=ay-by*z1;            S1.y=(C.y)+(C.v)*pRR->st*y1;
  
  /* line intersects sphere = 2 */
  z2=(b-sqrt(delta))/a;	  S2.z=(C.z)+(C.v)*pRR->st*z2; 
  x2=ax-bx*z2;            S2.x=(C.x)+(C.v)*pRR->st*x2;
  y2=ay-by*z2;            S2.y=(C.y)+(C.v)*pRR->st*y2;
  
  vT.dx= T.x-C.x;  v1.dx=S1.x-C.x;  v2.dx=S2.x-C.x;
  vT.dy= T.y-C.y;  v1.dy=S1.y-C.y;  v2.dy=S2.y-C.y;
  vT.dz= T.z-C.z;  v1.dz=S1.z-C.z;  v2.dz=S2.z-C.z;
                   a1=v_a(&v1,&vT); a2=v_a(&v2,&vT);
  
  if(a1>a2) S=S1;
  else S=S2;
  
  S.v=get_vel_lin(pVV,&S);
  return S;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
point3D wfttr(rays* pRR,cube* pVV,int it, int ii, int ia)
_XFUNCPROTOBEGIN
#else
point3D wfttr(pRR,pVV,it, ii, ia)
rays* pRR;cube* pVV;int it, ii, ia;
#endif
{
  float xn,yn,zn,rn,xm,ym,zm,rm;
  float denom,x1,y1,z1,x2,y2,z2;
  double ax,bx,ay,by,a,b,c,delta;
  point3D T,C,S1,S2,S,N1,N2,M1,M2;
  float a1,a2;  
  v3D v1,v2,vT;
  float deps;

  C=pRR->c0[ii][ia][it];
  T=pRR->c0[ii][ia][it-1];

  N1=(ii!=0        )?pRR->c0[ii-1][ia  ][it]:pRR->c0[pRR->ni-1][ia       ][it];
  N2=(ii!=pRR->ni-1)?pRR->c0[ii+1][ia  ][it]:pRR->c0[0        ][ia       ][it];
  M1=(ia!=0        )?pRR->c0[ii  ][ia-1][it]:pRR->c0[ii       ][pRR->na-1][it];
  M2=(ia!=pRR->na-1)?pRR->c0[ii  ][ia+1][it]:pRR->c0[ii       ][0        ][it];

  xn=(N2.x)-(N1.x);            xm=(M2.x)-(M1.x);
  yn=(N2.y)-(N1.y);            ym=(M2.y)-(M1.y);
  zn=(N2.z)-(N1.z);            zm=(M2.z)-(M1.z);
  rn=((N2.v)-(N1.v))*pRR->st;  rm=((M2.v)-(M1.v))*pRR->st;

  deps=0.01*(rn+rm)/2;
  denom=xn*ym-xm*yn;
  if(denom==0) denom=deps;

  ax=(rn*ym-rm*yn)/denom;  bx=(zn*ym-zm*yn)/denom;
  ay=(xn*rm-xm*rn)/denom;  by=(xn*zm-xm*zn)/denom;
  
  a=1+bx*bx+by*by;
  b=ax*bx+ay*by;
  c=ax*ax+ay*ay-1;

  delta=b*b-a*c;
  if(delta<=0) {
    delta=0;
    seperr("Trouble! Imaginary solutions...\n");
  }
  
  z1=(b+sqrt(delta))/a;	S1.z=(C.z)+(C.v)*pRR->st*z1; 
  x1=ax-bx*z1;          S1.x=(C.x)+(C.v)*pRR->st*x1;
  y1=ay-by*z1;          S1.y=(C.y)+(C.v)*pRR->st*y1;

  z2=(b-sqrt(delta))/a;	S2.z=(C.z)+(C.v)*pRR->st*z2; 
  x2=ax-bx*z2;          S2.x=(C.x)+(C.v)*pRR->st*x2;
  y2=ay-by*z2;          S2.y=(C.y)+(C.v)*pRR->st*y2;

  vT.dx= T.x-C.x; v1.dx=S1.x-C.x; v2.dx=S2.x-C.x;
  vT.dy= T.y-C.y; v1.dy=S1.y-C.y; v2.dy=S2.y-C.y;
  vT.dz= T.z-C.z; v1.dz=S1.z-C.z; v2.dz=S2.z-C.z;

  a1=v_a(&v1,&vT);
  a2=v_a(&v2,&vT);

  if(a1>a2) S=S1;
  else S=S2;

  S.v=get_vel_lin(pVV,&S);

  return S;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int iscusp(rays* pRR,int it, int ii, int ia)
_XFUNCPROTOEND
#else
int iscusp(pRR,it,ii, ia)
rays* pRR;int it, ii,ia;
#endif
{
  point3D T,C,N1,N2,M1,M2;
  int sjN1M1,sjN1M2, sjN2M1,sjN2M2, sjM1N1,sjM1N2, sjM2N1,sjM2N2;

  C=pRR->c0[ii][ia][it];
  T=pRR->c0[ii][ia][it-1];

  N1=(ii!=0        )?pRR->c0[ii-1][ia  ][it]:pRR->c0[pRR->ni-1][ia       ][it];
  N2=(ii!=pRR->ni-1)?pRR->c0[ii+1][ia  ][it]:pRR->c0[0        ][ia       ][it];
  M1=(ia!=0        )?pRR->c0[ii  ][ia-1][it]:pRR->c0[ii       ][pRR->na-1][it]; 
  M2=(ia!=pRR->na-1)?pRR->c0[ii  ][ia+1][it]:pRR->c0[ii       ][0        ][it];
  
  sjN1M1=sjacob(&C,&T,&N1,&M1); sjN1M2=sjacob(&C,&T,&N1,&M2);
  sjN2M1=sjacob(&C,&T,&N2,&M1); sjN2M2=sjacob(&C,&T,&N2,&M2);
  sjM1N1=sjacob(&C,&T,&M1,&N1); sjM1N2=sjacob(&C,&T,&M1,&N2);
  sjM2N1=sjacob(&C,&T,&M2,&N1); sjM2N2=sjacob(&C,&T,&M2,&N2);

  if (sjN1M1*sjN1M2==1 || sjN2M1*sjN2M2==1 || sjM1N1*sjM1N2==1 || sjM2N1*sjM2N2==1 )
    return YES;
  else
    return NO;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int sjacob(point3D *C,point3D *T,point3D *P, point3D *Q)
_XFUNCPROTOBEGIN
#else
int sjacob(C,T,P, Q)
point3D *C;point3D *T;point3D *P; point3D *Q;
#endif
{
  float m[9];
  m[0]=C->x-P->x;  m[1]=C->x-Q->x;  m[2]=C->x-T->x;
  m[3]=C->y-P->y;  m[4]=C->y-Q->y;  m[5]=C->y-T->y;
  m[6]=C->z-P->z;  m[7]=C->z-Q->z;  m[8]=C->z-T->z;

  return signum(det3(m));
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int gottooclose(rays* pRR,cube *pVV,int it,int ii,int ia)
_XFUNCPROTOEND
#else
int gottooclose(pRR,pVV,it,ii,ia)
rays* pRR;cube *pVV;int it;int ii;int ia;
#endif
{
  point3D N1,N2,M1,M2;
  float dN,dM,d;

  d=MIN(pVV->d1,MIN(pVV->d2,pVV->d3));

  N1=(ii!=0        )?pRR->c0[ii-1][ia  ][it]:pRR->c0[pRR->ni-1][ia       ][it];
  N2=(ii!=pRR->ni-1)?pRR->c0[ii+1][ia  ][it]:pRR->c0[0        ][ia       ][it];
  M1=(ia!=0        )?pRR->c0[ii  ][ia-1][it]:pRR->c0[ii       ][pRR->na-1][it]; 
  M2=(ia!=pRR->na-1)?pRR->c0[ii  ][ia+1][it]:pRR->c0[ii       ][0        ][it];

  dN=sqrt((N1.x-N2.x)*(N1.x-N2.x)+(N1.y-N2.y)*(N1.y-N2.y)+(N1.z-N2.z)*(N1.z-N2.z));
  dM=sqrt((M1.x-M2.x)*(M1.x-M2.x)+(M1.y-M2.y)*(M1.y-M2.y)+(M1.z-M2.z)*(M1.z-M2.z));

  if (dN<=2*d || dM<=2*d)
    return YES;
  else
    return NO;
}


#define VEL(i1,i2,i3) (pVV->c0[i1+i2*pVV->n1+i3*pVV->n1*pVV->n2])
#define VEL2 yoda

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double get_vel_lin(cube* pVV,point3D* p)
_XFUNCPROTOEND
#else
double get_vel_lin(pVV,p)
cube* pVV;point3D* p;
#endif
{
  double x,y,z;
  int i1,i2,i3;
  double v;
  double v000,v0x0,v00y,v0xy,vz00,vzx0,vz0y,vzxy;
  double v00,v0y,vz0,vzy,v0,vz;
  double m,n,q;
  float min2,min3,min1,max2,max3,max1;


  min1=pVV->o1; max1=pVV->o1+(pVV->n1-1)*pVV->d1;
  min2=pVV->o2; max2=pVV->o2+(pVV->n2-1)*pVV->d2;
  min3=pVV->o3; max3=pVV->o3+(pVV->n3-1)*pVV->d3;


  if( p->z <= min1 )  z=min1;
  if( p->z >= max1 - pVV->d1 )  z=max1-pVV->d1;
  if( p->z > min1 && p->z < max1 - pVV->d1 )  z=p->z;

  if( p->x<=min2) x=min2;
  if( p->x>=max2-pVV->d2) x=max2-pVV->d2;
  if( p->x>min2 && p->x<max2-pVV->d2) x=p->x;
  
  if( p->y<=min3) y=min3;
  if( p->y>=max3-pVV->d3) y=max3-pVV->d3;
  if( p->y>min3 && p->y<max3-pVV->d3) y=p->y;

  i1=(int)((z-pVV->o1)/pVV->d1);  
  i2=(int)((x-pVV->o2)/pVV->d2);
  i3=(int)((y-pVV->o3)/pVV->d3);



  v000=1/VEL(i1,i2,i3);
  v0x0=1/VEL(i1,i2+1,i3);
  v00y=1/VEL(i1,i2,i3+1);
  v0xy=1/VEL(i1,i2+1,i3+1);


  vz00=1/VEL(i1+1,i2,i3);
  vzx0=1/VEL(i1+1,i2+1,i3);
  vz0y=1/VEL(i1+1,i2,i3+1);
  vzxy=1/VEL(i1+1,i2+1,i3+1);


  m=(x-(pVV->o2+i2*pVV->d2))/pVV->d2;
  n=(y-(pVV->o3+i3*pVV->d3))/pVV->d3;
  q=(z-(pVV->o1+i1*pVV->d1))/pVV->d1;


  v00=(1-m)*v000+m*v0x0;
  v0y=(1-m)*v00y+m*v0xy;
  vz0=(1-m)*vz00+m*vzx0;
  vzy=(1-m)*vz0y+m*vzxy;


  v0 =(1-n)*v00 +n*v0y ;
  vz =(1-n)*vz0 +n*vzy ;


  v=(1-q)*v0  +q*vz  ;


  return 1/v;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double get_vel_pinc(cube* pVV,point3D* p)
_XFUNCPROTOEND
#else
double get_vel_pinc(pVV,p)
cube* pVV;point3D* p;
#endif
{
  int i1,i2,i3; /* loop variable in the velocity model */
  int j1,j2,j3; /*            local cube loop variable */ 
  int k1,k2,k3; /*  corrected local cube loop variable */ 
  double xloop,yloop,zloop,rloop;
  int lpinc;
  double coef,sv,sc,v;

  i2=(int)(((p->x)-pVV->o2)/pVV->d2);
  i3=(int)(((p->y)-pVV->o3)/pVV->d3);
  i1=(int)(((p->z)-pVV->o1)/pVV->d1);

  lpinc=5;   /* length of the pinc operator */
  sv=0;      /* summ of velocity*coefficients */ 
  sc=0;      /* summ of the coefficients */

  /* loop a local 3-D cube to compute the pinc coefficients */
  for(j2=i2-lpinc;j2<=i2+1+lpinc;j2++)
  for(j3=i3-lpinc;j3<=i3+1+lpinc;j3++)
  for(j1=i1-lpinc;j1<=i1+1+lpinc;j1++)
    {
      /* where am I in the cube? */
      /* values scaled by the grid size */
      xloop=(pVV->o2+pVV->d2*j2-(p->x))/pVV->d2;
      yloop=(pVV->o3+pVV->d3*j3-(p->y))/pVV->d3;
      zloop=(pVV->o1+pVV->d1*j1-(p->z))/pVV->d1;

      /* pinc is radially symmetric - so compute the local radius */
      rloop=sqrt(xloop*xloop+yloop*yloop+zloop*zloop);
      
      /* P coefficients */
      if(rloop>0.001) /* rloop!=0 */
	coef=(sin(PI*rloop)-PI*rloop*cos(PI*rloop))/(2*PI*PI*rloop*rloop*rloop);
      else
	coef=PI/6;
      
      /* summation 1 -> for sc*/
      if(rloop<=sqrt(3.0)*lpinc) sc+=coef;

      /* make sure the local cube of data doesn't exceed the 
	 limits of the velocity cube */
      k2=j2<0?0:(j2>pVV->n2-1?pVV->n2-1:j2);
      k3=j3<0?0:(j3>pVV->n3-1?pVV->n3-1:j3);
      k1=j1<0?0:(j1>pVV->n1-1?pVV->n1-1:j1);

      /* summation 2 -> for sv*/
      if(rloop<=sqrt(3)*lpinc) sv+=coef*VEL(k1,k2,k3);
    }
  /* velocity is the ratio of the two sums */
  v=sv/sc;

  return v;
}
