#include<image_factory.h>
#include <QColor>

QImage *image_factory::makeImage(unsigned char * uc, int nx, int ny)
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
/*Sets the background color for the graph.
*/
void image_factory::set_bcolor(QString col){
fprintf(stderr,"IN BCLOR \n");
  if(col.contains("none")) scale[0]=scale[1];
  else  change_map(0,col);
  bcolor=col;
}
/*Changes the color scale according to the given index and color.
*/
void image_factory::change_map(int i,QString col){
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
void image_factory::set_ecolor(QString col){
    if(col.contains("none"))  scale[255]=scale[254];
    else   change_map(255,col);
   ecolor=col;
}
/*
	Sets the color of the wiggles
*/
void image_factory::set_wiggle_color(QString col){
	if (col.contains("black")){
		wiggleColor = Qt::black;
	}
	if (col.contains("blue")){
		wiggleColor = Qt::blue;
	}
	if (col.contains("gray")){
		wiggleColor = Qt::gray;
	}
	if (col.contains("green")){
		wiggleColor = Qt::green;
	}
	if (col.contains("red")){
		wiggleColor = Qt::red;
	}
	if (col.contains("white")){
		wiggleColor = Qt::white;
	}
	if (col.contains("yellow")){
		wiggleColor = Qt::yellow;
	}
}
/*
	Sets the color for the contour lines.
*/
void image_factory::set_contour_color(QString col){
		if (col.contains("black")){
		wiggleColor = Qt::black;
	}
	if (col.contains("blue")){
		wiggleColor = Qt::blue;
	}
	if (col.contains("gray")){
		wiggleColor = Qt::gray;
	}
	if (col.contains("green")){
		wiggleColor = Qt::green;
	}
	if (col.contains("red")){
		wiggleColor = Qt::red;
	}
	if (col.contains("white")){
		wiggleColor = Qt::white;
	}
	if (col.contains("yellow")){
		wiggleColor = Qt::yellow;
	}
}
