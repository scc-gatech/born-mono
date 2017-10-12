#include "color_panel.h"
using namespace SEP;
color_panel:: color_panel(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk, std::shared_ptr<slice_types>c,std::shared_ptr<maps>mym){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  
    
     mainLay=new QVBoxLayout();
     barLay=new QHBoxLayout();
     
     loadsave= new basicButtonBox("Read/Write",":./images/open.png","Load a color table",true);
     loadsave->addButton(":./images/save.png","Save a color table",true);
     
     std::vector<QString> ctypes;
     ctypes.push_back("grey");ctypes.push_back("flag"); ctypes.push_back("cbl");
     ctypes.push_back("cgsi"); ctypes.push_back("rainbow"); ctypes.push_back("ava");
     ctypes.push_back("patriotic"); ctypes.push_back("jet");
     color=new basicComboBox("Source","Source colortable to edit",ctypes);
     
     barLay->addWidget(loadsave->group());
     barLay->addWidget(color->group());
     colorBox=new QGroupBox();
     colorBox->setLayout(barLay);
     
     std::shared_ptr<user_color> us=std::static_pointer_cast<user_color>(my_slices->slices["user"]);
     std::shared_ptr<color_bar2> cb2(new color_bar2(us));
     
    cbar=cb2;
       cbar->setMinimumSize(300,240);

    mainLay->addWidget(cbar.get());
    mainLay->addWidget(colorBox);
    setLayout(mainLay);

    mainLay->addStretch(1);
    setLayout(mainLay);
   connect(cbar.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));


}
void color_panel::update_menu(std::vector<QString>){


}

void color_panel::actionRespond(std::vector<QString> coms){
  emit actionDetected(coms);

}
color_bar2::color_bar2(  std::shared_ptr<user_color>col){
	pm=QPixmap(100,100);
	color="red";
	maps["red"]=new float[33];
	maps["green"]=new float[33];
	maps["blue"]=new float[33];
	for(int i=0; i < 33; i++){
	  maps["red"][i]=-1;
	  maps["green"][i]=-1;
	  maps["blue"][i]=-1;
	}
	maps["red"][0]=0.001;
	maps["red"][32]=255.;
	maps["green"][0]=.001;
	maps["green"][32]=255.;
	maps["blue"][0]=0.0001;
	maps["blue"][32]=255;
    bh["red"]=0; bh["green"]=0; bh["blue"]=0;
	nh["red"]=0; nh["green"]=0; nh["blue"]=0;
    ctable=col;
}
void color_bar2::scale(){
    int h = height();

 //   if ( image->isNull() ) return;

 //   QApplication::setOverrideCursor( waitCursor ); // this might take time
    if ( width() == pm.width() && h == pm.height() )
    {						// no need to scale if widget
	pmScaled = pm;				// size equals pixmaps size
    } else {

	    QMatrix m;				// transformation matrix
	    m.scale(((double)width())/pm.width(),// define scale factors
		    ((double)h)/pm.height());
	    pmScaled = pm.transformed( m );		// create scaled pixmaps
    }
	pmScaledCopy = pmScaled;
//    QApplication::restoreOverrideCursor();	// restore original cursor
}
void color_bar2::to_pt(QString col,float px, float py, int *ix, int *iy){
  *ix=(int)(W*1.*px);
   *iy=(int)(nh[col]-nh[col]*py)+bh[col];
}
void color_bar2::to_pct(QString col,int ix, int iy, float *px, float *py){
  *px=(ix)/1./W;
  //*py=(H-iy)/H;
  *py=(float)(nh[col]-(iy-bh[col] )   )/(float)nh[col];
  
}
void color_bar2::paintEvent( QPaintEvent *e ){

    if ( pm.size() != QSize( 0,  0 ) ) {		// is an image loaded?
	QPainter painter(this);
	painter.setClipRect(e->rect());
	painter.drawPixmap(0, 0, pmScaled);
 // QPainter painter2(this);
 // painter2.drawPixmaps(0, 0, pmScaled);
 //   this->myv->viewit(&painter,&pen,fact,dat,pos,border,W-2*border,border,H-2*border);

    }
}
void color_bar2::resizeEvent( QResizeEvent *e ){
 if(e==0){;}
    if ( pm.size() == QSize( 0, 0 ) )		// we couldn't load the image
	   return;

    if ( width() != pmScaled.width() || height() != pmScaled.height())
    {						// if new size,
   	  scale();				// scale pmScaled to window
   	  set_sizes();
    	
    }
  
  draw_bar();
}
void color_bar2::set_sizes(){
  W=width();
  H=height();
  int n=H-6;
  bh["red"]=0; nh["red"]=n/4; n=n-nh["red"];
  bh["green"]=nh["red"]+2; nh["green"]=n/3; n=n-nh["green"];
  bh["blue"]=nh["green"]+4+nh["red"]; nh["blue"]=n/2; 
  nh_bar=n-nh["blue"];
  fh_bar=H-nh_bar;
}
int *color_bar2::return_map(QString col){

  int ib=0;
  bool found=false;
  QString csave=color;
  int *array=new int[256];
  color=col;
  while(ib < 33 && !found){
    if(maps[color][ib]>-0.01) found=true;
    else ib++;
  }
  int ie=32; found=false;
  while(ie >=0&& !found){
    if(maps[color][ie]>-.001) found=true;
    else ie--;
  }
  QString v;
  if(ib==33) { //No pts
  fprintf(stderr,"FOUND NO POINTS\n");
    for(int i=0; i < 256; i++){
      array[i]=i;
    }
  }
  else if(ib==ie){
  fprintf(stderr,"Found 1 point \n");
    for(int i=0; i < 256; i++){
      array[i]=(int)maps[color][ib];
    }
  }
  else{
    for(int i=0; i<ib*8; i++){
      array[i]=(int)maps[color][ib];
    }
    while(ib<ie){
      int ia=ib+1; found=false;
      while(ia<33 && !found){
        if(maps[color][ia]>-.0001) found=true;
        else ia++;
      }
      if(ia<=ie){
        float b=maps[color][ib];
        float e=maps[color][ia];

        //assert(ie==ia);
        float d=(float)(std::min(ia*8,256)-ib*8);
        for(int i=ib*8; i <std::min(ia*8,256); i++){
        float f=(float)(i-ib*8)/d;
          int val=(int)(.5+(1.-f)*b+f*e);
          array[i]=val;

        }
      }
      ib=ia;
    }
    for(int i=ie*8; i<256;i++) array[i]=(int)maps[color][ie];
  }

   color=csave;
   return array;
}
QString color_bar2::create_maps(){

  int *array=return_map(color);
  QString temp=util::string_from_int_array(256,array);
  delete [] array;
  return temp;
}
void color_bar2::set_source(QString text){
  for(int i=0; i < 33; i++){
    maps["red"][i]=maps["green"][i]=maps["blue"][i]=-1;
  }
  if(text.contains("grey")){
        maps["red"][0]=0.01;  maps["green"][0]=0.01; maps["blue"][0]=0.01;
        maps["red"][32]=255.; maps["green"][32]=255.;maps["blue"][32]=255.;
  }
  else if(text.contains("flag")){
      maps["red"][0]=0.01;  maps["green"][0]=255. ; maps["blue"][0]=255.;
      maps["red"][16]=255.; maps["green"][16]=255.;maps["blue"][16]=255.;
      maps["red"][32]=255.; maps["green"][32]=255.;maps["blue"][32]=0.01;
  }
  else if(text.contains("cbl")){
    maps["blue"][0]=0.01; maps["blue"][32]=255.0;
    maps["red"][0]=255.;  maps["red"][32]=.01;
    maps["green"][0]=0.01; maps["green"][32]=0.01;
  }
  else if(text.contains("cgsi")){
    maps["blue"][0]=0.01; maps["blue"][16]=255.; maps["blue"][32]=0.01;
    maps["green"][0]=0.01; maps["green"][16]=255.; maps["green"][32]=0.01;
    maps["red"][0]=0.01; maps["red"][16]=255.; maps["red"][32]=255.;
  }
  else if(text.contains("rainbow")){
    maps["red"][0]=0.01;  maps["green"][0]=255.; maps["blue"][0]=0.;
    maps["red"][5]=0.01;  maps["green"][5]=255.; maps["blue"][5]=255.;
    maps["red"][11]=0.01; maps["green"][11]=0.01;maps["blue"][11]=255.;
    maps["red"][16]=255.; maps["green"][16]=255.;maps["blue"][16]=255.;
    maps["red"][22]=255.; maps["green"][22]=255.;maps["blue"][22]=0.01;
    maps["red"][27]=255.; maps["green"][27]=0.01;maps["blue"][27]=0.01;
    maps["red"][32]=255.; maps["green"][32]=0.01;maps["blue"][32]=255.1;
  }
  else if(text.contains("jet")){
     maps["red"][0]=0.01 ; maps["green"][0]=0.01; maps["blue"][0]=127; 
     maps["red"][4]=0.01;  maps["green"][4]=0.01; maps["blue"][4]=255.;
     maps["red"][12]=0.01; maps["green"][12]=255.;maps["blue"][12]=255.;
     maps["red"][20]=255.; maps["green"][20]=255.;maps["blue"][20]=0.01;
     maps["red"][28]=255.; maps["green"][28]=0.01;maps["blue"][28]=0.01;
     maps["red"][32]=127.; maps["green"][32]=0.01;maps["blue"][32]=.01;
  }
  else if(text.contains("patriotic")){
     maps["red"][0]=0.01; maps["green"][0]=0.01; maps["blue"][0]=128.;
     maps["red"][16]=255; maps["green"][16]=255.; maps["blue"][16]=255.;
     maps["red"][32]=128.; maps["green"][32]=0.01; maps["blue"][32]=0.01;
  }
  else if(text.contains("ava")){
    maps["red"][0]=0.01; maps["green"][0]=0.01; maps["blue"][0]=128.;
    maps["red"][4]=192;  maps["green"][4]=192;  maps["blue"][4]=192;
    maps["red"][16]=255.;maps["green"][16]=255.;maps["blue"][16]=255.;
    maps["red"][28]=192; maps["green"][28]=192.;maps["blue"][28]=192;
    maps["red"][32]=128.;maps["green"][32]=0.01;maps["blue"][32]=0.01;
  }
  color="red"; redo_line();
  color="green"; redo_line();
  color="blue"; redo_line();
}
void color_bar2::load_cbar(QString text){

 FILE *fd;
  for(int i=0; i < 33; i++){
    maps["red"][i]=maps["green"][i]=maps["blue"][i]=-1;
  }

  char  color2[1024];
  QString col;
  int isamp;
  float val;
  char line[2048];
   if ((fd=fopen(text.toAscii(),"r")) == NULL) fprintf(stderr,"PROBLEM OPENING \n");
  while (!feof(fd)){
    fgets(line, 1000, fd);

    if (0!=strncmp (line,"#",1)) {

      sscanf(line,"%s %d %f ", color2,&isamp,&val);
      col=color2;
      maps[col][isamp]=val;

    }
  }
  fclose (fd);
  color="red"; redo_line();
  color="green"; redo_line();
  color="blue"; redo_line();
}
void color_bar2::save_cbar(QString text){

  FILE *fd;
  fd=fopen(text.toAscii(),"w");
  for(int i=0; i < 33; i++){
    if(maps["red"][i]>-.0001)   fprintf(fd,"%s %d %f \n","red",i,maps["red"][i]);
    if(maps["green"][i]>-.0001)   fprintf(fd,"%s %d %f \n","green",i,maps["green"][i]);
    if(maps["blue"][i]>-.0001)   fprintf(fd,"%s %d %f \n","blue",i,maps["blue"][i]);
  }
  fclose(fd);
}
void color_bar2::extract_draw(QString col, QPainter *painter){

  QPolygon array,pt;
  std::vector<bar_pt> pts;
  for(int i=0; i< 33; i++){
    if(maps[col][i]>0.){
      pts.push_back(bar_pt((float)i/32.,maps[col][i]/255.));
    }
  }
  	for(int i=0; i < (int) pts.size(); i++) {
	  float c=pts[i].clip;
	  int ix,iy;
	  to_pt(col,pts[i].pct,c,&ix,&iy);
	  array.putPoints(i, 1,ix,iy);
	  pt.putPoints(0,5,ix-2,iy-2,ix+2,iy-2,ix+2,iy+2,ix-2,iy+2,ix-2,iy-2);
	  painter->drawPolyline(pt);
	}
	painter->drawPolyline(array);

}
void color_bar2::draw_bar(){
    set_sizes();
	if(W<=0 || H<=0) { W=150; H=40;}
     pm=QPixmap(W,H);

	QPainter painter(&pm);
	QPen pen;
	pen.setWidth(2);
    painter.setBackground(Qt::white);
	painter.eraseRect(0,0,pm.width(), pm.height());

    QPolygon array,pt,h;
	pen.setColor(Qt::black); 
	painter.setPen(pen);

	pt.putPoints(0,2,0,bh["green"],W,bh["green"]);
    painter.drawPolyline(pt);
	pt.putPoints(0,2,0,bh["blue"],W,bh["blue"]);
    painter.drawPolyline(pt);
    pt.putPoints(0,2,0,fh_bar,W,fh_bar);
    painter.drawPolyline(pt);
	
	pen.setColor(Qt::red);painter.setPen(pen);
	extract_draw("red",&painter);
    pen.setColor(Qt::green);painter.setPen(pen);
	extract_draw("green",&painter);
    pen.setColor(Qt::blue); painter.setPen(pen);
	extract_draw("blue",&painter);
		
	unsigned char *uc=new unsigned char[256];
    int ii=0;
    for(int i=0; i < 256; i++,ii++){uc[ii]=(unsigned int) i;
   }
  QImage *img=ctable->makeImage(uc,256,1,W,nh_bar);
  QImage newt=img->scaled(W,nh_bar);
  QPixmap t2=QPixmap::fromImage(newt,Qt::OrderedAlphaDither);
  //t2.convertFromImage(newt, Qt::OrderedAlphaDither);
  painter.drawPixmap(0,fh_bar        ,t2);
  delete img;
  delete [] uc;
	
	
	
	
    scale();
    update();

}
QString color_bar2::find_col_panel(int ih){
  if(bh["red"] <=ih && ih<=bh["red"]+nh["red"]) return "red";
  if(bh["green"] <=ih && ih<= bh["green"]+nh["green"]) return "green";
  if(bh["blue"] <=ih && ih <= bh["blue"]+nh["blue"]) return "blue";
  return "none";

}
void color_bar2::mouseReleaseEvent(QMouseEvent *e){
  set_sizes();
  if(e->button() == Qt::MidButton){
     del_pt(e->x(),e->y());
   }
   else if(e->button() == Qt::LeftButton){
     add_pt(e->x(),e->y());
   }
}
void color_bar2::redo_line(){
 std::vector<QString> com;
  com.push_back("-1");
  com.push_back("ctable");  
 if(color.contains("red")>0) com.push_back("set_red");
 if(color.contains("green")>0) com.push_back("set_green");
 if(color.contains("blue")>0) com.push_back("set_blue");
 com.push_back(create_maps());
 emit actionDetected(com);
 draw_bar();
}
void color_bar2::del_pt(int ix, int iy){
  float px,py;
  QString col=find_col_panel(iy);
  if(col!="none"){
    color=col;
    to_pct(color, ix,iy,&px,&py);
    int isamp=(int)(.5+px*32);
    if(maps[color][isamp]>=0.){
      maps[color][isamp]=-1;
      redo_line();
    }
  }
 }
void color_bar2::add_pt(int ix, int iy){
  float px,py;
  QString col=find_col_panel(iy);
  if(col!="none"){
    color=col;
    to_pct(color,ix,iy,&px,&py);
    int isamp=(int)(.5+px*32);
    maps[color][isamp]=py*255.;
    redo_line();
  }
}




  
