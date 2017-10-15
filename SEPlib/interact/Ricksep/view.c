#include <sepConfig.h>
#include <math.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
view object code
*/
#include <stdio.h>
#include <string.h>
#include "rick.h"

/* set rendering attributes for a view */
RenderAttr _attr_;
#define RENDER_ATTR(_transp,_skew,_mark,_zbuffer,_image,_axes,_shadow,_lines,_box,_pick) \
	_attr_->image = (_image);\
	_attr_->shadow = (_shadow);\
	_attr_->zbuffer = (_zbuffer);\
	_attr_->box = (_box);\
	_attr_->axes = (_axes);\
	_attr_->lines = (_lines);\
	_attr_->pick = (_pick);\
	_attr_->skew = (_skew);\
	_attr_->transp = (_transp);\
	_attr_->mark = (_mark);\
/*	fprintf (stderr,"image=%d shadow=%d zbuffer=%d box=%d axes=%d lines=%d pick=%d skew=%d transp=%d mark=%d\n",_image,_shadow,_zbuffer,_box,_axes,_lines,_pick,_skew,_transp,_mark)*/
Message message;

/** internal history list functions **/
void ViewHistoryDebugPrint();
void ViewHistorySaveAll();
void ViewHistorySave(View view, ViewSaver* create);
ViewSaver* ViewHistoryCreateCurrent(View view);
void ViewHistoryLoadCurrent(View view);
int ViewHistoryDuplicate(ViewSaver* one, ViewSaver* two);
int ViewHistoryDuplicateArray(ViewSaver* ones[], ViewSaver* twos[]);
int ViewHistoryDelete(ViewSaver* start);
void ViewHistoryRenumber(View view);
void ViewHistoryMenus();

/** info display
void ViewSupplementInfo(string, string, string, string);
float ViewValueAtCursor(View view);
char* ViewValueScript(View view);

/* initialize view object */
string viewnamelist[] = VIEW_NAMELIST;

Views views;

void ViewsInit(){
int i;
extern Datalist datalist;
int num_view,cur_view;
string temp_ch;
View ff;
float tempr[NUM_VIEWS],tot;
int tempi,j;
int tempa[2];
int ndata;
float sumx,sumy;
char tag[1024]; 

NEW(Views,views,1);

ff=0;

ndata=ReturnNdata();
for(i=0; i < NUM_VIEWS; i++)
	views->view[0]=0; /*start by initializing to zero  */
	
num_view=1; i=rick_getch("nview","d",&num_view); 

views->num_view=num_view;
if(num_view < 1 || num_view > NUM_VIEWS)
	err("unacceptable, number of views must be 1 < nviews < %d:%d \n",NUM_VIEWS,num_view);

 views->roundRobinOn = 0;

for(i=0; i < num_view; i++){
 views->cur_view=i;
 sprintf(temp_ch,"view%d_data",i); 
 if(0==rick_getch(temp_ch,"s",tag)) strcpy(tag,"in");
 views->view[i]=ViewInit(tag,i);
 views->view[i]->first=0;
 views->view[i]->active=0;
}


views->view[0]->first=1;
views->cur_view=0;
views->view[0]->active=1;

if(2==rick_getch("nview_dim","d",tempa)){
	if(tempa[0] * tempa[1] != num_view) 
		err("product(nview_dim[0])!=nview");
}
else{
tempa[0]=num_view;
tempa[1]=1;
}
views->view_dim[0]=tempa[0];
views->view_dim[1]=tempa[1];

if(views->view_dim[0]==rick_getch("view_ratio_x","f",tempr)){
	tot=0.;for(i=0; i < views->view_dim[0];i++) tot+=tempr[i];
	   if(fabs(tot-1.) > 0.001)err("view_ratio_x does not sum to 1 (%f)\n",tot);
	for(j=0; j < views->view_dim[1]; j++){
		for(i=0; i < views->view_dim[0]; i++){
		 views->ratx[i+j*views->view_dim[0]]=tempr[i];
		}
	}
}
else{ for(j=0; j < num_view; j++){ views->ratx[j]=1./(float)views->view_dim[0];}}


if(views->view_dim[0]==rick_getch("view_ratio_y","f",tempr)){
	tot=0.;for(i=0; i < views->view_dim[1];i++) tot+=tempr[i];
  if(fabs(tot-1.) > 0.001)err("view_ratio_y does not sum to 1 (%f)\n",tot);
	for(j=0; j < views->view_dim[1]; j++){
		for(i=0; i < views->view_dim[0]; i++){
		 views->raty[i+j*views->view_dim[0]]=tempr[j];
		}
	}
}
else{ for(j=0; j < num_view; j++){ views->raty[j]=1./(float)views->view_dim[1];}}

tot=0.;
for(i=0; i < views->view_dim[0]; i++){
	for(j=0; j < views->view_dim[1]; j++){
		views->begx[i+j*views->view_dim[0]]=tot;
	}
	tot+=views->ratx[i];
}


tot=0.;
for(j=0; j < views->view_dim[1]; j++){
	for(i=0; i < views->view_dim[0]; i++){
		views->begy[i+j*views->view_dim[0]]=tot;
	}
	tot+=views->raty[j*views->view_dim[0]];
}

}

View ViewInit (char *tag, int iview)
	{
	View view;
	string option,temp_ch,add_to;
	int i,itot[5],j;
	int norder[5],found,ib,ie;
	Map_ A[5];
	Data data;

	if(iview==0) strcpy(add_to,"");
	else sprintf(add_to,"%d",iview+1);

	NEW(View,view,1);
	strcpy(view->tag,tag);
	data=DataFromTag(tag);
	view->data=data;
	view->margins[MARGIN_LEFT] = LEFT_MARGIN;
	view->margins[MARGIN_RIGHT] = RIGHT_MARGIN;
	view->margins[MARGIN_BASE] = BASE_MARGIN;
	view->margins[MARGIN_TOP] = TOP_MARGIN;
	view->style = VIEW_THREE;
	if (rick_auxgetch ("style","s",option,add_to)) {
		for (view->style=0; view->style<sizeof (viewnamelist) / sizeof(viewnamelist[0]); view->style++) {
			if (!strcmp (viewnamelist[view->style],option)) break;
			}
		}
	else rick_auxgetch ("style","d",&view->style,add_to);
	if(data_is_2d(view->data)==1) view->style=VIEW_CUBE; // DC06
	rick_auxgetch ("wide","d",&view->wide,add_to);
	rick_auxgetch ("hite","d",&view->hite,add_to);
	switch (view->style) {
	case VIEW_TRANSP:
	case VIEW_SIDE:
		view->movie = MOVIE_SIDE;
		break;
	case VIEW_TOP:
		view->movie = MOVIE_TOP;
		break;
	default:
		view->movie = MOVIE_FRONT;
		}
	if (rick_auxgetch ("movie","s",option,add_to)) {
		if (!strcmp (option,"in")) {
			view->movie = MOVIE_FRONT;
			MovieSetDir (MOVIE_REVERSE);
			MovieSetRun (1);
			}
		if (!strcmp (option,"out")) {
			view->movie = MOVIE_FRONT;
			MovieSetDir (MOVIE_FORWARD);
			MovieSetRun (1);
			}
		if (!strcmp (option,"left")) {
			view->movie = MOVIE_SIDE;
			MovieSetDir (MOVIE_REVERSE);
			MovieSetRun (1);
			}
		if (!strcmp (option,"right")) {
			view->movie = MOVIE_SIDE;
			MovieSetDir (MOVIE_FORWARD);
			MovieSetRun (1);
			}
		if (!strcmp (option,"up")) {
			view->movie = MOVIE_TOP;
			MovieSetDir (MOVIE_REVERSE);
			MovieSetRun (1);
			}
		if (!strcmp (option,"down")) {
			view->movie = MOVIE_TOP;
			MovieSetDir (MOVIE_REVERSE);
			MovieSetRun (1);
			}
		}
	if (view->style == VIEW_TRANSP) view->shape = SHAPE_PIXEL;
	else view->shape = SHAPE_FIT;
	if (rick_auxgetch ("shape","s",option,add_to)) {
		if (!strcmp (option,"fit")) view->shape = SHAPE_FIT;
		else if (!strcmp (option,"cube")) view->shape = SHAPE_FIT;
		else if (!strcmp (option,"0")) view->shape = SHAPE_FIT;
		else if (!strcmp (option,"true")) view->shape = SHAPE_TRUE;
		else if (!strcmp (option,"1")) view->shape = SHAPE_TRUE;
		else if (!strcmp (option,"pixel")) view->shape = SHAPE_PIXEL;
		else if (!strcmp (option,"2")) view->shape = SHAPE_PIXEL;
		}
	view->showpicks = DRAW;
	rick_auxgetch ("showpicks","d",&view->showpicks,add_to);
	view->nacross = 1;
	rick_auxgetch ("across","d",&view->nacross,add_to);
	view->ndown = 1;
	rick_auxgetch ("down","d",&view->ndown,add_to);
	view->dacross = 1;
	rick_auxgetch ("dacross","d",&view->dacross,add_to);
	view->rate = VIEW_RATE_TENTHS;
	rick_auxgetch ("rate","d",&view->rate,add_to);
	view->fence = DRAW_ALL;
	rick_auxgetch ("fence","d",&view->fence,add_to);
	view->map[AXIS_DOWN] = MapInit (DataAxis(data,DATA_AXIS1),"DOWN",AXIS_DOWN);
	view->map[AXIS_ACROSS] = MapInit (DataAxis(data,DATA_AXIS2),"ACROSS",AXIS_ACROSS);
	view->map[AXIS_DEEP] = MapInit (DataAxis(data,DATA_AXIS3),"DEEP",AXIS_DEEP);
	view->map[AXIS_4D] = MapInit (DataAxis(data,DATA_AXIS4),"4D",AXIS_4D);
	view->map[AXIS_5D] = MapInit (DataAxis(data,DATA_AXIS5),"5D",AXIS_5D);
       
	if (rick_auxgetch ("option","s",option,add_to)) {
		if (!strcmp (option,"side")) {
			MapSwap (view->map[AXIS_ACROSS],view->map[AXIS_DEEP]);
			}
		else if (!strcmp (option,"top")) {
			MapSwap (view->map[AXIS_DOWN],view->map[AXIS_DEEP]);
			}
		else if (!strcmp (option,"extra")) {
			MapSwap (view->map[AXIS_4D],view->map[AXIS_DEEP]);
			}
		}
	view->map[AXIS_COLOR] = MapInit (DataAxis(data,DATA_VALUE),"COLOR",AXIS_COLOR);
	MapFlip (view->map[AXIS_DEEP]);
	if (view->style == VIEW_FENCE) strcpy (option,"center");
	else strcpy (option,"option");


	rick_auxgetch ("origin","s",option,add_to);
	if (!strcmp (option,"center") || !strcmp (option,"middle")) {
		for (i=1; i<VIEW_NAXIS; i++) {
			MapSetFrame (view->map[i],AxisSize(MapAxis(view->map[i]))/2);
			}
		}

	
	i=rick_auxgetch("norder","d",norder,add_to);
	if(i==5){ 
		for(i=1; i< 5; i++){ 
      A[i-1]=*view->map[i];
			found=0;
			for(j=0; j<5;j++) if(norder[j]==i) found=1;
			if(found==0) err("must specify axes 1-5 with norder \n");
		}

	reorient(view,norder);
	}
  view->change_loc_func=dummy_loc_change;
  view->close_view_func=dummy_close_func;


	NEW (RenderAttr,_attr_,1);
	// view savers uninitialized to begin with 
	view->firstSaved = NULL;
	view->lastSaved = NULL;
	view->currentSaved = NULL;
	for (i=0; i<DATA_NAXIS; i++)
	  view->map0[i] = view->map[i];
	return (view);
	}

void reorient(View view, int *norder){
int i,j,ib;

/*loop over the axes putting in order */
for(i=1; i < 5; i++){
  ib=i;
	for(j=i; j <= 5; j++){
		if(AxisDir(MapAxis(view->map[j]))==norder[i-1]) ib=j;
	}
	if(i!=ib) MapSwap(view->map[i],view->map[ib]);
}

}


/* adjust view size: reallocate buffers,adjust map axes */
void ViewSize (Views views)
	{
	int wide,hite, size, deep;
	float fwide,fhite, scale;
  int shape_temp,tempi,full_wide,full_hite;
	int cube_wide, cube_hite,resize,i;
	View myv;
	int i1,i2;
	float x0,y0;
	
	if (!views) return;
	/* get new size */
	UICanvasSize (&full_wide,&full_hite);
	
	/*by default the width is dividing into equal areas */
	RenderSize (full_wide,full_hite);

i=-1;
for(i2=0; i2 < views->view_dim[1]; i2++){
for(i1=0; i1 < views->view_dim[0]; i1++){
i+=1;

 if (views->roundRobinOn && i == views->roundRobinView) {
   wide = full_wide;
   hite = full_hite;
   x0 = views->begx[0];
   y0 = views->begy[0];
 }
 else if (views->roundRobinOn) {
   wide = 1;
   hite = 1;
   x0 = 1;
   y0 = 1;
 }
 else {
   wide=full_wide*views->ratx[i];
   hite=full_hite*views->raty[i];
   x0 = views->begx[i];
   y0 = views->begy[i];
 }

	myv=views->view[i];
	shape_temp=myv->style;
	myv->wide = wide;
	myv->hite = hite;
	myv->x0=x0*full_wide;
	myv->y0=y0*full_hite;

	/* adjust axes lengths depending upon shape and style */
	switch (shape_temp) {
	/* the four cubical views */
	case VIEW_CUBE:
	case VIEW_FENCE:
	case VIEW_THREE:
	case VIEW_TRANSP:
		/* actual real estate */
		wide = (myv->wide - HMARGIN - GAP);
		hite = (myv->hite - VMARGIN - GAP);
		switch (myv->shape){
		case SHAPE_FIT:
			deep = (wide < hite ? wide : hite) * (1. - FRAC);
			MapSetSize (myv->map[AXIS_DOWN],hite-deep);
			MapSetSize (myv->map[AXIS_ACROSS],wide-deep);
			MapSetSize (myv->map[AXIS_DEEP],deep);
			break;
		case SHAPE_TRUE:
			fwide = (float)wide / (float)(MapZoom (myv->map[AXIS_ACROSS]) +
				MapZoom (myv->map[AXIS_DEEP]));
			fhite = (float)hite / (float)(MapZoom (myv->map[AXIS_DOWN]) +
				MapZoom (myv->map[AXIS_DEEP]));
			if (fwide < fhite ) {
				MapSetSize (myv->map[AXIS_DOWN],
				(int)(fwide*MapZoom(myv->map[AXIS_DOWN])));
				MapSetSize (myv->map[AXIS_ACROSS],
				(int)(fwide*MapZoom(myv->map[AXIS_ACROSS])));
				MapSetSize (myv->map[AXIS_DEEP],
				(int)(fwide*MapZoom(myv->map[AXIS_DEEP])));
				}
			else	{
				MapSetSize (myv->map[AXIS_DOWN],
				(int)(fhite*MapZoom(myv->map[AXIS_DOWN])));
				MapSetSize (myv->map[AXIS_ACROSS],
				(int)(fhite*MapZoom(myv->map[AXIS_ACROSS])));
				MapSetSize (myv->map[AXIS_DEEP],
				(int)(fhite*MapZoom(myv->map[AXIS_DEEP])));
				}
			break;
		case SHAPE_PIXEL:
			fwide = (float)wide / (float)(MapWindow (myv->map[AXIS_ACROSS]) + MapWindow (myv->map[AXIS_DEEP]));
			fhite = (float)hite / (float)(MapWindow (myv->map[AXIS_DOWN]) +
				MapWindow (myv->map[AXIS_DEEP]));
			if (fwide < 1.0) {
				UIMessage ("dataset too wide; shrink data or enlarge window");
/*				ViewSetShape (SHAPE_FIT);*/
				break;
				}
			if (fhite < 1.0) {
				UIMessage ("dataset too high; window data or enlarge window");
/*				ViewSetShape (SHAPE_FIT);*/
				break;
				}
			MapSetSize (myv->map[AXIS_DOWN],
			MapWindow(myv->map[AXIS_DOWN]));
			MapSetSize (myv->map[AXIS_ACROSS],
			MapWindow(myv->map[AXIS_ACROSS]));
			MapSetSize (myv->map[AXIS_DEEP],
			MapWindow(myv->map[AXIS_DEEP]));
			break;
			}
		break;
	case VIEW_FRONT:
		ViewSingleSize (myv,myv->map[AXIS_ACROSS],myv->map[AXIS_DOWN],myv->map[AXIS_DEEP]);
		break;
	case VIEW_SIDE:
		ViewSingleSize (myv,myv->map[AXIS_DEEP],myv->map[AXIS_DOWN],myv->map[AXIS_ACROSS]);
		break;
	case VIEW_TOP:
		ViewSingleSize (myv,myv->map[AXIS_ACROSS],myv->map[AXIS_DEEP],myv->map[AXIS_DOWN]);
		break;
	case VIEW_XSEC:
		MapSetSize (myv->map[AXIS_DOWN],myv->hite-VMARGIN);
		MapSetSize (myv->map[AXIS_ACROSS],(myv->wide-HMARGIN)/2);
		break;
	case VIEW_ARRAY:
		MapSetSize (myv->map[AXIS_ACROSS],
		(myv->wide-(myv->nacross-1)*GAP)/myv->nacross);
		MapSetSize (myv->map[AXIS_DOWN],
		(myv->hite-(myv->ndown-1)*GAP)/myv->ndown);
		break;
		}
}
}
	DrawFreePixmaps();
}

/* size for single panel */
void ViewSingleSize (View view,Map hmap,Map vmap,Map zmap)
	{
	float fwide, fhite, scale;
	int size;

	switch (view->shape) {
	case SHAPE_FIT:
		MapSetSize (hmap,view->wide-HMARGIN);
		MapSetSize (vmap,view->hite-VMARGIN);
		break;
	case SHAPE_TRUE:
		fwide = (float)(view->wide - HMARGIN) / (float)MapZoom(hmap);
		fhite = (float)(view->hite - VMARGIN) / (float)MapZoom(vmap);
		scale = fwide < fhite ? fwide : fhite;
		MapSetSize (vmap,(int)scale*MapZoom(vmap));
		MapSetSize (hmap,(int)scale*MapZoom(vmap));
		break;
	case SHAPE_PIXEL:
		fwide = (float)(view->wide - HMARGIN) / (float)MapWindow(hmap);
		fhite = (float)(view->hite - VMARGIN) / (float)MapWindow(vmap);
		size = (fhite < 1. ? fhite : 1.) *
			(MapLast (vmap) -
			MapFirst (vmap));
		MapSet (vmap,
			MapAxis(vmap),
			size>0?size:-size,
			MapFirst (vmap),
			MapLast (vmap),
			MapFirst (vmap),
			MapLast (vmap),
			MapDmovie (hmap));
		size = (fwide < 1. ? fwide : 1.) *
			(MapLast (hmap) -
			MapFirst (hmap));
		MapSet (hmap,
			MapAxis(hmap),
			size>0?size:-size,
			MapFirst (hmap),
			MapLast (hmap),
			MapFirst (hmap),
			MapLast (hmap),
			MapDmovie (hmap));
		break;
		}
	}

/* callback for initial view size */
int ViewSize0 ()
{
  ViewSize (views);
  return(0);
}

/* extract view from data; depends upon movie and view mode */
void ViewDraw (View view,int mode)
	{
	Map map;
	int pixels;
	int ishow,showpicks;
	if(view->data->pickable==0) showpicks=0;
	else showpicks=view->showpicks;
	if (UIFirst()) return;
	if (!view || !view->map[AXIS_DEEP]) return;
	pixels = 0;

	if (views->roundRobinOn && view != current_view()) return;

	if (mode == DRAW_ALL && view->first==1) {
		TymeStart ();
		DrawWatch (1);
		if (view->style != VIEW_FENCE && using_overlays()==1) {
			RenderClear ();
			DrawClear();
			}
		PlaneReset ();
		}
	if (showpicks && view->style != VIEW_TRANSP){
	  if(using_overlays()==1){
		 AnoDraw (NO_INDEX,ERASE);
		 PikDraw (NO_INDEX,ERASE);
		}
	}

	switch (view->style) {
	case VIEW_CUBE:
		ViewDrawCube (view,mode);
		pixels += MapSize (view->map[AXIS_DOWN]) * MapSize (view->map[AXIS_ACROSS]);
		pixels += MapSize (view->map[AXIS_DEEP]) * MapSize (view->map[AXIS_DOWN]);
		pixels += MapSize (view->map[AXIS_ACROSS]) * MapSize (view->map[AXIS_DEEP]);
		break;
	case VIEW_FENCE:
		ViewDrawFence (view,view->fence);
		if (view->fence & DRAW_FRONT) {
			pixels += MapSize (view->map[AXIS_DOWN]) * MapSize (view->map[AXIS_ACROSS]);
			}
		if (view->fence & DRAW_SIDE) {
			pixels += MapSize (view->map[AXIS_DEEP]) * MapSize (view->map[AXIS_DOWN]);
			}
		if (view->fence & DRAW_TOP) {
			pixels += MapSize (view->map[AXIS_ACROSS]) * MapSize (view->map[AXIS_DEEP]);
			}
		break;
	case VIEW_THREE:
		ViewDrawPlan (view,mode);
		pixels += MapSize (view->map[AXIS_DOWN]) * MapSize (view->map[AXIS_ACROSS]);
		pixels += MapSize (view->map[AXIS_DEEP]) * MapSize (view->map[AXIS_DOWN]);
		pixels += MapSize (view->map[AXIS_ACROSS]) * MapSize (view->map[AXIS_DEEP]);
		break;
	case VIEW_FRONT:
	case VIEW_XSEC:
	RENDER_ATTR(0,0,mode==DRAW_ALL,0,(mode&DRAW_IMAGE),(AXIS_LEFT|AXIS_BASE)*!MovieRun()*!UIPathViewAnimateRun()*!UIHistoryAnimateRun(),!MovieRun()*!UIPathViewAnimateRun()*!UIHistoryAnimateRun(),0,!MovieRun()*!UIPathViewAnimateRun()*!UIHistoryAnimateRun(),showpicks);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DOWN],view->map[AXIS_DEEP],
		view->map[AXIS_4D],view->map[AXIS_5D],
		view->x0+view->margins[MARGIN_LEFT],view->y0+view->margins[MARGIN_TOP],
		view->margins,_attr_);
		pixels += MapSize (view->map[AXIS_DOWN]) * MapSize (view->map[AXIS_ACROSS]);
		break;
	case VIEW_SIDE:
	RENDER_ATTR(0,0,mode==DRAW_ALL,0,(mode&DRAW_IMAGE),(AXIS_LEFT|AXIS_BASE)*!UIMotionRun(),!UIMotionRun(),0,!UIMotionRun(),showpicks);
	RenderHorz (view->data,view->map[AXIS_DEEP],view->map[AXIS_DOWN],
		view->map[AXIS_ACROSS],view->map[AXIS_4D],view->map[AXIS_5D],
		view->x0+view->margins[MARGIN_LEFT],view->y0+view->margins[MARGIN_TOP],
		view->margins,_attr_);
		pixels += MapSize (view->map[AXIS_DOWN]) * MapSize (view->map[AXIS_DEEP]);
		break;
	case VIEW_TOP:
	RENDER_ATTR(0,0,mode==DRAW_ALL,0,(mode&DRAW_IMAGE),(AXIS_LEFT|AXIS_BASE)*!UIMotionRun(),!UIMotionRun(),0,!UIMotionRun(),showpicks);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DEEP],
		view->map[AXIS_DOWN],view->map[AXIS_4D],view->map[AXIS_5D],
	 view->x0+view->margins[MARGIN_LEFT],view->y0+view->margins[MARGIN_TOP],
		view->margins,_attr_);
		pixels += MapSize (view->map[AXIS_ACROSS]) * MapSize (view->map[AXIS_DEEP]);
		break;
	case VIEW_ARRAY:
		ViewDrawArray (view);
		pixels += MapSize (view->map[AXIS_DOWN]) * MapSize (view->map[AXIS_ACROSS]) * view->nacross * view->ndown;
		break;
	case VIEW_TRANSP:
		ViewDrawTransp (view);
		pixels += MapSize (view->map[AXIS_DOWN]) * MapSize (view->map[AXIS_ACROSS]) * MapSize(view->map[AXIS_DEEP]);
		break;
		}

	map = ViewMovieMap (view);
	switch (view->style) {
	case VIEW_CUBE:
	case VIEW_FENCE:
	case VIEW_THREE:
	case VIEW_FRONT:
	case VIEW_SIDE:
	case VIEW_TOP:
   if(view->active){
		strcpy (message,AxisScript(MapAxis(map),MapFrame(map)));
		strcat (message,"                                   ");
/*		DrawText (view->margins[MARGIN_LEFT]+view->x0,0,TEXT_H0|TEXT_V0,DataTitle(view->data));*/
/*		DrawText (view->margins[MARGIN_LEFT]+view->x0,15,TEXT_H0|TEXT_V0,message);*/
   }
		break;
		}
	if (showpicks && view->style != VIEW_TRANSP){
		 AnoDraw (NO_INDEX,DRAW);
		 PikDraw (NO_INDEX,DRAW);
		}
	if (mode == DRAW_ALL && view->style != VIEW_TRANSP) {
		TymeEnd (pixels);
		DrawWatch (0);
		}
	}

/* set array */
void ViewArray (View view,int nacross,int ndown,int across0,int dacross)
	{
	if (!view) return;
	if (view->style != VIEW_ARRAY) {
		view->style = VIEW_ARRAY;
		MapSetFrame (ViewMovieMap(view),0);
		}
	view->nacross = nacross;
	view->ndown = ndown;
	view->dacross = dacross;
	view->across0 = across0;
	MapSetFrameBounds (view->map[AXIS_DEEP],view->across0,view->across0+view->nacross*view->ndown*view->dacross);
	ViewSize (views);
	ViewDrawAll();
}


/* draw array */
void ViewDrawArray (View view)
	{
	int panel,frame, ih, iv, margins[4],iy,ix;
	string label;
	char *labelp,labelp1;
	int ishow,showpicks;
	if(view->data->pickable==0) showpicks=0;
	else showpicks=view->showpicks;
	
	MapSaveFrame (view->map[AXIS_DEEP]);
	for (panel=0; panel<view->nacross*view->ndown; panel++) {
		frame = panel * view->dacross + view->across0;
		if (frame >= AxisSize(MapAxis(view->map[AXIS_DEEP]))) break;
		iv = panel / view->nacross;
		ih = panel % view->nacross;
		ix=ih*(MapSize(view->map[AXIS_ACROSS])+GAP)+view->x0;
		iy=iv*(MapSize(view->map[AXIS_DOWN])+GAP)+view->y0;
		MapSetFrame (view->map[AXIS_DEEP],frame);
		RENDER_ATTR(0,0,1,0,DRAW_FRONT,0,1,0,0,showpicks);
		RenderHorz (view->data,view->map[AXIS_ACROSS],
			view->map[AXIS_DOWN],view->map[AXIS_DEEP],view->map[AXIS_4D],view->map[AXIS_5D],
      ix,iy,
			view->margins,_attr_);
			if (MapSize(view->map[AXIS_ACROSS]) > 100) {
			DrawText (ix,iy+2,TEXT_H0|TEXT_V0,AxisScript(ViewDataAxis(view,AXIS_DEEP),frame));
			}
		else{
			labelp = AxisScript(ViewDataAxis(view,AXIS_DEEP),frame);
			while (*labelp != '=' && *labelp != '\0') labelp++;
			if (*labelp == '=') labelp++;
			else labelp = AxisScript(ViewDataAxis(view,AXIS_DEEP),frame);
			DrawText (ix,iy,TEXT_H0|TEXT_V0,labelp);
			}
		}
	MapRestoreFrame (view->map[AXIS_DEEP]);
	}

/* draw array of picked renders */
void ViewDrawPicks (View view)
	{
	int panel,frame, ih, iv, margins[4], save;
	string label;
	char *labelp;
	int ishow,showpicks;
	if(view->data->pickable==0) showpicks=0;
	else showpicks=view->showpicks;

	save = MapFrame(view->map[AXIS_DEEP]);
	for (panel=0; panel<view->nacross*view->ndown; panel++) {
		if  (panel > PickSize()) break;
		frame = PickFrame (panel);
		if (frame == NO_INDEX||
		frame >= AxisSize(MapAxis(view->map[AXIS_DEEP]))) break;
		iv = panel / view->nacross;
		ih = panel % view->nacross;
		MapSetFrame (view->map[AXIS_DEEP],frame);
		RENDER_ATTR(0,0,1,0,DRAW_FRONT,0,1,0,0,showpicks);
		RenderHorz (view->data,view->map[AXIS_ACROSS],
			view->map[AXIS_DOWN],view->map[AXIS_DEEP],view->map[AXIS_4D],view->map[AXIS_5D],
			ih=ih*(MapSize(view->map[AXIS_ACROSS])+GAP),
			iv=iv*(MapSize(view->map[AXIS_DOWN])+GAP),
			view->margins,_attr_);
		if (MapSize(view->map[AXIS_ACROSS]) > 100) {
			DrawText (ih,iv,TEXT_H0|TEXT_V0,AxisScript(ViewDataAxis(view,AXIS_DEEP),frame));
			}
		else	{
			labelp = AxisScript(ViewDataAxis(view,AXIS_DEEP),frame);
			while (*labelp != '=' && *labelp != '\0') labelp++;
			if (*labelp == '=') labelp++;
			else labelp = AxisScript(ViewDataAxis(view,AXIS_DEEP),frame);
			DrawText (ih,iv,TEXT_H0|TEXT_V0,labelp);
			}
		}
	MapSetFrame (view->map[AXIS_DEEP],save);
	}

/* cube extracts three panels */
void ViewDrawCube (View view,int mode)
	{
	int showpicks;
	if(view->data->pickable==0) showpicks=0;
	else showpicks=view->showpicks;
	if (!view) return;

	/* front */
	RENDER_ATTR(0,0,mode==DRAW_ALL,0,(mode&DRAW_FRONT)*(mode&DRAW_IMAGE),(AXIS_LEFT|AXIS_BASE)*!UIMotionRun(),!UIMotionRun(),1,!UIMotionRun(),showpicks);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DOWN],
		view->map[AXIS_DEEP],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT]+view->x0,
		view->margins[MARGIN_TOP]+view->y0+MapSize(view->map[AXIS_DEEP])+GAP,
		view->margins,_attr_);
	/* side */
	RENDER_ATTR(0,1,mode==DRAW_ALL,0,(mode&DRAW_SIDE)*(mode&DRAW_IMAGE),(AXIS_BASE)*!UIMotionRun(),!UIMotionRun(),1,!UIMotionRun(),showpicks);
	RenderVert (view->data,view->map[AXIS_DEEP],view->map[AXIS_DOWN],
		view->map[AXIS_ACROSS],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT]+view->x0+MapSize(view->map[AXIS_ACROSS])+GAP+1,
		MapSize(view->map[AXIS_DEEP])+view->y0+view->margins[MARGIN_TOP]+GAP,
		view->margins,_attr_);
	/* top */
	RENDER_ATTR(0,1,mode==DRAW_ALL,0,(mode&DRAW_TOP)*(mode&DRAW_IMAGE),(AXIS_LEFT)*!UIMotionRun(),!UIMotionRun(),1,!UIMotionRun(),showpicks);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DEEP],
		view->map[AXIS_DOWN],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT]+view->x0,
		view->margins[MARGIN_TOP]+view->y0,
		view->margins,_attr_);
	}

/* fence extracts three panels */
void ViewDrawFence (View view,int mode)
	{
	if (!view) return;
	RenderClear();
	DrawClear();

	/* front */
	if (mode & DRAW_FRONT) {
	RENDER_ATTR(0,0,mode==DRAW_ALL,1,mode&DRAW_FRONT,0,!UIMotionRun(),0,0,0);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DOWN],
		view->map[AXIS_DEEP],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_DEEP])-
		MapInverse(view->map[AXIS_DEEP],
		MapFrame(view->map[AXIS_DEEP])),
		view->margins[MARGIN_TOP]+MapInverse(view->map[AXIS_DEEP],
		MapFrame(view->map[AXIS_DEEP])),
		view->margins,_attr_);
		}
	RENDER_ATTR(0,0,mode==DRAW_ALL,0,0,(AXIS_LEFT|AXIS_BASE),0,0,0,0);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DOWN],
		view->map[AXIS_DEEP],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT],
		view->margins[MARGIN_TOP]+MapSize(view->map[AXIS_DEEP]),
		view->margins,_attr_);
	/* side */
	if (mode & DRAW_SIDE) {
	RENDER_ATTR(0,1,mode==DRAW_ALL,1,mode&DRAW_SIDE,0,!UIMotionRun(),0,0,0);
	RenderVert (view->data,view->map[AXIS_DEEP],view->map[AXIS_DOWN],
		view->map[AXIS_ACROSS],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT]+MapInverse(view->map[AXIS_ACROSS],
		MapFrame(view->map[AXIS_ACROSS]))-1,
		MapSize(view->map[AXIS_DEEP])+view->margins[MARGIN_TOP],
		view->margins,_attr_);
		}
	RENDER_ATTR(0,1,mode==DRAW_ALL,0,0,(AXIS_BASE),0,0,0,0);
	RenderVert (view->data,view->map[AXIS_DEEP],view->map[AXIS_DOWN],
		view->map[AXIS_ACROSS],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS])+GAP+1,
		MapSize(view->map[AXIS_DEEP])+view->margins[MARGIN_TOP],
		view->margins,_attr_);
	/* top */
	if (mode & DRAW_TOP) {
	RENDER_ATTR(0,1,mode==DRAW_ALL,1,mode&DRAW_TOP,0,!UIMotionRun(),0,0,0);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DEEP],
		view->map[AXIS_DOWN],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT],
		view->margins[MARGIN_TOP]+MapInverse(view->map[AXIS_DOWN],
		MapFrame(view->map[AXIS_DOWN])),
		view->margins,_attr_);
		}
	RENDER_ATTR(0,1,mode==DRAW_ALL,0,0,(AXIS_LEFT),0,0,0,0);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DEEP],
		view->map[AXIS_DOWN],view->map[AXIS_4D],view->map[AXIS_5D],
		view->margins[MARGIN_LEFT],
		view->margins[MARGIN_TOP],
		view->margins,_attr_);
	}

/* plan view extracts three panels */
void ViewDrawPlan (View view,int mode)
	{
	int showpicks;
	if(view->data->pickable==0) showpicks=0;
	else showpicks=view->showpicks;
	if (!view) return;

	/* front */
	RENDER_ATTR(0,0,mode==DRAW_ALL,0,(mode&DRAW_FRONT)*(mode&DRAW_IMAGE),(AXIS_LEFT|AXIS_BASE)*!UIMotionRun(),!UIMotionRun(),1,!UIMotionRun(),showpicks);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DOWN],
		view->map[AXIS_DEEP],view->map[AXIS_4D],view->map[AXIS_5D],
		view->x0+view->margins[MARGIN_LEFT],
		view->y0+MapSize(view->map[AXIS_DEEP])+view->margins[MARGIN_TOP]+GAP,
		view->margins,_attr_);
	/* side */
	RENDER_ATTR(0,0,mode==DRAW_ALL,0,(mode&DRAW_SIDE)*(mode&DRAW_IMAGE),(AXIS_BASE)*!UIMotionRun(),!UIMotionRun(),1,!UIMotionRun(),showpicks);
	RenderVert (view->data,view->map[AXIS_DEEP],view->map[AXIS_DOWN],
		view->map[AXIS_ACROSS],view->map[AXIS_4D],view->map[AXIS_5D],
		view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS])+GAP+1,
		view->y0+MapSize(view->map[AXIS_DEEP])+view->margins[MARGIN_TOP]+GAP,
		view->margins,_attr_);
	/* top */
	RENDER_ATTR(0,0,mode==DRAW_ALL,0,(mode&DRAW_TOP)*(mode&DRAW_IMAGE),(AXIS_LEFT)*!UIMotionRun(),!UIMotionRun(),1,!UIMotionRun(),showpicks);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DEEP],
		view->map[AXIS_DOWN],view->map[AXIS_4D],view->map[AXIS_5D],
		view->x0+view->margins[MARGIN_LEFT],
		view->y0+view->margins[MARGIN_TOP],
		view->margins,_attr_);
	}

/* draw transparent cube */
void ViewDrawTransp (View view)
	{
	int iz,nz;
	/* draw all axes */
	RENDER_ATTR(0,0,DRAW_ALL,0,0,(AXIS_LEFT|AXIS_BASE),0,0,0,0);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DOWN],
		view->map[AXIS_DEEP],view->map[AXIS_4D],view->map[AXIS_5D],
		view->x0+view->margins[MARGIN_LEFT],
		view->y0+view->margins[MARGIN_TOP]+MapSize(view->map[AXIS_DEEP]),
		view->margins,_attr_);
	RENDER_ATTR(0,1,DRAW_ALL,0,0,(AXIS_BASE),0,0,0,0);
	RenderVert (view->data,view->map[AXIS_DEEP],view->map[AXIS_DOWN],
		view->map[AXIS_ACROSS],view->map[AXIS_4D],view->map[AXIS_5D],
		view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS])+1,
		view->y0+MapSize(view->map[AXIS_DEEP])+view->margins[MARGIN_TOP],
		view->margins,_attr_);
	RENDER_ATTR(0,1,DRAW_ALL,0,0,(AXIS_LEFT),0,0,0,0);
	RenderHorz (view->data,view->map[AXIS_ACROSS],view->map[AXIS_DEEP],
		view->map[AXIS_DOWN],view->map[AXIS_4D],view->map[AXIS_5D],
		view->x0+view->margins[MARGIN_LEFT],
		view->y0+view->margins[MARGIN_TOP],
		view->margins,_attr_);
	MapSaveFrame (ViewMovieMap(view));
	switch (view->movie) {
	case MOVIE_FRONT:
		MapSetFrame1 (view->map[AXIS_DEEP],0);
		break;
	case MOVIE_SIDE:
		MapSetFrame1 (view->map[AXIS_ACROSS],0);
		break;
	case MOVIE_TOP:
		MapSetFrame1 (view->map[AXIS_DOWN],MapSize(view->map[AXIS_DOWN])-1);
		break;
		}
	// UITimer(1,ViewDrawTranspCallback(view));
	DrawLine (view->x0+view->margins[MARGIN_LEFT],
		view->y0+view->margins[MARGIN_TOP]+MapSize(view->map[AXIS_DEEP]),
		view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS]),
		view->y0+view->margins[MARGIN_TOP]+MapSize(view->map[AXIS_DEEP]),
		DRAW);
	DrawLine (view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS])+
		MapSize(view->map[AXIS_DEEP]),
		view->y0+view->margins[MARGIN_TOP],
		view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS]),
		view->y0+view->margins[MARGIN_TOP]+MapSize(view->map[AXIS_DEEP]),
		DRAW);
	DrawLine (view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS]),
		view->y0+view->margins[MARGIN_TOP]+MapSize(view->map[AXIS_DOWN])+
		MapSize(view->map[AXIS_DEEP]),
		view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS]),
		view->y0+view->margins[MARGIN_TOP]+MapSize(view->map[AXIS_DEEP]),
		DRAW);
	DrawLine (view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_DEEP]),
		view->y0+view->margins[MARGIN_TOP],
		view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_DEEP])+
		MapSize(view->map[AXIS_ACROSS]),
		view->y0+view->margins[MARGIN_TOP],
		DRAW);
	DrawLine (view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_ACROSS])+
		MapSize(view->map[AXIS_DEEP]),
		view->y0+view->margins[MARGIN_TOP],
		view->x0+view->margins[MARGIN_LEFT]+MapSize(view->map[AXIS_DEEP])+
		MapSize(view->map[AXIS_ACROSS]),
		view->y0+view->margins[MARGIN_TOP]+MapSize(view->map[AXIS_DOWN]),
		DRAW);
	}

/* callback to animate transparent drawing */
XtTimerCallbackProc ViewDrawTranspCallback (View view)
	{
	int block;

	switch (view->rate) {
	case VIEW_RATE_ALL: block = 1; break;
	case VIEW_RATE_TENTHS: block = 10; break;
	case VIEW_RATE_EACH: block = 10000; break;
		}
	switch (view->movie) {
	/* front */
	case MOVIE_FRONT:
		block = MapSize (view->map[AXIS_DEEP]) / block;
		block = block > 0 ? block : 1;
		MapSetFrame (view->map[AXIS_DEEP],
		MapMap(view->map[AXIS_DEEP],MapFrame1(view->map[AXIS_DEEP])));
		RENDER_ATTR(1,0,0,0,1,0,0,0,0,0);
		RenderHorz (view->data,view->map[AXIS_ACROSS],
			view->map[AXIS_DOWN],view->map[AXIS_DEEP],view->map[AXIS_4D],view->map[AXIS_5D],
			view->x0+view->margins[MARGIN_LEFT]+
			MapSize(view->map[AXIS_DEEP])-
			MapFrame1(view->map[AXIS_DEEP]),
			view->y0+view->margins[MARGIN_TOP] +
			MapFrame1(view->map[AXIS_DEEP]),
			view->margins,_attr_);
		if (MapFrame1(view->map[AXIS_DEEP]) && (MapFrame1(view->map[AXIS_DEEP])%block) == 0) {
/*			RenderDraw ();*/
			ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
			}
		if (MapFrame1(view->map[AXIS_DEEP]) < MapSize(view->map[AXIS_DEEP])-1) {
			UITimer(1,ViewDrawTranspCallback(view));
			}
		else	{
/*			RenderDraw ();*/
			ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
			MapRestoreFrame (view->map[AXIS_DEEP]);
			TymeEnd (MapSize (view->map[AXIS_DOWN]) *
				MapSize (view->map[AXIS_ACROSS]) *
				MapSize(view->map[AXIS_DEEP]));
		DrawWatch(0);
			}
		MapSetFrame1 (view->map[AXIS_DEEP],MapFrame1(view->map[AXIS_DEEP])+1);
		break;
	/* side */
	case MOVIE_SIDE:
		block = MapSize (view->map[AXIS_ACROSS]) / block;
		block = block > 0 ? block : 1;
		MapSetFrame (view->map[AXIS_ACROSS],
		MapMap(view->map[AXIS_ACROSS],MapFrame1(view->map[AXIS_ACROSS])));
		RENDER_ATTR(1,1,0,0,1,0,0,0,0,0);
		RenderVert (view->data,view->map[AXIS_DEEP],view->map[AXIS_DOWN],
			view->map[AXIS_ACROSS],view->map[AXIS_4D],view->map[AXIS_5D],
			view->x0+view->margins[MARGIN_LEFT]+MapFrame1(view->map[AXIS_ACROSS]),
			view->y0+MapSize(view->map[AXIS_DEEP])+view->margins[MARGIN_TOP],
			view->margins,_attr_);
		if (MapFrame1(view->map[AXIS_ACROSS]) && MapFrame1(view->map[AXIS_ACROSS])%block == 0){ 
/*			RenderDraw ();*/
			ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
      }
		if (MapFrame1(view->map[AXIS_ACROSS]) < MapSize (view->map[AXIS_ACROSS])-1) {
			UITimer(1,ViewDrawTranspCallback(view));
			}
		else	{
/*			RenderDraw ();*/
			ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
			MapRestoreFrame (view->map[AXIS_ACROSS]);;
			TymeEnd (MapSize (view->map[AXIS_DOWN]) *
				MapSize (view->map[AXIS_ACROSS]) *
				MapSize(view->map[AXIS_DEEP]));
			DrawWatch(0);
			}
		MapSetFrame1 (view->map[AXIS_ACROSS],MapFrame1(view->map[AXIS_ACROSS])+1);
		break;
	/* top */
	case MOVIE_TOP:
		block = MapSize (view->map[AXIS_DOWN]) / block;
		block = block > 0 ? block : 1;
		MapSetFrame (view->map[AXIS_DOWN],
		MapMap(view->map[AXIS_DOWN],MapFrame1(view->map[AXIS_DOWN])));
		RENDER_ATTR(1,1,0,0,1,0,0,0,0,0);
		RenderHorz (view->data,view->map[AXIS_ACROSS],
			view->map[AXIS_DEEP],view->map[AXIS_DOWN],view->map[AXIS_4D],view->map[AXIS_5D],
			view->x0+view->margins[MARGIN_LEFT],
			view->y0+view->margins[MARGIN_TOP]+MapFrame1(view->map[AXIS_DOWN]),
			view->margins,_attr_);
		if (MapFrame1(view->map[AXIS_DOWN]) != MapSize(view->map[AXIS_DOWN])-1 && 
			MapFrame1(view->map[AXIS_DOWN])%block == 0) {
/*			RenderDraw ();*/
			ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
			}
		if (MapFrame1(view->map[AXIS_DOWN]) > 0) {
			UITimer(1,ViewDrawTranspCallback(view));
			}
		else	{
/*			RenderDraw ();*/
			ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
			MapRestoreFrame (view->map[AXIS_DOWN]);
			TymeEnd (MapSize (view->map[AXIS_DOWN]) *
				MapSize (view->map[AXIS_ACROSS]) *
				MapSize(view->map[AXIS_DEEP]));
			DrawWatch (0);
			}
		MapSetFrame1 (view->map[AXIS_DOWN],MapFrame1(view->map[AXIS_DOWN])-1);
		break;
		}
  return(0);
	}


/* callback to draw entire view */
int ViewDrawAll ()
{
  View view;
  int i,iold, fenced;

  if (!views) return(0);
  if (UIFirst()) MainFirst();
  UIDrawColorbar(NULL);
  iold=current_view_num();
  fenced = 0;
  for (i=0; i < num_views(); i++){
    SetActiveView(i);
    view=current_view();
    ViewDraw (view,DRAW_ALL);
    fenced = (view->style == VIEW_FENCE);
  }
  SetActiveView(iold);

  if (view->style != VIEW_TRANSP){ 
    RenderDraw ();
    if (!fenced) ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
  }

  ViewHistorySaveAll();
  ViewHistoryMenus();
  ViewInfo(current_view());
  return(0);
}

void ViewChange(int change)
{
View myv;
 int item, i, iold;

 myv=current_view();
 item=myv->style+change;
 if(item<VIEW_FRONT) item=VIEW_CUBE;
 if(item>VIEW_CUBE) item=VIEW_FRONT;
 ViewStyleChoice(myv, item);
}

int ViewStyleChoice(View myv, int item) {
  extern UI ui;
  
  switch (item) {
  case VIEW_ARRAY:
    MovieOff();
    UIArrayRaise ();
    ViewArray (myv,ui->across,ui->down,ui->start,ui->delta);
    break;
  case VIEW_FRONT:
    ViewSetMovie (MOVIE_FRONT);
    break;
  case VIEW_SIDE:
    ViewSetMovie (MOVIE_SIDE);
    break;
  case VIEW_TOP:
    ViewSetMovie (MOVIE_TOP);
    break;
  case VIEW_FENCE:
    MovieOff ();
    UIFenceRaise ();
    break;
  case VIEW_TRANSP:
    MovieOff();
    UITranspRaise ();
    UIToggleSet (ui->shape_stretch,0);
    UIToggleSet (ui->shape_true,0);
    UIToggleSet (ui->shape_pixel,1);
    break;
  }

  if (item == VIEW_ROUNDROBIN+1) {
    ViewRoundRobinForward(DRAW);
  }
  else if (item == VIEW_ROUNDROBIN+2) {
    ViewRoundRobinBackward(DRAW);
  }
  else if (item == VIEW_ROUNDROBIN) {
    ViewRoundRobinToggle(DRAW);
  }
  else {
    ViewSetStyle(item);
    UISyzeReset ();
  }

  if (item < VIEW_ROUNDROBIN && item >= 0)
    ui->style = item;
  return(0);
}

void ViewDrawPart(int mode){
  View view;
  int i,iold;
  char* ViewValueScript(View);

  view=current_view();
  iold=current_view_num();
  if(using_overlays()==1) return;
  if (view->style != VIEW_TRANSP) RenderDraw ();
  for (i=0; i < num_views(); i++){
    view=views->view[i];
    if (views->roundRobinOn && i != views->roundRobinView) continue;

    ViewDraw (views->view[i],mode);
    sprintf (message,"%s %s %s %s %s VALUE=%s",
	     AxisScript(MapAxis(view->map[AXIS_DOWN]),MapFrame(view->map[AXIS_DOWN])),
	     AxisScript(MapAxis(view->map[AXIS_ACROSS]),MapFrame(view->map[AXIS_ACROSS])),
	     AxisScript(MapAxis(view->map[AXIS_DEEP]),MapFrame(view->map[AXIS_DEEP])),
	     AxisScript(MapAxis(view->map[AXIS_4D]),MapFrame(view->map[AXIS_4D])),
	     AxisScript(MapAxis(view->map[AXIS_5D]),MapFrame(view->map[AXIS_5D])),
	     ViewValueScript(view));
    strcat (message,"                                   ");
    DrawString(view->margins[MARGIN_LEFT]+view->x0+0.5*view->wide,10,0,
	       DataTitle(view->data), 7, DRAW);
    DrawString(view->margins[MARGIN_LEFT]+view->x0+0.5*view->wide,20,0,
	       (char*)message, 7, DRAW);
  }
  SetActiveView(iold);
  ViewHistorySaveAll();
}

/* callback to draw only current movie frame panel plus crosslines */
XtTimerCallbackProc ViewDrawMovie () {
  extern UI ui;
  if (UICanvasResizing(0)) {
    if ((ui->movieOn = MovieRun()) != 0)
      MovieOff();
    return(0);
  }
  
  int h0, v0, nh, nv, mode=0;
  View myv,v2;
  int iold, fenced;
  int i,imovie,ic;
  PickPoint_ pick;
  Map map;
  
  myv=current_view();
  if (!myv) return(0);
  map = ViewMovieMap (myv);
  /*	MapNextFrame (map);*/
  
  
  imovie=AxisDir(MapAxis(map));
  
  if (MovieCache()) {
    mode = DrawPixmap(MapFrame(map));
  }
  /*  if(using_overlays()==0) mode=0;*/
  mode = (1 - mode) * DRAW_IMAGE;
  
  ic=0;
  fenced = 0;
  iold=current_view_num();
  for(i=0; i < num_views() ; i++){
    SetActiveView(i);
    v2=current_view();
    switch(v2->style){
    case VIEW_THREE:
    case VIEW_CUBE:
    case VIEW_FENCE:
      if (v2->style == VIEW_FENCE)
	fenced = 1;
      if(imovie == AxisDir(MapAxis(v2->map[AXIS_DOWN]))) {
	MapNextFrame (v2->map[AXIS_DOWN]);
	ViewDraw(v2,DRAW_TOP|mode);
	ic+=1;
      }
      else if(imovie==AxisDir(MapAxis(v2->map[AXIS_ACROSS]))){
	MapNextFrame (v2->map[AXIS_ACROSS]);
	ViewDraw(v2,DRAW_SIDE|mode);
	ic+=1;
      }
      else if(imovie==AxisDir(MapAxis(v2->map[AXIS_DEEP]))){
	MapNextFrame (v2->map[AXIS_DEEP]);
	ViewDraw(v2,DRAW_FRONT|mode);
	ic+=1;
      }
      break;
    case VIEW_FRONT:
      if(imovie==AxisDir(MapAxis(v2->map[AXIS_DEEP]))){
	MapNextFrame (v2->map[AXIS_DEEP]);
	ViewDraw(v2,DRAW_FRONT|mode);
	ic+=1;
      }
      break;
    case VIEW_SIDE:
      if(imovie==AxisDir(MapAxis(v2->map[AXIS_ACROSS]))){
	MapNextFrame (v2->map[AXIS_ACROSS]);
	ViewDraw(v2,DRAW_SIDE|mode);
	ic+=1;
      }
      break;
    case VIEW_TOP:
      if(imovie == AxisDir(MapAxis(v2->map[AXIS_DOWN]))) {
	MapNextFrame (v2->map[AXIS_DOWN]);
	ViewDraw(v2,DRAW_TOP|mode);
	ic+=1;
      }
      break;
    default: /*we are showing a single frame */
      ViewDraw (v2,DRAW_ALL);
      break;
    }
  }

  SetActiveView(iold);
  SetMovieMulti(ic-1);	
  if (fenced == 1) {
    ViewDrawAll();
  }
  else if (using_overlays()==1) {
    RenderDraw ();
  }
  else {
    ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
  }
  if (mode) {
    if (MovieCache()) {
      DrawSavePixmap (MapFrame(map));
    }
  }

#if defined(XAW)
  if (MovieRun()) UITimer(MovieDelay()+1,ViewDrawMovie);
#endif
#if defined(MOTIF_FOUND)
  if (MovieRun()) UITimer(MovieDelay()*10+1,(XtTimerCallbackProc)ViewDrawMovie);
#endif
  
  
  iold=current_view_num();
  for(i=0; i < num_views() ; i++){
    SetActiveView(i);
    myv=current_view();
    myv->change_loc_func();
  }
  SetActiveView(iold);
  
  UIWakeup();
  ViewHistorySaveAll();
  
  return(0);
}

/* set movie option */
void ViewSetMovie (int movie)
	{
	View myv;
	Map map;
	int i;

	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  myv->movie = movie;
	  map = ViewMovieMap (myv);
	}

/*	PlaneFind (AxisDir(MapAxis(map)),NO_INDEX);*/
	DrawFreePixmaps();
	if (myv->style == VIEW_TRANSP) ViewDraw (myv,DRAW_ALL);
	}

/* initialize movie bounds */
int ViewMovieFullBounds ()
	{
	  int imap, i;
	View myv;

	UIMotionOff(NULL);
	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  if (views->roundRobinOn == 0 && i != current_view_num()) continue;

	  for (imap=1; imap<VIEW_NAXIS; imap++) {
	    MapSetFrameBounds (myv->map[imap],MapFirst(myv->map[imap]),
			       MapLast(myv->map[imap]));
	    MapSetDmovie (myv->map[imap],1);
	  }
	}
  return(0);
	}

/* one of 3d views? */
int View3D ()
	{
	View myv;

	myv=current_view();

	switch (myv->style) {
	case VIEW_THREE:
	case VIEW_CUBE:
	case VIEW_FENCE:
		return (1);
	default:
		return (0);
		}
	}

/* OK to turn on movie */
int ViewMovieOK (void) {
  View myv;
  int i;
  
  for (i=0; i<num_views(); i++) {
    myv = views->view[i];
    
    switch (myv->style) {
    case VIEW_CUBE:
    case VIEW_THREE:
    case VIEW_FRONT:
    case VIEW_SIDE:
    case VIEW_TOP:
      continue;
    default:
      return(0);
    }
  }
  return(1);
}

/* set view option */
int ViewSetStyle (int style)
	{
	View myv;
	int size, i;

	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  if (!views->roundRobinOn && i != current_view_num()) continue;

	  if (style == myv->style) return(0);
	  myv->style = style;
	  /* shape defaults with view style */
	  /* set movie directions too */
	  switch (style) {
	  case VIEW_TRANSP:
	    myv->shape = SHAPE_PIXEL;
	    myv->movie = MOVIE_SIDE;
	    break;
	  case VIEW_FRONT:
	    myv->movie = MOVIE_FRONT;
	    break;
	  case VIEW_SIDE:
	    myv->movie = MOVIE_SIDE;
	    break;
	  case VIEW_TOP:
	    myv->movie = MOVIE_TOP;
	    break;
	  }
	}

	/* movie status */
	if (ViewMovieOK () == 0) {
	  MovieOff ();
	  UIPathViewAnimateOff(NULL);
	}
	ViewSize (views);
	ViewDrawAll();
	return(0);
	}

/* set fence mode */
int ViewSetFence (int mode)
	{
	View myv;
	int i;

	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  if (!views->roundRobinOn && i != current_view_num()) continue;
	  myv->fence = mode;
	  myv->style = VIEW_FENCE;
	}
	DrawFreePixmaps ();
	ViewDrawAll();
  return(0);
	}

/* set transparency draw rate */
int ViewSetTranspRate (int mode)
	{
	View myv;
	int i;

	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  if (!views->roundRobinOn && i != current_view_num()) continue;
	  myv->rate = mode;
	}
  return(0);
	}

/* set shape mode */
int ViewSetShape (int shape)
	{
	View myv;
	int i;

	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  if (!views->roundRobinOn && i != current_view_num()) continue;
	  myv->shape = shape;
	}
	ViewSize (views);
	ViewDrawAll();
	
   return(0);
	}

/* set fence mode */
int ViewToggleFence (int mode)
	{
	View myv;
	int i;

	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  if (views->roundRobinOn == 0 && i != current_view_num()) continue;
	  myv->fence ^= mode;
	  myv->style = VIEW_FENCE;
	}

	DrawFreePixmaps ();
	ViewDrawAll();
	return(0);
	}

/* set two cross line frames */
int ViewSetFrames (int x,int y)
{
  Map map;
  PickPoint_ pick;
  int iaxis,iv,i,ib;
  int iview_axes[2],iold,iview_loc[2];
  View myv;

  iv=FindView(x,y);
  if(iv!=NO_INDEX){
    SetActiveView(iv);
    iold=iv;
  }
  else iold=current_view_num();
  
  myv=current_view();
  
  if (!myv) return(0);
  /* undefined views */
  switch (myv->style) {
  case VIEW_FRONT:
  case VIEW_SIDE:
  case VIEW_TOP:
  case VIEW_XSEC:
  case VIEW_ARRAY:
  case VIEW_TRANSP:
    return(0);
  }
  /* translate pick */
  PickDecode (x,y,&pick,1);
  
  
  iview_axes[0]=pick.iaxis[1];
  iview_axes[1]=pick.iaxis[2];
  if(pick.iaxis[1]==-1 || pick.iaxis[2]==-1) return(0);

  /* reset frames */
  for (iaxis=0; iaxis<2; iaxis++) {
    for (i=0; i < num_views(); i++){
      for(ib=1; ib < DATA_NAXIS; ib++){
        if(AxisDir(MapAxis(views->view[i]->map[ib]))==iview_axes[iaxis]){
	  MapSetFrame (views->view[i]->map[ib],pick.index[iview_axes[iaxis]]);
        }
      }
    }
  }
  DrawFreePixmaps ();
  for(i=0; i < num_views() && !views->roundRobinOn; i++){
    SetActiveView(i);
    myv=current_view();
    myv->change_loc_func();
  }
  ViewDrawAll();
  SetActiveView(iold);
  
  return(0);
}

/* set frame from slider */
int ViewSetFrame (int index,int mode)
	{
	Map map;
	int frame;
	View myv;

	myv=current_view();

	map = ViewMovieMap (myv);
	if (mode == FRAME_PERCENT) {
		frame = MapLow(map) + (index * (MapHigh(map) - MapLow(map))) / 100;
		}
	MapSetFrame (map,frame);
	if (myv->style == VIEW_ARRAY) {
		RenderClear();
		DrawClear();
		}
	switch (myv->movie) {
	case MOVIE_TOP: ViewDraw (myv,DRAW_TOP); break;
	case MOVIE_SIDE: ViewDraw (myv,DRAW_SIDE); break;
	case MOVIE_FRONT: ViewDraw (myv,DRAW_FRONT); break;
		}
/*	RenderDraw ();*/
	 ViewDrawPart(DRAW_FRONT|DRAW_SIDE|DRAW_TOP|DRAW_AXES);
  return(0);
	}

/* set pick mode */
int ViewSetPick (int pick)
	{
	View myv;

	myv=current_view();

	if (!myv) return(0);
	if (pick != myv->showpicks) {
		PickDrawAll (pick);
		}
	myv->showpicks = pick;
	PikDraw (NO_INDEX,myv->showpicks);
	AnoDraw (NO_INDEX,myv->showpicks);
  return(0);
	}

/* toggle pick mode */
int ViewTogglePick ()
	{
	View myv;

	myv=current_view();

	if (!myv) return(0);
	myv->showpicks = 1 - myv->showpicks;
	PickDrawAll (myv->showpicks);
	PikDraw (NO_INDEX,myv->showpicks);
	AnoDraw (NO_INDEX,myv->showpicks);
  return(0);
	}

/* return pick state */
int ViewPick ()
	{
	View myv;


	myv=current_view();
	if (!myv) return (0);
	else return (myv->showpicks);
	}


/* return view movie */
int ViewMovie ()
	{
	View myv;


	myv=current_view();
	if (!myv) return (NO_INDEX);
	return (myv->movie);
	}

/* return map axis */
Map ViewMovieMap (View myv)
	{
	return (myv->map[myv->movie]);
	}

/* set frames to middle */
int ViewFramesMiddle ()
	{
	  int imap, i;
	View myv;
	
	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  for (imap=1; imap<VIEW_NAXIS; imap++) {
	    MapSetFrameBounds (myv->map[imap],MapFirst(myv->map[imap]),MapLast(myv->map[imap]));
	    MapSetFrame (myv->map[imap],(MapFirst(myv->map[imap])+MapLast(myv->map[imap]))/2);
	  }
	}
	ViewDrawAll();
  return(0);
	}

/* set frames to origin */
int ViewFramesOrigin ()
	{
	  int imap, i;
	View myv;

	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  for (imap=1; imap<VIEW_NAXIS; imap++) {
	    MapSetFrame (myv->map[imap],0);
	  }
	}
	ViewDrawAll();
	return(0);
	}

/* swap two map axes and draw */
int ViewSwapAxis (int a,int b)
{
	Map map;
	View myv;
	int i;
	
	for (i=0; i<num_views(); i++) {
	  myv = views->view[i];
	  if (!myv) continue;
	  if (!views->roundRobinOn && i != current_view_num())
	    continue;

	  if (MapSize(myv->map[b]) <= 1 || MapSize(myv->map[b]) <= 1) return(0);
	  // if (myv->shape == SHAPE_FIT) {
	  // MapSwap (myv->map[a],myv->map[b]);
	  // DrawFreePixmaps ();
	  // }
	  //else	{
	    map = myv->map[a];
	    myv->map[a] = myv->map[b];
	    myv->map[b] = map;
	    ViewSize (views);
	    //}
	  ViewSetMovie (myv->movie);
	}
	myv = views->view[0];
	if (myv->style != VIEW_ARRAY) ViewDrawAll();
	return(0);
}

/* flip an axes direction */
int ViewFlipAxis (int imap)
	{
	View myv;
	int i;

	for (i=0; i<num_views(); i++) {
	  myv=views->view[i];
	  if (!myv) continue;
	  if (!views->roundRobinOn && i != current_view_num()) continue;
	  MapFlip (myv->map[imap]);
	  DrawFreePixmaps ();
	}
	ViewDrawAll();
	return(0);
	}

/* magnification window given corner coordinates */
int ViewWindow (int x1,int y1,int x2,int y2,int hzoom,int vzoom)
	{
	Map map;
	int v, iaxis, haxis, vaxis,iv1,iv2,do1,i,iold, temp;
	PickPoint_ pick11, pick12, pick21, pick22;
	View myv;

 iv1=FindView(x1,y1);
 iv2=FindView(x2,y2);
	if(iv1==iv2 && iv1!= NO_INDEX) SetActiveView(iv1);

	myv=current_view();
	if (!myv) return(0);

	/* slippy mouse tolerance is five pixels */
	if (hzoom && (x1-x2)*(x1-x2) < 25) return(0);
	if (vzoom && (y1-y2)*(y1-y2) < 25) return(0);
	/* fetch indices */
	PickDecode ((x1<x2?x1:x2),(y1<y2?y1:y2),&pick11,0);
	PickDecode ((x1<x2?x1:x2),(y1>y2?y1:y2),&pick12,0);
	PickDecode ((x1>x2?x1:x2),(y1<y2?y1:y2),&pick21,0);
	PickDecode ((x1>x2?x1:x2),(y1>y2?y1:y2),&pick22,0);

do1=0; 
iold=current_view_num();
for(i=0; i < num_views(); i++){
	SetActiveView(i);
	myv=current_view();


	/* window across an array of frames */
	if (myv->style == VIEW_ARRAY && PickSameDir(&pick11,&pick22) && !PickSameFrame(&pick11,&pick22)) {
		MapSetFrameBounds (myv->map[AXIS_DEEP],pick11.index[haxis],pick22.index[vaxis]);
		UIArrayReset (0);
		UIArrayDraw ();
		return(0);
		}
	/* within a frame */
	if (!PickSameFrame(&pick11,&pick22)) return(0);
	/* sort out horizontal and vertical axes;
	   horizontal is that which differs for two corners with same y coordinate
	   vertical is that which differs for two corners with same x coordinate
	*/
	haxis = PickSharedDir (&pick11,&pick12);
	vaxis = PickSharedDir (&pick11,&pick21);
	/* one of these will fail for a skewed plot;
           then the answer is the remaining coordinate */
	if (!haxis) {
		if (!vaxis) return(0);
		if (vaxis == pick11.iaxis[AXIS_ACROSS]) {
			haxis = pick11.iaxis[AXIS_DOWN];
			}
		else	{
			haxis = pick11.iaxis[AXIS_ACROSS];
			}
		}
	if (!vaxis) {
		if (!haxis) return(0);
		if (haxis == pick11.iaxis[AXIS_ACROSS]) {
			vaxis = pick11.iaxis[AXIS_DOWN];
			}
		else	{
			vaxis = pick11.iaxis[AXIS_ACROSS];
			}
		}

	/* map */
	if (hzoom) {
		for (iaxis=1; iaxis<VIEW_NAXIS; iaxis++) {
			if (AxisDir(MapAxis(myv->map[iaxis])) == haxis) break;
			}
		map = myv->map[iaxis];
		if (MapFirst(map) > MapLast(map) && 
		    pick11.index[haxis] < pick22.index[haxis]) {
		  SWAP(pick11.index[haxis], pick22.index[haxis]);
		}
		else if (MapFirst(map) < MapLast(map) &&
			 pick11.index[haxis] > pick22.index[haxis]) {
		  SWAP(pick11.index[haxis], pick22.index[haxis]);
		}
		MapSet (map,MapAxis(map),MapSize(map),
		pick11.index[haxis],pick22.index[haxis],
		pick11.index[haxis],pick22.index[haxis],
		MapDmovie(map));
		}
	if (vzoom) {
		for (iaxis=1; iaxis<VIEW_NAXIS; iaxis++) {
			if (AxisDir(MapAxis(myv->map[iaxis])) == vaxis) break;
			}
		map = myv->map[iaxis];
		if (MapFirst(map) > MapLast(map) && 
		    pick11.index[vaxis] < pick22.index[vaxis]) {
		  SWAP(pick11.index[vaxis], pick22.index[vaxis]);
		}
		else if (MapFirst(map) < MapLast(map) &&
			 pick11.index[vaxis] > pick22.index[vaxis]) {
		  SWAP(pick11.index[vaxis], pick22.index[vaxis]);
		}
		MapSet (map,MapAxis(map),MapSize(map),
		pick11.index[vaxis],pick22.index[vaxis],
		pick11.index[vaxis],pick22.index[vaxis],
		MapDmovie(map));
		}

	if (myv->shape == SHAPE_TRUE || myv->shape == SHAPE_PIXEL)do1=1;
}
	SetActiveView(iold);

	/* non-cube shapes require resize */
	if (do1==1) ViewSize (views);
	else DrawFreePixmaps ();
	ViewDrawAll();
	return(0);
}

/* restore initial map */
int ViewWindow0 ()
{
  View myv;
  Map map;
  int iaxis,i,iold,flip;
  
  iold=current_view_num();
  
  for(i=0;i< num_views();i++){
    SetActiveView(i);
    myv=current_view();
    if (!myv) return(0);
    for (iaxis=1; iaxis<VIEW_NAXIS; iaxis++) {
      map = myv->map[iaxis];
      flip = (MapFirst(map) > MapLast(map));
      MapSet (map,MapAxis(map),MapSize(map),0,AxisSize(MapAxis(map))-1,0,AxisSize(MapAxis(map))-1,1);
      if (flip) MapFlip(map);
    }

    /* some shapes require resize */
    if (myv->shape == SHAPE_TRUE || myv->shape == SHAPE_PIXEL) ViewSize (views);
    else DrawFreePixmaps ();
    ViewSetMovie (myv->movie);
  }
  SetActiveView(iold);  
  ViewDrawAll();
  return(0);
}

/* return view map */
Map ViewMap (View view,int index)
{
  if (!view || index>=VIEW_NAXIS) return(0);
  return (view->map[index]);
}


/* return view map data axis */
Axis ViewDataAxis (View view,int imap)
	{
	return (MapAxis(view->map[imap]));
	}

/** supplemental view info **/
void ViewSupplementInfo(string values, string roundRobin, string path, string velocity) {
  int i;
  char buffer[64];
  char* ViewValueScript(View);

  if (num_views() == 1 || views->roundRobinOn)
    sprintf(values, "value=%s", ViewValueScript(current_view()));
  else {
    sprintf(values, "values=(");
    for (i=0; i<num_views(); i++) {
      sprintf(buffer, "%s", ViewValueScript(views->view[i])); strcat(values,buffer);
      if (i<num_views()-1) { sprintf(buffer, ", "); strcat(values,buffer); }
    }
    sprintf(buffer, ")"); strcat(values,buffer);
  }
  if (views->roundRobinOn == 1)
    sprintf(roundRobin, "round-robin on (%d/%d)", views->roundRobinView+1, num_views());
  else sprintf(roundRobin, "round-robin off (%d/%d)", current_view_num()+1, num_views());
  if (PathViewOn()) sprintf(path, "path-view on");
  else sprintf(path, "path-view off");
  if (VelViewNum() > 0) sprintf(velocity, "velocity-view on (%d/2)", VelViewNum());
  else sprintf(velocity, "velocity-view off");
}

/** value at cursor location **/
float ViewValueAtCursor(View view) {
  Map map;
  int index, i;
  float result;
  long cursor;

  if (!view) return(0);
  cursor = 0;
  for (i=AXIS_DOWN; i<=AXIS_5D; i++) {
    map = view->map[i];
    if (map->map == 0) continue;
    index =  MapFrame(map);
    cursor += index*AxisStride(MapAxis(map));
  }

  result = 0;
  if (cursor >= 0 && cursor < view->data->size) {
    result = view->data->buffer[cursor];
    result = DataValue(view->data, result);
  }
  return (result);
}

/** print value **/
char* ViewValueScript(View view) {
  float value;
  value = ViewValueAtCursor(view);
  if (abs(value) > 1)
    sprintf(&view->valueTag[0], "%6.2f", ViewValueAtCursor(view));
  else
    sprintf(&view->valueTag[0], "%6.6f", ViewValueAtCursor(view));
  return (&view->valueTag[0]);
}
  
/* print view information */
void ViewInfo (View view)
	{
	if (!view || view->currentSaved == NULL) return;
	string roundRobin, path, velocity, values;
	ViewSupplementInfo(values, roundRobin, path, velocity);
	sprintf (message,"View: %s, %s, %s, %s, history=%d/%d, wide=%d, hite=%d, movie=%d, style=%d, shape=%d, pick=%d, across=%d, down=%d, delta=%d, fence=%d, rate=%d",
		 values,
		 roundRobin,
		 path,
		 velocity,
		 view->currentSaved->idNum - view->firstSaved->idNum + 1,
		 view->lastSaved->idNum - view->firstSaved->idNum + 1,
		 view->wide,
		 view->hite,
		 view->movie,
		 view->style,
		 view->shape,
		 view->showpicks,
		 view->nacross,
		 view->ndown,
		 view->dacross,
		 view->fence,
		 view->rate);
	UIMessage (message);
	}

/* save view parameters */
void ViewSavePar (View view)
	{
	int imap;

	if (!view) return;
	sprintf (message,"Style: wide=%d hite=%d movie=%d style=%d shape=%d showpicks=%d\n      across=%d down=%d delta=%d fence=%d rate=%d",
		view->wide,
		view->hite,
		view->movie,
		view->style,
		view->shape,
		view->showpicks,
		view->nacross,
		view->ndown,
		view->dacross,
		view->fence,
		view->rate);
	UISaveMessage (message);
	for (imap=1; imap<VIEW_NAXIS; imap++) {
		MapSavePar (view->map[imap]);
		}
	}

/* initial axis orientation */

int FindView(int x, int y){
int i,iold;
View myv;

iold=current_view_num();
for(i=0;i < num_views();i++){
	SetActiveView(i);
	myv=current_view();
	if(x > myv->x0 && x< myv->x0+myv->wide &&
	y > myv->y0 && y< myv->y0+myv->hite)
		return(i);
}
SetActiveView(iold);
return(NO_INDEX);

}

void SetActiveView(int iview){
  views->view[views->cur_view]->active=0;
  views->cur_view=iview;
  views->view[views->cur_view]->active=1;
  ViewInfo(current_view());
}

View current_view(void) {
  return(views->view[current_view_num()]);
}

int current_view_num(void) {
  return(views->cur_view);
}

int num_views() {
  return(views->num_view);
}

int ViewsOrient0 ()
	{
	int i;
	View view;

	for (i=0; i<num_views(); i++) {
	  view = views->view[i];
	  if (!views->roundRobinOn && i != current_view_num()) continue;
	  ViewOrient0(view);
	  ViewSetMovie (view->movie);
	}
	ViewSize0();
	
	ViewDrawAll();
	return(0);
	}

int ViewOrient0(View view) {
  int imap;
  Map_ map_[VIEW_NAXIS];
  Map map[VIEW_NAXIS], temp;

  /* copy map axes */
  for (imap=1; imap<VIEW_NAXIS; imap++) {
    map_[imap] = *view->map[imap];
    map[imap] = &map_[imap];
  }
  /* replace map axes in original order */
  for (imap=1; imap<VIEW_NAXIS; imap++) {
    view->map[imap] = view->map0[imap];
    if (imap == AXIS_DEEP) {
      if (MapFirst(view->map[imap]) == MapLow(view->map[imap]))
	MapFlip(view->map[imap]);
    }
    else {
      if (MapFirst(view->map[imap]) != MapLow(view->map[imap]))
	MapFlip(view->map[imap]);
    }
  }
  return(0);
}


#ifdef SGI
#include <sys/time.h>
double Tyme()
	{
	double tyme;
	struct timeval tp;
	struct timezone tzp;

	gettimeofday (&tp,&tzp);
	tyme = tp.tv_sec;
	tyme = tyme*1000. + tp.tv_usec*.001;
	return (tyme);
	}
#else
#include <sys/types.h>
#include <sys/timeb.h>
double Tyme()
	{
	double tyme;
	struct timeb tp;
/*
	ftime (&tp);
	tyme = tp.time;
	tyme = tyme*1000. + tp.millitm;
*/
	return (tyme);
	}
#endif

double tyme;
int TymeStart (void)
{
  tyme = Tyme();
  return(0);
} 

int TymeEnd (int pixels)
{
  View view;
  view = current_view();
  if (!view || view->currentSaved == NULL) return(0);
  if (pixels == 0 || tyme == 0.0) return(0);
  tyme = Tyme() - tyme;
  string roundRobin, path, velocity, values;
  ViewSupplementInfo(values, roundRobin, path, velocity);
  sprintf (message,"View: %s, %s, %s, %s, history=%d/%d, render=%d, msec pixels=%d, rate=%d pixels/sec",
	   values,
	   roundRobin,
	   path,
	   velocity,
	   view->currentSaved->idNum - view->firstSaved->idNum + 1, 
	   view->lastSaved->idNum - view->firstSaved->idNum + 1,
	   (int)tyme,pixels,
	   (int)(1000.*pixels/tyme));
  UIMessage (message);
  tyme = 0.0;
  return(0); 
}

View ViewFromTag(char *tag){
int i;

i=ViewNumFromTag(tag);
if(i==-1) err("Unable to find view  %s \n",tag);
  return(views->view[i]);
}

int ViewNumFromTag(char *tag){
int i;

for(i=0; i < num_views(); i++)
  if(strcmp(views->view[i]->tag,tag)==0) return(i);

return(-1);
}

int ViewNumFromXY(int x, int y){
int i;


for(i=0; i< num_views(); i++){
	if(x >= views->view[i]->x0 && x <= views->view[i]->x0+views->view[i]->wide &&
	y >= views->view[i]->y0 && y <= views->view[i]->y0+views->view[i]->hite)
		 return(i);
	}
return(-1);
}


/**********************************************************
 * HISTORY LIST
 **********************************************************/

/* debug function */
void ViewHistoryDebugPrint() {
  int i, j;
  View view;
  ViewSaver* create;

  return;

  for (i=0; i<num_views(); i++) {
    view = views->view[i];
    printf("View: %d\n", i);
    for (create = view->firstSaved; create != NULL; create = create->next) {
      printf("ViewSave: (%d) ", create->idNum);

      for (j=AXIS_DOWN; j<=AXIS_5D; j++) {

	printf("%d ", create->frame[i]);
	printf("%d ", create->framePrev[i]);
	printf("%d ", create->frameFirst[i]);
	printf("%d ", create->frameLast[i]);
	printf("%d ", create->frameMFirst[i]);
	printf("%d ", create->frameMLast[i]);
	printf("%d ", create->frameMDelta[i]);
	printf(" | ");
      }
      printf("%d ", create->velocityBuffer);
      printf("%d ", create->velocityBufferParam);
      printf("%d ", create->pathBuffer);
      printf("%d ", create->roundRobin);
      printf("%d ", create->roundRobinView);
      printf("\n");

      /**
      for (j=0; j<DATA_NAXIS; j++)
	printf("%x ", create->map[j]);
      **/

      printf("%d ", create->base);
      printf("%d ", create->color);
      printf("%d ", create->overlay);
      printf("%d ", create->background);
      printf("%d ", create->mark);
      printf("%d ", create->contrast);
      printf("%d ", create->contrast0);
      
      if (create == view->currentSaved)
	printf(" (c) ");
      if (create == view->firstSaved)
	printf(" (f) ");
      if (create == view->lastSaved)
	printf(" (l) ");
      printf("\n");
    }
    printf("--------------------\n");
  }
}

/* load views from file */
int ViewHistoryLoadFile() {
  int i, j, k;
  int size;
  View view;
  ViewSaver *create, *prevCreate, *temp[NUM_VIEWS];
  ViewSaver *current[NUM_VIEWS], *first[NUM_VIEWS], *last[NUM_VIEWS];
  extern FILE* savefd;
  char buffer[1024];
  
  UIMotionOff(NULL);
  if(1 > fscanf(savefd, "%s", buffer)) { // number-of-views
     perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
  }
  if(1 > fscanf(savefd, "%d", &i)) {      // n views
     perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
  }
  if (i != num_views()) {
    UIDelayMessage("History List: number of views and history file incompatible.");
    return(0);
  }

  if(1 > fscanf(savefd, "%s", buffer)) { // begin-view-info
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
  for (i=0; i<num_views(); i++) {
    view = views->view[i];
    prevCreate = NULL;

    if(1 > fscanf(savefd, "%d", &size)) { // volume size
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
    if (size != view->data->size) {
      UIDelayMessage("History List: data size and history file incompatible.");
      return(0);
    }

    while(1) {
      if(1 > fscanf(savefd, "%s", buffer)) { // ViewSave:
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
      NEW(ViewSaver*, create, 1);
      if(1 > fscanf(savefd, "%d", &create->idNum)) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
      for (j=AXIS_DOWN; j<=AXIS_5D; j++) {
	if(1 > fscanf(savefd, "%d", &create->frame[j])) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
	if(1 > fscanf(savefd, "%d", &create->framePrev[j])) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
	if(1 > fscanf(savefd, "%d", &create->frameFirst[j])) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
	if(1 > fscanf(savefd, "%d", &create->frameLast[j])) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
	if(1 > fscanf(savefd, "%d", &create->frameMFirst[j])) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
	if(1 > fscanf(savefd, "%d", &create->frameMLast[j])) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
	if(1 > fscanf(savefd, "%d", &create->frameMDelta[j])) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
      }
      if(1 > fscanf(savefd, "%d", &create->movie)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->style)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->shape)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->fence)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->velocityBuffer)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->velocityBufferParam)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->pathBuffer)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->roundRobin)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->roundRobinView)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      for (j=0; j<DATA_NAXIS; j++) {
	if(1 > fscanf(savefd, "%d", &k)) {
           perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
        }
	create->map[j] = view->map0[k];
      }
      if(1 > fscanf(savefd, "%d", &create->base)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->color)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->overlay)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->background)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->mark)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->contrast)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->contrast0)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }
      if(1 > fscanf(savefd, "%d", &create->polarity)) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
       }

      if(1 > fscanf(savefd, "%s", &(buffer[0]))) {
         perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
      }

      // update pointers
      if (0 == strcmp(buffer,"(c)") || 0 == strcmp(buffer,"(cl)"))
	current[i] = create;
      create->prev = prevCreate;
      if (create->prev == NULL) {
	first[i] = create;
	first[i]->prev = NULL;
      }
      else
	prevCreate->next = create;
      create->next = NULL;
      prevCreate = create;

      // check if last element of history list
      if (0 == strcmp(buffer,"(l)")|| 0 == strcmp(buffer,"(cl)")) {
	last[i] = create;
	last[i]->next = NULL;
	break;
      }
    }

    if(1 > fscanf(savefd, "%s", &(buffer[0]))) { // end-view-info
      perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
    }

    // check if end of file
    if(1 > fscanf(savefd, "%s", &(buffer[0]))) { // end-view-info again?
      perror("ViewHistoryLoadFile ");
     UIDelayMessage("Unable to load History List.");
     return(0);
    }
    if (0 == strcmp(buffer,"end-view-info")) {
      break;
    }
  }

  // go to start and link
  ViewHistoryStart();
  for (i=0; i<num_views(); i++) {
    view = views->view[i];
    SetActiveView(i);
    ViewOrient0(view);
    temp[i] = view->currentSaved->next;
    if (temp[i] != NULL)
      temp[i]->prev = NULL;
    view->currentSaved->next = first[i];
    first[i]->prev = view->currentSaved;
    view->lastSaved = last[i];
    ViewHistoryRenumber(view);
  }
  SetActiveView(0);

  // view is last view handled
  while (view->currentSaved != current[num_views()-1]) {
    ViewHistoryRedo(!DRAW); // redo without drawing
  }

  // clean up memory
  for (i=0; i<num_views(); i++) {
    ViewHistoryDelete(temp[i]);
  }

  // redraw
  UIDrawCanvasMovie(NULL);
  return(1);
}

/* save all views to file */
void ViewHistorySaveFile() {
  View view;
  ViewSaver* current;
  extern FILE* savefd;
  int i, j, k;

  UIMotionOff(NULL);
  DrawWatch(1);

  fprintf(savefd, "number-of-views %d\n", num_views());
  for (i=0; i<num_views(); i++) {
    view = views->view[i];
    
    fprintf(savefd, "begin-view-info\n");
    fprintf(savefd, "%lld\n", view->data->size);
    
    for (current = view->firstSaved; current != NULL; current = current->next) {
      fprintf(savefd, "ViewSave ");
      fprintf(savefd, "%d ", current->idNum-view->firstSaved->idNum+1);
      for (j=AXIS_DOWN; j<=AXIS_5D; j++) {
	fprintf(savefd, "%d ", current->frame[j]);
	fprintf(savefd, "%d ", current->framePrev[j]);
	fprintf(savefd, "%d ", current->frameFirst[j]);

	fprintf(savefd, "%d ", current->frameLast[j]);
	fprintf(savefd, "%d ", current->frameMFirst[j]);
	fprintf(savefd, "%d ", current->frameMLast[j]);
	fprintf(savefd, "%d ", current->frameMDelta[j]);
      }
      fprintf(savefd, "%d ", current->movie);
      fprintf(savefd, "%d ", current->style);
      fprintf(savefd, "%d ", current->shape);
      fprintf(savefd, "%d ", current->fence);
      fprintf(savefd, "%d ", current->velocityBuffer);
      fprintf(savefd, "%d ", current->velocityBufferParam);
      fprintf(savefd, "%d ", current->pathBuffer);
      fprintf(savefd, "%d ", current->roundRobin);
      fprintf(savefd, "%d ", current->roundRobinView);
      for (j=0; j<DATA_NAXIS; j++) {
	for (k=0; k<DATA_NAXIS; k++) {
	  if (view->map0[k] == current->map[j])
	    fprintf(savefd, "%d ", k);
	}
      }
      fprintf(savefd, "%d ", current->base);
      fprintf(savefd, "%d ", current->color);
      fprintf(savefd, "%d ", current->overlay);
      fprintf(savefd, "%d ", current->background);
      fprintf(savefd, "%d ", current->mark);
      fprintf(savefd, "%d ", current->contrast);
      fprintf(savefd, "%d ", current->contrast0);
      fprintf(savefd, "%d ", current->polarity);

      if (current == view->lastSaved && current == view->currentSaved)
	fprintf(savefd, "(cl) \n");
      else if (current == view->lastSaved)
	fprintf(savefd, "(l) \n");
      else if (current == view->currentSaved)
	fprintf(savefd, "(c) \n");
      else
	fprintf(savefd, "(nc) \n");
    }
    fprintf(savefd, "end-view-info\n");
  }

  DrawWatch(0);
}

/* save all views */
void ViewHistorySaveAll() {
  int i, record;
  ViewSaver* create[NUM_VIEWS];
  ViewSaver* current[NUM_VIEWS];

  // save non-empty views in history list
  record = 0;
  for (i=0; i<num_views(); i++) {
    create[i] = ViewHistoryCreateCurrent(views->view[i]);
    current[i] = views->view[i]->currentSaved;
    if (current[i] == NULL) {
      record = 1;
    }
  }
  
  // record = 1 here signifies first time through
  if (record == 0)
    record = !ViewHistoryDuplicateArray(create, current);

  for (i=0; i<num_views(); i++) {
    if (record && !MovieRun() && !UIPathViewAnimateRun() && !UIHistoryAnimateRun())
      ViewHistorySave(views->view[i], create[i]);
    else
      ViewHistoryDelete(create[i]);
  }
  ViewInfo(views->view[0]);
   
  ViewHistoryDebugPrint();
}

/* save view */
void ViewHistorySave(View view, ViewSaver* create) {
  UIHistoryAnimateOff(NULL);

  if (view->currentSaved == NULL) {
    view->currentSaved = create;
    view->firstSaved = view->currentSaved;  // make this the head
    view->lastSaved = view->currentSaved;   // also make this the tail
    view->firstSaved->prev = NULL;
    view->lastSaved->next = NULL;
    view->currentSaved->next = NULL; // no other structs follow yet
    view->currentSaved->prev = NULL; // no other structs before
    view->currentSaved->idNum = 1;
  }
  else {
    // since in middle of list, remove remainder of list
    if (view->currentSaved->next != NULL)
      view->currentSaved->next->prev = NULL;
    ViewHistoryDelete(view->currentSaved->next);

    view->currentSaved->next = create;
    create->prev = view->currentSaved;
    view->currentSaved = create;
    view->lastSaved = create;
    view->lastSaved->idNum = view->lastSaved->prev->idNum+1;
  }
  if (view->lastSaved->idNum - view->firstSaved->idNum >= MAX_HISTORY) {
    view->firstSaved = view->firstSaved->next;
    view->firstSaved->prev->next = NULL;
    ViewHistoryDelete(view->firstSaved->prev);
    view->firstSaved->prev = NULL;
  }
  if (view->lastSaved->idNum >= MAX_IDNUM)
    ViewHistoryRenumber(view);
}

void ViewHistoryRenumber(View view) {
  int i;
  ViewSaver* current;
  for (i=1, current = view->firstSaved; current != NULL; current=current->next, i++) {
    current->idNum = i;
  }
}

/* record current state */
ViewSaver* ViewHistoryCreateCurrent(View view) {
  int i;
  ViewSaver* create;
  extern ColorTable ctable;
  extern Render render;

  NEW(ViewSaver*, create, 1);
  create->prev = NULL;
  create->next = NULL;

  // frames
  for (i=AXIS_DOWN; i<=AXIS_5D; i++) {
    create->frame[i] = MapFrame(view->map[i]);
    create->framePrev[i] = MapPrev(view->map[i]);
    create->frameFirst[i] = MapFirst(view->map[i]);
    create->frameLast[i] = MapLast(view->map[i]);
    create->frameMFirst[i] = MapMovie1(view->map[i]);
    create->frameMLast[i] = MapMovie2(view->map[i]);
    create->frameMDelta[i] = MapDmovie(view->map[i]);
  }
  for (i=0; i<DATA_NAXIS; i++)
    create->map[i] = view->map[i];

  // view parameters
  create->movie = view->movie;
  create->style = view->style;
  create->shape = view->shape;
  create->fence = view->fence;

  // feature parameters
  create->roundRobin = views->roundRobinOn;
  create->roundRobinView = views->roundRobinView;
  create->pathBuffer = PathViewOn();
  create->velocityBuffer = VelViewOn() * VelViewNum();
  create->velocityBufferParam = VelViewOn() * VelViewParam();

  // color
  create->base = ctable->base;
  create->color = ctable->color;
  create->overlay = ctable->overlay;
  create->background = ctable->background;
  create->mark = ctable->mark;
  create->contrast = ctable->contrast;
  create->contrast0 = ctable->contrast0;
  create->polarity = render->polarity;
  return(create);
}

/* load saved state */
void ViewHistoryLoadCurrent(View view) {
  int i, update;
  Map temp;
  extern ColorTable ctable;
  extern Render render;
  if (view->currentSaved == NULL) return;

  // restore round robin
  if (view == views->view[0]) {
    if (views->roundRobinOn != view->currentSaved->roundRobin)
      ViewRoundRobinToggle(!DRAW);
    views->roundRobinView = view->currentSaved->roundRobinView;
  }
  ViewRoundRobinUpdate(!DRAW);

  // restore velocity view
  if (VelViewNum() != view->currentSaved->velocityBuffer)
    VelViewSetBuffer(view->currentSaved->velocityBuffer);
  if (abs(VelViewParam() - view->currentSaved->velocityBufferParam) 
      > BUFFER_PARAM_RESOLUTION)
    VelViewSetParam(view->currentSaved->velocityBufferParam);
  
  // restore path view
  if (PathViewOn() != view->currentSaved->pathBuffer && view == views->view[0])
    PathViewToggle();
  
  // restore maps
  for (i=0; i<DATA_NAXIS; i++)
    view->map[i] = view->currentSaved->map[i];

  // restore view parameters
  view->movie = view->currentSaved->movie;
  view->style = view->currentSaved->style;
  view->fence = view->currentSaved->fence;
  view->shape = view->currentSaved->shape;

  // restore frame bounds
  for (i=AXIS_DOWN; i<=AXIS_5D; i++) {
    MapSet(view->map[i], MapAxis(view->map[i]), MapSize(view->map[i]),
	   view->currentSaved->frameFirst[i], view->currentSaved->frameLast[i],
	   view->currentSaved->frameMFirst[i], view->currentSaved->frameMLast[i],
	   MapDmovie(view->map[i]));
    MapSetFrame(view->map[i], view->currentSaved->frame[i]);
    view->map[i]->prev = MapBound(view->currentSaved->framePrev[i],
				  view->map[i]->first, view->map[i]->last);
  }

  // restore color
  update = 0;
  if (ctable->base != view->currentSaved->base) {
    ctable->base = view->currentSaved->base; update = 1;
  }
  if (ctable->color != view->currentSaved->color) {
    ColorSetChoice(view->currentSaved->color); update = 1;
  }
  if (ctable->overlay != view->currentSaved->overlay) {
    ColorSetOverlay(view->currentSaved->overlay); update = 1;
  }
  if (ctable->background != view->currentSaved->background) {
    ColorSetBackground(view->currentSaved->background); update = 1;
  }
  if (ctable->mark != view->currentSaved->mark) {
    ColorSetMark(view->currentSaved->mark); update = 1;
  }
  if (ctable->contrast != view->currentSaved->contrast) {
    ColorSetContrast(view->currentSaved->contrast); update = 1;
  }
  if (ctable->contrast0 != view->currentSaved->contrast0) {
    ColorSetContrast0(view->currentSaved->contrast0); update = 1;
  }
  if (update) ColorSwitch();
  if (render->polarity != view->currentSaved->polarity) {
    render->polarity = view->currentSaved->polarity; 
    RenderMap();
  }
}

/* update menus */
void ViewHistoryMenus() {
  ViewSaver* current;
  WidgetList list;
  extern UI ui;

  if ((current = current_view()->currentSaved) == NULL) return;
}

/* delete history record */
int ViewHistoryDelete(ViewSaver* start) {
  int nDeleted;
  if (start == NULL) 
    return(0);
  if (start->prev != NULL)
    start->prev->next = NULL;
  nDeleted = ViewHistoryDelete(start->next) + 1;
  FREE(start);
  return (nDeleted);
}

/* check if arrays of records are duplicates */
int ViewHistoryDuplicateArray(ViewSaver* ones[], ViewSaver* twos[]) {
  int i;

  for (i=0; i<num_views(); i++) {
    if (ViewHistoryDuplicate(ones[i], twos[i]) == 0) return(0);
  }
  return(1);
}

/* check if records are duplicates */
int ViewHistoryDuplicate(ViewSaver* one, ViewSaver* two) {
  int i;

  for (i=AXIS_DOWN; i<=AXIS_5D; i++) {
      if (one->frame[i] != two->frame[i]) return(0);
      if (one->framePrev[i] != two->framePrev[i] && one->style != VIEW_ARRAY) return(0);
      if (one->frameFirst[i] != two->frameFirst[i]) return(0);
      if (one->frameLast[i] != two->frameLast[i]) return(0);
      if (one->frameMFirst[i] != two->frameMFirst[i]) return(0);
      if (one->frameMLast[i] != two->frameMLast[i]) return(0);
      if (one->frameMDelta[i] != two->frameMDelta[i]) return(0);
  }
  if (one->movie != two->movie) return(0);
  if (one->style != two->style) return(0);
  if (one->shape != two->shape) return(0);
  if (one->fence != two->fence) return(0);
  if (one->velocityBuffer != two->velocityBuffer) return(0);
  if (one->velocityBufferParam != two->velocityBufferParam) return(0);
  if (one->pathBuffer != two->pathBuffer) return(0);
  if (one->roundRobin != two->roundRobin) return(0);
  if (one->roundRobinView != two->roundRobinView) return(0);
  for (i=0; i<DATA_NAXIS; i++) {
    if (one->map[i] != two->map[i]) return(0);
  }
  if (one->base != two->base) return(0);
  if (one->color != two->color) return(0);
  if (one->overlay != two->overlay) return(0);
  if (one->background != two->background) return(0);
  if (one->mark != two->mark) return(0);
  if (one->contrast != two->contrast) return(0);
  if (one->contrast0 != two->contrast0) return(0);
  if (one->polarity != two->polarity) return(0);
  return(1);
}

/* undo to last state */
void ViewHistoryUndo(int draw) {
  View view;
  int i, updated;
  Map temp;
  
  // update internal states
  updated = 0;
  for (i=0; i<num_views(); i++) {
    view = views->view[i];
    if (view->currentSaved == NULL) continue;
    if (view->currentSaved == view->firstSaved) continue;
    updated = 1;
    view->currentSaved = view->currentSaved->prev;
    ViewHistoryLoadCurrent(view);
  }

  // draw
  if (updated == 1 && draw == DRAW) {
    UIDrawCanvasMovie(NULL);
  }
  
  ViewHistoryDebugPrint();
}

/* redo to next state */
void ViewHistoryRedo(int draw) {
  View view;
  int i, updated;
  Map temp;

  // update internal states
  updated = 0;
  for (i=0; i<num_views(); i++) {
    view = views->view[i];
    if (view->currentSaved == NULL) continue;
    if (view->currentSaved == view->lastSaved) continue;
    updated = 1;
    view->currentSaved = view->currentSaved->next;
    ViewHistoryLoadCurrent(view);
  }

  // draw
  if (updated == 1 && draw == DRAW) {   
    UIDrawCanvasMovie(NULL);
  }

  ViewHistoryDebugPrint();
}

/* at start of history list */
int ViewHistoryAtStart() {
  View view;
  view = current_view();
  return (view->currentSaved == view->firstSaved);
}

/* at end of history list */
int ViewHistoryAtEnd() {
  View view;
  view = current_view();
  return (view->currentSaved == view->lastSaved);
}

/* undo to start */
void ViewHistoryStart() {
  int updated;
  
  DrawWatch(1);
  updated = !ViewHistoryAtStart();
  while (!ViewHistoryAtStart()) {
    ViewHistoryUndo(!DRAW); // undo without drawing
  }

  if (updated == 1)
    UIDrawCanvasMovie(NULL);
  DrawWatch(0);

  ViewHistoryDebugPrint();
}

/* redo to end */
void ViewHistoryEnd() {
  int updated;

  DrawWatch(1);
  updated = !ViewHistoryAtEnd();
  while (!ViewHistoryAtEnd()) {
    ViewHistoryRedo(!DRAW); // redo without drawing
  }

  if (updated == 1) 
    UIDrawCanvasMovie(NULL);
  DrawWatch(0);

  ViewHistoryDebugPrint();
}

/**********************************************************
 * ROUND ROBIN
 **********************************************************/

/* toggle round robin on/off */
void ViewRoundRobinToggle(int draw) {
  int i;
  if (views->roundRobinOn == 0 && num_views() <= 1) return;

  // turn on feature
  else if (views->roundRobinOn == 0) {
    views->roundRobinView = 0;
    if (draw == DRAW) UIMotionOff(NULL);
  }
  
  // turn off feature
  else {
    if (draw == DRAW) {
      UIMotionOff(NULL);
      ViewHistorySaveAll();
      for (i=1; i<num_views(); i++)
	ViewHistoryLoadCurrent(views->view[i]);
    }
  }

  views->roundRobinOn = 1 - views->roundRobinOn;
  ViewRoundRobinUpdate(draw);
}

/* go to next view */
void ViewRoundRobinForward(int draw) {
  views->roundRobinView++;
  if (views->roundRobinView >= num_views())
    views->roundRobinView = 0;
  ViewRoundRobinUpdate(draw);
}

/* go to previous view */
void ViewRoundRobinBackward(int draw) {
  views->roundRobinView--;
  if (views->roundRobinView < 0)
    views->roundRobinView = num_views()-1;
  ViewRoundRobinUpdate(draw);
}

/* update round robin on screen */
void ViewRoundRobinUpdate(int draw) {
  SetActiveView(views->roundRobinView);
  if (draw == DRAW) {
    UIDrawCanvasMovie(NULL);
  }
}

/**********************************************************
 * PATH VIEW
 **********************************************************/

/* view latest path movement */
int ViewPathViewMove() {
  int i1, i2, ind, i, j, paint;
  float coord[3];
  float m, c1, c2, temp;
  View view;
  extern UI ui;

  i = PathViewMoveCoordinates(&coord[0], &coord[1], &coord[2]);
  if (!PathViewOn() || i==NO_INDEX) return(0);
    
  // set frames to match path coordinates for all views
  paint = 1;
  for (i=0; i<num_views(); i++) {
    view = views->view[i];
    for (j=1; j<=3; j++) {
      // calculate index
      i1 = 0;
      i2 = AxisSize(MapAxis(view->map0[j]))-1;
      c1 = AxisFirst(MapAxis(view->map0[j]));
      c2 = AxisLast(MapAxis(view->map0[j]));
      m = ((float)(i2 - i1)) / (c2 - c1);
      ind = (int)(m*(coord[j-1] - c1));

      // check zoom bounds
      i1 = MapFirst(view->map0[j]);
      i2 = MapLast(view->map0[j]);
      if (i1 > i2)
	SWAP(i1, i2);
      if (ind >= i1 && ind <= i2) {
	MapSetFrame(view->map0[j], ind);
      }
      else {
	MapNextFrame(view->map0[j]);
	paint = 0;
      }
    }
  }
  
  // repaint
  if (paint)
    UIDrawCanvasMovie(NULL);
  return(0);
}

void ViewCommands(){
  OutputCommands(views);
}

#endif
