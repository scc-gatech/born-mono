#include <sepConfig.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <math.h>
#include <sepmath.h>
#include <septravel.h>
#include <hwt.h>
extern int sepwarn(int, const char *, ... );

/*$

=head1 NAME

hwt_trace_rays - trace rays

=head1 SYNOPSIS

C<int hwt_trace_rays(int *n,float *o,float *d,float *shot,float aimi,float aima,
 float oi,float si,int ni, float sa, int na,float st,int nt,float *vel,
 float *xray,float *yray,float *zray,float *vray)>

=head1 INPUT PARAMETERS

=over 4

=item n -  int[3]    

      Dimensions of elements in velocity cube (3)

=item o -  float[3]  

      First element of velocity cube (3)

=item d -  float[3] 

      Sampling of velocity cube velocity cube (3)

=item shot -  float[3] 

      Location of shot

=item aimi -  float    

      Inclination

=item aima -  float    

      Azimuth

=item oi -  float    

      First aperture angle

=item si -  float    

      Sampling  of aperture

=item ni -  int      

      Number of half aperture angles

=item sa -  float    

      Sampling of radial steps

=item na -  int      

      Number of radial steps

=item st -  float    

      Sampling in time

=item nt -  int      

      Number of time samples

=item vel -  float*   

      Velocity

=back

=head1 OUTPUT PARAMETERS

=over 4

=item xray -  float*   

      X position of ray

=item yray -  float*   

      Y position of ray

=item zray -  float*   

      Z position of ray

=item vray -  float*   

      Velocity of ray at positon

=back

=head1 DESCRIPTION

Trace rays using Huegyn wavefrom raytracing

=head1 SEE ALSO

L<hwt_travel_cube>, L<Hwt3d>

=head1 LIBRARY

B<septravel>

=cut

*/

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int hwt_trace_rays(int *n,float *o,float *d,float *shot,float aimi,float aima,
 float oi,float si,int ni, float sa, int na,float st,int nt,float *vel,
 float *xray,float *yray,float *zray,float *vray)
_XFUNCPROTOEND
#else
int hwt_trace_rays(n,o,d,shot,aimi,aima,oi,si,ni,sa,na,st,nt,vel,xray,yray,zray,vray)
int *n;float *o;float *d;float *shot;float aimi;float aima;
 float oi;float si;int ni; float sa; int na;float st;int nt;float *vel;
 float *xray;float *yray;float *zray;float *vray;
#endif
{
rays pRR;cube pVV;
/*init rays */

/*init velocity*/
init_rays(&pRR,shot,nt,st,aimi,aima,oi,si,ni,sa,na);
init_cube(&pVV,vel,n,o,d);
tracerays(&pRR,&pVV);
transfer_rays(&pRR,xray,yray,zray,vray);
return(0);
}

/*$

=head1 NAME

hwt_travel_cube - get travel times 

=head1 SYNOPSIS

C<int hwt_travel_cube(int *n,float *o,float *d,float *shot,float aimi,float aima,
 float oi,float si,int ni, float sa, int na,float st,int nt,float *vel,
 int quick, float *travel)>

=head1 INPUT PARAMETERS

=over 4

=item n -  int[3]    

      Dimensions of elements in velocity cube (3)

=item o -  float[3]  

      First element of velocity cube (3)

=item d -  float[3] 

      Sampling of velocity cube velocity cube (3)

=item shot -  float[3] 

      Location of shot

=item aimi -  float    

      Inclination

=item aima -  float    

      Azimuth

=item oi -  float    

      First aperture angle

=item si -  float    

      Sampling  of aperture

=item ni -  int      

      Number of half aperture angles

=item sa -  float    

      Sampling of radial steps

=item na -  int      

      Number of radial steps

=item st -  float    

      Sampling in time

=item nt -  int      

      Number of time samples

=item vel -  float*   

      Velocity

=item quick -  int      

      Whether or not do quick interpolation

=back

=head1 OUTPUT PARAMETERS

=over 4

=item travel -  float*   

      Travel time cube

=back

=head1 DESCRIPTION

Do Huygens wavefront tracing and then interpolate


=head1 SEE ALSO

hwt_trace_rays, Hwt3d

=head1 LIBRARY

B<septravel>

=cut

*/

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int hwt_travel_cube(int *n,float *o,float *d,float *shot,float aimi,float aima,
 float oi,float si,int ni, float sa, int na,float st,int nt,float *vel,
	int fast,float *travel) 
_XFUNCPROTOEND
#else
int hwt_travel_cube(n,o,d,shot,aimi,aima,oi,si,ni,sa,na,st,nt,vel,fast,travel)
int *n;float *o;float *d;float *shot;float aimi;float aima;
 float oi;float si;int ni; float sa; int na;float st;int nt;float *vel;
	int fast;float *travel;
#endif
{
rays pRR;cube pVV;
/*init rays */

/*init velocity*/
init_rays(&pRR,shot,nt,st,aimi,aima,oi,si,ni,sa,na);
init_cube(&pVV,vel,n,o,d);
tracerays(&pRR,&pVV);
tesselate_rays(&pRR,&pVV,travel,fast);
return(0);
}








#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void transfer_rays(rays *pRR, float *xrays, float *yrays, float *zrays, float *vrays)
_XFUNCPROTOEND
#else
void transfer_rays(pRR,xrays,yrays,zrays,vrays)
rays *pRR;
float *xrays, *yrays,  *zrays,  *vrays;
#endif
{
int ii,ia,it,idata;
idata=0;

for(ii=0;ii<pRR->ni;ii++){
  for(ia=0;ia<pRR->na;ia++){
    for(it=0;it<pRR->nt;it++) {
		  xrays[idata]=pRR->c0[ii][ia][it].x;
		  yrays[idata]=pRR->c0[ii][ia][it].y;
		  zrays[idata]=pRR->c0[ii][ia][it].z;
		  vrays[idata++]=pRR->c0[ii][ia][it].v;
     }
	free(pRR->c0[ii][ia]);
	}
 free(pRR->c0[ii]);
}
free(pRR->c0);

return;

}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
 int init_cube(cube *pVV,float *vel,int *n,float *o,float *d)
_XFUNCPROTOEND
#else
int init_cube(pVV,vel,n,o,d)
cube *pVV;
int *n; 
float *o,*d,*vel;
#endif
{
pVV->n1=n[0]; pVV->o1=o[0]; pVV->d1=d[0];
pVV->n2=n[1]; pVV->o2=o[1]; pVV->d2=d[1];
pVV->n3=n[2]; pVV->o3=o[2]; pVV->d3=d[2];

pVV->c0=vel;

return(0);
}
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void init_rays(rays *pRR,float *shot,int nt,float st,float aimi, float aima,float oi,float si,int ni,float sa,int na)
_XFUNCPROTOEND
#else
void init_rays(pRR,shot,nt,st,aimi,aima,oi,si,ni,sa,na)
rays *pRR;
float *shot;int nt;float st;float aimi; float aima;float oi;float si;int ni;float sa;int na;
#endif
{

sepwarn( 0, "Top of init_rays()\n");
pRR->xshot=shot[0];
pRR->yshot=shot[1];
pRR->zshot=shot[2];

pRR->nt=nt;
pRR->ot=0.;
pRR->st=st;

pRR->aimi=aimi;
pRR->aima=aima;

pRR->oi=oi;
pRR->si=si;
pRR->ni=ni;

pRR->oa=0.0;
pRR->sa=sa;
pRR->na=na;

allocate_rays(pRR);
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void allocate_rays(rays* pRR)
_XFUNCPROTOEND
#else
void allocate_rays(pRR)
rays* pRR;
#endif
{
  int ii,ia;

  pRR->c0=(point3D***) malloc(pRR->ni*sizeof(point3D**));
  for(ii=0;ii<pRR->ni;ii++) {
    pRR->c0[ii]=(point3D**) malloc(pRR->na*sizeof(point3D*));
  }
  for(ii=0;ii<pRR->ni;ii++) {
    for(ia=0;ia<pRR->na;ia++) {
      pRR->c0[ii][ia]=(point3D*) malloc(pRR->nt*sizeof(point3D));
    }}
}

