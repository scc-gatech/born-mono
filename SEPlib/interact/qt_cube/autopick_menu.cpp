#include "autopick_menu.h"


autopick_menu::autopick_menu(datasets *dats, QString num,int nd)
{
  this->set_basics(dats,num,"Autopick",nd);
     
      mainWidget=new QWidget(this);
      Widget2d=new QWidget(this);
    //  brownWidget=new QWidget(this);
    // flatWidget=new QWidget(this);
}

void autopick_menu::createit() {
	lay = new QHBoxLayout;
	lay2d = new QHBoxLayout;
	//layB = new QHBoxLayout;


	
	
	
	picksTypeTBox = new QGroupBox( tr("Type"));
	QHBoxLayout  *layouttype=new QHBoxLayout;
	pickTypeBox = new QComboBox(picksTypeTBox);
	pickTypeBox->insertItem(0,tr("amplitude"));
	pickTypeBox->insertItem(1,tr("correlation"));
	pickTypeBox->setCurrentIndex(1);
  layouttype->addWidget(pickTypeBox);
  picksTypeTBox->setLayout(layouttype);
  	picksTypeTBox->setMinimumSize(QSize(40,25));

    
	picksCorTBox = new QGroupBox(tr("Len"));
	picksCorTBox->setMinimumSize(QSize(40,25));
    layouttype->setContentsMargins(2,2,2,2);

	QHBoxLayout  *layoutcor=new QHBoxLayout;
	picksCorBox = new QComboBox(picksCorTBox);
	picksCorBox->insertItem(0,tr("5"));
	picksCorBox->insertItem(1,tr("10"));
	picksCorBox->insertItem(2,tr("20"));
	picksCorBox->insertItem(3,tr("40"));
	picksCorBox->setCurrentIndex(2);
	layoutcor->addWidget(picksCorBox);
	picksCorTBox->setLayout(layoutcor);
	layoutcor->setContentsMargins(2,2,2,2);
		


	picksDistTBox = new QGroupBox(tr("MaxD"));
			QHBoxLayout  *layoutdist=new QHBoxLayout;
	picksMaxDist = new QComboBox(picksDistTBox);
		picksCorTBox->setMinimumSize(QSize(40,25));

		picksMaxDist->insertItem(0,tr("3"));
	picksMaxDist->insertItem(1,tr("10"));
	picksMaxDist->insertItem(2,tr("20"));
	picksMaxDist->insertItem(3,tr("30"));
	picksMaxDist->insertItem(4,tr("50"));
    picksMaxDist->setCurrentIndex(2);
  layoutdist->addWidget(picksMaxDist);
  picksDistTBox->setLayout(layoutdist);
	layoutdist->setContentsMargins(2,2,2,2);
	
	pickTypeBox->setToolTip( tr("Measure for comparison"));
	connect(pickTypeBox, SIGNAL(activated(const QString&)), this, 
	  SLOT(pickTypeBoxActivated(const QString&)));
	picksCorBox->setToolTip( tr("Correlation length to use"));
	connect(picksCorBox, SIGNAL(activated(const QString&)), this, 
	  SLOT(picksCorBoxActivated(const QString&)));
	picksMaxDist->setToolTip(tr("Maximum distance off straight line to search"));
	connect(picksMaxDist, SIGNAL(activated(const QString&)), this, 
	  SLOT(picksMaxDistActivated(const QString&)));


   	picksErrorBox = new QGroupBox(tr("Error"));
	QHBoxLayout  *layouterror=new QHBoxLayout;
	picksErrorPct = new QComboBox(picksErrorBox);
		picksErrorPct->setMinimumSize(QSize(40,25));

		picksErrorPct->insertItem(0,tr("0"));
		picksErrorPct->insertItem(1,tr("2"));
	picksErrorPct->insertItem(2,tr("4"));
	picksErrorPct->insertItem(3,tr("6"));
	picksErrorPct->insertItem(4,tr("10"));
	picksErrorPct->insertItem(5,tr("20"));


	connect(picksErrorPct, SIGNAL(activated(const QString&)), this, 
	
	
	  SLOT(picksErrorActivated(const QString&)));
	  picksErrorPct->setToolTip(tr("Allowable error in subsampling picks "));
	layouterror->addWidget(picksErrorPct);
	picksErrorBox->setLayout(layouterror);
	layouterror->setContentsMargins(2,2,2,2);




	pick2dBox = new QGroupBox( tr("Do"));
	QHBoxLayout  *layoutBox=new QHBoxLayout;
		  	layoutBox->setContentsMargins(2,2,2,2);

	pickButtonB = new QPushButton("pick",	pick2dBox);
	pickButtonB->setToolTip( tr("Pick visible planes"));
	connect(pickButtonB, SIGNAL(clicked()),this, SLOT(pickButtonClicked()));
	 pickButtonB->setMinimumSize(QSize(40,20));


	acceptButtonB = new QPushButton("Accept",	pick2dBox);
	acceptButtonB->setToolTip( tr("Accept all auto picks"));
	connect(acceptButtonB, SIGNAL(clicked()),this, SLOT(acceptButtonClicked()));
    acceptButtonB->setMinimumSize(QSize(40,20));


  declineButtonB = new QPushButton("Decline",	pick2dBox);
	declineButtonB->setToolTip( tr("Decline last auto pick"));
	connect(declineButtonB, SIGNAL(clicked()),this, SLOT(declineButtonClicked()));

	extendButtonB = new QPushButton("Extend",	pick2dBox);
	extendButtonB->setToolTip( tr("Extend picks between planes"));
	connect(extendButtonB, SIGNAL(clicked()),this, SLOT(extendButtonClicked()));
	 extendButtonB->setMinimumSize(QSize(40,20));

	
  layoutBox->addWidget(pickButtonB);
  layoutBox->addWidget(acceptButtonB);
   layoutBox->addWidget(declineButtonB);
  layoutBox->addWidget(extendButtonB);
  pick2dBox->setLayout(layoutBox);
  pick2dBox->setMinimumSize(QSize(140,20));


    methodTypeBox2d = new QGroupBox( tr("Method"));
    
	QHBoxLayout  *layoutmeth2d=new QHBoxLayout;
			  	layoutmeth2d->setContentsMargins(2,2,2,2);

	methodBox2d = new QComboBox(methodTypeBox2d);
	methodBox2d->insertItem(0,tr("Viterbi"));
	methodBox2d->insertItem(1,tr("Brown"));
	methodBox2d->setCurrentIndex(0);
    layoutmeth2d->addWidget(methodBox2d);
    methodTypeBox2d->setLayout(layoutmeth2d);
    methodTypeBox2d->setMinimumSize(QSize(40,25));
	connect(methodBox2d, SIGNAL(activated(const QString&)), this, 
	  SLOT(methodActivated(const QString&)));
	  methodBox2d->setToolTip(tr("Picking method "));
	  
	  

    planeTypeBox2d = new QGroupBox( tr("Plane"));
	QHBoxLayout  *layoutplane2d=new QHBoxLayout;
				  	layoutplane2d->setContentsMargins(2,2,2,2);

	planeBox2d = new QComboBox();
	planeBox2d->insertItem(0,tr("In-out"));
	planeBox2d->insertItem(1,tr("Top-bottom"));
	planeBox2d->insertItem(1,tr("Left-Right"));
	planeBox2d->setCurrentIndex(1);
    layoutplane2d->addWidget(planeBox2d);
    planeTypeBox2d->setLayout(layoutplane2d);
  	planeTypeBox2d->setMinimumSize(QSize(40,25));
	connect(planeBox2d, SIGNAL(activated(const QString&)), this, 
	  SLOT(planeActivated(const QString&)));
	planeBox2d->setToolTip(tr("Plane to extend along"));



	  
	
}

void autopick_menu::layout() {
//	picksBoxLayout->addWidget(picksVisibleBox);
	//lay->addWidget(picksFileBox);
		lay->addWidget(picksTypeTBox);
			lay->addWidget(picksCorTBox);
				lay->addWidget(picksDistTBox);

					lay->addWidget(picksErrorBox);

	lay->addStretch(1);
	lay2d->addWidget(pick2dBox );
	
	lay2d->addWidget(methodTypeBox2d);
	lay2d->addWidget(planeTypeBox2d);


//	layB->addWidget(picksFileBoxB);
	lay2d->addStretch(1);
  Widget2d->setLayout(lay2d);
  mainWidget->setLayout(lay);
//  Widget2d->setLayout(layB);
		this->addTab(mainWidget,"General");
 	this->addTab(Widget2d,"2-D");

		//this->addTab(Widget3d,"Extend");
			//	this->addTab(flatWidget,"Lloyd");
}


void autopick_menu::pickTypeBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
  com.push_back(tr("type")); com.push_back(text);
  if(!(text.contains("hoose")>0)) send_message(com);

}
void autopick_menu::picksCorBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("cor")); com.push_back(text);
	if(!(text.contains("hoose")>0)) 	send_message(com);

}
void autopick_menu::picksMaxDistActivated(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("dist")); com.push_back(text);
	if(!(text.contains("hoose")>0)) 	send_message(com);

}
void autopick_menu::picksErrorActivated(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("error_lloyd")); com.push_back(text);
		send_message(com);

}

void autopick_menu::pickButtonClicked() {


	 std::vector <QString> com; com.push_back("auto");
	 com.push_back(tr("pick"));
	  com.push_back(tr("run2d"));
	    com.push_back(tr("do"));
	    	send_message(com);

}
void autopick_menu::acceptButtonClicked() {

	 std::vector <QString> com; com.push_back("auto");
	 com.push_back(tr("pick"));
	  com.push_back(tr("accept"));
	    com.push_back(tr("do"));
	    	send_message(com);

}
void autopick_menu::declineButtonClicked() {

	 std::vector <QString> com; com.push_back("auto");
	 com.push_back(tr("pick"));
	  com.push_back(tr("decline"));
	    com.push_back(tr("do"));
	    	send_message(com);

}
void autopick_menu::extendButtonClicked() {

	 std::vector <QString> com; com.push_back("auto");
	 com.push_back(tr("pick"));
	  com.push_back(tr("extend"));
	    com.push_back(tr("do"));
	    	send_message(com);

}



void autopick_menu::planeActivated(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("plane")); com.push_back(text);
		send_message(com);

}
void autopick_menu::methodActivated(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("method")); com.push_back(text);
		send_message(com);

}
