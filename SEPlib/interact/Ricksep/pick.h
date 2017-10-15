#ifndef PICK_RICK
#define PICK_RICK 2
/*
pick object definition
A pick object contains pick flags and the picks.
*/

#ifdef __convex__
#define NPICK		300
#define	PICKNLIST	2000
#endif
#ifndef  __convex__
#define NPICK		200
#define PICKNLIST	500
#endif


/* Pick object */
typedef struct {
	int	iaxis[DATA_NAXIS];	/* orientation */
	int	index[DATA_NAXIS];	/* sample indices */
	} *PickPoint, PickPoint_;

/* line of picks */
typedef struct {
	int	iaxis[DATA_NAXIS];	/* orientation */
	int	npick;			/* number of picks */
	int	index[NPICK][DATA_NAXIS];	/* data coordinates */
	} *PickLine;


#endif
