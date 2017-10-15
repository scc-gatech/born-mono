#include "dataset.h"
#include "math.h"
#include <stdlib.h>
#include <QFileInfo>
void dataset::set_basics(std::string titl,QString nm,hypercube *g,io_func *i, param_func *p, int in,int im){


  name=QFileInfo(nm).baseName();
  title=titl.c_str();
  if(title.length()<20) title_short=title;
  else{
    title_short=title.remove(0,title.length()-20);
  }
  io=i;
  par=new util(p);
  inum=in;
  nmax_buf=im;
  grid=g;
  pickable=true;
  restrict="NONE";
  moveable=true;
  show_picks=true;
  update_pick=false;

  build_conv();

  for(int i=0; i < 8; i++){
    data_contains[i]=true;
    display_axis[i]=true;
  }
  
  

  set_contains();
  bcolor="none";
  ecolor="none";
  if(!match_grid()){
    fprintf(stderr,"FILE=%s \n",nm.toAscii().constData());
    fprintf(stderr,"description does not match grid ");
  }
  

}

bool dataset::match_grid(){
  hypercube *iod=io->return_hyper();

  bool valid=true;
  for(int i=0; i < 8 ; i++){

    if(data_contains[i]){
       float bg,eg,bd,ed;
       
       axis ga=grid->get_axis(i+1);
       axis da=iod->get_axis(i+1);
       if(ga.n >1){      
         bg=ga.o; eg=ga.o+ga.d*(ga.n-1);
         bd=da.o; ed=da.o+da.d*(da.n-1);
       
         if(((bg-bd)/ga.d) >.01){
            fprintf(stderr,"Axis %d data axis (%f) begins before grid axis (%f) \n",
              i+1,bd,bg);
            valid=false;
         }
         if(((ed-eg)/ga.d) > .01) {
            fprintf(stderr,"Axis %d data axis (%f) ends after grid axis (%f) \n",
              i+1,ed,eg);
            valid=false;
         }
      }
    }
 }
 return valid;
}
void dataset::set_contains(){
 for(int i=0; i < 8; i++) {
    data_contains[i]=true;
    display_axis[i]=true;
  }

}

unsigned char *dataset::get_char_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){


   int ibuf=check_load_buffer(pos,iax1,iax2);

   unsigned char *cbuf=buf[ibuf]->get_char_data(pos,iax1,f1,e1,iax2,f2,e2);

   for(int i=0; i < abs((e1-f1)*(e2-f2)); i++) cbuf[i]=conv[cbuf[i]];
   return cbuf;
   
}
float *dataset::get_float_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){
   int ibuf=check_load_buffer(pos,iax1,iax2);
   return buf[ibuf]->get_float_data(pos,iax1,f1,e1,iax2,f2,e2);
}
float dataset::get_value(orient_cube *pos){
  if((int)buf.size()==0) return 0.;
  
  int ibuf=find_buffer(pos);
  return buf[ibuf]->get_value(pos);
}


long long dataset::get_trace_num(orient_cube *pos){
  long long ret=0;
  long long block=1;
  int n[8];
  pos->get_ns(n);
  for(int i=1; i < 8;i++){
   if(data_contains[i]){
     ret=ret+block*(long long)pos->get_loc(i);
     block=block*(long long)n[i];
   }
  }
  return ret;
}
void  dataset::snap_location(float *floc, int single, QString stype){

  if((int)buf.size()==0) return ;
  if(stype.contains("no")>0) return;
    orient_cube pos(grid);
  for(int i=0; i < 8; i++) pos.set_pos(i,floc[i]);

  int ibuf=find_buffer(&pos);
  int ilocs[8];
  pos.get_locs(ilocs);
  axis ax=grid->get_axis(single+1);

  int ichoose=0;
  float val=buf[ibuf]->get_value(&pos);
  int iloc=pos.get_loc(single);
  for(int i=0,j=-5; i< 5; i++,j++){
    if(iloc+j>=0 && iloc+j < ax.n){
      pos.set_loc(single,iloc+j);
      float test=buf[ibuf]->get_value(&pos);
      if(stype.contains("low")>0 &&  test < val){
         val=test; ichoose=j;
      }
      else if(stype.contains("high")>0 &&  test > val){
         val=test; ichoose=j;
      }
      else if(stype.contains("zero")>0 &&  fabs(test) < fabs(val)){
         val=test; ichoose=j;
      }
    }
  }
  pos.set_loc(single,ichoose+iloc);
  floc[single]=pos.get_pos(single);

}
int dataset::check_load_buffer(orient_cube *pos, int iax1,  int iax2){
        
 
 for(int i=0; i < (int)buf.size(); i++){
   if(buf[i]->hold_slice(pos,iax1,iax2,data_contains)) return i;
 }
   if((int)buf.size() == nmax_buf) delete_dataset(pos,iax1,iax2);

 buf.push_back(create_buffer(pos,iax1,iax2));

 std::vector<QString> com2; com2.push_back(QString::number(inum)); com2.push_back("menu");
 com2.push_back("Clip");
 com2.push_back("set_histo");
 com2.push_back(return_histogram());
 emit actionDetected(com2);
 return buf.size()-1;  
      
}
void dataset::delete_dataset(orient_cube *pos,int iax1, int iax2){
  if(pos==0 || iax1==0 || iax2==0){}
  delete buf[0];
   buf.erase(buf.begin());
}
int dataset::find_buffer(orient_cube *pos){

  float loc[8];
  pos->get_poss(loc);
  
  for(int i=0; i < (int)buf.size(); i++){
    if(buf[i]->hold_point(loc,data_contains)) return i;
  }
  fprintf(stderr,"BUFFER PROBLEM %d %s %d %d %d \n",inum,name.ascii(),
    pos->loc[0],pos->loc[1],pos->loc[2]);
  char errmsg[] = "Internal error finding point in buffer\n";
  par->error(errmsg);
  return 0;
}
QString dataset::return_clip(){
 QString buf;
 if(clip.size()==0) return "";
 buf=QString::number(clip[0].pct)+":"+QString::number(clip[0].clip);
 for(int i=1; i< (int)clip.size();i++){
   buf=buf+":"+QString::number(clip[i].pct)+":"+QString::number(clip[i].clip);
 }
 return buf;
}
QString dataset::return_histogram(){
 QString b;
 float *histo;
 if((int)buf.size()==0){
   histo=new float [256];
   for(int i=0; i < 256; i++) histo[i]=0.;
 }
 else   histo=buf[0]->return_histo();
 b=QString::number(histo[0]);
 for(int i=1; i< 256;i++){
   b=b+":"+QString::number(histo[i]);
 }
 delete [] histo;
 return b;
}
void dataset::get_pt(int ipt, float *pct, float *clp){
  *pct=clip[ipt].pct;
  *clp=clip[ipt].clip;

}
void dataset::del_pt(int ipt){
 if(clip.size() >1){
   for(int i=ipt; i< (int)clip.size()-1; i++) clip[i]=clip[i+1];
   clip.pop_back();
 }
 else clip.clear();
  build_conv();
}
void dataset::clear_pts(){
clip.clear();
build_conv();

}
int dataset::add_pt(float pct,float clp){
  int iin=0;

  if(clip.size()==0) clip.push_back(bar_pt(pct,clp));
  else{


  if(clip[0].pct >pct) iin=0;
  else if(clip[clip.size()-1].pct <= pct) {
  

  iin=clip.size();

  
}
  else{

    for(int i=0; i < (int) clip.size()-1; i++){
       if(pct > clip[i].pct && pct <= clip[i+1].pct) iin=i+1;
    }
  }

  clip.push_back(bar_pt(0.,0.));

  for(int i=clip.size()-2; i !=iin-1; i--){
    clip[i+1].pct=clip[i].pct; clip[i+1].clip=clip[i].clip; 
  }

  clip[iin].pct=pct; clip[iin].clip=clp;
 }
    build_conv();
    return iin;
}
QString dataset::set_pts(float *pcts){
  QString cur=return_clip();
  clip.clear();
  for(int i=0; i< 256; i++) clip.push_back(bar_pt((float)i/255.,pcts[i]));
  build_conv();
  return cur;
  
}
void dataset::build_conv(){
  //Handle
  float bpct=0.,bclip=1,epct,eclip,f;
  for(int ipt=0; ipt < (int) clip.size(); ipt++){
    epct=clip[ipt].pct*255;
    eclip=clip[ipt].clip*255;
    for(int i=(int)(bpct+.5); i <= (int)(epct+.5); i++){
      f=((float)i-bpct)/(epct-bpct);
      conv[i]=(int)(((1.-f)*bclip+(f)*eclip)+.5);

    }
    bpct=epct; bclip=eclip;
  }
  epct=255;
  eclip=254;
  for(int i=(int)(bpct+.5); i <= (int)(.5+epct); i++){
      f=((float)i-bpct)/(epct-bpct);
      if(epct-bpct < .001) f=1.;
      conv[i]=(int)(((1.-f)*bclip+(f)*eclip)+.5);

  }


  std::vector<QString> com; com.push_back(QString::number(inum)); com.push_back("menu");
  com.push_back("Clip");
  com.push_back("set_pts"); com.push_back(return_clip());
  emit actionDetected(com);
}
QString dataset::validate_view(QString nm, orient_cube *pos){


// pos->get_axes(as);
 std::vector<int> n=return_io_hyper()->return_ns();
 
 int order[8]; pos->get_orders(order);
 bool three;
 bool valid[8];
 for(int i=0; i < 8; i++){
   valid[i]=valid_display(i,order,n);
}
 
 //See if requested view works
 if(nm=="CUBE" || nm=="CUT" || nm=="THREE"){
   if(valid[0] && valid[1] && valid[2]) return nm;
   three=true;
 }
 else {
    if(nm=="FRONT" && valid[0] && valid[1]) return nm;
    else if(nm=="SIDE" && valid[0] && valid[2]) return nm;
    else if(nm=="TOP" && valid[1] &&  valid[2]) return nm;
    three=false;
 }
 //Now choose another view
 if((three || (!three)) && valid[0]){
   if(valid[1])return "FRONT";
   else if(valid[2]) return "SIDE";
 }
 if(valid[1] && valid[2]) return "TOP";
 //At this stage just find the first two valid axes
 int iv1=-1,iv2=-1;
 for(int i=0; i < 8; i++){
   if(valid[i]){
     if(iv1<0) iv1=i;
     else if(iv2<0) iv2=i;
   }
 }

 //Time to mess with the order
 int iold=order[0]; order[0]=iv1; order[iv1]=iold;
 iold=order[1]; order[1]=iv2; order[iv2]=iold;
 for(int i=0; i < 8; i++) order[i]=order[i]+1;

 pos->set_orders(order);

 return "FRONT";
}

  bool dataset::valid_display(int iax, int *order,std::vector<int> ns){

      if(ns[order[iax]]>1 && display_axis[order[iax]] )return true;
      return false;
    }

