#include <sepConfig.h>
#if defined (HAVE_MOTIF) || defined(HAVE_ATHENA)
/*
data object code
data object consists of axes, buffers, and dataset info
four input data formats recognized:
	FORMAT	CONDITIONS		ACTIONS
	vgrid	DataGridHeader()==1	DataLoadFloat() or DataLoadByte()
	segy	else segy=1		DataGetpar(), DataLoadFloat()
	floats	else float=1		DataGetpar(), DataLoadFloat()
	bytes	else			DataGetpar(), DataLoadByte()
*/
/* Modified:
   Hector Urdaneta 7/26/96 
		   Had to bring up in the code the line 
                   data->value_size = DATA_VALUE_SIZE;
                   since data->value_size was being used in the 
                   subroutine GetDatapar without initialized
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#if HAVE_SYS_TYPE_H
#include <sys/types.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if defined(__APPLE__) || defined(LINUX)
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif
#include "main.h"
#include "axis.h"
#include "map.h"
#include "data.h"
#include "render.h"
#include "draw.h"
#include "color.h"
#include "ui.h"

/* initialize dataset from getpar */
Data
DataInit (void)
	{
	Data data;
	Axis axis;
	extern int infd;
	int iaxis, i, vfd, ncolor;
	FILE *fd;

	NEW(Data,data,1);
	data->value_size = DATA_VALUE_SIZE;
	/* fetch grid parameters from tail record or pars */
	data->vgrid = -1;
	GETPARINT ("vgrid","d",&data->vgrid);
	if (data->vgrid != 0) {
		if (DataGridHeader (data,infd) == 0) {
			DataGetpar (data);
			}
		}
	else	{
		DataGetpar (data);
		}
	lseek (infd,0,0);
	strcpy (data->title,"stdin");
	if (GETPARSTRING ("title","s",data->title) == 0) 
		GETPARSTRING ("in","s",data->title);
	GETPARSTRING ("in","s",data->file);
	/* get external script file */
	if (GETPARSTRING ("script","s",data->script)) {
		fd = fopen (data->script,"r");
		if (fd == NULL) {UIMessage ("cant open script file");}
		else	{
			for (i=0; i<AxisSize(data->axis[DATA_AXIS3]) &&
			fgets(AxisScript(data->axis[DATA_AXIS3],i),sizeof(string),fd) != NULL; i++) {
				AxisScript(data->axis[DATA_AXIS3],i)[
					strlen(AxisScript(data->axis[DATA_AXIS3],i))-1] = '\0';
				}
			}
		fclose (fd);
		}
	data->tpow = 0.;
	GETPARFLOAT ("tpow","f",&data->tpow);
	data->gpow = 1.;
	GETPARFLOAT ("gpow","f",&data->gpow);
	data->transp = 0;
	GETPARINT ("transp","d",&data->transp);
	/* perform transpose by swapping axes */
	if (data->transp) {
		NEW (Axis,axis,1);
		axis[0] = data->axis[DATA_AXIS1][0];
		data->axis[DATA_AXIS1][0] = data->axis[DATA_AXIS2][0];
		data->axis[DATA_AXIS2][0] = axis[0];
		FREE (axis);
		}
	data->value_base = DATA_VALUE_BASE;

/*      Got moved up in the program, just before call to DataGetpar
	data->value_size = DATA_VALUE_SIZE; */

	if (GETPARINT ("ncolor","d",&ncolor)) data->value_size = ncolor - data->value_base;
	strcpy (data->veldata,"");
	GETPARSTRING ("veldata","s",data->veldata);
	if (strcmp (data->veldata,"")) {
		if ((data->vfd = open (data->veldata,0)) < 0) {
			UIMessage ("cant open veldata");
			data->vfd = 0;
			}
		else if (lseek (infd,0,2) != lseek (data->vfd,0,2)) {
			UIMessage ("veldata different size from input file");
			data->vfd = 0;
			}
		else	{
			GETPARFLOAT ("vlow","f",&(data->vlow));
			GETPARFLOAT ("vhigh","f",&(data->vhigh));
			}
		}
	NEW(Buffer,data->buffer,data->size);
	return (data);
	}

/* fetch grid parameters from getpar */
void DataGetpar (Data data)
	{
	int iaxis;
	string label;

	data->segy = DATA_SEGY;
	GETPARINT ("segy","d",&data->segy);
	data->esize = 0;  GETPARINT ("esize","d",&data->esize);
	if (data->segy) data->esize = 4;
	else if (data->esize == 0) data->esize = 1;
	data->size = 1;
	if (data->segy) data->hbytes = 3600;
	else data->hbytes = 0;
	GETPARINT ("hbytes","d",&data->hbytes);
	for (iaxis=DATA_AXIS1; iaxis<=DATA_AXIS3; iaxis++) {
		/* accumulating size are axis strides */
		data->axis[iaxis] = AxisInit (iaxis,data->size);
		data->size *= AxisSize (data->axis[iaxis]);
		}
	data->axis[DATA_AXIS4] = AxisInit2 (DATA_AXIS4,data->size,"n4",1,0.,1.,1.);
	data->size *= AxisSize (data->axis[DATA_AXIS4]);
	data->axis[DATA_AXIS5] = AxisInit2 (DATA_AXIS5,data->size,"n5",1,0.,1.,1.);
	data->size *= AxisSize (data->axis[DATA_AXIS5]);
	data->max = DATA_HIGH;
	GETPARFLOAT ("max","f",&data->max);
	data->high = data->max;
	GETPARFLOAT ("high","f",&data->high);
	GETPARFLOAT ("pclip","f",&data->high);
	GETPARFLOAT ("clip","f",&data->high);
	if (data->high != DATA_HIGH) {
		data->min = -data->high;
		data->low = data->min;
		}
	else	{
		data->min = DATA_LOW;
		data->low = data->min;
		}
	GETPARFLOAT ("low","f",&data->low);
	GETPARFLOAT ("nclip","f",&data->low);
	GETPARFLOAT ("min","f",&data->min);
	data->cent = DATA_CENT;
	GETPARFLOAT ("perc","f",&data->cent);
	GETPARFLOAT ("cent","f",&data->cent);
	strcpy (label,"samples"); GETPARSTRING ("value","s",label);
	data->axis[DATA_VALUE] = AxisInit2(DATA_VALUE,1,label,data->value_size,
	data->low,(data->high-data->low)/(data->value_size-1),1);
	}

/* fetch grid parameters from tail of vgrid format */
int DataGridHeader (Data data,int fd)
	{
	int size, n1, n2, n3, n4, n5, size1;
	string label;

	size = lseek (fd,-sizeof(data->gh),2);
	if (read(fd,&data->gh,sizeof(data->gh)) < sizeof(data->gh)) { lseek (fd,0,0); data->gh.dtype = 0.; return (0); }
	if (data->gh.scale == 0.0) { data->gh.dtype = 0.; return (0); }
	size1 = rint(data->gh.n1/data->gh.scale)
		* rint(data->gh.n2/data->gh.scale)
		* rint(data->gh.n3/data->gh.scale)
		* rint(data->gh.n4/data->gh.scale)
		* rint(data->gh.n5/data->gh.scale)
		* rint(data->gh.dtype/data->gh.scale);
	if (size != size1) {lseek (fd,0,0); data->gh.dtype = 0.; return (0);}
	data->vgrid = 1;
	strcpy (label,"time"); GETPARSTRING ("label1","s",label);
	n1 = rint(data->gh.n1 / data->gh.scale);
	data->axis[DATA_AXIS1] = AxisInit2 (DATA_AXIS1,1,label,n1,data->gh.o1/data->gh.scale,data->gh.d1/data->gh.scale,1.);
	strcpy (label,"cdp"); GETPARSTRING ("label2","s",label);
	n2 = rint(data->gh.n2 / data->gh.scale);
	data->axis[DATA_AXIS2] = AxisInit2 (DATA_AXIS2,n1,label,
		n2,(data->gh.o2/data->gh.scale),(data->gh.d2/data->gh.scale),1.);
	strcpy (label,"line"); GETPARSTRING ("label3","s",label);
	n3 = rint(data->gh.n3 / data->gh.scale);
	GETPARINT("n3","d",&n3);
	data->axis[DATA_AXIS3] = AxisInit2 (DATA_AXIS3,n1*n2,label,
		n3,(data->gh.o3/data->gh.scale),(data->gh.d3/data->gh.scale),1.);
	strcpy (label,"n4"); GETPARSTRING ("label4","s",label);
	n4 = rint(data->gh.n4 / data->gh.scale);
	GETPARINT("n4","d",&n4);
	data->axis[DATA_AXIS4] = AxisInit2 (DATA_AXIS4,n1*n2*n3,label,
		n4,(data->gh.o4/data->gh.scale),(data->gh.d4/data->gh.scale),1.);
	strcpy (label,"n5"); GETPARSTRING ("label5","s",label);
	n5 = rint(data->gh.n5 / data->gh.scale);
	GETPARINT("n5","d",&n5);
	data->axis[DATA_AXIS5] = AxisInit2 (DATA_AXIS5,n1*n2*n3*n4,label,
		n5,(data->gh.o5/data->gh.scale),(data->gh.d5/data->gh.scale),1.);
	strcpy (label,"amplitude"); GETPARSTRING ("amplitude","s",label);
	data->low = data->gh.gmin / data->gh.scale;
	data->high = data->gh.gmax / data->gh.scale;
	GETPARFLOAT ("high","f",&data->high);
	GETPARFLOAT ("pclip","f",&data->high);
	GETPARFLOAT ("clip","f",&data->high);
	GETPARFLOAT ("low","f",&data->low);
	GETPARFLOAT ("nclip","f",&data->low);
	data->axis[DATA_VALUE] = AxisInit2(DATA_VALUE,1,label,
		data->value_size,data->low,(data->high-data->low)/(data->value_size-1),1.);
	data->esize = rint(data->gh.dtype/data->gh.scale);
	data->size = n1 * n2 * n3 * n4 * n5;
	data->max = data->high;
	data->min = data->low;
	return (1);
	}
	
/* decide which data reading routine to use */
void DataLoad (void)
	{
	extern Data data;

	switch (data->esize) {
	case 4: DataLoadFloat (data); break;
	case 1: DataLoadByte (data); break;
		}
	UIMessage ("data loaded");
	}

/* load byte data; compress to 7 bits- 0-128 */
void DataLoadByte (Data data)
	{
	extern int infd;
	byte table;
	int i, value, ncolor;
	register byte tp;
	register Buffer bp, be;

	/* read data */
	UIMessage ("loading byte data ...");
	lseek (infd,0,0);
	if (data->hbytes) if( 0 > read (infd,data->buffer,data->hbytes)) return;
	if(0 > read (infd,data->buffer,data->size)) return;

	/* compute color compression table table */
	NEW (byte,table,256);
	/* bounds */
	table[0] = 0;
	/* compute lookup table */
	ncolor = ColorSize();
	printf ("ncolor=%d\n",ncolor);
	for (i=0; i<256; i++) {
		table[i] = (i * ncolor) / 255;
		}
	for (bp=data->buffer, be=bp+data->size, tp=table; bp<be; bp++) {
		*bp = tp[*bp];
		}
	DataComputeHistogram (data);
	FREE (table);
	}


/* load and convert float or segy data */
void DataLoadFloat (Data data)
	{
	int i1, i2, i3, n1, n2, n3, base, size, head;
	float *buf1, *buf2, *trace, *tgain;
	register float scale, bias, *bp1, *bp2, *fp, *fe, *gp;
	register int datum;
	register byte dp;
	extern int infd;

	if (data->segy) UIMessage ("loading segy data ...");
	else UIMessage ("loading float data ...");
	/* calculate gain parameters: clip, gpow */
	n1 = AxisSize (data->axis[DATA_AXIS1]);
	n2 = AxisSize (data->axis[DATA_AXIS2]);
	n3 = AxisSize (data->axis[DATA_AXIS3])
	   * AxisSize (data->axis[DATA_AXIS4])
	   * AxisSize (data->axis[DATA_AXIS5]);
	if (data->segy) head = 60;
	else head = 0;
	NEW (float*,buf1,n1*n2);
	NEW (float*,buf2,n1*n2);
	NEW (float*,tgain,n1);
	size = (n1+head)>DATA_HEAD1?(n1+head):DATA_HEAD1;
	size = size > data->hbytes ? size : data->hbytes;
	NEW (float*,trace,size);
	fe = trace + n1 + head;
	bp1 = buf1;
	bp2 = buf2;
	/* time gain vector */
	for (i1=0; i1<n1-1; i1++) {
		tgain[i1] = AxisValue(data->axis[DATA_AXIS1],i1+1);
		tgain[i1] = tgain[i1] != 0.0 ? pow(tgain[i1],data->tpow) : 1.0;
		}
	tgain[n1-1] = tgain[n1-2];
	/* remember first panel in buf1; copy for percentile */
	if (data->segy) {
		if(0 > read (infd,trace,DATA_HEAD0*sizeof(trace[0]))) return;
		if(0 > read (infd,trace,DATA_HEAD1*sizeof(trace[0]))) return;
		}
	else if (data->hbytes) {
		if(0 > read (infd,trace,data->hbytes)) return;
		}
	for (i2=0; i2<n2; i2++) {
		if(0 > read (infd,trace,(n1+head)*sizeof(trace[0]))) return;
		for (fp=trace+head, fe=fp+n1, gp=tgain; fp<fe; fp++, bp1++, bp2++, gp++) {
			*bp1 = *fp * *gp;
			*bp2 = *bp1;
			}
		}
	if (data->low == DATA_LOW && data->high == DATA_HIGH) {
		string label;
		data->min = DataCent (buf2,n1*n2,DATA_CENT_MIN);
		data->low = DataCent (buf2,n1*n2,DATA_CENT_LOW);
		data->high = DataCent (buf2,n1*n2,DATA_CENT_HIGH);
		data->max = DataCent (buf2,n1*n2,DATA_CENT_MAX);
		strcpy (label,AxisLabel(data->axis[DATA_VALUE]));
		FREE (data->axis[DATA_VALUE]);
		data->axis[DATA_VALUE] = AxisInit2(DATA_VALUE,1,label,data->value_size+1,
		data->low,(data->high-data->low)/data->value_size,1);
		}
	FREE (buf2);
	if (data->low == 0.0 && data->high == 0.) {
		err ("first panel appears to be all zeros; set clip");
		}
	if (data->high > data->low) {
		bias = -data->low;
		scale = data->value_size / (data->high - data->low);
		}
	else	{
		bias = 0.;
		scale = 0.5 * data->value_size / data->high;
		}
	dp = data->buffer;
	for (fp=buf1, fe=buf1+n1*n2; fp<fe; fp++, dp++) {
		datum = (*fp + bias) * scale;
		datum = datum > 0 ? datum : 0;
		datum = datum < data->value_size ? datum : data->value_size-1;
		datum += DATA_VALUE_BASE;
		*dp = datum;
		}
	FREE (buf1);
	/* convert rest of traces */
	for (i3=1; i3<n3; i3++)
	for (i2=0; i2<n2; i2++) {
		if (read (infd,trace,(n1+head)*sizeof(trace[0]))<0) break;
		for (fp=trace+head, fe=fp+n1, gp=tgain; fp<fe; fp++, dp++, gp++) {
			datum = (*fp* *gp + bias) * scale;
			datum = datum > 0 ? datum : 0;
			datum = datum < data->value_size ? datum : data->value_size-1;
			datum += DATA_VALUE_BASE;
			*dp = datum;
			}
		}
	DataComputeHistogram (data);
	FREE (trace);
	FREE (tgain);
	}

/*      percentile subroutine
 *      based on Canales, SEP-10
 *      p - percentile <0.,99.999999999999>
 *      x - data
 *      n - vector length
 *	this routine changes data order, so sort a copy
 */
float DataCent (float *x,int n,float p)
        {
        int q;
        register float *i, *j, ak;
        float *low, *hi, buf, *k;
	p = p < 99.999 ? p : 99.999;
	p = p > 0.0 ? p : 0.0;
	q = (p * n) / 100.;
	if (q == n) q--;
        for (low=x, hi=x+n-1, k=x+q; low<hi;)
                {
                ak = *k;
                i = low; j = hi;
                do
                        {
                        while (*i < ak) i++;     
                        while (*j > ak) j--;     
                        if (i<=j)     
                                {
                                buf = *i;
                                *i++ = *j;
                                *j-- = buf;
                                }
                        } while (i<=j);
                if (j<k) low = i; if (k<i) hi = j;
                }
        return (*k);
        }

/* return long data name */
char*
DataLabel (Data data)
	{
	static Message message;

	if (!data) return (0);
	sprintf (message,"%s: %dx%dx%dx%dx%d=%d samples label:%s",data->title,
		AxisSize(data->axis[DATA_AXIS1]),
		AxisSize(data->axis[DATA_AXIS2]),
		AxisSize(data->axis[DATA_AXIS3]),
		AxisSize(data->axis[DATA_AXIS4]),
		AxisSize(data->axis[DATA_AXIS5]),
		data->size,
		AxisLabel(data->axis[DATA_VALUE]));
	return (message);
	}

/* return short data name */
char*
DataTitle (Data data)
	{
	if (!data) return (0);
	return (data->title);
	}

/* return data file */
char*
DataFile (Data data)
	{
	if (!data) return (0);
	return (data->file);
	}

/* return short name of dataset from filename */
char*
DataShortName (Data data)
	{
	static string name;
	char *c;

	/* truncate leading pathname */
	if ((c = strrchr (data->file,'/')) != NULL) strcpy (name,c+1);
	else strcpy (name,data->file);
	/* truncate trailing tail */
	if ((c = strchr (name,'.')) != NULL) *c = '\0';
	return (name);
	}
/* return data buffer */
Buffer
DataBuffer (Data data)
	{
	if (!data) return (0);
	return (data->buffer);
	}

/* return axis reference */
Axis
DataAxis (Data data,int iaxis)
	{
	if (!data || iaxis>DATA_NAXIS) return (0);
	return (data->axis[iaxis]);
	}

/* return data size */
int DataSize (Data data)
	{
	return (data->size);
	}

/* return longest dimension */
int DataMaxDim (Data data)
	{
	int n[DATA_NAXIS], max, iaxis;

	if (!data) return (0);
	for (iaxis=0, max=1; iaxis<DATA_NAXIS-1; iaxis++) {
		max = AxisSize(data->axis[iaxis+1]) > max ? AxisSize(data->axis[iaxis+1]) : max;
		}
	return (max);
	}

/* return data amplitude */
float
DataValue (Data data,int value)
	{
	return (data->low+((data->high-data->low)*((value&0x7F)-data->value_base))/data->value_size);
	}

/* return data value size */
int DataValueSize (Data data)
	{
	if (!data) return (0);
	else return (data->value_size);
	}

/* return base data value */
int DataValueBase (Data data)
	{
	if (!data) return (0);
	else return (data->value_base);
	}


/* return data high clip */
float
DataHigh (void)
	{
	extern Data data;

	if (!data) return (0.0);
	return (data->high);
	}

/* return data low clip */
float
DataLow (void)
	{
	extern Data data;

	if (!data) return (0.0);
	return (data->low);
	}

/* return integer percentage of histogram */
int DataHistogram (int i)
	{
	extern Data data;

	if (!data || i<0 || i>255) return (0);
	return ((int)(100*data->histogram[i]));
	}

void DataComputeHistogram (Data data)
	{
	register Buffer bp, be;
	register float *hp;
	int i;
	float max;

	for (i=0; i<256; i++) {
		data->histogram[i] = 0;
		}
	for (bp=data->buffer,be=bp+data->size,hp=data->histogram; bp<be; bp++) {
		hp[*bp] += 1.;
		}
	for (i=0, max=0, data->hist_low=255, data->hist_high=0; i<256; i++) {
		data->histogram[i] = data->histogram[i] / data->size;
		max = data->histogram[i] > max ? data->histogram[i] : max;
		if (data->hist_low == 255 && data->histogram[i] != 0) data->hist_low = i;
		if (data->histogram[i] != 0) data->hist_high = i;
		}
	max = max > 0. ? 1. / max : 0.;
	for (i=0; i<256; i++) {
		data->histogram[i] = data->histogram[i] * max;
		}
	}

/* print data information */
void DataInfo (Data data)
	{
	Message message;

	sprintf (message,"Data: %s: in=%s %dx%dx%dx%dx%d=%d esize=%d segy=%d hbytes=%d veldata=%s script=%s",
		data->title,data->file,
		AxisSize(data->axis[DATA_AXIS1]),
		AxisSize(data->axis[DATA_AXIS2]),
		AxisSize(data->axis[DATA_AXIS3]),
		AxisSize(data->axis[DATA_AXIS4]),
		AxisSize(data->axis[DATA_AXIS5]),
		data->size,
		data->esize,
		data->segy,
		data->hbytes,
		data->veldata,
		data->script);
	UIMessage (message);
	}

/* data value parameters */
void DataValueInfo (Data data)
	{
	Message message;

	sprintf (message, "Value: min=%g low=%g high=%g max=%g tpow=%g gpow=%g base=%d size=%d",
		data->min,
		data->low,
		data->high,
		data->max,
		data->tpow,
		data->gpow,
		data->value_base,
		data->value_size);
	UIMessage (message);
	}

/* save data parameters */
void DataSavePar (Data data)
	{
	int iaxis;
	Message message;

	if (!data) return;
	sprintf (message,"Data: %s: in=%s %dx%dx%dx%dx%d=%d esize=%d segy=%d hbytes=%d script=%s",
		data->title,
		data->file,
		AxisSize (data->axis[DATA_AXIS1]),
		AxisSize (data->axis[DATA_AXIS2]),
		AxisSize (data->axis[DATA_AXIS3]),
		AxisSize (data->axis[DATA_AXIS4]),
		AxisSize (data->axis[DATA_AXIS5]),
		data->size,
		data->esize,
		data->segy,
		data->hbytes,
		data->script);
	UISaveMessage (message);
	sprintf (message, "Value: min=%g low=%g high=%g max=%g tpow=%g gpow=%g base=%d size=%d",
		data->min,
		data->low,
		data->high,
		data->max,
		data->tpow,
		data->gpow,
		data->value_base,
		data->value_size);
	UISaveMessage (message);
	for (iaxis=DATA_AXIS1; iaxis<=DATA_AXIS5; iaxis++) {
		AxisSavePar (data->axis[iaxis]);
		}
	}

/* dump data bytes */
void DataDumpBytes (Data data,char *file, int fd)
	{
	string parfile, datafile;
	register byte bp, be;
	Message message;

	DrawWatch(1);
	for (bp=data->buffer, be=bp+data->size; bp<be;) *bp++ <<= 1;
	if (data->size !=
	write (fd,data->buffer,data->size)) {
           perror("DataDumpBytes ");
        }
	for (bp=data->buffer, be=bp+data->size; bp<be;) *bp++ >>= 1;
	DataDumpHeader (data,file,fd,1);
	DrawWatch(0);
	}

/* dump data vgrid header */
void DataDumpHeader (Data data,string file,int datafd,int esize)
	{
	Message message;
	char parfile[80];
	extern FILE *savefd;

	if (data->gh.dtype == 0.) { /* build a header */
		data->gh.scale = 1.;
		data->gh.n1 = AxisSize (data->axis[DATA_AXIS1]) * data->gh.scale;
		data->gh.n2 = AxisSize (data->axis[DATA_AXIS2]) * data->gh.scale;
		data->gh.n3 = AxisSize (data->axis[DATA_AXIS3]) * data->gh.scale;
		data->gh.n4 = AxisSize (data->axis[DATA_AXIS4]) * data->gh.scale;
		data->gh.n5 = AxisSize (data->axis[DATA_AXIS5]) * data->gh.scale;
		data->gh.d1 = AxisDelta (data->axis[DATA_AXIS1]) * data->gh.scale;
		data->gh.d2 = AxisDelta (data->axis[DATA_AXIS2]) * data->gh.scale;
		data->gh.d3 = AxisDelta (data->axis[DATA_AXIS3]) * data->gh.scale;
		data->gh.d4 = AxisDelta (data->axis[DATA_AXIS4]) * data->gh.scale;
		data->gh.d5 = AxisDelta (data->axis[DATA_AXIS5]) * data->gh.scale;
		data->gh.o1 = AxisFirst (data->axis[DATA_AXIS1]) * data->gh.scale;
		data->gh.o2 = AxisFirst (data->axis[DATA_AXIS2]) * data->gh.scale;
		data->gh.o3 = AxisFirst (data->axis[DATA_AXIS3]) * data->gh.scale;
		data->gh.o4 = AxisFirst (data->axis[DATA_AXIS4]) * data->gh.scale;
		data->gh.o5 = AxisFirst (data->axis[DATA_AXIS5]) * data->gh.scale;
		data->gh.dcdp2 = 1.;
		data->gh.dline3 = 1.;
		data->gh.ocdp2 = 1.;
		data->gh.oline3 = 1.;
		data->gh.gmin = data->low * data->gh.scale;
		data->gh.gmax = data->high * data->gh.scale;
		}
	data->gh.dtype = esize * data->gh.scale;
	if (sizeof(data->gh) !=
	write (datafd,&data->gh,sizeof(data->gh))) {
           perror("DataDumpHeader ");
           UIMessage("Cant write full Dump Header");
        }
	close (datafd);
	sprintf (parfile,"%s.H",file);
	if ((savefd = fopen (parfile,"w")) == NULL); UIMessage ("cant create vgrid parfile");
	DataSavePar (data);
	sprintf (message, "Byte data dump into %s",parfile);
	UIMessage (message);
	fclose (savefd);
	}

/* dump data floats */
void DataDumpFloats (Data data,char *file,int fd)
	{
	string parfile, datafile;
	int nn, n3, i;
	Buffer dbuf;
	float *fbuf, min, max;
	Message message;

	DrawWatch (1);
	nn = AxisSize (data->axis[DATA_AXIS1]) * AxisSize (data->axis[DATA_AXIS2]);
	n3 = data->size / nn;
	NEW (float*,fbuf,nn);
	/* convert and dump a plane at a time */
	min = data->high;
	max = data->low;
	for (i=0, dbuf=data->buffer; i<n3; i++, dbuf+=nn) {
		Data2Float (dbuf,fbuf,nn,&min,&max);
		if ((nn*sizeof(fbuf[0])) !=
		write (fd,fbuf,nn*sizeof(fbuf[0]))) {
                    perror("DataDumpFloats ");
                    break;
                   }
		}
	data->gh.gmin = min * data->gh.scale;
	data->gh.gmax = max * data->gh.scale;
	DataDumpHeader (data,file,fd,4);
	FREE (fbuf);
	DrawWatch (0);
	}

void Data2Float (Buffer dbuf,float *fbuf,int n,float *min,float *max)
	{
	register Buffer bp, be;
	register float *fp, *mp, *fe, Min, Max;
	extern Data data;
	Min = *min;
	Max = *max;
	/* convert */
	for (mp=AxisValues(data->axis[DATA_VALUE])-DATA_VALUE_BASE, bp=dbuf, fp=fbuf, be=dbuf+n; bp<be; bp++, fp++) {
		*fp = mp[*bp];
		Max = Max > *fp ? Max : *fp;
		Min = Min < *fp ? Min : *fp;
		}
	*min = Min;
	*max = Max;
	}

#ifdef SGI
int rint (float x)
	{
	int i;

	i = x;
	return ((x-i) < (1-(x-i)) ? i : i+1);
	}
#endif
#endif
