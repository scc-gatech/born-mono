/*$
Self-doc area

scale_it.x

Usage
scale_it.x <input.H scale=  >output.H


Input Parameters:
 scale - real scale value

Output Parameters:

Description

	Intro program that simply scales N-D data

>*/
/*

Author: Robert Clapp, ESMB 463, 7230253

Date Created:Mon Jul 5  8:50:57 PDT 1997
               esize=1
*/

/* standard seplib include and macro for setting up main programs */

#include<sep.main>
#include <cfortran.h>


/*we need to prototype are calls to fortran*/

/*we need to prototype are calls to fortran*/
#if defined(LINUX)
PROTOCCALLSFSUB3(SCALE_TRACE,scale_trace_,INT,FLOATV,FLOAT)
#define SCALE_TRACE(n,data,scale) CCALLSFSUB3(SCALE_TRACE,scale_trace_,INT,FLOATV,FLOAT,n,data,scale)
#else
PROTOCCALLSFSUB3(SCALE_TRACE,scale_trace_,INT,FLOATV,FLOAT)
#define SCALE_TRACE(n,data,scale) CCALLSFSUB3(SCALE_TRACE,scale_trace,INT,FLOATV,FLOAT,n,data,scale)
#endif


MAIN()
{
int i1,i2;
int n[2],tempi,esize,ndim,mem,logic=0;
float *data,scale;
char temp_ch[3];

/* call to get information from history file */
if(0==hetch("esize","d",&esize)) esize=4;

/* Error checking mechanism */
if(esize!=4 && esize !=8) 
	seperr("Unacceptable esize value \n");

/* Get the number of data axes */
if(0!=sep_get_number_data_axes("in",&ndim)) 
	seperr("Trouble obtaining number of data axes \n");

n[1]=1;mem=1;

if(0==hetch("n1","i",&n[0])) 
	seperr("can not obtain n1 required for SEP datasets\n");

/*read in cube dimensions, calculating what size subcube we
	can reasonably read in at one time */

for(i1=1;i1<ndim;i1++){
	sprintf(temp_ch,"%c%d",'n',i1+1);
	if(0==hetch(temp_ch,"i",&tempi)) 
		seperr("trouble obtaining %s from the dataset \n",temp_ch);
	if(mem * tempi < 2000000 && logic==0){
		mem=mem*tempi; n[0]=n[0]*tempi;
	}
	else{
		logic=1; n[1]=n[1]*tempi;
	}
}

/* fake a real dataset if data is complex */
n[0]=n[0]*esize/4;

/* obtain scale factor from the command line */
if(0==getch("scale","f",&scale)) 
	seperr("trouble obtaining scale factor. \n");

/* write scale factor to output history file */
if(0!=putch("scale","f",&scale)) 
	seperr("trouble putting scale factor into output history file. \n");

/* close the output history file */
hclose();


/* allocate storage array */
data= (float *) malloc (n[0] * sizeof(float));


/* loop over calculated subcubes */
for(i2=0; i2 < n[1]; i2++){
	
	/* read in data  from stdin ("in")*/
	if(n[0]*4 != sreed("in", data , n[0] *4))
		seperr("trouble reading in data \n");

	
	
	/* scale the data */
		SCALE_TRACE(n[0],data,scale);
	
	/* write out data  to stdout ("out")*/
	if(n[0]*4 != srite("out", data , n[0] *4))
		seperr("trouble writing  data \n");

}
	
/* free memory */
free(data);
}
