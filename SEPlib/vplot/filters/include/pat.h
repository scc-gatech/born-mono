#ifndef VP_PAT_H
#define VP_PAT_H
/*
 * area fill pattern stuff
 */

struct pat
{
	int    ydim;
	int    xdim;
	int    ydim_orig;
	int    xdim_orig;
	int   *patbits;
};

extern struct pat pat[];
#endif
