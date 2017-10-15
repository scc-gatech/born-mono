/* Simple weight operator */
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

/*^*/

#include "sergey_rsf.h"

static float* w;

void weight_init(float *w1)
/*< initialize >*/
{
    w = w1;
}

void weight_lop (bool adj, bool add, int nx, int ny, float* xx, float* yy)
/*< linear operator >*/
{
    int i;

    if (ny!=nx) sf_error("%s: size mismatch: %d != %d",__FILE__,ny,nx);

    sf_adjnull (adj, add, nx, ny, xx, yy);
  
    for (i=0; i < nx; i++) {
	if (adj) {
	    xx[i] += yy[i] * w[i];
	} else {
	    yy[i] += xx[i] * w[i];
	}
    }
}

/* 	$Id: weight.c 1131 2005-04-20 18:19:10Z fomels $	 */
