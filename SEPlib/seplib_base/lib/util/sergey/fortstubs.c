#include <sergey_rsf.h>
#include <cfortran.h>

void pwdsl_lop_t(int,int,int,int,float*,float*);
void trianglen_lop_t(int,int,int,int,float*,float*);
FCALLSCSUB3(dip_set,SF_PWDSL_DIP_SET,sf_pwdsl_dip_set,PFLOAT,INT,INT)
FCALLSCSUB0(pwdsl_close,SF_PWDSL_CLOSE,sf_pwdsl_close)
FCALLSCSUB5(pwdsl_init,SF_PWDSL_INIT,sf_pwdsl_init,INT,INT,INT,INT,FLOAT)
FCALLSCSUB3(trianglen_init,SF_TRIANGLEN_INIT,sf_trianglen_init,INT,PINT,PINT)
FCALLSCSUB6(pwdsl_lop_t,SF_PWDSL_LOP,sf_pwdsl_lop,INT,INT,INT,INT,PFLOAT,PFLOAT)
FCALLSCSUB6(trianglen_lop_t,SF_TRIANGLEN_LOP,sf_trianglen_lop,INT,INT,INT,INT,PFLOAT,PFLOAT)
void trianglen_lop_t(int adj, int add, int nx, int ny, float *x, float *y)
{
  bool aj,ad;
  if(adj==1) aj=true;
  else aj=false;
  
  if(add==1) ad=true;
  else ad=false;
  trianglen_lop(aj,ad,nx,ny,x,y);
}

void pwdsl_lop_t(int adj, int add, int nx, int ny, float *x, float *y)
{
  bool aj,ad;
  if(adj==1) aj=true;
  else aj=false;
  
  if(add==1) ad=true;
  else ad=false;
  pwdsl_lop(aj,ad,nx,ny,x,y);
}

