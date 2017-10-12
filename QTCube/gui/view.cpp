 #include<view.h>
//Added by qt3to4:
#include <QKeyEvent>
#include "raster.h"
using namespace SEP;
void view::set_size(int b_x,int e_x, int b_y, int e_y){
  bx=b_x; ex=e_x;by=b_y;ey=e_y;

}
int view::get_bx(){return bx;}
int view::get_ex(){return ex;}
int view::get_by(){return by;}
int view::get_ey(){return ey;}
void view::convert_to_local_loc(float x,float y, int *ix, int *iy){
   *ix=((int)(x*(ex-bx)))+bx;
   *iy=((int)(y*(ey-by)))+by;

}
std::vector<QString> view::key_release_view( QKeyEvent *e,std::shared_ptr<orient_cube>pos ){
  if(pos==0){;}
  if(e==0){;}
  com[1]="none";
  switch( e->key() )
  {
  
    default:
      return com;  
  }
}
std::vector<QString> view::key_press_view( QKeyEvent *e,std::shared_ptr<orient_cube>pos ){
if (e==0 || pos==0){;}
 com[1]="none";return com;

}
void view::delete_slices(){
  slices.clear();
}
void view::set_title_pos(QPainter *painter, std::shared_ptr<dataset>dat){
  float px,py;
  if(painter==0){;}
   get_pcts(&py,&px);
     int pw = fm->width( dat->get_name() );
      int ph = fm->height();
      int lx=(int)((1.-px)*(ex-bx));
      int ly=(int)(py*(ey-by));
      if(ph < ly-ph/2 && pw <lx-pw/10 && trspace) {
      cx_t=ex-lx/2;
         cy_t=ly/2+ph+by;
         title_bot=false;

     }else{
      cx_t=(bx+ex)/2;
        cy_t=ey;
        title_bot=true;
     }
    


}
void view::draw_bar( int xshift,QPainter *painter,QPen *pen, std::shared_ptr<raster>fact,  unsigned char *conv, bool draw_title){

  QString aa,title=cur_dat->get_values();
  int pw = fm->width( title );
    int ph=  fm->height();

   int yshift=0;
   if(draw_title && title_bot) yshift+=(int)(1.5*ph);

  painter->setPen(*pen);
  //Make the bar
  unsigned char *uc=new unsigned char[256];
  int ii=0;

    for(int i=0; i < 256; i++,ii++){uc[ii]=(unsigned int) conv[i];
    
   }
  int bbar=bx+xshift+ph*3,ebar=ex-ph*3,rbar=ebar-bbar;
  QImage *img=fact->makeImage(uc,256,1,rbar,ph*2);
  QImage newt=img->scaled(rbar,ph*2);
  QPixmap t2=QPixmap::fromImage(newt, Qt::OrderedAlphaDither);
  painter->drawPixmap(bbar,ey-5*ph-yshift,t2);
  delete img;
  //Make the histogram

  float *histo=cur_dat->buf[0]->return_histo();

   QPolygon histarray;
   int ix,iy;
   for(int i=0; i < 256; i++){
       ix=(int)((float)i/256.*rbar+bbar);
       iy=(int)(ey-3*ph-histo[i]*2.*ph-yshift);
      histarray.putPoints(i,1,ix,iy);
   }

   	painter->drawPolyline(histarray);

  //Make the axis
  float minv=cur_dat->get_minval(),maxv=cur_dat->get_maxval();
  std::vector<float> tics=slices[0]->optimal_with_font(rbar,minv,maxv);

  float pct;
  QString ab;
  int tic_len=fm->height()/3;
  int xb;

  for(int i=0; i< (int)tics.size(); i++){
    pct=((float)(tics[i]-minv))/((float)(maxv-minv));
    xb=(int)(bbar+pct*rbar);
    painter->drawLine(xb,ey-ph*3-yshift,xb,ey-ph*3-yshift+tic_len);
    aa.sprintf("%1.5g", tics[i]);
    pw = fm->width( aa );
    painter->drawText((int)(xb-pw/2),(int)(ey-ph*1.5)-yshift,aa);
  }

    QPolygon pointarray;
    pointarray.putPoints(0,5,bbar,ey-5*ph-yshift,bbar,ey-3*ph-yshift,
      ebar,ey-3*ph-yshift,ebar,ey-5*ph-yshift,bbar,ey-5*ph-yshift);
		painter->drawPolyline(pointarray);

  //Make the title

  painter->drawText((bx+ex-pw)/2,ey-ph/2-yshift,title);

  delete [] histo;
}
void view::draw_title(int xshift,QPainter *painter,QPen *pen,QString title){
   if(xshift==0){;}
   int pw = fm->width( title );
      int ph = fm->height();
        painter->setPen(*pen);
      painter->drawText(cx_t-pw/2,cy_t-ph/2,title);
      
      
 }
std::vector<QString> view::do_noth(float x, float y, std::shared_ptr<orient_cube>pos){
 if(x==0. && y==0. && pos==0) {;}
 com[1]="none";
 return com;
}
std::vector<QString> view::r_mouse_d_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;
 this->convert_to_local_loc(x,y,&ix,&iy);
 bool found=false;
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->r_mouse_d(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
     }
 }
  if(!found) mode->r_mouse_d(slices,-1,ix,iy,pos,&com,draw_o);
  if(com[1].contains("pass")>0) return r_mouse_d_e(ix,iy,pos,mode,draw_o);
  return com;
}
std::vector<QString> view::r_mouse_m_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;

 com[1]="pass";
 bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->r_mouse_m(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
     }
 }
 if(!found) mode->r_mouse_m(slices,-1,ix,iy,pos,&com,draw_o);
 if(com[1].contains("pass")>0) return r_mouse_m_e(ix,iy,pos,mode,draw_o);
   return com;
}
std::vector<QString> view::r_mouse_r_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;
 bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->r_mouse_r(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
     }
 }
 if(!found) mode->r_mouse_d(slices,-1,ix,iy,pos,&com,draw_o);
  if(com[1].contains("pass")>0) return r_mouse_r_e(ix,iy,pos,mode,draw_o);
  return com;
}

std::vector<QString> view::m_mouse_d_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;
 bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->m_mouse_d(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
    }
 }
 if(!found){
   mode->m_mouse_d(slices,-1,ix,iy,pos,&com,draw_o);
  }
  if(com[1].contains("pass")>0) return m_mouse_d_e(ix,iy,pos,mode,draw_o);
    return com;
}
std::vector<QString> view::m_mouse_m_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;
bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->m_mouse_m(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
    }
 }
 if(!found) {
   mode->m_mouse_m(slices,-1,ix,iy,pos,&com,draw_o);

  }
  if(com[1].contains("pass")>0) return m_mouse_m_e(ix,iy,pos,mode,draw_o);
    return com;
}
std::vector<QString> view::m_mouse_r_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;

 this->convert_to_local_loc(x,y,&ix,&iy);
 bool found=false;
 for(int islice=0; islice <  (int)slices.size(); islice++){

    if(slices[islice]->in_slice(ix,iy)==true){
      mode->m_mouse_r(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
    }
 }
 if(!found) mode->m_mouse_r(slices,-1,ix,iy,pos,&com,draw_o);
  if(com[1].contains("pass")>0) return m_mouse_r_e(ix,iy,pos,mode,draw_o);
    return com;

}
std::vector<QString> view::l_mouse_d_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;
 bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->l_mouse_d(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
    }
 }
 if(!found) mode->l_mouse_d(slices,-1,ix,iy,pos,&com,draw_o);
  if(com[1].contains("pass")>0) return l_mouse_d_e(ix,iy,pos,mode,draw_o);
    return com;
}
std::vector<QString> view::l_mouse_m_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;


bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->l_mouse_m(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
      
     }
     
 }
 if(!found) mode->l_mouse_m(slices,-1,ix,iy,pos,&com,draw_o);
  if(com[1].contains("pass")>0) return l_mouse_m_e(ix,iy,pos,mode,draw_o);
    return com;
}
std::vector<QString> view::l_mouse_r_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;
 bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->l_mouse_r(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
    }
 }
 if(!found) mode->l_mouse_r(slices,-1,ix,iy,pos,&com,draw_o);
 if(com[1].contains("pass")>0) return l_mouse_r_e(ix,iy,pos,mode,draw_o);
   return com;

}
std::vector<QString> view::l_mouse_double_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;
 bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->l_mouse_double(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
    }
 }
 if(!found) mode->l_mouse_double(slices,-1,ix,iy,pos,&com,draw_o);
  if(com[1].contains("pass")>0) return l_mouse_double_e(ix,iy,pos,mode,draw_o);
    return com;
}
std::vector<QString> view::m_mouse_double_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;
 bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->m_mouse_double(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
    }
 }
 if(!found) mode->m_mouse_double(slices,-1,ix,iy,pos,&com,draw_o);
  if(com[1].contains("pass")>0) return m_mouse_double_e(ix,iy,pos,mode,draw_o);
    return com;
}
std::vector<QString> view::r_mouse_double_view(float x, float y, std::shared_ptr<orient_cube>pos, std::shared_ptr<mouse_func>mode){
 int ix,iy;

 bool found=false;
 this->convert_to_local_loc(x,y,&ix,&iy);
 for(int islice=0; islice <  (int)slices.size(); islice++){
    if(slices[islice]->in_slice(ix,iy)==true){
      mode->r_mouse_double(slices,islice,ix,iy,pos,&com,draw_o);
      found=true;
    }
 }
 if(!found) mode->r_mouse_double(slices,-1,ix,iy,pos,&com,draw_o);
  if(com[1].contains("pass")>0) return r_mouse_double_e(ix,iy,pos,mode,draw_o);
    return com;
}
void view::render_it(QPainter *painter,QPen *pen, std::shared_ptr<slice>fact,  std::shared_ptr<dataset>dat, std::shared_ptr<orient_cube>pos, bool overlay, std::shared_ptr<draw_what>draws)
{

     for(int i=0; i < (int)slices.size(); i++){
       slices[i]->draw_slice(painter,dat,pen,pos,overlay,draws->draw_grid);
       //if(draws->draw_picks && cur_dat->pickable ) this->pg->draw(painter, slices[i],
        // dat);
       if(draws->draw_picks && cur_dat->pickable && !overlay ) this->pk->draw(painter, slices[i]);
    }
     if(draws->draw_pos && !overlay){
     
	     
	     pen->setColor(Qt::darkGreen); 
	     painter->setPen(*pen);
	      draw_o->draw(painter);

	      draw_o->reset();

   }

      if(draws->draw_bar && !overlay) draw_bar(4*bound_l,painter,pen,std::static_pointer_cast<raster>(fact),dat->conv,draws->draw_title);
      if(draws->draw_title&& !overlay) draw_title(0,painter,pen,dat->get_name());




}

