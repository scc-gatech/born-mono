#include "picking.h"
#include "my_colors.h"
#ifndef MAX
#define MAX(a,b) ( ((a)>(b)) ? (a):(b) )
#endif
#ifndef MIN
#define MIN(a,b) ( ((a)<(b)) ? (a):(b) )
#endif

picking::picking(){
  draw_what="all";
  changed=false;
  extra_state=0;
  active_col="red";
  my_colors cols=my_colors();
  bulk=0;
  std::vector<QString> cvec=cols.return_cvec_big();

  for(int i=0; i < (int)cvec.size(); i++){
    QColor col=cols.return_qcolor_big(cvec[i]);
    all[cvec[i]]=new picks(col);
   }
   for(int i=0; i <8; i++) dig[i]=1;
   


}
void picking::set_position(position *pos){

  pos->get_ns(n);
  pos->get_os(o);
  pos->get_ds(d);
    my_colors cols=my_colors();
  std::vector<QString> cvec=cols.return_cvec_big();
blk[0]=1;
for(int i=1; i <8; i++) blk[i]=blk[i-1]*n[i-1];
for(int i=0; i <8; i++) d[i]*=dig[i];
for(int i=0; i < (int)cvec.size(); i++){
    QColor col=cols.return_qcolor_big(cvec[i]);
    all[cvec[i]]->set_ns(n);
   }

}
void picking::create_planes(){

 for(int i=0; i < 8; i++){
   for(int j=i+1; j<8; j++){
     if(n[i] >1 && n[j]>1) planes.push_back(plane_contain(n,i,j,&all));
     
   }
  }
  set_dist(4);


}
plane_contain *picking::find_plane_contain(int i1, int i2){
   int iax1=i1, iax2=i2;
  if(i1 > i2) {
    iax1=i2; iax2=i1;
   
  }
for(int i=0; i < (int)planes.size(); i++){
     if(iax1==planes[i].iax1 && iax2==planes[i].iax2) 
         return &planes[i];
     }
  return NULL;
}
pick_plane  *picking::find_plane(int i1, int i2,int *ilocs){
 int iax1=i1, iax2=i2;
  if(i1 > i2) {
    iax1=i2; iax2=i1;
   
  }
  //int ilocs[8];
  //pos->get_locs(ilocs);
  int j=0;
  int ipos[6];

  for(int i=0; i < 8; i++){
    if(i!=iax1 && i!=iax2){
       if(dig[i]==0.) ipos[j]=0;
       else ipos[j]=ilocs[i];
      j++;
    }
  }
  
  for(int i=0; i < (int)planes.size(); i++){
     if(iax1==planes[i].iax1 && iax2==planes[i].iax2) {
 // fprintf(stderr,"RETURN (%d %d) %d %d %d \n",i1,i2,ipos[0],ipos[1],ipos[2]);
  return planes[i].tot[ipos[5]][ipos[4]][ipos[3]][ipos[2]][ipos[1]][ipos[0]];
     }
   }
     

   return NULL;

}
void picking::find_neighbors(int i_1, int i_2, int *ilocs, std::vector<pick_plane*>*neigh,
  std::vector<int> *dist){
  
    int i1=i_1,i2=i_2;
  if(i_1>i_2){i2=i_1; i1=i_2;}
  int j=0;
  int ipos[6];
 // fprintf(stderr,"neighbors ? %d %d %d %d\n",i_1,i_2,i1,i2);
  for(int i=0; i < 8; i++){
    if(i!=i1 && i!=i2){
      ipos[j]=ilocs[i];
      j++;
    }
  }
  for(int i=0; i < (int)planes.size(); i++){
     if(i1==planes[i].iax1 && i2==planes[i].iax2) {
        planes[i].form_neighbors(ipos,ndist,neigh,dist);
        break;
     }
   }
  
  }
void picking::draw(QPainter *painter, orient_cube *pos,slice *slc,dataset *dat){
  std::vector<pick> lst;
  QPen pen;
  int iax1,iax2,iax3;
  if(dat==0);
 
  slc->get_slice_axes(pos,&iax1,&iax2,&iax3);
  int iloc[8];
  pos->get_locs(iloc);
  for(int i=0; i< 8; i++) iloc[i]*=dig[i];
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  std::vector<pick_plane*> neighs;
  std::vector<int> dist;
  find_neighbors(iax1,iax2,iloc,&neighs,&dist);
  plane->draw(painter,ndist,&neighs,&dist,pos,slc,draw_what);
}
pairs picking::get_pts_sort(orient_cube *pos,slice *slc,int ival,int isort, QString col){
  int iax1,iax2, iax3;
  if(col=="None") col=active_col;
  slc->get_slice_axes(pos,&iax1,&iax2,&iax3);
  int is=1;
  if(iax1==isort) is=0;
    int iloc[8];
  pos->get_locs(iloc);
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  float o2[2],d2[2];
  o2[0]=o[iax1]; o2[1]=o[iax2];
  d2[0]=d[iax1]; d2[1]=d[iax2];
  return plane->get_pts_sort(iax1,iax2,o2,d2,ival,is,col,pos);
}
pairs picking::get_pts_sort(int *iloc,int iax1, int iax2, int isort,int ival, QString col){
 if(col=="None") col=active_col;
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  float o2[2],d2[2];
  o2[0]=o[iax1]; o2[1]=o[iax2];
  d2[0]=d[iax1]; d2[1]=d[iax2];
  return plane->get_pts_sort(iax1,iax2,o2,d2,ival,isort,col);
}
pairs picking::get_pts_sort_le(orient_cube *pos,slice *slc,int ival,int isort, QString col){
  int iax1,iax2, iax3;
  if(col=="None") col=active_col;
  slc->get_slice_axes(pos,&iax1,&iax2,&iax3);
  int is=1;
  if(iax1==isort) is=0;
    int iloc[8];
  pos->get_locs(iloc);
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  float o2[2],d2[2];
  o2[0]=o[iax1]; o2[1]=o[iax2];
  d2[0]=d[iax1]; d2[1]=d[iax2];
  return plane->get_pts_sort_le(iax1,iax2,o2,d2,is,ival,col,pos);
}
pairs picking::get_pts_sort_le(int *iloc,int iax1, int iax2, int isort,int ival, QString col){
 if(col=="None") col=active_col;
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  float o2[2],d2[2];
  o2[0]=o[iax1]; o2[1]=o[iax2];
  d2[0]=d[iax1]; d2[1]=d[iax2];
  return plane->get_pts_sort_le(iax1,iax2,o2,d2,isort,ival,col);
}
pairs picking::get_pts_sort_le(orient_cube *pos,int *iloc,int iax1, int iax2, int isort,
int ival, QString col){
 if(col=="None") col=active_col;
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  float o2[2],d2[2];
  o2[0]=o[iax1]; o2[1]=o[iax2];
  d2[0]=d[iax1]; d2[1]=d[iax2];
  return plane->get_pts_sort_le(iax1,iax2,o2,d2,isort,ival,col,pos);
}
 void picking::clear_picks(orient_cube *pos, int iax1 ,int iax2, QString col){
  if(col=="None") col=active_col;
  int iloc[8];
  pos->get_locs(iloc);
    pick_plane *plane=find_plane(iax1,iax2,iloc);
  std::vector<long long> list=plane->find_picks(iax1,iax2,col,pos);

  delete_pick_list(col,list);
//  clear_picks(iloc,iax1,iax2,col);
 }
  void picking::clear_picks(int *iloc, int iax1 ,int iax2, QString col){
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  std::vector<long long> list=plane->find_picks(iax1,iax2,col);
  delete_pick_list(col,list);
 }
void  picking::shift_pick_window(orient_cube *pos,int *iloc, int iax1, int iax2, int *idelta){
  
    pick_plane *plane=find_plane(iax1,iax2,iloc);
    
      std::vector<pick_plane*> neighs;
  std::vector<int> dists;
  find_neighbors(iax1,iax2,iloc,&neighs,&dists);
  
  int beg[8],end[8];
    for(int i=0; i < 8; i++){ beg[i]=iloc[i]-ndist; end[i]=iloc[i]+ndist;}

    std::vector<long long> hashes=plane->shift_pick_window(active_col,beg,end,extra_state);

    for(int i=0; i < (int)neighs.size(); i++){
    std::vector<long long> junk=neighs[i]->shift_pick_window(active_col,beg,end,extra_state);
    hashes.insert(hashes.end(),junk.begin(),junk.end());
    }
    if((int)hashes.size()>0){
      delete_pick_list(active_col,hashes);
      for(int i=0; i < (int)hashes.size(); i++){ 
        int ipt[8];
        float fpt[8];
        long long v=hashes[i];
        long long dist=0;
        for(int i=7; i >=0 ;i--) {
          ipt[i]=v/blk[i]; 
          v=v-ipt[i]*blk[i];
          dist+=(ipt[i]-iloc[i])*(ipt[i]-iloc[i]);
        }
        float shift=0.;
        if(ndist>0) shift=(float)std::max(0.f,(float)ndist-sqrtf((float)dist))/(float)ndist;
        else if(dist==0) shift=1;
        for(int i=0; i < 8; i++){
          ipt[i]+=(int)(idelta[i]*shift);
        }
        int_to_float(ipt,fpt);
        add_pt(ipt,0,active_col,extra_state);
      }

    }
  }

std::vector<pick*> picking::all_picks(orient_cube *pos, int *iloc, int iax1, int iax2, QString col){
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  std::vector<long long> list=plane->find_picks(iax1,iax2,col,pos);
  return all[col]->all_picks(&list);

}
std::vector<pick*> picking::all_picks(int *iloc, int iax1, int iax2, int isingle, QString col){
  plane_contain *planes=find_plane_contain(iax1,iax2);
  std::vector<long long>  list=planes->find_picks(col,iloc,isingle);
   return all[col]->all_picks(&list);
}
void picking::delete_picks(orient_cube *pos,int ival, QString col){
  if(col=="None") col=active_col;
  int iloc[8];
  pos->get_locs(iloc);
  std::vector<long long> list=all.find(col)->second->find_picks(ival);
  delete_pick_list(col,list);
}
void picking::delete_pick_vals(int ival, QString col){
  if(col=="None") col=active_col;
  std::vector<long long> list=all[col]->find_picks(ival);
  delete_pick_list(col,list);


}
void picking::add_pt(float *floc, int type, QString col, int extra){
 int iloc[8];
 float_to_int(floc,iloc);
 add_pt(iloc,type,col,extra);
}

void picking::add_pt(int *iloc, int type, QString col, int extra){
  bool del;
  if(col=="None") col=active_col;
  if(extra==-99) extra=extra_state;
  picks *pck=all.find(col)->second;
  long long hash=pck->add_pick(iloc,type,extra,&del);
  for(int i=0; i < (int)planes.size(); i++) {
    if(del) planes[i].del_pt(col,iloc,hash);
    planes[i].add_pt(col,iloc,hash);
   }
}
void picking::del_pt( orient_cube *pos,int *iloc, int iax1, int iax2){
  QString col=active_col;
  pick_plane *plane=find_plane(iax1,iax2,iloc);
  std::vector<long long> hashes;
  hashes.push_back(plane->find_closest(pos,col,iloc));
  delete_pick_list(col,hashes);

}
void picking::delete_pick_list(QString col, std::vector<long long> hashes){
   picks *pck=all.find(col)->second;
   int iloc[8];
  for(int i=0; i < (int)hashes.size(); i++){
    long long hash=hashes[i];
    if(hash!=-1){
      pck->delete_pick(hash);
      long long v=hash;
      iloc[7]=v/blk[7]; v=v-blk[7]*iloc[7];
      iloc[6]=v/blk[6]; v=v-blk[6]*iloc[6];
      iloc[5]=v/blk[5]; v=v-blk[5]*iloc[5];
      iloc[4]=v/blk[4]; v=v-blk[4]*iloc[4];
      iloc[3]=v/blk[3]; v=v-blk[3]*iloc[3];
      iloc[2]=v/blk[2]; v=v-blk[2]*iloc[2];
      iloc[1]=v/blk[1]; 
      iloc[0]=v-blk[1]*iloc[1];
      for(int i=0; i < (int)planes.size(); i++) {
        planes[i].del_pt(col,iloc,hash);
       }
     }
   }
}
void picking::float_to_int(const float *floc, int *iloc){
 for(int i=0; i < 8; i++){
  if(d[i] ==0.) iloc[i]=0;
  else
  iloc[i]=MAX(0,MIN(n[i]-1,(int)((floc[i]-o[i])/d[i]+.5)));
  }
}
void picking::int_to_float(const int *iloc, float *floc){
  floc[0]=iloc[0]*d[0]+o[0];
  floc[1]=iloc[1]*d[1]+o[1];
  floc[2]=iloc[2]*d[2]+o[2];
  floc[3]=iloc[3]*d[3]+o[3];
  floc[4]=iloc[4]*d[4]+o[4];
  floc[5]=iloc[5]*d[5]+o[5];
  floc[6]=iloc[6]*d[6]+o[6];
  floc[7]=iloc[7]*d[7]+o[7];

}
void picking::increase_size(){
  if(draw_what=="all"){
     std::map<QString, picks*>::const_iterator i;
     for ( i=all.begin(); i !=all.end();  ++i){
       picks *x=i->second;
        x->increase_size();
     }
  }
  else all.find(active_col)->second->increase_size();
  
 }

 void picking::decrease_size(){
  if(draw_what=="all"){
     std::map<QString, picks*>::const_iterator i;
     for 
     ( i=all.begin(); 
     i !=all.end(); 
     ++i){
       picks *x=i->second;
        x->decrease_size();
     }
  }
  else all.find(active_col)->second->decrease_size();
  
 }
void picking::set_single(int is){
  if(draw_what=="all"){
     std::map<QString, picks*>::const_iterator i;
     for 
     ( i=all.begin(); 
     i !=all.end(); 
     ++i){
       picks *x=i->second;
        x->set_single(is);
     }
  }
  else all.find(active_col)->second->set_single(is);
  
 }
void picking::read_file(const QString &text){
  FILE *fd;
  char line[2048];
  float floc[8];
  int iloc[8];
  int type,extra;
  char  color[1024];
  QString col;
   if ((fd=fopen(text.ascii(),"r")) == NULL) fprintf(stderr,"PROBLEM OPENING \n");
  while (!feof(fd)){
    fgets(line, 1000, fd);

    if (0!=strncmp (line,"#",1)) {

      sscanf(line,"%s %f %f %f %f %f %f %f %f %d %d", color,&floc[0],&floc[1],&floc[2],&floc[3],
        &floc[4],&floc[5],&floc[6],&floc[7],&type,&extra);
      col=color;
      add_pt(iloc,type,col,extra);
    }
  }
  fclose (fd);
}
void picking::write_file(const QString& text){
  FILE *fd;
  float floc[8];
  fd=fopen(text,"w");
  std::map<QString, picks*>::const_iterator i;
  for ( i=all.begin();  i != all.end(); ++i){
    std::map<long long, pick>::const_iterator j;
//    picks pk=i->second;
    for(j=i->second->bucket2.begin(); j !=i->second->bucket2.end();++j){    
      int_to_float(j->second.iloc,floc);
      
      fprintf(fd,"%s %f %f %f %f %f %f %f %f %d %d\n",i->first.ascii(),
       floc[0],floc[1],floc[2],floc[3],floc[4],floc[5],floc[6],floc[7],
        j->second.type,j->second.extra);

    }
  }
  fclose(fd);
}
