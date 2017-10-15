#include<seplib.h>
#include<cfortran.h>



int scale_trace(int,float*,float);

FCALLSCFUN3(INT,scale_trace,SCALE_TRACE,scale_trace,INT,PFLOAT,FLOAT)

/*g77 adds an extra underscore */
FCALLSCFUN3(INT,scale_trace,SCALE_TRACE_,scale_trace_,INT,PFLOAT,FLOAT)









int scale_trace(int n,float *data, float scale)
{
 int i;

for(i=0; i < n; i++) data[i]=data[i]*scale;
return 0;
}

