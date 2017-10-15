#include "main_menu.h"

#include <QPixmap>



main_menu::main_menu(datasets *dats, QString num,int nd)
{
  this->set_basics(dats,num,"Main",nd);
  mainWidget=new QWidget(this,"main-main");
  infoWidget = new QWidget(this, "load-main");
  loadWidget = new QWidget(this, "load-main");
  
}



void main_menu::createit() {
	lay = new QHBoxLayout;
	lay2 = new QHBoxLayout;
	layi = new QHBoxLayout;

//Main tab
	mainButtonGroup = new QGroupBox(tr("Lock views"));
	QHBoxLayout *layoutmb=new QHBoxLayout;

	mainyesButton = new QRadioButton(tr("Yes"), mainButtonGroup, tr("main locked"));
	mainnoButton = new QRadioButton(tr("No"), mainButtonGroup, tr("main not locked"));
	if (!mainyesButton->isOn()) mainnoButton->toggle();
	QToolTip::add(mainyesButton, tr("Yes- Lock views for color and view type"));
	QToolTip::add(mainnoButton, tr("NO -- Unlock views for color and view type"));
  layoutmb->addWidget(mainyesButton);
  layoutmb->addWidget(mainnoButton);
  mainButtonGroup->setLayout(layoutmb);
  layoutmb->setContentsMargins(2,2,2,2);

	actionButtonGroup = new QGroupBox(tr("Main actions"));
	QHBoxLayout *layoutab = new QHBoxLayout;
	connect(mainnoButton, SIGNAL(clicked()), this, SLOT(mainNoButtonClicked()));
	connect(mainyesButton, SIGNAL(clicked()), this, SLOT(mainYesButtonClicked()));
	
	
	
    attachButtonGroup = new QGroupBox(tr("Attach views"));
	QHBoxLayout *layouta=new QHBoxLayout;

	attachyesButton = new QRadioButton(tr("Yes"), attachButtonGroup, tr("main locked"));
	attachnoButton = new QRadioButton(tr("No"), attachButtonGroup, tr("main not locked"));
	if (!attachnoButton->isOn()) attachyesButton->toggle();
	QToolTip::add(attachyesButton, tr("Yes- All views in one window"));
	QToolTip::add(attachnoButton, tr("NO -- All views in different windows"));
  layouta->addWidget(attachyesButton);
  layouta->addWidget(attachnoButton);
  attachButtonGroup->setLayout(layouta);
  layouta->setContentsMargins(2,2,2,2);

	connect(attachnoButton, SIGNAL(clicked()), this, SLOT(attachNoButtonClicked()));
	connect(attachyesButton, SIGNAL(clicked()), this, SLOT(attachYesButtonClicked()));
	
	
	
	
	
	mainQuitButton = new QPushButton(QIcon(qPixmapFromMimeSource("stop.png")),
		0, actionButtonGroup, tr("mainQuitButton - MenuBar"));
	mainQuitButton->setFixedWidth(BUTTON_SIZE);
        QToolTip::add(mainQuitButton, tr("QUIT -- Quit the program."));

	connect(mainQuitButton, SIGNAL(clicked()), this, SLOT(mainQuitButtonClicked()));
	
	layoutab->addWidget(mainQuitButton);
	actionButtonGroup->setLayout(layoutab);
	layoutab->setContentsMargins(2,2,2,2);

  mainMouseGroup = new QGroupBox(tr("Mouse actions"));
	QHBoxLayout *layoutm=new QHBoxLayout;
	mouseBox= new QComboBox(mainMouseGroup, vnum+tr("-facesComboBox - view_menu"));
	mouseBox->insertItem(tr("zoom"));
	mouseBox->insertItem(tr("navigate"));
	mouseBox->insertItem(tr("pick"));
//		mouseBox->insertItem(tr("region"));

	mouseBox->setMinimumSize(QSize(40,20));


	QToolTip::add(mouseBox, tr("Choose mouse control actions"));
	connect(mouseBox, SIGNAL(activated(const QString&)),this, SLOT(mouseBoxActivated(const QString&)));
	layoutm->addWidget(mouseBox);
	layoutm->setContentsMargins(2,2,2,2);
	mainMouseGroup->setLayout(layoutm);
	
	//
	mainStatusGroup = new QGroupBox(tr("Staus"));
	QHBoxLayout *layoutss=new QHBoxLayout;
	statusBox= new QComboBox();
	statusBox->insertItem(tr("mouse"));
	statusBox->insertItem(tr("position"));
	statusBox->insertItem(tr("value"));
		statusBox->insertItem(tr("picking"));


	statusBox->setMinimumSize(QSize(40,20));


	QToolTip::add(statusBox, tr("Choose what to view in the status bar"));
	connect(statusBox, SIGNAL(activated(const QString&)),this, SLOT(statusBoxActivated(const QString&)));
	layoutss->addWidget(statusBox);
	layoutss->setContentsMargins(2,2,2,2);
	mainStatusGroup->setLayout(layoutss);
	
	
	
	
  mainBox = new QGroupBox( tr("History"));
	QHBoxLayout  *layout=new QHBoxLayout;

	connect(mainnoButton, SIGNAL(clicked()), this, SLOT(mainNoButtonClicked()));
	connect(mainyesButton, SIGNAL(clicked()), this, SLOT(mainYesButtonClicked()));
        
     historySaveButton = new QPushButton(QIcon(qPixmapFromMimeSource("save.png")),
		0,mainBox, tr("historySaveButton - pick_menu"));
	historySaveButton->setFixedWidth(BUTTON_SIZE);
	QToolTip::add(historySaveButton, tr("SAVE -- Save history file."));
	historyLoadButton = new QPushButton(QIcon(qPixmapFromMimeSource("open.png")),
		0, mainBox, tr("picksLoadButton - pick_menu"));
	QToolTip::add(historyLoadButton, tr("LOAD -- Load and run history file"));
	connect(historySaveButton, SIGNAL(clicked()), this, SLOT(historySaveButtonClicked()));
	connect(historyLoadButton, SIGNAL(clicked()), this, SLOT(historyLoadButtonClicked()));
 
	layout->addWidget(historySaveButton);
	layout->addWidget(historyLoadButton);
	mainBox->setLayout(layout);
	layout->setContentsMargins(2,2,2,2);
	
//	Load Tab
	loadButtonGroup = new QGroupBox(tr("Load file"));
	QHBoxLayout *layoutlb = new QHBoxLayout;
	
	connect(mainnoButton, SIGNAL(clicked()), this, SLOT(mainNoButtonClicked()));
	connect(mainyesButton, SIGNAL(clicked()), this, SLOT(mainYesButtonClicked()));
	
	loadButton = new QPushButton(QIcon(qPixmapFromMimeSource("open.png")),
		0, loadButtonGroup, tr("loadButton - MenuBar"));
	QToolTip::add(loadButton, tr("Load File -- Load new data file."));
	//loadButton->setEnabled(false);	
		
	connect(loadButton, SIGNAL(clicked()), this, SLOT(loadButtonClicked()));

	
	layoutlb->addWidget(loadButton);
	loadButtonGroup->setLayout(layoutlb);
	layoutlb->setContentsMargins(2,2,2,2);
	
	
	fileGroup = new QGroupBox(tr("File type"));
	QHBoxLayout *layoutft = new QHBoxLayout;
	
	
	connect (mainnoButton, SIGNAL(clicked()), this, SLOT(mainNoButtonClicked()));
	connect (mainyesButton, SIGNAL(clicked()), this, SLOT(mainYesButtonClicked()));
	
	dataTypeComboBox = new QComboBox;
	dataTypeComboBox->insertItem(tr("SEP-Byte"));
	dataTypeComboBox->insertItem(tr("SEP-Float"));
	dataTypeComboBox->insertItem(tr("SEGY-Float"));
	dataTypeComboBox->insertItem(tr("SU-Float"));
	dataTypeComboBox->insertItem(tr("SEISPAK-Float"));
	dataTypeComboBox->setMinimumSize(QSize(40,20));
	QToolTip::add(dataTypeComboBox, tr("File type -- Pick the kind of file."));

	connect(dataTypeComboBox, SIGNAL(activated(const QString&)), this, SLOT(enableDataFileLoadButton()));

	layoutft->addWidget(dataTypeComboBox);
	fileGroup->setLayout(layoutft);
	layoutft->setContentsMargins(2,2,2,2);
	
	storageGroup = new QGroupBox(tr("Storage"));
	QHBoxLayout * layouts = new QHBoxLayout;
	
	connect (mainnoButton, SIGNAL(clicked()), this, SLOT(mainNoButtonClicked()));
	connect (mainyesButton, SIGNAL(clicked()), this, SLOT(mainYesButtonClicked()));
	
	storageTypeComboBox = new QComboBox;
	storageTypeComboBox->insertItem("incore");
	storageTypeComboBox->insertItem("out-core");
	storageTypeComboBox->insertItem("partial");
	QToolTip::add(storageTypeComboBox, tr("Storage -- Pick dataset storage."));
	
	connect(storageTypeComboBox, SIGNAL(activated(const QString&)), this, SLOT(enableDataFileLoadButton()));

	
	layouts->addWidget(storageTypeComboBox);
	storageGroup->setLayout(layouts);
	layouts->setContentsMargins(2,2,2,2);
}

void main_menu::layout() {
	lay->addWidget(mainButtonGroup);
    lay->addWidget(attachButtonGroup);
	lay->addWidget(actionButtonGroup);
	layi->addWidget(mainMouseGroup);
    layi->addWidget(mainStatusGroup);

	lay->addStretch(1);
	mainWidget->setLayout(lay);
	
//	lay2->addWidget(loadButtonGroup);
	lay2->addWidget(mainBox);
//	lay2->addWidget(fileGroup);
//	lay2->addWidget(storageGroup);
	lay2->addStretch(1);
		layi->addStretch(1);

	infoWidget->setLayout(layi);
	loadWidget->setLayout(lay2);
	
		this->addTab(mainWidget,"Main");
				this->addTab(infoWidget, "Info");

		this->addTab(loadWidget, "History");
	

}
void main_menu::historySaveButtonClicked() {


  QString filename= QFileDialog::getSaveFileName(this);

	 std::vector <QString> com; com.push_back("main");
	 com.push_back(tr("save")); com.push_back(filename);
	if ( !filename.isNull() )  
	    	send_message(com);

}
void main_menu::historyLoadButtonClicked() {
  
	QString filename = QFileDialog::getOpenFileName(this);
	 std::vector <QString> com; com.push_back("main");
	 com.push_back(tr("open")); com.push_back(filename);
	if ( !filename.isNull() )  
	    	send_message(com);

}
void main_menu::mainNoButtonClicked() {
  std::vector<QString> com; com.push_back(tr("main"));
  com.push_back(tr("locked")); com.push_back(tr("no"));
	send_message(com);
}
void main_menu::mouseBoxActivated(QString val) {
  std::vector<QString> com;com.push_back("menu"); com.push_back(tr("main"));
  com.push_back(tr("mouse")); com.push_back(val);
	send_message(com);
}
void main_menu::statusBoxActivated(QString val) {
  std::vector<QString> com;com.push_back("menu"); com.push_back(tr("main"));
  com.push_back(tr("status")); com.push_back(val);
	send_message(com);
}
void main_menu::mainYesButtonClicked() {
  std::vector<QString> com; com.push_back(tr("main"));
  com.push_back(tr("locked")); com.push_back(tr("yes"));
	send_message(com);
}
void main_menu::attachNoButtonClicked() {
  std::vector<QString> com; com.push_back(tr("main"));
  com.push_back(tr("attach")); com.push_back(tr("false"));
	send_message(com);
}

void main_menu::attachYesButtonClicked() {
  std::vector<QString> com; com.push_back(tr("main"));
  com.push_back(tr("attach")); com.push_back(tr("true"));
	send_message(com);
}

void main_menu::mainQuitButtonClicked() {
  std::vector<QString> com; com.push_back(tr("main"));
  com.push_back(tr("quit"));
	send_message(com);
}


void main_menu::loadButtonClicked() {
    std::vector<QString> com; com.push_back(tr("main"));
  com.push_back(tr("loadFile")); 
	QString file = QFileDialog::getOpenFileName(this);
	com.push_back(file);
	QString fileType = dataTypeComboBox->currentText();
	com.push_back (fileType);
	QString fileStorage = storageTypeComboBox->currentText();
	
	com.push_back(fileStorage);
if ( !file.isNull() )  
	  send_message(com);
}

void main_menu::enableDataFileLoadButton(){
	if (storageTypeComboBox->currentText() == "Choose..." || dataTypeComboBox->currentText() == "Choose...")
		loadButton->setEnabled(false);
	else
		loadButton->setEnabled(true);
}
