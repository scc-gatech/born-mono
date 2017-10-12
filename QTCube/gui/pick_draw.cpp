#include "pick_draw.h"
#include "pairs_new.h"
#include "pick_plane_new.h"
using namespace SEP;
pick_orient::pick_orient(std::shared_ptr<position >pos,int o){
        std::shared_ptr<pick_planes_new> pl(new pick_planes_new());
        std::shared_ptr<pick_bucket> pb(new pick_bucket (pos));
        planes=pl;
        buck=pb;

        oc=o;
    }
pick_draw::pick_draw(){
 std::shared_ptr<my_colors> c(new my_colors());
 std::shared_ptr<my_fonts> f(new my_fonts());
  myc=c;
  draw_what="all";
  extra=0;
  dist=4;
  bulk=0;
  active_col="red";
  active_txt="";
  std::vector<QString> cvec=myc->return_cvec_big();
   myf=f;   //Create font server object
    font_name="Arial-12-Normal";  //Set the default font name
  for(int i=0; i < (int)cvec.size(); i++){
    std::shared_ptr<pick_param> p(new  pick_param(0,DISPLAY_POINTS,2,myc->return_qcolor(cvec[i]),font_name));
    all[cvec[i]]=p;
   }
}
void pick_draw::set_server(std::shared_ptr<orientation_server >s){
  serv=s;
  std::shared_ptr<pick_planes_new> pl(new pick_planes_new());
  planes=pl;
}
void pick_draw::set_position(std::shared_ptr<position >pos){
  std::shared_ptr<pick_bucket> b(new pick_bucket(pos));
  buck=b;
  for(int i=0; i <8; i++){
    axis a=pos->getAxis(i);
    o[i]=a.o;d[i]=a.d;
    n[i]=a.n;
  }
  myp=pos;
}
void pick_draw::increase_size(){
  if(draw_what=="all"){
     std::map<QString, std::shared_ptr<pick_param>>::const_iterator i;
     for ( i=all.begin(); i !=all.end();  ++i){
       std::shared_ptr< pick_param >x=i->second;
        x->increase_size();
     }
  }
  else all.find(active_col)->second->increase_size();
}
void pick_draw::decrease_size(){
  if(draw_what=="all"){
     std::map<QString, std::shared_ptr<pick_param>>::const_iterator i;
     for ( i=all.begin(); i !=all.end();  ++i){
       std::shared_ptr< pick_param >x=i->second;
        x->decrease_size();
     }
  }
  else all.find(active_col)->second->decrease_size();
}
void pick_draw::set_single(int ex){
  if(draw_what=="all"){
     std::map<QString, std::shared_ptr<pick_param>>::const_iterator i;
     for ( i=all.begin(); i !=all.end();  ++i){
        std::shared_ptr< pick_param >x=i->second;
        x->set_single(ex);
     }
  }
  else all.find(active_col)->second->set_single(ex);
}
int pick_draw::get_single(){
  return all.find(active_col)->second->get_single();
}
int pick_draw::get_display(){
  return all.find(active_col)->second->get_display();
}
void pick_draw::set_display(int ex){
  if(draw_what=="all"){
     std::map<QString, std::shared_ptr<pick_param>>::const_iterator i;
     for ( i=all.begin(); i !=all.end();  ++i){
       std::shared_ptr<pick_param>x=i->second;
        x->set_display(ex);
     }
  }
  else all.find(active_col)->second->set_display(ex);
}
void pick_draw::delete_pick_draw(){
   buck.reset();

}
void pick_draw::int_to_float(const int *iloc, float *floc){
  floc[0]=iloc[0]*d[0]+o[0];
  floc[1]=iloc[1]*d[1]+o[1];
  floc[2]=iloc[2]*d[2]+o[2];
  floc[3]=iloc[3]*d[3]+o[3];
  floc[4]=iloc[4]*d[4]+o[4];
  floc[5]=iloc[5]*d[5]+o[5];
  floc[6]=iloc[6]*d[6]+o[6];
  floc[7]=iloc[7]*d[7]+o[7];

}
void pick_draw::float_to_int(const float *floc, int *iloc){
 for(int i=0; i < 8; i++){
  if(d[i] ==0.) iloc[i]=0;
  else
  iloc[i]=std::max(0,std::min(n[i]-1,(int)((floc[i]-o[i])/d[i]+.5)));
  }
}
void pick_draw::read_file(const QString &text){
  FILE *fd;
  char line[2048],txt[99];
  int iloc[8];
  int type,extra;
  float floc[8];
  char  color[1024];
  QString col;
   fprintf(stderr,"what is the text string :%s: \n",text.toAscii().constData());
   if ((fd=fopen(text.toAscii().constData(),"r")) == NULL) fprintf(stderr,"PROBLEM OPENING \n");
   int i=0;
  while (!feof(fd)){
    fgets(line, 1000, fd);
    if (0!=strncmp (line,"#",1)) {

      sscanf(line,"%s %f %f %f %f %f %f %f %f %d %d %s", color,&floc[0],&floc[1],&floc[2],&floc[3],
        &floc[4],&floc[5],&floc[6],&floc[7],&type,&extra,txt);
      col=color;
      float_to_int(floc,iloc);
      i++;
//fprintf(stderr,"chek ad %d %d %d %d \n",i,iloc[0],iloc[1],iloc[2]);
      add_pt(iloc,type,col,extra,active_txt);
    }
  }
  buck->print();
  fclose (fd);
}
void pick_draw::write_file(const QString& text){
  FILE *fd;
  float floc[8];
  fd=fopen(text.toAscii().constData(),"w");
  for(int i=0 ; i< buck->get_nbuckets(); i++){
    std::shared_ptr<picks_new> b(buck->get_bucket(i));
    for(int j=0; j< b->return_size(); j++){
      std::shared_ptr<pick_new> p( b->return_pick(j));
      int_to_float(p->iloc,floc);
      fprintf(fd,"%s %f %f %f %f %f %f %f %f %d %d %s\n",p->col.toAscii().constData(),
       floc[0],floc[1],floc[2],floc[3],floc[4],floc[5],floc[6],floc[7],
        p->type,p->extra,p->txt.toAscii().constData());

    }
  }
  fclose(fd);
}
void pick_draw::add_rotated_pt(int orient_num, int *iloc, int type, QString col, int ex,QString t){
   long long pos=buck->loc_to_index(iloc);
   std::shared_ptr<pick_new>pk( new pick_new(iloc,pos,1,active_col,ex,t));
   std::shared_ptr<pick_new>orig(serv->convert_back_pick(orient_num,pk));
  add_pt(orig->iloc,type,col,ex,active_txt);
}
void pick_draw::add_pt(int *iloc, int type, QString col, int ex,QString txt){
  if(col=="None") col=active_col;
  if(txt=="None") txt=active_txt;
  if(ex==-99) ex=extra;

  long long pos=buck->loc_to_index(iloc);
  std::shared_ptr<pick_new>pk=buck->get_pick(pos,col);
  if(pk){
    for(std::map<int, std::shared_ptr<pick_orient>>::iterator p=orient_planes.begin(); p!=orient_planes.end(); p++){
      std::shared_ptr<pick_new >pk2=serv->convert_pick(p->second->oc,pk);
      std::shared_ptr<pick_new >pk3=p->second->buck->get_pick(pk2->pos,col);
      assert(pk3!=0);
      p->second->planes->del_pick(pk3);
      p->second->buck->del_pick(pk3->pos,col);
    }
    planes->del_pick(pk);
    buck->del_pick(pk->pos,col);
  }
  pk=buck->add_pick(iloc,pos,type,col,extra,active_txt);
      for(std::map<int, std::shared_ptr<pick_orient>>::iterator p=orient_planes.begin(); p!=orient_planes.end(); p++){
        
        std::shared_ptr<pick_new >pk2=serv->convert_pick(p->second->oc,pk);

        std::shared_ptr<pick_new >pk3=p->second->buck->add_pick(pk2->iloc,pk2->pos,pk2->type,pk2->col,pk2->extra,pk2->txt);
        p->second->planes->add_pick(pk3);
      }
  planes->add_pick(pk);



}
void pick_draw::add_multi_picks(std::vector<long long> locs){

  for(int i=0; i < (int)locs.size(); i++){
     int iloc[8];
     buck->index_to_loc(locs[i],iloc);
     add_pt(iloc,1);
  }

}
void pick_draw::delete_multi_picks(std::vector<long long> locs){
  
  QString col=active_col;
  for(int i=0; i <(int) locs.size(); i++){
    std::shared_ptr<pick_new >pk;
    pk=buck->get_pick(locs[i],col);
    if(pk!=0){
       del_pick_orients(pk);
       planes->del_pick(pk);
       buck->del_pick(locs[i],col);
    }
  }
  



}
void pick_draw::del_nearest( std::shared_ptr<orient_cube> pos,int *iloc, int iax1, int iax2){

  QString col=active_col;
  if(pos==0){;}
  if(iax1==0 && iax2==0 ){;}
  long long hsh=buck->loc_to_index(iloc);
  hsh=buck->find_nearest(hsh,iax1,iax2,col);
  
  std::shared_ptr<pick_new>pk=buck->get_pick(hsh,col);
  if(!pk) { 
    return;
}
  del_pick_orients(pk);
  planes->del_pick(pk);
  buck->del_pick(hsh,col);

}
void pick_draw::del_pt( std::shared_ptr<orient_cube>pos,int *iloc, int iax1, int iax2){
  QString col=active_col;
  if(pos==0){;}
  if(iax1==0 && iax2==0 ){;}
  long long hsh=buck->loc_to_index(iloc);
  std::shared_ptr<pick_new>pk=buck->get_pick(hsh,col);
  if(!pk) { 
    return;
}
  del_pick_orients(pk);
  planes->del_pick(pk);
  buck->del_pick(hsh,col);
  }
void pick_draw::del_pick_orients(std::shared_ptr<pick_new>pk){
  for(std::map<int, std::shared_ptr<pick_orient>>::iterator p=orient_planes.begin(); p!=orient_planes.end(); p++){
     std::shared_ptr<pick_new>pk2=serv->convert_pick(p->second->oc,pk);
     std::shared_ptr<pick_new>pk3=p->second->buck->get_pick(pk2->pos,pk2->col);
     assert(pk3!=0);
     p->second->planes->del_pick(pk3);
     p->second->buck->del_pick(pk3->pos,pk3->col);
  }
}
std::shared_ptr<pairs_new>pick_draw::get_pts_sort(std::shared_ptr<orient_cube>pos,int *iloc, int iax1,int iax2,  QString col){
  //Check to see if slice is created
  std::shared_ptr<picks_vec>all=create_load_plane(pos,iax1,iax2,iloc);
  //Convert to pairs
  std::shared_ptr<pairs_new>prs(new pairs_new());
  for(int i=0; i < all->return_size(); i++){
    std::shared_ptr<pick_new>p=all->return_pick(i);
    if(col==p->col){
      prs->add_point(p->iloc[iax1],p->iloc[iax2]);
    }
  }
  //Sort picks
  prs->sort2();
  return prs;
}
void pick_draw::check_create_oc(std::shared_ptr<orient_cube>pos){
  int orient_num=pos->get_orient_num();
  if(orient_planes.count(orient_num)){
    for(std::list<int>::iterator i=orients.begin(); i!=orients.end(); i++){
      if(*i==orient_num) orients.erase(i); orients.push_front(orient_num);  }  
    return;
  }
  if(orient_planes.size()==MAX_ORIENTS){
     int o=orients.back();
     orient_planes.erase(o);

  }
  std::shared_ptr<pick_orient> x(new pick_orient(pos,orient_num));
  orient_planes[orient_num]=x;
  std::shared_ptr<picks_vec>old=buck->return_all_picks("None");
  std::shared_ptr<picks_vec>nw( new picks_vec());
  
  pos->convert_picks(old,nw);
  for(int i=0; i < nw->return_size(); i++){
     std::shared_ptr<pick_new>p=nw->return_pick(i),dumb;
     dumb=orient_planes[orient_num]->buck->add_pick(p->iloc,p->pos,p->type,p->col,p->extra,p->txt);
  }
}
std::shared_ptr<picks_vec>pick_draw::create_load_plane(std::shared_ptr<orient_cube>pos,int iax1,int iax2, int *iloc){
  int oc=pos->get_orient_num();
  if(iloc==0){;}
  std::shared_ptr<picks_vec>all;
  int loc[8];
  pos->get_viewed_locs(iax1,iax2,loc); loc[iax1]=loc[iax2]=-1;
  check_create_oc(pos);
  if(!orient_planes[oc]->planes->map_exists(loc)){
    //Create slice if necessary
    all=orient_planes[oc]->buck->return_pick_plane(loc);
    orient_planes[oc]->planes->add_plane(loc,all );
    //delete [] map;
  }
  else all=orient_planes[oc]->planes->return_picks(loc);
  return all;
}
std::shared_ptr<picks_vec>pick_draw::return_all_picks(std::shared_ptr<orient_cube>pos, QString col){
 int orient_num=-1;
 if(pos!=0) orient_num=pos->get_orient_num();
  if(orient_num==-1) return buck->return_all_picks(col);
  check_create_oc(pos);
  return orient_planes[orient_num]->buck->return_all_picks(col);


}
std::shared_ptr<picks_vec>pick_draw::return_iloc_based(int *iloc,  QString col){


  std::shared_ptr<picks_vec>all=buck->return_all_picks(col);

  std::shared_ptr<picks_vec>sub(new picks_vec());
  for(int i=0; i < all->return_size(); i++){
    std::shared_ptr<pick_new>p=all->return_pick(i);
    if(p->col==col)
      if((iloc[0]!=1 || iloc[0]!=p->iloc[0]))
       if (iloc[1]!=1 || iloc[1]!=p->iloc[1])
        if (iloc[2]!=1  ||  iloc[2]!=p->iloc[2])
          if (iloc[3]!=1 || iloc[3]!=p->iloc[3])
            if(iloc[4]!=1 || iloc[4]!=p->iloc[4])
              if(iloc[5]!=1 || iloc[5]!=p->iloc[5])
                if(iloc[6]!=1 || iloc[6]!=p->iloc[6])
                  if (iloc[7]!=1 || iloc[7]!=p->iloc[7]) {sub->add_pick(p);}
                  


  }

   return sub;
}
void pick_draw::delete_picks(std::shared_ptr<orient_cube>pos,int ival, QString col){
  if(ival==0 && pos==0  && col.contains("nssf")){;}
   delete_pick_vals(ival,col);
}
void pick_draw::update_slice_maps(int *iloc,int *inew,int iax1,int iax2, int iax3,std::shared_ptr<slice>slc){
  if(iax1==0 && iax2==0){;};
  int n3=myp->getAxis(iax3).n;


  int oc=slc->get_orient_num();

  check_create_oc(slc->get_orient());
  std::vector<int> new_planes;
  for(int i=std::max(0,iloc[iax3]-dist); i<=std::min(n3-1,iloc[iax3]+dist); i++){
    inew[iax3]=i;
    if(!orient_planes[oc]->planes->map_exists(inew)){
      new_planes.push_back(i);
     }
  }

  std::map<int,std::shared_ptr<picks_vec>>::iterator iter;
  assert(orient_planes.count(oc)==1);
  std::map<int,std::shared_ptr<picks_vec>> all_picks=orient_planes[oc]->buck->return_picks_from_range(iax3,new_planes,inew);

  for(iter=all_picks.begin(); iter!=all_picks.end(); iter++){
     inew[iax3]=iter->first;
     orient_planes[oc]->planes->add_plane(inew,iter->second);     
  }
}
void pick_draw::draw(QPainter *painter,std::shared_ptr<slice>slc){
  std::vector<pick_new> lst;
  QPen pen;
  int iax1,iax2;
  std::map<long long,int> pick_map;

  std::shared_ptr<orient_cube>pos=slc->get_orient();
    int iloc[8]; 
 iax1=slc->get_iax1();
 iax2=slc->get_iax2();
 int i3=slc->get_i3();
 pos->get_viewed_locs(iax1,iax2,iloc);
   int oc=slc->get_orient_num();
  iloc[i3]+=slc->get_offset();


   int inew[8]; memcpy(inew,iloc,8*sizeof(int));
  inew[iax1]=-1;
  inew[iax2]=-1;
  update_slice_maps(iloc,inew,iax1,iax2,i3,slc);
  int n3=pos->getAxis(i3).n;
  std::vector<QString> colors=myc->return_cvec();
  for(int i=std::max(0,iloc[i3]-dist); i<=std::min(n3-1,iloc[i3]+dist); i++){
    float rat=((float)(dist-abs(i-iloc[i3]))/(float)dist);
    if(dist==0) rat=1.;
    inew[i3]=i;
    std::shared_ptr<picks_vec>pk=orient_planes[oc]->planes->return_picks(inew);
    if(draw_what=="all"){
      for(int j=0; j <(int) colors.size(); j++){
      
        std::shared_ptr<pairs_new>myp=slc->return_pick_locs(colors[j],pk);
        if(myp->size()>0) all[colors[j]]->draw(painter,iax1,iax2,myp,rat);
      }
    }
    else{
      std::shared_ptr<pairs_new >myp=slc->return_pick_locs(draw_what,pk);
      if(myp->size()>0) all[draw_what]->draw(painter,iax1,iax2,myp,rat);
    }
  }
  
}
void pick_param::draw(QPainter *painter, int iax1, int iax2, std::shared_ptr<pairs_new>myp, float rat){
  bool draw_pt=true;
  
  QPen pen;
  pen.setWidth(sz);
  QColor c=col;
  c.setAlphaF(rat);
  pen.setColor(c);
  painter->setBrush(c);
  painter->setPen(pen);
  
  
  if(display_type==DISPLAY_LINE &&  myp->size()>1){
    if(iax1==single){ myp->sort2(); draw_pt=false;}
    if(iax2==single){ myp->sort1(); draw_pt=false;}
  }
   bool txt=false;
    if(display_type==DISPLAY_TEXT) txt=true;
    if(draw_pt) draw_points(painter,myp,txt);
    else draw_line(painter,myp,rat);
}
void pick_param::draw_points(QPainter *painter,std::shared_ptr<pairs_new>points,bool txt ){
  QVector<QRectF> rects;
  QPolygon pointarray;
  std::shared_ptr<QFontMetrics> fm=myf->return_font_metric(font_name);

  for(int i=0; i < (int)points->size(); i++){
    int ix,iy,bx,ex,by,ey;
    iy=points->vals[i].y;
    ix=points->vals[i].x;
    if(txt){

      int pw = fm->width( points->vals[i].txt);
      painter->drawText(ix-pw/2,iy,points->vals[i].txt);
    }
    else{
       bx=ix-sz; by=iy-sz; ex=ix+sz; ey=iy+sz;
       pointarray.putPoints(0, 5,bx,by,bx,ey,ex,ey,ex,by,bx,by);
       painter->drawPolyline(pointarray);	
     }
   }
 painter->drawRects(rects);
}
void pick_param::draw_line(QPainter *painter,std::shared_ptr<pairs_new>points,float rat ){
  QPolygon pointarray,pointarray2;
 for(int i=0; i < (int)points->size(); i++){
    int ix,iy,bx,by,ex,ey;
    ix=points->vals[i].x;
    iy=points->vals[i].y;
    pointarray.putPoints(i,1,ix,iy);
    bx=ix-sz; by=iy-sz; ex=ix+sz; ey=iy+sz;
  	if(rat<.001){
  	  pointarray2.putPoints(0, 5,bx,by,bx,ey,ex,ey,ex,by,bx,by);
      painter->drawPolyline(pointarray2);
 	}
  }
  painter->drawPolyline(pointarray);
}
void pick_draw::delete_pick_vals(int ival, QString col){

   std::shared_ptr<picks_vec>pks=buck->get_pick_type(ival,col);

   for(int i=0; i < pks->return_size(); i++){
     std::shared_ptr<pick_new>pik=pks->return_pick(i);
     del_pick_orients(pik);
     planes->del_pick(pik);
     buck->del_pick(pik->pos,col);
   }
}
void pick_draw::clear_picks(std::shared_ptr<orient_cube>pos, int *iloc,QString col){
  std::shared_ptr<picks_vec>pka=orient_planes[pos->get_orient_num()]->planes->return_picks(iloc);
  std::shared_ptr<picks_vec>pk(new picks_vec());
  for(int i=0; i < pka->return_size(); i++)
    serv->convert_back_pick(pos->get_orient_num(),pka->return_pick(i));
  for(int i=0; i < pk->return_size(); i++){
    std::shared_ptr<pick_new>pik=pk->return_pick(i);
    if(pik->col==col){
      del_pick_orients(pik);
      planes->del_pick(pik);
      buck->del_pick(pik->pos,pik->col);
   }
  }
}

std::shared_ptr<pairs_new>pick_draw::get_pts_sort_le(std::shared_ptr<orient_cube>pos, int iax1, int iax2,int ival, int isort,QString col){
   int iloc[8];
   pos->get_locs(iloc); iloc[iax1]=iloc[iax2]=-1;
   std::shared_ptr<picks_vec>pks=create_load_plane(pos,iax1,iax2,iloc);
   std::shared_ptr<pairs_new>prs( new pairs_new());
   for(int i=0; i < pks->return_size(); i++){
     std::shared_ptr<pick_new>p=pks->return_pick(i);
     if((p->col==col) && p->type<= ival) {
        if(iax1!=isort)    prs->add_point(p->iloc[iax1],p->iloc[iax2]);
        else  prs->add_point(p->iloc[iax2],p->iloc[iax1]);
    }
   }
   //if(iax1==isort) {
    // fprintf(stderr,"in one \n");
   //  prs->sort1();}
  // else {
  // fprintf(stderr,"in two \n");
     prs->sort2();
   
   // }
   return prs;
}
