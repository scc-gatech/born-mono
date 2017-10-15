#include <sepConfig.h>
#include "../include/vplot.h"
/*
 * vp_rascoltab
 * This routine sets up a "raster color table", to be used with vp_raster
 * with an offset of 256.
 * It sets this color table up in colors 256 through 511.
 * Colors 0 through nreserve-1 are left untouched; you can either let
 * these default or set them yourself.
 * "colname" is a string which defines what raster color table you
 * want to use. For now, it is the same as the "color" option of "Movie",
 * with the extensions that appending a "C" means to flag clipped colors
 * in red, and a string longer than 2 characters is interpreted as a
 * "colfile" name.
 *
 * keywords: raster color-table vplot utility
 */

#define A1(RASCOLTAB,SUF) \
int RASCOLTAB (int nreserve,char *colname) \
{ \
/* \
 * Junk temporary variables \
 */ \
int             ii, jj, kk, incr,ncol; \
int             smap[32768]; \
int             ncol2;  \
/* \
 * red[32768], green[32768], blue[32768] \
 * are float arrays with numbers in the range 0. through 1., \
 * that define our "raster" color table. \
 */ \
float           red[32768], green[32768], blue[32768]; \
 \
/* \
 * This call makes a color table given its name. We ask for \
 * a color table 32768 long. \
 */ \
    ncol=name_to_coltab (colname, 16384, red, green, blue); \
    ncol2=1; \
    while(ncol2 < ncol) ncol2=ncol2*2; \
    for(ii=ncol+1; ii < ncol2; ii++) { \
        red[ii]=green[ii]=blue[ii]=0.; \
    } \
   if(ncol2!=ncol) \
     fprintf(stderr, "best results when color table is a power of 2 \n"); \
 \
    /* \
     * In colors ncol2 through 2*ncol2, we define our "raster" color table, \
     * unscrambled and with a full range of values. When we call "vp_raster", \
     * we'll give an offset of ncol2 to shift 0-ncol2 into this range.  \
     */ \
    for (ii = ncol2; ii < 2*ncol2; ii++) \
    { \
	jj = ii - ncol2; \
	vp_coltab ## SUF (ii, red[jj], green[jj], blue[jj]); \
/*   fprintf(stderr,"putting color %d-%d %d %d %d \n",ii,jj,red[jj],green[jj],blue[jj]);*/ \
    } \
 \
 \
    /* \
     * set the lower "scrambled" scale, which will give us the optimal raster \
     * color scale possible for any device, no matter how many color table \
     * entries it has.  \
     */ \
 \
    ii = 0; \
    smap[ii] = 0; \
    ii++; \
    smap[ii] = ncol2-1; \
    ii++; \
    /* \
     * This obscure code does the scrambling. A uniform grey scale, for \
     * example, that goes "0, 1, 2, 3, ..., 255" gets reordered to "0, 255, \
     * 128, 64, 196, 32, 96, 160, ... 1, 3, ... 251, 253". This way the \
     * "most important" colors are closer to the front. This way the first \
     * colors lost due to a less-than-256-color device or less than 256 \
     * colors set aside for raster are the least important ones.  \
     */ \
    for (incr = 2; incr < ncol2*2; incr *= 2) \
    { \
	for (kk = 1; kk < incr; kk += 2) \
	{ \
	    jj = kk * ncol2 / incr; \
	    smap[ii] = jj; \
	    ii++; \
	    if (ii == ncol2) \
		break; \
	} \
    } \
 \
    for (ii = nreserve; ii < ncol2; ii++) \
    { \
	jj = ii - nreserve; \
 \
	vp_coltab ## SUF (ii, red[smap[jj]], \
		   green[smap[jj]], \
		   blue[smap[jj]]); \
    } \
  vp_fixpc ## SUF(); \
  return(ncol); \
 \
}

#ifdef FORTRAN
A1(vprascol16tab_, )
A1(vprascol16tabg_,_g)
#else
A1(vp_rascol16tab, )
A1(vp_rascol16tab_g,_g)
#endif
