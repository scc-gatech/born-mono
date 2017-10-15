#include <sepConfig.h>
#include "../include/vplot.h"
/*
 * vp_rascoltab
 * This routine sets up a "raster color table", to be used with vp_raster
 *
 * keywords: raster color-table vplot utility
 */

#define A1(VP_RASCOLTAB,SUF) \
int VP_RASCOLTAB (int nreserve,char *colname) \
{ \
/* \
 * Junk temporary variables \
 */ \
int             ii, jj, kk, incr; \
int             smap[65536]; \
/* \
 * red[65536], green[65536], blue[65536] \
 * are float arrays with numbers in the range 0. through 1., \
 * that define our "raster" color table. \
 */ \
float           red[65536], green[65536], blue[65536]; \
 \
/* \
 * This call makes a color table given its name. We ask for \
 * a color table 256 long. \
 */ \
    name_to_coltab (colname, 256, red, green, blue); \
 \
    /* \
     * In colors 256 through 512, we define our "raster" color table, \
     * unscrambled and with a full range of values. When we call "vp_raster", \
     * we'll give an offset of 256 to shift 0-255 into this range.  \
     */ \
    for (ii = 256; ii < 512; ii++) \
    { \
	jj = ii - 256; \
 \
	vp_coltab ## SUF (ii, red[jj], green[jj], blue[jj]); \
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
    smap[ii] = 255; \
    ii++; \
    /* \
     * This obscure code does the scrambling. A uniform grey scale, for \
     * example, that goes "0, 1, 2, 3, ..., 255" gets reordered to "0, 255, \
     * 128, 64, 196, 32, 96, 160, ... 1, 3, ... 251, 253". This way the \
     * "most important" colors are closer to the front. This way the first \
     * colors lost due to a less-than-256-color device or less than 256 \
     * colors set aside for raster are the least important ones.  \
     */ \
    for (incr = 2; incr < 512; incr *= 2) \
    { \
	for (kk = 1; kk < incr; kk += 2) \
	{ \
	    jj = kk * 256 / incr; \
	    smap[ii] = jj; \
	    ii++; \
	    if (ii == 256) \
		break; \
	} \
    } \
 \
    for (ii = nreserve; ii < 256; ii++) \
    { \
	jj = ii - nreserve; \
 \
	vp_coltab ## SUF (ii, red[smap[jj]], \
		   green[smap[jj]], \
		   blue[smap[jj]]); \
    } \
    return (vp_fixpc ## SUF()); \
 \
}
#ifdef FORTRAN
A1(vprascoltab_, )
A1(vprascoltabg_,_g)
#else
A1(vp_rascoltab, )
A1(vp_rascoltab_g,_g)
#endif
