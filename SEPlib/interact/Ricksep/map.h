#ifndef RICK_map
#define RICK_map 3
#include "axis.h"
/*
map axis

Remembers image size, windowing, frame selection and orientation
Uses axis definition
*/

/* constants */
/* best fit tic spacing to pixels */
#define	MAP_INTERP	4096
#define	MAP_TIC	100

/* mapping index of data to and from image */
typedef unsigned int* Vec;

/* Map object */
typedef struct {
	Axis	axis;	/* current data axis */
	string	name;	/* name of map axis */
	int	size;	/* pixels long */
	int	first;	/* minimum data axis sample */
	int	last;	/* maximum data axis sample; can be less than first */
	float	dtic;	/* tic spacing in axis value */
	float	tic0;	/* mimimum tic value */
	float	tic2;	/* maximum tic value */
	int	ntic;	/* number of tics */
	string	format;	/* printf format of tic label */
	int	frame1;	/* animation start */
	int	frame2;	/* animation end */
	int	dframe;	/* animation increment */
	int	frame;	/* current animation frame */
	int	imap;	/* current map position */
	int	prev;	/* previous animation frame */
	int	save;	/* save frame */
	Vec	map;	/* data sample for each display sample */
	Vec	inv;	/* display sample for data sample */
	Vec	interp;	/* 16 bit interpolation for smooth mapping */
	} *Map, Map_;




#endif
