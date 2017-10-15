
#include<stdio.h>
#include<sepaux.h>
#include <math.h>
#include<seplib.h>
int oc_patch_axis[3];
int oc_patch_nwind[3];
int oc_patch_npatch[3];
int oc_patch_naxes;
int oc_patch_ndim;
int oc_patch_nin[5];
int oc_patch_nout[8];
int **oc_patch_fpatch;
int oc_patch_npatches;
int **oc_patch_overlap;


void oc_patch_init(int ndim,int *nin, int *nwind, int *npatch, int *npatches)
{
  int i,iax,ip,overlap,id,car[3],last;
  int f[10000];
  if(ndim >5) seperr("ndim must be less than 6 to do pathing");
  oc_patch_npatches=1;
  oc_patch_ndim=ndim;
  for(i=0, oc_patch_naxes=0; i< ndim; i++){
    oc_patch_nin[i]=nin[i];
    oc_patch_nout[i]=nin[i];

    if(npatch[i] != 1){
      if(oc_patch_naxes==3) seperr("can only patch 3 axes");
      oc_patch_axis[oc_patch_naxes]=i;
      oc_patch_nwind[oc_patch_naxes]=nwind[i];
      oc_patch_npatch[oc_patch_naxes]=npatch[i];
      oc_patch_nout[i]=nwind[i];
      oc_patch_nout[oc_patch_ndim+oc_patch_naxes]=npatch[i];
      oc_patch_npatches=oc_patch_npatches*npatch[i];
      oc_patch_naxes++;
    }

    if(npatch[i] > nin[i]) seperr("axis=%d number of patches %d greater than n=%d \n",
      i+1,npatch[i],nin[i]);
    if(nwind[i] > nin[i]) seperr("axis=%d size of window %d greater than n=%d \n",
      i+1,nwind[i],nin[i]);
    if(nwind[i]*npatch[i]< nin[i]) seperr("patches doesn't extend the entire axis(%d) n=%d npatch=%d nwind=%d \n",i+1,nin[i],npatch[i],nwind[i]);
    if(npatch[i]>10000) seperr("can handle no more than 10000 patches along an axis");
  }
  *npatches=oc_patch_npatches;
/*  oc_patch_ndim=oc_patch_naxes+ndim;*/

  /*construct the patch windows*/
  oc_patch_fpatch=(int **) malloc(sizeof(int*)*oc_patch_npatches);
  for(i=0; i < oc_patch_npatches; i++){
    oc_patch_fpatch[i]=(int*)malloc(sizeof(int)*ndim);
    for(iax=0; iax< ndim;iax++) oc_patch_fpatch[i][iax]=0;
  } 

  oc_patch_overlap=(int **) malloc(sizeof(int*)*oc_patch_naxes);

  for(iax=0; iax < oc_patch_naxes; iax++){
    oc_patch_overlap[iax]=(int*)malloc(sizeof(int)*oc_patch_npatch[iax]);
    id=oc_patch_axis[iax];
    overlap=npatch[id]*nwind[id]-nin[id];
    last=0;f[0]=0;
    for(i=0; i < oc_patch_npatch[iax]-1; i++){
      oc_patch_overlap[iax][i]=ceilf((float)overlap/(float)(oc_patch_npatch[iax]-i-1));
      f[i+1]=last+nwind[id]-oc_patch_overlap[iax][i];
      last= f[i+1];
      overlap-=oc_patch_overlap[iax][i];
    }
    for(i=0; i < oc_patch_npatches;i++){
      h2c(i,oc_patch_npatch,oc_patch_naxes,car);
      oc_patch_fpatch[i][iax]=f[car[iax]];
    }
  }
}

int oc_patch_get(char *tag, int ipatch, int esize,char *buf)
{
  int fwind[5],jwind[5],nwind[5];
  int i;
  for(i=0; i < oc_patch_ndim; i++){
    fwind[i]=0; jwind[i]=1; nwind[i]=oc_patch_nout[i];
  }
  for(i=0; i < oc_patch_naxes;i++){
    fwind[oc_patch_axis[i]]= oc_patch_fpatch[ipatch][i];
    
    }
    for(i=0; i < oc_patch_ndim; i++){

}

   
   
   return(sreed_window(tag,&oc_patch_ndim,oc_patch_nin,nwind,fwind,jwind,esize,buf));
}

void oc_patch_clean()
{
int i;
for(i=0; i < oc_patch_naxes;i++) free(oc_patch_overlap[i]);
for(i=0; i < oc_patch_npatches;i++) free(oc_patch_fpatch[i]);
free(oc_patch_fpatch);
free(oc_patch_overlap);
}



int oc_patch_put(char *tag, int ipatch, int esize,char *buf)
{
  int fwind[5],jwind[5],nwind[5];
  int i;
  for(i=0; i < oc_patch_ndim; i++){
    fwind[i]=0; jwind[i]=1; nwind[i]=oc_patch_nout[i];
  }
  for(i=0; i < oc_patch_naxes;i++)
    fwind[oc_patch_axis[i]]=oc_patch_fpatch[ipatch][i];

   return(srite_window(tag,&oc_patch_ndim,oc_patch_nin,nwind,fwind,jwind,esize,buf));
}

void oc_patch_bound(int ipatch, int esize, float *buf)
{
int iax,i1,i2,i3,i,is;
int car[3],nre[3];
float *scale;



if(esize!=4 && esize!=8) seperr("can only handle esize=4 or esize=8");
h2c(ipatch,oc_patch_npatch,oc_patch_naxes,car);
for(iax=0; iax < oc_patch_naxes; iax++){
   
   /* reshape data (before axis, axis, after axis)*/
   nre[0]=esize/4; nre[1]=1; nre[2]=1;
   for(i=0; i < oc_patch_ndim; i++){
     if(i < oc_patch_axis[iax]) nre[0]=nre[0]*oc_patch_nout[i];
     else if(i == oc_patch_axis[iax]) nre[1]=oc_patch_nout[i];
     else  nre[2]=nre[2]*oc_patch_nout[i];
   }

   
   if(car[iax]!=0) {  /*we need to do scale beg*/
     scale=(float*) malloc(sizeof(float)*oc_patch_overlap[iax][car[iax]-1]);
     for(i=0; i < oc_patch_overlap[iax][car[iax]-1];i++){
       scale[i]= ((float)(i+1))/((float)(1+oc_patch_overlap[iax][car[iax]-1]));
     }
     
     for(i3=0; i3 < nre[2]; i3++){
       for(is=0,i2=0; i2 <oc_patch_overlap[iax][car[iax]-1] ; i2++,is++){
         for(i1=0; i1< nre[0]; i1++){
           i=i1+i2*nre[0]+i3*nre[1]*nre[0];
           buf[i]=buf[i]*scale[is];
         }
       }
     }
     free(scale); 
   }

   if(car[iax]<oc_patch_npatch[iax]-1) {  /*we need to do scale end*/
     scale=(float*) malloc(sizeof(float)*oc_patch_overlap[iax][car[iax]]);
     for(i=0; i < oc_patch_overlap[iax][car[iax]];i++){
       scale[oc_patch_overlap[iax][car[iax]]-1-i]= 
         ((float)(i+1))/((float)(oc_patch_overlap[iax][car[iax]]+1));

   }
     
     for(i3=0; i3 < nre[2]; i3++){
       for(is=0,i2=nre[1]-oc_patch_overlap[iax][car[iax]] ; i2 < nre[1]; i2++,is++){
         for(i1=0; i1< nre[0]; i1++){
           i=i1+i2*nre[0]+i3*nre[1]*nre[0];
           buf[i]=buf[i]*scale[is];
         }
       }
     }
     free(scale); 
   }
} 

}


