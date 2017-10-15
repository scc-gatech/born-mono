#ifndef RICKMOVIE_REGION_H
#define RICKMOVIE_REGION_H
/*
region picking object
*/
#define MARK(cell)	(cell) |= 0x80
#define UNMARK(cell)	(cell) &= 0x7F
#define ISMARK(cell)	((cell) & 0x80)
#define NOTMARK(cell)	(!(((cell) & 0x80)))
#define INRANGE(cell)	(NOTMARK(cell) && (cell)>=min && (cell)<=max)
#define OUTRANGE(cell)	(((cell)&0x7F)<min || ((cell)&0x7F)>max)
#define	MARK_FACE	1
#define	MARK_EDGE	2
#define	MARK_CORNER	4
#define	REGION_NLIST	100000

typedef struct {
	int	live;		/* regions enabled */
	int	seed[4];	/* seed index */
	int	value;		/* data value at seed */
	int	bound[2];	/* range bounds */
	int	neighborhood;	/* neighborhood mode: face, edge, corner */
	Shadow	list;		/* list of marked points */
	Shadow index;		/* list of saved point indexes */
	Buffer	save;		/* value of marked points */
	int	nlist;		/* list size */
	int	size;		/* count of marked points */
	} *REgion;

/* API */
extern void RegionInit (void);
extern void RegionSetLive (int live);
extern void RegionSetBound (int index,int ibound);
extern void RegionSetNeighborhood (int mode);
extern void RegionSetSeed ( int seed[4]);
extern int RegionBound (int ibound);
extern int RegionSize (void);
extern void RegionMark0 (void);
extern void RegionFill ( int seed[4], int bound1, int bound2, int value);
extern void RegionClear (void);
extern void RegionSetValue (int bound1,int bound2);
extern void RegionRestoreValue (void);
extern void RegionInfo (void);
extern int RegionNeighbors (void);
extern void RegionSavePar (void);
extern int RegionMark (Buffer data,int size1,int size2,int size3,int seed1,int seed2,int seed3,int min,int max,int neighborhood,Shadow list,int nlist);
extern void RegionMarkBorder ( Buffer data, int size1, int size2, int size3);
extern void RegionUnMarkBorder ( Buffer data, int size1, int size2, int size3, int Min, int Max);
extern void RegionDump (void);

/* Region used by X11/Intrinsic.h */
#endif
