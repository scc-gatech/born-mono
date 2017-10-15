#ifndef RICKMOVIE_PICK_H
#define RICKMOVIE_PICK_H
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

/* typed returns */
extern void PickInit (void);
extern int PickDecode (int x,int y,PickPoint pick,int print);
extern void PickDecodeShadow ( Shadow_ shadow, int index[]);
extern PickLine PickFind (int dir3,int frame3,int dir4,int frame4,int dir5,int frame5);
extern int PickSize (void);
extern int PickAxis (void);
extern int PickDir ( PickLine pickline);
extern int PickCount ( PickLine pickline);
extern int PickIndex ( PickLine pickline, int ipick, int idim);
extern int PickFrame (int ipick);
extern void PickInfo (void);
extern void PickAdd (int x,int y);
extern void PickInsert (int x,int y);
extern void PickReplace (int x,int y);
extern void PickDelete (int x,int y);
extern void PickDraw ( PickLine pickline, int draw);
extern void PickDrawAll (int draw);
extern void PickRead (void);
extern void PickWrite (void);
extern void PickClear ( PickLine pickline);
extern void PickClear0 (void);
extern void PickSetFrame (int index);
extern void PickListInfo (void);
extern void PickSavePar (void);
extern int PickSameFrame (PickPoint pick1,PickPoint pick2);
extern int PickSameDir (PickPoint pick1,PickPoint pick2);
extern int PickSharedDir (PickPoint pick1,PickPoint pick2);
extern int PickBetween (int *a,int *b,int *c);

#endif
