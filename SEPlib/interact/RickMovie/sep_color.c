#include <sepConfig.h>
#if defined (HAVE_MOTIF) || defined(HAVE_ATHENA)
/*
color table object
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "main.h" 
#include "axis.h"
#include "data.h"
#include "map.h"
#include "render.h"
#include "draw.h"
#include "color.h"
#include "ui.h"

ColorTable ctable = 0;

/* some color tables in terms of pivot values */
string ct_names[] = {
"gray",
"straw",
"flag",
"tiger",
"blue",
"rainbow",
"avo",
"veldata",
};

Color_ gray[] = {
0.,	0.,	0.,	0.,
.5,	.5,	.5,	.5,
1.,	1.,	1.,	1.,
1.,	1.,	0.,	0.,
-1.
};

Color_ flag[] = {
0.,	0.,	0.,	1.,
.5,	1.,	1.,	1.,
1.,	1.,	0.,	0.,
1.,	0.,	1.,	0.,
-1.,
};

Color_ tiger[] = {
0.,	0.,	0.,	0.,
.5,	1.,	1.,	1.,
1.,	1.,	0.,	0.,
1.,	0.,	1.,	0.,
-1.,
};

Color_ rainbow[] = {
0.,	0.,	0.,	0.,
.1,	0.,	.3,	1.,
.3,	0.,	1.,	.3,
.5,	1.,	1.,	0.,
.7,	1.,	.5,	0.,
.9,	1.,	0.,	0.,
1.,	1.,	1.,	1.,
1.,	0.,	1.,	1.,
-1.,
};

Color_ veldata[] = {
0.,	1.,	0.,	1.,
0.,	0.,	0.,	1.,
0.,	0.,	1.,	1.,
0.,	0.,	1.,	0.,
0.,	1.,	1.,	0.,
0.,	1.,	0.,	0.,
0.,	1.,	1.,	1.,
0.,	0.,	0.,	0.,
-1.,
};
Color_ blue[] = {
0.,	0.,	0.,	0.,
.5,	0.,	0.,	1.,
1.,	1.,	1.,	1.,
1.,	1.,	0.,	0.,
-1.,
};


Color_ straw[] = {
0.,	0.,	.3,	1.,
.5,	.7,	.7,	0.,
1.,	1.,	1.,	0.,
1.,	1.,	0.,	0.,
-1.,
};

Color_ avo[] = {
0.,	0.,	0.,	1.,
.399,	0.,	0.,	1.,
.4,	.5,	.5,	1.,
.599,	.5,	.5,	1.,
.6,	1.,	.7,	0.,
.749,	1.,	.7,	0.,
.75,	1.,	1.,	0.,
.89,	1.,	1.,	0.,
.9,	1.,	0.,	0.,
1.,	1.,	0.,	0.,
-1.,
};

/* overlay colors */
string over_names[] = {
"red",
"green",
"white",
"black",
};

Color_ overlay[] = {
0.,	1.,	0.,	0.,	/* 1 red */
0.,	0.,	1.,	0.,	/* 0 green */
0.,	1.,	1.,	1.,	/* 3 white */
0.,	0.,	0.,	0.,	/* 2 black */
};

string mark_names[] = {
"red",
"green",
"white",
"black"};

string background_names[] = {
"red",
"green",
"white",
"black"};

/* initialize color */
void ColorInit (void)
	{
	int i, veldata=0;
	string name;
	extern ColorTable ctable;

	NEW (ColorTable,ctable,1);
	ctable->base = DrawColorBase();
	ctable->size = DrawColorSize() - 4;
	ctable->contrast = CONTRAST;
	ctable->contrast0 = CONTRAST0;
	GETPARINT ("contrast","d",&ctable->contrast);
	GETPARINT ("contrast0","d",&ctable->contrast0);
	/* load default color table */
	GETPARINT ("veldata","d",&veldata);
	if (veldata) ctable->color = COLOR_VELDATA;
	else ctable->color = COLOR_GRAY;
	if (GETPARSTRING ("color","s",name)) {
		if (!strcmp (name,"gray")) ctable->color = COLOR_GRAY;
		if (!strcmp (name,"straw")) ctable->color = COLOR_STRAW;
		if (!strcmp (name,"flag")) ctable->color = COLOR_FLAG;
		if (!strcmp (name,"tiger")) ctable->color = COLOR_TIGER;
		if (!strcmp (name,"rainbow")) ctable->color = COLOR_RAINBOW;
		if (!strcmp (name,"blue")) ctable->color = COLOR_BLUE;
		if (!strcmp (name,"avo")) ctable->color = COLOR_AVO;
		if (!strcmp (name,"veldata")) ctable->color = COLOR_VELDATA;
		}
	ctable->overlay = COLOR_OVERLAY;
	if (GETPARSTRING ("overlay","s",name)) {
		for (i=0; i<sizeof(overlay)/sizeof(overlay[0])-1; i++) {
			if (!strcmp (over_names[i],name)) {
				ctable->overlay = i;
				break;
				}
			}
		}
	for (i=0; i<NXCOLOR; i++) {
		ctable->red0[i] = 0;
		ctable->green0[i] = 0;
		ctable->blue0[i] = 0;
		ctable->red[i] = 0;
		ctable->green[i] = 0;
		ctable->blue[i] = 0;
		}
	DrawColor1 (ctable->base,BACKGROUND_RED);
	ColorSwitch ();
	}


/* interpret color menu positions */
void ColorSwitch (void)
	{
	extern ColorTable ctable;

	if (!ctable) return;
	switch (ctable->color) {
	case COLOR_GRAY: ColorLoad (gray); break;
	case COLOR_STRAW: ColorLoad (straw); break;
	case COLOR_FLAG: ColorLoad (flag); break;
	case COLOR_TIGER: ColorLoad (tiger); break;
	case COLOR_RAINBOW: ColorLoad (rainbow); break;
	case COLOR_BLUE: ColorLoad (blue); break;
	case COLOR_AVO: ColorLoad (avo); break;
	case COLOR_VELDATA: ColorLoad (veldata); break;
		}
	}

/* set contrast-gpow value 0 - 100 */
void ColorSetContrast (int contrast)
	{
	extern ColorTable ctable;

	if (!ctable) return;
	ctable->contrast = contrast;
	}

/* set contrast-middle value 0-100 */
void ColorSetContrast0 (int contrast0)
	{
	extern ColorTable ctable;

	if (!ctable) return;
	ctable->contrast0 = contrast0;
	}

/* set color option */
void ColorSetChoice (int color)
	{
	extern ColorTable ctable;

	if (!ctable) return;
	ctable->color = color;
	}

/* set overlay color (see above constants) */
void ColorSetOverlay (int index)
	{
	extern ColorTable ctable;

	if (!ctable) return;
	ctable->overlay = index;
	}

/* set color background */
void ColorSetBackground (int index)
	{
	extern ColorTable ctable;

	if (!ctable) return;
	ctable->background = index;
	switch (index) {
	case COLOR_RED: DrawColor1 (ctable->base,BACKGROUND_RED); break;
	case COLOR_GREEN: DrawColor1 (ctable->base,BACKGROUND_GREEN); break;
	case COLOR_WHITE: DrawColor1 (ctable->base,BACKGROUND_WHITE); break;
	case COLOR_BLACK: DrawColor1 (ctable->base,BACKGROUND_BLACK); break;
		}
	}

/* set mark color */
void ColorSetMark (int index)
	{
	extern ColorTable ctable;

	if (!ctable) return;
	ctable->mark = index;
	switch (index) {
	case COLOR_RED: DrawColor1 (ctable->base+ctable->size+2,MARK_RED); break;
	case COLOR_GREEN: DrawColor1 (ctable->base+ctable->size+2,MARK_GREEN); break;
	case COLOR_WHITE: DrawColor1 (ctable->base+ctable->size+2,MARK_WHITE); break;
	case COLOR_BLACK: DrawColor1 (ctable->base+ctable->size+2,MARK_BLACK); break;
		}
	}

/* return color table size */
int ColorSize (void)
	{
	int ncolor;

	extern ColorTable ctable;

	if (!ctable) {
		ncolor = NCOLOR;
		GETPARINT ("ncolor","d",&ncolor);
		ncolor -= 4;
		return (ncolor);
		}
	return (ctable->size);
	}

/* print color table information */
void ColorInfo (void)
	{
	Message message;
	extern ColorTable ctable;

	if (!ctable) return;
	sprintf (message, "Color: base=%d size=%d color=%s overlay=%s mark=%s back=%s contrast=%d contrast0=%d gpow=%g",
		ctable->base,
		ctable->size,
		ct_names[ctable->color],
		over_names[ctable->overlay],
		mark_names[ctable->mark],
		background_names[ctable->background],
		ctable->contrast,
		ctable->contrast0,
		ctable->gpow);
	UIMessage (message);
	}

/* return background color */
int ColorBackground (void)
	{
	extern ColorTable ctable;

	if (!ctable) return (NO_INDEX);
	return (ctable->base);
	}

/* return base color */
int ColorBase (void)
	{
	extern ColorTable ctable;

	if (!ctable) return (NO_INDEX);
	return (ctable->base+2);
	}

/* return mark color */
int ColorMark (void)
	{
	extern ColorTable ctable;

	if (!ctable) return (NO_INDEX);
	return (ctable->base+ctable->size+2);
	}

/* create color table given pivot colors; apply contrast */
void ColorLoad (Color cdata)
	{
	float frac, stretch[257], value;
	int i, icolor, midpoint;
	extern ColorTable ctable;

	if (!ctable) return;
	if (ctable->color == COLOR_VELDATA) {
		ColorLoadVeldata ();
		return;
		}
	/* convert contrast (0-100) into gpow (.2-5.) */
	ctable->gpow = ctable->contrast > 50 ?
		(ctable->contrast - 40.) / 10. :
		10. / (60. - ctable->contrast);
	/* computer index stretching into color pallete */
	for (i=0; i<ctable->size/2; i++) {
		frac = (float)(i - 1) / (float)(ctable->size/2 - 2);
		frac = frac > 0. ? frac : 0.;
		frac = frac < 1. ? frac : 1.;
		frac = pow (frac,ctable->gpow);
		stretch[ctable->size/2-1+i] = .5 * (1. + frac);
		stretch[ctable->size/2-1-i] = .5 * (1. - frac);
		}
	stretch[ctable->size-1] = .5 * (1. + frac);
	/* compute color table entries */
	for (icolor=1, i=0; cdata[icolor].index >= 0.; icolor++) {
		/* cdata must be monotonically increasing */
		if (cdata[icolor].index > cdata[icolor-1].index) {
			/* compute all stretch's between color range */
			while (i < ctable->size &&
				stretch[i] <= cdata[icolor].index) {
				/* spectral colors */
				if ((i%2)==0) {
			frac = (stretch[i] - cdata[icolor-1].index) /
				(cdata[icolor].index - cdata[icolor-1].index);
			value = ((1. - frac) * cdata[icolor-1].red +
				frac * cdata[icolor].red) * COLOR_SCALE;
			ctable->red0[i+2] = value < COLOR_SCALE ?
				value : COLOR_SCALE;
			value = ((1. - frac) * cdata[icolor-1].green +
				frac * cdata[icolor].green) * COLOR_SCALE;
			ctable->green0[i+2] = value < COLOR_SCALE ?
				value : COLOR_SCALE;
			value = ((1. - frac) * cdata[icolor-1].blue +
				frac * cdata[icolor].blue) * COLOR_SCALE;
			ctable->blue0[i+2] = value < COLOR_SCALE ?
				value : COLOR_SCALE;
					}
				i++;
				}
			}
		}
	/* overlay */
	for (i=1; i<ctable->size+4; i+=2) {
		ctable->red[i] = overlay[ctable->overlay].red * COLOR_SCALE;
		ctable->green[i] = overlay[ctable->overlay].green * COLOR_SCALE;
		ctable->blue[i] = overlay[ctable->overlay].blue * COLOR_SCALE;
		}
	/* zero point contrast */
	midpoint = (ctable->contrast0 * ctable->size) / 100;
	for (i=0; i<ctable->size; i+=2) {
		if (i < midpoint) {
			icolor = (i * 50) / ctable->contrast0;
		} else {
			icolor = ctable->size/2 + ((i - midpoint) * ctable->contrast0) / 50;
			}
		icolor = icolor > 0 ? icolor : 0;
		icolor = icolor < ctable->size-1 ? icolor : ctable->size-1;
		icolor = (icolor / 2) * 2;
		ctable->red[i+2] = ctable->red0[icolor+2];
		ctable->green[i+2] = ctable->green0[icolor+2];
		ctable->blue[i+2] = ctable->blue0[icolor+2];
		}
	/* mark color */
	for (icolor=0; cdata[icolor].index >= 0.;) icolor++;
	icolor--;
	ctable->red[ctable->size+2] = cdata[icolor].red * COLOR_SCALE;
	ctable->green[ctable->size+2] = cdata[icolor].green * COLOR_SCALE;
	ctable->blue[ctable->size+2] = cdata[icolor].blue * COLOR_SCALE;
	DrawColors (ctable->red,ctable->green,ctable->blue,ctable->size+4);
	}

void ColorLoadVeldata (void) {
	int i, j, k;
	float frac, scale;
	ctable->gpow = ctable->contrast > 50 ?
		(ctable->contrast - 40.) / 10. :
		10. / (60. - ctable->contrast);
	for (j=0; j<9; j++) {
		frac = (j+1) / 9.;
		scale = pow (frac,ctable->gpow) * COLOR_SCALE;
		for (i=0; i<7; i++) {
			k = i*18+j*2;
			ctable->red[k] = veldata[i].red * scale;
			ctable->green[k] = veldata[i].green * scale;
			ctable->blue[k] = veldata[i].blue * scale;
			}
		}
        for (i=1; i<ctable->size+4; i+=2) {
                ctable->red[i] = overlay[ctable->overlay].red * COLOR_SCALE;
                ctable->green[i] = overlay[ctable->overlay].green * COLOR_SCALE;
                ctable->blue[i] = overlay[ctable->overlay].blue * COLOR_SCALE;
		}
        DrawColors (ctable->red,ctable->green,ctable->blue,ctable->size+4);
	}

/* save color parameters */
void ColorSavePar (void)
	{
	Message message;
	extern ColorTable ctable;

	if (!ctable) return;
	sprintf (message, "Color: color=%s overlay=%s contrast=%d contrast0=%d",
		ct_names[ctable->color],
		over_names[ctable->overlay],
		ctable->contrast,
		ctable->contrast0);
	UISaveMessage (message);
	}

/* write color table contents to a file */
void ColorTableDump (void)
	{
	extern ColorTable ctable;
	string filename;
	FILE *fd;
	int i;

	sprintf (filename,"color.table.%d.txt",ctable->size);
	fd = fopen (filename,"w");
	for (i=0; i<ctable->size; i++) {
		fprintf (fd,"%3d : %5d %5d %5d : %5d %5d %5d\n",i,
			ctable->red0[i],ctable->green0[i],ctable->blue0[i],
			ctable->red[i],ctable->green[i],ctable->blue[i]);
		}
	fclose (fd);
	UIMessage ("color table dumped to file");
	}
#endif
