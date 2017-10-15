
#include "pick_groups.h"
//#include "fstream.h"
#include "stdio.h"
//Added by qt3to4:
#include <QPolygon>
#include <q3mimefactory.h>

pick_groups::pick_groups(pick_maps *m){
  mp=m;


  sz=2;
  extra_state=0;
  draw_what="all";
  changed=false;
}

void pick_groups::change_active(QString name){ 
  mp->change_active(name);

}
void pick_groups::draw(QPainter *painter, orient_cube *pos,slice *slc,dataset *dat){
  std::vector<pik> lst;
  QPen pen;
 
  slc->get_slice_axes(pos,&iax1,&iax2,&iax3);
  opa=pos->get_axis(iax3);
  float os[8];
  float ds[8];

  pos->get_os(os);
  pos->get_ds(ds);
  pos->get_ns(ns);
 // float fr;
  //fr=.98/(range*1.);
  

    for(int i=0; i < 8; i++){
    if(iax2==i || iax1==i || !dat->get_data_contain(i)){
      beg[i]=pos->get_beg(i)*ds[i]+os[i];
      end[i]=(pos->get_end(i)-1)*ds[i]+os[i];
    }
  //  else if(iax1==i){
   //   beg[i]=(pos->get_beg(i))*ds[i]+os[i];
    //  end[i]=(pos->get_end(i)-1)*ds[i]+os[i];
   //  }
    else if(ns[i] ==1){
      beg[i]=-10000000;
      end[i]=10000000;
    
    }
    else{
      beg[i]=pos->get_pos(i)-ds[i]/2;
      end[i]=beg[i]+ds[i]/2;

    }
    if(beg[i] > end[i]){
      float s=beg[i]; beg[i]=end[i]; end[i]=s;
    }

  }
  std::map<QString, pikset*>::const_iterator i;
  int doline;
  for ( i=mp->groups.begin(); i != mp->groups.end(); ++i){
  
     if(draw_what=="all" || draw_what==i->first){
    fprintf(stderr,"CHECK IAX %d %d \n",iax1,i->second->get_single());
       if(iax1!=i->second->get_single() && iax2!=i->second->get_single()){
         
          doline=0;
        }
       else {
        
          doline=i->second->get_line();
         
        }
       
       //Make main pen
	     pen.setWidth(sz);
	     QColor c=i->second->get_color();
	     c.setAlphaF(1.);
	     pen.setColor(c); 
	     painter->setBrush(c);

	     painter->setPen(pen);

      if     (doline==0) {
       this->draw_pts(i->second,painter,pos,slc,0,os,ds,dat);
      }
      else if(doline==1) this->draw_line(i->second,painter,pos,slc,0,os,ds,dat);
      //Make sub pen
      
      pen.setColor(c);
        painter->setBrush(c);
      painter->setPen(pen);
      int ndist=i->second->get_dist();
      for(int idist=1; idist < ndist; idist++){
        c.setAlphaF((float)(ndist-idist)/ndist);
        pen.setColor(c);
        painter->setBrush(c);
        painter->setPen(pen);
        if(doline==0){
          this->draw_pts(i->second,painter,pos,slc,idist,os,ds,dat);
          this->draw_pts(i->second,painter,pos,slc,-idist,os,ds,dat);
        }
        else{
          this->draw_line(i->second,painter,pos,slc,idist,os,ds,dat);
          this->draw_line(i->second,
            painter,pos,slc,-idist,os,ds,dat);
        }
      }
    }
  }
}
void pick_groups::draw_pts(pikset *piks,QPainter *painter,orient_cube *pos, slice *slc,
 int idist, float *os, float *ds,dataset *dat){


   int ib,ie;
       QVector<QRectF> rects;
    for(int idir=0; idir < 8; idir++){
    float bold,eold;
      if(idir!=iax1 && idir!=iax2 && dat->get_data_contain(idir) &&( ns[idir]>1 || idist!=0)) {
      opa=pos->get_axis(idir);

    
 ie=pos->get_loc(idir)+idist;
 if(ie <0) ie=0;
if (ie> opa.n-1) ie=opa.n-1;
ib=ie;
 if(os==0);


  if(ib >= 0 && ie <= opa.n-1){

   bold=beg[idir]; eold=end[idir];
  beg[idir]=bold+idist*ds[idir]-ds[idir]/2.;
  end[idir]=eold+idist*ds[idir]+ds[idir]/2.;

  std::vector<pik> points=piks->get_pts(beg,end);
  int ix,iy,bx,by,ex,ey;
  QPolygon pointarray;
 if((int)points.size()>0){

   //QVector<

fprintf(stderr,"in draw pts \n");

  for(int i=0; i < (int)points.size(); i++){
    slc->get_shifted_image_pos_from_loc(points[i].floc[iax2],
      points[i].floc[iax1],&ix,&iy);
   if(points[i].type!=1 && points[i].extra==0){
      bx=ix-sz; by=iy-sz; ex=ix+sz; ey=iy+sz;
  	  pointarray.putPoints(0, 5,bx,by,bx,ey,ex,ey,ex,by,bx,by);
		  painter->drawPolyline(pointarray);
		}
		else if(points[i].extra!=0){
		// painter->drawEllipse(ix,iy,sz*3,sz*3);
		  rects.push_back(QRectF(ix,iy,sz*4,sz*4));
		}
	  else {
	   // painter->drawRect(ix,iy,sz*2,sz*2);
	    rects.push_back(QRectF(ix,iy,sz*2,sz*2));
	   }
	}
   
  }
   beg[idir]=bold; end[idir]=eold;
  }
  
  }
//  if(idist==0) break;
 }
 painter->drawRects(rects);
}
void pick_groups::draw_line(pikset *piks,QPainter *painter,orient_cube *pos, slice *slc, 
 int idist, float *os,float *ds, dataset *dat){
  
    int ib,ie;
    
    

    for(int idir=0; idir < 8; idir++){
    float bold,eold;
     if(idir!=iax1 && idir!=iax2 && dat->get_data_contain(idir)  &&( ns[idir]>1 || idist!=0)) {
     //if(1==1){
      opa=pos->get_axis(idir);
    
 ie=pos->get_loc(idir)+idist;
 if(ie <0) ie=0;
if (ie> opa.n-1) ie=opa.n-1;
ib=ie;
 if(os==0);

  
//  if(ib >= 0 && ie <= opa.n-1){
if(1==1){
  bold=beg[idir]; eold=end[idir];
  beg[idir]=bold+idist*ds[idir]-ds[idir]/2.;
  end[idir]=eold+idist*ds[idir]+ds[idir]/2.;

    

 
     
  
  int ising;
  if(iax2==piks->get_single()) ising=iax1;
  else ising=iax2;
  
  
  std::vector<pik> points=piks->get_pts_sort(beg,end,ising,ds);
  int ix,iy,bx,by,ex,ey;
  QPolygon pointarray,pointarray2;

if((int)points.size()>0){


  for(int i=0; i < (int)points.size(); i++){
 
    slc->get_shifted_image_pos_from_loc(points[i].floc[iax2],
      points[i].floc[iax1],&ix,&iy);
    pointarray.putPoints(i,1,ix,iy);

  
    bx=ix-sz; by=iy-sz; ex=ix+sz; ey=iy+sz;
    if(points[i].type!=1){
  	  pointarray2.putPoints(0, 5,bx,by,bx,ey,ex,ey,ex,by,bx,by);
		  if(idist==0) painter->drawPolyline(pointarray2);
		}
		else  if(idist==0)
	 painter->drawRect(ix,iy,sz*2,sz*2);
  }
 	painter->drawPolyline(pointarray);
  
 }
  beg[idir]=bold;
  end[idir]=eold; 	
 }
 
 }
 }


}
void pick_groups::write_file(const QString& text){
  FILE *fd;
  fd=fopen(text,"w");
  std::map<QString, pikset*>::const_iterator i;
  for ( i=mp->groups.begin(); i != mp->groups.end(); ++i){
    for(int j=0; j < (int)i->second->pts.size(); j++){
      fprintf(fd,"%s %f %f %f %f %f %f %f %f %d %d\n",i->first.ascii(),
       i->second->pts[j].floc[0],
       i->second->pts[j].floc[1],i->second->pts[j].floc[2],
         i->second->pts[j].floc[3], i->second->pts[j].floc[4],i->second->pts[j].floc[5], 
         i->second->pts[j].floc[6],i->second->pts[j].floc[7],i->second->pts[j].type,
         i->second->pts[j].extra);
    }
  }
  fclose(fd);
}
void pick_groups::read_file(const QString &text){

  FILE *fd;
  char line[2048];
  float iloc[8];
  int type,extra;
  char  color[1024];
  QString col;
   if ((fd=fopen(text.ascii(),"r")) == NULL) fprintf(stderr,"PROBLEM OPENING \n");
  while (!feof(fd)){
    fgets(line, 1000, fd);

    if (0!=strncmp (line,"#",1)) {

      sscanf(line,"%s %f %f %f %f %f %f %f %f %d %d", color,&iloc[0],&iloc[1],&iloc[2],&iloc[3],
        &iloc[4],&iloc[5],&iloc[6],&iloc[7],&type,&extra);
      col=color;

      mp->groups[col]->add_pt(iloc,type,extra);
    }
  }
  fclose (fd);
}


 void pick_groups::change_single(int iax){
        std::map<QString, pikset*>::const_iterator i;
  for ( i=mp->groups.begin(); i != mp->groups.end(); ++i){
     if(draw_what=="all" || draw_what==i->first){
       i->second->change_single(iax);
     }
    }
    }



 void pick_groups::set_range(int irange){
    std::map<QString, pikset*>::const_iterator i;
    for ( i=mp->groups.begin(); i != mp->groups.end(); ++i){
     if(draw_what=="all" || draw_what==i->first){
     
            i->second->set_dist(irange);

     
     }
    }
  }  
  void pick_groups::draw_line(){
    std::map<QString, pikset*>::const_iterator i;
    for ( i=mp->groups.begin(); i != mp->groups.end(); ++i){
     if(draw_what=="all" || draw_what==i->first){
       i->second->set_line(1);
     }
    }
  }
 void pick_groups::draw_pts(){
    std::map<QString, pikset*>::const_iterator i;
    for ( i=mp->groups.begin(); i != mp->groups.end(); ++i){
     if(draw_what=="all" || draw_what==i->first){
        int dist= i->second->get_dist()-1; if(dist <0) dist=0;
         i->second->set_line(0);
     }
    }
  }
    

 
