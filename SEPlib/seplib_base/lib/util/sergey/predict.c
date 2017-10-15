/* Trace prediction with plane-wave destruction */
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

#include <sergey_rsf.h>
/*^*/


static int n1, n2, nb, k2;
static sf_bands slv;
static float *diag, **offd, eps, **dip, *tt;
static pwd w;

static void stepper(bool adj /* adjoint flag */,
		    int i2   /* trace number */);

void predict_init (int nx, int ny /* data size */, 
		   float e        /* regularization parameter */,
		   int k          /* radius */)
/*< initialize >*/
{
    const int nw=1;

    n1 = nx;
    n2 = ny;
    nb = 2*nw;

    eps = e;

    slv = sf_banded_init (n1, nb);
    diag = sf_floatalloc (n1);
    offd = sf_floatalloc2 (n1,nb);

    w = pwd_init (n1, nw);
    tt = NULL;

    k2 = k;
    if (k2 > n2-1) sf_error("%s: k2=%d > n2-1=%d",__FILE__,k2,n2-1);
}

void predict_close (void)
/*< free allocated storage >*/
{
    sf_banded_close (slv);
    free (diag);
    free (*offd);
    free (offd);
    pwd_close (w);
    if (NULL != tt) free(tt);
}

void predict_step(bool adj     /* adjoint flag */,
		  bool forw    /* forward or backward */, 
		  float* trace /* trace */, 
		  float* pp    /* slope */)
/*< prediction step >*/
{
    int i1;

    for (i1=0; i1 < n1; i1++) {
	diag[i1] = 6.*eps;
	offd[0][i1] = -4.*eps;
	offd[1][i1] = eps;
    }
    diag[0] = diag[n1-1] = 1.+eps;
    diag[1] = diag[n1-2] = 1.+5.*eps;
    offd[0][0] = offd[0][n1-2] = -2.*eps;

    pwd_define (forw, w, pp, diag, offd);
    sf_banded_define (slv, diag, offd);

    if (adj) {
	sf_banded_solve (slv, trace);
	pwd_set (true, w, trace, trace, diag);
    } else {
	pwd_set (false, w, trace, trace, diag);
	sf_banded_solve (slv, trace);
    }
}

void predict_set(float **dip1 /* dip field [n2][n1] */)
/*< set the local slopes for applying the linear operator >*/
{
    dip=dip1;
    if (NULL == tt) tt = sf_floatalloc(n1);
}

static void stepper(bool adj /* adjoint flag */,
		    int i2   /* trace number */)
{
    if (i2 < k2) {
	predict_step(adj,false,tt,dip[k2-1-i2]);
    } else if (i2 < n2+k2-1) {
	predict_step(adj,true,tt,dip[i2-k2]);
    } else {
	predict_step(adj,false,tt,dip[2*n2+k2-3-i2]);
    }
}

void predict_lop(bool adj, bool add, int nx, int ny, float *xx, float *yy)
/*< linear operator >*/
{
    int i1, i2;

    if (nx != ny || nx != n1*n2) sf_error("%s: Wrong dimensions",__FILE__);

    sf_adjnull(adj,add,nx,ny,xx,yy);

    for (i1=0; i1 < n1; i1++) {
	tt[i1] = 0.;
    }

    if (adj) {
	for (i2=n2-1; i2 >= 0; i2--) {
	    predict_step(true,true,tt,dip[i2]);
	    for (i1=0; i1 < n1; i1++) {
		tt[i1] += yy[i1+i2*n1];
	    }
	    for (i1=0; i1 < n1; i1++) {
		xx[i1+i2*n1] += tt[i1];
	    }
	}
    } else {
	for (i2=0; i2 < n2; i2++) {
	    for (i1=0; i1 < n1; i1++) {
		tt[i1] += xx[i1+i2*n1];
	    }
	    for (i1=0; i1 < n1; i1++) {
		yy[i1+i2*n1] += tt[i1];
	    }
	    predict_step(false,true,tt,dip[i2]);
	}
    }
}

void predicter_lop(bool adj, bool add, int nx, int ny, float *xx, float *yy)
/*< linear operator >*/
{
    int i1, i2;

    if (nx != ny || nx != n1*(n2+2*k2)) sf_error("%s: Wrong dimensions",__FILE__);

    sf_adjnull(adj,add,nx,ny,xx,yy);

    for (i1=0; i1 < n1; i1++) {
	tt[i1] = 0.;
    }

    if (adj) {
	for (i2=n2+2*k2-1; i2 >= 0; i2--) {
	    stepper(true,i2);
	    for (i1=0; i1 < n1; i1++) {
		tt[i1] += yy[i1+i2*n1];
	    }
	    for (i1=0; i1 < n1; i1++) {
		xx[i1+i2*n1] += tt[i1];
	    }
	}
    } else {
	for (i2=0; i2 < n2+2*k2; i2++) {
	    for (i1=0; i1 < n1; i1++) {
		tt[i1] += xx[i1+i2*n1];
	    }
	    for (i1=0; i1 < n1; i1++) {
		yy[i1+i2*n1] += tt[i1];
	    }
	    stepper(false,i2);
	}
    }
}

void subtracter_lop(bool adj, bool add, int nx, int ny, float *xx, float *yy)
/*< linear operator >*/
{
    int i1, i2, j2, m2;

    if (nx != ny || nx != n1*(n2+2*k2)) sf_error("%s: Wrong dimensions",__FILE__);

    sf_adjnull(adj,add,nx,ny,xx,yy);

    if (adj) {
	for (j2=0; j2 < n2+2*k2; j2++) {
	    i2=j2+k2;
	    if (i2 < n2+2*k2) {
		for (i1=0; i1 < n1; i1++) {
		    tt[i1] = yy[i1+i2*n1];
		}
		for (m2=i2-1; m2 >= j2; m2--) {
		    stepper(true,m2);
		}
		for (i1=0; i1 < n1; i1++) {
		    xx[i1+j2*n1] += yy[i1+j2*n1]-tt[i1];
		}
	    } else {
		for (i1=0; i1 < n1; i1++) {
		    xx[i1+j2*n1] += yy[i1+j2*n1];
		}
	    }
	}
    } else {
	for (i2=0; i2 < n2+2*k2; i2++) { 
	    j2=i2-k2;
	    if (j2 >=0) {
		for (i1=0; i1 < n1; i1++) {
		    tt[i1] = xx[i1+j2*n1];
		}
		for (m2=j2; m2 < i2; m2++) {
		    stepper(false,m2);
		}
		for (i1=0; i1 < n1; i1++) {
		    yy[i1+i2*n1] += xx[i1+i2*n1]-tt[i1];
		}
	    } else {
		for (i1=0; i1 < n1; i1++) {
		    yy[i1+i2*n1] += xx[i1+i2*n1];
		}
	    }
	}
    }
}

void subtract_lop(bool adj, bool add, int nx, int ny, float *xx, float *yy)
/*< linear operator >*/
{
    int i1, i2, j2, m2;

    if (nx != ny || nx != n1*n2) sf_error("%s: Wrong dimensions",__FILE__);

    sf_adjnull(adj,add,nx,ny,xx,yy);

    if (adj) {
	for (j2=0; j2 < n2; j2++) {
	    i2=j2+k2;
	    if (i2 < n2) {
		for (i1=0; i1 < n1; i1++) {
		    tt[i1] = yy[i1+i2*n1];
		}
		for (m2=i2-1; m2 >= j2; m2--) {
		    predict_step(true,true,tt,dip[m2]);
		}
		for (i1=0; i1 < n1; i1++) {
		    xx[i1+j2*n1] += yy[i1+j2*n1]-tt[i1];
		}
	    } else {
		for (i1=0; i1 < n1; i1++) {
		    xx[i1+j2*n1] += yy[i1+j2*n1];
		}
	    }
	}
    } else {
	for (i2=0; i2 < n2; i2++) { 
	    j2=i2-k2;
	    if (j2 >=0) {
		for (i1=0; i1 < n1; i1++) {
		    tt[i1] = xx[i1+j2*n1];
		}
		for (m2=j2; m2 < i2; m2++) {
		    predict_step(false,true,tt,dip[m2]);
		}
		for (i1=0; i1 < n1; i1++) {
		    yy[i1+i2*n1] += xx[i1+i2*n1]-tt[i1];
		}
	    } else {
		for (i1=0; i1 < n1; i1++) {
		    yy[i1+i2*n1] += xx[i1+i2*n1];
		}
	    }
	}
    }
}

void predict_flat(int i0     /* reference trace number */, 
		  float** d  /* input */, 
		  float** mm /* output */, 
		  float** pp /* slope */)
/*< predictive flattening >*/
{
    int i1, i2, m2;
    float *trace;

    /* prediction from the left */
    for (i2=0; i2 <= i0; i2++) {
        for (i1=0; i1 < n1; i1++) {
            mm[i2][i1] = d[i2][i1];
        }

        if (i2 == i0) break;

	for (i1=0; i1 < n1; i1++) {
	    diag[i1] = 6.*eps;
	    offd[0][i1] = -4.*eps;
	    offd[1][i1] = eps;
	}
	diag[0] = diag[n1-1] = 1.+eps;
	diag[1] = diag[n1-2] = 1.+5.*eps;
	offd[0][0] = offd[0][n1-2] = -2.*eps;

        pwd_define (true, w, pp[i2], diag, offd);
        sf_banded_define (slv, diag, offd);

        for (m2=0; m2 <= i2; m2++) {
            trace = mm[m2];

            pwd_set (false, w, trace, trace, diag);
            sf_banded_solve (slv, trace);
        }
    }
    
    /* prediction from the right */
    for (i2=n2-1; i2 > i0; i2--) {
        for (i1=0; i1 < n1; i1++) {
            mm[i2][i1] = d[i2][i1];
        }

	for (i1=0; i1 < n1; i1++) {
	    diag[i1] = 6.*eps;
	    offd[0][i1] = -4.*eps;
	    offd[1][i1] = eps;
	}
	diag[0] = diag[n1-1] = 1.+eps;
	diag[1] = diag[n1-2] = 1.+5.*eps;
	offd[0][0] = offd[0][n1-2] = -2.*eps;

        pwd_define (false, w, pp[i2-1], diag, offd);
        sf_banded_define (slv, diag, offd);

        for (m2=n2-1; m2 >= i2; m2--) {
            trace = mm[m2];

            pwd_set (false, w, trace, trace, diag);
            sf_banded_solve (slv, trace);
        }
    }
}
