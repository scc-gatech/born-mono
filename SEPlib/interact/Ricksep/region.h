#ifndef RICK_REGION
#define RICK_REGION 1
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
/* Region used by X11/Intrinsic.h */

extern void RegionInit (void);
extern int RegionSetLive (int live);
extern int RegionSetBound (int index,int ibound);
extern int RegionSetNeighborhood (int mode);
extern int RegionSetSeed (int *seed);
extern int RegionBound (int ibound);
extern int RegionSize ();
extern int RegionMark0 ();
extern int RegionFill (int *seed,int bound1,int bound2,int value);
extern int RegionClear ();
extern int RegionSetValue (int bound1,int bound2);
extern int RegionRestoreValue ();
extern int RegionInfo ();
extern int RegionNeighbors ();
extern int RegionSavePar();
extern int RegionMark (Buffer data,int size1,int size2,int size3,int seed1,int seed2,int seed3,int min,int max,int neighborhood,Shadow list,int nlist);
extern int RegionMarkBorder (Buffer data,int size1,int size2,int size3);
extern int RegionUnMarkBorder (Buffer data,int size1,int size2,int size3,int Min,int Max);
extern int RegionDump ();








#endif
