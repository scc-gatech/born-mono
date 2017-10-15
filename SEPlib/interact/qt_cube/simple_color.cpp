#include<simple_color.h>
#include "my_colors.h"

void simple_color::fill_it(int *red, int *green, int *blue,int alpha)
{
  int r,g,b;
  float f;

  for(int i=0; i < 256; i++) scale.push_back(qRgba(0,0,0,0));
  for(int i=0; i < 127 ;i++){
    f=(float)i/127.;
    r=((int)(red[0]*(1.-f)+f*red[1]));
    g=((int)(green[0]*(1.-f)+f*green[1]));
    b=((int)(blue[0]*(1.-f)+f*blue[1]));
    scale[i+1]=qRgba(r,g,b,alpha);
  }
  for(int i=0; i< 127; i++){
    f=(float)i/127.;
    r=((int)(red[2]*(1.-f)+f*red[3]));
    g=((int)(green[2]*(1.-f)+f*green[3]));
    b=((int)(blue[2]*(1.-f)+f*blue[3]));
    scale[i+128]=qRgba(r,g,b,alpha);
 //   fprintf(stderr,"GRAY SCALE %d %d %d %d \n",i,r,g,b);
  }
   for(int i=0; i< 1; i++){
   scale[i]=scale[255-i]=qRgba(255,0,0,alpha);
 }
 // memcpy((void*)scale,(const void*)cp,1024);   
 // for(int i=0; i < 256; i++) cp[i]=this->scale[i];

  //delete image;
  bcolor="none";
  ecolor="none";
  is_raster=true;
}

float simple_color::fill_color (int *cola2,int x, int ncol)
{
  int             ncolo2;
  ncolo2 = (int)(ncol / 2.);
  is_raster=true;
 
   if(x < ncolo2){
     float f=(float)x/(float)ncolo2;
     return ((1.-f)*cola2[0]+f*cola2[1])/((float)(ncol));
   }
   else{
      float f=(float)(x-ncolo2)/(float)(ncol-ncolo2);
     return ((1.-f)*cola2[1]+f*cola2[2])/((float)(ncol));
   
   }
 }
user_color::user_color(int alpha){
  name="user";
  int red[4],green[4],blue[4];
  red[0]=green[0]=blue[0]=0;
  red[1]=green[1]=blue[1]=127;
  red[2]=green[2]=blue[2]=128;
  red[3]=green[3]=blue[3]=255;
  this->fill_it(red,green,blue,alpha);
}
void user_color::set_red(int *red){
 for(int i=0; i < (int)scale.size(); i++){
   QRgb z=scale[i];
   scale[i]=qRgba(red[i],qGreen(z),qBlue(z),qAlpha(z));
 }

}
void user_color::set_green(int *green){
 for(int i=0; i < (int)scale.size(); i++){
   QRgb z=scale[i];
   scale[i]=qRgba(qRed(z),green[i],qBlue(z),qAlpha(z));
 }

}
void user_color::set_blue(int *blue){
 for(int i=0; i < (int)scale.size(); i++){
   QRgb z=scale[i];
   scale[i]=qRgba(qRed(z),qGreen(z),blue[i],qAlpha(z));
 }


}
jet::jet(int alpha)
{
  int ir,ig,ib;
   for(int i=0; i < 256; i++) scale.push_back(qRgba(0,0,0,0));
     is_raster=true;


    bcolor="none";
    ecolor="none";
    name="jet";
    
    for(int i=0; i< 32; i++){
      ir=0; ig=0; ib=127+i*4;
      scale[i]=qRgba(ir,ig,ib,alpha);
    }
    for(int i=32; i < 96; i++){
       ir=0; ig=(i-32)*4; ib=255;
       scale[i]=qRgba(ir,ig,ib,alpha);
    }
    for(int i=96; i < 160; i++){
       int id=i-96;
       ir=id*4; ig=255; ib=255-id*4;
       scale[i]=qRgba(ir,ig,ib,alpha);
    }
    for(int i=160; i < 224; i++){
       int id=i-160;
       ir=255; ib=0; ig=255-id*4;
       scale[i]=qRgba(ir,ig,ib,alpha);
    }
    for(int i=224; i < 256; i++){
       int id=i-224; 
       ir=127+(32-id)*4; ig=0; ib=0;
       scale[i]=qRgba(ir,ig,ib,alpha);
    }
  

  is_raster=true;
}

ava::ava(int alpha){
  is_raster=true;
  name="ava";
  int ir,ig,ib;
     for(int i=0; i < 256; i++) scale.push_back(qRgba(0,0,0,0));

  for(int i=0; i < 32; i++){
    ir=(int)(192./32.*i);
    ig=(int)(192./32.*i); 
    ib=(int)(128.+2*i);
    scale[i]=qRgba(ir,ig,ib,alpha);
  }
    for(int i=0; i < 96; i++){
    ir=(int)(192+63./96.*i);
    ig=(int)(192+63./96.*i); 
    ib=(int)(192+63./96.*i);
    scale[i+32]=qRgba(ir,ig,ib,alpha);
  }
    for(int i=0; i <96 ; i++){
    ir=(int)(255-63./96.*i);ig=(int)(255-63./96.*i); ib=(int)(255.-63./96.*i);
    scale[i+128]=qRgba(ir,ig,ib,alpha);
  }
    for(int i=0; i < 32; i++){
    ir=(int)(192-64./32.*i);ig=(int)(192.-192./32.*i); ib=(int)(192-192/32.*i);
    scale[i+224]=qRgba(ir,ig,ib,alpha);
  }
  




}

rainbow::rainbow(int alpha)
{
  int ir,ig,ib;
   for(int i=0; i < 256; i++) scale.push_back(qRgba(0,0,0,0));
     is_raster=true;

   //QImage *image = new QImage(300,300, 8, 256, QImage::IgnoreEndian);
 // QRgb * cp = image->colorTable();
//QVector<uint>  cp = image->colorTable();
     bcolor="none";
     ecolor="none";
   name="rainbow";
     int col1[]={0,0,0,84,84,0,0,84,84};
     for(int i=0; i < 85; i++){
       ir=(int)(this->fill_color(col1,i,86)*255);
       ig=(int)(fill_color(&col1[3],i,86)*255);
       ib=(int)(fill_color(&col1[6],i,86)*255);
       scale[i+1]=qRgba(ir,ig,ib,alpha);
     }
     int col2[]={0,84,84,0,84,84,84,84,0};
     for(int i=0; i < 85; i++){
       ir=(int)(fill_color(col2,i,86)*255);
       ig=(int)(fill_color(&col2[3],i,86)*255);
       ib=(int)(fill_color(&col2[6],i,86)*255);
       scale[i+86]=qRgba(ir,ig,ib,alpha);
     }
     int col3[]={83,83,83,83,0,0,0,0,83};
     for(int i=0; i < 84; i++){
       ir=(int)(fill_color(col3,i,84)*255);
       ig=(int)(fill_color(&col3[3],i,84)*255);
       ib=(int)(fill_color(&col3[6],i,84)*255);
       scale[i+171]=qRgba(ir,ig,ib,alpha);
     }

  is_raster=true;
}
color_cycle::color_cycle(int alpha)
{
  my_colors colors=my_colors();  
  std::map<QString, QColor>::const_iterator i;
 // int doline;
 // int ncol=colors.cmap.size();
  //int beg=0,icol=0;
  for(int i=0; i < 256; i++) scale.push_back(qRgba(0,0,0,alpha));
  for ( i=colors.cmap.begin(); i !=colors.cmap.end(); ++i){
    int mid=colors.cnum[i->first];
    QColor col=i->second;
    int red=col.red(),green=col.green(), blue=col.blue();
    for(int j=mid-8; j < mid+8; j++){
      scale[j]=qRgba(red,green,blue,alpha);
    }
  }
  is_raster=true;

}
       

       
