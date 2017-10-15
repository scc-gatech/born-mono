#include "region_menu.h"
#include <QPixmap>
#include "my_colors.h"


region_menu::region_menu(datasets * dats, QString num,int nd)
{
  this->set_basics(dats,num,"Picks",nd);
  mainWidget=new QWidget(this,"File");
  colorWidget=new QWidget(this,"Color/Draw");
  sizeWidget=new QWidget(this,"Size/Dist");
  //drawWidget=new QWidget(this,"Draw");
}

void region_menu::createit() {
//Layouts for the file, color and size tabs, respectively.
	layMain = new QHBoxLayout;
	layColor = new QHBoxLayout;	
	laySize = new QHBoxLayout;	
	//layDraw = new QHBoxLayout;



	picksFileBox = new QGroupBox( tr("Save"));
	QHBoxLayout  *layoutfile=new QHBoxLayout;
//		  	layoutfile->setContentsMargins(2,2,2,2);


	//picksFileBox->setFixedHeight(MINIMUM_HEIGHT_MENU);
	regionSaveButton = new QPushButton(QIcon(qPixmapFromMimeSource("save.png")),
		0, picksFileBox, tr("regionSaveButton - region_menu"));
	
	QToolTip::add(regionSaveButton, tr("SAVE -- Save a picks file."));

	connect(regionSaveButton, SIGNAL(clicked()), this, SLOT(regionSaveButtonClicked()));
   regionSaveButton->setMinimumSize(QSize(20,20));

  layoutfile->addWidget(regionSaveButton);
  picksFileBox->setLayout(layoutfile);
  
	
   
   
   horizdirBox = new QGroupBox(tr("Boundary direction"));
	QHBoxLayout  *layouthoriz=new QHBoxLayout;
 	layouthoriz->setContentsMargins(2,2,2,2);

    horizdir = new QComboBox(horizdirBox, vnum+tr("-picks-view- region_menu"));
    horizdir->insertItem(tr("none"));
    horizdir->insertItem(tr("up"));
    horizdir->insertItem(tr("down"));
    
     vertdir = new QComboBox(horizdirBox, vnum+tr("-picks-view- region_menu"));
    vertdir->insertItem(tr("none"));
    vertdir->insertItem(tr("up"));
    vertdir->insertItem(tr("down"));
    
    horizdirBox->setMinimumSize(QSize(40,20));

	QToolTip::add(horizdirBox, tr("Set horizontal direction for secondary color"));
	connect(horizdir, SIGNAL(activated(const QString&)), this, 
	  SLOT(horizdirBoxActivated(const QString&)));
	
	QToolTip::add(horizdirBox, tr("Set vertical direction for secondary color"));
	connect(horizdir, SIGNAL(activated(const QString&)), this, 
	  SLOT(horizdirBoxActivated(const QString&)));
	
  layouthoriz->addWidget(horizdir);
    layouthoriz->addWidget(vertdir);

  horizdirBox->setLayout(layouthoriz);


   picksChooseBox = new QGroupBox( tr("Primary"));
	QHBoxLayout  *layoutchoose=new QHBoxLayout;
		  	layoutchoose->setContentsMargins(2,2,2,2);

  my_colors cols=my_colors();
  std::vector<QString> cvec=cols.return_cvec();
	primaryColorBox = new QComboBox(picksChooseBox, vnum+tr("-picks-active- region_menu"));
	for(int i=0; i < (int) cvec.size(); i++)
	  primaryColorBox->insertItem(cvec[i]);

	layoutchoose->addWidget(primaryColorBox);
	picksChooseBox->setLayout(layoutchoose);
	    primaryColorBox->setMinimumSize(QSize(40,20));


	   picksChooseBox2 = new QGroupBox(tr("Secondary"));
	QHBoxLayout  *layoutchoose2=new QHBoxLayout;
  	layoutchoose2->setContentsMargins(2,2,2,2);

    secondColorBox = new QComboBox(picksChooseBox2, vnum+tr("-picks-view- region_menu"));
    secondColorBox->insertItem(tr("all"));
    for(int i=0 ; i < (int)cvec.size(); i++)
      secondColorBox->insertItem(cvec[i]);
  	secondColorBox->insertItem(tr("none"));
		    secondColorBox->setMinimumSize(QSize(40,20));

	QToolTip::add(primaryColorBox, tr("Set active region color"));
	connect(primaryColorBox, SIGNAL(activated(const QString&)), this, 
	  SLOT(primaryColorBoxActivated(const QString&)));
	QToolTip::add(primaryColorBox, tr("Set secondary region color"));
	connect(secondColorBox, SIGNAL(activated(const QString&)), this, 
	  SLOT(secondColorBoxActivated(const QString&)));
  layoutchoose2->addWidget(secondColorBox);
  picksChooseBox2->setLayout(layoutchoose2);


	
}

void region_menu::layout() {
	layMain->addWidget(picksFileBox);
	layMain->addWidget(horizdirBox);
	
  layMain->addWidget(picksChooseBox);
  layMain->addWidget(picksChooseBox2);

	
	layMain->addStretch(1);

  mainWidget->setLayout(layMain);
 
	this->addTab(mainWidget,"Main");

	

}


void region_menu::secondColorBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("region");
  com.push_back(tr("secondary")); com.push_back(text);
  send_message(com);

}
void region_menu::horizdirBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("region");
  com.push_back(tr("horiz")); 
  com.push_back(text);
  send_message(com);

}

void region_menu::vertdirBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("region");
  com.push_back(tr("vert")); 
  com.push_back(text);
  send_message(com);

}
void region_menu::primaryColorBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("region");
   com.push_back(tr("primary")); com.push_back(text);
		send_message(com);

}

void region_menu::regionSaveButtonClicked() {
	QString filename = QFileDialog::getSaveFileName( this);
	 std::vector <QString> com; com.push_back("region");
	 com.push_back(tr("save")); com.push_back(filename);
	if ( !filename.isNull() )  
	    	send_message(com);

}








