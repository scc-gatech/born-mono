#include <sepConfig.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "rick.h"
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
#ifdef SEPLIB
/*
plot sections and profiles
*/
#include <stdio.h>
#include "rick.h"
extern int sep_put_data_axis_par(const char *tag_history,int *i_axis,int *n,float *o,float *d,const char *label);

#if defined (HAVE_STDLIB_H)
#include<stdlib.h>
#else
extern char *getenv ();
#endif /* _STDLIB_H  */
static int out_coms;
int done_overlay;
char file_name[1024];


void OutputCommands(Views views) {
  View view;
  ViewSaver* current;
  extern FILE* savefd;
  int i, j, k;
  char command[2048];
                                                                                      
                                                                                      
  for (i=0; i<num_views(); i++) {
    view = views->view[i];
    window_data(view,i,command);
    transpose_data(view,command,i);
    if(i!=0 || view->currentSaved->velocityBuffer !=1) display_command(view,command,i);
    fprintf(stderr,"%s\n",command);
  }
  if(views->view[0]->currentSaved->velocityBuffer ==1) {
    sprintf(command,"Merge_field < a0.T vel=a1.T scale=%d >a%d.T \n",
       views->view[0]->currentSaved->velocityBufferParam,num_views());
    display_command(views->view[0],command,num_views());
    fprintf(stderr,"%s\n",command);
  }

}


void window_data(View view, int icom, char *command){
  char data[1024],temp_ch[512];
  int f[5],n[5],i,j,k,b,e;
    

  if(icom==0) strcpy(data,"stdin");
  else{
    sprintf(temp_ch,"data%d",icom);
    if(rick_getch(temp_ch,"s",data)==0) strcpy(data,temp_ch);
  }
    for (j=1; j<DATA_NAXIS; j++) {
        for (k=1; k<DATA_NAXIS; k++) {
          if (view->map0[k] == view->map[j]){
            b=view->currentSaved->frameFirst[j];
            e=view->currentSaved->frameLast[j];
            if(b < e) { 
              f[k-1]=b;
              n[k-1]=e-b+1;
            }
            else{
              f[k-1]=e;
              n[k-1]=b-e+1;
            }
          }
        }
      }


  sprintf(command," < %s Window3d  f1=%d f2=%d f3=%d f4=%d f5=%d n1=%d n2=%d n3=%d n4=%d n5=%d ", data,f[0],f[1],f[2],f[3],f[4],n[0],n[1],n[2],n[3],n[4]);

}

void transpose_data(View view, char *command,int iview)
{
int idata[3],j,k;
char temp_ch[1024];
    strcpy(temp_ch,command);

      for (j=0; j<3; j++) {
        for (k=0; k<DATA_NAXIS; k++) {
          if (view->map0[k] == view->map[j])
           idata[j]=k;
        }
      }
      if(idata[0] < idata[1]){ /*123 132 312*/
        if(idata[1] < idata[2]){ /*123*/
           /*Do nothing*/
              sprintf(command,"%s >a%d.T\n",temp_ch,iview);
        }
        else{ /*132 312*/
          if(idata[0] < idata[2]){ /*132*/
              sprintf(command,"%s| Transp plane=23 maxsize=512 >a%d.T\n",temp_ch,iview);
          }
          else{ /*312*/
              sprintf(command," %s| Transp reshape=2,3 maxsize=512 >a%d.T\n",temp_ch,iview);
          }
        } 
     }
     else{  /*213 231 321*/
        if(idata[0] < idata[2]){ /*213*/
              sprintf(command," %s |Transp plane=12 maxsize=512 >a%d.T\n",temp_ch,iview);
        }
        else{ /*231 or 321*/
          if(idata[1] < idata[2]){ /*231*/
              sprintf(command," %s |Transp reshape=1,3 maxsize=512 >a%d.T\n",temp_ch,iview);
      
          }
          else{ /*321*/
              sprintf(command," %s |Transp plane=13 maxsize=512 >a%d.T\n",temp_ch,iview);
          }
        } 
     }

}

void display_command(View view, char *command,int iview)
{
int flat;
char temp_ch[2048];
strcpy(temp_ch,command);
int f[3],b,e;


if(view->currentSaved->frameFirst[1] < view->currentSaved->frameLast[1]) f[0]=view->currentSaved->frameFirst[1];
else f[0]=view->currentSaved->frameLast[1];

if(view->currentSaved->frameFirst[2] < view->currentSaved->frameLast[2]) f[1]=view->currentSaved->frameFirst[2];
else f[1]=view->currentSaved->frameLast[2];

if(view->currentSaved->frameFirst[3] < view->currentSaved->frameLast[3]) f[2]=view->currentSaved->frameFirst[3];
else f[2]=view->currentSaved->frameLast[3];



f[0]=view->currentSaved->frame[1]-f[0];
f[1]=view->currentSaved->frame[2]-f[1];
f[2]=view->currentSaved->frame[3]-f[2];



if(view->style < 3){
  if(view->style==0) {
     sprintf(command,"%s  < a%d.T Window3d f3=%d |Grey |Tube ",temp_ch,iview,f[2]);
  }
  else if(view->style==1) {
     sprintf(command,"%s < a%d.T  Window3d f2=%d |Grey |Tube ",temp_ch,iview,f[1]);
  }
  else if(view->style==2) {
     sprintf(command,"%s < a%d.T  Window3d f1=%d |Grey |Tube ",temp_ch,iview,f[0]);
  }
}
else{
  if(view->style==3) flat=1;
  if(view->style==4) flat=0;
  sprintf(command,"%s  < a%d.T Cubeplot flat=%d frame1=%d frame2=%d frame3=%d  |Tube",
        temp_ch, iview,flat,f[0],f[1],f[2]);
}

}


int PlotFrontContour ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Contour","|Tube","");
  return(0);
	}

int PlotSideContour ()
	{
	PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"Contour","|Tube","");
  return(0);
	}

int PlotTopContour ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"Contour","|Tube","");
  return(0);
	}

int PlotFrontGrey ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Grey ","|Tube","");
  return(0);
	}

int PlotSideGrey ()
	{
	PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"Grey ","|Tube","");
  return(0);
	}

int PlotTopGrey ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"Grey ","|Tube","");
  return(0);
	}

int PlotFrontWiggle ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Wiggle ","|Tube","");
  return(0);
	}

int PlotSideWiggle ()
	{
	PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"Wiggle ","|Tube","");
  return(0);
	}

int PlotTopWiggle ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"Wiggle ","|Tube","");
  return(0);
	}

int PrintFrontContour ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Contour ","|Pspen","");
  return(0);
	}

int PrintSideContour ()
	{
	PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"Contour ","|Pspen","");
  return(0);
	}

int PrintTopContour ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"Contour ","|Pspen","");
  return(0);
	}

int PrintFrontGrey ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Grey ","|Pspen","");
  return(0);
	}

int PrintSideGrey ()
	{
	PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"Grey ","|Pspen","");
  return(0);
	}

int PrintTopGrey ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"Grey ","|Pspen","");
  return(0);
	}

int PrintFrontWiggle ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Wiggle ","|Pspen","");
  return(0);
	}

int PrintCube ()
	{
	PlotmyCube (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP," Grey eout=1 gainpanel=a | Cubeplot ","|Pspen","");
  return(0);
	}
int PlotCube ()
	{
	PlotmyCube (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Grey eout=1 gainpanel=a | Cubeplot ","|Tube","");
  return(0);
	}
int PrintSideWiggle ()
	{
	PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"Wiggle ","|Pspen","");
  return(0);
	}

int PrintTopWiggle ()
	{
	PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"Wiggle ","|Pspen","");
  return(0);
	}

int PlotView(){
out_coms=0;
BeginView();
return(0);
}

int PlotCom()
{
out_coms=1;
BeginView();
return(0);
}

int BeginView(){
  int i,iold,iview,cube;
  char com1[1024],file[128];
  char file_list[2048];
	View myv;
  extern Views views;



 iold=current_view_num();
strcpy(file_list,"");
for(iview=0; iview<num_views(); iview++){
  SetActiveView(iview); 
	myv=current_view();
  sprintf(file,"%s.%d",myv->data->tag,iview);
  sprintf(com1,"%s.%d.V ",myv->data->tag,iview);
  strcat(file_list,com1);
  cube=0;

  switch(myv->style){

    case VIEW_FRONT:
      PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Grey ",">/usr/tmp/data.V",file);
      break;
    case VIEW_SIDE:
      PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"Grey ",">/usr/tmp/data.V",file);
      break;
    case VIEW_TOP:
      PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"Grey ",">/usr/tmp/data.V",file);
      break;
    case VIEW_THREE:
    case VIEW_CUBE:
     cube=1;
	   PlotmyCube (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"Grey eout=1 gainpanel=a | Cubeplot ",">/usr/tmp/data.V",file);
     break;
   default:
     fprintf(stderr,"Unimplemented plot view \n");
     return(0);
	}
   if(grab_n_ano()>0){
     strcpy(file_name,file);
     done_overlay=0;
/*     UIVplotPanelRaise();*/
     OverlayFinish(cube);
  }
  /*scale the vplot file */
  sprintf(com1,"Vppen xscale=%f yscale=%f < %s.V >c.V vpstyle=n; Cp c.V %s.V", 
    (float)views->view_dim[1],(float)views->view_dim[0],file,file);
  if(out_coms!=1) { if(-1 == system(com1)) perror(com1); }
  else fprintf(stderr,"%s\n",com1);
}
 sprintf(com1,"Vppen gridnum=%d,%d %s >all.V",
     views->view_dim[0],views->view_dim[1],file_list);
 SetActiveView(iold); 
 if(out_coms!=1) { 
   if(-1 == system(com1)) perror(com1);
   if(-1 == system("Stube < all.V")) perror("Stube < all.V");
 }
 else{
   fprintf(stderr,"%s\n",com1);
   fprintf(stderr,"Tube < all.V");
   fflush(stderr);
 }
 return(0);
		
}

int OverlayFinish(int cube)
{
  int i;
  char com1[4096],file[128],com2[4096],com3[4096];
  char sep[3];

	if(out_coms==1) strcpy(sep,"\n");
	else strcpy(sep,";");

  if(cube==1) set_vplot_size(2.29,12.53,1.26,9.11);
  else set_vplot_size(2.29,12.53,1.28,8.94);
 
   strcpy(com1,"");
   AnoDraw(0,SEP_COMMANDS); 
   for(i=0;i < grab_vplot_ncom(); i++){
     if(i==0) strcpy(file,"b1.V");
     else strcpy(file,"a1.V");

     sprintf(com3,"%s >%s %s ",grab_vplot_com(i),file,sep);
     strcat(com1,com3);
     if(i!=0){
        strcpy(com3,"Vppen erase=o <b1.V a1.V >c1.V vpstyle=n; Cp c1.V b1.V;"); 
     	  strcat(com1,com3);
     	  strcat(com1,sep);
			}
   }
    sprintf(com2,"Vppen erase=o < %s.V b1.V >c1.V vpstyle=n;Cp c1.V %s.V;",file_name,file_name);
    strcat(com1,com2);
 	  strcat(com1,sep);
		if(out_coms!=1) {
                   if(-1 == system(com1)) perror(com1);
                }
		else fprintf(stderr,"%s\n",com1);
	return(0);
}


int SaveFront (char *filename)
	{
	PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"save","",filename);
	return(0);
	}

int SaveSide (char *filename)
	{
	PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"save","",filename);
	return(0);
	}

int SaveTop (char *filename)
	{
	PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"save","",filename);
	return(0);
	}

int SaveDeep (char *filename)
	{
	PlotPlane (AXIS_ACROSS,AXIS_DOWN,AXIS_DEEP,"save","",filename);
	return(0);
	}

int SaveAcross (char *filename)
	{
	PlotPlane (-AXIS_DEEP,AXIS_DOWN,AXIS_ACROSS,"save","",filename);
	return(0);
	}

int SaveDown (char *filename)
	{
	PlotPlane (AXIS_ACROSS,AXIS_DEEP,AXIS_DOWN,"save","",filename);
	return(0);
	}

int PlotPlane (int across,int down,int deep,char *program,char *pipeto,char *file)
	{
	float *buffer, hfirst, hlast, vfirst, vlast;
	int n1, n2, vdir, hdir, fd,tempi,i,j;
	float o1,o2,d1,d2;
	char filename[80], command[255], program1[80];
	char temp_ch[2048],temp2_ch[1024];
	View myv;

	myv=current_view();



	vdir = down > 0 ? 1 : -1;
	down = down > 0 ? down : -down;
	hdir = across > 0 ? 1 : -1;
	across = across > 0 ? across : -across;
	/* extract map plane */
	n1 = MapWindow (myv->map[down]);
	n2 = MapWindow (myv->map[across]);
	hfirst = MapValue (myv->map[across],1);
	hlast = MapValue (myv->map[across],MapSize(myv->map[across]));
	vfirst = MapValue (myv->map[down],1);
	vlast = MapValue (myv->map[down],MapSize(myv->map[down]));
	o1= vdir>0?vfirst:vlast;
	d1=vdir*(vlast-vfirst)/(n1-1);
	o2= hdir>0?hfirst:hlast;
	d2= hdir*(hlast-hfirst)/(n2-1);
  if(out_coms!=1){
		NEW (float*,buffer,n1*n2);
		PlotExtractPlane (myv->map[across],
			myv->map[down],
			myv->map[deep],
			myv->map[4],
			myv->map[5],
			buffer,
			n1,
			n2,
			hdir,
			vdir);
	}
	if (!strcmp (program,"save")) {
		strcpy (filename,file);
		auxout(filename);
		}
	else{
		sprintf (filename,"/usr/tmp/Movie%d",getpid());
		if(out_coms!=1) auxout(filename);
		}
	/* plot on screen */
  if(out_coms!=1){
		srite(filename,buffer,n1*n2*sizeof(float));
		tempi=4; auxputch("esize","d",&tempi,filename);
	tempi=1;
	tempi=1;

	sep_put_data_axis_par(filename,&tempi,&n1,&o1,&d1,AxisLabel(MapAxis(myv->map[down])));
	tempi=2;
	sep_put_data_axis_par(filename,&tempi,&n2,&o2,&d2,AxisLabel(MapAxis(myv->map[across])));
	sprintf(temp_ch,"%s :%s", DataTitle(myv->data),
      AxisScript(MapAxis(myv->map[deep]),MapFrame(myv->map[deep])));
	auxputch("title","s",temp_ch,filename);

	auxclose(filename);

   
	if(0!=strcmp(program,"save")){
			sprintf(command,"%s < %s %s &",program,filename,pipeto);
	}
	if(-1 == system (command)) perror(command);
	FREE (buffer);
	}
	else{
    grab_cube_display(temp_ch);
    sprintf(temp2_ch,"| Window f%d=%d n3=1 | Grey >%s.V",MapFrame(myv->map[deep])-1,deep,file);
    strcat(temp_ch,temp2_ch);
    fprintf(stderr,"%s \n",temp_ch);
  }
	return(0);
	}

int PlotmyCube (int across,int down,int deep,char *program,char *pipeto,char *file)
	{
	float *buffer, zfirst, zlast;
	int n3,n1,n2, zdir, fd,f1,f2,f3;
	string filename;
	char command[1024];
	float hfirst,hlast,vfirst,vlast,dfirst,dlast;
	float o1,d1,o2,d2,o3,d3,point1,point2;
	int tempi,vdir,hdir,ddir,ng1,ng2,ng3;
	char temp_ch[2048],temp2_ch[2049];
	int frame1,frame2,frame3,flat,np1,np2,np3,i,j;
	View myv;

	myv=current_view();


	vdir = down > 0 ? 1 : -1;
	down = down > 0 ? down : -down;
	hdir = across > 0 ? 1 : -1;
	across = across > 0 ? across : -across;
	ddir = deep > 0 ? -1 : 1;
	zdir = deep > 0 ? -1 : 1;
	deep = deep > 0 ? deep : -deep;
	/* extract cubeplot */
	n1 = MapWindow (myv->map[down]);
	n2 = MapWindow (myv->map[across]);
	n3 = MapWindow (myv->map[deep]);
	ng1= myv->map[down]->axis->size;
	ng2= myv->map[across]->axis->size;
	ng3= myv->map[deep]->axis->size;
	np1= myv->map[down]->size;
	np2= myv->map[across]->size;
	np3= myv->map[deep]->size;
	point1=(float)(np1)/(float)(np1+np3);
	point2=(float)(np2)/(float)(np2+np3);




	if(vdir>0){ 
		f1=MapFirst(myv->map[down]);
		frame1=myv->map[down]->frame-f1;
	}
	else{
		f1=n1-MapFirst(myv->map[down])-1;
		frame1=myv->map[down]->frame-f1;
	}
	if(hdir>0){
	 	f2=MapFirst(myv->map[across]);
		frame2=myv->map[across]->frame-f2;
	}
	else{ 
		f2=n2-MapFirst(myv->map[across])-1;
		frame2=myv->map[across]->frame-f2;
	}
	if(zdir>0){
		 f3=MapFirst(myv->map[deep]);
		  frame3=myv->map[deep]->frame-f3;
	}
	else{
	 	f3=n3-MapFirst(myv->map[deep])-1;
		frame3=myv->map[deep]->frame-f3;
	}

  switch (myv->style) {
  	case VIEW_CUBE:
		flat=0;
		break;
	  case VIEW_THREE:
		flat=1;
		break;
		default:
		fprintf(stderr,"Not in cube myv \n");
		break;
	}



	if(out_coms!=1){
	NEW (float*,buffer,n1*n2*n3);
	

	PlotExtractCube (myv->map[across],
		myv->map[down],
		myv->map[deep],
		myv->map[4],
		myv->map[5],
		buffer,
		n1,
		n2, n3,
		hdir,
		vdir, ddir);

	if (!strcmp (program,"save")) {
		strcpy (filename,file);
		auxout(filename);
		}
	else	{
		sprintf (filename,"/usr/tmp/Movie%d",getpid());
		auxout(filename);
		}
	}

	hfirst = MapValue (myv->map[across],1);
	hlast = MapValue (myv->map[across],MapSize(myv->map[across]));
	vfirst = MapValue (myv->map[down],1);
	vlast = MapValue (myv->map[down],MapSize(myv->map[down]));
	dfirst = MapValue (myv->map[deep],1);
	dlast = MapValue (myv->map[deep],MapSize(myv->map[down]));

	tempi=1;
	o1= vdir>0?vfirst:vlast;
	d1=vdir*(vlast-vfirst)/(n1-1);
	o2= hdir>0?hfirst:hlast;
	d2= hdir*(hlast-hfirst)/(n2-1);
	o3= ddir>0?dfirst:dlast;
	d3= ddir*(dlast-dfirst)/(n3-1);
	tempi=1;
	if(out_coms!=1){
	tempi=4; auxputch("esize","d",&tempi,filename);
	srite(filename,buffer,n1*n2*n3*sizeof(float));
   tempi=1;
	sep_put_data_axis_par(filename,&tempi,&n1,&o1,&d1,AxisLabel(MapAxis(myv->map[down])));
	tempi=2;
	sep_put_data_axis_par(filename,&tempi,&n2,&o2,&d2,AxisLabel(MapAxis(myv->map[across])));
	tempi=3;
	sep_put_data_axis_par(filename,&tempi,&n3,&o3,&d3,AxisLabel(MapAxis(myv->map[deep])));
	auxputch("title","s",DataTitle(myv->data),filename);
	auxclose(filename);

   
	if(0!=strcmp(program,"save")){
			sprintf(command,"<%s  %s  frame1=%d frame2=%d frame3=%d flat=%d point1=%f point2=%f %s &",filename,program,frame1,frame2,frame3,flat,point1,point2,pipeto);
	}
	if(-1 == system (command)) perror(command);
	FREE (buffer);
	}
	else{
    grab_cube_display(temp_ch);
    sprintf(temp2_ch,"|Cubeplot  frame1=%d frame2=%d frame3=%d flat=%d point1=%f point2=%f  >%s.V",frame1,frame2,frame3,flat,point1,point2,file);
    strcat(temp_ch,temp2_ch);
    fprintf(stderr,"%s \n",temp_ch);
  }
return(0);
}

int PlotExtractCube (Map hmap,Map vmap,Map zmap,Map map4,Map map5,float *buffer,int n1,int n2,int n3,int hdir,int vdir,int zdir)
	{
	Buffer dp;
	register Buffer dp1, de,dp2,da2;
	register float *map, *fp;
	int i2, hinc,dinc,i3,iloc;
	register int vinc;
	View myv;

	myv=current_view();




	iloc=0;
	dp = DataBuffer(myv->data);
	if (hdir > 0) {
		dp += MapFirst(hmap) * AxisStride(MapAxis(hmap));
		iloc += MapFirst(hmap) * AxisStride(MapAxis(hmap));
		}
	else	{
		dp += MapLast(hmap) * AxisStride(MapAxis(hmap));
		iloc += MapLast(hmap) * AxisStride(MapAxis(hmap));
		}
	if (vdir > 0) {
		dp += MapFirst(vmap) * AxisStride(MapAxis(vmap));
		iloc += MapFirst(vmap) * AxisStride(MapAxis(vmap));
		}
	else	{
		dp += MapLast(vmap) * AxisStride(MapAxis(vmap));
		iloc += MapLast(vmap) * AxisStride(MapAxis(vmap));
		}
/*	dp += MapFrame(zmap) * AxisStride(MapAxis(zmap));*/
	dp += MapFrame(map4) * AxisStride(MapAxis(map4));
	dp += MapFrame(map5) * AxisStride(MapAxis(map5));
/*	iloc += MapFrame(zmap) * AxisStride(MapAxis(zmap));*/
	iloc += MapFrame(map4) * AxisStride(MapAxis(map4));
	iloc += MapFrame(map5) * AxisStride(MapAxis(map5));
	map = AxisValues(DataAxis(myv->data,DATA_VALUE));
	fp = buffer;

	dinc = AxisStride(MapAxis(zmap));
	dinc = MapFirst(zmap) < MapLast(zmap) ? dinc : -dinc;
	dinc = zdir > 0 ? dinc : -dinc;

	hinc = AxisStride(MapAxis(hmap));
	hinc = MapFirst(hmap) < MapLast(hmap) ? hinc : -hinc;
	hinc = hdir > 0 ? hinc : -hinc;

	vinc = AxisStride(MapAxis(vmap));
	vinc = MapFirst(vmap) < MapLast(vmap) ? vinc : -vinc;
	vinc = vdir > 0 ? vinc : -vinc;

	da2=dp;
	for (i3=0; i3<n3; i3++) {
		dp=da2;
	for (i2=0; i2<n2; i2++, dp+=hinc) {
		for (dp1=dp, de=dp1+n1*vinc; dp1 != de; dp1+=vinc) {
			*fp++ = map[*dp1-DATA_VALUE_BASE];
			}
		}
		da2+=dinc;
		iloc+=dinc;
	}


return(0);

}


int PlotExtractPlane (Map hmap,Map vmap,Map zmap,Map map4,Map map5,float *buffer,int n1,int n2,int hdir,int vdir)
	{
	Buffer dp;
	register Buffer dp1, de;
	register float *map, *fp;
	int i2, hinc;
	register int vinc;
	View myv;

	myv=current_view();

	dp = DataBuffer(myv->data);
	if (hdir > 0) {
		dp += MapFirst(hmap) * AxisStride(MapAxis(hmap));
		}
	else	{
		dp += MapLast(hmap) * AxisStride(MapAxis(hmap));
		}
	if (vdir > 0) {
		dp += MapFirst(vmap) * AxisStride(MapAxis(vmap));
		}
	else	{
		dp += MapLast(vmap) * AxisStride(MapAxis(vmap));
		}
	dp += MapFrame(zmap) * AxisStride(MapAxis(zmap));
	dp += MapFrame(map4) * AxisStride(MapAxis(map4));
	dp += MapFrame(map5) * AxisStride(MapAxis(map5));
	map = AxisValues(DataAxis(myv->data,DATA_VALUE));
	fp = buffer;
	hinc = AxisStride(MapAxis(hmap));
	hinc = MapFirst(hmap) < MapLast(hmap) ? hinc : -hinc;
	hinc = hdir > 0 ? hinc : -hinc;
	vinc = AxisStride(MapAxis(vmap));
	vinc = MapFirst(vmap) < MapLast(vmap) ? vinc : -vinc;
	vinc = vdir > 0 ? vinc : -vinc;
	for (i2=0; i2<n2; i2++, dp+=hinc) {
		for (dp1=dp, de=dp1+n1*vinc; dp1 != de; dp1+=vinc) {
			*fp++ = map[*dp1-DATA_VALUE_BASE];
			}
		}
	return(0);
	}








int PlotExtractProfile (Map hmap,Map vmap,Map zmap,Map map4,Map map5,float *buffer,int n3,int zdir)
	{
	Buffer dp;
	register float *map, *fp;
	register int i3, zinc;
	View myv;
	myv=current_view();

	dp = DataBuffer(myv->data);
	dp += MapFrame(hmap) * AxisStride(MapAxis(hmap));
	dp += MapFrame(vmap) * AxisStride(MapAxis(vmap));
	if (zdir  > 0) {
		dp += MapFirst(zmap) * AxisStride(MapAxis(zmap));
		}
	else	{
		dp += MapLast(zmap) * AxisStride(MapAxis(zmap));
		}
	dp += MapFrame(map4) * AxisStride(MapAxis(map4));
	dp += MapFrame(map5) * AxisStride(MapAxis(map5));
	map = AxisValues(DataAxis(myv->data,DATA_VALUE));
	fp = buffer;
	zinc = AxisStride(MapAxis(zmap));
	zinc = MapFirst(zmap) < MapLast(zmap) ? zinc : -zinc;
	for (i3=0; i3<n3; i3++, dp+=zinc) {
		*fp++ = map[*dp-DATA_VALUE_BASE];
		}

	return(0);
	}

void grab_cube_display(char *command){
	View myv;
  int i,idir;
  char temp_ch[1024];
  float beg,end,min,max;
  int reverse,i1,i2,i3;
	myv=current_view();
 	 
	/*figure out the dataset */
	sprintf(command,"Window3d < %s ",myv->data->tag);

  reverse=0;
	for(i=1; i < DATA_NAXIS;i++){
    idir=AxisDir(MapAxis(myv->map[i]));
    if(i<4) {
      beg=MapValue(myv->map[i],1);
      end=MapValue(myv->map[i],MapSize(myv->map[i]));
      if(end<beg){ 
        if(i==1) reverse+=1;
        else if(i==2) reverse+=2;
        else reverse+=4;
        min=end; max=beg;
      }
      else {
        max=end; min=beg;
      }
      sprintf(temp_ch,"min%d=%f max%d=%f ",idir,min,idir,max);
      strcat(command,temp_ch);
    }
    else if(MapSize(myv->map[i]) >1){
      sprintf(temp_ch,"f%d=%d n%d=1 ",idir,MapFrame(myv->map[i])-1,idir);
      strcat(command,temp_ch);
		}
	}
  /*reverse any axes*/
	if(reverse==0){
		sprintf(temp_ch,"|Reverse which=%d \n",reverse);
    strcat(command,temp_ch);
  }

  /* transpose the cube */
  i1=AxisDir(MapAxis(myv->map[1]));
  i2=AxisDir(MapAxis(myv->map[2]));
  i3=AxisDir(MapAxis(myv->map[3]));
  /* i2 i1 i3 */
	if(i2 < i1 && i1 < i3){
    sprintf(temp_ch,">Junk.H ;<Junk.H Transp plane=12 ");
    strcat(command,temp_ch);
  }
  /* i1 i3 i2 */
	else if(i1 < i3 && i3 < i2){
    sprintf(temp_ch,"Junk.H ;<Junk.H|Transp plane=23 ");
    strcat(command,temp_ch);
  }
  /* i3 i1 i2 */
	else if(i3 < i1 && i1 < i2){
    sprintf(temp_ch,"Junk.H ;<Junk.H|Transp plane=12 |Transp plane=23 ");
    strcat(command,temp_ch);
  }
  /* i3 i2 i1 */
	else if(i3 < i2 && i2 < i1){
    sprintf(temp_ch,"Junk.H ;<Junk.H|Transp plane=12 |Transp plane=23 |Transp plane=12 ");
    strcat(command,temp_ch);
  }
  /* i2 i3 i1 */
	else if(i2 < i3 && i3 < i1){
    sprintf(temp_ch,"Junk.H ;<Junk.H|Transp plane=23 |Transp plane=12 ");
    strcat(command,temp_ch);
  }
}
#else
int SaveFront(){}
int SaveSide(){}
int SaveDeep(){}
int SaveTop(){}
int SaveAcross(){}
int SaveDown(){}
int PlotFrontGrey(){}
int PlotSideGrey(){}
int PlotTopGrey(){}
int PlotFrontContour(){}
int PlotSideContour(){}
int PlotTopContour(){}
int PlotTopWiggle(){}
int PlotSideWiggle(){}
int PlotFrontWiggle(){}
int PrintFrontContour(){}
int PrintSideContour(){}
int PrintTopContour(){}
int PrintFrontGrey(){}
int PrintSideGrey(){}
int PrintTopGrey(){}
int PrintFrontWiggle(){}
int PrintSideWiggle();
int PrintTopWiggle(){}
int PrintCube(){}
int PlotCube(){}
int PlotView(){}
int PrintSideWiggle(){}
int PlotCom(){}

#endif
#endif
