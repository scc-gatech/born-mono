#include "stdafx.h"

#include "axis.h"
#include <math.h>

extern "C" {
#include "seplib.h"
}

using std::string;

axis::axis(int n, float o, float d, string label){
  string unit="Undefined";
  this->basic_set(n,o,d,label,unit);
}
axis::axis(int n, float o, float d, string label, string unit){
  this->basic_set(n,o,d,label,unit);
  }
axis::axis(int n, float o, float d){  
  string unit="Undefined",label="Undefined";
  this->basic_set(n,o,d,label,unit);
}
axis::axis(int n)
{
   string unit="Undefined",label="Undefined";
   this->basic_set(n,0.,1,label,unit);
 }

void axis::set(int n, float o, float d, string label){
  string unit="Undefined";
  this->basic_set(n,o,d,label,unit);
}
void axis::set(int n, float o, float d, string label, string unit){
  this->basic_set(n,o,d,label,unit);
  }
void axis::set(int n, float o, float d){  
  string unit="Undefined",label="Undefined";
  this->basic_set(n,o,d,label,unit);
}
void axis::set(int n)
{
   string unit="Undefined",label="Undefined";
   this->basic_set(n,0.,1,label,unit);
}

bool axis::same_axis(const axis& ax) const {
   bool match=true;  
  if(this->d < 1e-7) errPrt("d of axis is to small");
  if(this->n !=ax.n) match=false;
  if(fabs((this->o-ax.o)/this->d) > .01) match=false;
  if(fabs(this->d/ax.d-1.) > .01) match=false;
  return match;
}

  void axis::basic_set(int n, float o, float d, string label, string unit)
  {
  this->n=n;
  this->o=o;
  this->d=d;
  this->label=label;
  this->unit=unit;
  }
 
  
  //return a truncated axis based on the input range beg->end.
  axis axis::return_truncated(float beg, float end) const {
    axis ax = *this;
    int off0 = this->get_offset(beg); VERIFY(off0>=0);
    int offn = this->get_offset(end); VERIFY(offn<= (this->n)-1 );
    ax.o = this->get_coord(off0); ax.n = offn-off0+1;
    return ax;
  }
  
  //given a smaller ax, return it location on my own axis
  void axis::return_alignedoffset(const axis& ax, int& offset_, int &n_) const {
    offset_ = this->get_offset(ax.o); VERIFY(offset_>=0);
    n_ = ax.n;
    return;
  }

  void alignAxis(float& o1beg, int& n1,float& d1, const axis& ax1, const axis& ax2) {
    float d = ax1.d; assert(equalf(d,ax2.d)); d1=d;
    bool bPos = (d > 0.0f);
    float o1,o2; o1=ax1.o; o2=ax2.o;
    if (bPos) {
    o1beg = MAX(o1,o2);
    } else { 
    o1beg = MIN(o1,o2);
    }
    float e1,e2; e1=o1+(ax1.n-1)*d; e2=o2+(ax2.n-1)*d;
    float o1end;
    if (bPos) {
    o1end = MIN(e1,e2);
    } else {
    o1end = MAX(e1,e2);
    }
    n1 = int((o1end - o1beg)/d+0.5f) + 1;
    //PRV4(n1,o1end,o1beg,d);
  }
