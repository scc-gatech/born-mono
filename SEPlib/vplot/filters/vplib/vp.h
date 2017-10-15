#include <sepConfig.h>
#include <stdio.h>
#define F_COL		(0001)
#define F_FAT		(0002)
#define F_CLIP		(0004)
#define F_FONT		(0010)
#define F_JUST		(0020)
#define F_DASH		(0040)
#define F_COLT		(0100)
#define F_OVLY		(0200)

#if 0
#define VPPEN_NUM_COL	(16384)
#endif
#define VPPEN_NUM_COL	(256)
extern int vpscoltabinfo[VPPEN_NUM_COL][4];
#define ISITSET		0

extern int vpdumb;
extern int vpstat;
extern int vpalign;
extern int vpfit;
extern float xsize, ysize;
extern int vpbig;
extern int vpstyle;
extern char vpaligns[];
extern int lost;
extern int vpcolor, vpfat;
extern int vpsetflag;
extern int vpblast;
extern int vpbit;
extern int vparray[];
extern int vpasize[];
extern int vpframe, vpframecount;
extern int vpsetcoltabanyway;
extern int vpxmax, vpxmin, vpymax, vpymin;

extern int default_hshift, default_vshift;
extern float default_scale, default_xscale, default_yscale, fatmult_orig;

#if defined (HAVE_STDLIB_H)
#include <stdlib.h>
#include <string.h>
#else
extern char * malloc();
extern char * strcat();
#endif

extern int vpopen_name(int);
extern void vpderase(int);
void vp_check_filep (FILE *);
