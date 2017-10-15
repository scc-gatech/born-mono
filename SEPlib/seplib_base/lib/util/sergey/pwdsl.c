/* Triangle smoothing along slope using plane-wave destruction */
/*
  Copyright (C) 2004 University of Texas at Austin
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <math.h>

#include <sergey_rsf.h>
/*^*/


static int n2, n1, n, k2;
static float *tmp1, *tmp2, amp,**dip2d;

void pwdsl_init(int m1, int m2       /* data dimensions */, 
		int rect1, int rect2 /* triangle radius */,
		float eps            /* regularization parameter */)
/*< initialize >*/
{
    predict_init (m1,m2,eps,rect2);
    sf_triangle1_init(rect1,m1);

    n2 = m2+2*rect2;
    n = m1*n2;

    repeat_init(m1,n2,sf_triangle1_lop);

    tmp1 = sf_floatalloc(n);
    tmp2 = sf_floatalloc(n);

    n2 = m2;
    n1 = m1;
    k2 = rect2;
    amp = 1./rect2;
}

void pwdsl_set(float **dip /* dip field [n2][n1] */)
/*< set the local slopes for applying the linear operator >*/
{
    predict_set(dip);
}
void dip_set(float *dip,int n1,int n2)
{
dip2d=sf_floatalloc2(n1,n2);
memcpy((void*)dip2d[0],(const void*)dip,sizeof(float)*n1*n2);
pwdsl_set(dip2d);
}

void pwdsl_close(void)
/*< free allocated storage >*/
{
    sf_triangle1_close();
    predict_close();
    
    free(tmp1);
    free(tmp2);
}

void pwdsl_lop(bool adj, bool add, int nx, int ny, float* x, float* y)
/*< linear operator >*/
{
    int i;
    float *inp, *out;

    sf_adjnull(adj,add,nx,ny,x,y);

    if (adj) {
	inp = y;
	out = x;
    } else {
	inp = x;
	out = y;
    }

    for (i=0; i < k2*n1; i++) {
	tmp1[i] = 0.;
	tmp1[i+(k2+n2)*n1] = 0.;
    }



    for (i=0; i < n2*n1; i++) {
	tmp1[i+k2*n1] = amp*inp[i];
    }

    predicter_lop  (true,  false, n, n, tmp2, tmp1);
    subtracter_lop (true,  false, n, n, tmp1, tmp2);
    repeat_lop     (true,  false, n, n, tmp2, tmp1);
    subtracter_lop (false, false, n, n, tmp2, tmp1);
    predicter_lop  (false, false, n, n, tmp1, tmp2);

    for (i=0; i < n2*n1; i++) {
	out[i] += amp*tmp2[i+k2*n1];
    }
}
