#include <sepConfig.h>
#if defined (HAVE_MOTIF) || defined(HAVE_ATHENA)
/*
Identifies and processes continginous region between bounds starting at a seed.
There are three definitions of continginous: 6 faces +- 12 edges +- 8 corners.
The eighth bit of the data bytes stores the region mark.
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <unistd.h>
#include <fcntl.h>
#endif
#include <stdio.h>
#include "main.h"
#include "axis.h"
#include "map.h"
#include "data.h"
#include "render.h"
#include "region.h"
#include "view.h"
#include "ui.h"

REgion region;

/* initialize region object */
void RegionInit (void)
	{
	extern Data data;
	extern REgion region;
	int n1, n2, n3, iaxis;

	NEW (REgion,region,1);
	region->live = 1;
	region->neighborhood = MARK_FACE | MARK_EDGE | MARK_CORNER;
	region->bound[0] = 0;
	region->bound[1] = 0;
	for (iaxis=DATA_AXIS0; iaxis<=DATA_AXIS3; iaxis++) region->seed[iaxis] = NO_INDEX;
	n1 = AxisSize (DataAxis(data,DATA_AXIS0));
	n2 = AxisSize (DataAxis(data,DATA_AXIS1));
	n3 = AxisSize (DataAxis(data,DATA_AXIS2));
	region->nlist = 4 * (n1 * n2 + n2 * n3 + n3 * n1);
	NEW (Shadow,region->list,region->nlist);
	region->size = 0;
	region->save = 0;
	region->index = 0;
	}

/* turn on region picking callback */
void RegionSetLive (int live)
	{
	extern REgion region;

	if (!region) return;
	region->live = live;
	}

/* set bound */
void RegionSetBound (int index,int ibound)
	{
	extern REgion region;

	if (!region) return;
	if (ibound < 0 || ibound > 1) return;
	region->bound[ibound] = index;
	}

/* set region neighborhood mode callback */
void RegionSetNeighborhood (int mode)
	{
	extern REgion region;
	
	if (!region) return;
	if (region->neighborhood != mode) {
		region->neighborhood = mode;
		RegionMark0 ();
		}
	}

/* set region seed callback */
void RegionSetSeed ( int seed[4])
	{
	int iaxis;
	extern REgion region;
	extern Data data;

	if (!region) return;
	if (!region->live) return;
	for (iaxis=DATA_AXIS0; iaxis<=DATA_AXIS3; iaxis++) region->seed[iaxis] = seed[iaxis];
	UNMARK(region->seed[DATA_VALUE]);
	}

/* return bound */
int RegionBound (int ibound)
	{
	if (!region) return (NO_INDEX);
	if (ibound < 0 || ibound > 1) return (NO_INDEX);
	return (region->bound[ibound]);
	}

/* return region size */
int RegionSize (void)
	{
	if (!region) return (NO_INDEX);
	return (region->size);
	}

/* mark a region callback */
void RegionMark0 (void)
	{
	extern REgion region;
	extern Data data;
	Message message;

	if (!region) return;
	if (!region->live) return;
	if (region->seed[DATA_AXIS1] == NO_INDEX) return;
	region->size = RegionMark (DataBuffer(data),
		AxisSize(DataAxis(data,DATA_AXIS1)),
		AxisSize(DataAxis(data,DATA_AXIS2)),
		AxisSize(DataAxis(data,DATA_AXIS3)),
		region->seed[DATA_AXIS1],
		region->seed[DATA_AXIS2],
		region->seed[DATA_AXIS3],
		region->bound[0]<region->bound[1]?region->bound[0]:region->bound[1],
		region->bound[0]>region->bound[1]?region->bound[0]:region->bound[1],
		region->neighborhood,
		region->list,
		region->nlist);
	if (region->size) ViewDrawAll();
	sprintf (message,"Region: min=%g max=%g size=%d",
		DataValue (data,region->bound[0]),
		DataValue (data,region->bound[1]),
		region->size);
	UIMessage (message);
	}

/* fill a region with a value callback */
void RegionFill ( int seed[4], int bound1, int bound2, int value)
	{
	int iaxis;
	extern REgion region;
	extern Data data;
	Message message;

	if (!region) return;
	for (iaxis=DATA_AXIS0; iaxis<=DATA_AXIS3; iaxis++) region->seed[iaxis] = seed[iaxis];
	region->bound[0] = bound1;
	region->bound[1] = bound2;
	region->size = RegionMark (DataBuffer(data),
		AxisSize(DataAxis(data,DATA_AXIS1)),
		AxisSize(DataAxis(data,DATA_AXIS2)),
		AxisSize(DataAxis(data,DATA_AXIS3)),
		region->seed[DATA_AXIS1],
		region->seed[DATA_AXIS2],
		region->seed[DATA_AXIS3],
		region->bound[0],
		region->bound[1],
		region->neighborhood,
		region->list,
		region->nlist);
	if (region->size) {
		RegionSetValue (value,value);
		RegionClear ();
		}
	sprintf (message,"Region: min=%g max=%g size=%d",
		DataValue (data,region->bound[0]),
		DataValue (data,region->bound[1]),
		region->size);
	UIMessage (message);
	}

/* clear region marks in data */
void RegionClear (void)
	{
	extern Data data;
	register Buffer datap, edata;

	if (!region || !data) return;
	for (datap=DataBuffer(data), edata=datap+DataSize(data); datap<edata;) {
		UNMARK (*datap++);
		}
	if (region->size > 0) {
		ViewDrawAll ();
		}
	}

/* set all marked cells to value */
void RegionSetValue (int bound1,int bound2)
	{
	extern Data data;
	unsigned char _table[256];
	register Buffer datap, edata, savep, buf, table;
	register Shadow indexp;
	int ibound;

	if (!region || !data) return;
	if (bound1 == NO_INDEX) return;
	/* initialize table */
	if (region->bound[0] < region->bound[1]) {
		for (ibound=region->bound[0]; ibound<=region->bound[1]; ibound++) {
			_table[ibound] = bound1 + (ibound*(bound2-bound1))/(region->bound[1]-region->bound[0]);
			}
		}
	else if (region->bound[0] > region->bound[1]) {
		for (ibound=region->bound[1]; ibound<=region->bound[0]; ibound++) {
			_table[ibound] = bound1 + (ibound*(bound2-bound1))/(region->bound[1]-region->bound[0]);
			}
		}
	else _table[region->bound[0]] = bound2;
	table = _table;
	FREE (region->index);
	FREE (region->save);
	NEW (Shadow,region->index,region->size);
	NEW (byte,region->save,region->size);
	buf = DataBuffer(data);
	indexp = region->index;
	savep = region->save;
	for (datap=buf, edata=datap+DataSize(data); datap<edata; datap++) {
		if (ISMARK(*datap)) {
			*indexp++ = datap - buf;
			*savep = (*datap) & 0x7F;
			*datap = table[*savep++];
			}
		}
	}

/* undo last region smoothing */
void RegionRestoreValue (void)
	{
	extern REgion region;
	extern Data data;
	register Buffer datap, savep, endp;
	register Shadow indexp;

	if (!region) return;
	if (region->size == 0) {
		UIMessage ("no smoothing or can only unsmooth once");
		return;
		}
	indexp = region->index;
	datap = DataBuffer (data);
	savep = region->save;
	endp = savep + region->size;
	for (; savep<endp;) {
		datap[*indexp++] = *savep++;
		}
	region->size = 0;
	FREE (region->index);
	FREE (region->save);
	}

/* return message about geion callback */
void RegionInfo (void)
	{
	Message message;

	sprintf (message, "Region: live=%d size=%d bound1=%d bound2=%d seed=(%d,%d,%d) value=%d neighborhood=%d nlist=%d",
		region->live,
		region->size,
		region->bound[0],
		region->bound[1],
		region->seed[DATA_AXIS1],
		region->seed[DATA_AXIS2],
		region->seed[DATA_AXIS3],
		region->seed[DATA_VALUE],
		RegionNeighbors(),
		region->nlist);
	UIMessage (message);
	}

/* count neighbors */
int RegionNeighbors (void)
	{
	return (
		(region->neighborhood & MARK_FACE)/MARK_FACE * 6 +
		(region->neighborhood & MARK_EDGE)/MARK_EDGE * 12 +
		(region->neighborhood & MARK_CORNER)/MARK_CORNER * 8
		);
	}

/* save region parameters */
void RegionSavePar (void)
	{
	Message message;

	sprintf (message, "Region: seed=%d,%d,%d value=%d neighborhood=%d",
		region->seed[DATA_AXIS1],
		region->seed[DATA_AXIS2],
		region->seed[DATA_AXIS3],
		region->seed[DATA_VALUE],
		region->neighborhood);
	UISaveMessage (message);
	}

/* mark contiguous voxels, given seed, min and max;
   continginous defined by mode: MARK_FACE=6, MARK_EDGE=12, MARK_CORNER=8
   list returned; nlist is maximum size
 */
int RegionMark (Buffer data,int size1,int size2,int size3,int seed1,int seed2,int seed3,int min,int max,int neighborhood,Shadow list,int nlist)
	{
	int stride1, stride2, stride3, count=0;
	Shadow endold;
	register Shadow old, new, listend;
	register int cell1;
	register Buffer cell;
	int neighbor[26], *face1, *edge1, *corner1;
	register int *nbr, *face2, *edge2, *corner2;

	/* precondition data boundaries */
	region->size = 0;
	RegionClear ();
	region->size = 0;
	RegionMarkBorder (data,size1,size2,size3);
/*
	seed1 = seed1 > 0 ? seed1 : 1;
	seed2 = seed2 > 0 ? seed2 : 1;
	seed3 = seed3 > 0 ? seed3 : 1;
	seed1 = seed1 <= size1 ? seed1 : size1 - 2;
	seed2 = seed2 <= size2 ? seed2 : size2 - 2;
	seed3 = seed3 <= size3 ? seed3 : size3 - 2;
*/
	/* enummerate the neighbor offsets */
	nbr = neighbor;
	/* face neighbors (single stride combinations) */
	stride1 = 1;
	stride2 = size1;
	stride3 = size1 * size2;
	face1 = nbr;
	*nbr++ = stride1;
	*nbr++ = stride2;
	*nbr++ = stride3;
	*nbr++ = -stride1;
	*nbr++ = -stride2;
	*nbr++ = -stride3;
	face2 = nbr;
	/* edge neighbors (two stride combinations) */
	edge1 = nbr;
	*nbr++ = stride1 + stride2;
	*nbr++ = stride1 - stride2;
	*nbr++ = -stride1 + stride2;
	*nbr++ = -stride1 - stride2;
	*nbr++ = stride2 + stride3;
	*nbr++ = stride2 - stride3;
	*nbr++ = -stride2 + stride3;
	*nbr++ = -stride2 - stride3;
	*nbr++ = stride3 + stride1;
	*nbr++ = stride3 - stride1;
	*nbr++ = -stride3 + stride1;
	*nbr++ = -stride3 - stride1;
	edge2 = nbr;
	/* corner neighbors (three stride combinations) */
	corner1 = nbr;
	*nbr++ = stride3 + stride2 + stride1;
	*nbr++ = stride3 + stride2 - stride1;
	*nbr++ = stride3 - stride2 + stride1;
	*nbr++ = stride3 - stride2 - stride1;
	*nbr++ = -stride3 + stride2 + stride1;
	*nbr++ = -stride3 + stride2 - stride1;
	*nbr++ = -stride3 - stride2 + stride1;
	*nbr++ = -stride3 - stride2 - stride1;
	corner2 = nbr;

	cell = data;
	old = list;
	endold = list + 1;
	new = list + 1;
	listend = list + nlist;
	list[0] = seed1 * stride1 + seed2 * stride2 + seed3 * stride3;
	UNMARK(cell[list[0]]);
/*	printf ("list[0]=%d min=%d max=%d value=%d\n",list[0],min,max,cell[list[0]]);*/
	while (old<new && new<listend) {
/*		printf ("old=%d endold=%d new=%d\n",old-list,endold-list,new-list);*/
		/* make room in list by slide new entries over old */
		for (endold=new, new=old, old=list; new<endold;) *old++ = *new++;
		count += old-list;
		for (new=old, endold=old, old=list; old<endold && new<listend; old++) {
			/* examine face cells */
			if (neighborhood & MARK_FACE) {
				for (nbr=face1; nbr<face2 && new<listend; nbr++) {
					*new = *old + *nbr;
					cell1 = cell[*new];
					if (INRANGE(cell1)) {
						MARK(cell[*new++]);
						}
					}
				}
			/* examine edge cells */
			if (neighborhood & MARK_EDGE) {
				for (nbr=edge1; nbr<edge2 && new<listend; nbr++) {
					*new = *old + *nbr;
					cell1 = cell[*new];
					if (INRANGE(cell1)) {
						MARK(cell[*new++]);
						}
					}
				}
			/* examine corner cells */
			if (neighborhood & MARK_CORNER) {
				for (nbr=corner1; nbr<corner2 && new<listend; nbr++) {
					*new = *old + *nbr;
					cell1 = cell[*new];
					if (INRANGE(cell1)) {
						MARK(cell[*new++]);
						}
					}
				}
			}
		}
	/* unmark boundaries */
	RegionUnMarkBorder (data,size1,size2,size3,min,max);
	/* return list size */
	return (count);
	}

/* mark the borders of the data cube */
void RegionMarkBorder ( Buffer data, int size1, int size2, int size3)
	{
	Buffer d3, d2, e2;
	register Buffer d1, e1;
	int stride1, stride2, stride3, sz3, sz2, sz1, strd3, strd2;
	register int strd1;

	stride1 = 1;
	stride2 = size1;
	stride3 = size1 * size2;
	/* n3 edges */
	sz3 = size3;
	sz2 = size2;
	sz1 = size1;
	strd3 = stride3;
	strd2 = stride2;
	strd1 = stride1;
	d3 = data;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		MARK (*d1);
	d3 = data + (sz3-1)*strd3;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		MARK (*d1);

	/* n2 edges */
	sz3 = size2;
	sz2 = size1;
	sz1 = size3;
	strd3 = stride2;
	strd2 = stride1;
	strd1 = stride3;
	d3 = data;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		MARK (*d1);
	d3 = data + (sz3-1)*strd3;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		MARK (*d1);

	/* n1 edges */
	sz3 = size1;
	sz2 = size3;
	sz1 = size2;
	strd3 = stride1;
	strd2 = stride3;
	strd1 = stride2;
	d3 = data;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		MARK (*d1);
	d3 = data + (sz3-1)*strd3;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		MARK (*d1);
	}

/* unmark border of voxel cube */
void RegionUnMarkBorder ( Buffer data, int size1, int size2, int size3, int Min, int Max)
	{
	Buffer d3, d2, e2;
	register Buffer d1, e1;
	int stride1, stride2, stride3, sz3, sz2, sz1, strd2;
	register int strd1, strd3, min, max;

	stride1 = 1;
	stride2 = size1;
	stride3 = size1 * size2;
	min = Min;
	max = Max;
	/* n3 edges */
	sz3 = size3;
	sz2 = size2;
	sz1 = size1;
	strd3 = stride3;
	strd2 = stride2;
	strd1 = stride1;
	d3 = data;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		{if (NOTMARK(*(d1+strd3)) || OUTRANGE(*d1)) UNMARK(*d1);}
	d3 = data + (sz3-1)*strd3;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		{if (NOTMARK(*(d1-strd3)) || OUTRANGE(*d1)) UNMARK(*d1);}

	/* n2 edges */
	sz3 = size2;
	sz2 = size1;
	sz1 = size3;
	strd3 = stride2;
	strd2 = stride1;
	strd1 = stride3;
	d3 = data;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		{if (NOTMARK(*(d1+strd3)) || OUTRANGE(*d1)) UNMARK(*d1);}
	d3 = data + (sz3-1)*strd3;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		{if (NOTMARK(*(d1-strd3)) || OUTRANGE(*d1)) UNMARK(*d1);}

	/* n1 edges */
	sz3 = size1;
	sz2 = size3;
	sz1 = size2;
	strd3 = stride1;
	strd2 = stride3;
	strd1 = stride2;
	d3 = data;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		{if (NOTMARK(*(d1+strd3)) || OUTRANGE(*d1)) UNMARK(*d1);}
	d3 = data + (sz3-1)*strd3;
	for (d2=d3, e2=d2+sz2*strd2; d2<e2; d2+=strd2)
	for (d1=d2, e1=d1+sz1*strd1; d1<e1; d1+=strd1)
		{if (NOTMARK(*(d1-strd3)) || OUTRANGE(*d1)) UNMARK(*d1);}
	}

/* write region coordinates and values to a file */
void RegionDump (void)
	{
	extern REgion region;
	extern Data data;
	string filename;
	FILE *fd;
	int n1, n2, n3, i1, i2, i3, i, ii=1, value;
	float *values0, *values1, *values2, *values3;
	register Buffer save;
	register Buffer buf, datap, edata;

	if (region->size <= 0) return;
	n1 = AxisSize(DataAxis(data,DATA_AXIS1));
	n2 = AxisSize(DataAxis(data,DATA_AXIS2));
	n3 = AxisSize(DataAxis(data,DATA_AXIS3));
	values0 = AxisValues(DataAxis(data,DATA_VALUE));
	values1 = AxisValues(DataAxis(data,DATA_AXIS1));
	values2 = AxisValues(DataAxis(data,DATA_AXIS2));
	values3 = AxisValues(DataAxis(data,DATA_AXIS3));
	sprintf (filename,"region.list.%d.txt",region->size);
	fd = fopen (filename,"w");
	fprintf (fd,"%5s %5s %5s %5s %5s %10s %10s %10s %10s\n","i","i3","i2","i1","i0","v3","v2","v1","v0");
	fprintf (fd,"%5d %5d %5d %5d %5d %10g %10g %10g %10g\n",0,0,0,0,region->bound[0],values3[0],values2[0],values1[0],values0[region->bound[0]]);
	fprintf (fd,"%5d %5d %5d %5d %5d %10g %10g %10g %10g\n",region->size,n3,n2,n1,region->bound[1],values3[n3-1],values2[n2-1],values1[n1-1],values0[region->bound[1]]);
	buf = DataBuffer(data);
	for (datap=buf, edata=datap+DataSize(data); datap<edata; datap++) {
		if (ISMARK(*datap)) {
			i = datap - buf;
			i1 = i % n1;
			i = i / n1;
			i2 = i % n2;
			i3 = i / n2;
			fprintf (fd,"%5d %5d %5d %5d %5d %10g %10g %10g %10g\n",ii++,i3,i2,i1,(*datap) & 0x7f,values3[i3],values2[i2],values1[i1],values0[(*datap) & 0x7f]);
			}
		}
	fclose (fd);
	UIMessage ("region dumped to file");
	}

/* test
#define N 16
unsigned char x[16][16][16];
#define	NLIST	1024
int list[NLIST];
main () {
	int i, j, k;
	for (i=4; i<12; i++)
	for (j=4; j<12; j++)
	for (k=4; k<12; k++)
		x[i][j][k] = 8;
	i = RegionMark (x,N,N,N,6,9,2180,MARK_FACE|MARK_CORNER,list,NLIST);
	printf ("nlist=%d\n",i);
	write (creat("tmp",0644),x,N*N*N);
	}
*/
#endif
