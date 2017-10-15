#include "orient_cube.h"
#include <math.h>
#include<assert.h>
#include <utility>
#include "seplib.h"

orient_cube::orient_cube(position *pos,int *o,orientation_server *s){
  init=true;
  block[0]=1;
  for(int i=0; i < 8; i++){
    this->set_axis(i,pos->get_axis(i));
    order[i]=o[i]-1;
    reverse[i]=false;
    if(i>0) block[i]=block[i-1]*get_axis(i-1).n;
   }
     // reg_to_rot_1=0;
    rot_to_reg_1=0;
  //  reg_to_rot_2=0;
    rot_to_reg_2=0;
   serv=s;
    rot_ax[0]=1; rot_ax[1]=2;
    ax_rot[0]=pos->get_axis(rot_ax[0]); ax_rot[1]=pos->get_axis(rot_ax[1]);
   orient_num=serv->get_new_num();
    set_no_rotate();
   
   this->sync_pos(pos);
   int iloc[8];
   get_locs(iloc);
   set_rot_pt(iloc);
   init_map_1d();


    orient_num=serv->get_new_num(orient_num,rot_ax,ax_rot,ang,rot_cen);
    get_locs(rot_pt);
    init=false;
 }
 orient_cube::orient_cube(hypercube *h,orientation_server *s){
   init=true;
   block[0]=1;
   serv=s;
    for(int i=0; i < 8; i++){
       this->set_axis(i,h->get_axis(i+1));
           if(i>0) block[i]=block[i-1]*get_axis(i-1).n;
}
  //  reg_to_rot_1=0;
    rot_to_reg_1=0;
//    reg_to_rot_2=0;
    rot_to_reg_2=0;
    set_no_rotate();
  rot_ax[0]=1; rot_ax[1]=2;
  get_locs(rot_pt);
     init_map_1d();
  init=false;
      orient_num=serv->get_new_num(orient_num,rot_ax,ax_rot,ang,rot_cen);

 }
 orient_cube::orient_cube(orient_cube *ori) {
   init=true;
   for(int i=0; i < 8; i++){
    this->set_axis(i,ori->get_axis(i));
    this->order[i]=ori->get_order(i);
    this->reverse[i]=ori->get_reverse(i);
    this->beg[i]=ori->beg[i];
    this->end[i]=ori->end[i];
    this->block[i]=ori->block[i];
   }
       serv=ori->serv;
    orient_num=serv->get_new_num();

//  reg_to_rot_1=0;
    rot_to_reg_1=0;
 //   reg_to_rot_2=0;
    rot_to_reg_2=0;
    rotate=false;
   this->ang=ori->get_rot_angle();
   ax_rot[0]=ori->ax_rot[0];
   ax_rot[1]=ori->ax_rot[1];
   rot_cen[0]=ori->rot_cen[0];
   rot_cen[1]=ori->rot_cen[1];

   ori->get_rot_axes(&rot_ax[0],&rot_ax[1]);
   ori->get_rot_pt(rot_pt);
   set_rotation();
      init_map_1d();
         this->sync_pos(ori);



  init=false;

 }
void orient_cube::init_map_1d(){
 int ns[8];
 get_ns(ns);
 blocks[0]=1;
// map_1d=new int*[8];
 for(int i=0; i < 8; i++){
 //  map_1d[i]=new int[ns[i]];
  // for(int j=0; j <ns[i]; j++){
   //  map_1d[i][j]=j;
  // }
   if(i!=0) blocks[i]=blocks[i-1]*ns[i-1];
 }
 n123=blocks[7]*ns[7];
 one_shift=0;
 begs=0;
 ends=0;
 shift_ax=-1;
}
void  orient_cube::rotation_to_grid_loc(int iax1, int iax2, int *iloc){
   int i3a=0,i3b=0;
   int ibig=form_map_name(iax1,iax2,0,&i3a,&i3b);
   update_map_order(ibig,false);
   rot_maps[ibig]->rotation_to_grid_loc(iloc);

}
void  orient_cube::orient_data_loc(int iax1, int iax2, int *iloc){
   int i3a=0,i3b=0;
   int ibig=form_map_name(iax1,iax2,0,&i3a,&i3b);
   update_map_order(ibig,false);
   rot_maps[ibig]->orient_data_loc(loc,iloc);


}
int orient_cube::form_map_name(int iax1,int iax2,int idelta,int *i3,int *i3v){
     if(order[0]==iax1 && order[1]==iax2) *i3=order[2];
     else if(order[0]==iax2 && order[1]==iax2) *i3=order[2];
     else if(order[0]==iax1 && order[1]==iax2) *i3=order[1];
     else if(order[0]==iax2 && order[1]==iax1) *i3=order[1];
     else {
       *i3=order[0];
       }
 //    fprintf(stderr,"MAP %d %d %d %d \n",loc[0],loc[1],loc[2],*i3);
     *i3v= std::max(0,std::min(get_axis(*i3).n-1,idelta+loc[*i3]));
  return (*i3v)*100+iax1+iax2*10;


}
void orient_cube::form_index_map(int iax1, int iax2, bool rev1, bool rev2){
    int i3a=0,i3v=0;
    int ibig=form_map_name(iax1,iax2,0,&i3a,&i3v);

 if(rot_maps.count(ibig)==0){ //need to create map
     int ns[8],bs[8],es[8],iloc[8];
     get_ns(ns);
     get_begs(bs);
     get_ends(es);
     get_locs(iloc);
     int b_s=0,e_s=0;
     
     if(shift_ax!=-1){
 
        b_s=bs[shift_ax]; 
        bs[shift_ax]=begs[b_s];
        e_s=es[shift_ax]; 
        es[shift_ax]=ends[e_s-1];

     }
     rot_maps[ibig]=
       new orient_map(rotate,iax1,iax2,rot_ax,ax_rot,rot_to_reg_1,rot_to_reg_2,
       bs,iloc,es,ns,rev1,rev2,one_shift,shift_ax,b_s,e_s);
     update_map_order(ibig,true);
    }
    else update_map_order(ibig,false);
}
long long *orient_cube::get_index_map_ptr(int iax1, int iax2, int f1, int e1, int f2, int e2,int ioff){
    
    int i3a,i3v;
    int ibig=form_map_name(iax1,iax2,ioff,&i3a,&i3v);
    
    int f_1,e_1,f_2,e_2;
    f_1=f1; e_1=e1; f_2=f2; e_2=e2;
    if(f1 >e1) {f_1=e1; e_1=f1;}
    if(f2>e2) { f_2=e2; e_2=f2;}
    bool rev1,rev2;
    //long long *tmp;
    bool found=true;
    if(rot_maps.count(ibig)==0) {
      // assert(1==0);
       found=false;
       rev1=false; rev2=false;
    }
    else{
      int locs[8];
      get_locs(locs);
      locs[i3a]=i3v;
      found=rot_maps[ibig]->check_same(locs,f_1,e_1,f_2,e_2);
    rev1=rot_maps[ibig]->rev1;
      rev2=rot_maps[ibig]->rev2;
     }
    if(found){
      update_map_order(ibig,false);
      //fprintf(stderr,"in found \n");
      return rot_maps[ibig]->get_index_map_ptr(); 
    }
    else{
     // fprintf(stderr,"in create \n");
      int ns[8],bs[8],es[8],iloc[8];
      get_ns(ns);
      get_begs(bs);
      get_ends(es);
      get_locs(iloc);
      int b_s=0,e_s=0;
      bs[iax1]=f_1; es[iax1]=e_1; bs[iax2]=f_2; es[iax2]=e_2;
      
     if(shift_ax!=-1){
        b_s=bs[shift_ax]; 
        bs[shift_ax]=begs[b_s];
        e_s=es[shift_ax]; 
        es[shift_ax]=ends[e_s-1];
       
     }
     iloc[i3a]=i3v;

      rot_maps[ibig]=new 
       orient_map(rotate,iax1,iax2,rot_ax,ax_rot,rot_to_reg_1,rot_to_reg_2,
       
       bs,
       iloc,
       es,
       ns,
       rev1,
       rev2,
       one_shift,
       shift_ax,
       b_s,
       e_s);
       update_map_order(ibig,true);

                 long long *tmp=rot_maps[ibig]->get_index_map_ptr();

       return tmp;
     }
 }
 void orient_cube::update_map_order(int big, bool newo){
   if(!newo){
     std::list<int>::iterator it1;
     it1=map_order.begin();
     while(*it1!=big) it1++;
     map_order.erase(it1);
   }
   map_order.insert(map_order.begin(),big);
   if(map_order.size()>MAX_MAPS){
   std::list<int>::iterator rit;
   rit=map_order.end(); rit--;
     rot_maps.erase(*rit);
     map_order.erase(rit);
   }
 
 }
 std::map<long long, int> *orient_cube::get_rot_map(int iax1, int iax2,int *n1){
   int i3a=0,i3b=0;
   int ibig=form_map_name(iax1,iax2,0,&i3a,&i3b);
   update_map_order(ibig,false);
    return rot_maps[ibig]->return_samp_dat_map(n1); 
 }
 void orient_cube::set_one_shift(int iax, int *buf){ 
   rotation_change();
   one_shift=buf; shift_ax=iax;
   int n=get_axis(shift_ax).n;
   begs=new int[n];
   ends=new int[n];

   for(int i=0; i < n; i++){ ends[i]=0; begs[i]=n;}
   long long i=0;
   //Figure out the depth range for each flattened space i
   for(long long i2=0; i2 < n123/(long long)n; i2++){
      for(int i1=0; i1 < n; i1++,i++){
         int iam=one_shift[i];
         begs[iam]=MIN(begs[iam],i1);
         ends[iam]=MAX(ends[iam],i1);
         
      }
   }
 //  for(int i1=0; i1 < n i1++){
  //   fprintf(stderr,"check min max why you suck %d %d %d \n",
   //    i1,begs[i1],ends[i1]);
    //   }
  //Now we need to handle the case where some tau doesn't correspond to any samples
  int lastb=-1,laste=-1;
  for(int i=0; i < n; i++){
     if(ends[i]==0 && begs[i]==n){
        if(lastb!=-1){
          begs[i]=lastb; ends[i]=laste;
        }
        else{
          begs[i]=0; ends[i]=0;
        }
     }
     else{
       lastb=begs[i]; laste=ends[i];
     }
   }
  
  

   //Figure 
   
   int j=n-1; while(begs[j]>0 && j>0) j--;
   if(j!=0) {
     for(int i1=0; i1 < j; i1++) begs[i1]=0;
   }
   
   j=0;
   while(ends[j]!=n-1 && j<n-1) j++;
   if(j!=n-1){
     for(int i1=j; i1<n; i1++) ends[i1]=n-1;
   }
   //IF there is only a single time slice with this tau value expand it by 1 on each side
   for(int i1=0; i1 < n;i1++){
      if(begs[i1]==ends[i1]){
        if(begs[i1]>0) begs[i1]--;
        else ends[i1]++;
      }
      ends[i1]+=1;

  }
 
}
 void orient_cube::delete_all(){
 
    // for(int i=0; i < 8; i++){
     //  delete [] map_1d[i];
   // }
   // delete [] map_1d;
   if(begs!=0) delete [] begs;
   if(ends!=0) delete [] ends;
    delete_maps();
 }
 void orient_cube::delete_maps(){
   if(rot_to_reg_1!=0){

     for(int i2=0; i2 < ax_rot[1].n; i2++){
       delete [] rot_to_reg_1[i2];
       delete [] rot_to_reg_2[i2];
     }
     delete [] rot_to_reg_1;
     rot_to_reg_1=0;
     delete [] rot_to_reg_2;
     rot_to_reg_2=0;
   }
   rotation_change();
 }
 axis orient_cube::get_rot_axis(int iax){
   if(!rotate || (rot_ax[0]!=iax && rot_ax[1]!=iax)) {
   //  if(rotate) fprintf(stderr,"not rotated %d %d %d \n",iax,rot_ax[0],rot_ax[1]);
     return get_axis(iax);
     }
   if(rot_ax[0]==iax) return ax_rot[0];
    return ax_rot[1];
 }
 void orient_cube::get_axis_range(int iax, int *b, int *e){
 
 if(!rotate || (rot_ax[0]!=iax && rot_ax[1]!=iax)) {
     *b=get_beg(iax);
     *e=get_end(iax);
   }
   else if(rot_ax[0]==iax){
     *b=b_rot[0]; *e=e_rot[0];
  }
  else {
    *b=b_rot[1]; *e=e_rot[1];
  }
  if(iax==shift_ax){

    *b=begs[*b]; *e=ends[*e-1];
  }
 }
 void orient_cube::form_maps(){
   axis a1=get_axis(rot_ax[0]),a2=get_axis(rot_ax[1]);
  // reg_to_rot_1=new int*[a2.n];
  // reg_to_rot_2=new int*[a2.n];
   
//   float *buf=new float[a1.n*a2.n*2];
   float cs=cos(ang), sn=sin(ang);
   rot_to_reg_1=new int*[ax_rot[1].n];
   rot_to_reg_2=new int*[ax_rot[1].n];
   cs=cos(-ang);
   sn=sin(-ang);
 int i=0;
 for(int i2=0; i2 < ax_rot[1].n; i2++){
     rot_to_reg_1[i2]=new int[ax_rot[0].n];
     rot_to_reg_2[i2]=new int[ax_rot[0].n];
     float p2=ax_rot[1].o+ax_rot[1].d*i2;
     float p1=ax_rot[0].o;
     for(int i1=0; i1 < ax_rot[0].n; i1++,i++){
       rot_to_reg_1[i2][i1]=
        (int)((((p1-rot_cen[0])*cs+sn*(p2-rot_cen[1])+
          rot_cen[0])-a1.o)/a1.d+.5);
       rot_to_reg_2[i2][i1]=
        (int)(((-(p1-rot_cen[0])*sn+cs*(p2-rot_cen[1])+
          rot_cen[1])-a2.o)/a2.d+.5);  
       p1+=ax_rot[0].d;
            
       if(rot_to_reg_2[i2][i1] < 0 || rot_to_reg_2[i2][i1] >=a2.n ||
           rot_to_reg_1[i2][i1] <0 || rot_to_reg_1[i2][i1] >=a1.n){
             rot_to_reg_1[i2][i1]=-1; rot_to_reg_2[i2][i1]=-1;
        }

     }
   }

 }
 void orient_cube::set_rot_axes(int a1, int a2){
     if(shift_ax==a1 || shift_ax==a2){
       std::cerr<<"Can not rotate along a shift axis"<<std::endl;
       return;
     }

     rot_ax[0]=a1; rot_ax[1]=a2; 

     set_rotation();
     
    
     }
 void orient_cube::set_no_rotate(){
    ang=0;
    rotate=false;
    delete_maps();
  }
 void orient_cube::sync_pos(position *pos){
    for(int i=0; i<8; i++){
      this->set_loc(i,pos->get_loc(i));
      this->set_beg(i,pos->get_beg(i));
      this->set_end(i,pos->get_end(i));
     }
     this->set_movie_dir(pos->get_movie_axis(),pos->get_movie_dir()); 

   }
 void orient_cube::center_it(){
    this->set_loc(order[0],(this->get_beg(0)+this->get_end(0))/2);
    this->set_loc(order[1],(this->get_beg(1)+this->get_end(1))/2);
    this->set_loc(order[2],(this->get_beg(2)+this->get_end(2))/2);
 
 }
 void orient_cube::set_rotation(){
       axis a1=get_axis(rot_ax[0]),a2=get_axis(rot_ax[1]);

  rot_cen[0]=a1.o+a1.d*rot_pt[rot_ax[0]];
      rot_cen[1]=a2.o+a2.d*rot_pt[rot_ax[1]];
   if(fabs(ang)<0.01 ) {
     set_no_rotate();
   }
   else{
      rotate=true;
     
      float bb1,bb2,ee1,ee2,be1,be2,eb1,eb2;
      rotate_pt(a1.o,a2.o,&bb1,&bb2);
      rotate_pt(a1.o+a1.d*(a1.n-1),a2.o,&eb1,&eb2);
      rotate_pt(a1.o,a2.o+a2.d*(a2.n-1),&be1,&be2);
      rotate_pt(a1.o+a1.d*(a1.n-1),a2.o+a2.d*(a2.n-1),&ee1,&ee2);
      //This logic might need to be reversed
      float min1,max1,min2,max2;
      if(ang>0.){
         min1=bb1; max1=ee1; min2=eb2; max2=be2;
      }
      else{
        min1=be1;  max1=eb1; min2=bb2; max2=ee2;
      }
         delete_maps();

      ax_rot[0].o=min1; ax_rot[1].o=min2;
      ax_rot[0].n=a1.n; ax_rot[1].n=a2.n;
      ax_rot[0].d=(max1-min1)/(a1.n-1);
      ax_rot[1].d=(max2-min2)/(a2.n-1);
      form_maps();

      update_extents();

   }
 }
 void orient_cube::update_extents(){
   rotation_change();

   if(rotate){
   int b1=get_beg(rot_ax[0]), b2=get_beg(rot_ax[1]);
   int e1=get_end(rot_ax[0]), e2=get_end(rot_ax[1]);
   b_rot[0]=ax_rot[0].n; e_rot[0]=0;
   b_rot[1]=ax_rot[1].n; e_rot[1]=0;
   
   

   for(int i2=0; i2 <ax_rot[1].n; i2++){
     for(int i1=0; i1 < ax_rot[0].n; i1++){
       if(rot_to_reg_1[i2][i1] >=b1  && rot_to_reg_1[i2][i1] < e1 &&
          rot_to_reg_2[i2][i1] >=b2 && rot_to_reg_2[i2][i1] < e2){
          b_rot[0]=MIN(b_rot[0],i1);
          b_rot[1]=MIN(b_rot[1],i2);
          e_rot[0]=MAX(e_rot[0],i1);
          e_rot[1]=MAX(e_rot[1],i2);
        }
     
     }
   }
   e_rot[0]++; e_rot[1]++;

   }
 }
void orient_cube::edge(){
  this->set_loc(order[0],this->get_end(0)-1);
  this->set_loc(order[1],this->get_end(1)-1);
  this->set_loc(order[2],this->get_end(2)-1);
 
 }  
void orient_cube::update_loc(){
    if(!init){
    // rotatation_change();
     load_map_1d();
    }
}
void orient_cube::load_map_1d(){
  if(one_shift==0) return;
  int iloc[8]; get_locs(iloc);
  int ns[8]; get_ns(ns);
  long long base=0;
  for(int i=0; i < 8; i++) {
    if(i!=shift_ax) base+=blocks[i]*iloc[i];
  }
  for(int i=0; i < ns[shift_ax]; i++) {
  //  map_1d[shift_ax][i]=one_shift[base+(long long) blocks[shift_ax] *i];
    }
}
void orient_cube::transpose(int i1,int i2){
    int j=order[i1];
    order[i1]=order[i2];
    order[i2]=j;
    rotation_change();
 }


