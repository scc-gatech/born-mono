#include<raster.h>

QImage *raster::makeImage(unsigned char * uc, int nx, int ny)
{
	//assert(nx%4 == 0);
	

	QImage *image = new QImage(nx, ny, 8, 256, QImage::IgnoreEndian);
   // QRgb *tb=image->colorTable();
    //int n=tb->size();
      //    image->setAlphaBuffer(true);

    //if (nx/4*4 != nx) return NULL;
	
	int iy,ix;
	
	//for(iy=0; iy < ny/2; iy++) for(ix=0; ix < nx; ix++) uc_tmp[iy*nx+ix] = uc[ix*ny+iy]; 
	int diff=nx%4;
	if(diff!=0) diff=4-diff;
	unsigned char *uc_tmp = new unsigned char[diff+nx];

	for(iy=0; iy < ny; iy++) {
	  for(ix=0; ix < nx; ix++){
	  uc_tmp[ix] = uc[ix*ny+iy]; 
	 }
	 memcpy((void*)image->scanLine(iy),(const void*)uc_tmp,nx);
	}
	//memcpy(image->bits(), uc_tmp, nx*ny);
	delete []uc_tmp;
	
	
	//memcpy(image->bits(), uc, nx*ny);
	
	// set colors
//	QVector<uint>  cp = image->colorTable();
//	for(int i=0; i < 256; i++){
//	cp[i]=this->scale[i];

//	}

	image->setColorTable(scale);

	//memcpy(cp, this->scale, 256*4);
	
	return image;
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

void raster::draw_slice(QPainter *painter, dataset *dat,QPen *pen, 
 orient_cube *pos,bool ov,bool draw_grid){

   if(ov){}
  my_dat=dat;
  //axes[0]=dat->get_grid_axis(iax1+1);
  //axes[1]=dat->get_grid_axis(iax2+1);
  axes[0]=pos->get_rot_axis(iax1);
  axes[1]=pos->get_rot_axis(iax2);
  int e1,e2,f1,f2,n1,n2;
  pos->get_axis_range(iax1,&f1,&e1); n1=e1-f1;
  pos->get_axis_range(iax2,&f2,&e2); n2=e2-f2;

  
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

   unsigned char *buf=     dat->get_char_data(pos,iax1,bb1,ee1,iax2,bb2,ee2);



  b_1=f1; 
  b_2=f2;
  e_1=b_1+n1;
  e_2=b_2+n2;
//  int it;
//  if(rev_1){ it=e_1;e_1=b_1; b_1=it;}
 // if(rev_2){ it=e_2;e_2=b_2; b_2=it;}
   
   QImage *temp=makeImage(buf,n2,n1);

   
   temp->setAlphaBuffer(true);


   QImage newt=temp->scaled(xe-xb,ye-yb);
  QPixmap t2;
  t2.convertFromImage(newt, Qt::OrderedAlphaDither);

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
slice *raster::clone_alpha(int alpha){
  raster *x = new raster();
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
slice *raster::clone(){
  raster *x = new raster();
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
