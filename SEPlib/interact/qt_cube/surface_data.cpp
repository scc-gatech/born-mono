#include "surface_data.h"

surface_data::surface_data(hypercube *g, QString nm, dataset *dt,param_func *p,
  pick_draw *picks, QString col,int in){


  dat=dt;
  grid=g;
  pick_col=col;
  pickable=false;
  name=nm;
  inum=in;
  pk=picks;
  moveable=true;
  par=new util(p);
  restrict="NONE";
  moveable=true;
  bcolor="none";
  ecolor="none";
  isingle=0;
  display="single";
  show_picks=false;
  values=display;
  build_conv();
  update_pick=true;
  nslices=9;
   histo=new float [256];
   set_contains();
   
     data_contains[isingle]=display_axis[isingle]=false;
   for(int i=0; i < 256; i++) histo[i]=0;
   
}
unsigned char *surface_data::get_char_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){
     float *tb=check_load_slice(pos,iax1,f1,e1,iax2,f2,e2);
           int n1=abs(e1-f1), n2=abs(e2-f2);

     unsigned char *buf=float_to_byte(n1*n2,tb);
     delete [] tb;
     return buf;
      
}
float *surface_data::get_float_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
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
float surface_data::get_value(orient_cube *pos){
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
float  *surface_data::check_load_slice(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){


  for(int i=0; i < (int) slices.size(); i++){
     if(slices[i]->have_slice(display,pick_col,isingle,pos,iax1,iax2)){
        return slices[i]->return_slice(pos,f1,e1,f2,e2);
     }
  }
  if(nslices==(int)slices.size()){
    delete slices[0];
    slices.erase(slices.begin());
  }
  slices.push_back(new surface_slice(pos,iax1,iax2,isingle,display,pick_col,dat,pk));
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

void surface_data::set_display(orient_cube *pos,QString d){
   display=d; 
   form_histo(pos);
}
void surface_data::form_histo(orient_cube *lc){
   std::map<long long,pick_new*> all=pk->return_all(pick_col);
   orient_cube pos=orient_cube(lc);

   std::map<long long, pick_new*>::const_iterator i;
   float *vals=new float[all.size()];
   float o[8],d[8];
   lc->get_os(o);
   lc->get_ds(d);
   int j=0;

   if(display.contains("amp")){
     for ( i=all.begin(); i !=all.end();  ++i){
       pos.set_locs(i->second->iloc);
       vals[j]=dat->get_value(&pos);

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

   for(int i=0; i < (int)slices.size(); i++) delete slices[i];
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
void surface_data::set_dataset(orient_cube *pos,QString dt, dataset *da){
  dat=da;
  data_name=dt;
  form_histo(pos);
}
void surface_data::set_color(orient_cube *pos, QString col){
  pick_col=col;
  form_histo(pos);
}
void surface_data::set_single(orient_cube *pos,int is){
  isingle=is;
  for(int i=0; i < 8; i++) data_contains[i]=display_axis[i]=true;
  data_contains[isingle]=display_axis[isingle]=false;
  values=display;
  form_histo(pos);
}

surface_slice::surface_slice(orient_cube *pos, int i1, int i2,int ising,
QString dis, QString col, dataset *dat, pick_draw *pk){
   
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

   picks_vec *all=pk->return_iloc_based(iloc,color);
   float o[8],d[8];
   pos->get_os(o);
   pos->get_ds(d);
   n1=n[iax1];
   n2=n[iax2];
   if(display.contains("amp")) form_amp_slice(pos,all,dat);
   else form_depth_slice(pos,all,o[isingle],d[isingle]);
   
   
   delete all;


}
void surface_slice::form_amp_slice(orient_cube *ori,picks_vec *all, dataset *dat){
  orient_cube pos=orient_cube(ori);
  slice=new float[n1*n2];
  for(int i=0; i < n1*n2; i++) slice[i]=FUNKY_VAL;
  int n;
    int ibase=ori->get_loc(isingle);

  std::map<long long, int> *cmap=ori->get_rot_map(iax1,iax2,&n);
  for(int i=0; i <(int) all->return_size(); i++){
    pick_new *p=all->return_pick(i);
  long long ps=p->pos+ori->block[isingle]*(ibase-p->iloc[isingle]);
   if(cmap->count(ps)==1){
    pos.set_locs(p->iloc);
    slice[cmap->at(ps)]=dat->get_value(&pos);
  }
}

}
void surface_slice::form_depth_slice(orient_cube *ori,picks_vec *all,float o, float d){
if(o==0 && d==0){}
  slice=new float[n1*n2];
  for(int i=0; i < n1*n2; i++) slice[i]=FUNKY_VAL;
  int n;
  std::map<long long, int> *cmap=ori->get_rot_map(iax1,iax2,&n);
  int ibase=ori->get_loc(isingle);
  int count=0;
  for(int i=0; i <(int) all->return_size(); i++){
    pick_new *p=all->return_pick(i);
    long long ps=p->pos+ori->block[isingle]*(ibase-p->iloc[isingle]);
    if(cmap->count(ps)==1){
      slice[cmap->at(ps)]=p->iloc[isingle]*d+o;
    }
    else{
      count++;

    }
  }
}
bool surface_slice::have_slice(QString dis,QString col, int ising,orient_cube *pos, 
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
float *surface_slice::return_slice(orient_cube *pos,int f1, int e1, int f2, int e2){
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
