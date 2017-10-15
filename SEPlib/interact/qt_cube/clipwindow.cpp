#include "clipwindow.h"
#include <q3mimefactory.h>
#include "color_bar.h"
#include <qlayout.h>
#include <QVBoxLayout>

ClipWindow::ClipWindow( slice_types *colt,position *p,datasets *d, 
int nv, std::vector<DrawWindow *>ws, QWidget *parent, const char *name )
        : QTabWidget( parent, name ){
  pos=p;
  datas=d;
  nviews=nv;
  windows=ws;
 // QVBoxLayout *box1 = new QVBoxLayout;
    clipMain=new QTabWidget(this);

  symetric=false;
  create_clips(d,colt);
  
 
}
void ClipWindow::create_clips(datasets *datas,slice_types *colt){



  int ndat=datas->return_ndat();//Don't do region
  if(1==3){
  for(int i=0; i < ndat; i++){
         tabs.push_back(new QTabWidget(clipMain));
         clipWidgets.push_back(new QWidget(tabs[i]));
         
         
        // colorWidgets.push_back(new QWidget(tabs[i]));
         bars.push_back(new clip_bar(i));
        // bars_c.push_back(new color_bar(i));

         createit(clipWidgets[i],bars[i],i);
        //create_color(colorWidgets[i],bars_c[i],i);
         tabs[i]->addTab(clipWidgets[i],"Basic");
         
         this->addTab(tabs[i],datas->return_dat(i)->name);
   }
   
   }
      clipTab=new QTabWidget(clipMain);
     for(int i=0; i < ndat; i++){
        bars.push_back(new clip_bar(i));
        clipWidgets.push_back(new QWidget(clipTab));
        createit(clipWidgets[i],bars[i],i);
        clipTab->addTab(clipWidgets[i],datas->return_dat(i)->name);
     
     }
     this->addTab(clipTab,"Clip");
   
   
   
   QGroupBox *multi_bar= new QGroupBox();
   QVBoxLayout *mlay= new QVBoxLayout();
   cbar=new color_bar((user_color*)colt->slices["user"],multi_bar);
   mlay->addWidget(cbar);
   cbar->setMinimumSize(300,240);

   
   QGroupBox *picksFileBox = new QGroupBox( tr("Load/Save/Source"));
	QHBoxLayout  *layoutfile=new QHBoxLayout;
  layoutfile->setContentsMargins(1,1,1,1);


	QPushButton *picksSaveButton = new QPushButton(QIcon(qPixmapFromMimeSource("save.png")),
		0, picksFileBox, tr("picksSaveButton - pick_menu"));
	QPushButton *picksLoadButton = new QPushButton(QIcon(qPixmapFromMimeSource("open.png")),
		0, picksFileBox, tr("picksLoadButton - pick_menu"));
	QToolTip::add(picksSaveButton, tr("SAVE -- Save colorbar"));
	QToolTip::add(picksLoadButton, tr("LOAD -- Load colorbar."));
	connect(picksSaveButton, SIGNAL(clicked()), this, SLOT(picksSaveButtonClicked()));
	connect(picksLoadButton, SIGNAL(clicked()), this, SLOT(picksLoadButtonClicked()));
	QComboBox *sourceButton = new QComboBox();
	sourceButton->addItem(tr("grey"));
	sourceButton->addItem(tr("flag"));
	sourceButton->addItem(tr("cbl"));
    sourceButton->addItem(tr("cgsi"));
	sourceButton->addItem(tr("rainbow"));
	sourceButton->addItem(tr("ava"));
    sourceButton->addItem(tr("patriotic"));
    sourceButton->addItem(tr("jet"));

	connect(sourceButton, SIGNAL(activated(const QString&)),this, 
	SLOT(set_source(const QString&)));

   picksSaveButton->setMinimumSize(QSize(20,20));
   picksLoadButton->setMinimumSize(QSize(20,20));

  layoutfile->addWidget(picksSaveButton);
  layoutfile->addWidget(picksLoadButton);
   layoutfile->addWidget(sourceButton);
  picksFileBox->setLayout(layoutfile);
  
  mlay->addWidget(picksFileBox);
  
   mlay->setContentsMargins(6,6,6,6);

   multi_bar->setLayout(mlay);
   
   
   
   this->addTab(multi_bar,"Color");
       connect(cbar, SIGNAL(emitit(std::vector<QString>)), this, 
    SLOT(send_it(std::vector<QString>)));
}
void ClipWindow::createit(QWidget *clipWidget, clip_bar *bar,int idat) {





   bar->setMinimumSize(300,100);
  connect(bar, SIGNAL(emitit(std::vector<QString>)), this, 
    SLOT(send_it(std::vector<QString>)));
 
  colorBoxb = new QGroupBox( tr("Begin Clip"));

  QHBoxLayout  *layoutcb=new QHBoxLayout;
	layoutcb->setContentsMargins(2,2,2,2);
  QString id=QString::number(idat);
  bclipbox = new QComboBox();
	bclipbox->addItem(id+":"+tr("black"));
	bclipbox->addItem(id+":"+tr("red"));
	bclipbox->addItem(id+":"+tr("blue"));
  bclipbox->addItem(id+":"+tr("green"));
	bclipbox->addItem(id+":"+tr("white"));
	connect(bclipbox, SIGNAL(activated(const QString&)),this, SLOT(bactivated(const QString&)));

	bclipbox->addItem(id+":"+tr("transp"));
	bclipbox->setToolTip(tr("Set color for bclip value"));
	layoutcb->addWidget(bclipbox);
  	colorBoxb->setLayout(layoutcb);

    colorBoxe = new QGroupBox( tr("End clip"));

   QHBoxLayout  *layoutce=new QHBoxLayout;
	layoutce->setContentsMargins(2,2,2,2);
  eclipbox = new QComboBox();
	eclipbox->addItem(id+":"+tr("black"));
	eclipbox->addItem(id+":"+tr("red"));
	eclipbox->addItem(id+":"+tr("blue"));
		eclipbox->addItem(id+":"+tr("green"));
				eclipbox->addItem(id+":"+tr("white"));
					eclipbox->addItem(id+":"+tr("transp"));
	connect(eclipbox, SIGNAL(activated(const QString&)),this, SLOT(eactivated(const QString&)));
	eclipbox->setToolTip( tr("Set color for eclip value"));
	layoutce->addWidget(eclipbox);
	colorBoxe->setLayout(layoutce);
    
    
   QGroupBox *symetric = new QGroupBox(tr("Symmetric"));
		QHBoxLayout *layouts=new QHBoxLayout;
	  	layouts->setContentsMargins(2,2,2,2);

	
  QRadioButton *yesButton = new QRadioButton(tr("Yes"),symetric, tr("Force Symmetric	"));
	QRadioButton *noButton = new QRadioButton(tr("No"),symetric, tr("Non-symetric"));
	if (yesButton->isOn()) yesButton->toggle();
		if (!noButton->isOn()) noButton->toggle();

	QToolTip::add(yesButton, tr("Force symmetric changes on clip function"));
	QToolTip::add(noButton, tr("Allow non-symmetric changes to clip function"));
  connect(noButton, SIGNAL(clicked()), this, SLOT(noButtonClicked()));
	connect(yesButton, SIGNAL(clicked()), this, SLOT(yesButtonClicked()));
  layouts->addWidget(yesButton);
  layouts->addWidget(noButton);
  symetric->setLayout(layouts);

  
     QGroupBox *contrast = new QGroupBox(tr("Contrast"));
		QHBoxLayout *layoutc=new QHBoxLayout;

  QSlider *contrastSlider = new QSlider(Qt::Horizontal);
	contrastSlider->setValue(0);
	contrastSlider->setRange(0,100);
	contrastSlider->setMinimumSize(QSize(40,25));
  
  layoutc->addWidget(contrastSlider);
	contrast->setLayout(layoutc);

	QToolTip::add(contrastSlider, tr("Set clip by contrast"));
   connect(contrastSlider, SIGNAL(valueChanged(int)), this, SLOT(changeContrast (int)));

    QPushButton *clearButton = new QPushButton(tr("Clear"),contrast, tr("clear button"));
	QToolTip::add(clearButton, tr("Clear picks"));
	connect(clearButton, SIGNAL(clicked()), this, SLOT(clearButtonClicked()));

		    layoutc->addWidget(clearButton);
    
    
    
    QVBoxLayout *lay= new QVBoxLayout;  
   QHBoxLayout *lay2 = new QHBoxLayout;

     colorBoxb->setMaximumSize(90,60);
     colorBoxe->setMaximumSize(90,60);
	clipWidget->setLayout(lay);

    lay2->addWidget(colorBoxb);
    lay2->addWidget(colorBoxe);
    lay2->addWidget(symetric);
    lay2->addWidget(contrast);
    lay2->addStretch(1);
     QGroupBox *bot= new QGroupBox();
	lay2->setContentsMargins(5,5,5,5);

     bot->setLayout(lay2);
    lay->addWidget(bar);
    lay->addWidget(bot);
     clipWidget->setLayout(lay);
	
    
}
/* Conversion from Numerical recipes*/
float *ClipWindow::second_deriv(int npts, float *x, float *y){
  float *second=new float[npts];
  float  *u=new float[npts];

  second[0]=0; u[0]=0;
  for(int i=1; i < npts-1; i++){
    float sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
    float p=sig*second[i-1]+2;
    second[i]=(sig-1.)/p;
    u[i]=(6.*((y[i+1]-y[i])/(x[i+1]-x[i])-(y[i]-y[i-1])/
       (x[i]-x[i-1]))/(x[i+1]-x[i-1])-sig*u[i-1])/p;
  }
  second[npts-1]=0.;
  for(int i=npts-2; i >0 ;i--){
    second[i]=second[i]*second[i+1]+u[i];
  }
  delete [] u;
  return second;
}


void ClipWindow::send_it(std::vector<QString> com){
  emit actionDetected(com);
}
void ClipWindow::actionRespond(std::vector<QString> com){
      int id=com[0].toInt();
  if(com[3].contains("set_pt")) bars[id]->set_pts(com[4]);
  if(com[3].contains("set_histo")) bars[id]->set_histo(com[4]);
}
void ClipWindow::set_source(const QString& text) {
 
   
  cbar->set_source(text);


}
void ClipWindow::bactivated(const QString& text) {
 
  std::vector<QString> com; 
  com.push_back(text.section(":",0,0));
  com.push_back(tr("clip"));
    com.push_back(tr("bcolor")); com.push_back(text.section(":",1,1));
    	  emit actionDetected(com);


}
void ClipWindow::eactivated(const QString& text) {
  std::vector<QString> com; 
   com.push_back(text.section(":",0,0));
  com.push_back(tr("clip"));
    com.push_back(tr("ecolor")); com.push_back(text.section(":",1,1));
    	  emit actionDetected(com);
}
void ClipWindow::changeContrast(int contrast){
  float x[4],y[4];
  float pc=(float)contrast/400.;
  x[0]=0; y[0]=0.031;
  x[3]=1.; y[3]=1.-y[0];
  x[1]=.25+pc; y[1]=.25-pc;
  x[2]=1.-x[1]; y[2]=1.-y[1];
  float *second=second_deriv(4,x,y);
  int index=clipTab->currentIndex();
  float vals[256];
  for(int i=0; i < 256; i++){
     float px=(float)i/255.;
     int klo,khi;
     if(px<x[1]){klo=0; khi=1;}
     else if(px<x[2]) {klo=1 ;khi=2;}
     else{ klo=2; khi=3;}
    
     float h=x[khi]-x[klo];
     float a=(x[khi]-px)/h;
     float b=(px-x[klo])/h;
     vals[i]=a*y[klo]+b*y[khi]+((a*a*a-a)*second[klo]+
       (b*b*b-b)*second[khi])*(h*h)/6.;
     if(vals[i] < .03) vals[i]=.031;
     if(vals[i]>.97) vals[i]=.969;
  }
  bars[index]->set_pcts(vals);
  delete [] second;

}
void ClipWindow::clearButtonClicked() {
  int index=clipTab->currentIndex();

  bars[index]->clear_picks();

}
void ClipWindow::yesButtonClicked() {
  int index=clipTab->currentIndex();

  bars[index]->set_symmetric(true);

}
void ClipWindow::noButtonClicked() {
    int index=clipTab->currentIndex();

  bars[index]->set_symmetric(false);

}
void ClipWindow::picksSaveButtonClicked() {
	QString filename = QFileDialog::getSaveFileName( this);
	if ( !filename.isNull() ) {
	   cbar->save_cbar(filename);
	}
	   

}
void ClipWindow::picksLoadButtonClicked() {
	QString filename = QFileDialog::getOpenFileName( this);
						
		if ( !filename.isNull() ) cbar->load_cbar(filename); 
	   


}
