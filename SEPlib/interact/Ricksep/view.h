#ifndef RICK_view
#define RICK_view
#include<render.h>

/*
view object definition

A view consists of one to three data parts, axes labels, and intersection lines
Use axis and map definitions
*/

/* constants */
/* gap between render parts in pixels */
#define	GAP		3
/* portion sides and top of cube or plan occupy */
#define FRAC		.66666
/* flag to draw font; flag to mean is front */


#define	DRAW_FRONT	1
#define	DRAW_SIDE	2
#define	DRAW_TOP	4
#define	DRAW_AXES	8
#define	DRAW_PICKS	16
#define	DRAW_IMAGE	32
#define	DRAW_SHADOW	0
#define	DRAW_ALL	(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES|DRAW_IMAGE)
/* mask to examine determine which side of a shadow */
/* mask to examine index in a shadow */
#define	INDEX_MASK	0x3fffffff
/* axis flags */
#define VIEW_NAXIS	6
#define	AXIS_COLOR	0
#define	AXIS_DOWN	1
#define	AXIS_ACROSS	2
#define	AXIS_DEEP	3
#define	AXIS_4D		4
#define	AXIS_5D		5
/* movie options */
#define	MOVIE_FRONT	AXIS_DEEP
#define	MOVIE_SIDE	AXIS_ACROSS
#define	MOVIE_TOP	AXIS_DOWN
#define	FRAME_ACTUAL	0
#define	FRAME_PERCENT	1
/* view options */
#define	VIEW_FRONT	0
#define	VIEW_SIDE	1
#define	VIEW_TOP	2
#define VIEW_THREE	3
#define	VIEW_CUBE	4
#define	VIEW_ARRAY	5
#define	VIEW_FENCE	6
#define	VIEW_TRANSP	7
#define VIEW_ROUNDROBIN 8
#define	VIEW_XSEC	12
#define	VIEW_PICKS	13
#define VIEW_NAMELIST	{\
"front",\
"side",\
"top",\
"three",\
"cube",\
"array",\
"fence",\
"transp",\
"three-1",\
"three-2",\
"cube-1",\
"cube-2",\
}

/* transparency rate options */
#define VIEW_RATE_ALL	0
#define VIEW_RATE_TENTHS	1
#define VIEW_RATE_EACH	2
/* axes sizing relations */
#define	SHAPE_FIT	0
#define	SHAPE_TRUE	1
#define	SHAPE_PIXEL	2
/* four render margins in pixels */
#define	LEFT_MARGIN		35
#define	RIGHT_MARGIN		20
#define	TOP_MARGIN		45
#define	BASE_MARGIN		55
#define	HMARGIN		(LEFT_MARGIN+RIGHT_MARGIN)
#define VMARGIN		(TOP_MARGIN+BASE_MARGIN)
#define	MARGIN_LEFT		0
#define	MARGIN_RIGHT		1
#define	MARGIN_TOP		2
#define	MARGIN_BASE		3
/* axis drawing options */
#define	AXIS_LEFT	4
#define	TIC_LENGTH	10
#define	AXIS_BASE	8
/* text centering options */
#define	TEXT_H0		1
#define	TEXT_H50	2
#define	TEXT_H100	4
#define	TEXT_V0		8
#define	TEXT_V50	16
#define	TEXT_V100	32

#define MAX_HISTORY 5000
#define MAX_IDNUM 1000000000

typedef void (*CHANGE_LOC_FUNC)();
typedef void (*CLOSE_VIEW_FUNC)();

/** linked list of saved info for history list **/
typedef struct ViewSaver {
  /** frames and axes **/
  int frame[VIEW_NAXIS];           /** current frames **/
  int framePrev[VIEW_NAXIS];       /** previous frames **/
  int frameFirst[VIEW_NAXIS];      /** starting frames **/
  int frameLast[VIEW_NAXIS];       /** ending frames **/
  int frameMFirst[VIEW_NAXIS];     /** starting movie frames **/
  int frameMLast[VIEW_NAXIS];      /** ending movie frames **/
  int frameMDelta[VIEW_NAXIS];     /** delta movie frames **/
  Map map[DATA_NAXIS];             /** pointers to initial map-axes **/

  /** view parameters **/
  int movie;                       /** view->movie **/
  int style;                       /** view->style **/
  int shape;                       /** view->shape **/
  int fence;                       /** view->fence **/

  /** color **/
  int base;
  int color;
  int overlay;
  int background;
  int mark;
  int contrast;
  int contrast0;
  int polarity;
 
  /** feature parameters **/
  int velocityBuffer;              /** which velocity buffer is used **/
  int velocityBufferParam;         /** velocity parameter **/
  int pathBuffer;                  /** which path buffer is used **/
  int roundRobin;                  /** whether round robin is used **/
  int roundRobinView;              /** position within round robin loop **/

  /** linking **/
  int idNum;                       /** ID for this storage unit **/
  struct ViewSaver* next;          /** next struct in list **/
  struct ViewSaver* prev;          /** previous struct in list **/
} ViewSaver;

/* View object */
typedef struct {
  int x0,y0;
  int	wide;	/* drawing surface dimensions */
  int	hite;
  int	margins[4];
  int	movie;		/* side of cube animated */
  int	style;
  int	shape;		/* size relationship between axes */
  int	showpicks;
  int	nacross;	/* array across */
  int	across0;	/* array across start */
  int 	dacross;	/* array across increment */
  int 	ndown;		/* array down */
  int	down0;		/* array down start */
  int	ddown;		/* array down increment */
  int	fence;		/* fence mode */
  int	rate;		/* transparency rate */
  int first;  /*whether or not this is the first portion of the view */
  int active;
  Map	map[DATA_NAXIS]; /* map axes */
  Map map0[DATA_NAXIS];  /* initial map axes */
  Data data;
  char tag[1024];
  char valueTag[1024];
  CHANGE_LOC_FUNC change_loc_func;
  CLOSE_VIEW_FUNC  close_view_func;

  /** history list **/
  ViewSaver* firstSaved;       /** first element of history list **/
  ViewSaver* lastSaved;        /** last element of history list **/
  ViewSaver* currentSaved;     /** current element of history list **/
} *View;

typedef struct {
View view[NUM_VIEWS];
  int cur_view, num_view;
  int view_dim[2]; /*array specifying dimension for display */
  float ratx[NUM_VIEWS];
  float raty[NUM_VIEWS];
  float begx[NUM_VIEWS];
  float begy[NUM_VIEWS];
  
  /** round robin **/
  int roundRobinOn;            /** whether or not round robin is on **/
  int roundRobinView;          /** position within round robin loop **/
} *Views;

/* API */
int current_view_num(void);
int ViewMovieOK(void);
int FindView(int x, int y);
int TymeStart (void);
int TymeEnd (int pixels);

#endif
