#include <sepConfig.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "rick.h"
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
#ifdef SEPLIB
#include<sepcube.h>
#include <mpi_sep.h>
#endif
#ifdef SU_DATA
#include <su.h>
#include<segy.h>
segy tr;
#endif
#ifdef __APPLE__
#include <fcntl.h>
#endif


/*
data object code
data object consists of axes, buffers, and dataset info
four input data formats recognized:
	FORMAT	CONDITIONS		ACTIONS
	vgrid	DataGridHeader()==1	DataLoadFloat() or DataLoadByte()
	segy	else segy=1		DataGetpar(), DataLoadFloat()
	floats	else float=1		DataGetpar(), DataLoadFloat()
	bytes	else			DataGetpar(), DataLoadByte()
*/
/* Modified:
   Hector Urdaneta 7/26/96 
		   Had to bring up in the code the line 
                   data->value_size = DATA_VALUE_SIZE;
                   since data->value_size was being used in the 
                   subroutine GetDatapar without initialized
*/
#include <stdio.h>
#include "rick.h"
Datalist datalist;
void vel_init();
void nmoed_init(Data nmoed);
void vscan_init(Data vscan);
float cent (float  p,float *x,int n);
void reed_buf ( char *tag, float *buf2, int n1, int nread,int jdata,int *in_mem);
void reed_data(char *tag, void *buf, int n1);

/** interal velocity view functions **/
void VelViewInit(void);
void VelViewCreateBuffer1(void);
void VelViewCreateBuffer2(void);
void VelViewUpdateBuffer1(void);
void VelViewUpdateBuffer2(void);
int VelViewCheckCache(int choice);
int VelViewCacheEmpty(void);
void VelViewCreateCache(void);
int VelViewCacheLRU(void);

/** internal path view functions **/
void PathViewAddFilePick(float* pathX, float* pathY, float* pathZ, long pathInd);
void PathViewInit();
void PathViewAddPath(float* x, float* y, float* z, long length, char symbol);
void PathViewReplacePath(float* x, float* y, float* z, long length, int position);
int PathViewFindPath(char symbol);
void PathViewEmptyBuffers();
void PathViewUpdateBufferView(int mode, int pathNum);
void PathViewUpdateBuffers(int mode, int pathNum);
void PathViewUpdateBuffersAll(int mode);
void PathViewInterpolate(int pathNum);
float PathViewGapThreshold();
void PathViewSortPath(float* pathX, float* pathY, float* pathZ, long length);
int PathViewEmpty();
void PathViewPathsChange(int* update);
int PathViewCheckPickSymbol(char symbol, long* length);
int PathViewPathsCross(float* x1, float* y1, float* z1, long length1,
		       float* x2, float* y2, float* z2, long length2);
void PathViewPaintDirectional(Axis axis1, Axis axis2, Axis axis3, 
			      float axisRange1, long long dataBufferSize, 
			      Buffer result, Buffer dataBuffer,
			      int n1Path, float path1, float path2, float path3, 
			      float deltaM, float deltaN, float dimFactor);

/* initialize dataset from getpar */
void DataInit(){
int i,ierr;
char temp_ch[1024],temp2_ch[1024];

	NEW(Datalist,datalist,1);
	for (i=0; i < NUM_VIEWS; i++) datalist->data[i]=0;

  /*next we  need to see how many datasets we need to load */
	ierr=1;datalist->ndata=1;
	datalist->data[0]=InitData("in",0);
	while(ierr==1 && datalist->ndata < NUM_VIEWS){
		sprintf(temp_ch,"data%d",datalist->ndata);
		ierr=rick_getch(temp_ch,"s",temp2_ch);
		if(ierr==1){
      /*initialize extra data list */
			datalist->data[datalist->ndata]=
			  InitData(temp_ch,datalist->ndata);
			CheckData();
			datalist->ndata++;
		}
	}
	if(1==rick_getch("mode","s",temp_ch)){
		if(0==strcmp("velan",temp_ch)){
			vel_init();
		}
	}

	// check velocity view start-up initialization
	VelViewInit();
}

void vel_init(){
Data data,vscan,nmoed;
float o,d,km,tempr,ov,dv;
int n,nv,n1,n2,n3,n4,n5;
string label;


data=datalist->data[0];
NEW(Data,datalist->data[datalist->ndata],1);
vscan=datalist->data[datalist->ndata]; datalist->ndata++;
NEW(Data,datalist->data[datalist->ndata],1);
nmoed=datalist->data[datalist->ndata]; datalist->ndata++;

  tempr=AxisValue(data->axis[DATA_AXIS2],0)-AxisValue(data->axis[DATA_AXIS2],AxisSize(data->axis[DATA_AXIS2]));
  if(tempr < -100. || tempr > 100.) km=1.;
  else km=1000.;

  /*grab semblance parameters */
  if(0==rick_getch("v0","f",&ov)) ov=1.5*km;
  if(0==rick_getch("nv","d",&nv)) nv=50;
  if(0==rick_getch("dv","f",&dv)) dv=(3.5*km-ov)/(nv-1);

  n1=AxisSize(data->axis[1]);
  n2=AxisSize(data->axis[2]);
  n3=AxisSize(data->axis[3]);
  n4=AxisSize(data->axis[4]);
  n5=AxisSize(data->axis[5]);


/*initialize semblance view */
  vscan->value_size = DATA_VALUE_SIZE;
  vscan->esize = 4;
  vscan->hbytes = 0; vscan->size=1;
  vscan->axis[DATA_AXIS1]= CopyAxis(DATA_AXIS1,1,data->axis[DATA_AXIS1]);
  vscan->size=AxisSize(vscan->axis[DATA_AXIS1]);

  vscan->axis[DATA_AXIS2]= AxisInit3 (DATA_AXIS2,vscan->size,"my",nv,ov,dv,1.);
  vscan->size=vscan->size*AxisSize(vscan->axis[DATA_AXIS2]);

  vscan->axis[DATA_AXIS3]= CopyAxis(DATA_AXIS3,n1*nv,data->axis[DATA_AXIS3]);
  vscan->axis[DATA_AXIS4]= CopyAxis(DATA_AXIS4,n1*nv*n3,data->axis[DATA_AXIS4]);
  vscan->axis[DATA_AXIS5]= CopyAxis(DATA_AXIS5,n1*nv*n3*n4,data->axis[DATA_AXIS5]);




/*initialize semblance view */
  nmoed->value_size = DATA_VALUE_SIZE;
  nmoed->esize = 4;
  nmoed->hbytes = 0; nmoed->size=1;
  nmoed->axis[DATA_AXIS1]= CopyAxis(DATA_AXIS1,1,data->axis[DATA_AXIS1]);
  nmoed->size=AxisSize(nmoed->axis[DATA_AXIS1]);

  nmoed->axis[DATA_AXIS2]= CopyAxis(DATA_AXIS2,nmoed->size,data->axis[DATA_AXIS2]);
  nmoed->size=nmoed->size*AxisSize(nmoed->axis[DATA_AXIS2]);

  nmoed->axis[DATA_AXIS3]= CopyAxis(DATA_AXIS3,n1*n2,data->axis[DATA_AXIS3]);
  nmoed->axis[DATA_AXIS4]= CopyAxis(DATA_AXIS4,n1*n2*n3,data->axis[DATA_AXIS4]);
  nmoed->axis[DATA_AXIS5]= CopyAxis(DATA_AXIS5,n1*n2*n3*n4,data->axis[DATA_AXIS5]);

  strcpy(label,"Offset");
  nmoed->axis[DATA_VALUE] = AxisInit2(DATA_VALUE,1,label,nmoed->value_size,
  nmoed->low,(nmoed->high-nmoed->low)/(nmoed->value_size-1),1);
  strcpy(label,"Velocity");
  vscan->axis[DATA_VALUE] = AxisInit2(DATA_VALUE,1,label,vscan->value_size,
  vscan->low,(vscan->high-vscan->low)/(vscan->value_size-1),1);

  vscan->pickable=1;
  nmoed->pickable=0;
  vscan->data_2d=1;
  nmoed->data_2d=1;
	data->pickable=0;
	strcpy(nmoed->tag,"nmoed");
	strcpy(vscan->tag,"vscan");
}


void float_to_byte(float *input, Buffer output, int n1, float *tgain, 
		   register float bias, register float scale, int value_size){
int i1 ;
register float  *fp, *gp,*fe;
register byte dp;
register int datum;


  dp=output;


    for (fp=input, fe=fp+n1, gp=tgain; fp<fe; fp++, dp++, gp++) {
      datum = (*fp* *gp + bias) * scale;
      datum = datum > 0 ? datum : 0;
      datum = datum < value_size ? datum : value_size-1;
      datum += DATA_VALUE_BASE;
      *dp = datum;
      }
}

void CheckData(){
int i,ndata;
ndata=datalist->ndata;
for(i=DATA_AXIS1; i<= DATA_AXIS5; i++){
 if(datalist->data[0]->axis[i]->size != datalist->data[ndata]->axis[i]->size)
		err("Size of axis(%d) do not match for input=%d and data%d=%d \n",
       i,datalist->data[0]->axis[i]->size,ndata, 
          datalist->data[ndata]->axis[i]->size);
 if(datalist->data[0]->axis[i]->delta != datalist->data[ndata]->axis[i]->delta)
		fprintf(stderr,"WARNING: d of axis(%d) do not match for input=%f and data%d=%f \n",
       i,datalist->data[0]->axis[i]->delta,ndata, 
          datalist->data[ndata]->axis[i]->delta);
 if(datalist->data[0]->axis[i]->first != datalist->data[ndata]->axis[i]->first)
		fprintf(stderr,"WARNING: o of axis(%d) do not match for input=%f and data%d=%f \n",
       i,datalist->data[0]->axis[i]->delta,ndata, 
          datalist->data[ndata]->axis[i]->delta);
}
   
}

Data InitData (char *tag, int data_num)
	{
	Data data;
	Axis axis;
	int iaxis, i, vfd, ncolor;
	FILE *fd;
	char extra[1024];

  if(data_num>0) sprintf(extra,"%d",data_num);
	else strcpy(extra,"");

	NEW(Data,data,1);
	data->value_size = DATA_VALUE_SIZE;
		DataGetpar (data,tag,extra);
	strcpy (data->title,"stdin");
	if (rick_auxfetch("title","s",data->title,tag,extra) == 0) 
		rick_auxfetch("in","s",data->title,tag,extra);

	data->tpow = 0.;
	rick_auxfetch ("tpow","f",&data->tpow,tag,extra);
	data->gpow = 1.;
	rick_auxfetch ("gpow","f",&data->gpow,tag,extra);
	data->transp = 0;
	rick_auxfetch("transp","d",&data->transp,tag,extra);
	/* perform transpose by swapping axes */
	if (data->transp) {
		NEW (Axis,axis,1);
		axis[0] = data->axis[DATA_AXIS1][0];
		data->axis[DATA_AXIS1][0] = data->axis[DATA_AXIS2][0];
		data->axis[DATA_AXIS2][0] = axis[0];
		FREE (axis);
		}
	data->value_base = DATA_VALUE_BASE;
	strcpy(data->tag,tag);

	strcpy (data->veldata,"");
	rick_auxfetch ("veldata","s",data->veldata,tag,extra);
	NEW(Buffer,data->buffer,data->size);
	data->grab_buffer=return_buffer;
	data->pickable=1;
	
	return (data);
	}



/* fetch grid parameters from getpar */
int DataGetpar (Data data,char *tag, char *extra)
	{
	  int iaxis;
	string label;

	data->segy = DATA_SEGY;
#ifdef SEPLIB
        data->esize=4;
#endif

#ifdef SU_DATA
        data->esize=4;
#endif
	rick_auxfetch ("esize","d",&data->esize,tag,extra);
	data->size = 1;
	data->hbytes = 0;

	// DC06
	data->data_2d = 0;
	for (iaxis=DATA_AXIS1; iaxis<=DATA_AXIS3; iaxis++) {
	  /* accumulating size are axis strides */
	  data->axis[iaxis] = AxisInit (tag,iaxis,data->size,&data->data_2d);
	  data->size *= (long long) AxisSize (data->axis[iaxis]);
	}
	data->axis[DATA_AXIS4] = AxisInit (tag,4,data->size,0);
	data->axis[DATA_AXIS5] = AxisInit (tag,5,data->size,0);

	data->size *= AxisSize (data->axis[DATA_AXIS4]);
	data->size *= AxisSize (data->axis[DATA_AXIS5]);
	data->max = DATA_HIGH;
	rick_auxfetch ("max","f",&data->max,tag,extra);
	data->high = data->max;
	rick_auxfetch ("high","f",&data->high,tag,extra);
	rick_auxfetch ("pclip","f",&data->high,tag,extra);
	rick_auxfetch ("clip","f",&data->high,tag,extra);
	if (data->high != DATA_HIGH) {
		data->min = -data->high;
		data->low = data->min;
		}
	else	{
		data->min = DATA_LOW;
		data->low = data->min;
		}
	rick_auxfetch ("low","f",&data->low,tag,extra);
	rick_auxfetch ("nclip","f",&data->low,tag,extra);
	rick_auxfetch ("min","f",&data->min,tag,extra);
	data->cent = DATA_CENT;
	rick_auxfetch ("perc","f",&data->cent,tag,extra);
	rick_auxfetch ("cent","f",&data->cent,tag,extra);
	strcpy (label,"samples"); rick_auxfetch ("value","s",label,tag,extra);
	data->axis[DATA_VALUE] = AxisInit2(DATA_VALUE,1,label,data->value_size,
	data->low,(data->high-data->low)/(data->value_size-1),1);
	return(0);
}

/* decide which data reading routine to use */
void DataLoad ()
{
  int i,ib,is;
  char tag[1024],add_to[1024];
  
  for(i=0; i < datalist->ndata; i++){
    datalist->data[i]->value_size=DATA_VALUE_SIZE;
    if(0==strcmp(datalist->data[i]->tag,"vscan")){
      vscan_init(datalist->data[i]);}
    else if(0==strcmp(datalist->data[i]->tag,"nmoed")){
      nmoed_init(datalist->data[i]);
    }
    else{
      switch (datalist->data[i]->esize) {
      case 4: DataLoadFloat (datalist->data[i],datalist->data[i]->tag); break;
      case 1: DataLoadByte (datalist->data[i],datalist->data[i]->tag); break;
      }
      
      if(i>0) sprintf(add_to,"%d",i);
      else strcpy(add_to,"");
      if(1==rick_auxgetch("pick","s",datalist->data[i]->pik->file,add_to))
	PikRead(datalist->data[i]);
    }
    
  }
  
  // create velocity view and path view, if specified
  if (VelViewLoaded() && datalist->ndata >= 2) {
    VelViewCreateBuffer1();
    VelViewCreateBuffer2();
  }
  PathViewInit();
  UIDelayMessage ("Data: data loaded.");	
}



void reload_data(){
DataLoadFloat (datalist->data[0],datalist->data[0]->tag);
}

void nmoed_init(Data nmoed){
Data data;
float o,d,km,tempr,ov,dv;
int n,nv;
string label;


 data=datalist->data[0];
 
  /*initialize cmp view*/
 nmoed->max = DATA_HIGH;
 nmoed->high = nmoed->max;
 nmoed->min = DATA_LOW;
 nmoed->low = nmoed->min;
 nmoed->cent = data->cent;
 strcpy (label,"samples"); 
 strcpy (nmoed->title,"Nmo Result");
 
 nmoed->tpow = data->tpow;
 nmoed->gpow = data->gpow;
 nmoed->transp = data->transp;
 nmoed->value_base = data->value_base;
 
 NEW(Buffer,nmoed->buffer,nmoed->size);
 nmoed->grab_buffer=return_buffer;
 SetAxisStride(nmoed->axis[3],0);
 SetAxisStride(nmoed->axis[4],0);
 SetAxisStride(nmoed->axis[5],0);
 semb_init();
}

Data DataFromTag(char *tag){
int i;

i=DataNumFromTag(tag);
if(i==-1) err("Unable to find dataset %s \n",tag);
	return(datalist->data[i]);
}

int DataNumFromTag(char *tag){
int i;

for(i=0; i < ReturnNdata(); i++){
	if(strcmp(datalist->data[i]->tag,tag)==0) return(i);
}

return(-1);


}

void vscan_init(Data vscan){
Data data;
float o,d,km,tempr,ov,dv;
int n,nv;
string label;


data=datalist->data[0];

  vscan->max = DATA_HIGH;
  vscan->high = vscan->max;
  vscan->min = DATA_LOW;
  vscan->low = vscan->min;
  vscan->cent = DATA_CENT;
  strcpy (label,"samples"); 
  strcpy (vscan->title,"Semblance");
  vscan->tpow = 0.;
  vscan->gpow = 1.;
  vscan->transp = 0;
  vscan->value_base = DATA_VALUE_BASE;

  NEW(Buffer,vscan->buffer,vscan->size);
  vscan->grab_buffer=return_buffer;


  SetAxisStride(vscan->axis[3],0);
  SetAxisStride(vscan->axis[4],0);
  SetAxisStride(vscan->axis[5],0);
}





/* load byte data; compress to 7 bits- 0-128 */

void DataLoadByte (Data data,char *tag)
	{
	byte table;
	int i, value, ncolor,n1,n2,n3;
        long n12;
	register byte tp;
	register Buffer bp, be;

	/* read data */
	UIMessage ("Data: loading byte data.");
        n1=AxisSize(data->axis[1]);
	n2=AxisSize(data->axis[2]);
	n3=AxisSize(data->axis[3]);
	
	// DC06
	// special 2D data, duplicate along deep dimension
	if(1==data_is_2d(data)){
	  FREE(data->buffer);
	  NEW(Buffer, data->buffer, data->size);
	  reed_data(tag,&data->buffer[0],n1*n2);
	  for (i=1; i < n3; i++)
	    memcpy((void *)(data->buffer + i*n1*n2),(const void*) data->buffer, n1*n2);
	}

	else {
         n12=data->size/(long long)n1;
         for(i=0; i < n12; i++) reed_data(tag,&data->buffer[n1*i],n1);
       }

	/* compute color compression table table */
	NEW (byte,table,256);
	/* bounds */
	table[0] = 0;
	/* compute lookup table */
	ncolor = ColorSize();
	ncolor = DataValueSize(data);
	for (i=0; i<256; i++) {
		table[i] = (i * ncolor) / 255;
		}
	for (bp=data->buffer, be=bp+data->size, tp=table; bp<be; bp++) {
		*bp = tp[*bp];
		}
	DataComputeHistogram (data);
	FREE (table);
	}








void reed_buf ( char *tag, float *buf2, int n1,  int nread ,int jdata,int *in_mem)
{
  int i2,iout;
  for(i2=0; i2 < nread; i2++){
    reed_data(tag,&buf2[n1*i2],n1*4);
  }
  *in_mem=0;
}
void reed_data(char *tag, void *buf, int n1){

#ifdef SEPLIB
 sreed(tag,buf,n1);
#endif
#ifdef SU_DATA
  gettr(&tr);
  memcpy((void*)buf,(const void*) tr.data,n1);
#endif

}


/* load and convert float or segy data */
void DataLoadFloat (Data data,char *tag)
{
  int i1, i2, i3, n1, n2, n3, base, size, nread,jdata;
  int in_mem;
  float *buf1, *buf2,  *trace, *tgain;
  register float scale, bias, *bp1, *bp2, *fp, *fe, *gp;
  register int datum;
  register byte dp;
  double pow(), log();

  if (data->segy) UIMessage ("loading segy data ...");
  else UIMessage ("loading float data ...");
  n1 = AxisSize (data->axis[DATA_AXIS1]);
  n2 = AxisSize (data->axis[DATA_AXIS2]);
  n3 = AxisSize (data->axis[DATA_AXIS3])
  * AxisSize (data->axis[DATA_AXIS4])
   * AxisSize (data->axis[DATA_AXIS5]);
  nread=n2*n3;
  nread=n2;jdata=1;
  
  NEW (float*,buf2,n1*nread);
  NEW (float*,buf1,n1*nread);
  NEW (float*,tgain,n1);
  size = (n1)>DATA_HEAD1?(n1):DATA_HEAD1;
  size = size > data->hbytes ? size : data->hbytes;
  NEW (float*,trace,size);
  fe = trace + n1 ;
  bp2 = buf2;
  for (i1=0; i1<n1-1; i1++) {
    tgain[i1] = AxisValue(data->axis[DATA_AXIS1],i1+1);
    tgain[i1] = tgain[i1] != 0.0 ? pow(tgain[i1],data->tpow) : 1.0;
  }
  tgain[n1-1] = tgain[n1-2];
  reed_buf(tag,buf2,n1,nread,jdata,&in_mem);
  memcpy((void*)buf1,(const void*)buf2,n1*nread*sizeof(float));
  if (data->low == DATA_LOW && data->high == DATA_HIGH) {
    string label;
    data->min = DataCent (buf1,n1*nread,DATA_CENT_MIN);
    data->low = DataCent (buf1,n1*nread,DATA_CENT_LOW);
    data->high = DataCent (buf1,n1*nread,DATA_CENT_HIGH);
    data->max = DataCent (buf1,n1*nread,DATA_CENT_MAX);
    strcpy (label,AxisLabel(data->axis[DATA_VALUE]));
    FREE (data->axis[DATA_VALUE]);
    data->axis[DATA_VALUE] = AxisInit2(DATA_VALUE,1,label,data->value_size+1,
    data->low,(data->high-data->low)/data->value_size,1);
  }
  if(data->low==data->high){
    if(data->min!=data->low) data->low=data->min;
    if(data->max!=data->high) data->high=data->max;
  }

  FREE (buf1);
  if(data->low==data->high) err("minimum and maximum the same\n");


  if (data->low == 0.0 && data->high == 0.) 
    err ("first panel appears to be all zeros; set clip");
  if (data->high > data->low) {
    bias = -data->low;
    scale = data->value_size / (data->high - data->low);
  }
   else	{
     bias = 0.;
     scale = 0.5 * data->value_size / data->high;
   }
   dp = data->buffer;
     sep_open_lock(tag);
		       
   for (i3=0; i3<n3; i3++){
     for (i2=0; i2<n2; i2++) {
       if(i3==in_mem) /*in memory copy it*/
         memcpy((void*)trace,(const void*) (buf2+i2*n1),n1*sizeof(float));
       else reed_data(tag,trace,n1*4);
       for (fp=trace, fe=fp+n1, gp=tgain; fp<fe; fp++, dp++, gp++) {
         datum = (*fp* *gp + bias) * scale;
         datum = datum > 0 ? datum : 0;
         datum = datum < data->value_size ? datum : data->value_size-1;
         datum += DATA_VALUE_BASE;
         *dp = datum;
       }
     }
   }
     sep_close_lock(tag);
   DataComputeHistogram (data);
   FREE (trace); FREE (tgain); FREE (buf2);
   sseek(tag,0,0);
}

/*      percentile subroutine
 *      based on Canales, SEP-10
 *      p - percentile <0.,99.999999999999>
 *      x - data
 *      n - vector length
 *	this routine changes data order, so sort a copy
 */
float DataCent (float *x,int n,float p)
        {
        int q;
        register float *i, *j, ak;
        float *low, *hi, buf, *k;
	p = p < 99.999 ? p : 99.999;
	p = p > 0.0 ? p : 0.0;
	q = (p * n) / 100.;
	if (q == n) q--;
        for (low=x, hi=x+n-1, k=x+q; low<hi;)
                {
                ak = *k;
                i = low; j = hi;
                do
                        {
                        while (*i < ak) i++;     
                        while (*j > ak) j--;     
                        if (i<=j)     
                                {
                                buf = *i;
                                *i++ = *j;
                                *j-- = buf;
                                }
                        } while (i<=j);
                if (j<k) low = i; if (k<i) hi = j;
                }
        return (*k);
        }

/* return long data name */
char* DataLabel (Data data)
	{
	static Message message;

	if (!data) return (0);
	sprintf (message,"%s: %dx%dx%dx%dx%d=%lld %s samples",data->title,
		AxisSize(data->axis[DATA_AXIS1]),
		AxisSize(data->axis[DATA_AXIS2]),
		AxisSize(data->axis[DATA_AXIS3]),
		AxisSize(data->axis[DATA_AXIS4]),
		AxisSize(data->axis[DATA_AXIS5]),
		data->size,
		AxisLabel(data->axis[DATA_VALUE]));
	return (message);
	}

/* return short data name */
char* DataTitle (Data data)
	{
	if (!data) return (0);
	return (data->title);
	}

/* return data file */
char* DataFile (Data data)
	{
	if (!data) return (0);
	return (data->file);
	}

/* return short name of dataset from filename */
char* DataShortName (Data data)
	{
	static string name;
	char *c;

	/* truncate leading pathname */
	if ((c = strrchr (data->file,'/')) != NULL) strcpy (name,c+1);
	else strcpy (name,data->file);
	/* truncate trailing tail */
	if ((c = strchr (name,'.')) != NULL) *c = '\0';
	return (name);
	}
/* return data buffer */
Buffer DataBuffer (Data data)
	{
	if (!data) return (0);
	return (data->grab_buffer(data));
	}

/* return axis reference */
Axis DataAxis (Data data,int iaxis)
	{
	if (!data || iaxis>DATA_NAXIS) return (0);
	return (data->axis[iaxis]);
	}

/* return data size */
int DataSize (Data data)
	{
	return (data->size);
	}

/* return longest dimension */
int DataMaxDim (Data data)
	{
	int n[DATA_NAXIS], max, iaxis;

	if (!data) return (0);
	for (iaxis=0, max=1; iaxis<DATA_NAXIS-1; iaxis++) {
		max = AxisSize(data->axis[iaxis+1]) > max ? AxisSize(data->axis[iaxis+1]) : max;
		}
	return (max);
	}

/* return data amplitude */
float DataValue (Data data,int value)
	{
	return (data->low+((data->high-data->low)*((value&0x7F)-data->value_base))/data->value_size);
	}

/* return data value size */
int DataValueSize (Data data)
	{
	if (!data) return (0);
	else return (data->value_size);
	}

/* return base data value */
int DataValueBase (Data data)
	{
	if (!data) return (0);
	else return (data->value_base);
	}


/* return data high clip */
float DataHigh ()
	{
	View myv;

	myv=current_view();
	if (!myv->data) return (0.0);
	return (myv->data->high);
	}

/* return data low clip */
float DataLow ()
	{
	View myv;

	myv=current_view();

	if (!myv->data) return (0.0);
	return (myv->data->low);
	}

int ReturnNdata(){
return(datalist->ndata);
}

/* return integer percentage of histogram */
int DataHistogram (int i)
	{
	View myv;

	myv=current_view();

	if (!myv->data || i<0 || i>255) return (0);
	return ((int)(100*myv->data->histogram[i]));
	}

void DataComputeHistogram (Data data)
	{
	register Buffer bp, be;
	register float *hp;
	int i;
	float max;

	for (i=0; i<256; i++) {
		data->histogram[i] = 0;
		}
	for (bp=data->buffer,be=bp+data->size,hp=data->histogram; bp<be; bp++) {
		hp[*bp] += 1.;
		}
	for (i=0, max=0, data->hist_low=255, data->hist_high=0; i<256; i++) {
		data->histogram[i] = data->histogram[i] / data->size;
		max = data->histogram[i] > max ? data->histogram[i] : max;
		if (data->hist_low == 255 && data->histogram[i] != 0) data->hist_low = i;
		if (data->histogram[i] != 0) data->hist_high = i;
		}
	max = max > 0. ? 1. / max : 0.;
	for (i=0; i<256; i++) {
		data->histogram[i] = data->histogram[i] * max;
		}
	}

/* print data information */
int DataInfo (Data data)
	{
	Message message;

	sprintf (message,"Data: %s: in=%s %dx%dx%dx%dx%d=%lld esize=%d segy=%d hbytes=%d veldata=%s script=%s",
		data->title,data->file,
		AxisSize(data->axis[DATA_AXIS1]),
		AxisSize(data->axis[DATA_AXIS2]),
		AxisSize(data->axis[DATA_AXIS3]),
		AxisSize(data->axis[DATA_AXIS4]),
		AxisSize(data->axis[DATA_AXIS5]),
		data->size,
		data->esize,
		data->segy,
		data->hbytes,
		data->veldata,
		data->script);
	UIDelayMessage (message);
  return(0);
	}

/* data value parameters */
int DataValueInfo (Data data)
	{
	Message message;

	sprintf (message, "Value: min=%g low=%g high=%g max=%g tpow=%g gpow=%g base=%d size=%d",
		data->min,
		data->low,
		data->high,
		data->max,
		data->tpow,
		data->gpow,
		data->value_base,
		data->value_size);
	UIDelayMessage (message);
  return(0);
	}

/* save data parameters */
int DataSavePar (Data data)
	{
	int iaxis;
	Message message;

	if (!data) return(-1);
	sprintf (message,"Data: %s: in=%s %dx%dx%dx%dx%d=%lld esize=%d segy=%d hbytes=%d script=%s",
		data->title,
		data->file,
		AxisSize (data->axis[DATA_AXIS1]),
		AxisSize (data->axis[DATA_AXIS2]),
		AxisSize (data->axis[DATA_AXIS3]),
		AxisSize (data->axis[DATA_AXIS4]),
		AxisSize (data->axis[DATA_AXIS5]),
		data->size,
		data->esize,
		data->segy,
		data->hbytes,
		data->script);
	UISaveMessage (message);
	sprintf (message, "Value: min=%g low=%g high=%g max=%g tpow=%f gpow=%g base=%d size=%d",
		data->min,
		data->low,
		data->high,
		data->max,
		data->tpow,
		data->gpow,
		data->value_base,
		data->value_size);
	UISaveMessage (message);
	for (iaxis=DATA_AXIS1; iaxis<=DATA_AXIS5; iaxis++) {
		AxisSavePar (data->axis[iaxis]);
		}
  return(0);
	}

/* dump data bytes */
int DataDumpBytes (Data data,char *file,int fd)
	{
	string parfile, datafile;
	register byte bp, be;
	Message message;

	DrawWatch(1);
	for (bp=data->buffer, be=bp+data->size; bp<be;) *bp++ <<= 1;
	if(data->size != write (fd,data->buffer,data->size)) {
            perror("DataDumpBytes ");
            err("write error\n");
        }
	for (bp=data->buffer, be=bp+data->size; bp<be;) *bp++ >>= 1;
	DataDumpHeader (data,file,fd,1);
	DrawWatch(0);
  return(0);
	}

/* dump data vgrid header */
int DataDumpHeader (Data data,string file,int datafd,int esize)
	{
	Message message;
	char parfile[80];
	extern FILE *savefd;

	if (data->gh.dtype == 0.) { /* build a header */
		data->gh.scale = 1.;
		data->gh.n1 = AxisSize (data->axis[DATA_AXIS1]) * data->gh.scale;
		data->gh.n2 = AxisSize (data->axis[DATA_AXIS2]) * data->gh.scale;
		data->gh.n3 = AxisSize (data->axis[DATA_AXIS3]) * data->gh.scale;
		data->gh.n4 = AxisSize (data->axis[DATA_AXIS4]) * data->gh.scale;
		data->gh.n5 = AxisSize (data->axis[DATA_AXIS5]) * data->gh.scale;
		data->gh.d1 = AxisDelta (data->axis[DATA_AXIS1]) * data->gh.scale;
		data->gh.d2 = AxisDelta (data->axis[DATA_AXIS2]) * data->gh.scale;
		data->gh.d3 = AxisDelta (data->axis[DATA_AXIS3]) * data->gh.scale;
		data->gh.d4 = AxisDelta (data->axis[DATA_AXIS4]) * data->gh.scale;
		data->gh.d5 = AxisDelta (data->axis[DATA_AXIS5]) * data->gh.scale;
		data->gh.o1 = AxisFirst (data->axis[DATA_AXIS1]) * data->gh.scale;
		data->gh.o2 = AxisFirst (data->axis[DATA_AXIS2]) * data->gh.scale;
		data->gh.o3 = AxisFirst (data->axis[DATA_AXIS3]) * data->gh.scale;
		data->gh.o4 = AxisFirst (data->axis[DATA_AXIS4]) * data->gh.scale;
		data->gh.o5 = AxisFirst (data->axis[DATA_AXIS5]) * data->gh.scale;
		data->gh.dcdp2 = 1.;
		data->gh.dline3 = 1.;
		data->gh.ocdp2 = 1.;
		data->gh.oline3 = 1.;
		data->gh.gmin = data->low * data->gh.scale;
		data->gh.gmax = data->high * data->gh.scale;
		}
	data->gh.dtype = esize * data->gh.scale;
	if(sizeof(data->gh) != write (datafd,&data->gh,sizeof(data->gh))) {
             perror("DataDumpHeader: write ");
             err("write error in DataDumpHeader\n");
        }
	close (datafd);
	sprintf (parfile,"%s.H",file);
	if ((savefd = fopen (parfile,"w")) == NULL); UIMessage ("cant create vgrid parfile");
	DataSavePar (data);
	sprintf (message, "Byte data dump into %s",parfile);
	UIMessage (message);
	fclose (savefd);
	return(0);
	}

/* dump data floats */
int DataDumpFloats (Data data,char *file,int fd)
	{
	string parfile, datafile;
	int nn, n3, i;
	Buffer dbuf;
	float *fbuf, min, max;
	Message message;

	DrawWatch (1);
	nn = AxisSize (data->axis[DATA_AXIS1]) * AxisSize (data->axis[DATA_AXIS2]);
	n3 = data->size / nn;
	NEW (float*,fbuf,nn);
	/* convert and dump a plane at a time */
	min = data->high;
	max = data->low;
	for (i=0, dbuf=data->buffer; i<n3; i++, dbuf+=nn) {
		Data2Float (dbuf,fbuf,nn,&min,&max);
		if(nn*sizeof(fbuf[0]) != write (fd,fbuf,nn*sizeof(fbuf[0]))) {
                    perror("DataDumpFloats: write ");
                    err("write error in DataDumpFloats\n");
                }
	}
	data->gh.gmin = min * data->gh.scale;
	data->gh.gmax = max * data->gh.scale;
	DataDumpHeader (data,file,fd,4);
	FREE (fbuf);
	DrawWatch (0);
  return(0);
	}

void Data2Float (Buffer dbuf,float *fbuf,int n,float *min,float *max)
	{
	register Buffer bp, be;
	register float *fp, *mp, *fe, Min, Max;
	View myv;
	myv=current_view();
	Min = *min;
	Max = *max;
	/* convert */
	for (mp=AxisValues(myv->data->axis[DATA_VALUE])-DATA_VALUE_BASE, bp=dbuf, fp=fbuf, be=dbuf+n; bp<be; bp++, fp++) {
		*fp = mp[*bp];
		Max = Max > *fp ? Max : *fp;
		Min = Min < *fp ? Min : *fp;
		}
	*min = Min;
	*max = Max;
	}

int data_is_2d(Data data){
return(data->data_2d);
}

void set_data_lists(){
extern Pikers pikers;
extern Anoviews anoviews;
int i;
for(i=0; i < datalist->ndata;i++){
	datalist->data[i]->pik=pikers->pik[i];
	datalist->data[i]->ano=anoviews->ano[i];
}
}
Buffer return_buffer(Data data){

return(data->buffer);

}

int rint2 (float x) 
	{
	int i;

	i = x;
	return ((x-i) < (1-(x-i)) ? i : i+1);
	}
void compute_gain_pars(Data data,  float *buf2, int n, register float *scale, register float *bias ){
                                                                                                 
  if (data->low == DATA_LOW && data->high == DATA_HIGH) {
    string label;
    data->min = DataCent (buf2,n,DATA_CENT_MIN);
    data->low = DataCent (buf2,n,DATA_CENT_LOW);
    data->high = DataCent (buf2,n,DATA_CENT_HIGH);
    data->max = DataCent (buf2,n,DATA_CENT_MAX);

                                                                                                 
    strcpy (label,AxisLabel(data->axis[DATA_VALUE]));
    FREE (data->axis[DATA_VALUE]);
    data->axis[DATA_VALUE] = AxisInit2(DATA_VALUE,1,label,data->value_size+1,
    data->low,(data->high-data->low)/data->value_size,1);
    }
  if (data->low == 0.0 && data->high == 0.) {
    err ("first panel appears to be all zeros; set clip");
    }
  if (data->high > data->low) {
    *bias = -data->low;
    *scale = data->value_size / (data->high - data->low);
    }
  else  {
    *bias = 0.;
    *scale = 0.5 * data->value_size / data->high;
    }
                                                                                                 
}

/**********************************************************
 * VELOCITY VIEW
 **********************************************************/

/** internal function **/
void VelViewInit(void) {
  int i, flag;

  if (0==rick_getch("velview","d",&flag)) {
    datalist->velocityBufferLoaded = 0;
    return;
  }

  // initial cache parameters (signify empty cache)
  for (i=0; i<NUM_VEL_CACHE; i++) {
    datalist->velocityCache[i] = 0;
    datalist->velocityCacheID[i] = -1;
    datalist->velocityCacheParam[i] = -1;
    datalist->velocityCacheUsage[i]=0;
  }
  datalist->velocityBufferLoaded = flag;
  datalist->velocityBufferViewed = 0;
  return;
}

int VelViewLoaded() {
  return datalist->velocityBufferLoaded;
}

int VelViewOn() {
  return datalist->velocityBufferViewed;
}

Buffer VelViewBuffer(Data data) {
  int i;
  if (data == datalist->data[0] && VelViewOn()) {
    return datalist->velocityBuffer;
  }
  else {
    for (i=0; i<datalist->ndata; i++) {
     if (data == datalist->data[i]) 
       return datalist->data[i]->buffer;
    }
  }
  return(datalist->data[0]->buffer);
}

int VelViewNum() {
  return (datalist->velocityChoice);
}

int VelViewParam() {
  return (datalist->velocityBufferParam);
}

void VelViewSetParam(int param) {
  if (!VelViewOn()) return;
  if (VelViewLoaded() == 0 && datalist->ndata >= 2) {
    VelViewCreateBuffer1();
    VelViewCreateBuffer2();
    datalist->velocityBufferLoaded = 1;
  }
  if (VelViewLoaded() == 0) return;
  param = MAX_NUM(param, BUFFER_PARAM_LOW);
  param = MIN_NUM(param, BUFFER_PARAM_HIGH);
  datalist->velocityBufferParam = param;
  if (datalist->velocityChoice == 1) {
    VelViewUpdateBuffer1();
  }
  else if (datalist->velocityChoice == 2) {
    VelViewUpdateBuffer2();
  }
}

void VelViewSetBuffer(int choice) {
  int old;
  if (VelViewLoaded() == 0 && datalist->ndata >= 2) {
    VelViewCreateBuffer1();
    VelViewCreateBuffer2();
    datalist->velocityBufferLoaded = 1;
  }
  if (VelViewLoaded() == 0) return;
  old = datalist->velocityChoice;
  datalist->velocityChoice = choice;
  switch(choice) {
  case 1: 
    VelViewUpdateBuffer1();
    datalist->velocityBufferViewed = 1;
    break;
  case 2: 
    VelViewUpdateBuffer2();
    datalist->velocityBufferViewed = 1;
    break;
  default:
    datalist->velocityBuffer = 0;
    datalist->velocityBufferViewed = 0;
    break;
  }
  if (PathViewOn() && datalist->pathVelChoice != choice) {
    PathViewEmptyBuffers();
    PathViewUpdateBuffersAll(DRAW);;
  }
}

/** internal function **/
void VelViewCreateBuffer1(void) {
  long long n, bufferSize;
  int sign;
  Buffer result, invData, velData;

  if (datalist->ndata < 2) return;
  invData = datalist->data[0]->buffer;
  velData = datalist->data[1]->buffer;
  bufferSize = datalist->data[0]->size;

  // find maximum velocity element
  FindBufferMean(invData, bufferSize, &datalist->invDataMean);

  datalist->velocityBufferParam = BUFFER_PARAM_MID;
  VelViewUpdateBuffer1();
}

/** internal function **/
void VelViewCreateBuffer2(void) {
  long long n, bufferSize;
  Buffer result, invData, velData, diffFromMode;
  float *histogram;
  int mode;

  if (datalist->ndata < 2) return;
  invData = datalist->data[0]->buffer;
  velData = datalist->data[1]->buffer;
  bufferSize = datalist->data[0]->size;

  // calculate difference from mode
  Histogram(invData, bufferSize, histogram, &mode);
  NEW(Buffer, diffFromMode, bufferSize);
  for (n=0; n<bufferSize; n++) {
    diffFromMode[n] = abs(invData[n] - mode);
  }
  FindBufferMax(diffFromMode, bufferSize, &datalist->maxDiffFromMode);

  datalist->diffFromMode = diffFromMode;
  datalist->velocityBufferParam = BUFFER_PARAM_MID;
  VelViewUpdateBuffer2();
}

/** internal function **/
void VelViewUpdateBuffer1(void) {
  long long n;
  double delta, gamma, alpha, temp;
  int sign, cached;
  Buffer invData, velData, result;

  cached = VelViewCheckCache(1);
  if (cached) return;

  // otherwise update cache below
  DrawWatch(1);

  // aliases
  invData = datalist->data[0]->buffer;
  velData = datalist->data[1]->buffer;
  result = datalist->velocityBuffer;

  alpha = 0.6;
  for (n=0; n<datalist->data[0]->size; n++) {
    delta = alpha*(invData[n] - datalist->invDataMean);
    if (velData[n] < BUFFER_VALUE_LOW)
      result[n] = BUFFER_VALUE_LOW;
    else if ((temp = velData[n] + delta) >= BUFFER_VALUE_LOW && temp <= BUFFER_VALUE_HIGH)
      result[n] = velData[n] + delta;
    else
      result[n] = velData[n];
  }

  if (datalist->velocityBufferParam > BUFFER_PARAM_MID) {
    gamma = ((float)(BUFFER_PARAM_HIGH - datalist->velocityBufferParam)) / ((float)BUFFER_PARAM_MID);
    for (n=0; n<datalist->data[0]->size; n++) {
      result[n] = gamma*result[n] + (1-gamma)*velData[n];
    }
  }
  else {
    gamma = ((float)datalist->velocityBufferParam) / ((float)BUFFER_PARAM_MID);
    for (n=0; n<datalist->data[0]->size; n++) {
      result[n] = gamma*result[n] + (1-gamma)*invData[n];
    }
  }

  DrawWatch(0);
}

/** internal function **/
void VelViewUpdateBuffer2(void) {
  long long n;
  Buffer invData, velData, result;
  double beta, gamma;
  int cached;

  cached = VelViewCheckCache(2);
  if (cached) return;

  // otherwise update cache below
  DrawWatch(1);

  // aliases
  invData = datalist->data[0]->buffer;
  velData = datalist->data[1]->buffer;
  result = datalist->velocityBuffer;

  for (n=0; n<datalist->data[0]->size; n++) {
    // weight based on difference from mode
    beta = (double)datalist->diffFromMode[n] / (double)datalist->maxDiffFromMode;
    result[n] = beta*invData[n] + (1-beta)*MAX_NUM(velData[n],BUFFER_VALUE_LOW);
  }

  if (datalist->velocityBufferParam > BUFFER_PARAM_MID) {
    gamma = ((float)(BUFFER_PARAM_HIGH - datalist->velocityBufferParam)) / ((float)BUFFER_PARAM_MID);
    for (n=0; n<datalist->data[0]->size; n++) {
      result[n] = gamma*result[n] + (1-gamma)*velData[n];
    }
  }
  else {
    gamma = ((float)datalist->velocityBufferParam) / ((float)BUFFER_PARAM_MID);
    for (n=0; n<datalist->data[0]->size; n++) {
      result[n] = gamma*result[n] + (1-gamma)*invData[n];
    }
  }

  DrawWatch(0);
}

/** internal function **/
void VelViewCreateCache(void) {
  int i;
  for (i=0; i<NUM_VEL_CACHE; i++) {
    NEW(Buffer, datalist->velocityCache[i], datalist->data[0]->size);
  }
}

/** internal function **/
int VelViewCacheEmpty(void) {
  int cacheEmpty, i;
  cacheEmpty = 1;
  for (i=0; i<NUM_VEL_CACHE; i++) {
    if (datalist->velocityCache[i] != 0)
      cacheEmpty = 0;
  }
  return (cacheEmpty);
}

/** internal function **/
int VelViewCheckCache(int method) {
  int i, resolution;

  if (VelViewCacheEmpty() == 1) {
    VelViewCreateCache();
  }

  resolution = BUFFER_PARAM_RESOLUTION;
  for (i=0; i<NUM_VEL_CACHE; i++) {
    if (datalist->velocityCacheID[i] == method &&
	abs(datalist->velocityCacheParam[i]-datalist->velocityBufferParam) < resolution) 
      {
	datalist->velocityCacheUsage[i]++;
	datalist->velocityBuffer = datalist->velocityCache[i];
	return(1);
      }
  }

  // no cache match, use LRU replacement
  i = VelViewCacheLRU();
  datalist->velocityBuffer = datalist->velocityCache[i];
  datalist->velocityCacheID[i] = method;
  datalist->velocityCacheParam[i] = datalist->velocityBufferParam;
  datalist->velocityCacheUsage[i]++;
  return(0);
}

/** internal function **/
int VelViewCacheLRU(void) {
  int min, minInd, i;
  min = 1000000;
  for (i=0; i<NUM_VEL_CACHE; i++) {
    if (datalist->velocityCacheUsage[i] < min) {
      min = datalist->velocityCacheUsage[i];
      minInd = i;
    }
  }
  return (minInd);
}


/**********************************************************
 * PATH VIEW
 **********************************************************/

/** internal function **/
void PathViewInit() {
  int i, j;

  // initial empty values
  datalist->pathTotal = 0;
  datalist->pathTotalFilled = 0;
  datalist->pathBufferViewed = 0;
  datalist->pathView = -1;
  datalist->pathClose = 1;
  datalist->pathVelChoice = 0;
  datalist->pathNum = -1;
  datalist->pathTotal = 0;
  for (i=0; i<MAX_PATH_NUM; i++)
    datalist->pathSymbol[i] = 0;

  // initial empty buffers
  for (i=0; i<NUM_VIEWS; i++) {
    for (j=0; j<3; j++)// x, y, z
      datalist->pathBuffers[i][j] = 0; 
  }
  PathViewEmptyBuffers();

  // create array of path pointers
  datalist->pathX = (float**)malloc(MAX_PATH_NUM*sizeof(float*));
  datalist->pathY = (float**)malloc(MAX_PATH_NUM*sizeof(float*));
  datalist->pathZ = (float**)malloc(MAX_PATH_NUM*sizeof(float*));
  datalist->pathXPrev = (float**)malloc(MAX_PATH_NUM*sizeof(float*));
  datalist->pathYPrev = (float**)malloc(MAX_PATH_NUM*sizeof(float*));
  datalist->pathZPrev = (float**)malloc(MAX_PATH_NUM*sizeof(float*));

  // possibly a file to be loaded at start-up
  PathViewAddFile();
}

/** internal function **/
void PathViewAddPath(float* x, float* y, float* z, long length, char symbol) {
  if (datalist->pathTotal == MAX_PATH_NUM) {
    UIDelayMessage("Path View: reached path storage limit.");
    return;
  }
  if (x == 0 || y == 0 || z == 0 || length <= 0) {
    return;
  }
  datalist->pathX[datalist->pathTotal] = x;
  datalist->pathY[datalist->pathTotal] = y;
  datalist->pathZ[datalist->pathTotal] = z;
  datalist->pathLength[datalist->pathTotal] = length;
  datalist->pathSymbol[datalist->pathTotal] = symbol;
  datalist->pathXPrev[datalist->pathTotal] = 0;
  datalist->pathYPrev[datalist->pathTotal] = 0;
  datalist->pathZPrev[datalist->pathTotal] = 0;
  datalist->pathLengthPrev[datalist->pathTotal] = 0;
  datalist->pathTotal++;
  datalist->pathTotalFilled++;
  PathViewInterpolate(datalist->pathTotal-1);
}

/** internal function **/
void PathViewReplacePath(float* x, float* y, float* z, long length, int position) {
  if (position >= datalist->pathTotal) {
    UIDelayMessage("Path View: accessing beyond path storage limit.");
    return;
  }
  if (datalist->pathLengthPrev[position] > 0) {
    FreeVector(datalist->pathXPrev[position], 1, datalist->pathLengthPrev[position]);
    FreeVector(datalist->pathYPrev[position], 1, datalist->pathLengthPrev[position]);
    FreeVector(datalist->pathZPrev[position], 1, datalist->pathLengthPrev[position]);
  }
  datalist->pathXPrev[position] = datalist->pathX[position];
  datalist->pathYPrev[position] = datalist->pathY[position];
  datalist->pathZPrev[position] = datalist->pathZ[position];
  datalist->pathLengthPrev[position] = datalist->pathLength[position];
  datalist->pathX[position] = x;
  datalist->pathY[position] = y;
  datalist->pathZ[position] = z;
  datalist->pathLength[position] = length;
  if (datalist->pathLength[position] == 0 && datalist->pathLengthPrev[position] > 0)
    datalist->pathTotalFilled--;
  if (datalist->pathLength[position] > 0 && datalist->pathLengthPrev[position] == 0)
    datalist->pathTotalFilled++;
  PathViewInterpolate(position);
}

/** internal function **/
int PathViewFindPath(char symbol) {
  int n;
  for (n=0; n<datalist->pathTotal; n++) {
    if (datalist->pathSymbol[n] == symbol) return(n);
  }
  return(NO_INDEX);
}

/** internal function **/
int PathViewCheckPickSymbol(char symbol, long* length) {
  PikList piks;
  long n;
  int i, iold;
  float x, y, z;
  string message;

  // check minimum length of sublist with symbol
  *length = 0;
  iold = current_view_num();
  for (i=0; i<datalist->ndata; i++) {
    SetActiveView(i);
    piks = current_view()->data->pik;
    for (n=0; n<piks->npik; n++) {
      x = AxisValue(DataAxis(current_view()->data,AXIS_ACROSS), piks->pik[n].loc[AXIS_ACROSS]);
      y = AxisValue(DataAxis(current_view()->data,AXIS_DOWN), piks->pik[n].loc[AXIS_DOWN]);
      z = AxisValue(DataAxis(current_view()->data,AXIS_DEEP), piks->pik[n].loc[AXIS_DEEP]);
      if (abs(x) < TINY &&  abs(y) < TINY && abs(z) < TINY) continue;
      if (piks->pik[n].symbol == symbol) (*length)++;
    }
  }
  SetActiveView(iold);
  if (*length < MIN_PATH_LENGTH) {
    sprintf(message, "Path View: need two picks of type %c to form path.", symbol);
    UIDelayMessage(message);
    return (CHECK_BAD);
  }
  return (CHECK_GOOD);
}

int PathViewUpdatePicks() {
  PikList piks;
  long nPik, nAdd, length;
  int found, i, j, *update, nPath, oldTotal;
  char symbol, oldSymbol;
  float *pathX, *pathY, *pathZ;

  // recheck all symbols recorded and the current one
  oldTotal = datalist->pathTotal;
  for (nPath = -1; nPath < oldTotal; nPath++) {
    // check if path is long enough
    if (nPath == -1) {
      symbol = current_view()->data->pik->cur_symbol;
      if (PathViewFindPath(symbol) != NO_INDEX) continue; // will be checked anyway
    }
    else
      symbol = datalist->pathSymbol[nPath];
    PathViewCheckPickSymbol(symbol, &length);

    if (length > 0) {
      // fill path vectors with pik locations from all views
      pathX = Vector(1, length);
      pathY = Vector(1, length);
      pathZ = Vector(1, length);
      nAdd = 1;
      j = current_view_num();
      for (i=0; i<datalist->ndata; i++) {
	SetActiveView(i);
	piks = current_view()->data->pik;
	for (nPik=0; nPik<piks->npik; nPik++) {
	  if (nAdd > length) break;
	  if (piks->pik[nPik].symbol == symbol) {
	    pathX[nAdd] = AxisValue(DataAxis(current_view()->data,AXIS_ACROSS), 
				    piks->pik[nPik].loc[AXIS_ACROSS]);
	    pathY[nAdd] = AxisValue(DataAxis(current_view()->data,AXIS_DOWN), 
				    piks->pik[nPik].loc[AXIS_DOWN]);
	    pathZ[nAdd] = AxisValue(DataAxis(current_view()->data,AXIS_DEEP), 
				    piks->pik[nPik].loc[AXIS_DEEP]);
	    if (abs(pathX[nAdd]) < TINY && abs(pathY[nAdd]) < TINY 
		&& abs(pathZ[nAdd]) < TINY) continue;
	    nAdd++;
	  }
	}
      }
      SetActiveView(j);
      
      // sort path to give most line-like representation
      PathViewSortPath(pathX, pathY, pathZ, length);
    }
    else {
      pathX = 0;
      pathY = 0;
      pathZ = 0;
    }
    
    // if path (for this symbol) doesn't exist, add it
    if ((found = PathViewFindPath(symbol)) == NO_INDEX)
      PathViewAddPath(pathX, pathY, pathZ, length, symbol);
    // if path does exist, replace it
    else
      PathViewReplacePath(pathX, pathY, pathZ, length, found);
  }

  // record paths to be repainted
  update = IVector(0,datalist->pathTotal-1);
  PathViewPathsChange(update);

  // repaint 
  for (i=0; i<datalist->pathTotal; i++) {
    if (update[i]) {
      PathViewUpdateBuffers(ERASE, i);
      UIMotionOff(NULL);
    }
  }
  for (i=0; i<datalist->pathTotal; i++) {
    if (update[i]) {
      PathViewUpdateBuffers(DRAW, i);
    }
  }

  // clean up memory
  FreeIVector(update, 0, datalist->pathTotal-1);
  return(0);
}

/** internal function **/
void PathViewPathsChange(int* update) {
  long n;
  int i, current;

  for (i=0; i<datalist->pathTotal; i++) update[i]=0;

  // update buffers for explicit changes
  for (i=0; i<datalist->pathTotal; i++) {
    update[i] = (datalist->pathLength[i] != datalist->pathLengthPrev[i]);
    if (update[i]) continue;
    for (n=1; n<=datalist->pathLength[i]; n++) {
      update[i] = (datalist->pathX[i][n] != datalist->pathXPrev[i][n] ||
		   datalist->pathY[i][n] != datalist->pathYPrev[i][n] ||
		   datalist->pathZ[i][n] != datalist->pathZPrev[i][n]);
      if (update[i]) break;
    }
  }

  // update buffers for other paths crossing current path's previous locations
  for (current=0; current<datalist->pathTotal; current++) {
    for (i=0; i<datalist->pathTotal && update[current]; i++) {
      if (i == current) continue;
      if ( PathViewPathsCross(datalist->pathXPrev[current],datalist->pathYPrev[current],
			      datalist->pathZPrev[current],datalist->pathLengthPrev[current],
			      datalist->pathX[i],datalist->pathY[i],datalist->pathZ[i],
			      datalist->pathLength[i]) ) update[i] = 1;
    }
  }
}

/** internal function **/
void PathViewSortPath(float* pathX, float* pathY, float* pathZ, long length) {
  long found, found1, i, j, k;
  int *order, used;
  float maxDist, minDist, dist;
  float *oldX, *oldY, *oldZ;

  // find point farthest away from other points
  maxDist = 0;
  found = 1;
  for (i=1; i<=length; i++) {
    dist = 0;
    for (j=1; j<=length; j++) {
      dist += sqrt( (pathX[i]-pathX[j])*(pathX[i]-pathX[j]) +
		    (pathY[i]-pathY[j])*(pathY[i]-pathY[j]) +
		    (pathZ[i]-pathZ[j])*(pathZ[i]-pathZ[j]) );
    }
    if (dist > maxDist) {
      found = i;
      maxDist = dist;
    }
  }

  // move from starting point to next closest point and repeat
  order = IVector(1,length);
  order[1] = found;
  for (i=2; i<=length; i++) {
    minDist = 1e9;
    for (j=1; j<=length; j++) {
      used = 0;
      for (k=1; k<i; k++) {
	if (order[k] == j) used = 1;
      }
      if (j == found || used) continue;
      dist = sqrt( (pathX[j]-pathX[found])*(pathX[j]-pathX[found]) +
		   (pathX[j]-pathX[found])*(pathX[j]-pathX[found]) +
		   (pathX[j]-pathX[found])*(pathX[j]-pathX[found]) );
      if (dist < minDist) {
	found1 = j;
	minDist = dist;
      }
    }
    found = found1;
    order[i] = found1;
  }

  // replace vectors
  oldX = Vector(1,length); for (i=1; i<=length; i++) oldX[i] = pathX[i];
  oldY = Vector(1,length); for (i=1; i<=length; i++) oldY[i] = pathY[i];
  oldZ = Vector(1,length); for (i=1; i<=length; i++) oldZ[i] = pathZ[i];
  for (i=1; i<=length; i++) {
    pathX[i] = oldX[order[i]];
    pathY[i] = oldY[order[i]];
    pathZ[i] = oldZ[order[i]];
  }
  FreeIVector(order,1,length);
  FreeVector(oldX,1,length);
  FreeVector(oldY,1,length);
  FreeVector(oldZ,1,length);
}

/** internal function **/
int PathViewPathsCross(float* x1, float* y1, float* z1, long length1,
		       float* x2, float* y2, float* z2, long length2) {
  long n1, n2;
  float dx, dy, dz, threshold;

  threshold = 10*PathViewGapThreshold();
  for (n1=1; n1<=length1; n1++) {
    for (n2=1; n2<=length2; n2++) {
      dx = x1[n1] - x2[n2];
      dy = y1[n1] - y2[n2];
      dz = z1[n1] - z2[n2];
      if (sqrt(dx*dx + dy*dy + dz*dz) < threshold) return(1);
    }
  }
  return(0);
}

/** internal function **/
int PathViewCheckFile(long* length) {
  int flag;
  long n1, n2;
  
  // check if path view intended
  if (0==rick_getch("path","d",&flag)) {
    return(CHECK_BAD);
  }

  // check dimensions
  auxpar("n1","d",&n1,"path");
  auxpar("n2","d",&n2,"path");
  if (n1 != 4) {
    UIDelayMessage("Path View: expecting quadruplets x, y, z, symbol.");
    printf("Path View: expecting quadruplets x, y, z, symbol. \n");
    return(CHECK_BAD);
  }

  // check minimum length
  else if (n2 < MIN_PATH_LENGTH) {
    UIDelayMessage("Path View: expecting path at least 2 elements long.");
    printf("Path View: expecting path at least 2 elements long. \n");
    return(CHECK_BAD);
  }

  *length = n2;
  return (CHECK_GOOD);
}
  
int PathViewAddFile() {
  float *path, *pathX, *pathY, *pathZ, x1, x2, y1, y2, z1, z2;
  long length, i, lengthReal;
  int j;

  // check validity of file
  if (PathViewCheckFile(&length) == CHECK_BAD) return(0);
  
  // allocate float array for file reading
  path = (float*)malloc(4*length*sizeof(float));

  // get axes' dimensions
  x1 = AxisFirst(DataAxis(datalist->data[0],AXIS_ACROSS));
  x2 = AxisLast(DataAxis(datalist->data[0],AXIS_ACROSS));
  y1 = AxisFirst(DataAxis(datalist->data[0],AXIS_DOWN));
  y2 = AxisLast(DataAxis(datalist->data[0],AXIS_DOWN));
  z1 = AxisFirst(DataAxis(datalist->data[0],AXIS_DEEP));
  z2 = AxisLast(DataAxis(datalist->data[0],AXIS_DEEP));
 
  // read in the path coordinates
  sreed("path",&path[0],4*length*sizeof(float));

  // separate into three arrays
  lengthReal = 0;
  for (i=0; i<length; i++) {
    if (path[4*i] < y1 || path[4*i] > y2) continue;
    if (path[4*i+1] < z1 || path[4*i+1] > z2) continue;
    if (path[4*i+2] < x1 || path[4*i+2] > x2) continue;
    lengthReal++;
  }

  // copy only bounded locations
  pathX = Vector(1,lengthReal);
  pathY = Vector(1,lengthReal);
  pathZ = Vector(1,lengthReal);
  lengthReal = 1;
  for (i=0; i<length; i++) {
    if (path[4*i] < y1 || path[4*i] > y2) continue;
    if (path[4*i+1] < z1 || path[4*i+1] > z2) continue;
    if (path[4*i+2] < x1 || path[4*i+2] > x2) continue;
    pathY[lengthReal] = path[4*i];
    pathZ[lengthReal] = path[4*i+1];
    pathX[lengthReal] = path[4*i+2];
    lengthReal++;
  }
  length = lengthReal-1;
  FREE(path);

  // add representative picks
  PathViewAddFilePick(pathX, pathY, pathZ, 1);
  for (j=1; j<6; j++) {
    if ((i = (long int)(j*0.16*length)) > 1)
      PathViewAddFilePick(pathX, pathY, pathZ, i);
  }
  PathViewAddFilePick(pathX, pathY, pathZ, length);

  // add a new path
  PathViewAddPath(pathX, pathY, pathZ, length, 'F'); 

  // update buffers and view
  PathViewUpdateBuffers(DRAW, 0);
  if (!PathViewOn())
    PathViewToggle();
  return(0);
}

/** interal function **/
void PathViewAddFilePick(float* pathX, float* pathY, float* pathZ, long pathInd) {
  int i, iold;
  PikList piks;

  piks = current_view()->data->pik;
  piks->cur_symbol = 'F';
  piks->pik[piks->npik].loc[AXIS_ACROSS] = 
    AxisIndex(DataAxis(current_view()->data,AXIS_ACROSS), pathX[pathInd]);
  piks->pik[piks->npik].loc[AXIS_DOWN] = 
    AxisIndex(DataAxis(current_view()->data,AXIS_DOWN), pathY[pathInd]);
  piks->pik[piks->npik].loc[AXIS_DEEP] = 
    AxisIndex(DataAxis(current_view()->data,AXIS_DEEP), pathZ[pathInd]);
  piks->pik[piks->npik].symbol = piks->cur_symbol;
  piks->pik[piks->npik].my_display = 10;
  if (piks->npik+1==piks->nmax) realloc_pik(piks);
  PikDraw(piks->npik-1,DRAW);
  piks->npik++;
  piks->last_op = PIKOP_ADD;
  piks->last_item = piks->npik-1;
  iold = current_view_num();
  for (i=0; i<num_views(); i++) {
    SetActiveView(i);
    current_view()->data->pik->change_pik_func();
  }
  SetActiveView(iold);
}

/** internal function **/
int PathViewEmpty(void) {
  return(datalist->pathTotalFilled == 0);
}
 
int PathViewView(void) {
  return (datalist->pathView);
}

int PathViewOn(void) {
  return (datalist->pathBufferViewed);
}

int PathViewClose(void) {
  return (datalist->pathClose);
}

long PathViewLength(void) {
  int found;
  if ((found = PathViewPathNum()) != NO_INDEX)
    return (datalist->pathLength[found]);
  else
    return (0);
}

int PathViewPathNum(void) {
  datalist->pathNum = PathViewFindPath(current_view()->data->pik->cur_symbol);
  return (datalist->pathNum);
}

int PathViewToggle(void) {
  if (PathViewOn())
    datalist->pathBufferViewed = 0;
  else
    datalist->pathBufferViewed = 1;

  if (PathViewOn() && datalist->pathVelChoice != VelViewNum()) {
    PathViewEmptyBuffers();
    PathViewUpdateBuffersAll(DRAW);
  }
  return(0);
}

int PathViewMoveCoordinates(float* y, float* x, float* z) {
  int found;
  found = PathViewPathNum();
  if (found == NO_INDEX) return(found);
  found = MIN_NUM(found, datalist->pathLength[found]);
  *x = datalist->pathX[found][datalist->pathMove[found]];
  *y = datalist->pathY[found][datalist->pathMove[found]];
  *z = datalist->pathZ[found][datalist->pathMove[found]];
  return(found);
}

int PathViewMoveStart(void) {
  int m;
  m = PathViewPathNum();
  if (PathViewOn() == 0 || m == NO_INDEX) return(0);

  datalist->pathMove[m] = 1;
  return(0);
}

int PathViewMoveMiddle(void) {
  int m;
  m = PathViewPathNum();
  if (PathViewOn() == 0 || m == NO_INDEX) return(0);

  datalist->pathMove[m] = MAX_NUM(datalist->pathLength[m]/2, 1);
  return(0);
}

int PathViewMoveEnd(void) {
  int m;
  m = PathViewPathNum();
  if (PathViewOn() == 0 || m == NO_INDEX) return(0);

  datalist->pathMove[m] = datalist->pathLength[m];
  return(0);
}

int PathViewMoveIncrement(void) {
  int m;
  m = PathViewPathNum();
  if (PathViewOn() == 0 || m == NO_INDEX) return(0);

  datalist->pathMoveDir[m] = PATH_MOVE_FORWARD;
  datalist->pathMove[m]++;
  if (datalist->pathMove[m] >= datalist->pathLength[m])
    datalist->pathMove[m] = 1;
  return(0);
}

int PathViewMoveDecrement(void) {
  int m;
  m = PathViewPathNum();
  if (PathViewOn() == 0 || m == NO_INDEX) return(0);

  datalist->pathMoveDir[m] = PATH_MOVE_BACKWARD;
  datalist->pathMove[m]--;
  if (datalist->pathMove[m] < 1)
    datalist->pathMove[m] = datalist->pathLength[m]-1;
  return(0);
}

Buffer PathViewBuffer(Data data, int pathDir) {
  int i;
  for (i=0; i<datalist->ndata; i++) {
    if (datalist->data[i] == data) {
      datalist->pathView = i;
      break;
    }
  }
  return (datalist->pathBuffers[datalist->pathView][pathDir]);
}

/** internal function **/
void PathViewUpdateBuffersAll(int mode) {
  int i;
  for (i=0; i<datalist->pathTotal; i++) {
    PathViewUpdateBuffers(mode, i);
  }
}

/** internal function **/
void PathViewUpdateBuffers(int mode, int pathNum) {
  int i;

  // update buffers in each direction, for each view
  for (i=0; i<datalist->ndata; i++) {
    datalist->pathView = i;
    PathViewUpdateBufferView(mode, pathNum);
  }
}

/** internal function **/
float PathViewGapThreshold(void) {
  float result, axisXRange, axisYRange, axisZRange, minRange;
  Axis axisX, axisY, axisZ;

  // calculate min axis range
  axisX = current_view()->data->axis[AXIS_ACROSS];
  axisY = current_view()->data->axis[AXIS_DOWN];
  axisZ = current_view()->data->axis[AXIS_DEEP];
  axisXRange = abs(AxisFirst(axisX) - AxisLast(axisX));
  axisYRange = abs(AxisFirst(axisY) - AxisLast(axisY));
  axisZRange = abs(AxisFirst(axisZ) - AxisLast(axisZ));
  minRange = MIN_NUM(axisXRange, axisYRange);
  minRange = MIN_NUM(minRange, axisZRange);

  // set gap threshold
  result = PATH_GAP_THRESHOLD;
  result *= MAX_NUM( 1.0, (minRange*minRange)/(4000*4000) );
  return (result);
}

/** internal function **/
void PathViewInterpolate(int pathNum) {
  float pathVecDiffMax, pathVecDiff, pathGapThreshold, alpha;
  float pathXPrev, pathYPrev, pathZPrev;
  float *splineX, *splineY, *splineZ;
  float *keepVec;
  long keepCount, n;
  int i, m;
  
  pathGapThreshold = PathViewGapThreshold();
  m = pathNum;
  if (m == NO_INDEX || datalist->pathLength[m] <= 1) return;

  // special case of path length = 2,3
  if (datalist->pathLength[m] == 2) {
    splineX = Vector(1,4); 
    splineY = Vector(1,4);
    splineZ = Vector(1,4);
    for (i=1; i<=4; i++) {
      alpha = (4-i)*0.3333;
      splineX[i] = alpha*datalist->pathX[m][1] + (1-alpha)*datalist->pathX[m][2];
      splineY[i] = alpha*datalist->pathY[m][1] + (1-alpha)*datalist->pathY[m][2];
      splineZ[i] = alpha*datalist->pathZ[m][1] + (1-alpha)*datalist->pathZ[m][2];
    }
    FreeVector(datalist->pathX[m], 1, datalist->pathLength[m]);
    FreeVector(datalist->pathY[m], 1, datalist->pathLength[m]);
    FreeVector(datalist->pathZ[m], 1, datalist->pathLength[m]);
    datalist->pathX[m] = splineX;
    datalist->pathY[m] = splineY;
    datalist->pathZ[m] = splineZ;
    datalist->pathLength[m] = 4;
  }
  else if (datalist->pathLength[m] == 3) {
    splineX = Vector(1,5); 
    splineY = Vector(1,5);
    splineZ = Vector(1,5);
    for (i=1; i<=3; i++) {
      alpha = (3-i)*0.5;
      splineX[i] = alpha*datalist->pathX[m][1] + (1-alpha)*datalist->pathX[m][2];
      splineY[i] = alpha*datalist->pathY[m][1] + (1-alpha)*datalist->pathY[m][2];
      splineZ[i] = alpha*datalist->pathZ[m][i] + (1-alpha)*datalist->pathZ[m][2];
    }
    for (i=4; i<=5; i++) {
      alpha = (5-i)*0.5;
      splineX[i] = alpha*datalist->pathX[m][2] + (1-alpha)*datalist->pathX[m][3];
      splineY[i] = alpha*datalist->pathY[m][2] + (1-alpha)*datalist->pathY[m][3];
      splineZ[i] = alpha*datalist->pathZ[m][2] + (1-alpha)*datalist->pathZ[m][3];
    }
    FreeVector(datalist->pathX[m], 1, datalist->pathLength[m]);
    FreeVector(datalist->pathY[m], 1, datalist->pathLength[m]);
    FreeVector(datalist->pathZ[m], 1, datalist->pathLength[m]);
    datalist->pathX[m] = splineX;
    datalist->pathY[m] = splineY;
    datalist->pathZ[m] = splineZ;
    datalist->pathLength[m] = 5;
  }

  // check max gap within path
  while (1) {
    pathVecDiffMax = MaxVectorGap(datalist->pathX[m], datalist->pathLength[m]);
    pathVecDiff = MaxVectorGap(datalist->pathY[m], datalist->pathLength[m]);
    pathVecDiffMax = MAX_NUM(pathVecDiffMax, pathVecDiff);
    pathVecDiff = MaxVectorGap(datalist->pathZ[m], datalist->pathLength[m]);
    pathVecDiffMax = MAX_NUM(pathVecDiffMax, pathVecDiff);

    // if dense enough, exit
    if (pathVecDiffMax < pathGapThreshold || 
	datalist->pathLength[m] > MAX_PATH_LENGTH) break;
    
    // generate splines 3 times as dense (3*length - 2 actually)
    splineX = Vector(1, datalist->pathLength[m]*3 - 2);
    splineY = Vector(1, datalist->pathLength[m]*3 - 2);
    splineZ = Vector(1, datalist->pathLength[m]*3 - 2);
    SplineThree(datalist->pathX[m], splineX, datalist->pathLength[m]);
    SplineThree(datalist->pathY[m], splineY, datalist->pathLength[m]);
    SplineThree(datalist->pathZ[m], splineZ, datalist->pathLength[m]);
    
    // replace path vectors with splines
    FreeVector(datalist->pathX[m], 1, datalist->pathLength[m]);
    datalist->pathX[m] = splineX;
    FreeVector(datalist->pathY[m], 1, datalist->pathLength[m]);
    datalist->pathY[m] = splineY;
    FreeVector(datalist->pathZ[m], 1, datalist->pathLength[m]);
    datalist->pathZ[m] = splineZ;
    datalist->pathLength[m] = 3*datalist->pathLength[m] - 2;
  }

  // equalize gaps along path
  keepCount = 0;
  keepVec = Vector(1, datalist->pathLength[m]);
  pathXPrev = 0; 
  pathYPrev = 0; 
  pathZPrev = 0;
  for (n=1; n<=datalist->pathLength[m]; n++) {
    pathVecDiff = pow(datalist->pathX[m][n] - pathXPrev, 2);
    pathVecDiff += pow(datalist->pathY[m][n] - pathYPrev, 2);
    pathVecDiff += pow(datalist->pathZ[m][n] - pathZPrev, 2);
    pathVecDiff = sqrt(pathVecDiff);
    // if greater than threshold, keep path element
    if (pathVecDiff > pathGapThreshold) {
      keepVec[n] = 1;
      keepCount++;
      pathXPrev = datalist->pathX[m][n];
      pathYPrev = datalist->pathY[m][n];
      pathZPrev = datalist->pathZ[m][n];
    }
    else {
      keepVec[n] = 0;
    }
  }
  splineX = Vector(1,keepCount);
  splineY = Vector(1,keepCount);
  splineZ = Vector(1,keepCount);
  keepCount = 1;
  for (n=1; n<=datalist->pathLength[m]; n++) {
    if (keepVec[n]) {
      splineX[keepCount] = datalist->pathX[m][n];
      splineY[keepCount] = datalist->pathY[m][n];
      splineZ[keepCount] = datalist->pathZ[m][n];
      keepCount++;
    }
  }

  // replace path vectors with down-sampled vectors 
  FreeVector(datalist->pathX[m], 1, datalist->pathLength[m]);
  datalist->pathX[m] = splineX;
  FreeVector(datalist->pathY[m], 1, datalist->pathLength[m]);
  datalist->pathY[m] = splineY;
  FreeVector(datalist->pathZ[m], 1, datalist->pathLength[m]);
  datalist->pathZ[m] = splineZ;
  datalist->pathLength[m] = keepCount-1;
}

/** internal function **/
void PathViewUpdateBufferView(int mode, int pathNum) {
  long long nBuffer, dataBufferSize, length;
  int nStart, nEnd, nPath, nXPath, nYPath, nZPath, m, blank;
  Buffer resultXY, resultYZ, resultXZ, dataBuffer;
  float maxRadius, pathX, pathY, pathZ, theta, polarDelta, rho, radiusDelta, delta1, delta2;
  float *pathXVec, *pathYVec, *pathZVec, dim;
  Axis axisX, axisY, axisZ;
  float axisXRange, axisYRange, axisZRange, minRange;

  // aliases
  if (VelViewOn() && datalist->pathView == 0)
    dataBuffer = datalist->velocityBuffer;
  else
    dataBuffer = datalist->data[datalist->pathView]->buffer;
  resultXY = datalist->pathBuffers[datalist->pathView][PATH_XY];
  resultYZ = datalist->pathBuffers[datalist->pathView][PATH_YZ];
  resultXZ = datalist->pathBuffers[datalist->pathView][PATH_XZ];
  axisX = datalist->data[datalist->pathView]->axis[AXIS_ACROSS];
  axisY = datalist->data[datalist->pathView]->axis[AXIS_DOWN];
  axisZ = datalist->data[datalist->pathView]->axis[AXIS_DEEP];
  dataBufferSize = datalist->data[datalist->pathView]->size;

  // ranges of axes
  axisXRange = abs(AxisFirst(axisX) - AxisLast(axisX));
  axisYRange = abs(AxisFirst(axisY) - AxisLast(axisY));
  axisZRange = abs(AxisFirst(axisZ) - AxisLast(axisZ));
  minRange = MIN_NUM(axisXRange, axisYRange);
  if (!data_is_2d(datalist->data[0]))
    minRange = MIN_NUM(minRange, axisZRange);

  // set neighborhood radius
  maxRadius = minRange * 0.16;
  radiusDelta = maxRadius * 0.16;

  // current path
  m = pathNum;
  if (m == NO_INDEX) return;

  // erase old path from x, y, z slices
  if (mode == ERASE) {
    pathXVec = datalist->pathXPrev[m];
    pathYVec = datalist->pathYPrev[m];
    pathZVec = datalist->pathZPrev[m];
    length = datalist->pathLengthPrev[m];
    dim = PATH_DIM;
    blank = (pathXVec == 0 || pathYVec == 0 || pathZVec == 0);
  } 
  // draw new paths onto x, y, z slices
  else {
    pathXVec = datalist->pathX[m];
    pathYVec = datalist->pathY[m];
    pathZVec = datalist->pathZ[m];
    length = datalist->pathLength[m];
    dim = 1.0;
    blank = (pathXVec == 0 || pathYVec == 0 || pathZVec == 0 || length <= 1);
  }

  for (nPath=1; nPath<=length && !blank; nPath++) {
    // current path coordinates
    pathX = pathXVec[nPath];
    pathY = pathYVec[nPath];
    pathZ = pathZVec[nPath];

    // check bounds
    nXPath = AxisIndex(axisX, pathX);
    if (nXPath == NO_INDEX) continue;
    nYPath = AxisIndex(axisY, pathY);
    if (nYPath == NO_INDEX) continue;
    nZPath = AxisIndex(axisZ, pathZ);
    if (nZPath == NO_INDEX) continue;

    // neighborhood in polar space
    for (rho=0; rho<=maxRadius; rho += radiusDelta) {
      polarDelta = PI/(rho*rho+TINY);
      polarDelta = MAX_NUM(POLAR_DELTA_LOW, polarDelta);
      polarDelta = MIN_NUM(POLAR_DELTA_HIGH, polarDelta);
      for (theta=0; theta<=2*PI; theta += polarDelta) {
	// create delta's
	delta1 = rho*cos(theta);
	delta2 = rho*sin(theta);
	
	// along z direction; x,y faces
	PathViewPaintDirectional(axisZ, axisX, axisY, axisZRange, dataBufferSize, resultXY, 
		      dataBuffer, nZPath, pathZ, pathX, pathY, delta1, delta2, dim);
	
	// along x direction; y,z faces
	PathViewPaintDirectional(axisX, axisY, axisZ, axisXRange, dataBufferSize, resultYZ, 
		      dataBuffer, nXPath, pathX, pathY, pathZ, delta1, delta2, dim);
	
	// along y direction; x,z faces
	PathViewPaintDirectional(axisY, axisX, axisZ, axisYRange, dataBufferSize, resultXZ, 
		      dataBuffer, nYPath, pathY, pathX, pathZ, delta1, delta2, dim);

      } // end theta
    } // end rho
  } // end nPath
}

/** internal function **/
void PathViewPaintDirectional(Axis axis1, Axis axis2, Axis axis3, float axisRange1, 
		   long long dataBufferSize, Buffer result, Buffer dataBuffer,
		   int n1Path, float path1, float path2, float path3, 
		   float deltaM, float deltaN, float dimFactor) {
  float fadeVariance, shrink, fade;
  int nStart, nEnd, n1, n2, n3;
  long long n, nNeighbor;
  float cube1, cube2, cube3;

  fadeVariance = 0.7*axisRange1;
  fadeVariance *= fadeVariance;
  if (datalist->pathClose) {
    nStart = AxisIndex(axis1, path1 - PATH_CLOSE_FACTOR*axisRange1);
    nStart = (nStart == NO_INDEX) ? 0 : nStart;
    nEnd = AxisIndex(axis1, path1 + PATH_CLOSE_FACTOR*axisRange1);
    nEnd = (nEnd == NO_INDEX) ? axis1->size-1 : nEnd;
  }
  else {
    nStart = 0;
    nEnd = axis1->size-1;
  }
  for (n1=nStart; n1<=nEnd; n1++) {
    // calculate shrink factor
    cube1 = AxisValue(axis1, n1);
    shrink = abs(path1 - cube1)/AxisLast(axis1);
    shrink = sqrt(shrink);
    shrink = MAX_NUM(MIN_SHRINK, shrink);
    cube2 = path2 + shrink*deltaM;
    n2 = AxisIndex(axis2, cube2);
    if (n2 == NO_INDEX) continue;
    cube3 = path3 + shrink*deltaN;
    n3 = AxisIndex(axis3, cube3);
    if (n3 == NO_INDEX) continue;
    
    // path neighborhood index
    nNeighbor = n1Path*axis1->stride + n2*axis2->stride + n3*axis3->stride;
    if (nNeighbor < 0 || nNeighbor >= dataBufferSize) continue;
    
    // projection index
    n = n1*axis1->stride + n2*axis2->stride + n3*axis3->stride;
    if (n < 0 || n >= dataBufferSize) continue;
    
    // assign value
    if (PathViewClose()) {
      result[n] = (unsigned int)(dimFactor * dataBuffer[nNeighbor]);
      result[n] = MAX_NUM(result[n], BUFFER_VALUE_LOW);
    }
    else {
      fade = (double)(path1 - cube1)*(path1 - cube1);
      fade /= fadeVariance;
      fade = pow((double)E, -fade);
      fade = MAX_NUM(MIN_FADE, fade);
      result[n] = (unsigned int)(dimFactor * dataBuffer[nNeighbor] * fade);
    }
  }
}

/** internal function **/
void PathViewEmptyBuffers(void) {
  int i, j;
  long long n;
  Buffer dataBuffer;

  DrawWatch(1);
  datalist->pathVelChoice = VelViewNum();
  for (i=0; i<datalist->ndata; i++) {
    dataBuffer = (VelViewOn() && i == 0) ? datalist->velocityBuffer : datalist->data[i]->buffer;
    for (j=0; j<3; j++) {
      if (datalist->pathBuffers[i][j] == 0)
	NEW(Buffer, datalist->pathBuffers[i][j], datalist->data[i]->size);
      for (n=0; n<datalist->data[i]->size; n++) {
	datalist->pathBuffers[i][j][n] = (unsigned int)(PATH_DIM*dataBuffer[n]);
	datalist->pathBuffers[i][j][n] = 
	  MAX_NUM(datalist->pathBuffers[i][j][n], BUFFER_VALUE_LOW);
      }
    }
  }
  DrawWatch(0);
}

/**********************************************************
 * MATH FUNCTIONS
 **********************************************************/

/** begin Numerical Recipes routines **/

int* IVector(long nl, long nh)
     /* allocate an int vector with subscript range v[nl..nh] */
{
  int *v;
  
  v=(int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(int)));
  if (!v) seperr("allocation failure in IVector");
  return v-nl+NR_END;
}

void FreeIVector(int *v, long nl, long nh)
     /* free an int vector allocated with ivector() */
{
    free((FREE_ARG) (v+nl-NR_END));
}

float* Vector(long nl, long nh)
     /* allocate a float vector with subscript range v[nl..nh] */
{
  float *v;
  
  v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
  if (!v) seperr("allocation failure in Vector");
  return v-nl+NR_END;
}

void FreeVector(float *v, long nl, long nh)
     /* free a float vector allocated with vector() */
{
  free((FREE_ARG) (v+nl-NR_END));
}

float **Matrix(long nrl, long nrh, long ncl, long nch)
     /* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
  long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
  float **m;
  
  /* allocate pointers to rows */
  m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
  if (!m) seperr("allocation failure 1 in Matrix");
  m += NR_END;
  m -= nrl;
  
  /* allocate rows and set pointers to them */
  m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
  if (!m[nrl]) seperr("allocation failure 2 in Matrix");
  m[nrl] += NR_END;
  m[nrl] -= ncl;
  
  for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;
  
  /* return pointer to array of pointers to rows */
  return m;
}

void FreeMatrix(float **m, long nrl, long nrh, long ncl, long nch)
     /* free a float matrix allocated by matrix() */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}

/** end Numerical Recipes routines **/

float MaxVectorGap(float* vec, long length) {
  long n;
  float pathVecDiffMax, pathVecDiff;

  pathVecDiffMax = 0;
  for (n=1; n<length; n++) {
    pathVecDiff = abs((double)(vec[n+1] - vec[n]));
    if (pathVecDiff > pathVecDiffMax) {
      pathVecDiffMax = pathVecDiff;
    }
  }
  return pathVecDiffMax;
}

float MeanVectorGap(float* vec, long length) {
  long n;
  float sum;

  sum = 0;
  for (n=1; n<length; n++)
    sum += abs((double)(vec[n+1]-vec[n]));
  return ( sum / (float)length );
}

void SplineThree(float* fVec, float* result, long length) {
  long n, m;
  float t, *f2Vec, *tVec;

  /** calculate second derivatives for spline **/
  tVec = Vector(1,length);
  f2Vec = Vector(1,length);
  for (n=1; n<=length; n++)
    tVec[n] = n;
  Spline(tVec, fVec, length, 0, 0, f2Vec);
  f2Vec[1] = f2Vec[2];
  f2Vec[length] = f2Vec[length-1];

  /** upsample by factor of three **/
  for (n=1; n<length; n++) {
    for (m=1; m<=3; m++) {
      t = tVec[n] + (m-1)*0.3333;
      SplineInterp(tVec, fVec, f2Vec, length, t, &result[(n-1)*3+m]); 
    }
  }
  result[3*length-2] = fVec[length];

  /** clean up memory **/
  FreeVector(tVec, 1, length);
  FreeVector(f2Vec, 1, length);
}

void SplineInterp(float xa[], float ya[], float y2a[], long n, float x, float *y)
{
    long klo,khi,k;
    float h,b,a;

    klo = 1;
    khi = n;
    /**
       We will find the right place in the table by means of bisection.
       This is optimal if sequential calls to this routine are at random
       values of x. If sequential calls are in order, and closely spaced,
       one would do better to store previous values of klo and khi and
       test if they remain appropriate on the next call. klo and khi now
       bracket the input value of x.
    **/
    while (khi - klo > 1) {
        k = (khi + klo) >> 1;
        if (xa[k] > x) khi = k;
        else klo = k;
    }
    h = xa[khi] - xa[klo];
    if (h == 0.0) seperr("Bad xa input to routine SplineInterp.");
    a = (xa[khi] - x)/h;
    b = (x - xa[klo])/h;
    *y = a*ya[klo] + b*ya[khi] + ((a*a*a-a)*y2a[klo]+(b*b*b-b)*y2a[khi])*(h*h)/6.0;
}

void Spline(float x[], float y[], long n, float yp1, float ypn, float y2[])
{
    long i,k;
    float p,qn,sig,un,*u;

    u=Vector(1,n-1);
    /**
       The lower boundary condition is set either to be natural or else
       to have a specified first derivative.
    **/
    if (yp1 > 0.99e30)
        y2[1]=u[1]=0.0;
    else {
        y2[1] = -0.5;
        u[1]=(3.0/(x[2]-x[1]))*((y[2]-y[1])/(x[2]-x[1])-yp1);
    }

    /**
       This is the decomposition loop of the tridiagonal algorithm. y2 and
       u are used for temporary storage of the decomposed factors.
    **/
    for (i=2; i<=n-1; i++) {
        sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
        p=sig*y2[i-1]+2.0;
        y2[i]=(sig-1.0)/p;
        u[i]=(y[i+1]-y[i])/(x[i+1]-x[i]) - (y[i]-y[i-1])/(x[i]-x[i-1]);
        u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
    }

    /**
       The upper boundary condition is set either to be natural or else
       to have a specified first derivative.
    **/
    if (ypn > 0.99e30)
        qn=un=0.0;
    else {
        qn=0.5;
        un=(3.0/(x[n]-x[n-1]))*(ypn-(y[n]-y[n-1])/(x[n]-x[n-1]));
    }
    y2[n]=(un-qn*u[n-1])/(qn*y2[n-1]+1.0);

    /** This is the backsubstitution loop of the tridiagonal algorithm. **/
    for (k=n-1;k>=1;k--)
        y2[k]=y2[k]*y2[k+1]+u[k];
    FreeVector(u,1,n-1);
}

void Histogram(Buffer buffer, long long bufferSize, float* histogram, int* mode) {
  long long n;
  float maxFraction;

  NEW(float*, histogram, 256);
  for (n=0; n<256; n++) histogram[n] = 0;
  for (n=0; n<bufferSize; n++) {
    histogram[buffer[n]]++;
  }

  maxFraction = 0;
  for (n=0; n<256; n++) {
    histogram[n] = histogram[n]/bufferSize;
    if (histogram[n] > maxFraction) {
      maxFraction = histogram[n];
      *mode = n;
    }
  }
}

void FindBufferMax(Buffer buffer, long long bufferSize, int* max) {
  long long n;
  *max = 0;
  for (n=0; n<bufferSize; n++) {
    if (buffer[n] > *max) {
      *max = buffer[n];
    }
  }
}

void FindBufferMean(Buffer buffer, long long bufferSize, int* mean) {
  long long n;
  double sum;
  sum = 0;
  for (n=0; n<bufferSize; n++) {
    sum = sum + buffer[n];
  }
  *mean = (double)sum/(double)bufferSize;
}

#endif
