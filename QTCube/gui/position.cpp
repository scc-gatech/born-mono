#include<position.h>
#include<math.h>

using namespace SEP;
position::position(std::shared_ptr<hypercube>dims, int *c, int *b, int *e,int m_ax)
{
 block[0]=1;
  for(int i=0; i < 8; i++){
    axes[i]=dims->getAxis(i+1);
    loc[i]=c[i];
    beg[i]=b[i];
    end[i]=e[i];
    movie_ax=m_ax;
    dir=1;
    if(i>0) {block[i]=block[i-1]*(long long)axes[i-1].n;
    }
   }
 } 

 void position::full_range()
 {
    for(int i=0; i< 8; i++){
       beg[i]=0; end[i]=axes[i].n; 
       if(i<5) loc[i]=axes[i].n/2;
    }
 }

void position::fix_min(int imin){
  for(int i=0; i < 8;i++){
    if(axes[i].n >1){
      while(end[i] -beg[i] <imin){
        if(end[i]!=axes[i].n) end[i]++;
         else beg[i]--;
      }
    }
    if(loc[i] < beg[i]) loc[i]=beg[i];
    if(loc[i] >=end[i]) loc[i]=end[i]-1;
   }
}
 
void position::update_position(int iax1, int iax2, int iloc1, int iloc2){
    this->loc[iax1]=iloc1;
    this->loc[iax2]=iloc2;
    
 }
void position::update_range(int iax1, int iax2, int beg1, int end1, int beg2, int end2){
   this->beg[iax1]=beg1;
   this->beg[iax2]=beg2;
   this->end[iax1]=end1;
   this->end[iax2]=end2;
 }
void position::delete_position(){


}
void position::increment(int iax, int inc,std::shared_ptr<hypercube> h){
  axis a=h->getAxis(iax+1);
  float p=axes[iax].o+axes[iax].d*loc[iax]+inc*a.d;
   
  this->loc[iax]=(int)((p-axes[iax].o)/axes[iax].d+.5);

  if(this->loc[iax] < this->beg[iax]) this->loc[iax]=this->end[iax]-1;
  if(this->loc[iax] >=this->end[iax]) this->loc[iax]=this->beg[iax];

}
QString position::return_pos_label(){
  QString ret;
  ret="";
  for(int i=0; i < 8; i++){
    axis a=getAxis(i);
    if(a.n>1) ret+="Axis "+QString::number(i+1)+"("+QString::number(loc[i])+"):"+QString::number(a.o+a.d*loc[i])+"  ";

  }
  return ret;


}
std::shared_ptr<position> position::clone(){
 std::shared_ptr<position> x(new position());
 for(int i=0; i < 8; i++){  
   x->axes[i]=axes[i];
   x->loc[i]=loc[i];
   x->beg[i]=beg[i];
   x->end[i]=end[i];
   x->block[i]=block[i];
 }
 x->movie_ax=movie_ax;
 x->dir=dir;
 return x;
}
