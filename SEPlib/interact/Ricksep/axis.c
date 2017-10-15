#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
axis object code
*/

#include <stdio.h>
#include "rick.h"
extern int sep_get_data_axis_par(const char *, int *, int *, float *, float *, char *);

/*
axis init from mandatory getpar
n1=,o1=,d1=,label1= are length, origin, sampling, and name of first axis, etc.
*/
Axis AxisInit (char *tag, int iaxis,int stride, int* data2D)
{
	Axis axis;
	string par;
	int i;
        char temp_ch[20];

	NEW (Axis,axis,1);
	axis->dir = iaxis;
	axis->stride = stride > 0 ? stride : 1;
	axis->stride_s = stride > 0 ? stride : 1;
	sprintf (par,"label%d",iaxis);
        
#ifdef SEPLIB
	if(0!=sep_get_data_axis_par(tag,&iaxis,&axis->size,&axis->first,
   &axis->delta,axis->label))
		err("trouble obtaining axis=%d \n",iaxis);
#else
          sprintf(temp_ch,"n%d",iaxis);
          axis->size=1;
          GETPARINT(temp_ch,"d",&axis->size);
          axis->first=0; axis->delta=1.;
          sprintf(temp_ch,"o%d",iaxis);
          GETPARFLOAT(temp_ch,"d",&axis->first);
          sprintf(temp_ch,"d%d",iaxis);
          GETPARFLOAT(temp_ch,"d",&axis->delta);
#endif

	if(axis->size==1 && iaxis< 3){
		err("n%d can not be 1 \n",iaxis);
	}

	if (iaxis == 3 && axis->size == 1) {
	  axis->size = 2;
	  *data2D = 1;
	}

	sprintf (par,"z%d",iaxis);
	if (rick_getch (par,"f",&axis->zoom) == 0) {
		axis->zoom = 1.;
		}
	NEW (float*,axis->values,axis->size);
	NEW (string*,axis->script,axis->size);
	AxisSetScale (axis);
	AxisSetValues (axis);
	return (axis);
	}

/* axis init from arguments, superceded by getpar */
Axis AxisInit2 (int dir,int stride,string label,int size,float first,float delta, float zoom)
	{
	Axis axis;
	string par;
	int i;

	NEW (Axis,axis,1);
	axis->dir = dir;
	axis->stride = stride > 0 ? stride : 1;
	axis->stride_s = stride > 0 ? stride : 1;
	strcpy (axis->label,label);
	sprintf (par,"label%d",dir);
	rick_getch (par,"s",axis->label);
	axis->size = size;
	sprintf (par,"n%d",dir);
	rick_getch (par,"d",&axis->size);
	axis->first = first;
	sprintf (par,"o%d",dir);
	rick_getch (par,"f",&axis->first);
	axis->delta = delta;
	sprintf (par,"d%d",dir);
	rick_getch (par,"f",&axis->delta);
	if (axis->delta == 0.0) axis->delta = 1.0;
	axis->zoom = zoom;
	sprintf (par,"z%d",dir);
	rick_getch (par,"f",&axis->zoom);;
	NEW (string*,axis->script,axis->size);
	NEW (float*,axis->values,axis->size);
	AxisSetScale (axis);
	AxisSetValues (axis);
	return (axis);
	}
/* axis init from arguments, superceded by getpar */
Axis AxisInit3 (int dir,int stride,string label,int size,float first,float delta, float zoom)
  {
  Axis axis;
  string par;
  int i;

  NEW (Axis,axis,1);
  axis->dir = dir;
  axis->stride = stride > 0 ? stride : 1;
  axis->stride_s = stride > 0 ? stride : 1;
  strcpy (axis->label,label);
  sprintf (par,"label%d",dir);
  axis->size = size;
  sprintf (par,"n%d",dir);
  axis->first = first;
  sprintf (par,"o%d",dir);
  axis->delta = delta;
  sprintf (par,"d%d",dir);
  if (axis->delta == 0.0) axis->delta = 1.0;
  axis->zoom = zoom;
  sprintf (par,"z%d",dir);
  NEW (string*,axis->script,axis->size);
  NEW (float*,axis->values,axis->size);
  AxisSetScale (axis);
  AxisSetValues (axis);
  return (axis);
  }

Axis CopyAxis(int dir, int stride, Axis oldaxis){
  Axis axis;
  string par;
  int i;
  int size;
  float first,delta, zoom;
  char label[1024];


  strcpy(label,oldaxis->label);
  size=AxisSize(oldaxis);
  first=AxisFirst(oldaxis);
  delta=AxisDelta(oldaxis);
  zoom=AxisZoom(oldaxis);



  NEW (Axis,axis,1);
  axis->dir = dir;

  axis->stride = stride > 0 ? stride : 1;
  axis->stride_s = stride > 0 ? stride : 1;
  strcpy (axis->label,label);
  axis->size = size;
  sprintf (par,"n%d",dir);
  axis->first = first;
  sprintf (par,"o%d",dir);
  axis->delta = delta;
  sprintf (par,"d%d",dir);
  if (axis->delta == 0.0) axis->delta = 1.0;
  axis->zoom = zoom;
  sprintf (par,"z%d",dir);
  NEW (string*,axis->script,axis->size);
  NEW (float*,axis->values,axis->size);
  AxisSetScale (axis);
  AxisSetValues (axis);
  return (axis);


}
void SetAxisStride(Axis axis,int istride){
  if (!axis) return;
  axis->stride=istride;
}
void SetAxisStride_s(Axis axis,int istride){
  if (!axis) return;
  axis->stride_s=istride;
}

void SetAxisLabel (Axis axis, string label)
{
  if (!axis) return;
  strcpy(axis->label, label);
  AxisSetValues(axis);
}

/* return stride of axis */
int AxisStride_s (Axis axis)
  {
  if (!axis) return (0);
  return (axis->stride_s);
  }

/* compute axis scale */
void AxisSetScale (Axis axis)
	{
	int i;
	char n[20];

	sprintf (n,"%g",axis->delta-(int)axis->delta);
	axis->prec = strlen(n) - 2;
	axis->prec = axis->prec > 0 ? axis->prec : 0;
	axis->prec = axis->prec < 10 ? axis->prec : 10;
	for (axis->scale=1, i=0; i<axis->prec; i++) axis->scale *= 10;
	}

/* return axis prec */
int AxisPrec (Axis axis)
	{
	if (!axis) return (0);
	else return (axis->prec);
	}

/* return axis scale */
int AxisScale (Axis axis)
	{
	if (!axis) return (0);
	else return (axis->scale);
	}

/* build a list of values for the axis */
void AxisSetValues (Axis axis)
	{
	int i;
	if (!axis) return;
	
	for (i=0; i<axis->size; i++) {
		axis->values[i] = axis->first+i*axis->delta;
		sprintf (axis->script[i],"%s=%g",axis->label,axis->values[i]);
		}
	}

/* return sample size of axis */
int AxisSize (Axis axis)
	{
	if (!axis) return (0);
	return (axis->size);
	}

/* return axis delta */
float AxisDelta (Axis axis)
	{
	if (!axis) return (0.);
	return (axis->delta);
	}

/* return axis first value */
float AxisFirst (Axis axis)
	{
	if (!axis) return (0);
	return (axis->first);
	}

/* return last value */
float AxisLast (Axis axis)
	{
	if (!axis) return (0);
	return (axis->first+(axis->size-1)*axis->delta);
	}

/* return axis zoom */
float AxisZoom (Axis axis)
	{
	if (!axis) return (0);
	return (axis->zoom);
	}

/* return name of axis */
char* AxisLabel (Axis axis)
	{
	if (!axis) return (0);
	return (axis->label);
	}

/* return stride of axis */
int AxisStride (Axis axis)
	{
	if (!axis) return (0);
	return (axis->stride);
	}

/* return value of axis at given index */
float AxisValue (Axis axis,int index)
	{
	if (!axis || index < 0 || index >= axis->size) return (0.);
	else return (axis->values[index]);
	}

/* return scaled value */
int AxisScaledValue (Axis axis,int index)
	{
	if (!axis) return (0);
	if (index < 0 || index >= axis->size) return (0);
	return ((int)(axis->scale*AxisValue(axis,index)));
	}

/* return array of axis values */
float* AxisValues (Axis axis)
	{
	if (!axis) return (0);
	return (axis->values);
	}

/* return index given value */
int AxisIndex (Axis axis,float value)
	{
	int index;

	index = (value - axis->first) / axis->delta+.5;
	if (index < 0 || index >= axis->size) return (NO_INDEX);
	else return (index);
	}

/* return scaled index */
int AxisScaledIndex (Axis axis,int value)
	{
	float fvalue;
	if (!axis) return (NO_INDEX);
	fvalue = value;
	fvalue = fvalue / axis->scale;
	return (AxisIndex (axis,fvalue));
	}

	
/* return axis direction */
int AxisDir (Axis axis)
	{
	if (!axis) return (NO_INDEX);
	else return (axis->dir);
	}

/* return axis script */
char* AxisScript (Axis axis,int index)
	{
	if (!axis || index < 0 || index >= axis->size) return ("");
	else return (axis->script[index]);
	}

/* print info about axis */
void AxisInfo (Axis axis)
	{
	Message message;

	if (!axis) return;
	sprintf (message, "Axis: label%d=%s n%d=%d o%d=%g d%d=%g z%d=%g prec=%d scale=%d",
		axis->dir, axis->label,
		axis->dir, axis->size,
		axis->dir, axis->first,
		axis->dir, axis->delta,
		axis->dir, axis->zoom,
		axis->prec,
		axis->scale);
	UIDelayMessage (message);
	}

/* save axis parameters */
void AxisSavePar (Axis axis)
	{
	Message message;

	if (!axis) return;
	sprintf (message, "Axis%d: label%d=\"%s\" n%d=%d o%d=%g d%d=%g z%d=%g",
		axis->dir,
		axis->dir, axis->label,
		axis->dir, axis->size,
		axis->dir, axis->first,
		axis->dir, axis->delta,
		axis->dir, axis->zoom);
	UISaveMessage (message);
	}
#endif
