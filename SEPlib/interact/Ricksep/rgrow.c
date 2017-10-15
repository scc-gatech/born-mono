#include<sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
pick list subroutines
*/

#include <math.h>
#include <stdio.h>
#include "rick.h"
#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif
#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif


int check_add2(float *depth,int *loc,int nind, float *my_depth, float tol,int ishift, int *shift_dep, int shift_ind,int ncor, int nsearch, Buffer myb);
int check_add(float *depth,int *loc,int nind, float *my_depth, float tol,int ishift, int *shift_dep, int shift_ind,int ncor, int nsearch, Buffer myb);
int reset_edge_list(int *edge_list);
int find_edges(int ipos1, int ipos2, int *mye);
void add_point(int ipos1, int ipos2, float my_depth, int *nedge, float *depth, int *edge_list);
void wavekill(float aa, float pp, int n1,int n2, float *input, float *output);
void puck(float *buffer,int m1,int m2,float *s,float *c);

int *mark_map;
float *coher_map;
float max_coher;
int n1,n2;
int min[2],max[2];

void grow_region(float min_tol, float max_tol, float dtol, int nseed, Pik *seeds, int *grown,int *mn, int *mx){

int iter;
float toler,my_depth;
float *local;
int npts,ipt,ntol;
int iedge,nedge;
int idep[3],iind,i,j;
int dep,block[6];
int shift_dep[2],shift_ind;
int ishift;
Buffer myd;
int *edge_list;
int nbuf,nind;
int ncor, nsearch,iax;
View myv;
Message message;


myv=current_view();

max_coher=0.;
min[0]=mn[0]; min[1]=mn[1]; max[0]=mx[0]; max[1]=mx[1];


dep=grab_depend()+1;
ncor=grab_pik_radius();
nsearch=grab_pik_nwind();


/*figure out references into the dataset*/
j=0;
for(i=1; i <= 3 ;i++){
	iax=AxisDir(MapAxis(myv->map[i]));
	if(iax==dep) iind=dep;
	else{
		idep[j]=iax;
		j++;
	}
}



/*figure out shifts*/
block[1]=1;
for(i=2; i <= 5; i++){ block[i]=block[i-1]*AxisSize(myv->data->axis[i-1]);}
shift_ind=block[iind];
shift_dep[0]=block[idep[0]];
shift_dep[1]=block[idep[1]];
/*fprintf(stderr,"check sifts dork %d %d %d %d %d %d \n",iind,idep[0],idep[1],shift_ind,shift_dep[0],shift_dep[1]);*/
n1=AxisSize(myv->data->axis[idep[0]]);
n2=AxisSize(myv->data->axis[idep[1]]);
nind=AxisSize(myv->data->axis[iind]);


/*fprintf(stderr,"first thor \n");*/
/*now figure out if we have an initial offset defined by the 
  non-viewable axes */

ishift=0;
for(i=1; i <= 5; i++){ 
	/*this could be problem in this and ui_panel.  Maybe it shouldn't
     be frame ????*/
	if(i!=iind && i!= idep[0] && i!=idep[1]) { /*non-viewable*/
		ishift+=block[i]*myv->map[i]->frame;
	}
}
myd = DataBuffer(myv->data); /*data pointer */

/*fprintf(stderr,"first thor2\n");*/




/*allocate edge map*/
NEW(int*,mark_map,n1*n2);
NEW(int*,edge_list,2*n1*n2);
NEW(float*,coher_map,n1*n2);
NEW(float*,local,n1*n2);
for(i=0; i< n1*n2; i++) mark_map[i]=0;
for(i=0; i< n1*n2; i++) coher_map[i]=0.;
for(i=0; i< n1*n2; i++) local[i]=-10.;


nedge=0;
/*add initial points in */
for(ipt=0; ipt< nseed; ipt++){
/*	if(seeds[ipt].loc[idep[0]]+n1*seeds[ipt].loc[idep[1]]< 0 ||*/
/*	seeds[ipt].loc[idep[0]]+n1*seeds[ipt].loc[idep[1]]>=n1*n2)*/
/*		err("mark map 1 \n");*/
	mark_map[seeds[ipt].loc[idep[0]]+n1*seeds[ipt].loc[idep[1]]]=2;
}
for(ipt=0; ipt< nseed; ipt++){
	add_point(seeds[ipt].loc[idep[0]],seeds[ipt].loc[idep[1]],seeds[ipt].loc[iind],&nedge,local,edge_list);
}

/*fprintf(stderr,"first thor3 %d\n",nedge);*/

ntol=(max_tol-min_tol)/dtol+1;
/*fprintf(stderr,"se what i am doing %f %f %f %d \n",min_tol,max_tol,dtol,ntol);*/
/*loop over tolerance */
for(iter=0, toler=max_tol; iter <  ntol; iter++,toler=MAX(toler-dtol,min_tol)){
/*fprintf(stderr,"first thor4 %d %d %f \n",iter,nedge,toler);*/


	iedge=0;
	/*loop until we are out of edges */
	while(iedge < nedge){

 		/*check to see if we can add this point */
/*		if(iedge >= n1*n2 || iedge <0) err("edge list problem 1 \n");*/
		if(1==check_add2(local,&edge_list[iedge*2],nind,&my_depth,toler,ishift,shift_dep,shift_ind,ncor,nsearch,myd)){
			/*we can add it*/
/*			fprintf(stderr,"adding point %d %d -%d \n",edge_list[iedge*2],edge_list[iedge*2+1],my_depth);*/
			add_point(edge_list[iedge*2],edge_list[iedge*2+1],my_depth,&nedge,
        local,edge_list);
	}

		else{
			/*we can't add it*/
/*			if(edge_list[iedge*2]+edge_list[iedge*2+1]*n1<0 ||*/
/*        edge_list[iedge*2]+edge_list[iedge*2+1]*n1>=n1*n2)*/
/*				err("mark map  2 error \n");*/
			mark_map[edge_list[iedge*2]+edge_list[iedge*2+1]*n1]=-1;
		}
/*		fprintf(stderr,"check add again %d %d %f \n",edge_list[iedge*2],edge_list[iedge*2+1],local[edge_list[iedge*2]+n1*edge_list[iedge*2+1]]);*/
/*		fprintf(stderr,"2 iedge=%d nedge=%d \n",iedge,nedge);*/
		iedge++;
	}
	/*reset edge list*/
	nedge=reset_edge_list(edge_list);
/*		fprintf(stderr,"4 iedge=%d nedge=%d \n",iedge,nedge);*/
}


/*go back and delete all of the grown points that were initially in the map*/

for(i=0; i< n1*n2; i++) grown[i]=local[i]+.5;
free(local);
free(coher_map);
for(ipt=0; ipt< nseed; ipt++)
	grown[seeds[ipt].loc[idep[0]]+n1*seeds[ipt].loc[idep[1]]]=-1;


	sprintf(message,"Maximum coherence %f \n",max_coher);
  UIMessage (message);
	free(mark_map); free(edge_list);

}

int check_add2(float *depth,int *loc,int nind, float *my_depth, float tol,int ishift, int *shift_dep, int shift_ind, int ncor, int nsearch, Buffer myb){
float loc_depth;
int i1,i2,i,nextra,j,k;
float my_shift[10000],max_shift[10000];
float buffer[10000],buff2[10000],mys,my_max,max_sum,my_avg,auto_avg,auto_pct,my_pct;
int auto_n,my_n,nc;
float slope,coher,f,my_coher[9],my_pick[9],my_var,my_sum;
float slope_norm,coher_norm,my_sum2;
int idepth,m1;

for(i=0; i < nind;i++) my_shift[i]=0; /*initialize to 0 */

nextra=ncor+nsearch;
m1=nextra*2+1;


/*if(mark_map[loc[0]+loc[1]*n1]!=1)*/
/*	err ("PROBLEM %d %d  %d \n",loc[0],loc[1],mark_map[loc[0]+loc[1]*n1]);*/


/*loop nearby points */
i=0; nc=0;
for(i=0; i < nind; i++){
	max_shift[i]=0.;
	my_shift[i]=0.;
}
my_max=0.;
for(i2=loc[1]-1; i2<=loc[1]+1; i2++){
	if(i2>=0 && i2 < n2){
		for(i1=loc[0]-1; i1<=loc[0]+1; i1++){
			if(i1>=0 && i1 < n1){
/*         if(i1+i2*n1<0 || i1+i2*n1>= n1*n2) err("mark map 3 \n");*/
/*				fprintf(stderr,"check map mark %d %d %d \n",i1,i2,mark_map[i1+i2*n1]);*/
				if(mark_map[i1+i2*n1]==2){
					/*we have a nearby point with a selected value */
				  loc_depth=depth[i1+i2*n1];
				  idepth=loc_depth+.5;
/*					if(idepth<1.) err("BAD depth %d %d %d %f \n",i1,i2,i1+i2*n1,loc_depth);*/
/*					fprintf(stderr,"check this %d %d %d %f \n",i1,i2,idepth,loc_depth);*/
						

					for(i=idepth-ncor-nsearch,j=0; i<=idepth+ncor+nsearch;i++,j++){
						if(i<0 || i> nind){
							 buffer[j]=0.;
							 buffer[j+m1]=0.;
							 buff2[j]=0.;
							 buff2[j+m1]=0.;
						}
						else{
					   /*create correlation array for nearby */
						 buffer[j]=
              (float)myb[ishift+shift_dep[0]*i1+shift_dep[1]*i2+shift_ind*i];
					   /*create correlation array for given */
							 buffer[j+m1]=
              (float)myb[ishift+shift_dep[0]*loc[0]+
                  shift_dep[1]*loc[1]+shift_ind*i];
					   /*create standardization */
/*						 buff2[j]=buffer[j];*/
/*						 buff2[j+m1]=(buff2[j]+buffer[j+m1])/2.;*/
					  }
					}
					puck(buffer,m1,2,&slope,&coher);
/*					puck(buff2,m1,2,&slope_norm,&coher_norm);*/
/*					fprintf(stderr,"check me %d %d %f %f %f %f \n",loc[0],loc[1],slope_norm,coher_norm,slope,coher);*/
          slope+=loc_depth;
					my_coher[nc]=coher;
					my_pick[nc]=slope;
					nc+=1;
				}
			}
		}
	}
}
if(nc<1) err("nc ==0  %d %d makes no sense \n",loc[0],loc[1]);
if(nc==1){
	my_var=1.-my_coher[0];
	my_max=my_pick[0];
}
else   {
	my_max=0.;j=0; my_var=0;my_sum=0.;my_sum2=0.;
	for(i=0; i < nc; i++){
		my_max+=my_pick[i]*my_coher[i];
		my_sum+=my_coher[i];
	}
	my_max=my_max/my_sum;
/*	fprintf(stderr,"COMPARE %f",my_max);*/
	for(i=0; i < nc; i++){
		my_sum+=my_coher[i]*my_coher[i];
		my_var+=((my_pick[i]-my_max)*my_coher[i])*((my_pick[i]-my_max)*my_coher[i]);
/*	  fprintf(stderr," %f",my_pick[i]);*/
	}
	my_var=(float) sqrt(my_var/my_sum)*nc/my_sum;
/*	fprintf(stderr,"\n");*/
}

coher_map[loc[0]+loc[1]*n1]=1.-my_var;
max_coher=MAX(max_coher,1.-my_var);




*my_depth=my_max;
/*fprintf(stderr,"check i1=%d i2=%d nc=%d loc_depth=%f my_depth=%f coher=%f tol=%f \n",*/
/* loc[0],loc[1],nc,loc_depth,my_max,1.-my_var,tol);*/
if(1.-my_var > tol) return(1);
else return(0);
}
int check_add(float *depth,int *loc,int nind, float *my_depth, float tol,int ishift, int *shift_dep, int shift_ind, int ncor, int nsearch, Buffer myb){
float loc_depth;
int idepth;
int i1,i2,i,nextra,j,k;
float my_shift[10000],max_shift[10000];
float cor1[10000],cor2[10000],mys,my_max,max_sum,my_avg,auto_avg,auto_pct,my_pct;
int auto_n,my_n;

for(i=0; i < nind;i++) my_shift[i]=0; /*initialize to 0 */

nextra=ncor+nsearch;


/*if(mark_map[loc[0]+loc[1]*n1]!=1)*/
/*	err ("PROBLEM %d %d  %d \n",loc[0],loc[1],mark_map[loc[0]+loc[1]*n1]);*/


/*loop nearby points */
for(i=0; i < 9; i++) depth[i]=-1;
i=0;
for(i=0; i < nind; i++){
	max_shift[i]=0.;
	my_shift[i]=0.;
}
for(i2=loc[1]-1; i2<=loc[1]+1; i2++){
	if(i2>=0 && i2 < n2){
		for(i1=loc[0]-1; i1<=loc[0]+1; i1++){
			if(i1>=0 && i1 < n1){
/*				fprintf(stderr,"check map mark %d %d %d \n",i1,i2,mark_map[i1+i2*n1]);*/
				if(mark_map[i1+i2*n1]==2){
					/*we have a nearby point with a selected value */
				  loc_depth=depth[i1+i2*n1];
					idepth=loc_depth+.5;
					if(idepth<1) err("BAD depth %d %d %d %f \n",i1,i2,i1+i2*n1,loc_depth);
						
					/*create correlation array for nearby */
					for(i=idepth-ncor-nsearch,j=0; i<=idepth+ncor+nsearch;i++,j++)
						if(i<0 || i>=nind) cor1[j]=0.;
						cor1[j]=
              (float)myb[ishift+shift_dep[0]*i1+shift_dep[1]*i2+shift_ind*i];

					/*create correlation array for given */
					for(i=idepth-ncor-nsearch,j=0; i<=idepth+ncor+nsearch;i++,j++){
						if(i<0 || i>=nind) cor2[j]=0.;
						else
						cor2[j]=
              (float)myb[ishift+shift_dep[0]*loc[0]+
                  shift_dep[1]*loc[1]+shift_ind*i];
					}
					/*correlate*/
					for(i=idepth-nsearch,j=-nsearch,k=0;i<=idepth+nsearch; k++,i++,j++){
						my_shift[i]+=wshspickrc(cor1,cor2,nextra,nextra+j,
              1+2*nextra,ncor);
						max_shift[k]+=wshspickrc(cor1,cor1,nextra,nextra+j,
              1+2*nextra,ncor);
					}
				}
			}
		}
	}
}

/*standardize correlation and find max*/
mys=.0001; j=0; my_max=-10.;max_sum=.0001;
auto_n=1; my_n=1;

for(i=0; i< nsearch*2+1;i++) if(max_shift[i]>0.001){auto_n+=1; max_sum+=max_shift[i];}

for(i=0; i < nind; i++){
	if(my_shift[i]>0.001){ mys+=my_shift[i]; my_n++;}
	if(my_shift[i]>my_max){
		my_max=my_shift[i];j=i;
	}
}
*my_depth=j;
/*check criteria*/
my_avg=mys/auto_n;/*so a big spread doesn't help */
auto_avg=max_sum/auto_n;
auto_pct=(max_shift[nsearch]-auto_avg)/auto_avg;
my_pct=(my_max-my_avg)/my_avg;


/*fprintf(stderr,"check my check %d %d (%f -- %f) %d %d [%f-%f]\n",loc[0],loc[1],my_pct,auto_pct,j,loc_depth,my_pct/auto_pct,tol);*/
if(my_pct/auto_pct>tol && my_pct>0.) return(1);
else return(0);

}

int reset_edge_list(int *edge_list){
int i,i1,i2;
int nedge,mye[16],nfound;
int *temp_mask;

for(i=0;i< n1*n2;i++){
	if(mark_map[i] <0) mark_map[i]=0;
}


nedge=0;
for(i2=0;i2 < n2;i2++){
	for(i1=0;i1 < n1;i1++){
/*    if(i1+i2*n1<0 || i1+i2*n1>=n1*n2) err("mark map4 \n");*/
		if(mark_map[i1+i2*n1]==2){
			nfound=find_edges(i1,i2,mye);
			if(nfound>0){
				for(i=0; i< nfound; i++){
/*					if(nedge <0 || nedge >=n1*n2) err("edge list problem 2 \n");*/
					edge_list[nedge*2]=mye[i*2];
					edge_list[nedge*2+1]=mye[i*2+1];
/*    if(mye[i*2]+mye[i*2+1]*n1<0 || mye[i*2]+mye[i*2+1]*n1>=n1*n2) err("mark map5 \n");*/
					mark_map[mye[i*2]+mye[i*2+1]*n1]=1;
					nedge++;
				}
			}
		}
	}
}
/*fprintf(stderr,"finished reset \n");*/
return(nedge);
				 
}
int find_edges(int ipos1, int ipos2, int *mye){
int nedge;
int i1,i2;

nedge=0;
for(i2=ipos2-1; i2 <=ipos2+ 1; i2++){
	if(i2 >=min[1] && i2 <= max[1]){
		for(i1=ipos1-1; i1 <= ipos1+1;i1++){
			if(i1 >= min[0] && i1 <= max[0]){
				if(mark_map[i1+i2*n1]==0){
					mye[2*nedge]=i1;
					mye[1+2*nedge]=i2;
					nedge++;
/*					fprintf(stderr,"adding edge %d %d \n",i1,i2);*/
				}
			}
		}
	}
}


return(nedge);
}
			






void add_point(int ipos1, int ipos2, float my_depth, int *nedge,
     float *depth, int *edge_list){
int ned,ipos,nnew;	
int mye[16];
int i;

ned=*nedge;
ipos=ipos1+n1*ipos2;
mark_map[ipos]=2;

depth[ipos]=my_depth;
nnew=find_edges(ipos1,ipos2,mye);
/*fprintf(stderr,"just added depth %d %d %d %f \n",ipos,ipos2,ipos,depth[ipos]);*/
/*fprintf(stderr,"ned=%d found edges arround %d %d %d \n",ned,ipos,ipos1,ipos2);*/
if(nnew>0){
	for(i=0; i< nnew; i++){
		edge_list[ned*2]=mye[i*2];
		edge_list[ned*2+1]=mye[i*2+1];
/*		fprintf(stderr,"add edge (%d %d) %d=iedge %d %d \n",ipos1,ipos2,ned,edge_list[ned*2],edge_list[ned*2+1]);*/
/*		if(mye[i*2]+mye[i*2+1]*n1<0 || mye[i*2]+mye[i*2+1]*n1>=n1*n2)*/
/*			err("going beyond mark_mark 23 \n");*/
		mark_map[mye[i*2]+mye[i*2+1]*n1]=1;
		ned++;
	}
}
*nedge=ned;
/*fprintf(stderr,"just added dept2 %d %d %d %f \n",ipos,ipos2,ipos,depth[ipos]);*/
}

void puck(float *buffer,int m1,int m2,float *s,float *c)
{
float slope,coher;
float dx[10000],dt[10000];
float xx,xt,tt;
int i1,i2;

/*srite("co.H",buffer,m1*8);*/

wavekill(1.,0.,m1,m2,buffer,dx);
wavekill(0.,1.,m1,m2,buffer,dt);
xx=1.e-30;tt=1.e-30;xt=0.;
for(i1=0;i1 < m1-1; i1++){
	for(i2=0;i2 < m2-1; i2++){
		xt+=dt[i1+m1*i2]*dx[i1+m1*i2];
		tt+=dt[i1+m1*i2]*dt[i1+m1*i2];
		xx+=dx[i1+m1*i2]*dx[i1+m1*i2];
	}
}
*c = (float) sqrt((xt/tt)*(xt/xx));
*s  =  - xt/tt;
/*fprintf(stderr,"check these xt=%f tt=%f xx=%f c=%f s=%f \n",*/
/*  xt,tt,xx,*c,*s);*/
}

void wavekill(float aa, float pp, int m1,int m2, float *input, float *output){
int i1,i2;
float s11,s12,s21,s22;
s11 = -aa-pp; s12 = aa-pp;
s21 = -aa+pp; s22 = aa+pp;
for(i1= 0; i1<m1-1;i1++){ /* output is one point shorter than uu on both axes.*/
for(i2= 0; i2<m2-1;i2++){ 
  output[i1+m1*i2] =
    input[i1 +m1*i2] * s11 + input[i1+m1*(  i2+1)] * s12 +
    input[i1+1+m1*i2] * s21 + input[i1+1+m1*(i2+1)] * s22;
  }}
}



#endif
