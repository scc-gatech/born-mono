#ifndef RICKMOVE_DATA_H
#define RICKMOVE_DATA_H
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
	unsigned int	size;	/* number of samples */
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
	} *Data;

/* typed returns */
extern Data DataInit(void);
extern void DataLoad (void);
extern void DataLoadByte (Data data);
extern void DataLoadFloat (Data data);
extern float DataCent(float *x,int n,float p);
extern char* DataLabel(Data data);
extern char* DataTitle(Data data);
extern char* DataFile (Data data);
extern char* DataShortName (Data data);
extern Axis DataAxis(Data data,int iaxis);
extern int DataSize (Data data);
extern int DataMaxDim (Data data);
extern Buffer DataBuffer(Data data);
extern float DataValue(Data data,int index);
extern int DataValueSize (Data data);
extern int DataValueBase (Data data);
extern float DataLow (void);
extern float DataHigh (void);
extern int DataHistogram (int i);
extern void DataComputeHistogram (Data data);
extern int DataGridHeader (Data data,int fd);
extern void DataGetpar (Data data);
extern void DataInfo (Data data);
extern void DataValueInfo (Data data);
extern void DataSavePar (Data data);
extern void DataDumpBytes (Data data,char *file, int fd);
extern void DataDumpHeader (Data data,string file,int datafd,int esize);
extern void DataDumpFloats (Data data,char *file,int fd);
extern void Data2Float (Buffer dbuf,float *fbuf,int n,float *min,float *max);

#endif
