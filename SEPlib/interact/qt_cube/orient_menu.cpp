#include "orient_menu.h"
#include <QPixmap>
#include "util.h"


orient_menu::orient_menu(datasets * dats, QString num,int nd){

  this->set_basics(dats,num,"Orient",nd);
}
 void orient_menu::create_menu(){
   create_main_tab();

  create_tranpose_tab();

  create_rotate_2d_tab();
  create_rotate_3d_tab();
  create_map_tab();
 
 }
void orient_menu::create_main_tab(){
     mainWidget=new QWidget(this,"Main");
        layMain = new QHBoxLayout;
// flip selection
	
	orientFlipBox = new QGroupBox( tr("Flip Axis"));
	  
	  QHBoxLayout *layoutf=new QHBoxLayout;
//	  	layoutf->setContentsMargins(2,2,2,2);


	orientFlipDownButton = new QPushButton(QIcon(qPixmapFromMimeSource("down-arrow.png")),
		0, orientFlipBox, tr("orientFlipDownButton - orient_menu"));
	//orientFlipDownButton->setFixedWidth(BUTTON_SIZE);
	orientFlipAcrossButton = new QPushButton(QIcon(qPixmapFromMimeSource("across-arrow.png")),
		0, orientFlipBox, tr("orientFlipAcrossButton - orient_menu"));
	//orientFlipAcrossButton->setFixedWidth(BUTTON_SIZE);
	orientFlipDeepButton = new QPushButton(QIcon(qPixmapFromMimeSource("deep-arrow.png")),
		0, orientFlipBox, tr("orientFlipDeepButton - orient_menu"));
	//orientFlipDeepButton->setFixedWidth(BUTTON_SIZE);
	//orientFlipAcrossButton->setMinimumSize(QSize(20,20));
//	orientFlipDeepButton->setMinimumSize(QSize(20,20));

//	orientFlipDownButton->setMinimumSize(QSize(20,20));
	QToolTip::add(orientFlipDownButton, tr("DOWN -- Flip the down axis."));
	QToolTip::add(orientFlipAcrossButton, tr("ACROSS -- Flip the across axis."));
	QToolTip::add(orientFlipDeepButton, tr("DEEP -- Flip the deep axis."));
	connect(orientFlipDownButton, SIGNAL(clicked()),this, SLOT(orientFlipDownButtonClicked()));
	connect(orientFlipAcrossButton, SIGNAL(clicked()), this, SLOT(orientFlipAcrossButtonClicked()));
	connect(orientFlipDeepButton, SIGNAL(clicked()),this, SLOT(orientFlipDeepButtonClicked()));
  layoutf->addWidget(orientFlipDownButton);
  layoutf->addWidget(orientFlipAcrossButton);
  layoutf->addWidget(orientFlipDeepButton);
   	layoutf->setContentsMargins(2,2,2,2);

  orientFlipBox->setLayout(layoutf);
  
	// frame selection
	orientFrameBox = new QGroupBox( tr("Frame orient"));

	orientMoveMiddleButton = new QPushButton(QIcon(qPixmapFromMimeSource("middle.png")),
		0, orientFrameBox, tr("orientMoveMiddleButton - orient_menu"));
//	orientMoveMiddleButton->setFixedWidth(BUTTON_SIZE);
	orientMoveCornerButton = new QPushButton(QIcon(qPixmapFromMimeSource("corner.png")),
		0, orientFrameBox, tr("orientMoveCornerButton - orient_menu"));
//	orientMoveCornerButton->setFixedWidth(BUTTON_SIZE);
	//	orientMoveCornerButton->setMinimumSize(QSize(20,20));
//		orientMoveMiddleButton->setMinimumSize(QSize(20,20));

	QToolTip::add(orientMoveMiddleButton, tr("MIDDLE -- Move frames to middle of all three axes."));
	QToolTip::add(orientMoveCornerButton, tr("CORNER -- Move frames to ends of all three axes."));
	connect(orientMoveMiddleButton, SIGNAL(clicked()), this, SLOT(orientMoveMiddleButtonClicked()));
	connect(orientMoveCornerButton, SIGNAL(clicked()), this, SLOT(orientMoveCornerButtonClicked()));

	orientResetButton = new QPushButton(QIcon(qPixmapFromMimeSource("undo.png")),
		0, orientFrameBox, tr("orientResetButton - orient_menu"));
	//orientResetButton->setFixedWidth(BUTTON_SIZE);
	trueButton = new QPushButton(tr("true"),orientFrameBox, tr("true button"));
	//orientResetButton->setMinimumSize(QSize(20,20));
	//trueButton->setMinimumSize(QSize(20,20));

	QToolTip::add(orientResetButton, tr("RESET -- Reset orientation to default values."));
	QToolTip::add(trueButton, tr("True proportions"));
	//QToolTip::add(orientMenuPopButton, tr("MENU -- Bring up an orientation menu with more options."));
	connect(orientResetButton, SIGNAL(clicked()), this, SLOT(orientResetButtonClicked()));
	connect(trueButton, SIGNAL(clicked()), this, SLOT(trueButtonClicked()));
		  	QHBoxLayout *layoutm=new QHBoxLayout;
  		layoutm->setContentsMargins(2,2,2,2);

		 layoutm->addWidget(orientMoveMiddleButton);
		  layoutm->addWidget(orientMoveCornerButton);
		   layoutm->addWidget(orientResetButton);
		    layoutm->addWidget(trueButton);
		    orientFrameBox->setLayout(layoutm);
		    
    layMain->addWidget(orientFlipBox);
	layMain->addWidget(orientFrameBox);
	layMain->addStretch(1);
	mainWidget->setLayout(layMain);
	this->addTab(mainWidget,"Main");
}
void orient_menu::create_tranpose_tab(){
     swapWidget=new QWidget(this,"Transpose");
    	laySwap = new QHBoxLayout;
// swap selection
	orientswapBox = new QGroupBox(tr("Swap"));
 QHBoxLayout *layouts=new QHBoxLayout;

	orientSwapComboBox = new QComboBox(orientswapBox, tr("orientSwapComboBox - orient_menu"));
	//orientSwapComboBox->setMinimumSize(QSize(20,20));


	orientSwapComboBox->insertItem(tr("Choose ..."));
	orientSwapComboBox->insertItem(tr("Swap(12) Side and Top Faces"));
	ndim_transp=2;
	if(ndims>2){
	 orientSwapComboBox->insertItem(tr("Swap(23) Front and Side Faces"));
	 orientSwapComboBox->insertItem(tr("Swap(13) Top and Front Faces"));
	 ndim_transp=4;
	 if(ndims>3){
	   orientSwapComboBox->insertItem(tr("Swap(34) Deep and Fourth Axes"));
    	orientSwapComboBox->insertItem(tr("Swap(24) Across and Fourth Axes"));
    	orientSwapComboBox->insertItem(tr("Swap(14) Down and Fourth Axes"));
    	ndim_transp=7;
    }
    if(ndims>4){
    	orientSwapComboBox->insertItem(tr("Swap(35) Deep and Fifth Axes"));
    	orientSwapComboBox->insertItem(tr("Swap(25) Across and Fifth Axes"));
    	orientSwapComboBox->insertItem(tr("Swap(15) Down and Fifth Axes"));
    	ndim_transp=10;
    }
	}
	layouts->addWidget(orientSwapComboBox);
 	layouts->setContentsMargins(2,2,2,2);
	orientswapBox->setLayout(layouts);
	QToolTip::add(orientSwapComboBox, tr("SWAP FACES -- Swap two faces of the cube."));
	connect(orientSwapComboBox, SIGNAL(activated(const QString&)), this, SLOT(orientSwapComboBoxActivated(const QString&)));
	laySwap->addWidget(orientswapBox);
 
	laySwap->addStretch(1);
	swapWidget->setLayout(laySwap);
	this->addTab(swapWidget,"Transpose");
}
void orient_menu::create_rotate_2d_tab(){
   rot2dWidget=new QWidget(this,"Main");
  layRot2d = new QHBoxLayout;
  
//  rotate2dButton = new QPushButton("Accept");
    reset2dButton = new QPushButton("Reset");
  setpt2dButton  = new QPushButton("Set pt");
  	QToolTip::add(reset2dButton, tr("Reset display"));

//	QToolTip::add(rotate2dButton, tr("Rotate display"));
	QToolTip::add(setpt2dButton, tr("Set the current location as rotation point"));
//	connect(rotate2dButton, SIGNAL(clicked()),this, SLOT(rotateAcceptButtonClicked()));
  	connect(reset2dButton, SIGNAL(clicked()),this, SLOT(orientResetButtonClicked()));

  connect(setpt2dButton, SIGNAL(clicked()),this, SLOT(setptButtonClicked()));
	axis2dBox1 = new QComboBox();
//	axis2dBox2 = new QComboBox();
  axis2dBox1->insertItem(tr("none"));
  //axis2dBox2->insertItem(tr("none"));
  std::vector<int> ns=datas->return_dat(0)->return_grid()->return_ns();
  for(int i=0; i < 8; i++){
    for(int j=i+1; j < 8; j++){
      if(ns[i]>1 && ns[j]>1){
        axis2dBox1->insertItem("Plane "+QString::number(i+1)+QString::number(j+1));
     //   axis2dBox2->insertItem("Plane "+QString::number(i+1)+QString::number(j+1));
      }
     }
   }
	connect(axis2dBox1, SIGNAL(activated(const QString&)), this, 
	  SLOT(rotateAxis1_1(const QString&)));
//	connect(axis2dBox2, SIGNAL(activated(const QString&)), this, 
//	  SLOT(rotateAxis1_2(const QString&)));
	  	QToolTip::add(axis2dBox1, tr("Rotate axis pair"));
	//  		  	QToolTip::add(axis2dBox2, tr("Rotate axis pair"));
   
   RotateBox=new QGroupBox("-44 -Angle- 44");
   QHBoxLayout  *layangle=new QHBoxLayout();
   rotate2dSlider = new QSlider(Qt::Horizontal);
   
       rotate2dSlider->setRange(-45,45);
        rotate2dSlider->setValue(0);
        rotate2dSlider->setMinimumSize(QSize(40,25));
     rotate2dSlider->setTickInterval (45);
     rotate2dSlider->setTickPosition(QSlider::TicksAbove);
   QToolTip::add( rotate2dSlider, tr("Rotation angle"));
        connect(rotate2dSlider,
        SIGNAL(valueChanged(int)), this, SLOT(setRotateSlider (int)));
  layangle->addWidget(rotate2dSlider);
 
  angBox=new QLineEdit();
  QToolTip::add(angBox,tr("Rotation angle"));
  connect(angBox,SIGNAL(textEdited(const QString&)),this,
	  SLOT(setRotateAng(const QString&)));
 	angBox->setMaxLength(5);	

  layangle->setContentsMargins(2,2,2,2);
  RotateBox->setLayout(layangle); 
  
//  layRot2d->addWidget(rotate2dButton);
  layRot2d->addWidget(reset2dButton);
  layRot2d->addWidget(setpt2dButton);
  layRot2d->addWidget(axis2dBox1);
  layRot2d->addWidget(angBox);
  //layRot2d->addWidget(axis2dBox2);
  layRot2d->addWidget(RotateBox);
	layRot2d->addStretch(1);
	rot2dWidget->setLayout(layRot2d);
	this->addTab(rot2dWidget,"Rotate 2-D");
}
void orient_menu::create_rotate_3d_tab(){


}
void orient_menu::create_map_tab(){


}

void orient_menu::actionRespond(std::vector<QString> com){
  int *n=util::int_array_from_string(com[3]);
  set_transp(n);
  delete [] n;

}

void orient_menu::set_transp(int *ns){

  for(int i=ndim_transp; i >0; i--){
    orientSwapComboBox->removeItem(i-1);
  }

  QString sw1,sw2,sw3;
  ndim_transp=0;
  int ifake=-1;
  for(int i=3; i < 8; i++){
    if(ifake==-1 && ns[i]==1) {
      ifake=i; ns[i]=2;
    }
  }
  int ilast=0;
  for(int i=0; i<8; i++) if(ns[i] >1) ilast=i;
  for(int i=0; i < ilast ; i++) ns[i]=2;
    for(int i=ilast; i < 8; i++){
    if(ifake==-1 && ns[i]==1) {
      ifake=i; ns[i]=2;
    }
    
  }
  for(int i=0; i < 8; i++){
   
    if(ns[i] >1){
    QString sw1=QString::number(i+1);
    for(int j=i+1; j< 8; j++){
      if(ns[j]>1){
        ndim_transp+=1;
        QString sw2=QString::number(j+1);

        if(i==0){
         if(j==1) orientSwapComboBox->insertItem(tr("Swap(12) Side and Top Faces"));
         else if(j==2)orientSwapComboBox->insertItem(tr("Swap(13) Top and Front Faces"));
         else orientSwapComboBox->insertItem(tr("Swap(")+sw1+sw2+tr(")"));
       }
       else if(i==1){
        if(j==2) orientSwapComboBox->insertItem(tr("Swap(23)Front and Side Faces"));
        else orientSwapComboBox->insertItem(tr("Swap(")+sw1+sw2+tr(")"));
      }
      else orientSwapComboBox->insertItem(tr("Swap(")+sw1+sw2+tr(")"));
    }
   }
  }
 }
}
/*
 * When one fo the folowing actions takes place, the corresponding method sends a message to updater,
   in the form of a vector. The vector contains information about the event in the form of QStrings.
 */
void orient_menu::orientSwapComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("transpose")); com.push_back(text);
  send_message(com);

}
void orient_menu::setRotateSlider(const int& val) {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("rotate")); com.push_back(tr("angle"));
  com.push_back(QString::number(val));
  send_message(com);

}
void orient_menu::setRotateAng(const QString& val) {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("rotate")); com.push_back(tr("angle"));
  com.push_back(val);
  send_message(com);

}

void orient_menu::rotateAxis1_1(const QString& text) {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("rotate"));  com.push_back("axes");
  com.push_back(text);
  send_message(com);

}
void orient_menu::rotateAxis1_2(const QString& text) {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("rotate"));  com.push_back("axes");
  com.push_back(text);
  send_message(com);

}
void orient_menu::rotateAxis2(const QString& text) {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("rotate2")); com.push_back(text);
  send_message(com);

}
void orient_menu::orientFlipDownButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("flip")); com.push_back(tr("1"));
  send_message(com);
}
void orient_menu::orientFlipAcrossButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("flip")); com.push_back(tr("2"));
  send_message(com);

}
void orient_menu::orientFlipDeepButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("flip")); com.push_back(tr("3"));
  send_message(com);
	
}
void orient_menu::orientMoveMiddleButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("move")); com.push_back(tr("center"));
  send_message(com);

}
void orient_menu::orientMoveCornerButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("move")); com.push_back(tr("corner"));
  send_message(com);
}
void orient_menu::orientResetButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("reset"));
  send_message(com);
  com.clear();com.push_back(tr("orient"));
  com.push_back(tr("rotate")); com.push_back(tr("angle"));
  com.push_back("0");send_message(com);
	
}
void orient_menu::setptButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("rotate"));
  com.push_back(tr("set_pt"));
  send_message(com);
	
}
void orient_menu::rotateAcceptButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
    com.push_back(tr("rotate"));
  com.push_back(tr("accept"));
  send_message(com);
}
void orient_menu::rotateResetButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("rotate")); com.push_back(tr("angle"));
  com.push_back("0");send_message(com);
}
void orient_menu::trueButtonClicked() {
  std::vector<QString> com; com.push_back(tr("orient"));
  com.push_back(tr("true"));
  send_message(com);
	
}
