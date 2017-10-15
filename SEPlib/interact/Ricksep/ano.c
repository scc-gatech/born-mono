#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
pick list subroutines
*/

#include <stdio.h>
#include "rick.h"

#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif
#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif

Anoviews anoviews;
Vplot2sep vpconv;
int min_x, max_x,min_y, max_y;

	char label[1024];


/* main pik object */

/* initialize pik object; read pick= file */
void AnoInit ()
	{
	int i;
	NEW(Anoviews,anoviews,1);
	for(i=0; i < NUM_VIEWS; i++)
		anoviews->ano[i]=InitAnolist();


}
Anolist InitAnolist(){
	int i;
	Anolist myano;

	NEW(Anolist,myano,1);
	
	myano->do_anno=0;
	myano->cur_anno=0;
	myano->n_ano=0;
	for(i=0; i < MAX_ANO_PTS;i++){
		myano->type[i]=ANO_UNDEFINE;
		myano->npts[i]=0;
		myano->rotate[i]=0;
		myano->arrow[i]=0;
		myano->size[i]=8;
		myano->fat[i]=0;
	}

	myano->last_op=ANO_FINISHED;
	return(myano);

	}


/* read pick file */
/*
void PikRead ()
	{
	int iaxis, ipik;
	FILE *fd;
	float value[DATA_NAXIS];
	char line[100];
	Message message;
	View myv;

	myv=current_view();

	if (!pik) return;
	if ((fd=fopen(pik->file,"r")) == NULL){
		sprintf (message, "cant open pick=%s file for reading", pik->file);
		UIMessage (message);
		return;
		}
	
	for (pik->npik=0; pik->npik<pik->nmax && fgets(line,sizeof(line),fd) != NULL; pik->npik++) {
		sscanf (line,"%f %f %f %f %f",value+1,value+2,value+3,value+4,value+5,value);
		pik->pik[pik->npik][0]=NO_INDEX+1;
		for (iaxis=1; iaxis<DATA_NAXIS; iaxis++) {
			pik->pik[pik->npik][iaxis] = AxisIndex(DataAxis(myv->data,iaxis),value[iaxis]);
			}
		}
	for (ipik=pik->npik; ipik<pik->nmax; ipik++) pik->pik[ipik][0] = NO_INDEX;
	for (ipik=0; ipik<pik->npik; ipik++) 
	fclose(fd);
	sprintf (message, "%d picks read from file %s", pik->npik,pik->file);
	UIMessage (message);
	}
*/

/* draw pick pik0 or all picks (pik0 = NO_INDEX) on all planes */
void AnoDraw (int pik0,int draw)
	{
	int iano, range1, range2, pik1, pik2, dir, frame, x, y, hskew, vskew, size;
	int i;
	int x0[MAX_ANO_PTS],y0[MAX_ANO_PTS];
	Plane plane;
	extern Plane planelist;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

  min_x=9999; min_y=9999; max_x=0; max_y=0;
	if(myano->n_ano<1) return;

	for (plane=planelist; plane < planelist+NPLANE && 
		plane->attr->orient != NO_INDEX; plane++) {
    min_x=MIN(PlaneH0(plane),min_x);
    min_y=MIN(PlaneV0(plane),min_y);
    max_x=MAX(PlaneH0(plane)+PlaneNH(plane),max_x);
    max_y=MAX(PlaneV0(plane)+PlaneNV(plane),max_y);
  }

  vpconv.ncom=0;
	for (plane=planelist; plane < planelist+NPLANE && 
		plane->attr->orient != NO_INDEX; plane++) {
		dir = PlaneDir (plane);
		frame = PlaneFrame (plane);
		range1 = frame ;
		range2 = frame;
		range1 = range1 > 0 ? range1 : 0;
		range2 = range2 < AxisSize (DataAxis(myv->data,dir))-1 ? range2 : AxisSize (DataAxis(myv->data,dir))-1;

		for (iano=0; iano<=myano->n_ano; iano++){
			if (myano->my_loc[iano][0][dir] >= range1 && 
			myano->my_loc[iano][0][dir] <= range2){
				hskew = 0;
				vskew = 0;
				if (PlaneType(plane) == RENDER_HORZ) hskew = PlaneSkew(plane);
				else vskew = PlaneSkew(plane);
				for(i=0;i<myano->npts[iano];i++){
					x = MapInverse(PlaneHmap(plane),myano->my_loc[iano][i][AxisDir(MapAxis(PlaneHmap(plane)))]);
					if (PlaneType(plane) == RENDER_VERT) {
						x = MapSize(PlaneHmap(plane)) - x;
						}
					y = MapInverse(PlaneVmap(plane),myano->my_loc[iano][i][AxisDir(MapAxis(PlaneVmap(plane)))]);
					x0[i] = x + PlaneH0(plane) + hskew * (MapSize(PlaneVmap(plane)) - y);
					y0[i] = y + PlaneV0(plane) + vskew * (PlaneNH(plane) - x);
				}
			}
					if (myano->my_loc[iano][0][dir] == frame){
					SetFat(myano->fat[iano]);
          vpconv.fat=myano->fat[iano];
						switch(myano->type[iano]){
							case ANO_TEXT:
								AnoText(x0,y0,myano->text[iano],myano->rotate[iano],myano->size[iano],draw);	
								break;
							case ANO_OVAL:
								AnoOval(x0[0],y0[0],x0[1],y0[1],draw);
								break;
							case ANO_POLY:
								AnoPolyline(x0,y0,myano->npts[iano],myano->arrow[iano],draw);
								break;
							case ANO_CURVE:
								break;
							case ANO_BOX:
								AnoBox(x0,y0,draw);
								break;
							default:
								break;
						}
					SetFat(0);
					}
				}
			}
   if(using_overlays()==0 && draw==ERASE) quick_redo();
	}


void AnoOval (int x1,int y1,int x2,int y2,int draw)
	{
	int width,height,x,y,ws,hs;
  float xf1,xf2,yf1,yf2;
  float fwidth,fheight;
 	width=x2-x1; height=y2-y1;
	x=MIN(x1,x2); y=MIN(y1,y2);
  if(width < 0) width=-width; if(height<0) height=-height;
  if(draw!=SEP_COMMANDS) DrawOval (x1-width/2,y1-height/2,width,height,draw);
  else{
		Cnvrt2sep(x1,y1);xf1=vpconv.x_sep;yf1=vpconv.y_sep;
		Cnvrt2sep(x2,y2);xf2=vpconv.x_sep;yf2=vpconv.y_sep;
    fwidth=xf2-xf1; if(fwidth < 0) fwidth=-fwidth; 
    fheight=yf2-yf1; if(fheight < 0) fheight=-fheight; 
    sprintf(vpconv.commands[vpconv.ncom],"Overlay type=oval x=%f y=%f x_oval=%f y_oval=%f fat=%d ",xf1,yf1,fwidth/2.,fheight/2.,vpconv.fat*2);
    vpconv.ncom+=1;
  }
}


void AnoBox (int *x0,int *y0,int draw)
	{
float x1,x2,y1,y2,width,height;
  if(draw!=SEP_COMMANDS){
	  DrawLine (x0[0],y0[0],x0[1],y0[0],draw);
	  DrawLine (x0[1],y0[0],x0[1],y0[1],draw);
	  DrawLine (x0[1],y0[1],x0[0],y0[1],draw);
	  DrawLine (x0[0],y0[1],x0[0],y0[0],draw);
	}
	else{
		Cnvrt2sep(x0[0],y0[0]);x1=vpconv.x_sep;y1=vpconv.y_sep;
		Cnvrt2sep(x0[1],y0[1]);x2=vpconv.x_sep;y2=vpconv.y_sep;
    width=x2-x1; if(width < 0){ width=-width; x1=x2;}
    height=y2-y1; if(height < 0){ height=-height; y1=y2;}
    sprintf(vpconv.commands[vpconv.ncom],"Overlay type=box x=%f y=%f width=%f height=%f fat=%d ",x1,y1,width,height,vpconv.fat*2);
    vpconv.ncom+=1;
	}
}

void Cnvrt2sep(int x,int y)
{
float x_pct,y_pct; 
float x0,y0;

x0=x;
y0=y;

x_pct=(x0-min_x)/(max_x-min_x);
y_pct=1.-(y0-min_y)/(float)(max_y-min_y);
vpconv.x_sep=vpconv.x_ll+x_pct*(vpconv.x_ur-vpconv.x_ll);
vpconv.y_sep=vpconv.y_ll+y_pct*(vpconv.y_ur-vpconv.y_ll);

}
void AnoPolyline(int *x0,int *y0,int npts,int arrow,int draw)
{
int i,ntot;
float x1,y1;
char xlab[2048],ylab[2048],aa[40];
if(npts==1) return;


if(draw!=SEP_COMMANDS){
	if(arrow==0) ntot=npts-1;
	else ntot=npts-2;
	for(i=0; i < ntot; i++)
		DrawLine (x0[i],y0[i],x0[i+1],y0[i+1],draw);


	if(arrow==1)
		DrawArrow (x0[ntot+1],y0[ntot+1],x0[ntot],y0[ntot],10,draw);
}
else{
  sprintf(vpconv.commands[vpconv.ncom],"Overlay type=box fat=%d ",vpconv.fat*2);

 	strcpy(xlab,"x="); 
 	strcpy(ylab,"y="); 
  for(i=0;i< npts; i++){
	 Cnvrt2sep(x0[i],y0[i]);x1=vpconv.x_sep;y1=vpconv.y_sep;
   if(i==npts-1) sprintf(aa,"%f",x1);
   else sprintf(aa,"%f,",x1); 
   strcat(xlab,aa);
   if(i==npts-1) sprintf(aa,"%f",y1);
   else sprintf(aa,"%f,",y1); 
   strcat(ylab,aa);
  }
  if(arrow==1) strcpy(aa,"arrow");
  else strcpy(aa,"polyline");
  sprintf(vpconv.commands[vpconv.ncom],"Overlay type=%s %s %s fat=%d ",aa,xlab,ylab,vpconv.fat*2);
   vpconv.ncom+=1;
}




}
void AnoText(int *x0,int *y0,char *mys,int rotate,int size,int draw)
{
float x1,y1;
 if(draw!=SEP_COMMANDS) DrawString(x0[0],y0[0],rotate,mys,size,draw);
  else{
	 Cnvrt2sep(x0[0],y0[0]);x1=vpconv.x_sep;y1=vpconv.y_sep;
  sprintf(vpconv.commands[vpconv.ncom],"Overlay type=text boxit=0 x=%f y=%f angle=%d  size=%f fat=%d label=\"%s\" ",x1,y1,rotate,(float)(size)/8.*.25,vpconv.fat*2,mys);
   vpconv.ncom+=1;
 }


}

void AnoModify(int x,int y)
{
int icur;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

icur=AnoNear(x,y);
if(icur==NO_INDEX) return;
else{ /* we have a valid point nearby */
	
if(myano->last_op!=ANO_FINISHED) {
   /*we were in the middle of a polyobject */
	DeletePolyAno();
}
myano->cur_anno=icur;
myano->my_fat=myano->fat[icur];
if(myano->type[icur]==ANO_TEXT) UIAnoTextRaise();
else UIFinishPanelRaise();


}
}

int AnoNear (int x,int y)
	{
	PickPoint_ pick;
	View myv;
	Anolist myano;
	int i, ipik, near, inear, range1, range2, distance, frame, dir, dir1, dir2, x1, y1;
	Plane plane;

	myv=current_view();
	myano=myv->data->ano;

	if (myano->n_ano<1) return (NO_INDEX);
	PickDecode (x,y,&pick,1);
	plane = PlaneFind (pick.iaxis[AXIS_DEEP],pick.index[pick.iaxis[AXIS_DEEP]],-1,-1,-1,-1,0);
	dir = PlaneDir (plane);
	frame = PlaneFrame (plane);
	range1 = frame;
	range2 = frame;
	range1 = range1 > 0 ? range1 : 0;
	range2 = range2 < AxisSize (DataAxis(myv->data,dir))-1 ? range2 : AxisSize (DataAxis(myv->data,dir))-1;
	near = 100000000;
	inear = NO_INDEX;
	dir1 = pick.iaxis[1];
	dir2 = pick.iaxis[2];
	for (ipik=0; ipik<myano->n_ano; ipik++) {
		if (myano->my_loc[ipik][0][dir] >= range1 && myano->my_loc[ipik][0][dir] <= range2) {
			distance = (myano->my_loc[ipik][0][dir1] - pick.index[dir1]) * (myano->my_loc[ipik][0][dir1] - pick.index[dir1])
			+ (myano->my_loc[ipik][0][dir2] - pick.index[dir2]) * (myano->my_loc[ipik][0][dir2] - pick.index[dir2]);
			if (distance < near) {
				near = distance;
				inear = ipik;
				}
			}
		}
	if (!AnoCoord(inear,plane,&x1,&y1)) return (NO_INDEX);
	if ((x-x1)*(x-x1)+(y-y1)*(y-y1)>PIK_NEAR) return (NO_INDEX);
	return (inear);
	}


int AnoCoord (int ipik,Plane plane,int *x,int *y)
	{
	int hskew=0, vskew=0, x1, y1;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

	*x = NO_INDEX;
	*y = NO_INDEX;
	if (PlaneType(plane) == RENDER_HORZ) hskew = PlaneSkew(plane);
	else vskew = PlaneSkew(plane);
	x1 = MapInverse(PlaneHmap(plane),myano->my_loc[ipik][0][AxisDir(MapAxis(PlaneHmap(plane)))]);
	if (PlaneType(plane) == RENDER_VERT) {
		x1 = MapSize(PlaneHmap(plane)) - x1;
		}
	y1 = MapInverse(PlaneVmap(plane),myano->my_loc[ipik][0][AxisDir(MapAxis(PlaneVmap(plane)))]);
	if (x1 == NO_INDEX || y1 == NO_INDEX) return (0);
	*x = x1 + PlaneH0(plane) + hskew * (MapSize(PlaneVmap(plane)) - y1);
	*y = y1 + PlaneV0(plane) + vskew * (PlaneNH(plane) - x1);
	return (1);
	}


/* add a pick at mouse pick location */
void AnoAdd (int x,int y)
	{
	int iaxis,icur,ipt,iano;
	PickPoint_ pick;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

	PickDecode (x,y,&pick,1);
	if(myano->last_op==ANO_FINISHED){ /*new object */
		iano=myano->n_ano;
		myano->type[iano]=ANO_POLY;
		myano->n_ano++;
		myano->last_op=ANO_ADD_PT;
	}
	else{
		iano=myano->n_ano-1;
	}
	myano->cur_anno=iano;
	ipt=myano->npts[iano];
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) 
    myano->my_loc[iano][ipt][iaxis] = pick.index[iaxis];
	ipt=myano->npts[iano]++;
	
	AnoDraw (0,DRAW);
	}
void AddAnoOval(int x1,int y1,int x2,int y2)
{
	PickPoint_ pick;
	int icur,iaxis;
	int x,y,width,height;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

	if(myano->last_op!=ANO_FINISHED) {
    /*we were in the middle of a polyobject */
		DeletePolyAno();
	}
	myano->cur_anno=myano->n_ano;
	icur=myano->cur_anno;
	myano->n_ano++;
	myano->last_op=ANO_FINISHED;
	myano->type[icur]=ANO_OVAL;
	myano->npts[icur]=2;
	PickDecode (x1,y1,&pick,1);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
    myano->my_loc[icur][0][iaxis] = pick.index[iaxis];
	}
	PickDecode (x2,y2,&pick,1);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
    myano->my_loc[icur][1][iaxis] = pick.index[iaxis];
	}
}


void AddAnoRect(int x1,int y1,int x2,int y2)
{
	PickPoint_ pick;
	int icur,iaxis;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

	if(myano->last_op!=ANO_FINISHED) {
    /*we were in the middle of a polyobject */
		DeletePolyAno();
	}
	myano->cur_anno=myano->n_ano;
	icur=myano->cur_anno;
	myano->n_ano++;
	myano->last_op=ANO_FINISHED;
	myano->type[icur]=ANO_BOX;
	myano->npts[icur]=2;
	PickDecode (x1,y1,&pick,1);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
    myano->my_loc[icur][0][iaxis] = pick.index[iaxis];
	}
	PickDecode (x2,y2,&pick,1);
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
    myano->my_loc[icur][1][iaxis] = pick.index[iaxis];
	}
}



void AddAnoTextPt(int x,int y)
{
	int iaxis,icur,ipt,iano;
	PickPoint_ pick;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
	PickDecode (x,y,&pick,1);
	if(myano->last_op!=ANO_FINISHED) {
    /*we were in the middle of a polyobject */
		DeletePolyAno();
	}
	myano->cur_anno=myano->n_ano;
	icur=myano->cur_anno;
	myano->n_ano++;
	myano->last_op=ANO_FINISHED;
	myano->type[icur]=ANO_TEXT;
	for (iaxis=0; iaxis<DATA_NAXIS; iaxis++) {
    myano->my_loc[icur][0][iaxis] = pick.index[iaxis];
	}
	myano->npts[icur]=1;
	myano->size[icur]=8;
	strcpy(myano->text[icur]," ");
}


void DeletePolyAno()
{
int i,j,a;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

	AnoDraw (0,ERASE);


if(myano->cur_anno!=myano->n_ano -1){ 
  /*shift all annotate objects one down */
	for(j=myano->cur_anno;j<myano->n_ano-1;j++){ 
		for(i=0; i < myano->npts[j+1]; i++){ 
			for(a=0; a < DATA_NAXIS; a++){
				myano->my_loc[j][i][a]=myano->my_loc[j+1][i][a];
			}
		}
		myano->type[j]=myano->type[j+1];
		myano->npts[j]=myano->npts[j+1];
		myano->rotate[j]=myano->rotate[j+1];
		myano->arrow[j]=myano->arrow[j+1];
		myano->size[j]=myano->size[j+1];
		myano->fat[j]=myano->fat[j+1];
		strcpy(myano->text[j],myano->text[j+1]);
	}
}
	myano->n_ano-=1;
	myano->npts[myano->n_ano]=0;
	myano->type[myano->n_ano]=ANO_UNDEFINE;
	myano->last_op=ANO_FINISHED;
	AnoDraw(0,DRAW);

}
void FinishPolyAno()
{
int i;
int equal;
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

		equal=1;
		for(i=0;i<DATA_NAXIS;i++){ 
			if(myano->my_loc[myano->cur_anno][0][i]!=myano->my_loc[myano->cur_anno][myano->npts[myano->cur_anno]][i]) equal=0;
		
		}
		if(myano->npts[myano->cur_anno]>2){ /* open and close poly require at least 3 pts*/
			if(myano->do_close_poly==1 && equal==0){ /* close the polygon */
				for(i=0; i < DATA_NAXIS;i++)
					myano->my_loc[myano->cur_anno][myano->npts[myano->cur_anno]][i]=
					myano->my_loc[myano->cur_anno][0][i];
				myano->npts[myano->cur_anno]+=1;
			}
			else if(myano->do_close_poly==0 && equal==1) myano->npts[myano->cur_anno]-=1;
		}
	myano->last_op=ANO_FINISHED;
	myano->fat[myano->cur_anno]=myano->my_fat;
	AnoDraw(0,DRAW);
}

int grab_ano_fat(){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
return( myano->my_fat);
}

int grab_ano_rotate(){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
return( myano->rotate[myano->cur_anno]);

}
void set_ano_rotate(int irot){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
 myano->rotate[myano->cur_anno]=irot;

}

int grb_do_ano(){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
return(myano->do_anno);
}

int grab_do_ano(){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
return(myano->do_anno);

}
int grab_n_ano(){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
return(myano->n_ano);

}
int grab_do_close(){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

return(myano->do_close_poly);
}

int grab_last_op(){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
return( myano->last_op);
}

void set_do_ano(int iano){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
  myano->do_anno=iano;	
}
void set_close_status(int close, int arrow){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;

 myano->do_close_poly=close;
 myano->arrow[myano->cur_anno]=arrow;

}

void set_ano_fat(int ifat){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
 myano->my_fat=ifat;
}

int grab_ano_size(int isize){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
 return(myano->size[myano->cur_anno]);
}

void set_ano_size(int isize){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
 myano->size[myano->cur_anno]=isize;
}

void set_ano_text(char *text){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
strcpy(myano->text[myano->cur_anno],text);
}

char *grab_ano_text(){
	View myv;
	Anolist myano;

	myv=current_view();
	myano=myv->data->ano;
	strcpy(label,myano->text[myano->cur_anno]);
	return(label);
}

void set_vplot_size(float xll, float xur, float yll, float yur){

vpconv.x_ll=xll;
vpconv.x_ur=xur;
vpconv.y_ll=yll;
vpconv.y_ur=yur;
vpconv.x_ll=xll;
}

int grab_vplot_ncom(){
return(vpconv.ncom);
}

char *grab_vplot_com(int icom){

return(vpconv.commands[icom]);

}

#endif
