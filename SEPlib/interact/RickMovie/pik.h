#ifndef RICKMOVIE_PIK_H
#define RICKMOVIE_PIK_H
/*
point pick object
*/
#ifdef __convex__
#define NPIK		100000
#endif
#ifndef __convex__
#define NPIK		25000
#endif
#define	PIK_RANGE	5
#define	PIK_SIZE	5
#define PIK_NEAR	3000
#define	PIKOP_ADD	'a'
#define	PIKOP_DELETE	'd'
#define	PIKOP_MOVE	'm'

typedef short Pik[DATA_NAXIS];
typedef struct {
	Pik		*pik;		/* piks */
	string		file;		/* pik file name */
	int		npik;		/* number of piks */
	int		nmax;		/* maximum number of piks */
	int		range;		/* planes to display on */
	int		size;		/* display size in pixels */
	int		last_op;	/* last operation */
	int		last_item;	/* last item changed */
	} *PikList;

/* API */
extern void PikInit (void);
extern void PikRead (void);
extern void PikWrite ( char *filename, FILE *fd);
extern void PikDraw (int pik0,int draw);
extern void PikDrawEx (int x,int y,int draw);
extern void PikDrawBox (int x,int y,int size,int draw);
extern int PikNear (int x,int y);
extern int PikCoord (int ipik,Plane plane,int *x,int *y);
extern void PikQuery (int x,int y);
extern void PikAdd (int x,int y);
extern void PikDelete (int x,int y);
extern void PikMove (int x, int y);
extern void PikUndo (void);
extern void PikInfo (void);
extern void PikSave (void);
extern void PikIncreaseSize (void);
extern void PikDecreaseSize (void);
extern void PikIncreaseRange (void);
extern void PikDecreaseRange (void);

#endif
