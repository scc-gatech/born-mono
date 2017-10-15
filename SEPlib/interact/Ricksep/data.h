#ifndef RICK_data
#define RICK_data 3
#include <axis.h>
#include "pik.h"
#include "ano.h"
/*
data object definition

Three axes, global information, plus data buffer
Uses axis definitions
*/

#define DATA_NAXIS	6
#define	DATA_VALUE	0
#define	DATA_AXIS0	0
#define	DATA_AXIS1	1
#define	DATA_AXIS2	2
#define	DATA_AXIS3	3
#define	DATA_AXIS4	4
#define	DATA_AXIS5	5
#define	DATA_HEAD	60
#define	DATA_HEAD0	100
#define	DATA_HEAD1	800
#define	DATA_TSIZE	4096
#define	DATA_ESIZE	1
#define	DATA_SEGY	0
#define	DATA_CENT_MIN	0.
#define DATA_LOW	1.
#define	DATA_HIGH	255.
#define	DATA_CENT	99.
#define	DATA_CENT_LOW	(100 - data->cent)
#define	DATA_CENT_HIGH	data->cent
#define	DATA_CENT_MAX	100.
#define	DATA_MIN_GPOW	0.1
#define	DATA_MAX_GPOW	10.0
#define	DATA_TPOW	0.0
#define	DATA_VALUE_SIZE	127
#define DATA_VALUE_BASE 1
typedef Buffer (*BUFFER_PTR)();

/* Data object */
#include "gridhd.h"
typedef struct {
	string	title;	/* dataset name */
	string	file;	/* input file name */
	int	esize;	/* element size in bytes */
	int	segy;	/* segy format or not */
	int	vgrid;	/* vgrid format or not */
	int	hbytes;	/* header bytes to discard; 0 default, 3600 segy=1 */
	string	script;	/* script file name */
	Axis	axis[DATA_NAXIS];	/* axes */
	int	transp;		/* transpose axis */
	long long size;	/* number of samples */
	float	tpow;	/* time power correction */
	float	gpow;	/* gpow of data samples */
	float	low;	/* low clip value of data samples */
	float	high;	/* high clip value of data samples */
	float	min;	/* maximum value */
	float	max;	/* maximum value */
	float	cent;	/* clip percentile */
	float	half;	/* half value */
	int	value_base;	/* minimum value */
	int	value_size;	/* maximum value */
	Buffer	buffer;	/* samples */
	float	histogram[256];	/* histogram of data samples per color */
	int	hist_low;	/* low index of histogram */
	int	hist_high;	/* high index of histogram */
	ghed	gh;	/* vgrid header */
	string	veldata;	/* velocity data file */
	int	vfd;		/* velocity data file pointer */
	float	vlow;		/* low velocity value */
	float	vhigh;		/* high velocity value */
        int data_2d;   /* whether data is 2D */
	int pickable;
	int idat;
        PikList pik;  /* list of picks */
        Anolist ano;  /* list of annotations */
        BUFFER_PTR  grab_buffer;
	char tag[1024];
	} *Data;

#define BUFFER_PARAM_HIGH 100
#define BUFFER_PARAM_LOW 0
#define BUFFER_PARAM_MID 50
#define BUFFER_PARAM_RESOLUTION 10
#define BUFFER_VALUE_HIGH 127
#define BUFFER_VALUE_LOW 1
#define NUM_VEL_CACHE 5
#define PATH_XY 0
#define PATH_YZ 1
#define PATH_XZ 2
#define PATH_GAP_THRESHOLD 50
#define MAX_PATH_LENGTH 10000
#define MIN_PATH_LENGTH 2
#define NR_END 1
#define FREE_ARG char*
#define TINY 1.0e-20
#define SWAP(a,b) {temp=(a);(a)=(b);(b)=temp;}
#define MAX_NUM(a,b) (a > b ? a : b)
#define MIN_NUM(a,b) (a < b ? a : b)
#define PI 3.14159
#define E 2.718
#define POLAR_DELTA_HIGH PI/2
#define POLAR_DELTA_LOW PI/10
#define MIN_SHRINK 0.1
#define MIN_FADE 0.15
#define PATH_DIM 0.35
#define PATH_MOVE_FORWARD 1
#define PATH_MOVE_BACKWARD -1
#define PATH_FROM_FILE 0
#define PATH_FROM_PICKS 1
#define CHECK_GOOD 1
#define CHECK_BAD 0
#define PATH_CLOSE_FACTOR 0.12
#define MAX_PATH_NUM 100

typedef struct{
  Data data[NUM_VIEWS];
  int ndata;

  /** velocity view **/
  Buffer velocityBuffer;                 /** current velocity view buffer **/
  int velocityChoice;                    /** current velocity view method (0, 1, 2) **/
  int velocityBufferLoaded;              /** whether or not velocity buffer is filled **/
  int velocityBufferViewed;              /** whether or not velocity view is used **/
  int invDataMean;                       /** mean of physical cube **/
  Buffer diffFromMode;                   /** buffer of differences from physical cube mode **/
  int maxDiffFromMode;                   /** max difference from physical cube mode **/
  int velocityBufferParam;               /** velocity view parameter [0, 100] **/
  Buffer velocityCache[NUM_VEL_CACHE];   /** array of velocity view buffers for caching**/
  int velocityCacheID[NUM_VEL_CACHE];    /** cache id = the method number **/
  int velocityCacheParam[NUM_VEL_CACHE]; /** cache parameter = velocity view parameter **/
  int velocityCacheUsage[NUM_VEL_CACHE]; /** number of times a cache unit is used **/

  /** path view **/
  float** pathX;                            /** paths **/
  float** pathY; 
  float** pathZ;               
  long pathLength[MAX_PATH_NUM];            /** lengths of path **/
  float** pathXPrev;                        /** previous paths for quick erasing **/
  float** pathYPrev; 
  float** pathZPrev; 
  long pathLengthPrev[MAX_PATH_NUM];        /** backup of length of previous path **/
  char pathSymbol[MAX_PATH_NUM];            /** symbols for paths **/
  long pathMove[MAX_PATH_NUM];              /** indices of movement along paths **/
  int pathMoveDir[MAX_PATH_NUM];            /** directions of movement along paths **/
  int pathNum;                              /** current path **/
  int pathTotal;                            /** total number of paths (including blanks) **/
  int pathTotalFilled;                      /** total number of paths (excluding blanks) **/

  int pathBufferViewed;                     /** whether or not path view is used **/
  int pathView;                             /** which view is calling for path view info **/
  Buffer pathBuffers[NUM_VIEWS][3];         /** array of path view buffers for caching **/
  int pathClose;                            /** project only close to path (huge speed-up) **/
  int pathVelChoice;                        /** which velocity buffer is being used as background **/
} *Datalist;

/* API */
int DataGetpar (Data data,char *tag, char *extra);
void PathViewInterpolate(int pathNum);
int PathViewOn(void);

#endif
