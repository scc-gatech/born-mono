#ifndef RICKMOVIE_MAP_H
#define RICKMOVIE_MAP_H
/*
map axis

Remembers image size, windowing, frame selection and orientation
Uses axis definition
*/

#include "axis.h"

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
	string	format;	/*printf format of tic label */
	int	frame1;	/* animation start */
	int	frame2;	/* animation end */
	int	dframe;	/* animation incrment */
	int	frame;	/* current animation frame */
	int	imap;	/* current map position */
	int	prev;	/* previous animation frame */
	int	save;	/* save frame */
	Vec	map;	/* data sample for each display sample */
	Vec	inv;	/* display sample for data sample */
	Vec	interp;	/* 16 bit interption for smooth mapping */
	} *Map, Map_;

/* typed returns */
extern Map MapInit (Axis axis,string name,int imap);
extern void MapSet (Map map,Axis axis,int size,int first,int last,int frame1,int frame2,int dframe);
extern int MapBound (int index,int bound1,int bound2);
extern void MapSetTics (Map map);
extern void MapSwap (Map a,Map b);
extern void MapFlip (Map map);
extern Axis MapAxis (Map map);
extern char* MapName (Map map);
extern Vec MapVec (Map map);
extern Vec MapInterp (Map map);
extern int MapSize (Map map);
extern int MapWindow (Map map);
extern int MapZoom (Map map);
extern int MapFirst (Map map);
extern int MapLast (Map map);
extern int MapLow (Map map);
extern int MapHigh (Map map);
extern int MapFrame (Map map);
extern int MapFrame1 (Map map);
extern int MapNFrame (Map map);
extern int MapPrev (Map map);
extern int MapSave (Map map);
extern float MapValue (Map map, int index);
extern float MapDtic (Map map);
extern float MapTic0 (Map map);
extern float MapTic2 (Map map);
extern int MapNtic (Map map);
extern int MapMovie1 (Map map);
extern int MapMovie2 (Map map);
extern int MapDmovie (Map map);
extern char* MapFormat (Map map);
extern void MapSetFrame (Map map, int frame);
extern void MapSetFrameBounds (Map map,int frame1,int frame2);
extern void MapSetDmovie (Map map,int dframe);
extern void MapSetFrame1 (Map map,int frame);
extern void MapNextFrame (Map map);
extern void MapSetSize (Map map,int size);
extern int MapIndex (Map map,float value);
extern int MapMap (Map map,int index);
extern int MapInverse (Map map,int index);
extern void MapInfo (Map map);
extern void MapSavePar (Map map);
extern void MapDump (Map map);
extern void MapSaveFrame (Map map);
extern void MapRestoreFrame (Map map);

#endif
