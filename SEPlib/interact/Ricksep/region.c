#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
Identifies and processes continginous region between bounds starting at a seed.
There are three definitions of continginous: 6 faces +- 12 edges +- 8 corners.
The eighth bit of the data bytes stores the region mark.
*/
#include <stdio.h>
#include "rick.h"

REgion region;

/* initialize region object */
void RegionInit ()
	{
	int n1, n2, n3, iaxis;
	View myv;

	myv=current_view();

	NEW (REgion,region,1);
	region->live = 1;
	region->neighborhood = MARK_FACE | MARK_EDGE | MARK_CORNER;
	region->bound[0] = 0;
	region->bound[1] = 0;
	for (iaxis=DATA_AXIS0; iaxis<=DATA_AXIS3; iaxis++) region->seed[iaxis] = NO_INDEX;
	n1 = AxisSize (DataAxis(myv->data,DATA_AXIS0));
	n2 = AxisSize (DataAxis(myv->data,DATA_AXIS1));
	n3 = AxisSize (DataAxis(myv->data,DATA_AXIS2));
	region->nlist = 4 * (n1 * n2 + n2 * n3 + n3 * n1);
	NEW (Shadow,region->list,region->nlist);
	region->size = 0;
	region->save = 0;
	region->index = 0;
	}

/* turn on region picking callback */
int RegionSetLive (int live)
	{

	if (!region) return(0);
	region->live = live;
  return(0);
	}

/* set bound */
int RegionSetBound (int index,int ibound)
	{

	if (!region) return(0);
	if (ibound < 0 || ibound > 1) return(0);
	region->bound[ibound] = index;
  return(0);
	}

/* set region neighborhood mode callback */
int RegionSetNeighborhood (int mode)
	{
	
	if (!region) return(0);
	if (region->neighborhood != mode) {
		region->neighborhood = mode;
		RegionMark0 ();
		}
   return(0);
	}

/* set region seed callback */
int RegionSetSeed (int *seed)
	{
	int iaxis;

	if (!region) return(0);
	if (!region->live) return(0);
	for (iaxis=DATA_AXIS0; iaxis<=DATA_AXIS3; iaxis++) region->seed[iaxis] = seed[iaxis];
	UNMARK(region->seed[DATA_VALUE]);
  return(0);
	}

/* return bound */
int RegionBound (int ibound)
	{
	if (!region) return (NO_INDEX);
	if (ibound < 0 || ibound > 1) return (NO_INDEX);
	return (region->bound[ibound]);
	}

/* return region size */
int RegionSize ()
	{
	if (!region) return (NO_INDEX);
	return (region->size);
	}

/* mark a region callback */
int RegionMark0 ()
	{
	Message message;
	View myv;

	myv=current_view();

	if (!region) return(0);
	if (!region->live) return(0);
	if (region->seed[DATA_AXIS1] == NO_INDEX) return(0);
	region->size = RegionMark (DataBuffer(myv->data),
		AxisSize(DataAxis(myv->data,DATA_AXIS1)),
		AxisSize(DataAxis(myv->data,DATA_AXIS2)),
		AxisSize(DataAxis(myv->data,DATA_AXIS3)),
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
		DataValue (myv->data,region->bound[0]),
		DataValue (myv->data,region->bound[1]),
		region->size);
	UIMessage (message);
  return(0);
	}

/* fill a region with a value callback */
int RegionFill (int *seed,int bound1,int bound2,int value)
	{
	int iaxis;
	Message message;
	View myv;

	myv=current_view();

	if (!region) return(0);
	for (iaxis=DATA_AXIS0; iaxis<=DATA_AXIS3; iaxis++) region->seed[iaxis] = seed[iaxis];
	region->bound[0] = bound1;
	region->bound[1] = bound2;
	region->size = RegionMark (DataBuffer(myv->data),
		AxisSize(DataAxis(myv->data,DATA_AXIS1)),
		AxisSize(DataAxis(myv->data,DATA_AXIS2)),
		AxisSize(DataAxis(myv->data,DATA_AXIS3)),
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
		DataValue (myv->data,region->bound[0]),
		DataValue (myv->data,region->bound[1]),
		region->size);
	UIMessage (message);
		return(0);
	}

/* clear region marks in data */
int RegionClear ()
	{
	register Buffer datap, edata;
	View myv;
	myv=current_view();

	if (!region || !myv->data) return(0);
	for (datap=DataBuffer(myv->data), edata=datap+DataSize(myv->data); datap<edata;) {
		UNMARK (*datap++);
		}
	if (region->size > 0) {
		ViewDrawAll ();
		}
	return(0);
	}

/* set all marked cells to value */
int RegionSetValue (int bound1,int bound2)
	{
	unsigned char _table[256];
	register Buffer datap, edata, savep, buf, table;
	register Shadow indexp;
	int ibound;
	View myv;
	myv=current_view();

	if (!region || !myv->data) return(0);
	if (bound1 == NO_INDEX) return(0);
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
	buf = DataBuffer(myv->data);
	indexp = region->index;
	savep = region->save;
	for (datap=buf, edata=datap+DataSize(myv->data); datap<edata; datap++) {
		if (ISMARK(*datap)) {
			*indexp++ = datap - buf;
			*savep = (*datap) & 0x7F;
			*datap = table[*savep++];
			}
		}
	return(0);
	}

/* undo last region smoothing */
int RegionRestoreValue ()
	{
	register Buffer datap, savep, endp;
	register Shadow indexp;
	View myv;
	myv=current_view();

	if (!region) return(0);
	if (region->size == 0) {
		UIMessage ("no smoothing or can only unsmooth once");
		return(0);
		}
	indexp = region->index;
	datap = DataBuffer (myv->data);
	savep = region->save;
	endp = savep + region->size;
	for (; savep<endp;) {
		datap[*indexp++] = *savep++;
		}
	region->size = 0;
	FREE (region->index);
	FREE (region->save);
  return(0);
	}

/* return message about geion callback */
int RegionInfo ()
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
	UIDelayMessage (message);
	return(0);
	}

/* count neighbors */
int RegionNeighbors ()
	{
	return (
		(region->neighborhood & MARK_FACE)/MARK_FACE * 6 +
		(region->neighborhood & MARK_EDGE)/MARK_EDGE * 12 +
		(region->neighborhood & MARK_CORNER)/MARK_CORNER * 8
		);
	}

/* save region parameters */
int RegionSavePar ()
	{
	Message message;

	sprintf (message, "Region: seed=%d,%d,%d value=%d neighborhood=%d",
		region->seed[DATA_AXIS1],
		region->seed[DATA_AXIS2],
		region->seed[DATA_AXIS3],
		region->seed[DATA_VALUE],
		region->neighborhood);
	UISaveMessage (message);
	return(0);
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
int RegionMarkBorder (Buffer data,int size1,int size2,int size3)
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
	return(0);
	}

/* unmark border of voxel cube */
int RegionUnMarkBorder (Buffer data,int size1,int size2,int size3,int Min,int Max)
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
	return(0);
	}

/* write region coordinates and values to a file */
int RegionDump ()
	{
	string filename;
	FILE *fd;
	int n1, n2, n3, i1, i2, i3, i, ii=1, value;
	float *values0, *values1, *values2, *values3;
	register Buffer save;
	register Buffer buf, datap, edata;
	View myv;

	myv=current_view();

	if (region->size <= 0) return(0);
	n1 = AxisSize(DataAxis(myv->data,DATA_AXIS1));
	n2 = AxisSize(DataAxis(myv->data,DATA_AXIS2));
	n3 = AxisSize(DataAxis(myv->data,DATA_AXIS3));
	values0 = AxisValues(DataAxis(myv->data,DATA_VALUE));
	values1 = AxisValues(DataAxis(myv->data,DATA_AXIS1));
	values2 = AxisValues(DataAxis(myv->data,DATA_AXIS2));
	values3 = AxisValues(DataAxis(myv->data,DATA_AXIS3));
	sprintf (filename,"region.list.%d.txt",region->size);
	fd = fopen (filename,"w");
	fprintf (fd,"%5s %5s %5s %5s %5s %10s %10s %10s %10s\n","i","i3","i2","i1","i0","v3","v2","v1","v0");
	fprintf (fd,"%5d %5d %5d %5d %5d %10g %10g %10g %10g\n",0,0,0,0,region->bound[0],values3[0],values2[0],values1[0],values0[region->bound[0]]);
	fprintf (fd,"%5d %5d %5d %5d %5d %10g %10g %10g %10g\n",region->size,n3,n2,n1,region->bound[1],values3[n3-1],values2[n2-1],values1[n1-1],values0[region->bound[1]]);
	buf = DataBuffer(myv->data);
	for (datap=buf, edata=datap+DataSize(myv->data); datap<edata; datap++) {
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
	return(0);
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
