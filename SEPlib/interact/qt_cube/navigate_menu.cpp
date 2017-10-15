#include "navigate_menu.h"
#include <QPixmap>


navigate_menu::navigate_menu(datasets* dats, QString num,int nd)
{
  this->set_basics(dats,num,"Navigate",nd);
  movieWidget=new QWidget(this,"movie widget");
  dirWidget  =new QWidget(this,"direction widget");
 }

void navigate_menu::createit() {
// Layout for the movie tab.
	layMovie = new QHBoxLayout;
//Layout for the Direction tab.
	layDir = new QHBoxLayout;

	movieButtonGroup = new QGroupBox(tr("Movie Playing"));
	QHBoxLayout *layoutmovie2=new QHBoxLayout;
	layoutmovie2->setContentsMargins(2,2,2,2);


	movieGoButton = new QRadioButton(tr("Go"), movieButtonGroup, tr("movieGoButton - ControlBar"));
	movieNoButton = new QRadioButton(tr("No"), movieButtonGroup, tr("movieNoButton - ControlBar"));
	movieGoButton->setMinimumSize(QSize(40,25));
	movieNoButton->setMinimumSize(QSize(40,25));

	if (!movieNoButton->isOn()) movieNoButton->toggle();
	stepButton = new QPushButton(QIcon(qPixmapFromMimeSource("forward.png")),
		0, movieButtonGroup, tr("stepButton - ControlBar"));
			stepButton->setMinimumSize(QSize(40,25));
	layoutmovie2->addWidget(movieGoButton);
	layoutmovie2->addWidget(movieNoButton);
	layoutmovie2->addWidget(stepButton);
	movieButtonGroup->setLayout(layoutmovie2);
	QToolTip::add(stepButton, tr("STEP -- Step along current movie direction."));
	QToolTip::add(movieGoButton, tr("GO -- Play movie along currently selected direction."));
	QToolTip::add(movieNoButton, tr("NO -- Pause the movie."));
	connect(movieGoButton, SIGNAL(clicked()), this, SLOT(movieGoButtonClicked()));
	connect(movieNoButton, SIGNAL(clicked()), this, SLOT(movieNoButtonClicked()));
	connect(stepButton, SIGNAL(clicked()), this, SLOT(stepButtonClicked()));

	speedBox = new QGroupBox(tr("Movie Speed"));
	 	QHBoxLayout  *layoutspeed=new QHBoxLayout;
	layoutspeed->setContentsMargins(2,2,2,2);


	speedSlider = new QSlider(Qt::Horizontal, speedBox, tr("speedSlider - ControlBar"));
	speedSlider->setRange(10,1000);
	speedSlider->setValue(100);
	speedSlider->setMinimumSize(QSize(40,25));

	QToolTip::add(speedSlider, tr("SPEED -- Adjust the movie speed."));
	connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(setMovieSpeed (int)));
  layoutspeed->addWidget(speedSlider);
  speedBox->setLayout(layoutspeed);

	movieDirButtonGroup = new QGroupBox(tr("Movie Direction"));
		QHBoxLayout  *layoutmovie=new QHBoxLayout;
			layoutmovie->setContentsMargins(2,2,2,2);

	smallZButton = new QRadioButton(tr("-Z"), movieDirButtonGroup, tr("smallZButton - ControlBar"));
	if (!smallZButton->isOn()) smallZButton->toggle();
	largeZButton = new QRadioButton(tr("+Z"), movieDirButtonGroup, tr("largeZButton - ControlBar"));
	smallXButton = new QRadioButton(tr("-X"), movieDirButtonGroup, tr("smallXButton - ControlBar"));
	largeXButton = new QRadioButton(tr("+X"), movieDirButtonGroup, tr("largeXButton - ControlBar"));
	smallYButton = new QRadioButton(tr("-Y"), movieDirButtonGroup, tr("smallYButton - ControlBar"));
	largeYButton = new QRadioButton(tr("+Y"), movieDirButtonGroup, tr("largeYButton - ControlBar"));

	layoutmovie->addWidget(smallZButton);
	layoutmovie->addWidget(largeZButton);
	layoutmovie->addWidget(smallXButton);
	layoutmovie->addWidget(largeXButton);
	layoutmovie->addWidget(smallYButton);
	layoutmovie->addWidget(largeYButton);
	movieDirButtonGroup->setMinimumSize(QSize(70,20));

	movieDirButtonGroup->setLayout(layoutmovie);
	 connect(largeZButton, SIGNAL(clicked()), this, SLOT(largeZButtonClicked()));
        connect(smallZButton, SIGNAL(clicked()), this, SLOT(smallZButtonClicked()));
        connect(largeXButton, SIGNAL(clicked()), this, SLOT(largeXButtonClicked()));
        connect(smallXButton, SIGNAL(clicked()), this, SLOT(smallXButtonClicked()));
        connect(largeYButton, SIGNAL(clicked()), this, SLOT(largeYButtonClicked()));
        connect(smallYButton, SIGNAL(clicked()), this, SLOT(smallYButtonClicked()));
	QToolTip::add(smallZButton, tr("NEAR -- Move nearer along deep axis."));
	QToolTip::add(largeZButton, tr("FAR -- Move farther away along deep axis."));
	QToolTip::add(smallXButton, tr("LEFT -- Move left along horizontal axis."));
	QToolTip::add(largeXButton, tr("RIGHT -- Move right along horizontal axis."));
	QToolTip::add(smallYButton, tr("DOWN -- Move down along vertical axis."));
	QToolTip::add(largeYButton, tr("UP -- Move up along vertical axis."));

}

void navigate_menu::layout() {
	layMovie->addWidget(movieButtonGroup);
	layMovie->addWidget(speedBox);
	layDir->addWidget(movieDirButtonGroup);
	layMovie->addStretch(1);
  layDir->addStretch(1);
  movieWidget->setLayout(layMovie);
  dirWidget->setLayout(layDir);
    this->addTab(movieWidget,"Movie");
    this->addTab(dirWidget,"Direction");
}

/*
 * The following methods report an action to Updater in the form of a vector
 * storing a string that identifies the action.
 */
void navigate_menu::movieGoButtonClicked() {
std::vector<QString> com; com.push_back(tr("navigate"));
	com.push_back(tr("movie")); com.push_back(tr("go"));
	QString speed = QString::number(movieSpeed);
	com.push_back(speed);
	send_message(com);
}

void navigate_menu::movieNoButtonClicked() {
std::vector<QString> com; com.push_back(tr("navigate"));
	com.push_back(tr("movie")); com.push_back(tr("stop"));
	send_message(com);

}

void navigate_menu::stepButtonClicked(){
  std::vector<QString> com; com.push_back(tr("navigate"));
	com.push_back(tr("step")); 
	send_message(com);
}

void navigate_menu::largeZButtonClicked(){
  std::vector<QString> com; com.push_back(tr("navigate"));
	com.push_back(tr("direction")); com.push_back(tr("Z"));
	send_message(com);
	
}

void navigate_menu::smallZButtonClicked(){
  std::vector<QString> com; com.push_back(tr("navigate"));
	com.push_back(tr("direction")); com.push_back(tr("z"));
	send_message(com);

}

void navigate_menu::largeXButtonClicked(){
std::vector<QString> com; com.push_back(tr("navigate"));
 	com.push_back(tr("direction")); com.push_back(tr("X"));
	send_message(com);

}

void navigate_menu::smallXButtonClicked(){
std::vector<QString> com; com.push_back(tr("navigate"));
	com.push_back(tr("direction")); com.push_back(tr("x"));
	send_message(com);

}

void navigate_menu::largeYButtonClicked(){
std::vector<QString> com; com.push_back(tr("navigate"));
	com.push_back(tr("direction")); com.push_back(tr("Y"));
	send_message(com);
}

void navigate_menu::smallYButtonClicked(){
  std::vector<QString> com; com.push_back(tr("navigate"));
  com.push_back(tr("direction")); com.push_back(tr("y"));
	send_message(com);

}

/*	
	Sets the speed for the movie.
*/
void navigate_menu::setMovieSpeed(int speed)
{
	movieSpeed = speed;
}

