#ifndef RICKMOVIE_AXIS_H
#define RICKMOVIE_AXIS_H
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
	int	prec;	/* slider precision */
	int	scale;	/* slider scaling: 10**scale */
	float	*values;	/* tabulated values */
	string	*script;	/* label for each element */
	} *Axis;

/* typed returns */
extern Axis AxisInit (int iaxis, int step);
extern Axis AxisInit2 (int dir,int stride,string label,int size,float first,float delta,float zoom);
extern char* AxisLabel (Axis axis);
extern float AxisValue (Axis axis,int index);
extern char* AxisScript (Axis axis, int index);
extern float AxisFirst (Axis axis);
extern float AxisLast (Axis axis);
extern float AxisDelta (Axis axis);
extern float AxisZoom (Axis axis);
extern float* AxisValues (Axis axis);
extern void AxisSetScale (Axis axis);
extern void AxisSetValues (Axis axis);
extern int AxisPrec (Axis axis);
extern int AxisScale (Axis axis);
extern int AxisSize (Axis axis);
extern int AxisIndex (Axis axis, float value);
extern int AxisScaledIndex (Axis axis, int value);
extern int AxisScaledValue (Axis axis,int index);
extern void AxisInfo (Axis axis);
extern void AxisSavePar (Axis axis);
extern int AxisDir (Axis axis);
extern int AxisStride (Axis axis);

#endif
