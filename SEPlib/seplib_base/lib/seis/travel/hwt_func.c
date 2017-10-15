#include <math.h>
#include <sepmath.h>
#include <septravel.h>
#include <hwt.h>

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int signum(double n)
_XFUNCPROTOEND
#else
int signum(n)
double n;
#endif
{  /*returns the sign of a double (positive if zero) */
  if(n>=0) return 1;
/*   else if (n==0) return 0; */
  else return -1;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double v_l(v3D* pV)
_XFUNCPROTOEND
#else
double v_l(pV)
v3D* pV;
#endif
{  /*returns the length of a vector*/
  return sqrt ( (pV->dx)*(pV->dx) +
                (pV->dy)*(pV->dy) +
                (pV->dz)*(pV->dz) );
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double v_l2(v3D* pV)
_XFUNCPROTOEND
#else
double v_l2(pV)
v3D* pV;
#endif
{  /*returns the length squared of a vector*/
  return (pV->dx)*(pV->dx) +
         (pV->dy)*(pV->dy) +
         (pV->dz)*(pV->dz);
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double v_sina(v3D* pV1, v3D* pV2)
_XFUNCPROTOEND
#else
double v_sina(pV1,pV2)
v3D* pV1; v3D* pV2;
#endif
{  /*returns the sine of the angle between two vectors */
v3D v=v_vp(pV1,pV2);
  return v_l(&v)/(v_l(pV1)*v_l(pV2));
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double v_cosa(v3D* pV1, v3D* pV2)
_XFUNCPROTOEND
#else
double v_cosa(pV1,pV2)
v3D* pV1; v3D* pV2;
#endif
{  /* returns the cosine of the angle between two vectors */
  return v_dp(pV1,pV2)/(v_l(pV1)*v_l(pV2));
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double v_a(v3D* pV1, v3D* pV2)
_XFUNCPROTOEND
#else
double v_a(pV1,pV2)
v3D* pV1; v3D* pV2;
#endif
{  /* returns the measure of the angle between two vectors */
int sdp=signum(v_cosa(pV1,pV2));
  return 180*((1-sdp)*PI/2+sdp*asin((float)v_sina(pV1,pV2)))/PI;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double v_dp(v3D* pV1, v3D* pV2)
_XFUNCPROTOEND
#else
double v_dp(pV1,pV2)
v3D* pV1; v3D* pV2;
#endif
{  /*returns the dot product of two vectors*/
  return (pV1->dx)*(pV2->dx) +
         (pV1->dy)*(pV2->dy) +
         (pV1->dz)*(pV2->dz);
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
v3D v_vp(v3D* pV1, v3D* pV2)
_XFUNCPROTOEND
#else
v3D v_vp(pV1, pV2)
v3D* pV1; v3D* pV2;
#endif
{  /*returns the vector product of two vectors*/
v3D v;
  v.dx=(pV1->dy*pV2->dz) - (pV2->dy*pV1->dz);
  v.dy=(pV2->dx*pV1->dz) - (pV1->dx*pV2->dz);
  v.dz=(pV1->dx*pV2->dy) - (pV2->dx*pV1->dy);

  return v;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double v_mp(v3D* pV1, v3D* pV2, v3D* pV3)
_XFUNCPROTOEND
#else
double v_mp(pV1,pV2,pV3)
v3D* pV1, *pV2, *pV3;
#endif
{  /*returns the signed mixt product of three vectors (v1 x v2) * v3 */
  return( (pV3->dx)*(pV1->dy)*(pV2->dz)
        -(pV3->dx)*(pV2->dy)*(pV1->dz)
        +(pV2->dx)*(pV3->dy)*(pV1->dz)
        -(pV2->dx)*(pV1->dy)*(pV3->dz)
        +(pV1->dx)*(pV2->dy)*(pV3->dz)
        -(pV1->dx)*(pV3->dy)*(pV2->dz));
}
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double volume(v3D* pV1, v3D* pV2, v3D* pV3)
_XFUNCPROTOEND
#else
double volume(pV1,pV2,pV3)
v3D* pV1; v3D* pV2; v3D* pV3;
#endif
{  /*returns the volume of the T4 built on v1,v2,v3 */
  double t;

  t =(pV3->dx)*(pV1->dy)*(pV2->dz)
    -(pV3->dx)*(pV2->dy)*(pV1->dz)
    +(pV2->dx)*(pV3->dy)*(pV1->dz)
    -(pV2->dx)*(pV1->dy)*(pV3->dz)
    +(pV1->dx)*(pV2->dy)*(pV3->dz)
    -(pV1->dx)*(pV3->dy)*(pV2->dz);

  return signum(t)*t/6;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float det3(float *m)
_XFUNCPROTOEND
#else
float det3(m)
float *m;
#endif
{
  float pp,mm;

  pp=m[0]*m[4]*m[8]+m[3]*m[2]*m[7]+m[1]*m[6]*m[5];
  mm=m[2]*m[4]*m[6]+m[1]*m[3]*m[8]+m[0]*m[5]*m[7];

  return pp-mm;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float det2(float *m)
_XFUNCPROTOEND
#else
float det2(m)
float *m;
#endif
{
  return m[0]*m[3]-m[1]*m[2];
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void set_cube(cube p, float val)
_XFUNCPROTOEND
#else
void set_cube(p,val)
cube p; float val;
#endif
{
int i;
for(i=0; i < p.n1 * p.n2 * p.n3;i++) p.c0[i]=val;

}













