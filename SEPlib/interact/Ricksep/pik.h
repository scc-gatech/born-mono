#ifndef PIK_RICK
#define PIK_RICK 3

#include "plane.h"


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
#define	PIKOP_UNLIST	'v'
#define	PIKOP_LIST	'l'
#define	PIKOP_MOVE	'm'
#define	PIKOP_AUTO	't'

#define SEARCH_VIT    0
#define SEARCH_HARLAN 1	
#define DISPLAY_SYMBOL 0
#define DISPLAY_LINES 1
#define MAX_PTS_LINE 2000
#define MAX_PLANES   50

typedef struct {
short loc[DATA_NAXIS];
char  symbol;
int my_display;
} Pik;
typedef void (*CHANGE_PIK_UPDATE)();


typedef struct {
	Pik		*pik;		/* piks */
	string		file;		/* pik file name */
	int		npik;		/* number of piks */
	int		nmax;		/* maximum number of piks */
	int		range;		/* planes to display on */
	int		size;		/* display size in pixels */
	int		last_op;	/* last operation */
	int		last_item;	/* last item changed */
	int   run_cor ;/* run correlation */
	int   depend ;/* run correlation */
	int   search_radius; /*search radius*/	
	int   npaths;        /*number of paths*/
	int   nwind_cor;     /*half width for correlation */
	int   j_display;    /*How often to create symbol when doing auto stuff */
	float min_tol;  /*allowable tollerance when growing surface */
	float max_tol;  /*initial tollerance when growing surface */
	float dtol;     /*tolerance steps*/
	char  cur_symbol; /*symbol used for each added pick*/
	int   display_method; /* display symbols or lines */
	int x_lines[MAX_PLANES][MAX_PTS_LINE],y_lines[MAX_PLANES][MAX_PTS_LINE];
	int pts_per_plane[MAX_PLANES];
	CHANGE_PIK_UPDATE change_pik_func;
	} *PikList;

typedef struct {
PikList pik[NUM_VIEWS];
} *Pikers;

/* API */
void realloc_pik(PikList pik);
void PikGrow3D(void);
void PikMove (int x,int y);


#endif
