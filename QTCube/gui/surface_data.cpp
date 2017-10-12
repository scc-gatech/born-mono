#include "surface_data.h"
using namespace SEP;
surface_data::surface_data(std::shared_ptr<hypercube>g, QString nm, QString typ, std::shared_ptr<dataset>dt,std::shared_ptr<paramObj>p,
  std::shared_ptr<pick_draw>picks, QString col,int in){

  data_type="SURFACE";
  title=nm;
  title_short=nm;
  dat=dt;
  grid=g;
  pick_col=col;
  pickable=false;
  name=nm;
  inum=in;
  pk=picks;
  moveable=true;

  restrict="NONE";
  moveable=true;
  bcolor="none";
  ecolor="none";
  isingle=0;
  display=typ;
  show_picks=false;
  values=std::string(display.toAscii().constData());
  build_conv();
  update_pick=true;
  nslices=9;
   histo=new float [256];
   set_contains();
  for(int i=0; i<5; i++) axes_display.push_back(-1);

     data_contains[isingle]=display_axis[isingle]=false;

   for(int i=0; i < 256; i++) histo[i]=0;
   nbuf=1; 
   for(int i=0; i < 8; i++){
     std::shared_ptr<hypercube>h=return_grid();
     axis a=h->getAxis(i+1);
     if(i!=isingle){ 
        nbuf=nbuf*(long long) a.n;
    }
    else nrem=nbuf;
  }

  buf=new float[nbuf];
my_orient=-1;
}
void surface_data::create_buffer(std::shared_ptr<orient_cube>pos){
 if(pos->get_orient_num()==my_orient) return;
 my_orient=pos->get_orient_num();
  my_pos=pos;
 update_surface();
}
void surface_data::update_surface(){

 std::shared_ptr<picks_vec>mn=pk->return_all_picks(my_pos,pick_col);

 for(long long i=0; i < nbuf; i++) buf[i]=FUNKY_VAL;

 std::shared_ptr<orient_cube> ori(new orient_cube(my_pos));

long long nblock[8];
nblock[0]=1;
std::shared_ptr<hypercube>h=return_grid();

for(int i=1; i < 8; i++) {
   axis a=h->getAxis(i);
   if(i-1 !=isingle) nblock[i]=nblock[i-1]*(long long)a.n;
   else {
     nblock[i]=nblock[i-1];
    }
}
minv=1e32;
maxv=-1e32;
axis a=h->getAxis(isingle+1);
 if(display.contains("single")){
   for(int i=0; i< mn->return_size(); i++){
    std::shared_ptr<pick_new>p=mn->picks[i];
    long long o=0;
    for(int iax=0;iax <8; iax++)  {
      if(iax!=isingle) o+=(long long) p->iloc[iax]*nblock[iax];
    }

     buf[o] = p->iloc[isingle]*a.d+a.o;
     minv=std::min(buf[o],minv);
     maxv=std::max(buf[o],maxv);
  }
 }
 else{
    for(int i=0; i< mn->return_size(); i++){

       std::shared_ptr<pick_new>p=mn->picks[i];
       ori->set_locs(p->iloc);
           long long o=0;
    for(int iax=0;iax <8; iax++)  {
      if(iax!=isingle) o+=(long long) p->iloc[iax]*nblock[iax];
    }
     buf[o] = dat->get_value(ori);
          minv=std::min(buf[o],minv);
     maxv=std::max(buf[o],maxv);
     }
 }
 
}



unsigned char *surface_data::get_char_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){
     float *tb=check_load_slice(pos,iax1,f1,e1,iax2,f2,e2);
           int n1=abs(e1-f1), n2=abs(e2-f2);

     unsigned char *buf=float_to_byte(n1*n2,tb);

     delete [] tb;
     return buf;
      
}
unsigned char *surface_data::get_char_data(std::shared_ptr<orient_cube>pos,int iax1,int iax2, int n, long long *index){

   float *tb=get_float_data(pos,iax1,iax2,n,index);

   unsigned char *my=float_to_byte(n,tb);
   delete [] tb;
   return my;
}

float *surface_data::get_float_data(std::shared_ptr<orient_cube>pos,int iax1,int iax2, int n, long long *index){
    float *tb=new float[n];
    create_buffer(pos);
   long long nblocko[8],nblock[8],mult[8];
   nblock[0]=nblocko[0]=1;
   if(isingle==0) mult[0]=0;
   else mult[0]=1;
   for(int i=1; i < 8; i++){
     axis a=return_grid()->getAxis(i);
     mult[i]=1;
     if(i-1==isingle){ nblocko[i]=nblocko[i-1];}
     else nblocko[i]=nblocko[i-1]*a.n;
     if(i==isingle) mult[i]=0;

     nblock[i]=nblock[i-1]*a.n;
  }
   for(int i=0; i < n; i++){
     long long out=0;
     long long ps=index[i];
     for(int iax=7; iax>=0 ; iax--){
       int ind=(int)(ps/nblock[iax]);
       out+=(long long) ind*nblocko[iax]*mult[iax];
       ps-=ind*nblock[iax];
    }
 //  if(i==1000) assert(1==2);
       tb[i]=buf[out];

    }
   return tb;
}




float *surface_data::get_float_data(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){
         
     return  check_load_slice(pos,iax1,f1,e1,iax2,f2,e2);
      
}
unsigned char *surface_data::float_to_byte(int n,float *tb){

 unsigned char *buf=new unsigned char[n];
 float d=(maxv-minv)/254;
 for(int i=0; i < n; i++){
   if(tb[i]==FUNKY_VAL) buf[i]=0;
   else buf[i]=(int)std::max(1,std::min(255,(int)(((tb[i]-minv)/d)+1.)));
 }
 return buf;
}
float surface_data::get_value(std::shared_ptr<orient_cube>pos){
  int iloc[8];
  pos->get_locs(iloc);
   for(int i=0; i < (int)slices.size(); i++){
     if(slices[i]->have_slice(display,pick_col,isingle,pos,slices[i]->iax1,slices[i]->iax2)){
        return slices[i]->slice[iloc[slices[i]->iax1]+
        iloc[slices[i]->iax2]*slices[i]->n1];
      }
    }
    
  return 0.;
}

float surface_data::get_minval(){
  return minv;
}
float surface_data::get_maxval(){
  return maxv;
}
float  *surface_data::check_load_slice(std::shared_ptr<orient_cube>pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){


  for(int i=0; i < (int) slices.size(); i++){
     if(slices[i]->have_slice(display,pick_col,isingle,pos,iax1,iax2)){
        return slices[i]->return_slice(pos,f1,e1,f2,e2);
     }
  }
  if(nslices==(int)slices.size()){
    slices.erase(slices.begin());
  }
  std::shared_ptr<surface_slice> x(new surface_slice(pos,iax1,iax2,isingle,display,pick_col,dat,pk));
  slices.push_back(x);
  return slices[slices.size()-1]->return_slice(pos,f1,e1,f2,e2);    
 }

QString surface_data::return_histogram(){
 QString b;

 b=QString::number(histo[0]);
 for(int i=1; i< 256;i++){
   b=b+":"+QString::number(histo[i]);
 }
 return b;
}
void surface_data::set_display(std::shared_ptr<orient_cube>pos,QString d){
   display=d; 
   form_histo(pos);
}
void surface_data::form_histo(std::shared_ptr<orient_cube>lc){
   std::map<long long,std::shared_ptr<pick_new>> all=pk->return_all(pick_col);
   std::shared_ptr<orient_cube> pos(new orient_cube(lc));

   std::map<long long, std::shared_ptr<pick_new>>::const_iterator i;
   float *vals=new float[all.size()];
   float o[8],d[8];
   lc->get_os(o);
   lc->get_ds(d);
   int j=0;

   if(display.contains("amp")){
     for ( i=all.begin(); i !=all.end();  ++i){
       pos->set_locs(i->second->iloc);
       vals[j]=dat->get_value(pos);

                j++;

     }
    }
    else{
     for ( i=all.begin(); i !=all.end();  ++i){
       vals[j]=o[isingle]+d[isingle]*i->second->iloc[isingle];
       j++;
     }
   }
   

   minv=1e31; maxv=-1e31;
   for(int i=0; i< (int) all.size(); i++){
     if(vals[i]!=FUNKY_VAL){
     if(vals[i]< minv) minv=vals[i];
     if(vals[i]>maxv) maxv=vals[i];
     }
   }
   float mys=0,dv=(maxv-minv)/254;
   for(int i=0; i< 256; i++) histo[i]=0;
   for(int i=0; i < (int)all.size(); i++){
     if(vals[i]!=FUNKY_VAL){
     int iloc=(int)((vals[i]-minv)/dv+1);
     histo[iloc]++;
    
    }
   }
   for(int i=0; i < 256; i++){
     if(mys<histo[i]) mys=histo[i];
   }
   for(int i=0; i < 256; i++){
     histo[i]=histo[i]/mys;
   }
   delete [] vals;

   slices.clear();
 std::vector<QString> com2; com2.push_back(QString::number(inum)); com2.push_back("menu");
    com2.push_back("Clip");
 com2.push_back("set_histo");
 QString b=QString::number(histo[0]);
 for(int i=1; i< 256;i++){
   b=b+":"+QString::number(histo[i]);
 }

 com2.push_back(b);

 emit actionDetected(com2);
}
void surface_data::set_dataset(std::shared_ptr<orient_cube>pos,QString dt, std::shared_ptr<dataset>da){
  dat=da;
  data_name=dt;
  form_histo(pos);
}
void surface_data::set_color(std::shared_ptr<orient_cube>pos, QString col){
  pick_col=col;
  form_histo(pos);
}
void surface_data::set_single(std::shared_ptr<orient_cube>pos,int is){
  isingle=is;
  for(int i=0; i < 8; i++) data_contains[i]=display_axis[i]=true;
  data_contains[isingle]=display_axis[isingle]=false;
  values=std::string(display.toAscii().constData());
  form_histo(pos);
}
surface_slice::surface_slice(std::shared_ptr<orient_cube>pos, int i1, int i2,int ising,
QString dis, QString col, std::shared_ptr<dataset>dat, std::shared_ptr<pick_draw>pk){
   
   iax1=i1;
   iax2=i2;
   pos->get_locs(iloc);
   color=col;
   display=dis;
   isingle=ising;
   int n[8]; pos->get_ns(n);
   int iloc[8]; pos->get_locs(iloc);
   iloc[iax1]=-1;
   iloc[iax2]=-1;
   iloc[isingle]=-1;

   std::shared_ptr<picks_vec>all=pk->return_iloc_based(iloc,color);
   float o[8],d[8];
   pos->get_os(o);
   pos->get_ds(d);
   n1=n[iax1];
   n2=n[iax2];
   if(display.contains("amp")) form_amp_slice(pos,all,dat);
   else form_depth_slice(pos,all,o[isingle],d[isingle]);
   
   


}
void surface_slice::form_amp_slice(std::shared_ptr<orient_cube>ori,std::shared_ptr<picks_vec>all, std::shared_ptr<dataset>dat){
  std::shared_ptr<orient_cube> pos(new orient_cube(ori));
  slice=new float[n1*n2];
  for(int i=0; i < n1*n2; i++) slice[i]=FUNKY_VAL;
  int n;
    int ibase=ori->get_loc(isingle);

  std::map<long long, int> *cmap=ori->get_rot_map(iax1,iax2,&n);
  for(int i=0; i <(int) all->return_size(); i++){
    std::shared_ptr<pick_new>p=all->return_pick(i);
  long long ps=p->pos+ori->block[isingle]*(ibase-p->iloc[isingle]);
   if(cmap->count(ps)==1){
    pos->set_locs(p->iloc);
    slice[cmap->at(ps)]=dat->get_value(pos);
  }
}

}
void surface_slice::form_depth_slice(std::shared_ptr<orient_cube>ori,std::shared_ptr<picks_vec>all,float o, float d){
if(o==0 && d==0){;}
  slice=new float[n1*n2];
  for(int i=0; i < n1*n2; i++) slice[i]=FUNKY_VAL;
  int n;
  std::map<long long, int> *cmap=ori->get_rot_map(iax1,iax2,&n);
  int ibase=ori->get_loc(isingle);
  int count=0;
  for(int i=0; i <(int) all->return_size(); i++){
    std::shared_ptr<pick_new>p=all->return_pick(i);
    long long ps=p->pos+ori->block[isingle]*(ibase-p->iloc[isingle]);
    if(cmap->count(ps)==1){
      slice[cmap->at(ps)]=p->iloc[isingle]*d+o;
    }
    else{
      count++;

    }
  }
}
bool surface_slice::have_slice(QString dis,QString col, int ising,std::shared_ptr<orient_cube>pos, 
  int i1, int i2){
  if(dis!=display) return false;

  if(col!=color) return false;

  if(iax1!=i1) return false;

  if(iax2!=i2) return false;

  if(ising!=isingle) return false;
  int locs[8];
  pos->get_locs(locs);
  for(int i=0; i < 8; i++){
    if(iax1!=i && iax2!=i && i!=isingle && iloc[i] != locs[i] ){
       return false;

     }
  }
  return true;
}
float *surface_slice::return_slice(std::shared_ptr<orient_cube>pos,int f1, int e1, int f2, int e2){
  int n1s=abs(e1-f1);
  int n2s=abs(e2-f2);
  int j1=1, j2=1;
  if(f1>e1) j1=-1;
  if(f2>e2) j2=-1;
  float *buf=new  float [n1s*n2s];
  int i=0;
   //ThERE IS A BUG IN THE CURRENT FORM THIS EXPECTS FULL SLICE WHILE WE ARE CREATING
   //What is currently visible.  Choose to either never allow rotation or track
   //rotation map
  if(pos->get_rotate()){
      fprintf(stderr,"Currently can't rotate surface view\n");
      pos->set_no_rotate();
   }
//   long long *index=form_index_map(pos,iax1,f1,e1,iax2,f2,e2);

  
    for(int i2=0; i2 < n2s; i2++){
      for(int i1=0; i1 < n1s; i1++,i++){
        buf[i]=slice[f1+i1*j1+(f2+i2*j2)*n1];

      }
    }
   

  return buf;
}
