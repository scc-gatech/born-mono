/*<



}

semblance and nmo in c

*/


#include<sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
#ifdef SEPLIB
int create_vel(int *location, float *velocity);
int compute_lin_pars(float *data,float missing,int ndat,int nskip,int *ib, int *ie,float *frac);
void rite_vel();
void compute_semblance();
void linear_semb();
void compute_semb();
void quick_nmo();
float sqroot (float r, float t0, float epsilon);
void draw_vel_line(int mode,float *myv);
void calc_int_vel();

#include <rick.h> 
#include <math.h>
extern int sep_put_data_axis_par(const char *tag_history,int *i_axis,int *n,float *o,float *d,const char *label);
int oversample;
float *data_buf,*nmo_buf;
float *semb_buf;
int current_loc[3],old_loc[3];
int n1_semb,n2_semb,nv,ntout,nsmooth;
float o1,o2,ov;
float d1,d2,dv,smute;
float *num, *den, *nnz,*sem_small,*vel,*intvel;
float *tgain,*tgain_sem,scaled,biasd;
int first;
int *ib1,*ie1;
float *frac1;
float ignore;
int no_sem;
Data data,nmoed,vscan;
int max_wide;
int data_view_num,nmoed_view_num,vscan_view_num;

void semb_init()
{ 
int i;
int ndata;
View myv;


data=DataFromTag("in");
vscan=DataFromTag("vscan");
nmoed=DataFromTag("nmoed");

if(0==rick_getch("oversample","d",&oversample)) oversample=10;
if(0==rick_getch("ignore","f",&ignore)) ignore=0.;
if(0==rick_getch("smute","f",&smute)) smute=1.5;
current_loc[0]=-1;
current_loc[0]=-2;
current_loc[0]=-3;


myv=ViewFromTag("in");
myv->change_loc_func=change_loc;
myv->close_view_func=rite_vel;
myv=ViewFromTag("vscan");
myv->data->pik->change_pik_func=update_nmo;


data_view_num=ViewNumFromTag("in");
nmoed_view_num=ViewNumFromTag("nmoed");
vscan_view_num=ViewNumFromTag("vscan");




if (0==rick_getch("nsmooth","d",&nsmooth)) nsmooth = oversample*2+1;
n1_semb=AxisSize(data->axis[1]);
n2_semb=AxisSize(data->axis[2]);
nv=AxisSize(vscan->axis[2]);
o1=AxisFirst(data->axis[1]);
o2=AxisFirst(data->axis[2]);
ov=AxisFirst(vscan->axis[2]);
d1=AxisDelta(data->axis[1]);
d2=AxisDelta(data->axis[2]);
dv=AxisDelta(vscan->axis[2]);

ntout = 1+(n1_semb-1)/oversample;   
if (0==rick_getch("nsmooth","d",&nsmooth)) nsmooth = oversample*2+1;;
if (0==rick_getch("no_sem","d",&no_sem)) no_sem= 0;


NEW(float*,nmo_buf,n1_semb*n2_semb);
NEW(float*,data_buf,n1_semb*n2_semb);
NEW(float*,semb_buf,n1_semb*nv);
NEW(float*,num,n1_semb*nv);
NEW(float*,den,n1_semb*nv);
NEW(float*,nnz,n1_semb*nv);
NEW(float*,sem_small,ntout*nv);
NEW(float*,tgain,n1_semb);
NEW(float*,tgain_sem,n1_semb);
NEW(float*,vel,n1_semb);
NEW(float*,intvel,n1_semb);
NEW(int*,ib1,n1_semb);
NEW(int*,ie1,n1_semb);
NEW(float*,frac1,n1_semb);
first=1;

for(i=0; i < n1_semb; i++){
	 tgain_sem[i]=1.;
	 tgain[i]=1.;
}



change_loc();
first=0;

}




void change_loc(){
int i1,i2;
Buffer dp;
register Buffer dp1, de;
register float *map, *fp;
int  hinc;
register int vinc;
int ipt,i;
int icur_view;
PickPoint_ pick;
View myv;

/*
*/



if(first!=1){
icur_view=current_view_num();
SetActiveView(data_view_num);
myv=current_view();

for(i=1;i< DATA_NAXIS; i++){
	pick.index[AxisDir(MapAxis(myv->map[i]))]=MapFrame(myv->map[i]);
}
SetActiveView(icur_view);
myv=current_view();
}
else{
for(i=1;i< DATA_NAXIS; i++){
	pick.index[i]=0;
}
}




/*location has changed so read in a new cmp panel */
if(current_loc[0]!=pick.index[3] || current_loc[1]!=pick.index[4] || current_loc[2]!=pick.index[5]){

dp = DataBuffer(data);
dp += pick.index[3] * AxisStride(data->axis[3]);
dp += pick.index[4] * AxisStride(data->axis[4]);
dp += pick.index[5] * AxisStride(data->axis[5]);
map = AxisValues(DataAxis(data,DATA_VALUE));
fp = data_buf;
hinc=AxisStride(data->axis[2]);
vinc=AxisStride(data->axis[1]);



 for (i2=0; i2<n2_semb; i2++, dp+=hinc) {
    for (dp1=dp, de=dp1+n1_semb*vinc; dp1 != de; dp1+=vinc) {
      *fp++ = map[*dp1-DATA_VALUE_BASE];
      }
    }
	old_loc[0]=current_loc[0];
	old_loc[1]=current_loc[1];
	old_loc[2]=current_loc[2];
	current_loc[0]=pick.index[3];
	current_loc[1]=pick.index[4];
	current_loc[2]=pick.index[5];

	compute_semblance();
	update_nmo();
	old_loc[0]=current_loc[0];
	old_loc[1]=current_loc[1];
	old_loc[2]=current_loc[2];

/*
if(first!=1){
	ViewDrawAll();
}
*/

}

  SetActiveView(vscan_view_num);

}


void update_nmo(){
float scal,bia;
int i2,ipt;
float *tempb;
int max_hite;
int firsty;

firsty=first;


if(firsty!=1) {
draw_vel_line(ERASE,vel);
draw_vel_line(ERASE,intvel);
 UICanvasSize (&max_wide,&max_hite);
}



create_vel(current_loc,vel);
quick_nmo();
;
if(firsty==1){
	NEW(float*,tempb,n1_semb*n2_semb); memcpy((void *)tempb,(const void*)nmo_buf,n1_semb*n2_semb*sizeof(nmo_buf[0]));
	compute_gain_pars(nmoed,tempb,n2_semb*n1_semb,&scal,&bia);scaled=scal;biasd=bia;
	free(tempb);
}

for(i2=0, ipt=0; i2 < n2_semb; i2++, ipt+=n1_semb)
float_to_byte((nmo_buf+ipt),(nmoed->buffer+ipt),n1_semb,tgain_sem,biasd,scaled,nmoed->value_size);

if(firsty!=1){
	 SetActiveView(ViewNumFromTag("nmoed"));
	 ViewDraw(ViewFromTag("nmoed"),DRAW_ALL);
		RenderDraw();
}

draw_vel_line(DRAW,vel);
calc_int_vel();
draw_vel_line(DRAW,intvel);

}


/*look through the pick file for velocity marks, linear interpolate between*/
int create_vel(int *location, float *velocity){
PikList pik;
int i2,i,ib,ie;
float frac;

pik=vscan->pik;
for(i=0; i < n1_semb; i++) velocity[i]=-9000.;



for(i2=0; i2 < pik->npik; i2++){
	if(pik->pik[i2].loc[3]==location[0] && pik->pik[i2].loc[4]==location[1] && pik->pik[i2].loc[5]==location[2]){ /*we are on the cmp*/
		velocity[(int)(pik->pik[i2].loc[1])]=pik->pik[i2].loc[2]*dv+ov;
	}
}
if(0==compute_lin_pars(velocity,-9000.,n1_semb,1,ib1,ie1,frac1)){
	for(i=0; i < n1_semb; i++){
		velocity[i]=velocity[ib1[i]]*(1.-frac1[i])+velocity[ie1[i]]*frac1[i];
	}
}
else { 
	for(i=0; i < n1_semb; i++) velocity[i]=ov+dv*nv/2.; /*ov+dv* nv/2.;*/
	return(-1);
}
return(0);
}

int compute_lin_pars(float *data,float missing,int ndat,int nskip,int *ib, int *ie,float *frac){
int ibt, iet,i;

ibt=-1;i=0;
while(i<ndat && ibt==-1){ if(data[i*nskip]!=missing) ibt=i; else i++;}
if(i!=ndat){ /*we have it least one sample */
	for(i=0; i<= ibt; i++){ ib[i]=ibt; frac[i]=1.; ie[i]=ibt; }

	i=ibt+1;	
	while(i< ndat){ iet=0;
		while(i<ndat && iet==0){ if(data[nskip*i]!=-9000) iet=i; else i++;}
		if(iet!=0){
			for(i=ibt; i < iet; i++){ 
				ib[i]=ibt; ie[i]=iet; 
				frac[i]=(float)(i-ibt)/(float)(iet-ibt);
			}
			ibt=iet; iet=0; i=ibt+1;
		}
	}
	for(i=ibt; i < ndat; i++){ ib[i]=ibt; frac[i]=1.; ie[i]=ibt;}

}
else { return(-1);}

return(0);
}

void rite_vel(){
int i,j,i3,i2,i4,i5,n3,n4,n5;
int n;
float o,d;
string label;
float *bigvel;
int myloc[3],ibig;
int *ib,*ie;
float *frac;

i=1;
sep_put_data_axis_par("velocity",&i,&n1_semb,&o1,&d1,"time");
for(i=3; i <=5; i++){
	j=i-1;
	n=AxisSize(data->axis[i]);
	o=AxisFirst(data->axis[i]);
	d=AxisDelta(data->axis[i]);
	strcpy(label,AxisLabel(data->axis[i]));
 sep_put_data_axis_par("velocity",&j,&n,&o,&d,label);
}
n3=AxisSize(data->axis[3]);
if(data_is_2d(data)) n3=1;
n4=AxisSize(data->axis[4]);
n5=AxisSize(data->axis[5]);
NEW(float*,bigvel,n1_semb*n3*n4*n5);

ibig=0;
for(i5=0; i5 <  n5; i5++){ myloc[2]=i5;
	for(i4=0; i4 < n4 ; i4++){myloc[1]=i4;
	for(i3=0; i3 < n3; i3++){myloc[0]=i3;
		if(0!=create_vel(myloc,	(bigvel+ibig))){
			for(i=0; i < n1_semb;i++) bigvel[ibig+i]=-9000.;
		}
		ibig+=n1_semb;
	}
	}
}


if(n3>1){ /*do linear interpolation along the output 2 axis */
NEW(int*,ie,n3);
NEW(int*,ib,n3);
NEW(float*,frac,n3);
for(i5=0; i5 < n5; i5++){ 
	for(i4=0; i4 < n4; i4++){ 
		if(0==compute_lin_pars((bigvel+i4*n3*n1_semb),-9000.,n3,n1_semb,ib,ie,frac)){
			for(i3=0; i3 <n3; i3++){
				for(i=0; i <n1_semb; i++){
					bigvel[i+i3*n1_semb+i4*n1_semb*n3+i5*n1_semb*n3*n4]=
					bigvel[i+ib[i3]*n1_semb+i4*n1_semb*n3+i5*n1_semb*n3*n4]*(1.-frac[i3])+
					bigvel[i+ie[i3]*n1_semb+i4*n1_semb*n3+i5*n1_semb*n3*n4]*frac[i3];
				}
			}
		}
	}
}
free(ie); free(ib); free(frac);
}
		



if(n4>1){ /*do linear interpolation along the output 2 axis */
NEW(int*,ie,n4);
NEW(int*,ib,n4);
NEW(float*,frac,n4);
for(i5=0; i5 < n5; i5++){ 
	if(0==compute_lin_pars((bigvel+i5*n1_semb*n3*n4),-9000.,n4,n1_semb*n3,ib,ie,frac)){
		for(i4=0; i4 < n4; i4++){ 
			for(i3=0; i3 <n3; i3++){
				for(i=0; i <n1_semb; i++){
					bigvel[i+i3*n1_semb+i4*n1_semb*n3+i5*n1_semb*n3*n4]=
					bigvel[i+i3*n1_semb+ib[i4]*n1_semb*n3+i5*n1_semb*n3*n4]*(1.-frac[i4])+
					bigvel[i+i3*n1_semb+ie[i4]*n1_semb*n3+i5*n1_semb*n3*n4]*frac[i4];
				}
			}
		}
	}
}
free(ie); free(ib); free(frac);
}
if(n5>1){
NEW(int*,ie,n5);
NEW(int*,ib,n5);
NEW(float*,frac,n4);
if(0==compute_lin_pars(bigvel,-9000.,n5,n1_semb*n3*n4,ib,ie,frac)){
	for(i5=0; i5 < n5; i5++){ 
		for(i4=0; i4 < n4; i4++){ 
			for(i3=0; i3 <n3; i3++){
				for(i=0; i <n1_semb; i++){
					bigvel[i+i3*n1_semb+i4*n1_semb*n3+i5*n1_semb*n3*n4]=
					bigvel[i+i3*n1_semb+i4*n1_semb*n3+ib[i5]*n1_semb*n3*n4]*(1.-frac[i5])+
					bigvel[i+i3*n1_semb+i4*n1_semb*n3+ie[i5]*n1_semb*n3*n4]*frac[i5];
				}
			}
		}
	}
}
free(ie); free(ib); free(frac);
}

srite("velocity",bigvel,n1_semb*n3*n4*n5*4);
free(bigvel);

}


      



void compute_semblance(){
float scal,bia;
register float scale,bias;
int i2,ipt;
float *tempb;


compute_semb(); /*first compute it on the small grid */
linear_semb(); /*linear interpolate it to the larger grid*/
NEW(float*,tempb,n1_semb*nv); 
memcpy((void *)tempb,(const void*)semb_buf,n1_semb*nv*4);
compute_gain_pars(vscan,tempb,nv*n1_semb,&scal,&bia);scale=scal;bias=bia;
free(tempb);
for(i2=0, ipt=0; i2 < nv; i2++, ipt+=n1_semb)
float_to_byte((semb_buf+ipt),(vscan->buffer+ipt),n1_semb,tgain_sem,bias,scale,vscan->value_size);





}

void linear_semb(){
int i,j,iloc,i2,is,i1;
float f;


iloc=0; is=0;
for(i2=0; i2 < nv; i2++){ /* loop over traces */
	i1=0;
	for(j=0; j< ntout-1; j++){ /* loop over output space */
		for(i=0; i < oversample && i+ j*oversample < n1_semb;i++){/* loop over sampling  */
			f=(float)i/(float)oversample;
			semb_buf[iloc]=(1.-f)*sem_small[is]+f*sem_small[is+1];
			iloc+=1;
			i1+=1;
		}
		is+=1;
	}
	if(i1!=n1_semb){
		for(i=i1; i < n1_semb; i++, iloc++){ 
			semb_buf[iloc]=sem_small[is];	
		}
		is+=1;
	}
}



}


/*suvelan Hale??*/
void compute_semb(){
int iv,it,ismin,ismax,itout,i2,itmute,iti,is;
float nsum, dsum,v,offovs,offset;
float tnmute,fracc,tn,ti,frac,temp;

if(no_sem==1){
 for(iv=0; iv < nv; iv++){
 for(it=0; it < ntout; it++){
		sem_small[it+iv*ntout]=iv;
	}
	}
	return;
}

 for (iv=0; iv<nv*n1_semb; ++iv) {
    num[iv] = 0.0;
     den[iv] = 0.0;
     nnz[iv] = 0.0;
 }


for(i2=0; i2 < n2_semb; i2++){
	offset=o2+d2*i2;
 for (iv=0,v=ov; iv<nv; ++iv,v+=dv) {
    /* compute offset/velocity squared */
    offovs = (offset*offset)/(v*v);
      /* determine mute time after nmo */
      temp =offovs/(smute*smute-1.0);
      tnmute =sqrt(temp);
      if (tnmute > o1) {
        itmute = (tnmute-o1)/d1;
      } else {
        itmute = 0 ;
      }

      /* do nmo via quick and dirty linear interpolation
         (accurate enough for velocity analysis) and
         accumulate semblance numerator and denominator
      */

      for (it=itmute,tn=o1+itmute*d1; it<n1_semb; ++it,tn+=d1) {
        ti = (sqrt(tn*tn+offovs)-o1)/d1;
        iti = ti;
        if (iti<n1_semb-1) {
          frac = ti-iti;
          temp = (1.0-frac)*data_buf[iti+n1_semb*i2]+
            frac*data_buf[iti+1+n1_semb*i2];
          if (temp!=0.0) {
            num[iv*n1_semb+it] += temp;
            den[iv*n1_semb+it] += temp*temp;
            nnz[iv*n1_semb+it] += 1.0;
          }
        }
			}
		}

}

 for (iv=0; iv<nv; ++iv) {

        /* compute semblance quotients */
        for (itout=0; itout<ntout; ++itout) {
          it = itout*oversample;
          ismin = it-nsmooth/2;
          ismax = it+nsmooth/2;
          if (ismin<0) ismin = 0;
          if (ismax>n1_semb-1) ismax = n1_semb-1;
          nsum = dsum = 0.0;
          for (is=ismin; is<ismax; ++is) {
            nsum += num[iv*n1_semb+is]*
              num[iv*n1_semb+is];
            dsum += nnz[iv*n1_semb+is]*
              den[iv*n1_semb+is];
          }
          if(o1+itout*d1*oversample> ignore)
            sem_small[itout+iv*ntout] = (dsum!=0.0?nsum/dsum:0.0);
          else sem_small[itout+iv*ntout] =0.0;
        }
			

	}
}


#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif
#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif


void quick_nmo(){
int it,i2,inot,ti,iti,itmute;
float tnmute,iffovs,frac,offovs,offset,tn,mmax,mmin;

mmax=mmin=data_buf[0];

	for(i2=0; i2 < n2_semb; i2++){
		offset=o2+d2*i2;
      for (it=0,tn=o1; it<n1_semb; ++it,tn+=d1) {
				offovs=offset*offset/(vel[it]*vel[it]);
      	tnmute = sqrt(offovs/(smute*smute-1.0));
        ti = (sqrt(tn*tn+offovs)-o1)/d1;
        iti = ti;
        if (iti<n1_semb-1 && tnmute < tn) {
          frac = ti-iti;
					nmo_buf[it+i2*n1_semb]=data_buf[iti+i2*n1_semb]*(1.-frac)+data_buf[iti+1+i2*n1_semb]*frac;
				}
				else nmo_buf[it+i2*n1_semb]=0;
			}
		}

}





float sqroot (float r, float t0, float epsilon)
{
  float eps;

  if (r < 0.) r = -r;
  eps = t0 - r/t0;
  while (eps > epsilon) {
    eps = t0 - r/t0;
    t0 -= 0.5*eps;
  }
  return t0;
}


void draw_vel_line(int mode,float *myv){
Pik myp;
Plane plane;
int x1,x2,y1,y2,i,xl,yl,xh,yh;
float tempf,myf1,myf2;

myp.loc[3]=old_loc[0];
myp.loc[4]=old_loc[1];
myp.loc[5]=old_loc[2];

plane=PlaneFind (3,myp.loc[3],-1,-1,-1,-1,1);

myp.loc[1]=0;
myp.loc[2]=0;
x_y_from_pik_plane(myp,plane,&xl,&yl);
myp.loc[1]=n1_semb-1;
myp.loc[2]=nv-1;
x_y_from_pik_plane(myp,plane,&xh,&yh);


myf1=(float)(yh-yl)/(float)n1_semb;
myf2=(float)(xh-xl)/(float)nv;
y1=yl;
x1=((myv[0]-ov)/dv*myf2)+xl+.5;
for(i=2; i < n1_semb-2; i+=2){
	x2=((myv[i]-ov)/dv*myf2)+xl+.5;
	y2=myf1*(float)i+yl+.5;
	if(y2>max_wide-4) y2=max_wide-4;
  DrawLine (x1,y1,x2,y2,mode);
	x1=x2;y1=y2;
}
	
}


void calc_int_vel(){
int i;
float t;


for(i=0; i < n1_semb; i++) {
	t=o1+d1*(i-1);
	semb_buf[i]=vel[i]*t;
}
for(i=1; i < n1_semb; i++){
	intvel[i]=(semb_buf[i]-semb_buf[i-1])/d1;
}

intvel[0]=intvel[1];

}




#else
int semb_init(){}

#endif
#endif 
