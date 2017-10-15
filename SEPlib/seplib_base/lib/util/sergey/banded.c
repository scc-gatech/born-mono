/* Banded matrix solver. */
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

#include "sergey_rsf.h"
#ifndef _sf_banded_h

typedef struct sf_Bands *sf_bands;
/* abstract data type */
/*^*/

#endif

struct sf_Bands {
    int n, band;
    float *d, **o;
};

sf_bands sf_banded_init (int n    /* matrix size */, 
		   int band /* band size */)
/*< initialize >*/
{
    sf_bands slv;
    int i;
    
    slv = (sf_bands) sf_alloc (1,sizeof(*slv));
    slv->o = (float**) sf_alloc (band,sizeof(float*));
    for (i = 0; i < band; i++) {
	slv->o[i] = sf_floatalloc (n-1-i);
    }
    slv->d = sf_floatalloc (n);
    slv->n = n;
    slv->band = band;

    return slv;
}

void sf_banded_define (sf_bands slv, 
		    float* diag  /* diagonal [n] */, 
		    float** offd /* off-diagonal [band][n] */)
/*< define the matrix >*/
{
    int k, m, n;
    float t;
    
    slv->d[0] = diag[0];
    for (k = 0; k < slv->band-1; k++) {
	for (m = k; m >= 0; m--) {
	    t = offd[m][k-m];
	    for (n = m+1; n < k-1; n++) 
		t -= (slv->o[n][k-n])*(slv->o[n-m-1][k-n])*(slv->d[k-n]);
	    slv->o[m][k-m] = t/slv->d[k-m];
	}
	t = diag[k+1];
	for (m = 0; m <= k; m++)
	    t -= (slv->o[m][k-m])*(slv->o[m][k-m])*(slv->d[k-m]);
	slv->d[k+1] = t;
    }
    for (k = slv->band-1; k < slv->n-1; k++) {
	for (m = slv->band-1; m >= 0; m--) {
	    t = offd[m][k-m];
	    for (n = m+1; n < slv->band; n++) 
		t -= (slv->o[n][k-n])*(slv->o[n-m-1][k-n])*(slv->d[k-n]);
	    slv->o[m][k-m] = t/(slv->d[k-m]);
	}
	t = diag[k+1];
	for (m = 0; m < slv->band; m++) 
	    t -= (slv->o[m][k-m])*(slv->o[m][k-m])*slv->d[k-m];
	slv->d[k+1] = t;
    }
}

void sf_banded_const_define (sf_bands slv, 
			  float diag        /* diagonal */, 
			  const float* offd /* off-diagonal [band] */)
/*< define matrix with constant diagonal coefficients >*/
{
    int k, m, n;
    float t;
    
    slv->d[0] = diag;
    for (k = 0; k < slv->band-1; k++) {
	for (m = k; m >= 0; m--) {
	    t = offd[m];
	    for (n = m+1; n < k-1; n++) 
		t -= (slv->o[n][k-n])*(slv->o[n-m-1][k-n])*(slv->d[k-n]);
	    slv->o[m][k-m] = t/slv->d[k-m];
	}
	t = diag;
	for (m = 0; m <= k; m++)
	    t -= (slv->o[m][k-m])*(slv->o[m][k-m])*(slv->d[k-m]);
	slv->d[k+1] = t;
    }
    for (k = slv->band-1; k < slv->n-1; k++) {
	for (m = slv->band-1; m >= 0; m--) {
	    t = offd[m];
	    for (n = m+1; n < slv->band; n++) 
		t -= (slv->o[n][k-n])*(slv->o[n-m-1][k-n])*(slv->d[k-n]);
	    slv->o[m][k-m] = t/(slv->d[k-m]);
	}
	t = diag;
	for (m = 0; m < slv->band; m++) 
	    t -= (slv->o[m][k-m])*(slv->o[m][k-m])*slv->d[k-m];
	slv->d[k+1] = t;
    }
}

void sf_banded_solve (sf_bands slv, float* b)
/*< invert (in place) >*/
{
    int k, m;
    float t;
    
    for (k = 1; k < slv->band; k++) {
	t = b[k];
	for (m = 1; m <= k; m++)
	    t -= (slv->o[m-1][k-m]) * b[k-m];
	b[k] = t;
    }
    for (k = slv->band; k < slv->n; k++) { 
	t = b[k];
	for (m = 1; m <= slv->band; m++) 
	    t -= (slv->o[m-1][k-m]) * b[k-m];
	b[k] = t;
    }
    for (k = slv->n-1; k >= slv->n - slv->band; k--) {
	t = b[k]/slv->d[k];
	for (m = 0; m < slv->n - k - 1; m++)
	    t -= slv->o[m][k] * b[k+m+1];
	b[k] = t;
    }
    for (k = slv->n - slv->band -1; k >= 0; k--) {
	t = b[k]/slv->d[k];
	for (m = 0; m < slv->band; m++) 
	    t -= (slv->o[m][k]) * b[k+m+1];
	b[k] = t;
    }
}

void sf_banded_close (sf_bands slv)
/*< free allocated storage >*/
{
    int i;

    for (i = 0; i < slv->band; i++) {
	free(slv->o[i]);
    }
    free (slv->o);
    free (slv->d);
    free (slv);
}

/* 	$Id: banded.c 1134 2005-04-20 18:53:07Z fomels $	 */
