
#include "region.h"

void region_info::verb(int level){
  if(level>0){
    	if(set) fprintf(stderr, "Allocated value =%d \n",val);
    	else fprintf(stderr,"Pointed value=%d \n",val);
   }


}

region_tree::region_tree(hypercube *grid){

  name_tree="region tree defined";
  hyper=grid;
   inf2=new region_info();
   inf=inf2;changed=false;
   base=this;
   tree_parent=0;
   std::vector<float> new_loc;
   for(int i=0; i < 8; i++){
    b.push_back(0);
    int n=grid->get_axis(i+1).n;
    e.push_back(n);
    new_loc.push_back((float)n*.5);
    if(n>1) split_axes.push_back(i);
   }
   loc=new tree_loc(new_loc);
   tot_num=0; 
   inum=0;
     isregion=true;  
   
}

int region_tree::get_num(){
  tot_num+=1;
  return tot_num;
}
void region_tree::init_info(){
  this->inf2=new region_info();
  inf=inf2;
  inf2->val=((region_tree*)tree_parent)->inf2->val;

}
tmp_tree *region_tree::clone(){

 region_tree *x=new region_tree();
 
 x->basic_tmp_tree_alloc(tree_parent,base,loc,b,e,split_axes);
 region_tree *baset=(region_tree*) base;
 x->inum=baset->get_num();
 x->form_hash_key();
 return x;
}

bool region_tree::different_loc(int *lc,std::vector<int> *lcs){
  lcs->clear();
 
 fprintf(stderr,"in different \n");
  for(int i=0; i < 8; i++){
    if(this->e[i]-this->b[i] >1){ //Not an axis with a single point
      if(abs(lc[i]-this->loc->v[i]) >.501) { //This section was not created for this point
        lcs->push_back(i);
        fprintf(stderr,"AXIS %d doesn't work new=%d old=%f \n",
          i,lc[i],loc->v[i]);
      }
    }
  }
  if(lcs->size()>0) return false;
  return true;
}
int region_tree::return_oldest(){
  if(tree_child.size()>0){
    int iold=2*1000*1000*1000;
    int ibest=-1;
    for(int i=0; i <(int) tree_child.size(); i++){
        region_tree *x=(region_tree*) tree_child[i];
       int itest=x->return_oldest();
       if(itest >=0 && itest< iold) {
         ibest=i;
         iold=itest;
       }
    }
    if(ibest==-1){ //No valid members delete 
      for(int i=0; i <(int) tree_child.size(); i++){
        base->erase_limb(tree_child[i]->hash_key);
        delete tree_child[i];
      }
      tree_child.clear();
      set_limb();
      return -1;
    }
    if(ibest!=(int)tree_child.size()-1){
      region_tree *x=(region_tree*)tree_child[ibest];
      tree_child[ibest]=tree_child[tree_child.size()-1];
      tree_child[tree_child.size()-1]=x;
      this->inf2->val=x->inf2->val;
    }
    return iold;
  }
  else if(inf2->set) return inum;
  else return -1;
}
region::region(hypercube *grid){
  colors=my_colors();
   primary_c="red";
   secondary_c="green";
   _up=1;
   _left=1;
   primary=colors.cnum["red"];
   secondary=colors.cnum["green"];
  hyper_io=hyper_buf=grid;
  tr= new region_tree(grid);
  tr->inf2->val=primary;
  for(int i=0;i < 8; i++) hold[i]=true;
  io=tr;

}
unsigned char *region::get_char_data(orient_cube *pos, int iax1, int f1, int e1, int iax2,
      int f2, int e2){

      std::vector<tmp_tree*> branches;
      find_wind_branches(pos,iax1,f1,e1,iax2,f2,e2,&branches);
      int n1=e1-f1,n2=e2-f2;
      int f_1=f1,f_2=f2;
      if(n1 <0){
        n1=-n1;
        f_1=e1;
      }
      if(n2<0){
        n2=-n2;
        f_2=e2;
      }
      unsigned char *x=new unsigned char[n1*n2];
      
      
      
      if((int)branches.size()==0){
      //fprintf(stderr,"FOUND 0 setting to %d \n",primary);
        for(int i=0; i< n1*n2; i++) x[i]=primary;
      }
      
      for(int ib=0; ib <(int) branches.size(); ib++){
        int ib2=branches[ib]->b[iax2]-f_2;
        if(ib2 < 0) ib2=0;
        int ie2=branches[ib]->e[iax2]-f_2;
        if(ie2 >n2) ie2=n2;
        int ib1=branches[ib]->b[iax1]-f_1;
        if(ib1 < 0) ib1=0;
        int ie1=branches[ib]->e[iax1]-f_1;
        if(ie1 >n1) ie1=n1;
         region_info *inf=(region_info *) branches[ib]->inf;
        int my_val=inf->val;
       if(!inf->set && branches[ib]->tree_parent->npts==0){
         region_info *inf2=(region_info*) branches[ib]->tree_parent->inf;
         my_val=inf2->val;
       }
      //   fprintf(stderr,"FILLING IN %d,%d to %d,%d with %d (%f %f %f) \n",
       //    ib1,ib2,ie1,ie2,my_val,
        //     branches[ib]->loc->v[0],
         //    branches[ib]->loc->v[1],
          //   branches[ib]->loc->v[2]
           //  );
        for(int i2=ib2; i2 < ie2; i2++){
          for(int i1=ib1; i1 < ie1; i1++){
          
            x[i1+i2*n1]=my_val;
          }
        }

      }
     
      /*
      for(int i2=0; i2 < n2; i2++){
        for(int i1=0;i1 < n1; i1++){
         fprintf(stderr,"CHECK out %d %d %d -%d \n",
            i1,i2,i,x[i]);
            i++;
            }}
            */
      return x;
}
 void region::find_wind_branches(orient_cube *pos,int iax1, int f1, int e1, int iax2, int f2,
 int e2, std::vector<tmp_tree*> *branches){
 
       int b[8],e[8];
      for(int i=0; i < 8; i++) {
        if(i==iax1){
           if(e1>f1) {b[i]=f1; e[i]=e1;}
           else { b[i]=e1; e[i]=f1;}
        }
        else if(i==iax2){
          if(e2>f2) {b[i]=f2; e[i]=e2;}
          else{ b[i]=e2; e[i]=f2;}
        }
        else{
          b[i]=pos->get_loc(i);
          e[i]=b[i]+1;        
        }
      }
    
      tr->find_wind_branches(b,e,branches);
 }
 
 float *region::get_float_data(orient_cube *pos, int iax1, int f1, int e1, int iax2, int f2, int e2){
            int n1=e1-f1,n2=e2-f2;

    unsigned char *temp_buf=get_char_data(pos,iax1,f1,e1,iax2,f2,e2);
    float *x=new float[n1*n2];
    for(int i=0; i < n1*n2; i++) x[i]=(float)temp_buf[i];
    delete [] temp_buf;
    return x;
    
      
}
void region::init_info(){

}
float region::get_value(orient_cube *pos){
  region_tree *tr=(region_tree*)io;
  region_tree *trs=(region_tree*) tr->find_closest(pos->loc);
  if(trs==tr->base) return primary;
  region_info *inf2=(region_info*)trs->inf;
  if(inf2->set) return (float) inf2->val;
  
  inf2=(region_info*) trs->tree_parent->inf;
  return (float)inf2->val;
}

void region::add_pt(int *loc2, int cur_val2, std::vector<int> ax){

  int loc[8];
  memcpy(loc,loc2,32);
  
  int cur_val=cur_val2;
/*
  if(inum==0){ loc[0]=6; loc[1]=2; cur_val=8;}
  if(inum==1){ loc[0]=12; loc[1]=4;cur_val=24;}
  if(inum==2){loc[0]=2; loc[1]=15;cur_val=40;}
  if(inum==3){loc[0]=0; loc[1]=10;cur_val=56;}
  inum+=1;
  //fprintf(stderr,"LOCATION IS %d %d \n",loc[0],loc[1]);
  
*/
  region_tree *nearest=(region_tree*) tr->find_closest(loc);
  while(!nearest->single_pt(ax)){
    nearest->make_tree_children(ax,loc);
    for(int i=0; i < (int)nearest->tree_child.size(); i++){
       region_info *ii=(region_info*)nearest->tree_child[i]->inf;
       ii->val=cur_val2;
    }
    nearest=(region_tree*) nearest->find_closest(loc);
    assert(nearest->tree_parent!=0);
  }
  std::vector<int> same_axes;
  region_info *ii=(region_info*)nearest->inf;
  int vv=ii->val;
  int old_loc[8]; for(int i=0; i < 8; i++) old_loc[i]=nearest->loc->v[i];
  float old_locf[8]; for(int i=0; i < 8; i++) old_locf[i]=nearest->loc->v[i];
  while(!nearest->different_loc(loc,&same_axes)){
    nearest->make_tree_children(same_axes,loc);
        fprintf(stderr,"NEAREST2 FOUND %d %d %f \n",nearest->b[2],nearest->e[2],nearest->loc->v[2]);

    fprintf(stderr,"SPLIT AXIES %d \n",same_axes[0]);
    region_tree *old=(region_tree*)nearest->find_closest(old_loc);
            fprintf(stderr,"NEAREST3 FOUND %d %d %f \n",old->b[2],old->e[2],old->loc->v[2]);

    for(int i=0; i < 8; i++) old->loc->v[i]=old_locf[i];
    for(int i=0; i < (int)nearest->tree_child.size(); i++){
       region_info *ii=(region_info*)nearest->tree_child[i]->inf;
       fprintf(stderr,"looping children %d %d %d \n",i,nearest->tree_child[i]->b[2],
       nearest->tree_child[i]->e[2]);
       ii->val=vv;
       if(nearest->tree_child[i]==old) ii->set=true;
    }
    nearest=(region_tree*) nearest->find_closest(loc);
    fprintf(stderr,"NEAREST FOUND %d %d %f \n",nearest->b[2],nearest->e[2],nearest->loc->v[2]);
  }
 ii=(region_info*)nearest->inf;
 ii->val=cur_val2;
    
    fprintf(stderr,"LOCATION MATCH IS %f %f %f\n",nearest->loc->v[0],nearest->loc->v[1],nearest->loc->v[2]);
    
  //fprintf(stderr,"SET %d %d \n",loc[0],loc[1]);
  region_info *inf2=(region_info*) nearest->inf;
  inf2->set_val(cur_val-1);
//  inf2->val=cur_val-1;
//  fprintf(stderr,"before write statemetn \n");
//  fprintf(stderr,"SETTING %f %f %f to %d \n",nearest->loc->v[0],nearest->loc->v[1],nearest->loc->v[2],
   // (inf2->val));
 // if(tr->tree_parent!=0){
  //   region_tree *trp=(region_tree*) tr->tree_parent;
   //  region_info *infp=(region_info*) trp->inf;
    // infp->val=inf2->val;
  // }

  region_tree *par=(region_tree*) nearest->tree_parent;
  

  while(par!=0){
    int iam=-1;
    int ntree_child=par->tree_child.size();
    
    for(int i=0; i< ntree_child; i++){
      if(par->tree_child[i]==nearest) iam=i;
    }   
    assert(iam!=-1);
    tmp_tree *junk=par->tree_child[iam];
 // fprintf(stderr,"In this 2while \n");

    if(iam!=ntree_child-1){
      for(int i=iam+1; i < ntree_child; i++)
        par->tree_child[i-1]=par->tree_child[i];
      par->tree_child[ntree_child-1]=junk;  
   }
   region_tree *trp=(region_tree*) nearest;
   trp->inf2->val=inf2->val;
   //fprintf(stderr,"I THINK I SET THIS %d \n",(trp->inf2->val));
   nearest=par;
   
   par->npts+=1;
   par=(region_tree*) nearest->tree_parent;   
  }
  
  nearest->inf2->val=cur_val;
 // tr->verb(3);
}
void region::del_pt(orient_cube *pos, bool update_it){

//  region_tree *nearest=(region_tree*) tr->find_closest(pos->loc);
 // if(!nearest->single_pt()) assert(1==0);
 // delete_region_tree(nearest,update_it);
}
void region::delete_region_tree(region_tree *nearest, bool update_it){
  region_info *inf=(region_info*) nearest->inf;
  //region_tree *par=(region_tree*) nearest->tree_parent;
 // region_info *infp=(region_info*) par->inf;
  inf->del_val();
  if(update_it){
    region_tree *x=(region_tree*) nearest->base;
    int iold=x->return_oldest();
    if(iold==-1);
  }
  
}

void region::delete_range(orient_cube *pos, int iax1, int b1, int e1, int iax2 ,int b2, int e2){

  std::vector<tmp_tree*> branches;
  find_wind_branches(pos,iax1,b1,e1,iax2,b2,e2,&branches);
  for(int ib=0; ib < 0; ib++){
    delete_region_tree((region_tree*)branches[ib],false);
  }
   region_tree *x=(region_tree*) tr->base;
    int iold=x->return_oldest();
if(iold==0);
}
void region::form_continuous(std::vector<int> coord1,std::vector<int> coord2,
     std::vector<int> *use1,std::vector<int> *use2,int *beg1,int *beg2,int *end1,int *end2){
     
   *beg1=*end1=coord1[0]; 
   *beg2=*end2=coord2[0];
   
   int i=1;
   int oldp[2];
   oldp[0]=coord1[0],oldp[1]=coord2[0];
   use1->push_back(coord1[0]);
   use2->push_back(coord2[0]);
   
   while(i< (int)coord1.size()){
     int newp[2]; newp[0]=coord1[i],newp[1]=coord2[i];
     if(*beg1 > newp[0]) *beg1=newp[0];
     if(*beg2 > newp[1]) *beg2=newp[1];
     if(*end1 < newp[0]) *end1=newp[0];
     if(*end2 < newp[1]) *end2=newp[1];
   //  fprintf(stderr,"BEG SET TO AT POINT %d %d %d \n",*beg1,*beg2,i);
     int iline,iext;
     bool do1;
     float slope;
     if(abs(newp[1]-oldp[1]) > abs(newp[0]-oldp[0])){
       iline=1; iext=1; do1=true;
       slope=(float)(newp[0]-oldp[0])/((float)(newp[1]-oldp[1]));
     }
     else{
       iline=0; iext=1; do1=false;
       slope=(float)(newp[1]-oldp[1])/((float)(newp[0]-oldp[0]));

     }
//     if(do1) fprintf(stderr,"doing 1 old=%d new=%d slope=%f \n",
 //       oldp[1],newp[1],slope);
 //   else fprintf(stderr,"doing 0 old=%d new=%d slope=%f \n",
  //      oldp[0],newp[0],slope);
     int dir=-1;
     if(newp[iline]-oldp[iline]>0) dir=1;
     //fprintf(stderr,"THE SIZE %d dir=%d\n",abs(newp[iline]-oldp[iline]),dir);
     for(int ip=1; ip <= abs(newp[iline]-oldp[iline]); ip++){
       if(do1){
         use2->push_back(oldp[1]+dir*ip);
         use1->push_back(oldp[0]+(int)(dir*ip*slope));
       //  fprintf(stderr,"push back 1 %d %d \n",oldp[1]+dir*ip,oldp[0]+(int)(dir*ip*slope));
       }
       else{
         use1->push_back(oldp[0]+dir*ip);
         use2->push_back(oldp[1]+(int)(dir*ip*slope));
  //       fprintf(stderr,"push back 2 %d %d  (%d %d %f)\n",oldp[1]+(int)(dir*ip*slope),
       //    oldp[0]+dir*ip,dir,ip,slope);
       }
     }
     i+=1;
     oldp[0]=newp[0]; oldp[1]=newp[1];
   } 
   
}
void region::partition_region(orient_cube *pos, int iax1, std::vector<int> coord1,
  int iax2,std::vector<int> coord2){
 
// coord1.clear(); coord2.clear();
 //coord1.push_back(7); coord2.push_back(1);
// coord1.push_back(5); coord2.push_back(3);
 //coord1.push_back(10); coord2.push_back(12);
 
  int dir1=_up;
  int dir2=_left;
  int val1=primary;
  int val2=secondary;
  int beg1,beg2,end1,end2;
  std::vector<int> use1;
  std::vector<int> use2;
 // fprintf(stderr,"about to form continuous \n");
  form_continuous(coord1,coord2,&use1,&use2,&beg1,&beg2,&end1,&end2);
 // fprintf(stderr,"finished continuos \n");
  int n1a=pos->get_axis(iax1).n;
  int n2a=pos->get_axis(iax2).n;
  int n1=end1-beg1+3;
  int n2=end2-beg2+3;
  beg1-=1;
  beg2-=1;
  int **map=new int*[n1];
  int **sum=new int*[n1];
  for(int i1=0; i1 < n1; i1++){
    map[i1]=new int[n2];
    sum[i1]=new int[n2];
  }
  std::vector<int> ax; ax.push_back(iax1); ax.push_back(iax2);
  
  //fprintf(stderr,"WRITING OUT %d %d  (%d %d) \n",n1,n2,beg1,beg2);
  for(int i2=0; i2 < n2; i2++)
    for(int i1=0; i1 < n1; i1++) map[i1][i2]=0; 
    for(int i=0; i < (int) use1.size(); i++){
     // fprintf(stderr,"USE IS %d %d %d \n",i,use1[i]-beg1,use2[i]-beg2);
      map[use1[i]-beg1][use2[i]-beg2]=1;
   //   fprintf(stderr,"WAH \n");
     }
   //  fprintf(stderr,"FFF \n");
    for(int i1=0; i1 < n1; i1++){
      //srite("test.H",map[i1],n2*4);
      int b1=i1-1, e1=i1+1;
      if(b1<0) b1=0; if(e1>=n1) e1=n1-1;
      for(int i2=0; i2 < n2; i2++){
        int b2=i2-1, e2=i2+1;
        if(b2<0) b2=0; if(e2>=n2) e2=n2-1;
        sum[i1][i2]=0;
        for(int j=b2; j<=e2; j++){
          for(int i=b1; i <=e1; i++){
            sum[i1][i2]+=map[i][j];
          }
        }

      //          fprintf(stderr,"val is %d %d \n",i1,i2);

     //   fprintf(stderr,"LOC %d %d %d %d \n",i1,i2,map[i1][i2],sum[i1][i2]);
      }
    }
 // fprintf(stderr,"formed sum \n");
  
  //Figure out the place for the first element of the other line
  int delta[2];
  delta[0]=use1[1]-use1[0]; delta[1]=use2[1]-use2[0];
  int l1=0,l2=0;
  int u=dir1,l=dir2;
  if(u==0 && l==0) u=1;
  bool set=false;
  int ic=1;
  while(!set && ic < (int)use1.size()){
    if(abs(delta[0]) >0 && l!=0){
      l1=use1[0]-beg1;
      if(l<0) l2=use2[0]-1-beg2;
      else l2=use2[0]+1-beg2;
      set=true;
    }
    else if(abs(delta[1]) >0 && u!=0){
      l2=use2[0]-beg2;
      if(u<0) l1=use1[0]-1-beg1;
      else l1=use1[0]+1-beg1;
      set=true;
    }
    ic+=1;
    if(ic < (int)use1.size()){
     delta[0]=use1[ic]-use1[0]; delta[1]=use2[ic]-use2[0];
     }
    // if(set) fprintf(stderr,"i am set \n");
   //  else fprintf(stderr,"not set\n");
   }
   
  // fprintf(stderr,"about to start adding points \n");
   int loc[8];
   memcpy(loc,pos->loc,8*sizeof(int));
   for(int ip=0; ip< (int)use1.size(); ip++){
     loc[iax1]=use1[ip];
     loc[iax2]=use2[ip];
     add_pt(loc,val1,ax);

   }
   if(set){
  // fprintf(stderr,"AAA \n");
     delta[0]=use1[1]-use1[0]; delta[1]=use2[1]-use2[1];
  //   fprintf(stderr,"ABOUT TO SET END POINT TO %d %d (%d %d %d -%d %d %d)\n",
   //     use1[0]-beg1-delta[0],use2[0]-beg2-delta[1],use1[0],beg1,delta[0],
    //      use2[0],beg2,delta[1]);
          int i1,i2;
          i1=use1[0]-beg1-delta[0];
          i2=use2[0]-beg2-delta[1];
          map[i1][i2]=-1;
          int n=use1.size();
          delta[0]=use1[n-1]-use1[n-2]; delta[1]=use2[n-1]-use2[n-2];
          i1=use1[n-1]-beg1+delta[0];
          i2=use2[n-1]-beg2+delta[1];

     map[i1][i2]=-1;
   //  fprintf(stderr,"XXX \n");
     std::vector<int> ose1,ose2;
   //  fprintf(stderr,"before add boundaries \n");
     add_boundaries(l1,l2,n1,n2,beg1,beg2,map,sum,&ose1,&ose2);
  //   fprintf(stderr,"after add boundaries \n");
     for(int ip=0; ip< (int) ose1.size(); ip++){
       loc[iax1]=ose1[ip];
       loc[iax2]=ose2[ip];
       if(loc[iax1]>=0 && loc[iax1] <n1a && loc[iax2]>=0 && loc[iax2] < n2a){
         add_pt(loc,val2,ax);
       }
     }
     
   }
  // fprintf(stderr,"time to clean up \n");
   for(int i1=0; i1 < n1;i1++){
     delete [] map[i1];
     delete [] sum[i1];
   }
   delete [] map;
   delete [] sum;
   
//  tr->verb(2);
  
   
 }
 void region::add_boundaries(int l1,int l2, int n1, int n2, int beg1, int beg2, int **map,
 int **sum, std::vector<int>*ose1,
   std::vector<int> *ose2){
    
    
   //  fprintf(stderr,"that was the argument %d,%d - %d,%d \n",
   //    l1,l2,l1+beg1,l2+beg2);
     map[l1][l2]=2;
     ose1->push_back(l1+beg1);
     ose2->push_back(l2+beg2);
     for(int j=l1-1; j<=l1+1; j++){
      if(j>=0 && j<n1){
        for(int i=l2-1; i<=l2+1; i++){
          if(i>=0 && i <=n2){
            if(map[j][i]==0  && sum[j][i]>0) {//Then this could be a good point
            // if(l1==4 && l2==1) fprintf(stderr,"TESTING POINT %d %d \n",j,i);
              if(j !=l1 && i!= l2){
               //  if(l1==4 && l2==1)fprintf(stderr,"CHecking diagonal point %d %d \n",j,i);
                if(abs(map[j][l2])!=1 || abs(map[l1][i])!=1){
               //     if(l1==4 && l2==1)fprintf(stderr,"passed test %d-%d %d-%d \n",j,l2,l1,i);
                                 fprintf(stderr,"going to %d %d from %d %d \n",j+beg1,i+beg2,l1+beg1,l2+beg2);

                  add_boundaries(j,i,n1,n2,beg1,beg2,map,sum,ose1,ose2);
                 }
              }
              else{
                // if(l1==4 && l2==1)fprintf(stderr,"in else and adding %d %d \n",j,i);
               fprintf(stderr,"going to %d %d from %d %d \n",j+beg1,i+beg2,l1+beg1,l2+beg2);
                add_boundaries(j,i,n1,n2,beg1,beg2,map,sum,ose1,ose2);
              }
            }
          }
        }
      } 
    }
  }
  
void region::draw_region_pts(QPainter *painter, orient_cube *pos, slice *slice){

      std::vector<tmp_tree*> branches;
      
      int iax1=slice->iax1;
      int iax2=slice->iax2;
      
      int f1=pos->get_beg(iax1);
      int f2=pos->get_beg(iax2);

      int e1=pos->get_end(iax1);
      int e2=pos->get_end(iax2);
      
      find_wind_branches(pos,iax1,f1,e1,iax2,f2,e2,&branches);
      QPen pen,pen2;
     pen.setStyle(Qt::DashLine);
      pen.setColor(Qt::darkGreen); 
       pen2.setColor(Qt::darkGreen); 

	            painter->setPen(pen);
QBrush solid=QBrush(Qt::SolidPattern);
	QBrush empty=QBrush(Qt::NoBrush);
  // painter->setPen(*pen);
   for(int ib=0; ib <(int) branches.size(); ib++){
     region_info *inf=(region_info *) branches[ib]->inf;
    // if(inf->set){
       int x[4],y[4];
       int b_1=branches[ib]->b[iax1];
       int b_2=branches[ib]->b[iax2];
       int e_1=branches[ib]->e[iax1];
       int e_2=branches[ib]->e[iax2];
       if(b_1 < f1) b_1=f1;
       if(b_2 < f2) b_2=f2;
       if(e_1 > e1) e_1=e1;
       if(e_2 > e2) e_2=e2;

       slice->get_shifted_image_pos_from_sample(b_2,b_1,&x[0],&y[0]);
       slice->get_shifted_image_pos_from_sample(b_2,e_1,&x[1],&y[1]);
       slice->get_shifted_image_pos_from_sample(e_2,e_1,&x[2],&y[2]);
       slice->get_shifted_image_pos_from_sample(e_2,b_1,&x[3],&y[3]);
       pen.setStyle(Qt::DashLine);
       QPolygon poly;
       poly << QPoint(x[0],y[0])<<QPoint(x[1],y[1])<<QPoint(x[2],y[2])<<QPoint(x[3],y[3]);


/*
fprintf(stderr,"GET RANGE(%d %d %d %d) LOCATION %d-%d %d-%d %d-%d %d-%d \n",
slice->bx,slice->by,slice->ex,slice->ey,
  x[0],y[0],x[1],y[1],x[2],y[2],x[3],y[3]);
*/

    
      painter->drawPolygon(poly);
       if(inf->set){
      
    
              painter->setPen(pen2);
           int i_x[4],i_y[4];
          slice->get_shifted_image_pos_from_sample((int)(branches[ib]->loc->v[iax2]),
           (int)(branches[ib]->loc->v[iax1]),&i_x[0],&i_y[0]);
          slice->get_shifted_image_pos_from_sample((int)(branches[ib]->loc->v[iax2]+1),
           (int)(branches[ib]->loc->v[iax1]),&i_x[1],&i_y[1]);          
           slice->get_shifted_image_pos_from_sample((int)(branches[ib]->loc->v[iax2]),
           (int)(branches[ib]->loc->v[iax1]+1),&i_x[2],&i_y[2]);          
           slice->get_shifted_image_pos_from_sample((int)(branches[ib]->loc->v[iax2]+1),
           (int)(branches[ib]->loc->v[iax1]+1),&i_x[3],&i_y[3]);   
           int ix=i_x[0]+i_x[1]+i_x[2]+i_x[3];
           int iy=i_y[0]+i_y[1]+i_y[2]+i_y[3];
           ix=ix/4;
           iy=iy/4;
           //  fprintf(stderr,"CHECK THIS %d %d %f %f %f\n",ix,iy,branches[ib]->loc->v[iax1],
            //   branches[ib]->loc->v[iax2],branches[ib]->loc->v[2]);
             
             QPolygon poly2;
             poly2 << QPoint(ix-2,iy-2)<<QPoint(ix-2,iy+2)<<QPoint(ix+2,iy+2)<<QPoint(ix+2,iy-2);
              painter->drawPolygon(poly2);
              					painter->setBrush(Qt::SolidPattern);
        
             painter->drawPolygon(poly2);
             painter->setPen(pen);
             					painter->setBrush(empty);

           }
       
  //   }
   }
 }
