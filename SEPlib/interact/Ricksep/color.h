#ifndef RICK_COLOR
#define RICK_COLOR 2


/*
color object definition

Individual colors and color tables
*/

/* constants */
#define	NCOLOR 132
#ifndef NXCOLOR
#define NXCOLOR 512
#endif
#define	COLOR_SCALE 65535
/* initial contrast value */
#define	CONTRAST	50
#define	CONTRAST0	50
/* color table choices */
#define	COLOR_GRAY	0
#define	COLOR_STRAW	1
#define	COLOR_FLAG	2
#define	COLOR_TIGER	3
#define	COLOR_BLUE	4
#define	COLOR_RAINBOW	5
#define	COLOR_AVO	6
#define COLOR_VELDATA	7
#define	COLOR_RED	0
#define	COLOR_GREEN	1
#define	COLOR_WHITE	2
#define	COLOR_BLACK	3
#define	COLOR_OVERLAY	COLOR_GREEN
#define	COLOR_MARK	COLOR_RED
#define	COLOR_BACKGROUND	COLOR_RED
#define	BACKGROUND_RED		0.8,0.2,0.2
#define	BACKGROUND_GREEN	0.2,0.8,0.2
#define	BACKGROUND_BLUE		0.2,0.2,0.8
#define	BACKGROUND_WHITE	0.8,0.8,0.8
#define	BACKGROUND_BLACK	0.2,0.2,0.2
#define	MARK_RED		1.0,0.0,0.0
#define	MARK_GREEN		0.0,1.0,0.0
#define	MARK_WHITE		1.0,1.0,1.0
#define	MARK_BLACK		0.0,0.0,0.0

/* single color object */
typedef struct {
	float	index;	/* continuum index 0. - 1. */
	float	red;	/* red intensity 0.- 1. */
	float	green;	/* green intensity 0.-1. */
	float	blue;	/* blue intensity 0.-1.*/
	} Color_, *Color;

/* color table object */
typedef struct {
	int	base;	/* index of lowest element */
	int	size;	/* color table size */
	int	color;	/* current color table */
	int	overlay;	/* overlay color */
	int	background;	/* background color */
	int	mark;		/* mark color */
	int	contrast;	/* contrast value 0-100 */
	int	contrast0;	/* contrast value 0-100 */
	float	gpow;	/* gpow */
	unsigned short	red0[NXCOLOR];	/* after contrast red entries */
	unsigned short	green0[NXCOLOR];	/* after contrast green entries */
	unsigned short	blue0[NXCOLOR];	/* after contrast blue entries */
	unsigned short	red[NXCOLOR];	/* after contrast red entries */
	unsigned short	green[NXCOLOR];	/* after contrast green entries */
	unsigned short	blue[NXCOLOR];	/* after contrast blue entries */
	} *ColorTable;

#endif
