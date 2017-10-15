#include "color_menu.h"
#include "slice_types.h"
#include "my_colors.h"
color_menu::color_menu(datasets * dats, QString num,int nd)
{
  this->set_basics(dats,num,"Color",nd);
  colorWidget=new QWidget(this);
  wiggleWidget=new QWidget(this);
  contourWidget=new QWidget(this);

}
void color_menu::createit() {
	lay = new QHBoxLayout;
	lay_wig = new QHBoxLayout;
		lay_con= new QHBoxLayout;

	colorCubeBox = new QGroupBox(tr("Data"));
		QHBoxLayout  *layoutcube=new QHBoxLayout;
//Sets the color the data will be displayed in. Wiggle displays the data in wiggle form.

		colorCubeComboBox = new QComboBox(colorCubeBox);
	slice_types x=slice_types();
	for(int i=0; i < (int)x.names.size(); i++)colorCubeComboBox->addItem(x.names[i]);
	colorCubeComboBox->setToolTip(tr("DATA -- Set the color map for the cubes."));
	connect(colorCubeComboBox, SIGNAL(activated(const QString&)), this, SLOT(colorCubeComboBoxActivated(const QString&)));
  layoutcube->addWidget(colorCubeComboBox);
  colorCubeBox->setLayout(layoutcube);
  colorCubeComboBox->setMinimumSize(QSize(40,25));
 layoutcube->setContentsMargins(2,2,2,2);

	// overlay
	colorOverBox = new QGroupBox( tr("Lines"));
	 	QHBoxLayout  *layoutover=new QHBoxLayout;

	colorOverComboBox = new QComboBox(colorOverBox);
	colorOverComboBox->setMinimumSize(QSize(40,25));
    my_colors cols=my_colors();
    std::vector<QString> colors=cols.return_cvec();
    for(int i=0; i< (int) colors.size(); i++)
      colorOverComboBox->addItem(colors[i]);

	colorOverComboBox->setCurrentIndex(0); // green
	colorOverComboBox->setToolTip( tr("OVERLAY -- Set the color for the overlay."));
	connect(colorOverComboBox, SIGNAL(activated(const QString&)), this, SLOT(colorOverComboBoxActivated(const QString&)));
  layoutover->addWidget(colorOverComboBox);
	layoutover->setContentsMargins(2,2,2,2);
  colorOverBox->setLayout(layoutover);

	// background
	colorBgBox = new QGroupBox(tr("Back"));
		QHBoxLayout  *layoutbg=new QHBoxLayout;


	colorBgComboBox = new QComboBox(colorBgBox);
    colorBgComboBox->setMinimumSize(QSize(40,25));
    for(int i=0; i< (int) colors.size(); i++)
      colorBgComboBox->addItem(colors[i]);


  layoutbg->addWidget(colorBgComboBox);
  colorBgBox->setLayout(layoutbg);
	layoutbg->setContentsMargins(2,2,2,2);

  
	colorBgComboBox->setToolTip(tr("BACKGROUND -- Set the color for the background."));
	connect(colorBgComboBox, SIGNAL(activated(const QString&)),this, SLOT(colorBgComboBoxActivated(const QString&)));

	// The wiggle group box.
	colorWigBox = new QGroupBox(tr("Color"));
		QHBoxLayout  *layoutwig=new QHBoxLayout;
	layoutwig->setContentsMargins(2,2,2,2);
	colorWigComboBox = new QComboBox(colorWigBox);
    colorWigComboBox->setMinimumSize(QSize(40,25));
    for(int i=0; i< (int) colors.size(); i++)
      colorWigComboBox->addItem(colors[i]);
	colorWigComboBox->setMinimumSize(QSize(40,25));
  layoutwig->addWidget(colorWigComboBox);
  colorWigBox->setLayout(layoutwig);
	colorWigComboBox->setToolTip(tr("Wiggle -- Set the color for wiggle."));
	connect(colorWigComboBox, SIGNAL(activated(const QString&)),this, SLOT(colorWigComboBoxActivated(const QString&)));
	
		// The wiggle group box.
	posWigBox = new QGroupBox(tr("Positive fill"));
		QHBoxLayout  *layoutpos=new QHBoxLayout;
	layoutpos->setContentsMargins(2,2,2,2);
	posWigComboBox = new QComboBox(posWigBox);
    posWigComboBox->setMinimumSize(QSize(40,25));
    posWigComboBox->addItem(tr("none"));

    for(int i=0; i< (int) colors.size(); i++)
      posWigComboBox->addItem(colors[i]);
	posWigComboBox->setMinimumSize(QSize(40,25));
  layoutpos->addWidget(posWigComboBox);
  posWigBox->setLayout(layoutpos);
	posWigComboBox->setToolTip(tr("Wiggle -- Set the positive  fill color."));
	connect(posWigComboBox, SIGNAL(activated(const QString&)),this, SLOT(posWigComboBoxActivated(const QString&)));
	
		// The wiggle group box.
	negWigBox = new QGroupBox(tr("Negative fill"));
		QHBoxLayout  *layoutneg=new QHBoxLayout;
	layoutneg->setContentsMargins(2,2,2,2);
	negWigComboBox = new QComboBox(negWigBox);
    negWigComboBox->setMinimumSize(QSize(40,25));
          negWigComboBox->addItem(tr("none"));

    for(int i=0; i< (int) colors.size(); i++)
      negWigComboBox->addItem(colors[i]);
	negWigComboBox->setMinimumSize(QSize(40,25));
  layoutneg->addWidget(negWigComboBox);
  negWigBox->setLayout(layoutneg);
	negWigComboBox->setToolTip(tr("Wiggle -- Set the negative  fill color."));
	connect(negWigComboBox, SIGNAL(activated(const QString&)),this, SLOT(negWigComboBoxActivated(const QString&)));
	
	
	//Contour
	colorContourBox = new QGroupBox(tr("Color"));
	QHBoxLayout *layoutCont = new QHBoxLayout;
	layoutCont->setContentsMargins(2,2,2,2);
	colorContComboBox = new QComboBox(colorContourBox);
	for(int i=0; i< (int) colors.size(); i++)
		colorContComboBox->addItem(colors[i]);
	colorContComboBox->setMinimumSize(QSize(40,25));
	
	layoutCont->addWidget(colorContComboBox);
	colorContourBox->setLayout(layoutCont);

	colorContComboBox->setToolTip(tr("Contour -- Set the color for the contour lines."));
	connect(colorContComboBox, SIGNAL(activated(const QString&)),this, SLOT(colorContComboBoxActivated(const QString&)));
	
	conNumberBox = new QGroupBox(tr("Number"));
	QHBoxLayout *layoutCont2 = new QHBoxLayout;
	layoutCont2->setContentsMargins(2,2,2,2);
	numberContComboBox = new QComboBox();
		numberContComboBox->addItem("2");numberContComboBox->addItem("3");
		numberContComboBox->addItem("4");numberContComboBox->addItem("5");
		numberContComboBox->addItem("6");numberContComboBox->addItem("7");
		numberContComboBox->addItem("8");numberContComboBox->addItem("10");
		numberContComboBox->addItem("15");
		numberContComboBox->addItem("20");numberContComboBox->addItem("25");
	numberContComboBox->setMinimumSize(QSize(40,25));
	numberContComboBox->setToolTip(tr("Contour Number -- Set the number of contour lines."));
	layoutCont2->addWidget(numberContComboBox);
	connect(numberContComboBox, SIGNAL(activated(const QString&)),this,
	  SLOT(numberContComboBoxActivated(const QString&)));
	conNumberBox->setLayout(layoutCont2);
	
	c0NumberBox = new QGroupBox(tr("First"));
	QHBoxLayout *layoutc0 = new QHBoxLayout;
	layoutc0->setContentsMargins(2,2,2,2);
	c0Box = new QLineEdit();
	c0Box->setMaxLength(8);	
	c0Box->setMinimumSize(QSize(40,25));
	c0Box->setToolTip(tr("First contour value"));
	layoutc0->addWidget(c0Box);
	connect(c0Box, SIGNAL(textChanged(const QString&)),this,
	  SLOT(c0ContComboBoxActivated(const QString&)));
	c0NumberBox->setLayout(layoutc0);
	
	
		
	dcNumberBox = new QGroupBox(tr("Sampling"));
	QHBoxLayout *layoutdc = new QHBoxLayout;
	layoutdc->setContentsMargins(2,2,2,2);
	dcBox = new QLineEdit();
	dcBox->setMaxLength(8);	
	dcBox->setMinimumSize(QSize(40,25));
	dcBox->setToolTip(tr("First contour value"));
	layoutdc->addWidget(dcBox);
	connect(dcBox, SIGNAL(textChanged(const QString&)),this,
	  SLOT(dcContComboBoxActivated(const QString&)));
	dcNumberBox->setLayout(layoutdc);
	
	
	
	colorOverCBox = new QGroupBox( tr("Overlay"));
 	QHBoxLayout  *layoutco=new QHBoxLayout;

	//colorCubeBox->setInsideMargin(LAYOUT_MARGIN_SMALL);
	//colorCubeBox->setInsideSpacing(LAYOUT_SPACING_SMALL);
	colorOverCComboBox = new QComboBox();
	for(int i=0; i < (int)x.names.size(); i++)
	  colorOverCComboBox->addItem(x.names[i]);
	colorOverCComboBox->setToolTip( tr("Overlaymap -- Set the color map for the overlay"));
	connect(colorOverCComboBox, SIGNAL(activated(const QString&)),this, SLOT(colorOverCComboBoxActivated(const QString&)));
	layoutco->addWidget(colorOverCComboBox);
	colorOverCBox->setLayout(layoutco);
  	layoutco->setContentsMargins(2,2,2,2);
	colorOverCComboBox->setMinimumSize(QSize(20,20));

		speedBox = new QGroupBox(tr("Opacity"));
			QHBoxLayout *layouts=new QHBoxLayout;
				  	layouts->setContentsMargins(2,2,2,2);


	//speedBox->setInsideMargin(LAYOUT_MARGIN_SMALL);
	//speedBox->setInsideSpacing(LAYOUT_SPACING_SMALL);
	speedSlider = new QSlider();

	speedSlider->setRange(0,255);
	speedSlider->setValue(128);

	layouts->addWidget(speedSlider);
	speedBox->setLayout(layouts);

	speedSlider->setMinimumSize(QSize(30,20));

	speedSlider->setToolTip(tr("Opacity -- Adjust the opacity of the overlay"));
	connect(speedSlider, SIGNAL(valueChanged(int)), this, SLOT(setAlpha(int)));

	
	/*
	  	QLineEdit* c0Box,dcBox;
  	QGroupBox* dcNumberBox;
  	  	QGroupBox* c0NumberBox;
*/
	
	
}

void color_menu::layout() {
	lay->addWidget(colorCubeBox);
		  lay->addWidget(colorOverCBox);
      lay->addWidget(speedBox);
	lay->addWidget(colorOverBox);
	lay->addWidget(colorBgBox);
	lay_wig->addWidget(colorWigBox);
		lay_wig->addWidget(posWigBox);
			lay_wig->addWidget(negWigBox);
	lay_con->addWidget(colorContourBox);
	lay_con->addWidget(conNumberBox);
		lay_con->addWidget(c0NumberBox);
		lay_con->addWidget(dcNumberBox);
	//pcolorBoxLayout->addWidget(colorMarkBox);
//	lay->addWidget(colorBox2);
	lay->addStretch(1);
	lay_wig->addStretch(1);
		lay_con->addStretch(1);

	colorWidget->setLayout(lay);
	wiggleWidget->setLayout(lay_wig);
  contourWidget->setLayout(lay_con);

	this->addTab(colorWidget,"General");
	this->addTab(wiggleWidget,"Wiggle");
		this->addTab(contourWidget,"Contour");
}

void color_menu::colorCubeComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
  com.push_back(tr("colortable")); com.push_back(text);
  	send_message(com);

}

void color_menu::colorOverComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
	com.push_back(tr("line_color")); com.push_back(text);
		send_message(com);
}

void color_menu::colorBgComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
    com.push_back(tr("background")); com.push_back(text);
    	send_message(com);

}
void color_menu::colorWigComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
    com.push_back(tr("wiggle_color")); com.push_back(text);
    	send_message(com);

}
void color_menu::posWigComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
    com.push_back(tr("wiggle_pos")); com.push_back(text);
    	send_message(com);

}
void color_menu::negWigComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
    com.push_back(tr("wiggle_neg")); com.push_back(text);
    	send_message(com);

}

void color_menu::colorContComboBoxActivated(const QString& text) {
	std::vector<QString> com; com.push_back(tr("color"));
	  com.push_back(tr("contour_color")); com.push_back(text);
		send_message(com);
}

void color_menu:: numberContComboBoxActivated(const QString& text) {
	std::vector<QString> com; com.push_back(tr("color"));
	  com.push_back(tr("number of lines")); com.push_back(text);
	    send_message(com);
}
void color_menu:: dcContComboBoxActivated(const QString& text) {
	std::vector<QString> com; com.push_back(tr("color"));
	 bool ok;
	 double d = QString( text ).toDouble(&ok); // ok == false

	  com.push_back(tr("contour_dc")); com.push_back(text);
	if(ok)send_message(com);
			  if(d==0.);

}
void color_menu:: c0ContComboBoxActivated(const QString& text) {
	std::vector<QString> com; com.push_back(tr("color"));
	bool ok;
	double d = QString( text ).toDouble(&ok); // ok == false
	  com.push_back(tr("contour_c0")); com.push_back(text);
	  if(ok) send_message(com);
	  if(d==0.);
}
void color_menu::setAlpha(int alpha){
std::vector<QString> com; com.push_back(tr("color"));
  char temp_ch[40];
  sprintf(temp_ch,"%d",alpha);
  QString s=temp_ch;
  com.push_back(tr("alpha")); com.push_back(s);
	send_message(com);


}

void color_menu::colorOverCComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
  com.push_back(tr("overlay_color")); com.push_back(text);
	send_message(com);
}
