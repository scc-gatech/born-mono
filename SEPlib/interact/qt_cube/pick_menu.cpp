#include "pick_menu.h"
#include <QPixmap>
#include "my_colors.h"


pick_menu::pick_menu(datasets * dats, QString num,int nd)
{
  this->set_basics(dats,num,"Picks",nd);

}

void pick_menu::create_menu(){
  create_file_menu();
  create_size_dist_menu();
  create_color_menu();
  create_surface_menu();

}
void pick_menu::create_file_menu(){
  fileWidget=new QWidget(this,"File");
	layFile = new QHBoxLayout;

  picksFileBox = new QGroupBox( tr("Picks File"));
	QHBoxLayout  *layoutfile=new QHBoxLayout;
  layoutfile->setContentsMargins(2,2,2,2);


	picksSaveButton = new QPushButton(QIcon(qPixmapFromMimeSource("save.png")),
		0, picksFileBox, tr("picksSaveButton - pick_menu"));
	picksLoadButton = new QPushButton(QIcon(qPixmapFromMimeSource("open.png")),
		0, picksFileBox, tr("picksLoadButton - pick_menu"));
	QToolTip::add(picksSaveButton, tr("SAVE -- Save a picks file."));
	QToolTip::add(picksLoadButton, tr("LOAD -- Load a picks file."));
	connect(picksSaveButton, SIGNAL(clicked()), this, SLOT(picksSaveButtonClicked()));
	connect(picksLoadButton, SIGNAL(clicked()), this, SLOT(picksLoadButtonClicked()));
   picksSaveButton->setMinimumSize(QSize(20,20));
   picksLoadButton->setMinimumSize(QSize(20,20));

  layoutfile->addWidget(picksSaveButton);
  layoutfile->addWidget(picksLoadButton);
  picksFileBox->setLayout(layoutfile);
  
  layFile->addWidget(picksFileBox);
	layFile->addStretch(1);
  fileWidget->setLayout(layFile);
	this->addTab(fileWidget,"File");
}
void pick_menu::create_size_dist_menu(){

  sizeWidget=new QWidget(this,"Size/Dist");
	laySize = new QHBoxLayout;	
// actions
	picksActionBox = new QGroupBox(tr("Distance"));
  QHBoxLayout  *layoutaction=new QHBoxLayout;
	layoutaction->setContentsMargins(2,2,2,2);
	rangeSlider = new QSlider(Qt::Horizontal);

	rangeSlider->setRange(0,200);
	rangeSlider->setValue(20);
	rangeSlider->setMinimumSize(QSize(40,25));
	connect(rangeSlider, SIGNAL(valueChanged(int)), this, 
	  SLOT(setRangeSlider(int)));




 layoutaction->addWidget(rangeSlider);
//  layoutaction->addWidget(picksDecRangeButton);
 // layoutaction->addWidget(picksIncSizeButton);
//  layoutaction->addWidget(picksDecSizeButton);
  picksActionBox->setLayout(layoutaction);
  
  
  
  
    picksSnapBox = new QGroupBox(tr("Snap"));
	QHBoxLayout  *layoutsnap=new QHBoxLayout;
	
    picksSnap = new QComboBox();
    picksSnap->insertItem(tr("no"));
    picksSnap->insertItem(tr("low"));
    picksSnap->insertItem(tr("high"));
    picksSnap->insertItem(tr("center"));

    
		    picksSnapBox->setMinimumSize(QSize(40,20));

	QToolTip::add(picksSnapBox, tr("Snap to near high/low/zero"));
	connect(picksSnap, SIGNAL(activated(const QString&)), this, 
	  SLOT(picksSnapBoxActivated(const QString&)));
	
	layoutsnap->setContentsMargins(2,2,2,2);

  layoutsnap->addWidget(picksSnap);
  picksSnapBox->setLayout(layoutsnap);

	   extra_box = new QGroupBox(tr("Extra"));

	QHBoxLayout  *layoutextra=new QHBoxLayout;
	  	layoutextra->setContentsMargins(2,2,2,2);

 extra_select = new QComboBox();
  for(int i=0; i <50; i++)
      extra_select->insertItem(QString::number(i));
    


	layoutextra->addWidget(extra_select);

	extra_box->setLayout(layoutextra);

	   extra_select->setMinimumSize(QSize(40,20));

	QToolTip::add(extra_select, tr("Extra information for a given pick"));
	connect(extra_select, SIGNAL(activated(const QString&)), this, SLOT(set_extra_choice(const QString&)));
	
	
	 	picksIncSizeButton = new QPushButton(QIcon(qPixmapFromMimeSource("picks-large.png")),
		0, picksActionBox, tr("picksIncSizeButton - pick_menu"));
	picksDecSizeButton = new QPushButton(QIcon(qPixmapFromMimeSource("picks-small.png")),
		0, picksActionBox, tr("picksDecSizeButton - pick_menu"));

    picksIncSizeButton->setMinimumSize(QSize(20,20));
    picksDecSizeButton->setMinimumSize(QSize(20,20));
    QToolTip::add(picksIncSizeButton, tr("INCREASE SIZE -- Increase drawing size of picks."));
	QToolTip::add(picksDecSizeButton, tr("DECREASE SIZE -- Decrease drawing size of picks."));
	connect(picksIncSizeButton, SIGNAL(clicked()), this, SLOT(picksIncSizeButtonClicked()));
	connect(picksDecSizeButton, SIGNAL(clicked()), this, SLOT(picksDecSizeButtonClicked()));

    /*
	QGroupBox *bulkbox= new QGroupBox(tr("Multi-move"));
    QHBoxLayout  *layoutbulk=new QHBoxLayout;
    layoutbulk->setContentsMargins(2,2,2,2);
    QRadioButton *bon = new QRadioButton(tr("On"), bulkbox, tr("On"));
	QRadioButton *boff= new QRadioButton(tr("Off"), bulkbox, tr("Off"));
	if (!boff->isOn()) boff->toggle();
	QToolTip::add(bon, tr("Move multiple points"));
    QToolTip::add(boff, tr("Move single point"));
	connect(bon, SIGNAL(clicked()), this, SLOT(bulk_on()));
	connect(boff, SIGNAL(clicked()), this, SLOT(bulk_off()));
	layoutbulk->addWidget(bon);
	layoutbulk->addWidget(boff);
	bulkbox->setLayout(layoutbulk);
	*/
	laySize->addWidget(picksIncSizeButton);
	laySize->addWidget(picksDecSizeButton);
	laySize->addWidget(picksActionBox);
	laySize->addWidget(picksSnapBox);
		laySize->addWidget(extra_box);
		//laySize->addWidget(bulkbox);
		
  laySize->addStretch(1);
  sizeWidget->setLayout(laySize);
	this->addTab(sizeWidget,"Size/Distance");

}
void pick_menu::create_color_menu(){
  colorWidget=new QWidget(this,"Color/Draw");
	layColor = new QHBoxLayout;	
  picksChooseBox = new QGroupBox( tr("Pick group"));
	QHBoxLayout  *layoutchoose=new QHBoxLayout;
		  	layoutchoose->setContentsMargins(2,2,2,2);

  my_colors cols=my_colors();
  std::vector<QString> cvec=cols.return_cvec_big();

	picksActiveBox = new QComboBox(picksChooseBox, vnum+tr("-picks-active- pick_menu"));
	for(int i=0; i < (int) cvec.size(); i++)
	  picksActiveBox->insertItem(cvec[i]);

	layoutchoose->addWidget(picksActiveBox);
	picksChooseBox->setLayout(layoutchoose);
	    picksActiveBox->setMinimumSize(QSize(40,20));


	   picksChooseBox2 = new QGroupBox(tr("View/Change"));
	QHBoxLayout  *layoutchoose2=new QHBoxLayout;
	  	layoutchoose2->setContentsMargins(2,2,2,2);
	connect(picksActiveBox, SIGNAL(activated(const QString&)), this, 
	  SLOT(picksActiveBoxActivated(const QString&)));

    picksViewBox = new QComboBox(picksChooseBox2, vnum+tr("-picks-view- pick_menu"));
    picksViewBox->insertItem(tr("all"));
    for(int i=0 ; i < (int)cvec.size(); i++)
      picksViewBox->insertItem(cvec[i]);
  	picksViewBox->insertItem(tr("none"));
		    picksViewBox->setMinimumSize(QSize(40,20));

	QToolTip::add(picksActiveBox, tr("Pick Group"));
	QToolTip::add(picksActiveBox, tr("Which pick groups to view"));
	connect(picksViewBox, SIGNAL(activated(const QString&)), this, 
	  SLOT(picksViewBoxActivated(const QString&)));
  layoutchoose2->addWidget(picksViewBox);
  picksChooseBox2->setLayout(layoutchoose2);

	
    lineptgroup = new QGroupBox( tr("Display"));
    
    QHBoxLayout *layoutpt=new QHBoxLayout;
	  	layoutpt->setContentsMargins(2,2,2,2);


     QComboBox *display_box = new QComboBox(lineptgroup,tr("Display"));

display_box->insertItem(tr("points"));
	display_box->insertItem(tr("line"));
	display_box->insertItem(tr("extra"));
	QToolTip::add(display_box, tr("Display method for picks"));

    layoutpt->addWidget(display_box);
    lineptgroup->setLayout(layoutpt);
	connect(display_box, SIGNAL(activated(const QString&)), this,
	SLOT(display_changed(const QString&)));

  
  
    single_box = new QGroupBox(tr("Single"));


	QHBoxLayout  *layoutsingle=new QHBoxLayout;
	  	layoutsingle->setContentsMargins(2,2,2,2);

	single_select = new QComboBox(single_box, tr("Single box-pick"));
	single_select->insertItem(tr("1"));
	single_select->insertItem(tr("2"));
	single_select->insertItem(tr("3"));
	single_select->insertItem(tr("4"));
	single_select->insertItem(tr("5"));
	single_select->insertItem(tr("6"));
	single_select->insertItem(tr("7"));
	single_select->insertItem(tr("8"));
	layoutsingle->addWidget(single_select);
	single_box->setLayout(layoutsingle);
	   single_select->setMinimumSize(QSize(40,20));

	QToolTip::add(single_select, tr("For lines, axis which is single valued"));
	connect(single_select, SIGNAL(activated(const QString&)), this, SLOT(overlay_change(const QString&)));
	 layColor->addWidget(picksChooseBox);
  layColor->addWidget(picksChooseBox2);
	layColor->addWidget(lineptgroup);
	layColor->addWidget(single_box);
	layColor->addStretch(1);
	colorWidget->setLayout(layColor);
	this->addTab(colorWidget,"Color");
}
void pick_menu::create_surface_menu(){
  surfaceWidget=new QWidget(this,"Surface");
	laySurface = new QHBoxLayout;
	
	depthampgroup = new QGroupBox( tr("Surface property"));
    QHBoxLayout *layoutdepthamp=new QHBoxLayout;
	  	layoutdepthamp->setContentsMargins(2,2,2,2);


	depthbutton = new QRadioButton(tr("Single"), depthampgroup, tr("Single"));
	ampbutton= new QRadioButton(tr("Amplitude"), depthampgroup, tr("Amplitude"));
	if (!depthbutton->isOn()) depthbutton->toggle();
	QToolTip::add(depthbutton, tr("Surface based on single axis"));
	QToolTip::add(ampbutton, tr("Surface based on amplitude"));
	connect(depthbutton, SIGNAL(clicked()), this, SLOT(depth_button_clicked()));
	connect(ampbutton, SIGNAL(clicked()), this, SLOT(amp_button_clicked()));
  layoutdepthamp->addWidget(depthbutton);
  layoutdepthamp->addWidget(ampbutton);
  depthampgroup->setLayout(layoutdepthamp);
	
	surfcolorgroup= new QGroupBox( tr("Pickset"));
	QHBoxLayout  *layoutchoose=new QHBoxLayout;
		  	layoutchoose->setContentsMargins(2,2,2,2);

  my_colors cols=my_colors();
  std::vector<QString> cvec=cols.return_cvec();
	surfaceColor= new QComboBox();
	for(int i=0; i < (int) cvec.size(); i++)
	  surfaceColor->insertItem(cvec[i]);

	layoutchoose->addWidget(surfaceColor);
	surfcolorgroup->setLayout(layoutchoose);
	   surfcolorgroup->setMinimumSize(QSize(40,20));
	
	
	
	 picksDataBox = new QGroupBox(tr("Dataset"));
	QHBoxLayout  *layoutdata=new QHBoxLayout;

    picksdata = new QComboBox();
    for(int i=0; i < datas->return_ndat()-1; i++)
      picksdata->insertItem(datas->return_dat(i)->get_name());
		picksdata->setMinimumSize(QSize(40,20));

	QToolTip::add(picksdata, tr("Dataset to base surface on"));
	connect(picksdata, SIGNAL(activated(const QString&)), this, 
	  SLOT(picksDataBoxActivated(const QString&)));
  layoutdata->addWidget(picksdata);
  picksDataBox->setLayout(layoutdata);
		  	layoutdata->setContentsMargins(2,2,2,2);

	
	
		
	 updateSurfaceBox = new QGroupBox(tr("Update view"));
	QHBoxLayout  *layoutupdate=new QHBoxLayout;
	updateButton = new QPushButton("Update",	updateSurfaceBox);
	updateButton ->setToolTip( tr("Update view (min,max,histo)"));
	connect(updateButton, SIGNAL(clicked()),this, SLOT(updateButtonClicked()));
 layoutupdate->addWidget(updateButton);
 updateSurfaceBox->setLayout(layoutupdate);
 layoutupdate->setContentsMargins(2,2,2,2);


    picksdata = new QComboBox();
    for(int i=0; i < datas->return_ndat()-1; i++)
      picksdata->insertItem(datas->return_dat(i)->get_name());
		picksdata->setMinimumSize(QSize(40,20));



	
	
	
	laySurface->addWidget(depthampgroup);
	laySurface->addWidget(surfcolorgroup);
	laySurface->addWidget(picksDataBox);
	laySurface->addWidget(updateSurfaceBox);
	laySurface->addStretch(1);
  surfaceWidget->setLayout(laySurface);
  this->addTab(surfaceWidget,"Surface");	
  
  
  
}
 


void pick_menu::picksViewBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("pick");
  com.push_back(tr("view")); com.push_back(text);
  send_message(com);

}
void pick_menu::picksSnapBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("pick");
  com.push_back(tr("snap")); 
  com.push_back(text);
  send_message(com);

}
void pick_menu::picksActiveBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("pick");
   com.push_back(tr("active")); com.push_back(text);
		send_message(com);

}
void pick_menu::picksSaveButtonClicked() {
	QString filename = QFileDialog::getSaveFileName( this);
	 std::vector <QString> com; com.push_back("pick");
	 com.push_back(tr("save")); com.push_back(filename);
	if ( !filename.isNull() )  
	    	send_message(com);

}
void pick_menu::picksLoadButtonClicked() {
	QString filename = QFileDialog::getOpenFileName( this);
							  std::vector <QString> com; com.push_back("pick");
							  com.push_back(tr("load")); com.push_back(filename);
		if ( !filename.isNull() )  
	   
	    	send_message(com);


}
void pick_menu::setRangeSlider(int val) {
  std::vector <QString> com; com.push_back("pick");
  com.push_back(tr("range")); com.push_back(QString::number(val));
	send_message(com);
}
void pick_menu::picksIncSizeButtonClicked() {
  std::vector <QString> com; com.push_back("pick");
  com.push_back(tr("size")); com.push_back(tr("increase"));

		send_message(com);

}
void pick_menu::bulk_on() {
  std::vector <QString> com; com.push_back("pick");
  com.push_back(tr("bulk")); com.push_back(tr("on"));
		send_message(com);
}
void pick_menu::bulk_off() {
  std::vector <QString> com; com.push_back("pick");
  com.push_back(tr("bulk")); com.push_back(tr("off"));
		send_message(com);
}
void pick_menu::picksDecSizeButtonClicked() {
  std::vector <QString> com; com.push_back("pick");
  com.push_back(tr("size")); com.push_back(tr("decrease"));

		send_message(com);

}

void pick_menu::display_changed(const QString &text) {
std::vector <QString> com; com.push_back("pick");
 com.push_back("draw");com.push_back(text);

	send_message(com);
}
void pick_menu::depth_button_clicked() {
std::vector <QString> com; com.push_back("pick");
com.push_back("surface"); com.push_back("display");
com.push_back(tr("depth"));
		send_message(com);

}
void pick_menu::amp_button_clicked() {
std::vector <QString> com; com.push_back("pick");
 com.push_back("surface"); com.push_back("display");
com.push_back(tr("amplitude"));
	send_message(com);
}
void pick_menu::updateButtonClicked() {
std::vector <QString> com; com.push_back("pick");
 com.push_back("surface"); com.push_back("update");
	send_message(com);
}
void pick_menu::overlay_change(const QString& text) {
  std::vector <QString> com; com.push_back("pick");
   com.push_back(tr("single")); com.push_back(text);

	send_message(com);
}
void pick_menu::set_extra_choice(const QString& text) {
  std::vector <QString> com; com.push_back("pick");
   com.push_back(tr("extra")); com.push_back(text);

	send_message(com);
}
void pick_menu::pickSurfaceColor(const QString& text) {
  std::vector <QString> com; com.push_back("pick");
   com.push_back(tr("surface")); com.push_back(tr("color")); com.push_back(text);

	send_message(com);
}
void pick_menu::picksDataBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("pick"); com.push_back(tr("surface"));
   com.push_back(tr("data")); com.push_back(text);

	send_message(com);
}
