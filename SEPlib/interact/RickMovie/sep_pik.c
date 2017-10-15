#include <sepConfig.h>
#if defined (HAVE_MOTIF) || defined(HAVE_ATHENA)
/*
pick list subroutines
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
#include "data.h"
#include "map.h"
#include "render.h"
#include "view.h"
#include "plane.h"
#include "pick.h"
#include "pik.h"
#include "draw.h"
#include "ui.h"

/* main pik object */
PikList pik=0;

/* initialize pik object; read pick= file */
void PikInit (void)
	{
	extern Data data;

	NEW (PikList,pik,1);
	sprintf (pik->file,"%s_picks",DataShortName(data));
	GETPARSTRING ("pick","s",&pik->file);
	pik->npik = 0;
	pik->size = PIK_SIZE;
	GETPARINT ("picksize","d",&pik->size);
	pik->nmax = NPIK;
	GETPARINT ("npick","d",&pik->nmax);
	NEW (Pik*,pik->pik,pik->nmax);
	pik->range = PIK_RANGE;
	GETPARINT ("pickrange","d",&pik->range);
	PikRead ();
	PikDraw (NO_INDEX,DRAW);
	}

/* read pick file */
void PikRead (void)
	{
	int iaxis, ipik;
	FILE *fd;
	float value[DATA_NAXIS];
	char line[100];
	extern Data data;
	Message message;

	if (!pik) return;
	if ((fd=fopen(pik->file,"r")) == NULL){
		sprintf (message, "cant open pick=%s file for reading", pik->file);
		UIMessage (message);
		return;
		}
	for (pik->npik=0; pik->npik<pik->nmax && fgets(line,sizeof(line),fd) != NULL; pik->npik++) {
		sscanf (line,"%f %f %f %f %f %f",value+1,value+2,value+3,value+4,value+5,value);
		 pik->pik[pik->npik][0]=NO_INDEX+1;
		for (iaxis=1; iaxis<DATA_NAXIS; iaxis++) {
			pik->pik[pik->npik][iaxis] = AxisIndex(DataAxis(data,iaxis),value[iaxis]);
			}
		}
	for (ipik=pik->npik; ipik<pik->nmax; ipik++) pik->pik[ipik][0] = NO_INDEX;
	fclose(fd);
	sprintf (message, "%d picks read from file %s", pik->npik,pik->file);
	UIMessage (message);
	}

/* write pick file */
void PikWrite ( char *filename, FILE *fd)
	{
	int ipik, count=0;
	extern Data data;
	Message message;

	if (!pik || !pik->npik) return;
	strcpy (pik->file,filename);
	for (ipik=0, count=0; ipik < pik->npik; ipik++) {
		if (pik->pik[ipik][0] != NO_INDEX) {
			count++;
			fprintf (fd,"%10g %10g %10g %10g %10g %10g\n",
				AxisValue(DataAxis(data,1),pik->pik[ipik][1]),
				AxisValue(DataAxis(data,2),pik->pik[ipik][2]),
				AxisValue(DataAxis(data,3),pik->pik[ipik][3]),
				AxisValue(DataAxis(data,4),pik->pik[ipik][4]),
				AxisValue(DataAxis(data,5),pik->pik[ipik][5]),
				AxisValue(DataAxis(data,0),pik->pik[ipik][0]));
			}
		}
	fclose (fd);
	sprintf (message,"%d picks written to %s",count,pik->file);
	UIMessage (message);
	}

/* draw pick pik0 or all picks (pik0 = NO_INDEX) on all planes */
void PikDraw (int pik0,int draw)
	{
	int ipik, range1, range2, pik1, pik2, dir, frame, x, y, x0, y0, hskew, vskew, size;
	Plane plane;
	extern Plane planelist;
	extern Data data;

	if (!pik || !pik->npik) return;
	if (pik0 != NO_INDEX && pik0 > pik->npik) return;
	for (plane=planelist; plane < planelist+NPLANE && 
		plane->attr->orient != NO_INDEX; plane++) {
		dir = PlaneDir (plane);
		frame = PlaneFrame (plane);
		range1 = frame - pik->range;
		range2 = frame + pik->range;
/*		range1 = range1 > AxisFirst (DataAxis(data,dir)) ? range1 : AxisFirst (DataAxis(data,dir));*/
		range1 = range1 > 0 ? range1 : 0;
		range2 = range2 < AxisSize (DataAxis(data,dir))-1 ? range2 : AxisSize (DataAxis(data,dir))-1;
		if (pik0 == NO_INDEX) {
			pik1 = 0;
			pik2 = pik->npik - 1;
			}
		else	{
			pik1 = pik0;
			pik2 = pik0;
			}
		for (ipik=pik1; ipik<=pik2; ipik++) {
			if (pik->pik[ipik][0] != NO_INDEX && pik->pik[ipik][dir] >= range1 && pik->pik[ipik][dir] <= range2) {
				hskew = 0;
				vskew = 0;
				if (PlaneType(plane) == RENDER_HORZ) hskew = PlaneSkew(plane);
				else vskew = PlaneSkew(plane);
				x = MapInverse(PlaneHmap(plane),pik->pik[ipik][AxisDir(MapAxis(PlaneHmap(plane)))]);
				if (PlaneType(plane) == RENDER_VERT) {
					x = MapSize(PlaneHmap(plane)) - x;
					}
				y = MapInverse(PlaneVmap(plane),pik->pik[ipik][AxisDir(MapAxis(PlaneVmap(plane)))]);
				if (x != NO_INDEX && y != NO_INDEX) {
					x0 = x + PlaneH0(plane) + hskew * (MapSize(PlaneVmap(plane)) - y);
					y0 = y + PlaneV0(plane) + vskew * (PlaneNH(plane) - x);
					if (pik->pik[ipik][dir] == frame) PikDrawEx (x0,y0,draw);
					else if (pik->pik[ipik][dir] > frame) {
						PikDrawBox (x0,y0,pik->range-pik->pik[ipik][dir]+frame,draw);
						}
					else	{
						PikDrawBox (x0,y0,pik->range-frame+pik->pik[ipik][dir],draw);
						}
					}
				}
			}
		}
	}

/* draw an "x" */
void PikDrawEx (int x,int y,int draw)
	{
	if (!pik) return;
	DrawLine (x+pik->size,y+pik->size,x-pik->size,y-pik->size,draw);
	DrawLine (x-pik->size,y+pik->size,x+pik->size,y-pik->size,draw);
	}

/* draw a box of specified size */
void PikDrawBox (int x,int y,int size,int draw)
	{
	if (!pik) return;
	DrawLine (x+size,y+size,x-size,y+size,draw);
	DrawLine (x+size,y-size,x-size,y-size,draw);
	DrawLine (x+size,y+size,x+size,y-size,draw);
	DrawLine (x-size,y+size,x-size,y-size,draw);
	}

/* find nearest visible pick on mouse pick plane */
int PikNear (int x,int y)
	{
	PickPoint_ pick;
	int i, ipik, near, inear, range1, range2, distance, frame, dir, dir1, dir2, x1, y1;
	extern Data data;
	Plane plane;

	if (!pik || !pik->npik) return (NO_INDEX);
	PickDecode (x,y,&pick,1);
	plane = PlaneFind (pick.iaxis[AXIS_DEEP],pick.index[pick.iaxis[AXIS_DEEP]],-1,-1,-1,-1);
	dir = PlaneDir (plane);
	frame = PlaneFrame (plane);
	range1 = frame - pik->range;
	range2 = frame + pik->range;
	range1 = range1 > 0 ? range1 : 0;
	range2 = range2 < AxisSize (DataAxis(data,dir))-1 ? range2 : AxisSize (DataAxis(data,dir))-1;
	near = 100000000;
	inear = NO_INDEX;
	dir1 = pick.iaxis[1];
	dir2 = pick.iaxis[2];
	for (ipik=0; ipik<pik->npik; ipik++) {
		if (pik->pik[ipik][0] != NO_INDEX && pik->pik[ipik][dir] >= range1 && pik->pik[ipik][dir] <= range2) {
			distance = (pik->pik[ipik][dir1] - pick.index[dir1]) * (pik->pik[ipik][dir1] - pick.index[dir1])
			+ (pik->pik[ipik][dir2] - pick.index[dir2]) * (pik->pik[ipik][dir2] - pick.index[dir2]);
			if (distance < near) {
				near = distance;
				inear = ipik;
				}
			}
		}
	if (!PikCoord(inear,plane,&x1,&y1)) return (NO_INDEX);
	if ((x-x1)*(x-x1)+(y-y1)*(y-y1)>PIK_NEAR) return (NO_INDEX);
	return (inear);
	}

int PikCoord (int ipik,Plane plane,int *x,int *y)
	{
	int hskew=0, vskew=0, x1, y1;

	*x = NO_INDEX;
	*y = NO_INDEX;
	if (PlaneType(plane) == RENDER_HORZ) hskew = PlaneSkew(plane);
	else vskew = PlaneSkew(plane);
	x1 = MapInverse(PlaneHmap(plane),pik->pik[ipik][AxisDir(MapAxis(PlaneHmap(plane)))]);
	if (PlaneType(plane) == RENDER_VERT) {
		x1 = MapSize(PlaneHmap(plane)) - x1;
		}
	y1 = MapInverse(PlaneVmap(plane),pik->pik[ipik][AxisDir(MapAxis(PlaneVmap(plane)))]);
	if (x1 == NO_INDEX || y1 == NO_INDEX) return (0);
	*x = x1 + PlaneH0(plane) + hskew * (MapSize(PlaneVmap(plane)) - y1);
	*y = y1 + PlaneV0(plane) + vskew * (PlaneNH(plane) - x1);
	return (1);
	}

/* inquire about pick nearest to mouse pick */
void PikQuery (int x,int y)
	{
	int inear;
	extern Data data;
	Message message;

	inear = PikNear (x,y);
	if (inear != NO_INDEX) {
		sprintf (message,"Pik%d= %s=%g %s=%g %s=%g %s%g %s=%g %s=%g",inear,
			AxisLabel(DataAxis(data,1)),AxisValue(DataAxis(data,1),pik->pik[inear][1]),
			AxisLabel(DataAxis(data,2)),AxisValue(DataAxis(data,2),pik->pik[inear][2]),
			AxisLabel(DataAxis(data,3)),AxisValue(DataAxis(data,3),pik->pik[inear][3]),
			AxisLabel(DataAxis(data,4)),AxisValue(DataAxis(data,4),pik->pik[inear][4]),
			AxisLabel(DataAxis(data,5)),AxisValue(DataAxis(data,5),pik->pik[inear][5]),
			AxisLabel(DataAxis(data,0)),AxisValue(DataAxis(data,0),pik->pik[inear][0]));
		UIMessage (message);
		}
	}

/* add a pick at mouse pick location */
void PikAdd (int x,int y)
	{
	int iaxis;
	PickPoint_ pick;

	if (!pik) return;
	PickDecode (x,y,&pick,1);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) pik->pik[pik->npik][iaxis] = pick.index[iaxis];
	pik->npik = pik->npik < pik->nmax-1 ? pik->npik+1 : pik->nmax-1;
	PikDraw (pik->npik-1,DRAW);
	pik->last_op = PIKOP_ADD;
	pik->last_item = pik->npik-1;
	}

/* delete nearest pick to mouse pick location */
void PikDelete (int x,int y)
	{
	int inear, ipik, iaxis;

	if (!pik || !pik->npik) return;
	inear = PikNear (x,y);
	if (inear == NO_INDEX) return;
	PikDraw(inear,ERASE);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
		pik->pik[pik->npik][iaxis] = pik->pik[inear][iaxis];
		}
	for (ipik=inear; ipik<pik->npik; ipik++) {
		for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
			pik->pik[ipik][iaxis] = pik->pik[ipik+1][iaxis];
			}
		}
	pik->npik = pik->npik > 0 ? pik->npik-1 : 0;
	pik->pik[pik->npik][0] = NO_INDEX;
	pik->last_op = PIKOP_DELETE;
	pik->last_item = pik->npik;
	}

/* move nearest pick to mouse pick location */
void PikMove (int x, int y)
	{
	int inear, iaxis;
	PickPoint_ pick;

	if (!pik || !pik->npik) return;
	inear = PikNear (x,y);
	if (inear == NO_INDEX) return;
	PikDraw (inear,ERASE);
	PickDecode (x,y,&pick,1);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
		pik->pik[pik->npik][iaxis] = pik->pik[inear][iaxis];
		}
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
		pik->pik[inear][iaxis] = pick.index[iaxis];
		}
	PikDraw (inear,DRAW);
	pik->last_op = PIKOP_MOVE;
	pik->last_item = inear;
	}

/* undo a pick modification operation */
void PikUndo (void)
	{
	int iaxis, save;

	switch (pik->last_op) {
	case 'a':
		PikDraw (pik->npik-1,ERASE);
		pik->npik = pik->npik > 0 ? pik->npik-1 : 0;
		pik->pik[pik->npik][0] = NO_INDEX;
		pik->last_op = PIKOP_DELETE;
		pik->last_item = pik->npik;
		break;
	case 'd':
		pik->pik[pik->npik][0] = 0;
	pik->npik = pik->npik < pik->nmax-1 ? pik->npik+1 : pik->nmax-1;
		PikDraw (pik->npik,DRAW);
		pik->last_op = PIKOP_ADD;
		pik->last_item = pik->npik-1;
		break;
	case 'm':
		PikDraw (pik->last_item,ERASE);
		for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
			save = pik->pik[pik->last_item][iaxis];
			pik->pik[pik->last_item][iaxis] = pik->pik[pik->npik][iaxis];
			pik->pik[pik->npik][iaxis] = save;
			}
		PikDraw (pik->last_item,DRAW);
		break;
	case 'u':
		printf ("no undo\n");
		break;
		}
	}


/* print attributes of pick list */
void PikInfo (void)
	{
	Message message;

	sprintf (message,"Pik: file=%s npick=%d nmax=%d range=%d size=%d",
		pik->file,
		pik->npik,
		pik->nmax,
		pik->range,
		pik->size);
	UIMessage (message);
	}

/* save parameters */
void PikSave (void)
	{
	Message message;

	sprintf (message,"Pik: pick=%s npick=%d pickrange=%d picksize=%d",
		pik->file,
		pik->npik,
		pik->range,
		pik->size);
	UISaveMessage (message);
	}
void PikIncreaseSize (void)
	{
	if (!pik) return;
	PikDraw (NO_INDEX,ERASE);
	pik->size++;
	PikDraw (NO_INDEX,DRAW);
	PikInfo ();
	}

void PikDecreaseSize (void)
	{
	if (!pik) return;
	if (pik->size < 2) return;
	PikDraw (NO_INDEX,ERASE);
	pik->size--;
	PikDraw (NO_INDEX,DRAW);
	PikInfo ();
	}

void PikIncreaseRange (void)
	{
	if (!pik) return;
	PikDraw (NO_INDEX,ERASE);
	pik->range++;
	PikDraw (NO_INDEX,DRAW);
	PikInfo ();
	}



void PikDecreaseRange (void)
	{
	if (!pik) return;
	if (pik->range == 0) return;
	PikDraw (NO_INDEX,ERASE);
	pik->range--;
	PikDraw (NO_INDEX,DRAW);
	PikInfo ();
	}


#endif
