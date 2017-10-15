#ifndef AXIS_RICK
#define AXIS_RICK 2
/*
axis object definition

Three axes define a cube dataset
*/

/* Axis object */
typedef struct {
	string	label;	/* name */
	int	size;	/* length */
	float	first;	/* first value */
	float	delta;	/* sampling rate */
	float	zoom;	/* relative zoom */
	int	dir;	/* direction */
	int	stride;	/* array stride */
	  int stride_s; /* array stride */
	int	prec;	/* slider precision */
	int	scale;	/* slider scaling: 10**scale */
	float	*values;	/* tabulated values */
	string	*script;	/* label for each element */
	} *Axis;

/* API */
int AxisScaledValue (Axis axis,int index);
#endif
