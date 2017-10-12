#include<raster.h>
#include <tbb/tbb.h>
#include <tbb/blocked_range.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_for.h>
using namespace SEP;
QImage *raster::makeImage(unsigned char * uc,const  int nx,const  int ny, const int xsize, const int ysize)
{
  int nxuse=nx,nyuse=ny;
  bool resized=false;
  int xsu,ysu;
  unsigned char *ucuse=uc;
  
  int jx=nx/xsize;
  int jy=ny/ysize;
  if(jx >1 || jy>1){
    nxuse=nx/std::max(jx,1);
    nyuse=ny/std::max(jy,1);
   
    resized=true;
    ucuse=new unsigned char[nxuse*nyuse];
    unsigned char *tmp=new unsigned char[nyuse*nx];   

    if(jy>1){
     tbb::parallel_for(tbb::blocked_range<int>(0,nx),[&](
  const tbb::blocked_range<int>&r){
   int *space=new int[std::max(nx,ny)*2];
  for(int i2=r.begin(); i2!=r.end(); ++i2)
        
         resampleBand(uc,i2*ny,1,ny,tmp,i2*nyuse,1,nyuse,jy,space);
       delete []space;  
       });
       
    }
    else memcpy(tmp,uc,nyuse*nx);
    if(jx>1){
         tbb::parallel_for(tbb::blocked_range<int>(0,nyuse),[&](
  const tbb::blocked_range<int>&r){
       int *space=new int[std::max(nx,ny)*2];

 for(int  i1=r.begin(); i1!=r.end(); ++i1)
         resampleBand(tmp,i1,nyuse,nx,ucuse,i1,nyuse,nxuse,jx,space);
      delete [] space;
      });
    }
    else memcpy(ucuse,tmp,nxuse*nyuse);

  }
	
	
	
	
	QImage *image = new QImage(nxuse, nyuse, QImage::Format_Indexed8);



	
	int iy,ix;
	
	int diff=nxuse%4;
	if(diff!=0) diff=4-diff;
	unsigned char *uc_tmp = new unsigned char[diff+nxuse];

	for(iy=0; iy < nyuse; iy++) {
	  for(ix=0; ix < nxuse; ix++){
	  uc_tmp[ix] = ucuse[ix*nyuse+iy]; 
	 }
	 memcpy((void*)image->scanLine(iy),(const void*)uc_tmp,nxuse);
	}
	delete []uc_tmp;
    if(resized) delete [] ucuse;

	image->setColorTable(scale);
	
	return image;
}
void raster::resampleBand(const unsigned char *in, const int fin,const  int jin, const int nin,
unsigned char *out, const int fout, const int jout, const int nout, const int resamp, int *space) const{
  int jw=1+2*resamp;
  int *sp1=space;
  int *sp2=&space[nin];
  int jw2=jw*jw;
  sp1[0]=jw*in[fin];
  for(int i=1; i < jw; i++) sp1[i]=(int)in[fin+jin*i]-(int)in[fin]+sp1[i-1];
  for(int i=jw; i < nin; i++) {
     sp1[i]=(int)in[fin+jin*i]-(int)in[fin+jin*(i-jw)]+sp1[i-1];
   }
  sp2[nin-1]=sp1[nin-1]*jw;
  for(int i=1; i < jw; i++)  sp2[nin-1-i]=sp2[nin-i]+sp1[nin-i-1]-sp1[nin-1];
  for(int i=jw; i < nin; i++){
     sp2[nin-1-i]=sp2[nin-i]+sp1[nin-i-1]-sp1[nin-i-1+jw];
      }
  for(int i=0; i < nout; i++){
    out[fout+jout*i]=  (unsigned char) (sp2[i*resamp]/jw2);
 }
  // if(fin==0) assert(1==2);

}
void raster::set_bcolor(QString col){
  if(col.contains("none")) scale[0]=scale[1];
  else  change_map(0,col);
  bcolor=col;
}
void raster::change_map(int i,QString col){
  if(col.contains("red")){
      scale[i]=qRgba(255,0,0,255);

  }
  if(col.contains("blue")){
        scale[i]=qRgba(0,0,255,255);

  }
  if(col.contains("green")){
        scale[i]=qRgba(0,255,0,255);

  }
  if(col.contains("black")){
        scale[i]=qRgba(0,0,0,255);

  }
   if(col.contains("white")){
        scale[i]=qRgba(255,255,255,255);

  }
  if(col.contains("tran")){
    if(i==0){
          scale[i]=qRgba(0,0,0,0);

    }
    else if(i==255){
          scale[i]=qRgba(0,0,0,0);

    }
    else{
      scale[i]=qRgba(0,0,0,0);
    } 
  }

}
void raster::set_ecolor(QString col){
    if(col.contains("none"))  scale[255]=scale[254];
    else   change_map(255,col);
   ecolor=col;
}

void raster::draw_slice(QPainter *painter, std::shared_ptr<dataset>dat,QPen *pen, 
 std::shared_ptr<orient_cube>pos,bool ov,bool draw_grid){

   if(ov);
  my_dat=dat;
  //axes[0]=dat->get_grid_axis(iax1+1);
  //axes[1]=dat->get_grid_axis(iax2+1);
  axes[0]=pos->get_rot_axis(iax1);
  axes[1]=pos->get_rot_axis(iax2);
  int e1,e2,f1,f2,n1,n2;
  pos->getAxis_range(iax1,&f1,&e1); n1=e1-f1;
  pos->getAxis_range(iax2,&f2,&e2); n2=e2-f2;

  
  ox=axes[1].o;
  oy=axes[0].o;
  dx=axes[1].d;
  dy=axes[0].d;




   int bb1,bb2,ee1,ee2;
   if((rev_1 && !pos->get_reverse(iax1)) || (!rev_1 && pos->get_reverse(iax1))){
     ee1=f1; bb1=f1+n1; 
  }
   else { bb1=f1; ee1=f1+n1;}
   if((rev_2 && !pos->get_reverse(iax2)) || (!rev_2 && pos->get_reverse(iax2))){
      ee2=f2; bb2=f2+n2; 

   }
   else { bb2=f2; ee2=f2+n2;}

  // unsigned char *buf=     
   //  dat->get_char_data(pos,iax1,iax2,abs((ee1-bb1)*(ee2-bb2)),slice_to_grid_map);


   unsigned char *buf=     dat->get_char_data(pos,iax1,bb1,ee1,iax2,bb2,ee2);



  b_1=f1; 
  b_2=f2;
  e_1=b_1+n1;
  e_2=b_2+n2;
  
  int it;
//  if(rev_1){ it=e_1;e_1=b_1; b_1=it;}
 // if(rev_2){ it=e_2;e_2=b_2; b_2=it;}

   QImage *temp=makeImage(buf,n2,n1,xe-xb,ye-yb);

   
//   temp->setAlphaBuffer(true);


   QImage newt=temp->scaled(xe-xb,ye-yb);
  QPixmap t2=QPixmap::fromImage(newt, Qt::OrderedAlphaDither);

 //QPixmap m_pixmap ;
 //m_pixmap.convertFromImage(newt);

   painter->save();

   painter->translate(bx,by);
   painter->shear(sy,sx);  


  //painter->drawImage(xb,yb,temp->scaled(xe-xb,ye-yb));
  //QImage newt=temp->scale(xe-xb,ye-yb);
// QPixmap m_pixmap = QPixmap::fromImage(newt);
    
  
  painter->drawPixmap(xb,yb,t2);

   
   painter->setPen(*pen);
   painter->restore();
  // painter->translate(bx,by);


   painter->setPen(*pen);
   if(!is_overlay){
    this->draw_axes(painter,pos,draw_grid);
    this->draw_pos(painter,pos);
  }
  


   delete temp; delete [] buf;
}
std::shared_ptr<slice> raster::clone_alpha(int alpha){
  std::shared_ptr<raster>x ( new raster());
  for(int i=0; i < 256; i++) x->scale.push_back(qRgba(0,0,0,0));

  for(int i=0; i<scale.size(); i++) {
    QRgb z=scale[i];
    x->scale[i]=qRgba(qRed(z),qGreen(z),qBlue(z),alpha);
    
   }
  x->name=name;
  x->ecolor=ecolor;
  x->bcolor=bcolor;
  x->is_overlay=is_overlay;
  is_raster=true;
  return x;
}
std::shared_ptr<slice> raster::clone(){
  std::shared_ptr<raster>x ( new raster());
  for(int i=0; i < 256; i++) x->scale.push_back(qRgba(0,0,0,0));

  for(int i=0; i<scale.size(); i++) {
    QRgb z=scale[i];
    x->scale[i]=z;
   }
  x->name=name;
  x->ecolor=ecolor;
  x->bcolor=bcolor;
  x->is_overlay=is_overlay;
  is_raster=true;
  return x;
}
