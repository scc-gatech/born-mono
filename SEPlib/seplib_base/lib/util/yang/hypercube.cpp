#include "stdafx.h"
#include "hypercube.h"

extern "C" {
#include "seplib.h"
}

using namespace Yang;

//copy constructor
hypercube::hypercube(const hypercube& hpcube)
{
   this->init_nd(hpcube.get_axes());
}

// assgin operator redefinition
hypercube& hypercube::operator= (const hypercube & hpcube)
{
   this->init_nd(hpcube.get_axes());
   return *this;
}

void hypercube::init_nd(const std::vector<axis> &ax)
{
   //first deallocate, cleaned up the original info
   this->deallocate();
   this->n123=1;
   for(uint i=0; i < ax.size(); i++){   
      axes.push_back(ax[i]);
      this->n123=this->n123*(long long)ax[i].n;
  }
}

void hypercube::set_axis(int idim, axis myaxis){
  if(idim<1 || idim > int(axes.size())) {
    fprintf(stderr,"IDIM=%d axes.size()=%zd \n",idim,axes.size());
    errPrt("SET_AXIS ERROR\n");
  }
  this->axes[idim-1]=myaxis;
}

void hypercube::set_axes(const std::vector<axis>& axes){
  this->init_nd(axes);
}

//return the first nmax axis,if original dimension smaller than nmax, then pad the higher dimension with 1.
std::vector<axis> hypercube::return_axes(int nmax) const{
  std::vector<axis> ax;
  for(int i=0; i < nmax; i++){
    if(i+1 <= int(this->axes.size())){
       ax.push_back(axes[i]);
    }
    else {
      ax.push_back(axis(1));
    }
  }
  return ax;
}

// RETURN AXES for iSTART to iEND
std::vector<axis> hypercube::return_axes(int iAxisBeg, int iAxisEnd) const {
  std::vector<axis> ax;
  assert(iAxisEnd <= this->get_ndim());
  for(int i=iAxisBeg-1; i < iAxisEnd; i++){
       ax.push_back((this->axes)[i]);
  }
  return ax;
}


axis hypercube::get_axis(int idim) const{

  if(idim<1 || idim > int(this->axes.size())) {
    fprintf(stderr,"idim=%d axes.size()=%zd \n",idim,this->axes.size());
    prt("GET_AXIS ERROR\n");
    VERIFY(0);
  }
  axis myaxis=this->axes[idim-1];
  return myaxis;
}

//return the dimension excluding the highest n=1 dummy axes 
int hypercube::get_ndim_g1() const{
   int nd=this->axes.size();
   for(int i=this->axes.size()-1; i>=0; i--){
	  if(axes[i].n>1) return nd;
	   nd--;
   }
   PRT_LOC;
   prt("!! ndim_g1==1\n");
   return 0;
}


std::vector<int> hypercube::get_ns() const{
  uint i;
  std::vector<int> n;
  for(i=0; i < this->axes.size(); i++) n.push_back(axes[i].n);
  return n;
}

//Get d value for each axis
std::vector<float> hypercube::get_ds() const 
{
   uint i;
   std::vector<float> ds;
   for(i=0; i < this->axes.size(); i++)
      ds.push_back(axes[i].d);
   return ds;
}

// shape other is a subset of the first n axes of shape this
bool hypercube::compatible(const hypercube& other) const {
  int ndim = other.get_ndim();
   if(this->get_ndim() < ndim ) return false;
  for (int i=0; i < ndim; i++)
     {
    axis ax1=this->get_axis(i+1);
    axis ax2=other.get_axis(i+1);
    if(ax1.n != ax2.n) return false; 
   }
  return true;
}

hypercube hypercube::return_compatible(int iAxisStart, int iAxisEnd) const {
   VERIFY(iAxisStart >= 1);
   VERIFY(iAxisEnd <= this->get_ndim());
   
   vector<axis> axes;
   for (int i=iAxisStart; i <= iAxisEnd; i++)
   {
	  axes.push_back(this->get_axis(i));
   }
  return hypercube(axes);
}

bool hypercube::sameSize(const hypercube& other) const 

{
  if(this->get_ndim() != other.get_ndim()) return false;
  for (int i=0; i < this->get_ndim(); i++)
     {
    axis ax1=this->get_axis(i+1);
    axis ax2=other.get_axis(i+1);
    if(ax1.n != ax2.n) return false; 
   }
  return true;
}


//not only the size, but also the grid d,o equals.
bool hypercube::sameShape(const hypercube& other) const 
{
  if(this->get_ndim() != other.get_ndim()) return false;
  for (int i=0; i < this->get_ndim(); i++)
  {
    axis ax1=this->get_axis(i+1);
    axis ax2=other.get_axis(i+1);
    if (!ax1.same_axis(ax2)) {
       return false;
    }
  }
  return true;
}

void hypercube::reshape(const stdVecInt& shapeInt) {
  VERIFY( shapeInt.back()   == this->get_ndim() );
  int na_new = shapeInt.size();
  vecAxis vecaxis(na_new);
  axis ax1; int n;
  //do the first index in shapeInt
  VERIFY(shapeInt[0]>=1);
  n = 1;
  for (int j=0; j<shapeInt[0]; j++) {
    n = n*this->get_n(j+1);
  }
  ax1 = this->get_axis(0+1);
  ax1.n = n;
  vecaxis[0] = ax1;
  
  //do the rest axes
  for (int i=1; i < na_new; i++)
  {
    VERIFY(shapeInt[i-1]<shapeInt[i]);
    n = 1;
    for (int j=shapeInt[i-1]; j<shapeInt[i]; j++) {
      n = n*this->get_n(j+1);
    }
    ax1 = this->get_axis(shapeInt[i-1]+1);
    ax1.n = n;
    vecaxis[i] = ax1;
  }
  
  //set the axes to be different
  this->set_axes(vecaxis);
  return;
}

hypercube hypercube::return_arbAxesShape(const vector<int>& iAxes) const {
  FUNC_TRACE_BEG(hypercube return_arbAxesShape);
  vecAxis axes;
  size_t nAxis = iAxes.size();
  assert(nAxis>0);
  assert(*min_element(iAxes.begin(),iAxes.end()) > 0);
  assert(*max_element(iAxes.begin(),iAxes.end()) <= this->get_ndim());
  for (size_t i=0; i<nAxis; i++) {
    axes.push_back(this->get_axis(iAxes[i]));
  }
  FUNC_TRACE_END(hypercube(axes));
}
