#include <sepConfig.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
pick list subroutines
*/

#include<math.h>
#include <stdio.h>
#include "rick.h"
int internal_loop_crap(PickPoint_ pick1, PickPoint_ pick2,int *bshift, int *n_ind, int *n_dep, int *n_ind_shift, int *n_dep_shift,float *myslope, int *n_check,int  *f_dep, int *iaind, int *iadep, int *f_ind);
void quicksort(int index, int first, int last);
void copy_pik(Pik *new, Pik old);
int partition(int first, int last, int iaxis);
void draw_line_portion(int *x_draw, int *y_draw,int nseg, int draw);
void PikDrawLine (int pik0,int draw);

/* main pik object */
Pikers pikers=0;
PikList e_pik;
int search_method=0;
void find_piks(PikList new_list,int *min, int *max);
void realloc_pik(PikList pik);
int  ordered_e_piks(PikList pik,Plane plane,int *x_draw, int *y_draw);
PikList InitPikList(int list_num);

/* initialize pik object; read pick= file */
void PikInit ()
	{
	int i,nview;

	nview=1; rick_getch("nview","d",&nview);
	NEW (Pikers,pikers,1);
	for(i=0; i < nview+1; i++){
		pikers->pik[i]	=InitPikList(i);
	}
	e_pik=pikers->pik[nview];
}

PikList InitPikList(int list_num){
	PikList pik;
  int i;
	char add_to[1024],temp_ch[1024];
	if(list_num==0) strcpy(add_to,"");
	else sprintf(add_to,"%d",list_num);


	NEW (PikList,pik,1);
	sprintf (pik->file,"%s","picks");
	pik->npik = 0;
	pik->size = PIK_SIZE;
	rick_auxgetch ("picksize","d",&pik->size,add_to);
	pik->nmax = NPIK;
	rick_auxgetch("npick","d",&pik->nmax,add_to);
	NEW (Pik*,pik->pik,pik->nmax);
	pik->range = PIK_RANGE;
	pik->cur_symbol='x';
	pik->run_cor=1;
	rick_auxgetch("run_cor","d",&pik->run_cor,add_to);
	pik->search_radius=5;
	rick_auxgetch("search_radius","d",&pik->search_radius,add_to);
	pik->npaths=3;
	rick_auxgetch("npaths","d",&pik->npaths,add_to);
	pik->j_display=8;
	rick_auxgetch("j_display","d",&pik->j_display,add_to);
	pik->nwind_cor=8;
	rick_auxgetch("nwind_cor","d",&pik->nwind_cor,add_to);
	pik->depend=1;
	rick_auxgetch("ind_axis","d",&pik->depend,add_to);
	pik->min_tol=.96;
	rick_auxgetch("min_tol","f",&pik->min_tol,add_to);
	pik->max_tol=1.02;
	rick_auxgetch("max_tol","f",&pik->max_tol,add_to);
	pik->dtol=.022;
	rick_auxgetch("dtol","f",&pik->dtol,add_to);
	pik->display_method=0;
	rick_auxgetch("display_method","d",&pik->display_method,add_to);
  pik->change_pik_func=dummy_pik_change;
	for(i=0; i < MAX_PLANES;i++) pik->pts_per_plane[i]=0;
	rick_auxgetch ("pickrange","d",&pik->range,add_to);
	sprintf(temp_ch,"picks%s\n",add_to);
	strcpy(pik->file,add_to);
  rick_auxgetch("pick","s",pik->file,add_to);
	return(pik);
	}

/* read pick file */
int PikRead (Data data)
	{
	int iaxis, ipik,tempi;
	FILE *fd;
	float value[DATA_NAXIS];
	char line[100];
	Message message;
	View myv;
	char symbol[2];
	PikList pik;

	/*
	myv=current_view();
	pik=myv->data->pik;

	if (!myv->data->pik) return(0);
*/
	pik=data->pik;
	if ((fd=fopen(pik->file,"r")) == NULL){
		sprintf (message, "cant open pick=%s file for reading", pik->file);
		UIMessage (message);
		return(0);
		}
	
	for (pik->npik=0; fgets(line,sizeof(line),fd) != NULL; pik->npik++) {
		sscanf (line,"%s %f %f %f %f %f %f %d",symbol,value+1,value+2,value+3,value+4,value+5,value,&tempi);
		pik->pik[pik->npik].loc[0]=NO_INDEX+1;
		pik->pik[pik->npik].symbol=symbol[0];
		pik->pik[pik->npik].my_display=tempi;
		for (iaxis=1; iaxis<DATA_NAXIS; iaxis++) {
			pik->pik[pik->npik].loc[iaxis] = AxisIndex(DataAxis(data,iaxis),value[iaxis]);
			}
		 if(pik->npik+1==pik->nmax) {realloc_pik(pik);}
		}
	for (ipik=pik->npik; ipik<pik->nmax; ipik++) pik->pik[ipik].loc[0] = NO_INDEX;
	fclose(fd);
	sprintf (message, "%d picks read from file %s", pik->npik,pik->file);
	UIMessage (message);
	return(0);
	}

/* write pick file */
int PikWrite (char *filename,FILE *fd)
	{
	int ipik, count=0;
	Message message;
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;

	if (!pik || !pik->npik) return(0);
	strcpy (pik->file,filename);
	for (ipik=0, count=0; ipik < pik->npik; ipik++) {
		if (pik->pik[ipik].loc[0] != NO_INDEX) {
			count++;
			fprintf (fd,"%c %10g %10g %10g %10g %10g %10g %d\n",
        pik->pik[ipik].symbol,
				AxisValue(DataAxis(myv->data,1),pik->pik[ipik].loc[1]),
				AxisValue(DataAxis(myv->data,2),pik->pik[ipik].loc[2]),
				AxisValue(DataAxis(myv->data,3),pik->pik[ipik].loc[3]),
				AxisValue(DataAxis(myv->data,4),pik->pik[ipik].loc[4]),
				AxisValue(DataAxis(myv->data,5),pik->pik[ipik].loc[5]),
				AxisValue(DataAxis(myv->data,0),pik->pik[ipik].loc[0]),
        pik->pik[ipik].my_display);
			}
		}
	fclose (fd);
	sprintf (message,"%d picks written to %s",count,pik->file);
	UIMessage (message);
  return(0);
	}

/* draw pick pik0 or all picks (pik0 = NO_INDEX) on all planes */
void PikDraw (int pik0,int draw)
	{
	int ipik, range1, range2, pik1, pik2, dir, frame, x, y, x0, y0, hskew, vskew, size;
	Plane plane;
	extern Plane planelist;
	View myv;
	PikList pik;
	int xold,yold,nc,iold;
	int itype,my_range;
	int other_frame[2],ne,i,other_dir[2];

	myv=current_view();
	pik=myv->data->pik;

	iold=current_view_num();
	if(myv->data->pickable ==0) return;

	PikDrawLine(pik0,draw);

	if (!pik || !pik->npik) return;
	if (pik0 != NO_INDEX && pik0 > pik->npik) return;
	for (plane=planelist; plane < planelist+NPLANE && 
		plane->attr->orient != NO_INDEX; plane++) {
		if(plane->pickable==1){
		dir = PlaneDir (plane);
		frame = PlaneFrame (plane);
		my_range=pik->range;
		if(pik->display_method==1){
      /*if itype ==1 we are doing lines */
			if(AxisDir(MapAxis(PlaneHmap(plane))) == pik->depend ||
			AxisDir(MapAxis(PlaneVmap(plane))) == pik->depend ){
				 itype=1;
					my_range=0;
			}
		}
		else itype=0;
	  ne=0;
		for(i=1; i < DATA_NAXIS; i++){
			if( AxisDir(MapAxis(PlaneVmap(plane))) ==i || AxisDir(MapAxis(PlaneHmap(plane))) == i || PlaneDir (plane) ==i) {}
			else { other_dir[ne]=i; ne++;}
		}
		for(i=1; i < DATA_NAXIS; i++){
			if(other_dir[0]==AxisDir(MapAxis(myv->map[i]))) 
				other_frame[0]=MapFrame(myv->map[i]);
			if(other_dir[1]==AxisDir(MapAxis(myv->map[i]))) 
				other_frame[1]=MapFrame(myv->map[i]);
		}

		range1 = frame - my_range;
		range2 = frame + my_range;
		range1 = range1 > 0 ? range1 : 0;
		range2 = range2 < AxisSize (DataAxis(myv->data,dir))-1 ? range2 : AxisSize (DataAxis(myv->data,dir))-1;


		/*make the list of points we are going to loop to display */
		if (pik0 == NO_INDEX) {
			pik1 = 0;
			pik2 = pik->npik - 1;
			}
		else	{
			pik1 = pik0;
			pik2 = pik0;
			}

		for (ipik=pik1; ipik<=pik2; ipik++){
/*
pik->pik[ipik].loc[0],pik->pik[ipik].loc[dir],pik->pik[ipik].my_display);
*/
			if (pik->pik[ipik].loc[0] != NO_INDEX && pik->pik[ipik].loc[dir] >= range1 && pik->pik[ipik].loc[dir] <= range2 &&
      other_frame[0]==pik->pik[ipik].loc[other_dir[0]] &&
      other_frame[1]==pik->pik[ipik].loc[other_dir[1]] &&
      (pik->pik[ipik].my_display >0 || itype!=0)) {
         if(x_y_from_pik_plane(pik->pik[ipik],plane,&x0,&y0)==1 &&
					itype==0 || pik->pik[ipik].my_display >0){
					if (pik->pik[ipik].loc[dir] == frame) PikDrawSymb (x0,y0,draw,pik->pik[ipik].symbol);
					else if (pik->pik[ipik].loc[dir] > frame) {
						PikDrawBox (x0,y0,pik->range-pik->pik[ipik].loc[dir]+frame,draw);
						}
					else	{
						PikDrawBox (x0,y0,pik->range-frame+pik->pik[ipik].loc[dir],draw);
						}
					}
				}
			}
		}
	}
	if(using_overlays()==0 && draw==ERASE){
    quick_redo();
	}
}
void PikDrawLine (int pik0,int draw){
	Plane plane;
	View myv;
	PikList pik;
  int iplane,nfound;
	extern Plane planelist;
	myv=current_view();


	pik=myv->data->pik;
	if(pik->display_method!=1 || myv->data->pickable==0) return;

	if (pik0 != NO_INDEX && pik0 > pik->npik) return;
	for (iplane=0,plane=planelist; plane < planelist+NPLANE && 
		plane->attr->orient != NO_INDEX; plane++,iplane++) {
		if(iplane<MAX_PLANES){
    	/*erase the old line*/
			if(pik0!=NO_INDEX){
					 draw_line_portion(pik->x_lines[iplane],
            pik->y_lines[iplane],pik->pts_per_plane[iplane],ERASE);
				}

 			 if(draw==DRAW || pik0!=NO_INDEX){
         nfound=ordered_e_piks(pik,plane,pik->x_lines[iplane],
            pik->y_lines[iplane]);
					if(nfound>0){
            pik->pts_per_plane[iplane]=e_pik->npik;
            draw_line_portion(pik->x_lines[iplane],pik->y_lines[iplane],
              pik->pts_per_plane[iplane],DRAW);

        }
			}
			else{ /*erasing all the piks */
        draw_line_portion(pik->x_lines[iplane],pik->y_lines[iplane],
          pik->pts_per_plane[iplane],ERASE);
				pik->pts_per_plane[iplane]=0;

			}
		}
	}
}
void draw_line_portion(int *x_draw, int *y_draw,int nseg,int draw){
int i;

if(nseg<2) return;
for(i=0; i< nseg-1; i++)
	DrawLine(x_draw[i],y_draw[i],x_draw[i+1],y_draw[i+1],draw);
}

int x_y_from_pik_plane(Pik pik, Plane plane, int *xout, int *yout){
int hskew, vskew,x,y,x0,y0;
				hskew = 0;
				vskew = 0;
				if (PlaneType(plane) == RENDER_HORZ) hskew = PlaneSkew(plane);
				else vskew = PlaneSkew(plane);
				x = MapInverse(PlaneHmap(plane),pik.loc[AxisDir(MapAxis(PlaneHmap(plane)))]);
				if (PlaneType(plane) == RENDER_VERT) {
					x = MapSize(PlaneHmap(plane)) - x;
					}
				y = MapInverse(PlaneVmap(plane),pik.loc[AxisDir(MapAxis(PlaneVmap(plane)))]);
				if (x != NO_INDEX && y != NO_INDEX) {
					x0 = x + PlaneH0(plane) + hskew * (MapSize(PlaneVmap(plane)) - y);
					y0 = y + PlaneV0(plane) + vskew * (PlaneNH(plane) - x);
          *xout=x0; *yout=y0;
					return(1);
        }
				else return(0);

}


/* draw an "x" */
void PikDrawSymb (int x,int y,int draw,char symbol)
	{
	View myv;
	PikList pik;
	char temp_ch[2];

	myv=current_view();
	pik=myv->data->pik;
	if(symbol == 'x'){
		DrawLine (x+pik->size,y+pik->size,x-pik->size,y-pik->size,draw);
		DrawLine (x-pik->size,y+pik->size,x+pik->size,y-pik->size,draw);
	}
	else{
		sprintf(temp_ch,"%c",symbol);
		DrawString(x,y,0,temp_ch,pik->size+2,draw);
	}
	}

/* draw a box of specified size */
void PikDrawBox (int x,int y,int size,int draw)
	{
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;
	if (!pik) return;
	DrawLine (x+size,y+size,x-size,y+size,draw);
	DrawLine (x+size,y-size,x-size,y-size,draw);
	DrawLine (x+size,y+size,x+size,y-size,draw);
	DrawLine (x-size,y+size,x-size,y-size,draw);
	}

/* find nearest visible pick on mouse pick plane */
int  PikNear (int x,int y)
	{
	PickPoint_ pick;
	int i, ipik, near, inear, range1, range2, distance, frame, dir, dir1, dir2, x1, y1;
	View myv;
	PikList pik;
	Plane plane;

	myv=current_view();
	pik=myv->data->pik;

	if (!pik || !pik->npik) return (NO_INDEX);
	PickDecode (x,y,&pick,1);
	plane = PlaneFind (pick.iaxis[AXIS_DEEP],pick.index[pick.iaxis[AXIS_DEEP]],-1,-1,-1,-1,1);
	dir = PlaneDir (plane);
	frame = PlaneFrame (plane);
	range1 = frame - pik->range;
	range2 = frame + pik->range;
	range1 = range1 > 0 ? range1 : 0;
	range2 = range2 < AxisSize (DataAxis(myv->data,dir))-1 ? range2 : AxisSize (DataAxis(myv->data,dir))-1;
	near = 100000000;
	inear = NO_INDEX;
	dir1 = pick.iaxis[1];
	dir2 = pick.iaxis[2];
	for (ipik=0; ipik<pik->npik; ipik++) {
		if (pik->pik[ipik].loc[0] != NO_INDEX && pik->pik[ipik].loc[dir] >= range1 && pik->pik[ipik].loc[dir] <= range2) {
			distance = (pik->pik[ipik].loc[dir1] - pick.index[dir1]) * (pik->pik[ipik].loc[dir1] - pick.index[dir1])
			+ (pik->pik[ipik].loc[dir2] - pick.index[dir2]) * (pik->pik[ipik].loc[dir2] - pick.index[dir2]);
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
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;

	*x = NO_INDEX;
	*y = NO_INDEX;
	if (PlaneType(plane) == RENDER_HORZ) hskew = PlaneSkew(plane);
	else vskew = PlaneSkew(plane);
	x1 = MapInverse(PlaneHmap(plane),pik->pik[ipik].loc[AxisDir(MapAxis(PlaneHmap(plane)))]);
	if (PlaneType(plane) == RENDER_VERT) {
		x1 = MapSize(PlaneHmap(plane)) - x1;
		}
	y1 = MapInverse(PlaneVmap(plane),pik->pik[ipik].loc[AxisDir(MapAxis(PlaneVmap(plane)))]);
	if (x1 == NO_INDEX || y1 == NO_INDEX) return (0);
	*x = x1 + PlaneH0(plane) + hskew * (MapSize(PlaneVmap(plane)) - y1);
	*y = y1 + PlaneV0(plane) + vskew * (PlaneNH(plane) - x1);
	return (1);
	}

/* inquire about pick nearest to mouse pick */
int PikQuery (int x,int y)
	{
	int inear;
	Message message;
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;

	inear = PikNear (x,y);
	if (inear != NO_INDEX) {
		sprintf (message,"Pik%d= symb=%c %s=%g %s=%g %s=%g %s%g %s=%g %s=%g",inear,
			pik->pik[inear].symbol,	
			AxisLabel(DataAxis(myv->data,1)),AxisValue(DataAxis(myv->data,1),pik->pik[inear].loc[1]),
			AxisLabel(DataAxis(myv->data,2)),AxisValue(DataAxis(myv->data,2),pik->pik[inear].loc[2]),
			AxisLabel(DataAxis(myv->data,3)),AxisValue(DataAxis(myv->data,3),pik->pik[inear].loc[3]),
			AxisLabel(DataAxis(myv->data,4)),AxisValue(DataAxis(myv->data,4),pik->pik[inear].loc[4]),
			AxisLabel(DataAxis(myv->data,5)),AxisValue(DataAxis(myv->data,5),pik->pik[inear].loc[5]),
			AxisLabel(DataAxis(myv->data,0)),AxisValue(DataAxis(myv->data,0),pik->pik[inear].loc[0]));
		UIMessage (message);
		}
	return(0);
	}

/* add a pick at mouse pick location */
void PikAdd (int x,int y)
	{
	int iaxis,iold,i;
	PickPoint_ pick;
	View myv;
	PikList pik;


	iold=ViewNumFromXY(x,y);
	if(iold==-1) return;
	SetActiveView(iold);
	myv=current_view();
	pik=myv->data->pik;

	if (!pik) return;
	PickDecode (x,y,&pick,1);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++){
		 pik->pik[pik->npik].loc[iaxis] = pick.index[iaxis];
	}
	pik->pik[pik->npik].symbol=pik->cur_symbol;
	pik->pik[pik->npik].my_display=10;
	if(pik->npik+1==pik->nmax) {realloc_pik(pik);}
	pik->npik++;
	PikDraw (pik->npik-1,DRAW);
	pik->last_op = PIKOP_ADD;
	pik->last_item = pik->npik-1;
	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);
	}

void PikSymbol ()
	{
	int inear, ipik, iaxis;
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;
	UIPikTextRaise(pik->cur_symbol);
}

void set_display(int rad){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	pik->display_method=rad;
	PikDrawLine(NO_INDEX,pik->display_method);
}

void set_radius(int rad){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	pik->search_radius=rad;
}

void set_wind(int wind){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	pik->nwind_cor=wind;
}

void set_pikj(int j){
	View myv;
	PikList pik;
	int i,k,m;
	myv=current_view();
	pik=myv->data->pik;
	pik->j_display=j;
	m=0;
	for(i=0;i < pik->npik; i++){
		if(pik->pik[i].my_display<10 && pik->pik[i].loc[0]!=NO_INDEX){
 		  k=m/pik->j_display; k=m-k*pik->j_display;
			m++;
			if(k==0) pik->pik[i].my_display=1;
			else  pik->pik[i].my_display=0;
		}
	}
	PikDraw(NO_INDEX,ERASE);
	PikDraw(NO_INDEX,DRAW);
}
void set_path(int path){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	pik->npaths=path;
}
void set_search(int search){
search_method=search;
}
int grab_search(){
return(search_method);
}
float grab_min_tol(){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
return(pik->min_tol);
}
void set_min_tol(float tol){
	View myv;
	PikList pik;
	int i, iold;

	iold = current_view_num();
	for (i=0; i<num_views(); i++) {
	  SetActiveView(i);
	  myv=current_view();
	  pik=myv->data->pik;
	  pik->min_tol=tol;
	}
	SetActiveView(iold);
}
void set_max_tol(float tol){
	View myv;
	PikList pik;
	int i, iold;

	iold = current_view_num();
	for (i=0; i<num_views(); i++) {
	  SetActiveView(i);
	  myv=current_view();
	  pik=myv->data->pik;
	  pik->max_tol=tol;
	}
	SetActiveView(iold);
}
void set_dtol(float tol){
	View myv;
	PikList pik;
	int i, iold;

	iold = current_view_num();
	for (i=0; i<num_views(); i++) {
	  SetActiveView(i);
	  myv=current_view();
	  pik=myv->data->pik;
	  pik->dtol=tol;
	}
	SetActiveView(iold);
}

float grab_max_tol(){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	return(pik->max_tol);
}
float grab_dtol(){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
return(pik->dtol);
}



char grab_pik_info(){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	return(pik->cur_symbol);
}


void set_pik_info(char *text){
	View myv;
	PikList pik;
	int i, iold;

	iold = current_view_num();
	for (i=0; i<num_views(); i++) {
	  SetActiveView(i);
	  myv = current_view();
	  pik=myv->data->pik;
	  PikDrawLine(NO_INDEX,ERASE);
	  pik->cur_symbol=text[0];
	  PikDrawLine(NO_INDEX,DRAW);
	}
	SetActiveView(iold);
}
int grab_pik_j(int j){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	return(pik->j_display);
}
int grab_depend(){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	return(pik->depend-1);
}
int grab_run_cor(int mycor){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	return(pik->run_cor);
}


void ErasePikWindow(int x1, int y1, int x2, int y2){
	View myv;
	PikList pik;
	PickPoint_ pick1;
	PickPoint_ pick2;
	int idif1,idif2,iaxis,ipik,i;
	int ilow1,ilow2,ihigh1,ihigh2;
	int good,ipos,iold;

  iold=ViewNumFromXY(x1,y1);
  if(iold==-1) return;
  SetActiveView(iold);
  myv=current_view();
  pik=myv->data->pik;
	PickDecode (x1,y1,&pick1,1);
	PickDecode (x2,y2,&pick2,1);
	idif1=-1; idif2=-1;
	for (iaxis=1; iaxis<DATA_NAXIS; iaxis++) {
		if(pick1.index[iaxis]!=pick2.index[iaxis]){
			if(idif1==-1) idif1=iaxis;
			else if(idif2==-1) idif2=iaxis;
			else return;
		}
	}
	if(idif2==-1) return;
	if(pick1.index[idif1] < pick2.index[idif1]){ 
		ilow1=pick1.index[idif1];
		ihigh1=pick2.index[idif1];
	}
	else{
		ilow1=pick2.index[idif1];
		ihigh1=pick1.index[idif1];
	}

	if(pick1.index[idif2] < pick2.index[idif2]){ 
		ilow2=pick1.index[idif2];
		ihigh2=pick2.index[idif2];
	}
	else{
		ilow2=pick2.index[idif2];
		ihigh2=pick1.index[idif2];
	}

	for(ipik=0; ipik< pik->npik; ipik++){
		good=1;
		for(iaxis=1; iaxis < DATA_NAXIS && good==1; iaxis++){
				ipos=pik->pik[ipik].loc[iaxis];;
			if(iaxis==idif1){
				if(ipos < ilow1 || ipos > ihigh1){
					 good=0;
				}
			}
			else if(iaxis==idif2){
				if(ipos < ilow2 || ipos > ihigh2){ good=0;
				}
			}
			else{
				if(ipos != pick1.index[iaxis]){ good=0;
				}
			}
	
		}
		if(good==1){
			 delete_point(ipik);
			 ipik--;
		}
	}


	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);

}


void set_depend(int mydep){
	View myv;
	PikList pik;
	myv=current_view();
	PikDrawLine(NO_INDEX,ERASE);
	pik=myv->data->pik;
	pik->depend=mydep+1;
	PikDrawLine(NO_INDEX,DRAW);

}
void set_run_cor(int mycor){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	pik->run_cor=mycor;

}
int grab_pik_radius(){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	return(pik->search_radius);
}

int grab_pik_nwind(){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	return(pik->nwind_cor);
}

int grab_pik_npath(){
	View myv;
	PikList pik;
	myv=current_view();
	pik=myv->data->pik;
	return(pik->npaths);
}


/* delete nearest pick to mouse pick location */
void PikDelete (int x,int y)
	{
	int inear, ipik, iaxis,i,iold;
	View myv;
	PikList pik;


  iold=ViewNumFromXY(x,y);
  if(iold==-1) return;
  SetActiveView(iold);
  myv=current_view();
  pik=myv->data->pik;


	if (!pik || !pik->npik) return;
	inear = PikNear (x,y);

	delete_point(inear);
	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);
	return;

}

void delete_point(int inear){
	int ipik, iaxis;
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;
	if (inear == NO_INDEX) return;
	PikDraw(inear,ERASE);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
		pik->pik[pik->npik].loc[iaxis] = pik->pik[inear].loc[iaxis];
		}
	pik->pik[pik->npik].my_display=pik->pik[inear].my_display;
	pik->pik[pik->npik].symbol=pik->pik[inear].symbol;
	
	for (ipik=inear; ipik<pik->npik; ipik++) {
		for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
			pik->pik[ipik].loc[iaxis] = pik->pik[ipik+1].loc[iaxis];
			}
		pik->pik[ipik].my_display=pik->pik[ipik+1].my_display;
		pik->pik[ipik].symbol=pik->pik[ipik+1].symbol;
		}
	pik->npik = pik->npik > 0 ? pik->npik-1 : 0;
	pik->pik[pik->npik].loc[0] = NO_INDEX;
	pik->last_op = PIKOP_DELETE;
	pik->last_item = pik->npik;
	}

/* move nearest pick to mouse pick location */
void PikMove (int x,int y)
	{
	int inear, iaxis,iold,i;
	PickPoint_ pick;
	View myv;
	PikList pik;

	
  iold=ViewNumFromXY(x,y);
  if(iold==-1) return;
  SetActiveView(iold);
  myv=current_view();
  pik=myv->data->pik;


	if (!pik || !pik->npik) return;
	inear = PikNear (x,y);
	if (inear == NO_INDEX) return;
	PikDraw (inear,ERASE);
	PickDecode (x,y,&pick,1);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
		pik->pik[pik->npik].loc[iaxis] = pik->pik[inear].loc[iaxis];
		}
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
		pik->pik[inear].loc[iaxis] = pick.index[iaxis];
		}
	PikDraw (inear,DRAW);
	pik->last_op = PIKOP_MOVE;
	pik->last_item = inear;
	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);
	}

/* undo a pick modification operation */
int PikUndo ()
	{
	int iaxis, save,i,iold;
	View myv;
	PikList pik;

  iold=current_view_num();
  if(iold==-1) return(-1);
  myv=current_view();
  pik=myv->data->pik;


	switch (pik->last_op) {
	case 'a':
		PikDraw (pik->npik-1,ERASE);
		pik->npik = pik->npik > 0 ? pik->npik-1 : 0;
		pik->pik[pik->npik].loc[0] = NO_INDEX;
		pik->last_op = PIKOP_DELETE;
		pik->last_item = pik->npik;
		break;
	case PIKOP_LIST:
		PikDraw (NO_INDEX,ERASE);
		for(i=pik->last_item; i <pik->npik; i++){ 
			pik->pik[i].loc[0] = NO_INDEX;
		}
		i= pik->last_item;
		pik->last_op = PIKOP_UNLIST;
		pik->last_item = pik->npik;
		pik->npik = i;
		PikDraw (NO_INDEX,DRAW);
		break;
	case PIKOP_UNLIST:
		i=pik->npik;
		pik->npik =pik->last_item;
		pik->last_item =i;
		for(i=pik->last_item; i <pik->npik; i++){ 
			pik->pik[i].loc[0] = 1.;
			PikDraw (i,DRAW);
		}
		pik->last_op = PIKOP_LIST;
		break;
	case 'd':
		pik->pik[pik->npik].loc[0] = 0;
	  if(pik->npik+1==pik->nmax) {realloc_pik(pik);}
	   pik->npik++;
		PikDraw (pik->npik,DRAW);
		pik->last_op = PIKOP_ADD;
		pik->last_item = pik->npik-1;
		break;
	case 'm':
		PikDraw (pik->last_item,ERASE);
		for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
			save = pik->pik[pik->last_item].loc[iaxis];
			pik->pik[pik->last_item].loc[iaxis] = pik->pik[pik->npik].loc[iaxis];
			pik->pik[pik->npik].loc[iaxis] = save;
			}
		PikDraw (pik->last_item,DRAW);
		break;
	case 'u':
		printf ("no undo\n");
		break;
		}
	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);
  return(0);
	}


/* print attributes of pick list */
int PikInfo ()
	{
	Message message;
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;

	sprintf (message,"Pik: file=%s npick=%d nmax=%d range=%d size=%d",
		pik->file,
		pik->npik,
		pik->nmax,
		pik->range,
		pik->size);
	UIDelayMessage (message);
	return(0);
	}

/* save parameters */
int PikSave ()
	{
	Message message;
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;

	sprintf (message,"Pik: pick=%s npick=%d pickrange=%d picksize=%d",
		pik->file,
		pik->npik,
		pik->range,
		pik->size);
	UISaveMessage (message);
  return(0);
	}
int PikIncreaseSize ()
	{
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;
	if (!pik) return(0);
	PikDraw (NO_INDEX,ERASE);
	pik->size++;
	PikDraw (NO_INDEX,DRAW);
	PikInfo ();
  return(0);
	}

int PikDecreaseSize ()
	{
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;
	if (!pik) return(0);
	if (pik->size < 2) return(0);
	PikDraw (NO_INDEX,ERASE);
	pik->size--;
	PikDraw (NO_INDEX,DRAW);
	PikInfo ();
  return(0);
	}

int PikIncreaseRange ()
	{
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;
	if (!pik) return(0);
	PikDraw (NO_INDEX,ERASE);
	pik->range++;
	PikDraw (NO_INDEX,DRAW);
	PikInfo ();
  return(0);
	}



int PikDecreaseRange ()
	{
	View myv;
	PikList pik;

	myv=current_view();
	pik=myv->data->pik;
	if (!pik) return(0);
	if (pik->range == 0) return(0);
	PikDraw (NO_INDEX,ERASE);
	pik->range--;
	PikDraw (NO_INDEX,DRAW);
	PikInfo ();
  return(0);
	}

#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif
#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif

 void RunGrowPik(int x1, int y1, int x2, int y2){
	View myv;
	PikList pik;
	PickPoint_ pick1;
	PickPoint_ pick2;
	Pik cur_piks[NPIK];
	int idif1,idif2,iaxis,ipik;
	int ilow1,ilow2,ihigh1,ihigh2;
	int good,ipos,nd[5],icor,i,iold;
	int f1,j1,f2,n2,base,ib1,ie1,ib2,ie2,cb1,cb2,ce,cb,fb,fe;
	int extend;
	int nf[5],ff[5],i0,i1,i2,i3,i4,nsz[5],c_shift;
	int nind, ndep;
	int nind_shift,ndep_shift;
	int fdep,find,dir;
	float *buf1,*buf2,*sline,*buf3;
	int *my_line,idep,iind,ncheck,*iline;
	int ia_dep,ia_ind;
	int shift,iz,p1[2],p2[2],n_ind_l,ib_ind,ie_ind;
	int valid,idist,nfound,close,ilike,my_ind,my_dep,have_left,n1;
	int ia,ib,nail_dep[10000],nail_ind[10000],k;
	float slope,mys,samp,ne_ind,f;
	Buffer myd;
	

	
  iold=ViewNumFromXY(x1,y1);
  if(iold==-1) return;
  SetActiveView(iold);
  myv=current_view();
  pik=myv->data->pik;


	PickDecode (x1,y1,&pick1,1);
	PickDecode (x2,y2,&pick2,1);
  if (!PickSameFrame(&pick1,&pick2)) return;
 if(0!=internal_loop_crap(pick1,pick2,&c_shift,&nind,&ndep,&nind_shift,&ndep_shift,&slope,&ncheck,&fdep,&ia_ind,&ia_dep,&find)) return;
	n1=AxisSize(myv->data->axis[ia_ind]);

nfound=0;
	for(i=0; i< pik->npik; i++){
		valid=1; 
		if(pik->pik[i].loc[0]!=NO_INDEX &&
     pik->pik[i].symbol==pik->cur_symbol){ /* valid pik */
			for(iaxis=1; iaxis< DATA_NAXIS; iaxis++){
				cur_piks[nfound].loc[iaxis]=pik->pik[i].loc[iaxis];
				if(iaxis==ia_ind){
					if(cur_piks[nfound].loc[iaxis]< find || 
            cur_piks[nfound].loc[iaxis] > find+nind) valid=0;
				}
				else if(iaxis==ia_dep){
					if(cur_piks[nfound].loc[iaxis]< fdep || 
           cur_piks[nfound].loc[iaxis] > fdep+ndep) valid=0;
				}
			}
				if(valid==1){
					 nail_ind[nfound]=cur_piks[nfound].loc[ia_ind]-find;
					 nail_dep[nfound]=cur_piks[nfound].loc[ia_dep]-fdep;
					 nfound+=1;
				}
		}
	}
	if(nfound==0) return;
	
	 	

/* create my search buffer */
	extend=pik->nwind_cor;
  myd = DataBuffer(myv->data);
	NEW(float*,buf1,ndep*(2*extend+nind));
#define BUF1A(i1,i2) buf1[i1+(i2)*(2*extend+nind)]
	i=0;
	for(idep=fdep; idep < fdep+ndep; idep++){
		for(iind=find-extend; iind < find+nind+extend; iind++){
			if(iind<0 || iind  >= ncheck){
				 buf1[i]=0.;
			}
			else buf1[i]=(float)myd[c_shift+iind*nind_shift+idep*ndep_shift];
			i+=1;
		}
	}

	NEW(int*,iline,ndep);
if(search_method==SEARCH_VIT){
/*create my semblance search function */
	ia=0; ib=0;
	NEW(float*,buf2,ndep*(nind))
#define BUF2A(i1,i2) buf2[i1+(i2)*nind]
  for(idep=0; idep < ndep; idep++){
		/*find the closest point to the left (if it exists) */
		close=-ndep*10;
		for(ipik=0;ipik<nfound; ipik++){
			if(close< cur_piks[ipik].loc[ia_dep]-fdep-idep &&
         cur_piks[ipik].loc[ia_dep]-fdep-idep  <=0 ){
				close=cur_piks[ipik].loc[ia_dep]-fdep-idep ;
				ilike=ipik;
			}
		}
		if(close> -ndep*10){ /* if there was a point to the left */
			my_ind=cur_piks[ilike].loc[ia_ind]-(find-extend);
			my_dep=cur_piks[ilike].loc[ia_dep]-fdep;
    	for(i2=0; i2 < nind; i2++){
      	BUF2A(i2,idep)=wshspickrc(&BUF1A(0,my_dep),
         &BUF1A(0,idep),my_ind,extend+i2, 
          2*extend+nind,
               pik->nwind_cor);
    	}
			if(idep == my_dep && my_ind-extend >=0 && my_ind-extend <nind){
				if(ia==0 && ib==0) { ia=my_dep; ib=my_ind-extend;}
				 BUF2A(my_ind-extend,my_dep)=(float)ndep;
			}
			have_left=-close;
		}
		else have_left=-1;
		close=ndep*10;
		for(ipik=0;ipik<nfound; ipik++){
			if(close> cur_piks[ipik].loc[ia_dep] -fdep-idep &&
        cur_piks[ipik].loc[ia_dep] -fdep-idep >=0){
				close=cur_piks[ipik].loc[ia_dep]-fdep-idep;
				ilike=ipik;
			}
		}
		if(close< ndep*10 && close!=0){ /* if there was a point to the right */
			if(have_left==-1) f=1.;
			else f=(float)have_left/(float)(have_left+close);
			my_ind=cur_piks[ilike].loc[ia_ind]-(find-extend);
			my_dep=cur_piks[ilike].loc[ia_dep]-fdep;
    	for(i2=0; i2 < nind; i2++){
      	BUF2A(i2,idep)=(1.-f)*  BUF2A(i2,idep) +f*wshspickrc(&BUF1A(0,my_dep),
         &BUF1A(0,idep),my_ind,extend+i2, 
          2*extend+nind,
               pik->nwind_cor);

    	}
		}
  }

	vit_max(nind,ndep,pik->npaths,buf2,iline);
	free(buf2);
}
else {
 wshspickr(iline,buf1,nind+2*extend,ndep,nail_ind,nail_dep,nfound,pik->nwind_cor, 5);

}

	pik->last_op = PIKOP_LIST;
 	pik->last_item = pik->npik;

	for(i=0; i< ndep; i++){
		k=pik->npik/pik->j_display; k=pik->npik-k*pik->j_display;
		if(k==0) pik->pik[pik->npik].my_display=1;
		else  pik->pik[pik->npik].my_display=0;
		pik->pik[pik->npik].symbol=pik->cur_symbol;
  	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) pik->pik[pik->npik].loc[iaxis] = pick1.index[iaxis];
		pik->pik[pik->npik].loc[0]=BUF1A(iline[i]+find,i);
		pik->pik[pik->npik].loc[ia_dep]=fdep+i;
		pik->pik[pik->npik].loc[ia_ind]=iline[i]+find;
	  if(pik->npik+1==pik->nmax) {realloc_pik(pik);}
  	pik->npik++;
 	  PikDraw (pik->npik-1,DRAW);
	}
	free(buf1);

	free(iline);
	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);


}


int internal_loop_crap(PickPoint_ pick1, PickPoint_ pick2,int *bshift, int *n_ind, int *n_dep, int *n_ind_shift, int *n_dep_shift,float *myslope, int *n_check,int  *f_dep, int *iaind, int *iadep,int *f_ind)
{
	View myv;
	PikList pik;
	int nd[5],nsz[5],idif1,idif2;
	int nind, ndep,ia,ia_dep,ia_ind;
	int nind_shift,ndep_shift,i,fdep,ncheck;
	int c_shift,find,ilow;
	float slope;

	myv=current_view();
	pik=myv->data->pik;
 idif1=pick1.iaxis[1];
 idif2=pick1.iaxis[2];
	if(pik->depend==idif1) ilow=0;
	else if(pik->depend==idif2) ilow=1;
	else return(-1);
	
	/*FIGURE OUT PORTION OF THE DATA WE ARE INTERESTED IN*/
	nd[0]=AxisSize(myv->data->axis[DATA_AXIS1]);
	nd[1]=AxisSize(myv->data->axis[DATA_AXIS2]);
	nd[2]=AxisSize(myv->data->axis[DATA_AXIS3]);
	nd[3]=AxisSize(myv->data->axis[DATA_AXIS4]);
	nd[4]=AxisSize(myv->data->axis[DATA_AXIS5]);

	nsz[0]=1;
	c_shift=0;


	/*FIGURE OUT SHIFTS FOR INDEP AND DEP AXIS ALSO DESCRIBE LINE 
	   CONNECTING PICK BOX */
	for(i=0 ; i<5; i++){
		if(i>0) nsz[i]= nsz[i-1]* nd[i-1];
		if(i+1==idif1){
			if(ilow==0){
				nind_shift=nsz[i];
				ncheck=nd[i];
				ia_ind=i+1;
			}
			else{
				ia_dep=i+1;
				ndep=(pick1.index[idif1]-pick2.index[idif1]+1);
				nind=(pick1.index[idif2]-pick2.index[idif2]+1);
				ndep_shift=nsz[i];
				if(nind<0){
					ncheck=nd[i];
					nind_shift=nsz[i];
				}
				if(ndep <0){
					ndep=-ndep;
					fdep=pick1.index[idif1];find=pick1.index[idif2];
					slope=((float)(pick2.index[idif2]-pick1.index[idif2]))/
					((float)(pick2.index[idif1]-pick1.index[idif1]));
				}
				else{
					fdep=pick2.index[idif1];find=pick2.index[idif2];
					slope=((float)(pick1.index[idif2]-pick2.index[idif2]))/
					((float)(pick1.index[idif1]-pick2.index[idif1]));
				}
			}
		}
		else if(i+1==idif2){
			if(ilow!=0){
				ncheck=nd[i];
				nind_shift=nsz[i];
				ia_ind=i+1;
			}
			else{
				ia_dep=i+1;
				ndep_shift=nsz[i];
				ndep=(pick1.index[idif2]-pick2.index[idif2]+1);
				nind=(pick1.index[idif1]-pick2.index[idif1]+1);
				if(nind<0) nind=-nind;
				if(ndep <0){
					ndep=-ndep;
					fdep=pick1.index[idif2];find=pick1.index[idif1];
					slope=((float)(pick2.index[idif1]-pick1.index[idif1]))/
					((float)(pick2.index[idif2]-pick1.index[idif2]));
				}
				else{
					fdep=pick2.index[idif2];find=pick2.index[idif1];
					slope=((float)(pick1.index[idif1]-pick2.index[idif1]))/
					((float)(pick1.index[idif2]-pick2.index[idif2]));

				}
			}
		}
		else c_shift+=nsz[i]*pick1.index[i+1];
	}

	*bshift=c_shift;
	*n_ind=nind; *n_dep=ndep;
	*n_ind_shift=nind_shift;
	*n_dep_shift=ndep_shift;
	*myslope=slope;
	*n_check=ncheck;
	*iaind=ia_ind;
	*iadep=ia_dep;
	*f_ind=find;
	*f_dep=fdep;
	return(0);
}

 void RunAutoPik(int x1, int y1, int x2, int y2){
	View myv;
	PikList pik;
	PickPoint_ pick1;
	PickPoint_ pick2;
	int idif1,idif2,iaxis,ipik,iold;
	int ilow1,ilow2,ihigh1,ihigh2;
	int good,ipos,nd[5],icor,i;
	int f1,j1,f2,n2,base,ib1,ie1,ib2,ie2,cb1,cb2,ce,cb,fb,fe;
	int extend;
	int nf[5],ff[5],i0,i1,i2,i3,i4,nsz[5],c_shift;
	int nind, ndep;
	int nind_shift,ndep_shift;
	int fdep,find,dir;
	float *buf1,*buf2,*sline,*buf3;
	int *my_line,idep,iind,ncheck,*iline;
	int ia_dep,ia_ind,k;
	int shift,iz,p1[2],p2[2],nail_dep[3],nail_ind[3],n1;
	float f,slope,mys,samp;
	Buffer myd;

	
  iold=ViewNumFromXY(x1,y1);
  if(iold==-1) return;
  SetActiveView(iold);
  myv=current_view();
  pik=myv->data->pik;


	PickDecode (x1,y1,&pick1,1);
	PickDecode (x2,y2,&pick2,1);
  if (!PickSameFrame(&pick1,&pick2)) return;
	 	
 if(0!=internal_loop_crap(pick1,pick2,&c_shift,&nind,&ndep,&nind_shift,&ndep_shift,&slope,&ncheck,&fdep,&ia_ind,&ia_dep,&find)) return;

	n1=AxisSize(myv->data->axis[ia_ind]);

	/*calcuate the indepent value of the line connecting the box edges*/
	NEW(int*,my_line,ndep+1);
	for(i=0; i<= ndep; i++){
		my_line[i]=(int)( (float)find+slope*i);
	}

	
	
	extend=pik->nwind_cor+pik->search_radius;
  myd = DataBuffer(myv->data);
	NEW(float*,buf1,ndep*(1+2*extend))
#define BUF1B(i1,i2) buf1[i1+(i2)*((1+2*extend))]
	i=0;
	for(idep=fdep; idep < fdep+ndep; idep++){
		for(iind=my_line[idep-fdep]-extend; iind <= my_line[idep-fdep]+extend; iind++){
			if(iind<0 || iind  >= ncheck){
				 buf1[i]=0.;
			}
			else buf1[i]=(float)myd[c_shift+iind*nind_shift+idep*ndep_shift];
			i+=1;
		}
	}




	NEW(int*,iline,ndep);
if(search_method==SEARCH_VIT){

	NEW(float*,buf2,ndep*(1+2*pik->search_radius))
#define BUF2B(i1,i2) buf2[i1+(i2)*(1+2*pik->search_radius)]
	n2=(1+2*pik->search_radius);
	if(pik->run_cor==0) {
		for(idep=0; idep < ndep; idep++){
			for(i2=0; i2 < n2; i2++){
        BUF2B(i2,idep)=BUF1B(pik->nwind_cor+i2,idep);
			}
		}
	}
	else{ 
		iz=pik->search_radius+1;
		for(idep=0; idep < ndep; idep++){
			for(i2=0, shift=-pik->search_radius; i2 < n2; i2++,shift++){
				BUF2B(i2,idep)=wshspickrc(&BUF1B(0,0),
           &BUF1B(0,idep),extend,extend+shift,extend*2+1,pik->nwind_cor);

			}
		}
	}
	BUF2B(pik->search_radius,0)=ndep* BUF2B(pik->search_radius,0);
	BUF2B(pik->search_radius,ndep-1)=ndep* BUF2B(pik->search_radius,ndep-1);
	vit_max(n2,ndep,pik->npaths,buf2,iline);
	free(buf2);
	}
	else{
	 nail_dep[0]=0; nail_dep[1]=ndep-1; nail_ind[0]=extend;nail_ind[1]=extend;
	 wshspickr(iline,buf1,1+2*extend,ndep,nail_ind,nail_dep,2,pik->nwind_cor, 5);
	}



	pik->last_op = PIKOP_LIST;
 	pik->last_item = pik->npik;
	for(i=0; i< ndep; i++){
    k=pik->npik/pik->j_display; k=pik->npik-k*pik->j_display;
    if(k==0) pik->pik[pik->npik].my_display=1;
    else  pik->pik[pik->npik].my_display=0;

		if(my_line[i]+iline[i]-extend-1>=0 && my_line[i]+iline[i]-extend-1<n1){
			
		pik->pik[pik->npik].symbol=pik->cur_symbol;
  	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) pik->pik[pik->npik].loc[iaxis] = pick1.index[iaxis];
		pik->pik[pik->npik].loc[0]=BUF1B(my_line[i]+iline[i]-extend-1,i);
		pik->pik[pik->npik].loc[ia_dep]=fdep+i;
		pik->pik[pik->npik].loc[ia_ind]=my_line[i]+(iline[i]-1-pik->search_radius);
	  if(pik->npik+1==pik->nmax) {realloc_pik(pik);}
  	pik->npik++;
	}
	}
  PikDraw (NO_INDEX,DRAW);


	free(buf1); free(my_line);
	free(iline);
	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);
}

void PikBest(){
int nfound,i,ipik,iaxis;
Pik cur_piks[NPIK];
float *buf1,*buf2,*myt;
float my_max;
int *iline;
int base,block[6],k,iold;
int iind,j,nind,iter,extend;
View myv;
PikList pik;
Buffer myd;

	
  iold=current_view_num();
  if(iold==-1) return;
  myv=current_view();
  pik=myv->data->pik;


  myd = DataBuffer(myv->data);

/*Find all of the piks with the current symbol */
nfound=0;
	for(i=0; i< pik->npik; i++){
		if(pik->pik[i].loc[0]!=NO_INDEX &&
     pik->pik[i].symbol==pik->cur_symbol){ /* valid pik */
			for(iaxis=1; iaxis< DATA_NAXIS; iaxis++)
				cur_piks[nfound].loc[iaxis]=pik->pik[i].loc[iaxis];
			nfound++;
		}
	}
	if(nfound==0) return;

block[1]=1;
for(i=2; i <=5 ; i++)
	block[i]=block[i-1]*AxisSize(myv->data->axis[i-1]);

extend=pik->nwind_cor+pik->search_radius;
NEW(float*,buf1,nfound*(2*extend+1));
#define BUF1C(i1,i2) buf1[i1+(i2)*(2*extend+1)]

/* loop through points with the same symbol */
for(ipik=0; ipik < nfound; ipik++){
	/*find the sample location of the pik */
	base=0;
	for(i=1;i <= 5; i++) base+=cur_piks[ipik].loc[i]*block[i];

	/* loop over independant axis */
	for(iind=-extend,j=0;iind<=extend;iind++,j++){
		BUF1C(j,ipik)=myd[base+iind*block[pik->depend]];
	}
}

nind=2*pik->search_radius+1;
NEW(float*,buf2,nfound*nind);
NEW(float*,myt,2*pik->nwind_cor+1);
NEW(int*,iline,nfound);
#define BUF2C(i1,i2) buf2[i1+(i2)*nind]

/*initialize shift to 0 */
for(i=0; i< nfound; i++) iline[i]=0;

/*we will try to improve about 6 times*/
for(iter=0; iter<1; iter++){

	/*first create super trace */
	for(i=0;i<2*pik->nwind_cor+1;i++){
		myt[i]=0; 
    for(ipik=0; ipik<nfound;ipik++)
      myt[i]+=BUF1C(i+iline[ipik]+pik->search_radius,ipik);
	}
	for(ipik=0;ipik<nfound; ipik++)

	/*next correlate all the points with the super trace*/
	for(ipik=0;ipik<nfound; ipik++){
		for(i=0,j=-pik->search_radius; i < nind; i++, j++){
      	BUF2C(i,ipik)=wshspickrc(myt,
         &BUF1C(0,ipik),pik->nwind_cor,extend+j+iline[ipik],
          2*extend+nind, pik->nwind_cor);

		}
	}
			PikDraw(NO_INDEX,ERASE);

	/*find the maximum shift for this point */
	for(ipik=0; ipik < nfound; ipik++){
		my_max=-999;
		k=iline[ipik];
		for(i=0, j=-pik->search_radius; i < nind; i++, j++){
			if(BUF2C(i,ipik) > my_max) { 
        my_max=BUF2C(i,ipik); iline[ipik]=j;
			}
		}
		iline[ipik]=MAX(-pik->search_radius,MIN(pik->search_radius,k+iline[ipik]));
	}
}

/*time to correct the picks */
nfound=0;
			PikDraw(NO_INDEX,ERASE);
	for(i=0; i< pik->npik; i++){
		if(pik->pik[i].loc[0]!=NO_INDEX &&
     pik->pik[i].symbol==pik->cur_symbol){ /* valid pik */
			  pik->pik[i].loc[pik->depend]+=iline[nfound];
		  nfound++;
		}
	}
			PikDraw(NO_INDEX,DRAW);
free(buf2); free(buf1); free(myt);free(iline);
	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);


}

void PikGrow3D(void){
View myv;
PikList pik;
Buffer myd;
Pik seeds[NPIK];
int nseed;
int j,idep[3],iax,dep,i,iind,ipos[2],iother[2],n1,n2,i1,i2,k;
int iaxis,*grown;
int block[6],shift_dep[2],ishift,shift_ind,min[2],max[2],iv_ind,iv_dep[2];
int iold;


  iold=current_view_num();
  if(iold==-1) return;
  myv=current_view();
  pik=myv->data->pik;


/*figure out references into the dataset*/
j=0;
for(i=1; i <= 3 ;i++){
  iax=AxisDir(MapAxis(myv->map[i]));
  if(iax==pik->depend){
		 iind=pik->depend;
		iv_ind=i;
	}
  else{
		iv_dep[j]=i;
    idep[j]=iax;
    j++;
  }
}

if(j!=2) return;
min[0]=MapFirst(myv->map[iv_dep[0]]);
max[0]=MapLast(myv->map[iv_dep[0]]);
min[1]=MapFirst(myv->map[iv_dep[1]]);
max[1]=MapLast(myv->map[iv_dep[1]]);

if(min[0]>max[0]) { j=min[0]; min[0]=max[0]; max[0]=j;}
if(min[1]>max[1]) { j=min[1]; min[1]=max[1]; max[1]=j;}



for(i=4; i <= 5 ;i++){
  iother[i-4]=AxisDir(MapAxis(myv->map[i]));
	ipos[i-4]=MapFrame(myv->map[i]);
}



/*figure out shifts*/
block[1]=1;
for(i=2; i <= 5; i++){ block[i]=block[i-1]*AxisSize(myv->data->axis[i-1]);}
shift_ind=block[iind];
shift_dep[0]=block[idep[0]];
shift_dep[1]=block[idep[1]];
ishift=0;
for(i=1; i <= 5; i++){
  if(i!=iind && i!= idep[0] && i!=idep[1]) { /*non-viewable*/
    ishift+=block[i]*myv->map[i]->frame;
  }
}


	

/*Find all of the piks with the current symbol in the current viewing pos*/
nseed=0;
	for(i=0; i< pik->npik; i++){
		if(pik->pik[i].loc[0]!=NO_INDEX &&
     pik->pik[i].symbol==pik->cur_symbol &&
       pik->pik[i].loc[iother[0]]==ipos[0] &&
       pik->pik[i].loc[iother[1]]==ipos[1] &&
			 pik->pik[i].loc[idep[0]] >= min[0] &&
			 pik->pik[i].loc[idep[0]] <= max[0] &&
			 pik->pik[i].loc[idep[1]] >= min[1] &&
			 pik->pik[i].loc[idep[1]] <= max[1] ){ /* valid pik */
			for(iaxis=1; iaxis< DATA_NAXIS; iaxis++)
				seeds[nseed].loc[iaxis]=pik->pik[i].loc[iaxis];
			nseed++;
		}
	}
	if(nseed==0) return;



n1=AxisSize(myv->data->axis[idep[0]]);
n2=AxisSize(myv->data->axis[idep[1]]);
NEW(int*,grown,n1*n2);
grow_region(pik->min_tol, pik->max_tol,pik->dtol,nseed,seeds,grown,min,max);
myd = DataBuffer(myv->data); /*data pointer */


/*now write out new piks */
i=0;
pik->last_op = PIKOP_LIST;
pik->last_item = pik->npik;
for(i2=min[1] ; i2 < max[1]; i2++){
	for(i1=min[0] ; i1 < max[0]; i1++){
    k=pik->npik/pik->j_display; k=pik->npik-k*pik->j_display;
    if(k==0) pik->pik[pik->npik].my_display=1;
    else  pik->pik[pik->npik].my_display=0;
		i=i1+i2*n1;
		if(grown[i]>-1){
      pik->pik[pik->npik].symbol=pik->cur_symbol;
			pik->pik[pik->npik].loc[iind]=grown[i];
			pik->pik[pik->npik].loc[idep[0]]=i1;
			pik->pik[pik->npik].loc[idep[1]]=i2;
			pik->pik[pik->npik].loc[iother[0]]=ipos[0];
			pik->pik[pik->npik].loc[iother[1]]=ipos[1];
		  pik->pik[pik->npik].loc[0]=myd[ishift+shift_ind*grown[i]+shift_dep[0]*idep[0]+shift_dep[1]*iother[1]];
	    if(pik->npik+1==pik->nmax) {realloc_pik(pik);}
    	pik->npik++;
		}
	}
}
PikDraw (NO_INDEX,DRAW);
free(grown);
	for(i=0;i< num_views(); i++){
		SetActiveView(i);
	  myv=current_view();
	  myv->data->pik->change_pik_func();
	}
	SetActiveView(iold);
}

void realloc_pik(PikList pik){
pik->nmax=pik->nmax*1.5;
pik->pik=(Pik*) realloc(pik->pik,sizeof(Pik)*pik->nmax);
}
void find_piks(PikList cur_list, int *min, int *max){
int i,ipik,good,ind;
	e_pik->npik = 0;
	for(ipik=0; ipik < cur_list->npik; ipik++){
		good=0;
		for(i=1; i< DATA_NAXIS;i++) {
			ind=cur_list->pik[ipik].loc[i];
			if(ind < min[i] || ind > max[i]) good=1;
			e_pik->pik[e_pik->npik].loc[i]=ind;
		}
		if(good==0 && cur_list->cur_symbol== cur_list->pik[ipik].symbol ){
			e_pik->pik[e_pik->npik].loc[0]=cur_list->pik[ipik].loc[0];
			e_pik->npik++;
			if(e_pik->npik+1==e_pik->nmax) realloc_pik(e_pik);	
		}
	}
	
}

int  ordered_e_piks(PikList pik,Plane plane,int *x_draw, int *y_draw){
int ne,i,min[6],max[6],other_dir[3];
View myv;
int idep,iind,j;

	  myv=current_view();
	  ne=0;
		iind=-1;
		for(i=1; i < DATA_NAXIS; i++){
			if( AxisDir(MapAxis(PlaneVmap(plane))) ==i || 
        AxisDir(MapAxis(PlaneHmap(plane))) == i){
           if(pik->depend==i) iind=i;
            else idep=i;
			}
			else{ 
           other_dir[ne]=i; 
ne++;
      }
		}
		if(iind==-1) return(0);
		for(i=1; i < DATA_NAXIS; i++){
			if(other_dir[0]==AxisDir(MapAxis(myv->map[i]))) 
				max[other_dir[0]]=min[other_dir[0]]=MapFrame(myv->map[i]);
			else if(other_dir[1]==AxisDir(MapAxis(myv->map[i]))) 
				max[other_dir[1]]=min[other_dir[1]]=MapFrame(myv->map[i]);
			else if(other_dir[2]==AxisDir(MapAxis(myv->map[i]))) 
				max[other_dir[2]]=min[other_dir[2]]=MapFrame(myv->map[i]);
			else if(i==iind){
        min[iind]=MapFirst(myv->map[i]);
        max[iind]=MapLast(myv->map[i]);
			}
			else{
        min[idep]=MapFirst(myv->map[i]);
        max[idep]=MapLast(myv->map[i]);
			}
		}
		for(i=1; i< DATA_NAXIS;i++){
			if(max[i] < min[i]) { j=max[i]; max[i]=min[i]; min[i]=j;}
		}
		find_piks(pik,min,max);	
		if(e_pik->npik<2) return(0);
    e_pik->npik=MIN(e_pik->npik,MAX_PTS_LINE);
		quicksort(idep,0,e_pik->npik-1);
		ne=0;
		for(i=0;i < e_pik->npik;i++){ /* loop through the piks */
	   if(1==x_y_from_pik_plane(e_pik->pik[i],plane,&x_draw[ne],&y_draw[ne])){
				 ne++;
			}
		}
		if(e_pik->npik<2) return(0);
    return(e_pik->npik);

}

/*Adapted from http://ironbark.bendigo.latrobe.edu.au/courses/bcomp/c103/sem296/lectures/Lecture2.html*/

void quicksort(int index, int first, int last) {
    int pivindx; /* index of the element separating the two sub-arrays*/
    if (last > first) {  /* More than one element to be sorted?*/
        pivindx = partition(first, last,index);
        quicksort(index,first, pivindx - 1);
        quicksort(index,pivindx + 1, last);
    }
}

void copy_pik(Pik *new, Pik old)
{
int iaxis;
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) new->loc[iaxis] =old.loc[iaxis];
  new->symbol=old.symbol;
	new->my_display=old.my_display;
}

int partition(int first, int last, int iaxis) {
    int pivindx, top, i; 
     Pik pivot;

    /* Choose a pivot: select a random index between first and last.*/
    i = rand() % (last - first + 1) + first;

    /* Put the pivot first, remember pivot, initialise ready for loop.*/
		copy_pik(&pivot,e_pik->pik[i]);/* remember the pivot */
		copy_pik(&(e_pik->pik[i]),e_pik->pik[first]);
		copy_pik(&(e_pik->pik[first]),pivot);  /* pivot now first */
    pivindx = first;
    top = last;                       

    while (top > pivindx) {         /* Still unknown elements */
        /* top indicates the highest unknown element */
        if (e_pik->pik[top].loc[iaxis] >= pivot.loc[iaxis]) {
            top--;              /* where it belongs, count as >=*/
        } else {
					  copy_pik(&(e_pik->pik[pivindx]),e_pik->pik[top]);/*shift down*/
					  copy_pik(&(e_pik->pik[top]),e_pik->pik[pivindx+1]);/*shift displaced element up*/
					  copy_pik(&(e_pik->pik[pivindx+1]),pivot);/*Put pivot back*/
            pivindx++;            /* Alter record of pivot location*/
        }
    }
    return pivindx;
}





#endif
