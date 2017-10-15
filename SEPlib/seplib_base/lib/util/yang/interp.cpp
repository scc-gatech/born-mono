#include "stdafx.h"
#include "cubeData.hpp"
#include "interp.h"
#include <sepfilter.h>

using namespace Yang;
using namespace seplib;


interp* interp::_instance = 0;

/**
 * Returns the single instance of the object.
 */
interp* interp::getInstance() {
   // check if the instance has been created yet
   if (NULL == _instance) {
      // if not, then create it
      _instance = new interp;
   }
   // return the single instance.
   return _instance;
};



#define YES 1
#define NO 0

float* space=NULL;

//interpolate a 1d/2d/3d RSF incore data object, do it all in memory, please see seplib Interp.c
//Current support only float type
//assume that dimension does not change between in and out
void interp::interpolate(cubeData<float>& out, cubeData<float>& in, int* pn1out, float* po1out,
                         float *pd1out, int* pn2out, float* po2out, float* pd2out,int* pn3out, float *po3out,float *pd3out,
                         interpType type, int verb, int lsinc, int ntab, int maxsize)
//if the axis info is missing, then keep the same parameter as the input 
{ 
  int ndim = in.domain().get_ndim();
  in.resetCounter();
   
  //The shape of out will be set within this function
   
  //Start of the function 
	int i;
	float *data, *idata, *sinc, **psinc;
	char temp_ch[256];
	int n,n1,n2,n3,n1out,n2out,n3out,nbig;
	float o,d,d1out,d2out,d3out,o1out,o2out,o3out,o1,o2,o3,d1,d2,d3;
	int do_axis[3];
	int *iaxis1,*iaxis2,*iaxis3;
	int *iaxis1NN,*iaxis2NN,*iaxis3NN; //For nearest neighbor
	int *spt1,*spt2,*spt3;
	float *faxis1,*faxis2,*faxis3;
	int nlen1,nlen2,nlen3;
	float *input,*output;
	int in_size,out_size;
	int n1_lin,n2_lin,n3_lin;
	int n1_lout,n2_lout,n3_lout;
	int a,b,c,esize,nmax,unit;
	int loc1,loc2,loc3;
	int iloc1,iloc2,iloc3;
	float f1,f2,f3,loc,val;
	int i1,i2,i3,i4;
	int b3,b2,b1,e3,e2,e1;
   //Init the pointers
   iaxis1 = NULL; iaxis2 = NULL; iaxis3=NULL;
   spt1 = spt2 = spt3 = NULL;
   faxis1 = faxis2 = faxis3 = NULL;
   input = output = NULL;
   data = idata = sinc = space = NULL;
   psinc = NULL; //this is a ** pointer
   
   float dsinc = 0.0f;
   
   /* information about input data set */
	esize=sizeof(float);
	if(esize!=4) seperr("Can only deal with float data \n");
	nbig=1;
	o2=0;o3=0;d2=1;d3=1;n2=1;n3=1;
	n1=1;o1=0;d1=1;
   vector<axis> axes = in.domain().get_axes();
	for(i=1;i<=ndim;i++){
      n = axes[i-1].n;
      o = axes[i-1].o;
      d = axes[i-1].d;
	  if(d<=0) {
		 seperr("\n d<=0 !!in interpolate func.\n");
	  }
      if(i==1){ n1=n;o1=o; d1=d;}
		else if(i==2){ n2=n;o2=o; d2=d;}
		else if(i==3){ n3=n;o3=o; d3=d;}
		else nbig=nbig*n;
	}
	/* get interpolation parameters */
	if(type> seplib::sinc || type<nearest) seperr("Invalid type (0-nint 1-linear 2-sync)\n");
	do_axis[0]=NO; do_axis[1]=NO; do_axis[2]=NO;
   if(po1out==NULL) 
      o1out = o1;
   else
   {
      o1out = *po1out;
      do_axis[0] = YES;
   }
   if(pd1out==NULL) 
      d1out = d1;
   else
   {
      d1out = *pd1out;
      do_axis[0] = YES;
   }
   if(po2out==NULL) 
      o2out = o2;
   else
   {
      o2out = *po2out;
      do_axis[1] = YES;
   }
   if(pd2out==NULL) 
      d2out = d2;
   else
   {
      d2out = *pd2out;
      do_axis[1] = YES;
   }
   if(po3out==NULL) 
      o3out = o3;
   else
   {
      o3out = *po3out;
      do_axis[2] = YES;
   }
   if(pd3out==NULL) 
      d3out = d3;
   else
   {
      d3out = *pd3out;
      do_axis[2] = YES;
   }
   
	if(pn1out==NULL) n1out=int((o1+d1*(n1-1)-o1out)/d1out+1.5); 
	//!!!Different from the original code, where it is +1.0 not +1.5
	
	else { n1out=*pn1out; do_axis[0]=YES;}
	if(pn2out==NULL) n2out=int((o2+d2*(n2-1)-o2out)/d2out+1.5);
	else { n2out=*pn2out; do_axis[1]=YES;}
	if(pn3out==NULL) n3out=int((o3+d3*(n3-1)-o3out)/d3out+1.5);
	else { n3out=*pn3out; do_axis[2]=YES;}
	
	maxsize=maxsize*1000000;
   //The domain of outspace is decided
   vector<axis> axes_out;
   axes_out.push_back(axis(n1out,o1out,d1out));
   if (ndim>=2) {axes_out.push_back(axis(n2out,o2out,d2out));}
   if (ndim>=3) {axes_out.push_back(axis(n3out,o3out,d3out));}
   if (ndim>3) {
	  vecAxis axesBig = in.domain().return_axes(4,ndim);
	  axes_out.insert(axes_out.end(),axesBig.begin(),axesBig.end());
   }
   assert(axes_out.size()==(size_t)ndim);
   hypercube  outShape(axes_out);
   //Set the shape of out
   out.alloc(outShape);
   
   
	/*take care of the output space */
	if(do_axis[0]==NO && do_axis[1]==NO && do_axis[2]==NO)	
		seperr("You aren't interpolating along any axis \n");
	if(type==nearest) strcpy(temp_ch,"nearest neighbor");
	else if(type==linear) strcpy(temp_ch,"linear interpolation");
	else sprintf(temp_ch,"sinc interpolation with %d points",lsinc);
	//putlin(temp_ch);
	if(verb==1) fprintf(stderr,"%s\n",temp_ch);
	if(do_axis[0]==YES){
		sprintf(temp_ch,"n1=%d o1=%f d1=%f",n1out,o1out,d1out);
		//VERB_PRV(temp_ch);
		//putlin(temp_ch);if(verb==1) fprintf(stderr,"%s\n",temp_ch);
	}
	if(do_axis[1]==YES){
		sprintf(temp_ch,"n2=%d o2=%f d2=%f",n2out,o2out,d2out);
		//VERB_PRV(temp_ch);
		//putlin(temp_ch);if(verb==1) fprintf(stderr,"%s\n",temp_ch);
	}
	if(do_axis[2]==YES){
		sprintf(temp_ch,"n3=%d o3=%f d3=%f",n3out,o3out,d3out);
		//VERB_PRV(temp_ch)
		//putlin(temp_ch);if(verb==1) fprintf(stderr,"%s\n",temp_ch);
	}
		
   //Calculate the size of the interpolation chunk
   
	if(do_axis[2]==NO){ 
		nbig=n3*nbig;
		nmax=2; 
		if(do_axis[1]==NO){
			nbig=nbig*n2; nmax=1; in_size=n1;out_size=n1out;
			n2_lin=1; n3_lin=1;
			n2_lout=1; n3_lout=1;
		}
		else{
			in_size=(n1*n2);out_size=(n1out*n2out); 
			n3_lin=1; n2_lin=n2;
			n3_lout=1; n2_lout=n2out;
		}
	}	
	else{ 
      nmax = 3;
      in_size=(n1*n2*n3);out_size=n1out*n2out*n3out;
		n3_lout=n3out; n2_lout=n2out;
		n3_lin=n3; n2_lin=n2;
	}
	n1_lout=n1out;
	n1_lin=n1;
   
   
   
   vector<axis> axes_chunkIn, axes_chunkOut;
   axes_chunkOut.push_back(axis(n1out,o1out,d1out));
   axes_chunkIn.push_back(axis(n1,o1,d1));
   
   if (nmax>=2){
          axes_chunkOut.push_back(axis(n2out,o2out,d2out));
          axes_chunkIn.push_back(axis(n2,o2,d2));
   }
   if (nmax>=3){
      axes_chunkOut.push_back(axis(n3out,o3out,d3out));
      axes_chunkIn.push_back(axis(n3,o3,d3));
   }
   hypercube shapeUpdateChunkIn(axes_chunkIn);
   hypercube shapeUpdateChunkOut(axes_chunkOut);

   //Alloc two cube data, and then read from chunk in,
   //and after interpolation, update to chunk out;
   cubeData<float> chunkIn, chunkOut;
   chunkIn.init(&shapeUpdateChunkIn); chunkOut.init(&shapeUpdateChunkOut);
   
      
   unit=in_size+out_size;
	if(unit*esize  > maxsize) 
		seperr("maxsize is not big enough, must be %d with current pars\n",
     unit*esize/1000/1000);
   
	/*NOW LETS ALLOCATE THE TABLES*/
	iaxis1=(int*) malloc(n1out*sizeof(int));
	iaxis2=(int*) malloc(n2out*sizeof(int));
	iaxis3=(int*) malloc(n3out*sizeof(int));
	
	//index for nearest neighbor
	iaxis1NN=(int*) malloc(n1out*sizeof(int));
	iaxis2NN=(int*) malloc(n2out*sizeof(int));
	iaxis3NN=(int*) malloc(n3out*sizeof(int));
	
	if(type==linear){
		faxis1=(float*) malloc(n1out*sizeof(float));
		faxis2=(float*) malloc(n2out*sizeof(float));
		faxis3=(float*) malloc(n3out*sizeof(float));
		if(do_axis[0]==YES)nlen1=2; else nlen1=1;	
		if(do_axis[1]==YES)nlen2=2; else nlen2=1;	
		if(do_axis[2]==YES)nlen3=2; else nlen3=1;	
	}
	else if(type==seplib::sinc){
		spt1=(int*) malloc(n1out*sizeof(int));
		spt2=(int*) malloc(n2out*sizeof(int));
		spt3=(int*) malloc(n3out*sizeof(int));
		
		dsinc=1./(ntab-1);
		if(do_axis[0]==YES)nlen1=lsinc; else nlen1=1;	
		if(do_axis[1]==YES)nlen2=lsinc; else nlen2=1;	
		if(do_axis[2]==YES)nlen3=lsinc; else nlen3=1;	
	}
	else{ nlen1=1; nlen2=1; nlen3=1;}
   
	// DO POINTERS FOR AXIS 1 
	for(i=0; i< n1out; i++){
		loc=((o1out+d1out*i)-o1)/d1;
		iaxis1[i]=int(loc);
		iaxis1NN[i]=lroundf(loc);
		if(do_axis[0]==YES){
	 		if(type==linear) faxis1[i]=loc-iaxis1[i];
			else if(type==seplib::sinc) spt1[i]=int(((loc-iaxis1[i])/dsinc)+.5);
		}
		else{
	 		if(type==linear) faxis1[i]=0.;
			else if(type==seplib::sinc) spt1[i]=1;
		}
	}

	//DO POINTERS FOR AXIS 2
	for(i=0; i< n2out; i++){
		loc=((o2out+d2out*i)-o2)/d2;
		iaxis2[i]=int(loc);
		iaxis2NN[i]=lroundf(loc);
		if(do_axis[1]==YES){
	 		if(type==seplib::linear) faxis2[i]=loc-iaxis2[i];
			else if(type==seplib::sinc) spt2[i]=int(((loc-iaxis2[i])/(dsinc))+.5);
		}
		else{
	 		if(type==seplib::linear) faxis2[i]=0.;
			else if(type==seplib::sinc) spt2[i]=1;
		}
	}

	/* DO POINTERS FOR AXIS 3 */
	for(i=0; i< n3out; i++){
		loc=((o3out+d3out*i)-o3)/d3;
		iaxis3[i]=int(loc);
		iaxis3NN[i]=lroundf(loc);
		
		if(do_axis[2]==YES){
	 		if(type==seplib::linear) faxis3[i]=loc-iaxis3[i];
			else if(type==seplib::sinc) spt3[i]=int(((loc-iaxis3[i])/(dsinc))+.5);
		}
		else{
	 		if(type==seplib::linear) faxis3[i]=0.;
			else if(type==seplib::sinc) spt3[i]=1;
		}
	}

	if(verb) fprintf(stderr,"Finished constructing pointers \n");
  
  float *sinc_table = this->sinc_table;
  //Generate tables
	if(type==seplib::sinc){
    //first deal with possible boundary problem
		for(i=0;i<n1out;i++)if(spt1[i]>=ntab ||  spt1[i]<0) spt1[i]=0;
		for(i=0;i<n2out;i++)if(spt2[i]>=ntab ||  spt2[i]<0) spt2[i]=0;
		for(i=0;i<n3out;i++)if(spt3[i]>=ntab ||  spt3[i]<0) spt3[i]=0;
    
    //This sinc table should be reused, since generating it every call is very expensive.
    //reuse the table if lsinc and ntab and dsinc doesnot changed
    
    if (this->bSincTableBuilt && lsinc == this->lsinc_last && ntab == this->ntab_last) {
      //don't need to rebuild the sinc talbe
    } else 
    { //need to rebuild the sinc table
      this->lsinc_last = lsinc; this->ntab_last = ntab; this->bSincTableBuilt = true;
      if (sinc_table) {
        free(sinc_table);
        free(space);
      }
      space = (float *) alloc ( lsinc * 3 * sizeof(float) );
      sinc_table=(float*) malloc(lsinc*sizeof(float)*ntab);
      /* contruct sinc table */
      for(i=0; i< ntab; i++){
        d=i*dsinc;
        mksinc( &sinc_table[i*lsinc], lsinc, d, space);
      }
      if(verb) {
        fprintf(stderr,"finished constructing sinc table of size %d by %d\n",lsinc,ntab);
      }
      this->sinc_table = sinc_table;
    }
	}

	/*now it is time to get to work doing interpolation */
	//input=(float*)malloc(in_size*sizeof(float));
	//output=(float*)malloc(out_size*sizeof(float));
   
   output = chunkOut.pval;
   input  = chunkIn.pval;
   
   for(i4=0; i4 < nbig; i4++){
      //Read in the cubeData from in
      in.seq_grab(& chunkIn);
		/*NEAREST NEIGHBOR CASE */
		if(type==seplib::nearest){
			for(i3=0; i3 < n3_lout; i3++){
				iloc3=MIN(MAX(iaxis3NN[i3],0),n3_lin-1);
				for(i2=0; i2 < n2_lout; i2++){
					iloc2=MIN(MAX(iaxis2NN[i2],0),n2_lin-1);
					for(i1=0; i1 < n1_lout; i1++){
						iloc1=MIN(MAX(iaxis1NN[i1],0),n1_lin-1);
						output[i1+n1_lout*i2+i3*n1_lout*n2_lout]=
							input[iloc1+n1_lin*iloc2+iloc3*n1_lin*n2_lin];
					}
				}
			}
		}
		else if(type==seplib::linear){
			for(i3=0; i3 < n3_lout; i3++){
				b3=MAX(MIN(iaxis3[i3],n3_lin-1),0);
				e3=MIN(MAX(iaxis3[i3]+1,0),n3_lin-1);
				for(i2=0; i2 < n2_lout; i2++){
					b2=MAX(MIN(iaxis2[i2],n2_lin-1),0);
					e2=MIN(MAX(iaxis2[i2]+1,0),n2_lin-1);
					for(i1=0; i1 < n1_lout; i1++){
						b1=MAX(MIN(iaxis1[i1],n1_lin-1),0);
						e1=MIN(MAX(iaxis1[i1]+1,0),n1_lin-1);
						output[i1+n1_lout*i2+i3*n1_lout*n2_lout]=
							(1.-faxis1[i1])*(1-faxis2[i2])*(1.-faxis3[i3])*
							input[b1+b2*n1_lin+b3*n1_lin*n2_lin]+
							(faxis1[i1])*(1-faxis2[i2])*(1.-faxis3[i3])*
							input[e1+b2*n1_lin+b3*n1_lin*n2_lin]+
							(1.-faxis1[i1])*(faxis2[i2])*(1.-faxis3[i3])*
							input[b1+e2*n1_lin+b3*n1_lin*n2_lin]+
							(faxis1[i1])*(faxis2[i2])*(1.-faxis3[i3])*
							input[e1+e2*n1_lin+b3*n1_lin*n2_lin]+
							(1.-faxis1[i1])*(1.-faxis2[i2])*(faxis3[i3])*
							input[b1+b2*n1_lin+e3*n1_lin*n2_lin]+
							(faxis1[i1])*(1.-faxis2[i2])*(faxis3[i3])*
							input[e1+b2*n1_lin+e3*n1_lin*n2_lin]+
							(1.-faxis1[i1])*(faxis2[i2])*(faxis3[i3])*
							input[b1+e2*n1_lin+e3*n1_lin*n2_lin]+
							(faxis1[i1])*(faxis2[i2])*(faxis3[i3])*
							input[e1+e2*n1_lin+e3*n1_lin*n2_lin];
					}
				}
			}
		}
		else{  /* sinc interpolation */
      //cerr<<"n321_lout:"<<n3_lout<<' '<<n2_lout<<' '<<n1_lout<<' '<< endl;
      //cerr<<"nlen321:"<<nlen3<<' '<<nlen2<<' '<<nlen1<<' '<< endl;
			for(i3=0; i3 < n3_lout; i3++){
				for(i2=0; i2 < n2_lout; i2++){
					for(i1=0; i1 < n1_lout; i1++){
						val=0.;
						for(c=0; c<nlen3; c++){
							if(nlen3==1){ 
								f3=1; loc3=i3;
							}
							else{
								loc3=iaxis3[i3]+c-lsinc/2+1;
								loc3=MIN(MAX(loc3,0),n3_lin-1);
								f3=sinc_table[spt3[i3]*lsinc+c];
							}
							for(b=0; b<nlen2; b++){
								if(nlen2==1){ 
									f2=1.;
									loc2=i2;
								}
								else{
									f2=sinc_table[spt2[i2]*lsinc+b];
									loc2=iaxis2[i2]+b-lsinc/2+1;
									loc2=MIN(MAX(loc2,0),n2_lin-1);
								}
/*								fprintf(stderr,"check (%d %d) %d %f %d\n",
									i1,i2,loc2,f2,spt2[i2]);*/
								for(a=0; a<nlen1; a++){
									if(nlen1==1){ 
										f1=1.;
										loc1=i1;
									}
									else{ 
										f1=sinc_table[spt1[i1]*lsinc+a];
										loc1=iaxis1[i1]+a-lsinc/2+1;
										loc1=MIN(MAX(loc1,0),n1_lin-1);
									}
/*									fprintf(stderr,"check me %d %d %d %f %f %f\n",
										i1,i2,i3,f1,f2,f3);*/
									val+=f1*f2*f3*input[loc1+loc2*n1_lin+
										loc3*n2_lin*n1_lin];
								}
							}
						}
						output[i1+i2*n1_lout+i3*n1_lout*n2_lout]=val;
					}
				}
			}
		}

      //Here we update the out data object
      out.seq_update(&chunkOut);
		//if(out_size*esize!=srite("out",output,out_size*esize))
	}
   
 //Remember to free all the object allocated here
   free(iaxis1); free(iaxis2); free(iaxis3);
   free(iaxis1NN); free(iaxis2NN); free(iaxis3NN);
   
   free(faxis1); free(faxis2); free(faxis3);
   free(spt1); free(spt2); free(spt3);
   free(data);free(idata);free(sinc);
   //For the last one, !! ** psinc

   in.resetCounter();
   out.resetCounter();
   
   return;
}
