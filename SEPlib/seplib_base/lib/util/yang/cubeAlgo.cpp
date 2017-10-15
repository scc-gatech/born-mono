#include "stdafx.h"
#include "cubeData.hpp"
#include "interp.h"
#include "cubeAlgo.h"
#include "cubeAlgo2.h"

using namespace Yang;
using std::vector;
using std::pair;

//Calculate cross correlation 1D along lag tau, notice it is unsymmetric. out cube will be allocated inside.
//The time lag needs to go from -tau -> tau, we don't use any normalization or wrap-up 
//compensation here
//Assume input cube are ensemble of 1D traces.
//Formula C(tau)[in1,in2] = sigma_{t}{in1(t-tau)in2(t)} = sigma_{t}{in1(t)in2(t+tau)}, here assume both input traces
//starts from index=0
void Yang::crossCorr1d(cubeData<float>& out, const cubeData<float>& in1, const cubeData<float>& in2,
				 int istartLag, int iendLag, hypercube* pOutShape) 
{
   assert(in1.sameSize(in2));
   assert(iendLag >= istartLag);
   const hypercube& shape = in1.domain();
   int nTrc = shape.get_n123() / shape.get_n(1);
   //VERIFY(nTrc == in2.domain().get_n123() / in2.domain().get_n(1));
   
   vector<axis> axes = shape.get_axes();
   float d = shape.get_d(1);
   int nt = shape.get_n(1);
   axis ax_tau(iendLag-istartLag+1,istartLag*d,d,"lag","time(sec)");
   axes[0] = ax_tau;
   hypercube shapeOut(axes);
   
   if (pOutShape) { //Just query the output size.
     *pOutShape = shapeOut;
     return;
   }
   
   out.alloc(shapeOut);
   long long i;
   const float *p1,*p2,*p11,*p22;
   float *p3,*p33;
   const float *pin1,*pin2;
   float *pout;
   
   pin1 = in1.getDataPtr();
   pin2 = in2.getDataPtr();
   pout = out.getDataPtr();
   
   int it,itau;
   float sum;
   
   int startLag,endLag;
   //Note that the starting origin of in1 and in2 might not be the same.
   //But Assume in1 and in2 are aligned in time samples.
   startLag = istartLag;
   endLag   = iendLag;
   
   #pragma omp parallel for private(p1,p2,p3,p11,p22,p33,it,itau,sum)
   for(i=0; i<nTrc; i++)
   {
	  p1 = pin1+i*nt;
	  p2 = pin2+i*nt;
	  p3 = pout+i*ax_tau.n;
	 
	  //We can further optimize this by moving the if cond out of the #pragma loop
	  if (startLag<=0 && endLag<=0) {
		 p33 = p3;
		 for(itau=startLag;itau<=endLag;itau++)
		  {
			 p11 = p1; p22 = p2;
			 sum = 0.0;
			 //Do cross-correlation
			 for(it=0;it<nt+itau;it++)
			 {
				sum += *(p11-itau) * (*p22);
				p11++; p22++;
			 }
			 *p33 = sum;
			 p33++;
		 }
	  }
	  
	  if (startLag > 0 && endLag > 0) {
		 p33 = p3;
		 for(itau=startLag;itau<=endLag;itau++)
		  {
			 p11 = p1; p22 = p2;
			 sum = 0.0;
			 //Do cross-correlation
			 for(it=0;it<nt-itau;it++)
			 {
				sum += *(p11) * (*(p22+itau));
				p11++; p22++;
			 }
			 *p33 = sum;
			 p33++;
		 }
	  }
	  
	  if (startLag<=0 && endLag>0) {
		 p33 = p3;
		 for(itau=startLag;itau<=0;itau++)
		  {
			 p11 = p1; p22 = p2;
			 sum = 0.0;
			 //Do cross-correlation
			 for(it=0;it<nt+itau;it++)
			 {
				sum += *(p11-itau) * (*(p22));
				p11++; p22++;
			 }
			 *p33 = sum;
			 p33++;
		 }
		 
		for(itau=1;itau<=endLag;itau++)
		  {
			 p11 = p1; p22 = p2;
			 sum = 0.0;
			 //Do cross-correlation
			 for(it=0;it<nt-itau;it++)
			 {
				sum += *(p11) * (*(p22+itau));
				p11++; p22++;
			 }
			 *p33 = sum;
			 p33++;
		 }
	  }
   }
   
   //scale the cube by dt
   float dt = shape.get_d(1);
   out.scale(dt);
}


//Calculate cross correlation 1D along lag tau, notice it is unsymmetric. out cube will be allocated inside.
//The time lag needs to go from -tau -> tau, we don't use any normalization or wrap-up 
//compensation here
//Assume input cube are ensemble of 1D traces.
//Formula C(tau)[in1,in2] = sigma_{t}{in1(t-tau)in2(t)} = sigma_{t}{in1(t)in2(t+tau)} 
//input sequence may start from different origin, and may have different trace lengths.
// input 1(or 2) can be one single trace or has the same number of traces as input 2(or 1).
void Yang::crossCorr1dGeneral(cubeData<float>& out, const cubeData<float>& in1, const cubeData<float>& in2,
				 int istartLag, int iendLag, hypercube* pShapeOut) 
{
   assert(iendLag >= istartLag);
   bool bIn2SingleTrace = (1 == in2.domain().get_n23());
   bool bIn1SingleTrace = (1 == in1.domain().get_n23());
   const hypercube * pShape = NULL;
   pShape = &(in1.domain());
   if (bIn1SingleTrace & !bIn2SingleTrace) {pShape = &(in2.domain()); }
   const hypercube& shape = *pShape;
   
   int nTrc = shape.get_n23();
   if (!bIn2SingleTrace && !bIn1SingleTrace) {	//Then In2 should have same number of traces as in1
	  VERIFY(nTrc == (in2.domain().get_n23()));
   }
   
   vector<axis> axes = shape.get_axes();
   float d = shape.get_d(1);
   axis ax_tau(iendLag-istartLag+1,istartLag*d,d,"lag","time(sec)");
   axes[0] = ax_tau;
   hypercube shapeOut(axes);
   if (pShapeOut) {
     *pShapeOut = shapeOut; //output the shape of the output cube.
     return;
   }
   out.alloc(shapeOut);
   
   int nt1 = in1.domain().get_n(1);
   int nt2 = in2.domain().get_n(1);

   long long i;
   const float *p1,*p2,*p11,*p22;
   float *p3,*p33;
   const float *pin1,*pin2;
   float *pout;
   
   pin1 = in1.getDataPtr();
   pin2 = in2.getDataPtr();
   pout = out.getDataPtr();
   
   int it,itau;
   float sum;
   
   int startLag,endLag;
   bool lb1,ub1;
   int	shiftLow;
   
   //Note that the starting origin of in1 and in2 might not be the same.
   //But Assume in1 and in2 are aligned in time samples.
   int shift = lroundf((in1.domain().get_o(1)-in2.domain().get_o(1))/d);
   
   startLag = istartLag;
   endLag   = iendLag;
   
   
   #pragma omp parallel for private(p1,p2,p3,p11,p22,p33,it,itau,sum,lb1,ub1,shiftLow)
   for(i=0; i<nTrc; i++)
   {
	  if (bIn1SingleTrace) {
		 p1 = pin1; 
	  } else {
		 p1 = pin1+i*nt1;
	  }
	  
	  if (bIn2SingleTrace) {
		 p2 = pin2;
		 }
	  else {
		 p2 = pin2+i*nt2;
	  }
	  
	  p3 = pout+i*ax_tau.n;

	  p33 = p3;
	  for(itau=istartLag;itau<=iendLag;itau++)
	  {
		 //summation variable has a upper bound and a lower bound
		 if(shift+itau >=0)	{
			lb1 = true;
			shiftLow = shift+itau;
			//the Lower bound of it is o1+itau, then in2 traces starts at non-zero lag shiftLow
		 } else
		 {
			lb1 = false;
			shiftLow = -(shift+itau); //Now in1 trace starts at non-zero lag
			//the lower bound of it is o2
		 }

		 //calc upper bound
		 int tmp = itau + shift + nt1 - nt2;
		 if (tmp <= 0) {
			ub1 = true;
			//the upper bound of "it" is itau+end1, then in2 traces ends in the middle defined by shiftHigh
		 } else
		 {
			ub1 = false;
			//the upper bound of "it" is end2, in1 traces ends in the middle; 
		 }
		 
		 p11 = p1; p22 = p2;
		 sum = 0.0;
		 if (lb1 && ub1) {
			for (it = 0; it < nt1; it++) {
			   sum += *(p11) * *(p22+shiftLow);
			   p11++; p22++;
			}
		 }
		 if (lb1 && !ub1) {
			  for (it = 0; it < (nt2-itau-shift); it++) {
				 sum += *(p11) * *(p22+shiftLow);
				 p11++; p22++;
			  }
		   }
		 
		 if (!lb1 && ub1) {
			  for (it = 0; it < (nt1+shift+itau); it++) {
				 sum += *(p11+shiftLow) * *(p22);
				 p11++; p22++;
			  }
		   }
		 
		 if (!lb1 && !ub1) {
			  for (it = 0; it < nt2; it++) {
				 sum += *(p11+shiftLow) * *(p22);
				 p11++; p22++;
			  }
		   }
	  *p33 = sum;
	  p33++;
	  }
   }
   
   //scale the cube by dt
   float dt = shape.get_d(1);
   out.scale(dt);
}


//calc X(z)/A(z)=Y(z) 1D
//formula a0*y(n)=x(n)-(a1*y(n-1)+a2*y(n-2)+...+ak*y(n-k))
void Yang::polydiv(cubeData<float>& arrayY, const cubeData<float>& arrayX, const cubeData<float>& arrayA, int resultLen) {
	assert(arrayX.domain().get_ndim()==1);
	assert(arrayA.domain().get_ndim()==1);
	axis ax = arrayX.domain().get_axis(1);
	ax.n = resultLen;
	
	vector<axis> axes(1,ax);
	arrayY.alloc(hypercube(axes));
	//Get the data pointers
	const float *pX,*pA;
	float *pY;
	pX = arrayX.getDataPtr();
	pA = arrayA.getDataPtr();
	pY = arrayY.getDataPtr();
	
	int i,j,jmax;
	float sum;
	//asset a0 is not equal to zero
	VERIFY(!equalf(pA[0],0.0f));

	for(i=0;i<resultLen;i++)
	{
		jmax = MIN(arrayA.domain().get_n(1)-1,i);
		sum = 0.0f;
		for(j=1;j<=jmax;j++)
		{
			sum += pA[j] * pY[i-j];
		}
		pY[i] = 1.0/pA[0]* (pX[i]-sum);
	}
}

//Multiply each n-dimension slice by certain scaling factor
//For example out(x,y,z,w) = in(x,y,z,w) * scaler(z,w)
void Yang::scaleSlice(cubeData<float>& outTrcCube, const cubeData<float>& trcCube, const cubeData<float>& scalerCube)
{	
	int ndimScaler = scalerCube.domain().get_ndim();
	int ndim = trcCube.domain().get_ndim();
	int ndimSlice = ndim - ndimScaler;
   int nSlice = trcCube.domain().get_n23(ndim-ndimScaler+1,ndim);
	assert(nSlice == scalerCube.domain().get_n123());
	assert(outTrcCube.sameSize(trcCube));
	assert(ndimSlice >= 1);
  
	//Get the data pointers
	float *pData,*p;
	
	const float* pScaler; const float* pInData; const float* pIn;
	
	pData   = outTrcCube.getDataPtr();	
	pInData = trcCube.getDataPtr();
	pScaler = scalerCube.getDataPtr();
	
	long long i;
	int j,nt;
	nt = trcCube.domain().get_n23(1,ndimSlice);
	float fscale;
	//May use openmp here
	for(i=0;i<nSlice;i++)
	{
	   p = (pData+i*nt);
	   pIn = (pInData+i*nt);
	   fscale = *(pScaler+i);
	   for(j=0;j<nt;j++)
	   {
		  *(p+j) = *(pIn+j) * fscale;
	   }
	}
}

//Multiply ONE n-dimension slice, notice the difference from scaleSlice
//For example. out(x,y,z,w) = in(x,y,z,w)*slice(x,y);
void Yang::multiplySlice(cubeData<float>& outCube, const cubeData<float>& inCube, const cubeData<float>& sliceCube)
{	
   FUNC_TRACE_BEG(Yang::mutiplySlice);
	int ndimSlice = sliceCube.domain().get_ndim();
	assert(inCube.domain().compatible(sliceCube.domain()));
	int ndim = inCube.domain().get_ndim();
    VERIFY(ndimSlice < ndim);
    int nSlice = inCube.domain().get_n23(ndimSlice+1,ndim);
	assert(outCube.sameSize(inCube));
	
	//Get the data pointers
	float *pData,*p;
	
	const float* pScaler; const float* pInData; const float* pIn;
	
	pData   = outCube.getDataPtr();	
	pInData = inCube.getDataPtr();
	pScaler = sliceCube.getDataPtr();
	
	long long i;
	int j,nt;
	nt = inCube.domain().get_n23(1,ndimSlice);
	//May use openmp here
	for(i=0;i<nSlice;i++)
	{
	   p = (pData+i*nt);
	   pIn = (pInData+i*nt);
	   for(j=0;j<nt;j++)
	   {
		  *(p+j) = *(pIn+j) * pScaler[j];
	   }
	}
    FUNC_TRACE_END()
}

//Multiply n-dimension slice
//For example, given iAxisBeg=2, iAxisEnd=3, out(x,y,z,w) = in(x,y,z,w)*slice(y,z);
void Yang::multiplySubCube(cubeData<float>& outCube, const cubeData<float>& inCube, const cubeData<float>& sliceCube, int iAxisBeg, int iAxisEnd)
{	
   FUNC_TRACE_BEG(Yang::mutiplySubCube)
	assert(iAxisBeg >=1 && iAxisEnd >= iAxisBeg);
	//divide the shape into 3 hyper axis, nPlanes, nLines, nt;
	const hypercube& shape = inCube.domain();
	int ndimSlice = sliceCube.domain().get_ndim();
	VERIFY(iAxisEnd-iAxisBeg+1 == ndimSlice);
	
	int ndim = shape.get_ndim();
  assert(ndimSlice < ndim);
	assert(outCube.sameSize(inCube));
	
	if (iAxisEnd == ndim ) {
		scaleSlice(outCube,inCube,sliceCube);
		return;
	}
	if (iAxisBeg == 1) {
		multiplySlice(outCube,inCube,sliceCube);
		return;
	}
	
  int nPlanes = shape.get_n23(iAxisEnd+1,ndim);
	int nLines = shape.get_n23(iAxisBeg,iAxisEnd);
	assert(nLines == sliceCube.domain().get_n123());
	int nt = shape.get_n23(1,iAxisBeg-1);
	
	//Get the data pointers
	float *pData,*p;
	const float* pScaler; const float* pInData; const float* pIn;
	
	pData   = outCube.getDataPtr();	
	pInData = inCube.getDataPtr();
	pScaler = sliceCube.getDataPtr();
	
	long long i;
	int j,k;
	//May use openmp here
	for(i=0;i<nPlanes;i++)
	{
	   p = (pData+i*nt*nLines);
	   pIn = (pInData+i*nt*nLines);
	   for(j=0;j<nLines;j++)
	   {
			 for (k=0; k<nt; k++) {
				p[j*nt+k] = pIn[j*nt+k] * pScaler[j];
			 }
	   }
	}
  FUNC_TRACE_END()
}

//Multiply a trc by certain scaling factor
void Yang::scaleTrc(cubeData<float>& outTrcCube, const cubeData<float>& trcCube, const cubeData<float>& scalerCube)
{	
   assert(trcCube.domain().get_ndim() - scalerCube.domain().get_ndim() == 1);
   Yang::scaleSlice(outTrcCube,trcCube,scalerCube);
}

void Yang::scaleTrc(cubeData<float>& trcCube, const cubeData<float>& scalerCube)
{	
   scaleTrc(trcCube,trcCube,scalerCube);
}

//calc 1D gradient along axis1, this calculation has 2nd order precision, using center scheme
void Yang::timeDeriv1st(cubeData<float>& resultCube, const cubeData<float>& trcCube, bool bScaleBy_dt)
{
   //same size of result Cube
   assert(resultCube.sameSize(trcCube));
   int ntrc = trcCube.domain().get_n23();
	
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn, *pi;
	
	pIn   = trcCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	long long i;
	int j,nt;
	nt = trcCube.domain().get_n(1);
	
	//May use openmp here
	#pragma omp parallel for private(pi,po,j)
	for(i=0;i<ntrc;i++)
	{
	   pi = (pIn+(i)*nt);
	   po = (pOut+(i)*nt);
	   //Do the first one, use one-side finite stencil (-3*f0+4*f1-f2)/2.0
	   *po = (-3.0f*pi[0] + 4.0*pi[1] - pi[2]) / 2.0;
	   for(j=1;j<nt-1;j++)
	   {
		  po[j] = (pi[j+1]-pi[j-1])/2.0;
	   }
	   //Do the last one.
	   po[nt-1] = (3*pi[nt-1] - 4*pi[nt-2] + pi[nt-3]) / 2.0;
	}
	
	if(bScaleBy_dt) {
	   float dt = trcCube.domain().get_d(1);
	   float inv_dt = 1.0/dt;
	   resultCube.scale(inv_dt);
	}
}


//calc 2nd derivative along axis1, this calculation has 2nd order precision, using center scheme
//for 2nd derivative, the scheme of endpoints result in the same values as its neighbours.
void Yang::timeDeriv2nd(cubeData<float>& resultCube, const cubeData<float>& trcCube,
						bool bScaleBy_dt)
{
   //Alloc same size of result Cube
   assert(trcCube.sameSize(resultCube));
   
   int ntrc = trcCube.domain().get_n23();
	
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn;
	const float* pi;
	
	pIn   = trcCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	
	long long i;
	int j,nt;
	nt = trcCube.domain().get_n(1);
	
	//May use openmp here
	#pragma omp parallel for private(pi,po,j)
	for(i=0;i<ntrc;i++)
	{
	   pi = (pIn+i*nt);
	   po = (pOut+i*nt);
	   
	   for(j=1;j<nt-1;j++)
	   {
		  po[j] = (pi[j+1] -2*pi[j] +pi[j-1]);
		  //prt("%f,%f,%f\n",pi[j],pIn[j],po[j]);
	   }
	   //Do the first&last one.
	   po[0] = po[1];
	   po[nt-1] = po[nt-2];
	}
	
	//divide dt^2
	if(bScaleBy_dt) {
		float dt = trcCube.domain().get_d(1);
		float inv_dt = 1.0 / SQ(dt);
		resultCube.scale(inv_dt);
	 }
}

//calc 2nd derivative along certain axis, this calculation has 2nd order precision, using center scheme
//for 2nd derivative, the scheme of endpoints result in the same values as its neighbours. Not optimized.
//Axis starts from 1 to ndim
void Yang::deriv2nd(cubeData<float>& resultCube, const cubeData<float>& inCube, int iAxis, bool bScaleBy_d)
{
   //Alloc same size of result Cube
   assert(inCube.sameSize(resultCube));
	const hypercube& shape = inCube.domain();
   assert(iAxis >= 1 && iAxis<= shape.get_ndim());

	//Along the axis, divide the domain into three hyper axis. iPlane, iLine, and it
	int nPlanes,nt,nLine;
	if (iAxis != shape.get_ndim()) {
		nPlanes = shape.get_n23(iAxis+1,shape.get_ndim());
	} else {
		nPlanes=1;
	}
	if (iAxis != 1) {
		nt = shape.get_n23(1,iAxis-1);
	} else {
		nt = 1;
	}
	nLine = shape.get_n(iAxis); //The derivative operates on nLine axis.
	
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn;
	const float* pi;
	
	pIn   = inCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	
	long long i;
	int j,k;
	
	//May use openmp here
	//#pragma omp parallel for private(pi,po,j)
	for(i=0;i<nPlanes;i++)
	{
	   pi = (pIn+i*nLine*nt);
	   po = (pOut+i*nLine*nt);
	   
		//for each Line
	   for(j=1;j<nLine-1;j++)
	   {
			for(k=0;k<nt;k++) {
				po[j*nt+k] = (pi[(j+1)*nt+k] -2*pi[j*nt+k] +pi[(j-1)*nt+k]);
			}
	   }
		
		for (k=0; k<nt; k++) {
			//Do the first&last one.
			po[0*nt+k] = po[1*nt+k];
			po[(nLine-1)*nt+k] = po[(nLine-2)*nt+k];
		}
	}
	//divide dt^2
	if(bScaleBy_d) {
		float dt = inCube.domain().get_d(iAxis);
		float inv_dt = 1.0 / SQ(dt);
		resultCube.scale(inv_dt);
	 }
}

//calc 1st derivative along certain axis, this calculation has 2nd order precision, using center scheme
//for 1st derivative, the scheme of endpoints result in the same values as its neighbours. Not optimized.
//Axis starts from 1 to ndim
void Yang::deriv1st(cubeData<float>& resultCube, const cubeData<float>& inCube, int iAxis, bool bScaleBy_d)
{
   //Alloc same size of result Cube
   assert(inCube.sameSize(resultCube));
	const hypercube& shape = inCube.domain();
   assert(iAxis >= 1 && iAxis<= shape.get_ndim());

	//Along the axis, divide the domain into three hyper axis. iPlane, iLine, and it
	int nPlanes,nt,nLine;
	if (iAxis != shape.get_ndim()) {
		nPlanes = shape.get_n23(iAxis+1,shape.get_ndim());
	} else {
		nPlanes=1;
	}
	if (iAxis != 1) {
		nt = shape.get_n23(1,iAxis-1);
	} else {
		nt = 1;
	}
	nLine = shape.get_n(iAxis); //The derivative operates on nLine axis.
	
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn;
	const float* pi;
	
	pIn   = inCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	
	long long i;
	int j,k;
	
	//May use openmp here
	//#pragma omp parallel for private(pi,po,j)
	for(i=0;i<nPlanes;i++)
	{
	   pi = (pIn+i*nLine*nt);
	   po = (pOut+i*nLine*nt);
	   
		//for each Line
	   for(j=1;j<nLine-1;j++)
	   {
			for(k=0;k<nt;k++) {
				po[j*nt+k] = (pi[(j+1)*nt+k] - pi[(j-1)*nt+k]) / 2.0f;
			}
	   }
		
		for (k=0; k<nt; k++) {
			//Do the first&last one.
			po[0*nt+k] =          (-3.0f* pi[0*nt+k] + 4.0* pi[k+1*nt] - pi[k+2*nt]) / 2.0f;
			po[(nLine-1)*nt+k] =  ( 3.0f* pi[(nLine-1)*nt+k] - 4.0* pi[(nLine-2)*nt+k] + pi[(nLine-3)*nt+k]) / 2.0f;
		}
	}
	//divide dt
	if(bScaleBy_d) {
		float dt = inCube.domain().get_d(iAxis);
		float inv_dt = 1.0 / dt;
		resultCube.scale(inv_dt);
	 }
}

//compute the dotprod of a(x,y,z) and b(y) along axis 2, result in c(x,z)
void Yang::dotProdAlongAxis(cubeDataF& resultCube, const cubeDataF& inCube, const cubeDataF& dot2, int iAxis) {
  FUNC_TRACE_BEG(dotProdAlongAxis);
  //Just require the result cube have the same size as ticked out the designed ax iAxis;
	const hypercube& shape = inCube.domain();
  resultCube.zero();
  axis ax = shape.get_axis(iAxis);
  assert(iAxis >= 1 && iAxis<= shape.get_ndim());
  
  vecAxis axes = shape.get_axes();
  axes.erase(axes.begin()+iAxis-1);
  hypercube outShape(axes);
  assert(resultCube.domain().sameSize(outShape));
  assert(dot2.domain().get_n123() == ax.n);
  
	//Along the axis, divide the domain into three hyper axis. iPlane, iLine, and it
	int nPlanes,nt,nLine;
	if (iAxis != shape.get_ndim()) {
		nPlanes = shape.get_n23(iAxis+1,shape.get_ndim());
	} else {
		nPlanes=1;
	}
	if (iAxis != 1) {
		nt = shape.get_n23(1,iAxis-1);
	} else {
		nt = 1;
	}
	nLine = shape.get_n(iAxis); //The dot prod operate on nLine axis.
	//PRV(nPlanes);PRV(nLine);PRV(nt);PRV(index);
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn;
	const float* pi;
	
	pIn   = inCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	long long i;
	int j,k;
	//May use openmp here
	//#pragma omp parallel for private(pi,po,j)
	for(i=0;i<nPlanes;i++)
	{
	  pi = (pIn+i*nLine*nt);
	  po = (pOut+i*nt);
	  
    for (j=0;j<nLine;j++) {
      //for the pt index on this Line
      for(k=0;k<nt;k++) {
        po[k] += pi[j*nt+k] * dot2[j];
      }
    }
	}
  FUNC_TRACE_END();
}

//calc 1st derivative along certain axis, this calculation has 2nd order precision, using center scheme
//for 1st derivative, but just at one point at that axis.
//Axis starts from 1 to ndim, but index is 0 based.
void Yang::deriv1stAtIndex(cubeData<float>& resultCube, const cubeData<float>& inCube, int iAxis, int index, bool bScaleBy_d)
{
   //Just require the result cube have the same size as ticked out the designed ax iAxis;
	const hypercube& shape = inCube.domain();
  axis ax = shape.get_axis(iAxis);
  assert(iAxis >= 1 && iAxis<= shape.get_ndim());
  assert(index >= 1 && index< ax.n-1); //edge point is not allowed.
  
  vecAxis axes = shape.get_axes();
  axes.erase(axes.begin()+iAxis-1);
  hypercube outShape(axes);
  assert(resultCube.domain().sameSize(outShape));
  
	//Along the axis, divide the domain into three hyper axis. iPlane, iLine, and it
	int nPlanes,nt,nLine;
	if (iAxis != shape.get_ndim()) {
		nPlanes = shape.get_n23(iAxis+1,shape.get_ndim());
	} else {
		nPlanes=1;
	}
	if (iAxis != 1) {
		nt = shape.get_n23(1,iAxis-1);
	} else {
		nt = 1;
	}
	nLine = shape.get_n(iAxis); //The derivative operates on nLine axis.
	//PRV(nPlanes);PRV(nLine);PRV(nt);PRV(index);
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn;
	const float* pi;
	
	pIn   = inCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	long long i;
	int j,k;
	
	//May use openmp here
	//#pragma omp parallel for private(pi,po,j)
	for(i=0;i<nPlanes;i++)
	{
	  pi = (pIn+i*nLine*nt);
	  po = (pOut+i*nt);
	   
	  //for the pt index on this Line
	  j = index;
    for(k=0;k<nt;k++) {
      po[k] = (pi[(j+1)*nt+k] - pi[(j-1)*nt+k]) / 2.0f;
    }
	}
	
	//LOG(ll2);
	//divide dt
	if(bScaleBy_d) {
		float dt = inCube.domain().get_d(iAxis);
		float inv_dt = 1.0 / dt;
		resultCube.scale(inv_dt);
	 }
}

//calc 2nd derivative along certain axis, this calculation has 2nd order precision, using center scheme
//for 2nd derivative, but just at one point at that axis.
//Axis starts from 1 to ndim, but index is 0 based.
void Yang::deriv2ndAtIndex(cubeData<float>& resultCube, const cubeData<float>& inCube, int iAxis, int index, bool bScaleBy_d)
{
   //Just require the result cube have the same size as ticked out the designed ax iAxis;
	const hypercube& shape = inCube.domain();
  axis ax = shape.get_axis(iAxis);
  assert(iAxis >= 1 && iAxis<= shape.get_ndim());
  assert(index >= 1 && index< ax.n-1); //edge point is not allowed.
  
  vecAxis axes = shape.get_axes();
  axes.erase(axes.begin()+iAxis-1);
  hypercube outShape(axes);
  assert(shape.get_n123() == outShape.get_n123());
  
	//Along the axis, divide the domain into three hyper axis. iPlane, iLine, and it
	int nPlanes,nt,nLine;
	if (iAxis != shape.get_ndim()) {
		nPlanes = shape.get_n23(iAxis+1,shape.get_ndim());
	} else {
		nPlanes=1;
	}
	if (iAxis != 1) {
		nt = shape.get_n23(1,iAxis-1);
	} else {
		nt = 1;
	}
	nLine = shape.get_n(iAxis); //The derivative operates on nLine axis.
	
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn;
	const float* pi;
	
	pIn   = inCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	long long i;
	int j,k;
	
	//May use openmp here
	//#pragma omp parallel for private(pi,po,j)
	for(i=0;i<nPlanes;i++)
	{
	  pi = (pIn+i*nLine*nt);
	  po = (pOut+i*nt);
	   
		//for the pt index on this Line
    j = index;
    for(k=0;k<nt;k++) {
      po[k] = (pi[(j+1)*nt+k] + pi[(j-1)*nt+k] - 2.0f*pi[j*nt+k]) / 2.0f;
    }
	}
	//divide dt
	if(bScaleBy_d) {
		float dt = inCube.domain().get_d(iAxis);
		float inv_dt = 1.0 / SQ(dt);
		resultCube.scale(inv_dt);
	 }
}


//calc 1D gradient along axis 2(TWO), this calculation has 2nd order precision, using center scheme
void Yang::spatialXderiv1st(cubeData<float>& resultCube, const cubeData<float>& trcCube, bool bScaleBy_dx)
{
   //same size of result Cube
   assert(resultCube.sameSize(trcCube));
	int ndim = trcCube.domain().get_ndim();
	hypercube shape1 = trcCube.domain().return_compatible(3,ndim);
   int nSlices = shape1.get_n123();
	
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn, *pi;
	
	pIn   = trcCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	
	long long i;
	int j,k,nt,nx,ntx;
	nt = trcCube.domain().get_n(1); nx = trcCube.domain().get_n(2);
	ntx = nt*nx;
	
	//May use openmp here
	//#pragma omp parallel for private(pi,po,j)
	for(i=0;i<nSlices;i++)
	{
	   pi = (pIn+(i)*ntx);
	   po = (pOut+(i)*ntx);
	   //Do the first column, use one-side finite stencil (-3*f0+4*f1-f2)/2.0
		k = 0;
		for (j=0; j<nt; j++) {
			po[j+k*nt] = (-3.0f* pi[j+k*nt] + 4.0* pi[j+(k+1)*nt] - pi[j+(k+2)*nt]) / 2.0;
		}
		
	  for(k=1;k<nx-1;k++)
	  for(j=0;j<nt;j++)
		{
			po[j+k*nt] = (pi[j+(k+1)*nt]-pi[j+(k-1)*nt])/2.0;
		}
	   //Do the last column
		k = nx-1;
		for (j=0; j<nt; j++) {		
	   po[j+k*nt] = (3.0f* pi[j+k*nt] - 4.0f* pi[j+(k-1)*nt] + pi[j+(k-2)*nt]) / 2.0;
		}
	}
	
	if(bScaleBy_dx) {
	   float dx = trcCube.domain().get_d(2);
	   float inv_dx = 1.0/dx;
	   resultCube.scale(inv_dx);
	}
}


//calc 2nd derivative along axis1, this calculation has 2nd order precision, using center scheme
//for 2nd derivative, the scheme of endpoints result in the same values as its neighbours.
void Yang::spatialXderiv2nd(cubeData<float>& resultCube, const cubeData<float>& trcCube, bool bScaleBy_dx)
{
    //same size of result Cube
   assert(resultCube.sameSize(trcCube));
	int ndim = trcCube.domain().get_ndim();
	hypercube shape1 = trcCube.domain().return_compatible(3,ndim);
   int nSlices = shape1.get_n123();
	
	//Get the data pointers
	float *pOut,*po;
	
	const float* pIn, *pi;
	
	pIn   = trcCube.getDataPtr();
	pOut  = resultCube.getDataPtr();
	
	
	long long i;
	int j,k,nt,nx,ntx;
	nt = trcCube.domain().get_n(1); nx = trcCube.domain().get_n(2);
	ntx = nt*nx;
	
	//May use openmp here
	//#pragma omp parallel for private(pi,po,j)
	for(i=0;i<nSlices;i++)
	{
	   pi = (pIn+(i)*ntx);
	   po = (pOut+(i)*ntx);
		
	   for(k=1;k<nx-1;k++)
	   for(j=0;j<nt;j++)
		{
			po[j+k*nt] = (pi[j+(k+1)*nt] - 2.0f* pi[j+k*nt] + pi[j+(k-1)*nt])/2.0;
		}
	   //Do the first and last column
		k = nx-1;
		for (j=0; j<nt; j++) {		
			po[j+0*nt] = po[j+(0+1)*nt];
			po[j+k*nt] = po[j+(k-1)*nt];
		}
	}
	
	if(bScaleBy_dx) {
	   float dx = trcCube.domain().get_d(2);
	   float inv_dx = 1.0/SQ(dx);
	   resultCube.scale(inv_dx);
	}
}

hypercube Yang::shapeAfterStackAlongAxis(int iAxis, const hypercube& inShape) {
   FUNC_TRACE_BEG(shapeAfterStackAlongAxis)
   int ndim = inShape.get_ndim();

   std::vector<axis> axes1,axes2,axes;
   if (iAxis >=1 )
   axes1 = inShape.return_axes(1,iAxis-1);
   if (ndim > iAxis) {
      axes2 = inShape.return_axes(iAxis+1,ndim);
   }
   //Join axes1 and axes2
   axes.insert(axes.end(),axes1.begin(),axes1.end());
   axes.insert(axes.end(),axes2.begin(),axes2.end());
   hypercube outShape(axes);
   FUNC_TRACE_END(outShape)
}


//Multiply an array of length Aixs ix, and Stack Along Axis ix, pMultiplyCube is NULL if no multiplication is needed.
void Yang::multAndStackAlongAxis2(cubeData<float>& outCube, const cubeData<float>& inCube, cubeData<float>* pMultiplyCube )
{
   const hypercube& inShape = inCube.domain();
   int ndim = inShape.get_ndim();
   int iAxis=2;
   hypercube outShape = shapeAfterStackAlongAxis(iAxis, inShape);
   VERIFY(outCube.domain().sameSize(outShape));
   
   axis ax2 = inShape.get_axis(iAxis);
   if (pMultiplyCube) {
      assert(pMultiplyCube->domain().sameSize(hypercube(vecAxis(1,ax2))));
   }
   
   long long i,j,ix;
   //Get the data pointers
   float *pOut,*po;
   const float* pIn, *pi;
   
   pIn   = inCube.getDataPtr();
   pOut  = outCube.getDataPtr();
   po = pOut;
   
   int nt = inShape.get_n(1);
   int nx = inShape.get_n(iAxis);
   int ntx = nt*nx;
   int nplanes = 1;
   if (ndim > iAxis) {
      nplanes = inShape.get_n23(iAxis+1,ndim);
   }
   
   outCube.zero();
   
   if (NULL == pMultiplyCube) {
      for(i=0;i<nplanes;i++)
      {
        pi = (pIn+i*ntx);
        po = (pOut+i*nt);
        for (ix=0; ix<nx; ix++) {
          for(j=0;j<nt;j++)
          {
             po[j] += pi[j+ix*nt];
          }
        }
      }
   } else {
      const float* pM = pMultiplyCube->getDataPtr();
      for(i=0;i<nplanes;i++)
      {
        pi = (pIn+i*ntx);
        po = (pOut+i*nt);
        for (ix=0; ix<nx; ix++) {
          for(j=0;j<nt;j++)
          {
             po[j] += pi[j+ix*nt]*pM[ix];
          }
        }
      }
   }
}


//Stack trc to one trc
void Yang::stackTrc(cubeData<float>& resultTrc, const cubeData<float>& trcCube)
{
   int ntrc = trcCube.domain().get_n23();
   long long i;
   int j,nt;
   
   //Alloc same size of result Cube
   VERIFY(resultTrc.domain().get_ndim()==1);
   VERIFY(trcCube.compatible(resultTrc));
   
   //Get the data pointers
	float *pOut,*po;
	const float* pIn, *pi;
	
	pIn   = trcCube.getDataPtr();
	pOut  = resultTrc.getDataPtr();
	po = pOut;
	
	nt = trcCube.domain().get_n(1);
	
	resultTrc.zero();
	//May use openmp here
	for(i=0;i<ntrc;i++)
	{
   	   pi = (pIn+i*nt);
	   for(j=0;j<nt;j++)
	   {
		  po[j] += pi[j];
	   }
	}
}

void Yang::spreadTrc(cubeData<float>& resultCube, const cubeData<float>& inputTrc)
{
   int ntrc = resultCube.domain().get_n23();
   long long i;
   int j,nt;
   
   //Alloc same size of result Cube
   VERIFY(inputTrc.domain().get_ndim()==1);
   VERIFY(resultCube.compatible(inputTrc));
   resultCube.zero();
   //Get the data pointers
	float *pOut,*po;
	const float* pIn, *pi;
	
	pIn   = inputTrc.getDataPtr();
	pOut  = resultCube.getDataPtr();

	pi = pIn;
	
	nt = inputTrc.domain().get_n(1);
	
	//May use openmp here
	for(i=0;i<ntrc;i++)
	{
	   po = (pOut+i*nt);
	   for(j=0;j<nt;j++)
	   {
		  po[j] += pi[j];
	   }
	}
}

//Sum along time axis for each Trc
void Yang::sumAlongTimeTrc(cubeData<float>& resultCube, const cubeData<float>& trcCube)
{
   int ntrc = resultCube.domain().get_n123();
   long long i;
   int j,nt;
   
   //Alloc same size of result Cube
   VERIFY(trcCube.domain().get_n23() == ntrc);

   resultCube.zero();
   //Get the data pointers
	float *pOut,*po;
	const float* pIn, *pi;
	
	pIn   = trcCube.getDataPtr();
	pOut  = resultCube.getDataPtr();

	pi = pIn;
	po = pOut;
	
	nt = trcCube.domain().get_n(1);
	
	//May use openmp here
	for(i=0;i<ntrc;i++)
	{
	   pi = pIn+i*nt;
	   for(j=0;j<nt;j++)
	   {
		  po[i] += pi[j];
	   }
	}
}

/// Scale the velocity gradient by 1/(V(x)^3)*1e9
void Yang::scaleVelGradient(cubeData<float>& inOut, const cubeData<float>& velCube)
{
   long long i;   
   //Alloc same size of result Cube
   VERIFY(inOut.sameSize(velCube));

   
   //Get the data pointers
	float *pOut,*po;
	const float* pIn1,*pIn2,*pi1,*pi2;
	
	pIn1   = inOut.getDataPtr();
	pOut   = inOut.getDataPtr();
	pIn2   = velCube.getDataPtr();
	
	//Element by Elem operation
	//May use openmp here
	float scalar;
	pi1 = pIn1; pi2 = pIn2;
	po = pOut;
	for(i=0;i<inOut.domain().get_n123();i++)
	{
	   scalar = 1000.0f / (*pi2);
	   scalar = scalar*scalar*scalar;
	   *po = scalar * (*pi1);
		  
	   pi1 ++; pi2++; po++;
	}
}

//slowness gradient, should multiply by s, i.e divide velcube
void Yang::scaleSlowGradient(cubeData<float>& inOut, const cubeData<float>& velCube)
{
  //Alloc same size of result Cube
  VERIFY(inOut.sameSize(velCube));
  inOut.divide(velCube);
}

void Yang::findMaxValTimePerTrc(cubeData<float>& out, const cubeData<float>& C_cube) {
   int ntrc = out.domain().get_n123();
   long long i;
   int j,nt;
   
   //Alloc same size of result Cube
   VERIFY(C_cube.domain().get_n23() == ntrc);

   axis axisT = C_cube.domain().get_axis(1);
   float o = axisT.o;
   float dt = axisT.d;
   
   //Get the data pointers
   float *pOut,*po;
   const float* pIn, *pi;
	
	pIn   = C_cube.getDataPtr();
	pOut  = out.getDataPtr();

	pi = pIn;
	po = pOut;
	
	nt = C_cube.domain().get_n(1);
	
	float max; int maxPos = 0;
	//May use openmp here
	for(i=0;i<ntrc;i++)
	{
	   pi = pIn+i*nt;
	   maxPos = 0; max = pi[0];
	   for(j=1;j<nt;j++)
	   {
		  if (pi[j] > max) {
			 maxPos = j;
			 max = pi[j];
		  }
	   }
	   *po = (maxPos)*dt + o;
	   po++;
	}
}

//Shift each trace by a certain time lag, integer
void Yang::shiftTrc(cubeData<float>& trcCube, const cubeData<int>& shiftCube)
{	
    int ntrc = trcCube.domain().get_n23();
	assert(ntrc == shiftCube.domain().get_n123());
	
	//Get the data pointers
	float *pData,*p;
	const int* pShift;
	
	pData = trcCube.getDataPtr();
	pShift = shiftCube.getDataPtr();
	
	
	long long i;
	int j;
	int shift,nt;
	nt = trcCube.domain().get_n(1);
	//May use openmp here
	for(i=0;i<ntrc;i++)
	{
	   p = (pData+(i)*nt);
	   shift = *(pShift+i);
	   if (shift>0) //shift downward
	   {
		  for(j=nt-1;j>=shift;j--)
		  {
			 *(p+j) = *(p+j-shift);
		  }
		  for(j=shift-1;j>=0;j--)
		  {
			 *(p+j) = 0.0f;
		  }
	   }
	   else if(shift<0)
	   {
		 for(j=0;j<nt-shift;j++)
		 {
			*(p+j) = *(p+j+shift);
		 }
		 for(j=nt-shift;j<nt;j++)
		 {
			*(p+j) = 0.0f;
		 }
	   }
	   else {} //do nothing
	}
}


//Shift Data per Trc, Shift Data based on a shift time value per trc, this only shifts for integer grid pts, no fractal shift amount
//will be rounded. POUT(t) = PIN(t+tau)
void Yang::staticShiftPerTrc(cubeData<float>& out, const cubeData<float>& in, const cubeData<float>& shiftCube) {
   int ntrc = out.domain().get_n23();
   long long i;
   int j,nt;
   
   //Alloc same size of result Cube
   VERIFY(in.domain().get_n23() == ntrc && shiftCube.domain().get_n123() == ntrc);

   //Get the data pointers
   float *pOut,*po;
   const float* pIn, *pi;
  
	pIn   = in.getDataPtr();
	pOut  = out.getDataPtr();

	nt = in.domain().get_n(1);
	float dt = in.domain().get_d(1);
	
	int iShift;
	out.zero();
	//May use openmp here
	for(i=0;i<ntrc;i++)
	{
	   pi = pIn  +i*nt;
	   po = pOut +i*nt;
	   iShift = lroundf(shiftCube[i] / dt );
	   if (iShift < 0) {
		  for(j=-iShift;j<nt;j++)
		  {
			 po[j] = pi[j+iShift]; 
		  }
	   }
	   else {
		  for(j=iShift;j<nt;j++)
		  {
			 po[j-iShift] = pi[j]; 
		  }
	   }
	}
}

//Shift Data per Trc, Shift Data based on a shift time value per trc, this can shift Arbitrary amount.
//Will do linear interpolation if shift by fractal grid point. 
//POUT(t) = PIN(t+tau)
void Yang::staticShiftPerTrcArb(cubeData<float>& out, const cubeData<float>& in, const cubeData<float>& shiftCube) {
   int ntrc = out.domain().get_n23();
   long long i;
   int j,nt;
   
   
   //Alloc same size of result Cube
   VERIFY(in.domain().get_n23() == ntrc && shiftCube.domain().get_n123() == ntrc);
   out.zero();
   //Get the data pointers
   float *pOut,*po;
   const float* pIn, *pi;
  
   pIn   = in.getDataPtr();
   pOut  = out.getDataPtr();
   nt = in.domain().get_n(1);
   
  //option 2, do linear interploation
	float dt = in.domain().get_d(1);
	
	int iShift;
	
	//Do linear interpolation
	float wt1,wt2, fShift;
	//May use openmp here
	for(i=0;i<ntrc;i++)
	{
	   pi = pIn  +i*nt;
	   po = pOut +i*nt;
	   
	   fShift = (shiftCube[i] / dt) ;
	   iShift = int(fShift);
	   //calculate the weight, wt1 for iShift, wt2 for iShift+(-1)
	   wt2 = fabs(fShift - iShift);
	   wt1 = 1 - wt2;
	   if (fShift < 0.0f) {
		  for(j=-iShift;j<nt;j++)
		  {
			 po[j] += pi[j+iShift]*wt1; 
		  }

		  for(j=-iShift+1;j<nt;j++)
		  {
			 po[j] += pi[j+iShift-1]*wt2; 
		  }
	   }
	   else {
		  //wt1 for iShift, wt2 for iShift+1
		  for(j=iShift;j<nt;j++)
		  {
			 po[j-iShift] += pi[j]*wt1; 
		  }
		  for(j=iShift+1;j<nt;j++)
		  {
			 po[j-(iShift+1)] += pi[j]*wt2; 
		  }
	   }
	}
}

//Shift Data per Trc, Shift Data based on a shift time value per trc, this can shift Arbitrary amount.
//using sinc interpolation, more expensive but better to use
//POUT(t) = PIN(t+tau)
void Yang::staticShiftPerTrcArbSinc(cubeData<float>& out, const cubeData<float>& in, const cubeData<float>& shiftCube) {
   int ntrc = out.domain().get_n23();
   long long i;
   int nt;
   
   //Alloc same size of result Cube
   VERIFY(in.domain().get_n23() == ntrc && shiftCube.domain().get_n123() == ntrc);
   out.zero();
   //Get the data pointers
   float *pOut,*po;
   const float* pIn, *pi;
  
   pIn   = in.getDataPtr();
   pOut  = out.getDataPtr();
   nt = in.domain().get_n(1);
     //Option1, use Interp sep command to do the interpolation, may have better accuracy
   float o1,o1out; float d1 = in.domain().get_d(1);
   o1 = in.domain().get_o(1);
   
   cubeDataF inTrc, outTrc;
   inTrc.alloc(in.domain().return_compatible(1,1));
   outTrc.alloc(inTrc.domain());
   for(i=0;i<ntrc;i++) {
     float fShift = shiftCube[i];
     o1out = o1 + fShift;
     pi = pIn  +i*nt;
     po = pOut +i*nt;
     myMemcpy(inTrc.getDataPtr(),pi,nt);
     outTrc.zero();
     seplib::interp::getInstance()->interpolate(outTrc,inTrc,
                                       &nt,&o1out,&d1,
                                       NULL,NULL,NULL,
                                       NULL,NULL,NULL,seplib::sinc);
     myMemcpy(po,outTrc.getDataPtr(),nt);
   }  
}

//Causal integration
void Yang::causalInt(cubeData<float>& mod, cubeData<float>& dat, bool bAdj, bool bAdd) {
   int ntrc;
   ntrc = mod.domain().get_n23();
   long long i;
   int nt;
   //Alloc same size of result Cube
   assert(dat.domain().get_n23() == ntrc);
   addNull(mod,dat,bAdj,bAdd);
   
   //Get the data pointers
   float *pOut,*po;
   const float* pIn, *pi;
   if (!bAdj) {
     pIn   = mod.getDataPtr();
     pOut  = dat.getDataPtr();
   } else {
     pIn   = dat.getDataPtr();
     pOut  = mod.getDataPtr();
   }
   nt = mod.domain().get_n(1);
   
	//VERIFY(bAdj); dat.dump("dump11"); mod.dump("dump12");
	//PRV(bAdj);PRV(nt);PRV(ntrc);
	//dat.info("dat in causalInt"); mod.info("mod in causalInt");
   int j=0; float fsum = 0.0f;
   for(i=0;i<ntrc;i++) {
     po = pOut+i*nt;
     pi = pIn+i*nt;
     if (!bAdj) { //do forward, cannot openmp here
       fsum = 0.0f;
       for (j=0;j<nt;j++) {
         fsum += pi[j];
         po[j] += fsum;
       }
     } else { //do Adj, anticausal integ
       fsum = 0.0f;
       for (j=nt-1;j>=0;j--) {
         fsum += pi[j];
         po[j] += fsum;
       }
     }
   }
	//dat.info("dat in causalInt"); mod.info("mod in causalInt");
}


//if !bAdj
//Input regular data, and an array of irregular coord.
//output an array that contains the sampled value at these coord, use linear interpolation inside.
//if bAdj, then Input an irregular array and its correspoding coord, output the spreaded values into the regular array

void Yang::irregularSamp1d(cubeData<float>& out, const cubeData<float>& in, const cubeData<float>& coord, bool bAdj, bool bExtend, bool bAdd) {
  assert(in.domain().get_ndim()==1);
  if (!bAdj) {
    assert(out.sameSize(coord));
  } else {
    assert(in.sameSize(coord));
  }
  //reset the output
  if (!bAdd) {
    out.zero();
  }
  
  //Get the data pointers
  const float *pIn;
  float *pOut;
  pIn   = in.getDataPtr();
  pOut  = out.getDataPtr();

  //do linear interploation
  float o,d; int n;
  if (!bAdj) {
    d = in.domain().get_d(1);
    o = in.domain().get_o(1);
    n = in.domain().get_n(1);
  } else {
    d = out.domain().get_d(1);
    o = out.domain().get_o(1);
    n = out.domain().get_n(1);
  }
  
	//Do linear interpolation
	float wt1,wt2; float fcoord;
  int istart,iend; float fPos;
  assert(d>0.0f);
  
  int i,npts = coord.domain().get_n(1);
  if (!bAdj) { //do sampling with linear interpolation
    for(i=0;i<npts;i++)
    {
      fcoord = coord[i];
      //cut the coord in range.
      if (bExtend) {
        if (fcoord<o) {
          fcoord = o;
        }
        if (fcoord>o+(n-1)*d) {
          fcoord = o+(n-1)*d;
        }
      } else {
        if (fcoord<o || fcoord>o+(n-1)*d) {
          pOut[i] = 0.0f;
          continue;
        }
      }
      fPos = (fcoord-o)/d;
      istart = int(fPos); iend = istart + 1;
      if (iend > n-1) {iend=n-1;} //handle the right end case
      wt2 = fabs(fPos-istart);
      wt1 = 1 - wt2;
      pOut[i] = pIn[istart]*wt1 + pIn[iend]*wt2;
    }
  } else { //Do spreading with linear weights
    for(i=0;i<npts;i++)
    {
      fcoord = coord[i];
      //cut the coord in range.
      if (bExtend) {
        if (fcoord<o) {
          fcoord = o;
        }
        if (fcoord>o+(n-1)*d) {
          fcoord = o+(n-1)*d;
        }
      } else {
        if (fcoord<o || fcoord>o+(n-1)*d) {
          continue;
        }
      }
      fPos = (fcoord-o)/d;
      istart = int(fPos); iend = istart + 1;
      if (iend > n-1) {iend=n-1;} //handle the right end case
      wt2 = fabs(fPos-istart);
      wt1 = 1 - wt2;
      pOut[istart] += pIn[i]*wt1;
      pOut[iend]   += pIn[i]*wt2;
    }
  }
}

//input an ordered points with INCREASING coordinates, and input another set of coordinates (assume INCREASING order), output the linear interpolated results
void Yang::linInterp1d(float* out, const stdVecFloat& outCoord, const float* in, const stdVecFloat& inCoord, bool bExtend) {
  
  int nout = outCoord.size();
  int nin  = inCoord.size();
  
  //Get the data pointers

	//Do linear interpolation
	float wt1,wt2; float fcoord;
  int istart,iend; float fPos;
  int i,j, jstart = 0;
  for(i=0;i<nout;i++)
  {
    fcoord = outCoord[i];
    //find the interval that fcoord lies in the inCoord array
    for (j=jstart;j<nin;j++) {
      if (inCoord[j] > fcoord) {
        break;
      }
    }
    jstart = j;
    if (jstart == 0) {
      if (bExtend) {out[i] = in[0];}
      else {out[i]=0.0f;}
    } else if (jstart==nin) {
      if (bExtend) {out[i] = in[nin-1];}
      else {out[i]=0.0f;}
    } else { //normal case
      //now fcoord lies between j and j-1
      wt2 = (fcoord-inCoord[j-1]) / (inCoord[j]-inCoord[j-1]);
      wt1 = 1 - wt2;
      out[i] = wt1*in[j-1] + wt2*in[j];
    }
  }
  return;
}

//Take the square root of cube
void Yang::sqrtCube(cubeData<float>& out, const cubeData<float>& input) {
   const hypercube& shape = input.domain();
   assert(input.sameSize(out));
   assert(input.minvalAbs()>0.0f);
   
   const float* pIn = input.getDataPtr();
   float* pOut = out.getDataPtr();
   for (long long i=0; i<shape.get_n123(); i++) {
     *pOut = sqrtf(*pIn);
     pIn++; pOut++;
   }
}

void Yang::buildThresholdMask(cubeData<float>& Mask, const cubeData<float>& input, float vThres , bool bUseAbsVal) {
   assert(Mask.sameSize(input));
   const hypercube& shape = input.domain();

   const float* pIn = input.getDataPtr();
   float* pOut = Mask.getDataPtr();
   if (bUseAbsVal) {
      for (long long i=0; i<shape.get_n123(); i++) {
         if (fabs(*pIn)>vThres) {
            *pOut = 1.0f;
         } else {
            *pOut = 0.0f;
         }
         pIn++; pOut++;
      }
   } else {
      for (long long i=0; i<shape.get_n123(); i++) {
         if (*pIn>vThres) {
            *pOut = 1.0f;
         } else {
            *pOut = 0.0f;
         }
         pIn++; pOut++;
      }
   }
}

//Could be divide with Mask or add a damping epsilon, based on the percentile 0~1.0
//currently just use scaling not the actual quantile
//if (bMaskInputToo), then the result will be masked as well before doing the division.
//NOTE The divident HAS TO BE ALL POSITIVE or ALL NEGATIVE !
void Yang::divideWithCaveat(cubeData<float>& out, const cubeData<float>& in, cubeData<float>& divident,
                      float qt_mask, float qt_eps, bool bUseMask, cubeData<float>* pMask, bool bMaskResultToo) {
  assert(out.sameSize(divident));
  float fval, fmaxAbsVal = divident.maxvalAbs();
  bool bDividentAllpos = divident.sum()>0.0f;
  divident.info("divident divideWithCaveat");
  if (bDividentAllpos) {
    //assert(divident.minval() >= -epsilon);
  } else {
    //assert(divident.maxval() <= epsilon);
  }
  
  float fRMSnz = divident.RMS_NZ(1e-6);
  if (bUseMask) { //use mask
    assert(pMask);
    pMask->alloc(out.domain());
    fval = fmaxAbsVal * qt_mask;
    buildThresholdMask(*pMask,divident,fval,true);
    out.divideByMask(in,divident,*pMask);
	 if (bMaskResultToo) {
		out.multiply(*pMask); 
	 }
  }else { //use damping with epsilon
    fval = fRMSnz * qt_eps;
    if (!bDividentAllpos) {
      fval = -fval;
    }
    divident.add(fval);
    out.divide(in,divident);
  }
}

//Clip the values in a cube that is out of range)
//return the clippedRate (0~2.0)
float Yang::clipCubeVal(cubeData<float>& out, float fmax, float fmin,
                       bool bClipMax, bool bClipMin) {
  float clipRate = 0.0f;
  if (!bClipMax && !bClipMin) {
    return clipRate;
  }
  
  float* pOut;
  pOut = out.getDataPtr();
  long long i, n=out.domain().get_n123();
  long long clipCnt = 0;
  if (bClipMax) {
    for (i=0; i<n; i++) {
      if (*(pOut+i)>fmax) {
        *(pOut+i) = fmax;
        clipCnt++;
      }
    }
  }
  if (bClipMin) {
    for (i=0; i<n; i++) {
      if(*(pOut+i)<fmin) {
        *(pOut+i) = fmin;
        clipCnt++;
      }
    }
  }
  clipRate = float(clipCnt)/n;
  return clipRate;
}


//A stronger version of transp, but use dummy algorithm and works only incore
//First group axes with reshape vec, then axMapping tells how the 1,2,3,4th axis(after reshape)
//map to the desitnation cube, all the axis indexes are 1 based.
void Yang::shuffleAxis(cubeData<float>& out, const cubeData<float>& in, 
							  const stdVecInt& reshape, const stdVecInt& axMapping, bool bStuffDummyAxis, bool bOutMemAllocInside) {
  FUNC_TRACE_BEG(Yang::shuffleAxis);
  int nAxes = reshape.size();
  hypercube inShape = in.domain();
  assert(nAxes = int(axMapping.size()));
  stdVecInt axtt(axMapping);
  assert((unique(axtt.begin(),axtt.end())) == axtt.end());
  
  
  int ndim = reshape[nAxes-1]; int ndimIn = inShape.get_ndim();
  //PRV(ndim);PRV(ndimIn);
  
  if (!bStuffDummyAxis) {
	 assert(ndim == ndimIn);
  } else {
	 assert(ndim >=ndimIn);
	 //Stuff dummy axisT
	 vecAxis inAxes = inShape.get_axes();
	 for (int idim=ndimIn+1; idim<=ndim; idim++) {
		inAxes.push_back(axis(1,0.0f,23.75f));
	 }
	 inShape.set_axes(inAxes);
  }
  stdVecInt ns(nAxes) ; //The number of points on those super axis.
  
  //record the shuffle result
  std::vector<std::pair<int,int> > dimSegVec(nAxes);
  
  int iax,iaxMap, idimBeg=1,idimEnd, idim;
  for (iax=1;iax<=nAxes;iax++) {
	 ns[iax-1] = 1;
	 idimEnd = reshape[iax-1];
	 assert(idimEnd >= idimBeg);
	 for (idim=idimBeg; idim<=idimEnd; idim++) {
		ns[iax-1] *= inShape.get_n(idim);
	 }
	 //Need to put the axes to the correct location at outAxes accroding to the axMapping
	 iaxMap = axMapping[iax-1];
	 assert(iaxMap>0 && iaxMap<= nAxes);
	 dimSegVec[iaxMap-1] = pair<int,int>(idimBeg,idimEnd);
	 
	 //update idimBeg
	 idimBeg = idimEnd + 1;
  }
  //PRVEC(ns,nAxes); 
  
  //Figure out the out shape
  hypercube outShape;
  vecAxis outAxes;
  vector<long long> n123Out(nAxes+1); //The accumulated n123 for the output shape
  n123Out[0] = 1;
  n123Out[1] = 1;
  for (iax=1; iax<=nAxes; iax++) {
	 
	 idimBeg = dimSegVec[iax-1].first;
	 idimEnd = dimSegVec[iax-1].second;
	 for (idim=idimBeg; idim<=idimEnd; idim++) {
		outAxes.push_back(inShape.get_axis(idim));
	 }
	 n123Out[iax] *= inShape.get_n23(idimBeg,idimEnd);
	 if (iax!=nAxes) {
		n123Out[iax+1] = n123Out[iax];
	 }
  }
  //PRVEC(n123Out,nAxes+1); PRV(nAxes);
  
  assert(int(outAxes.size())==ndim);
  outShape.set_axes(outAxes);
  
  //outShape.info("outShape");
  
  if (bOutMemAllocInside) {
	 out.alloc(outShape);
  } else {
	 VERIFY(out.domain().sameSize(outShape));
  }
  
  //Transp the memory
  const float* pIn = in.getDataPtr();
  float* pOut = out.getDataPtr();

  vector<int> axMapT = axMapping;
#define MAX_ALLOW_AXES_NUM 7
  //tranverse this input nAxes dimension array, and put the input into correct output location.
  VERIFY(nAxes<=MAX_ALLOW_AXES_NUM); //Only handles dimension smaller than 7
  for (int i=nAxes; i<MAX_ALLOW_AXES_NUM; i++) {
	 ns.push_back(1);
	 axMapT.push_back(i+1);
  }
  for (int i=nAxes+1; i<MAX_ALLOW_AXES_NUM+1; i++) {
	 n123Out.push_back(n123Out[nAxes]);
  }
  
  //PRVEC(axMapT,MAX_ALLOW_AXES_NUM+1);
  
  long long off7,off6,off5,off4,off3,off2,off1, o8,o7,o6,o5,o3,o4,o2,o1;
  int nt;
  off7 = 0; o8 = 0;
  for (int i7=0; i7<ns[6]; i7++) {
	 o7 = o8 + i7*n123Out[axMapT[6]-1];
	 off6 = (off7+i7)*ns[5];
	 for (int i6=0; i6<ns[5]; i6++) {
		o6 = o7 + i6*n123Out[axMapT[5]-1];
		off5 = (off6+i6)*ns[4];
		for (int i5=0; i5<ns[4]; i5++) {
		  o5 = o6 + i5*n123Out[axMapT[4]-1];
		  off4 = (off5+i5)*ns[3];
		  for (int i4=0; i4<ns[3]; i4++){
			 o4 = o5 + i4*n123Out[axMapT[3]-1];
			 off3 = (off4+i4)*ns[2];
			 for (int i3=0; i3<ns[2]; i3++){
				o3 = o4 + i3*n123Out[axMapT[2]-1];
				off2 = (off3+i3)*ns[1];
				for (int i2=0; i2<ns[1]; i2++){
				  o2 = o3 + i2*n123Out[axMapT[1]-1];
				  off1 = (off2+i2)*ns[0];
				  for (int i1=0; i1<ns[0]; i1++){
					 nt = n123Out[axMapT[0]-1];
					 o1 = o2 + i1*nt;
					 pOut[o1] = pIn[off1+i1];
				  }
				}
			 }
		  }
		}
	 }
  }
  FUNC_TRACE_END();
}

//duplicate the input data along the given axis ( with respect to out). if axis=2 then it is like spread trace
void Yang::duplicateAlongAxis(cubeData<float>& out, const cubeData<float>& in, int iAxis) {
  FUNC_TRACE_BEG(Yang::duplicateAlongAxis);
  const hypercube& outShape = out.domain();
  const hypercube& inShape  = in.domain();
  
  int ndim =  outShape.get_ndim();
	assert(iAxis>0 && iAxis<= ndim);
  //check the size.outShape stripped out iAxis, should agree with inShape.
  vecAxis outAxes = outShape.get_axes();
  outAxes.erase(outAxes.begin()+iAxis-1);
  VERIFY(inShape.sameSize(hypercube(outAxes)));
  
	//divide the shape into 3 hyper axis, nPlanes, nLines, nt;
	int ndimSlice  = iAxis - 1;
  int ndimPlanes = ndim - iAxis;
	
  int nPlanes,nLines,nt;
  nLines = outShape.get_n(iAxis);
  nPlanes = 1; nt = 1;
  if (ndimSlice!=0) {
    nt      = outShape.get_n23(1,ndimSlice);
  }
  if (ndimPlanes != 0) {
    nPlanes = outShape.get_n23(iAxis+1,ndim);
  }
	//Get the data pointers
	float *pData,*p;
	const float* pInData; const float* pIn;
	
	pData   = out.getDataPtr();	
	pInData = in.getDataPtr();
	
	long long i;
	int j,k;
	//May use openmp here out(it,iLine,iPlane) and in(it,iPlane)
	for(i=0;i<nPlanes;i++)
	{
	   p = (pData+i*nt*nLines);
	   pIn = (pInData+i*nt);
	   for(j=0;j<nLines;j++)
	   {
			 for (k=0; k<nt; k++) {
				p[j*nt+k] = pIn[k];
			 }
	   }
	}
  FUNC_TRACE_END() 
}

void Yang::getGaussfunc(cubeDataF& gaussfunc,const axis& ax,float fHalfmaxWidth, float b, int order) {
  FUNC_TRACE_BEG(Yang:getGaussfunc);
  VERIFY(order>=0 && order <=2);
  //Set gaussian function at the zero location
  //Formulaa f(x) = a*exp(-(x-b)^2/(2*c*c));
  //f'(x)     = a*(-(x-b)/c^2)exp(...)
  //f''(x)    = a*((x-b)^2-c^2)/c^4 exp(..)
  float a = 1.0f;
  float c2 = SQ(fHalfmaxWidth/2.35482f);
  gaussfunc.alloc(ax);
  float xb;
  for (int i=0; i<ax.n; i++) {
    xb = ax.get_coord(i)-b;
    if (order==0) {
      gaussfunc(i) = a*expf(-0.5f*SQ(xb)/c2);
    } else if (order==1) {
      gaussfunc(i) = a*expf(-0.5f*SQ(xb)/c2)*(-xb/c2);
    } else {
      gaussfunc(i) = a*expf(-0.5f*SQ(xb)/c2)*(SQ(xb)-c2)/SQ(c2);
    }
  }
  FUNC_TRACE_END();
}

void Yang::CtoR(const cubeData<cmplxf>& cubC,    cubeData<float>& cubR,    cubeData<float>& cubI) {
  long long nSize = cubC.domain().get_n123();
  for (long long i = 0; i<nSize; i++) {
    cubR[i] = cubC[i].real();
    cubI[i] = cubC[i].imag();
  }
}

void Yang::RtoC(cubeData<cmplxf>& cubC, const cubeData<float>& cubR, const cubeData<float>& cubI) {
  long long nSize = cubC.domain().get_n123();
  for (long long i = 0; i<nSize; i++) {
    cubC[i] = cmplxf(cubR[i],cubI[i]);
  }
}

void Yang::cubeAbs(const cubeData<float>& cubi, cubeData<float>& cubo) {
	long long i;
	assert(cubi.domain().get_n123() == cubo.domain().get_n123());
	for (i=0; i<cubi.domain().get_n123();i++) {
		cubo[i] = fabs(cubi[i]);
	}
}

