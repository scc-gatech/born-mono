#include "view_menu.h"
#include <QPixmap>
#include "slice_types.h"
#include "my_colors.h"
view_menu::view_menu(datasets * ds, QString num,int nd)
{
  this->set_basics(ds,num,"View",nd);
  mainWidget=new QWidget(this,"main-view");
   saveWidget=new QWidget(this,"view-save");
   showWidget=new QWidget(this,"view-show");
    show2Widget=new QWidget(this,"view-show2");
      colorWidget=new QWidget(this);
  wiggleWidget=new QWidget(this);
  contourWidget=new QWidget(this);

}

void view_menu::createit() {
 view_createit();
 color_createit();

}
void view_menu::view_createit(){
//Layout for Main tab.
	layMain =new QHBoxLayout(mainWidget);
//Layout for Save tab.
	laySave =new QHBoxLayout(saveWidget);
//Layout for Show tab.
  layShow= new QHBoxLayout(showWidget);
  
  layShow2= new QHBoxLayout(show2Widget);

    
// The buttons in the Main tab.
	viewFacesBox = new QGroupBox(tr("Perspective"));
	QHBoxLayout *layoutvf=new QHBoxLayout;

	viewFacesComboBox = new QComboBox(viewFacesBox, vnum+tr("-facesComboBox - view_menu"));
	viewFacesComboBox->insertItem(tr("FRONT"));
	viewFacesComboBox->insertItem(tr("SIDE"));
	viewFacesComboBox->insertItem(tr("TOP"));
	viewFacesComboBox->insertItem(tr("THREE"));
	//viewFacesComboBox->insertItem(tr("CUT"));
	viewFacesComboBox->insertItem(tr("CUBE"));
	viewFacesComboBox->setCurrentIndex(3);
	viewFacesComboBox->setMinimumSize(QSize(40,20));

	//viewFacesComboBox->setCurrentItem(3); // default: three faces
	QToolTip::add(viewFacesComboBox, tr("PERSPECTIVE -- Choose a viewing angle."));
	connect(viewFacesComboBox, SIGNAL(activated(const QString&)),this, SLOT(viewFacesComboBoxActivated(const QString&)));
	layoutvf->addWidget(viewFacesComboBox);
 	layoutvf->setContentsMargins(2,2,2,2);
	viewFacesBox->setLayout(layoutvf);
		// faces
		
	 viewFontBox=new QGroupBox(tr("Font"));
	 QGridLayout *layout=new QGridLayout;
	 viewFontComboBox = new QComboBox(viewFontBox, vnum+tr("-fontComboBox - view_menu"));
	viewFontComboBox->insertItem(tr("arial-12-normal"));
	viewFontComboBox->insertItem(tr("arial-12-bold"));
	viewFontComboBox->insertItem(tr("arial-14-bold"));
	viewFontComboBox->insertItem(tr("arial-16-bold"));
	viewFontComboBox->insertItem(tr("arial-18-bold"));
		viewFontComboBox->setMinimumSize(QSize(40,20));

	layout->addWidget(viewFontComboBox);
	  	layout->setContentsMargins(2,2,2,2);
	viewFontBox->setLayout(layout);
	 
	//viewFacesComboBox->setCurrentItem(3); // default: three faces
	QToolTip::add(viewFontComboBox, tr("FONT- Chose font"));
	connect(viewFontComboBox, SIGNAL(activated(const QString&)),this, SLOT(viewFontComboBoxActivated(const QString&)));

//The buttons in the Show tab.
  	QHBoxLayout *layoutvs=new QHBoxLayout;
	  	layoutvs->setContentsMargins(2,2,2,2);

    
    barcButtonGroup = new QGroupBox(tr("Draw Colorbar"));
		QHBoxLayout *layoutbc=new QHBoxLayout;
	  	layoutbc->setContentsMargins(2,2,2,2);

	
  barcyesButton = new QRadioButton(tr("Yes"),barcButtonGroup, tr("main locked"));
	barcnoButton = new QRadioButton(tr("No"),barcButtonGroup, tr("main not locked"));
	if (!barcyesButton->isOn()) barcnoButton->toggle();
	layoutbc->addWidget(barcyesButton);
	layoutbc->addWidget(barcnoButton);
	barcButtonGroup->setLayout(layoutbc);
	QToolTip::add(barcyesButton, tr("Yes- Draw colorbar"));
	QToolTip::add(barcnoButton, tr("NO -- Don't draw colorbar"));
    connect(barcnoButton, SIGNAL(clicked()), this, SLOT(barcNoButtonClicked()));
	connect(barcyesButton, SIGNAL(clicked()), this, SLOT(barcYesButtonClicked()));
  layoutvs->addWidget(barcButtonGroup);
  barcButtonGroup->setMinimumSize(QSize(95,20));
  


 barpButtonGroup = new QGroupBox( tr("Draw Pos"));
	QHBoxLayout *layoutbp=new QHBoxLayout;
	layoutbp->setContentsMargins(2,2,2,2);
  transpSlider = new QSlider(Qt::Horizontal);
	transpSlider->setValue(100);
	transpSlider->setRange(0,100);
	transpSlider->setMinimumSize(QSize(40,25));
layoutbp->addWidget(transpSlider);
	QToolTip::add(transpSlider, tr("Set tranparency for positions"));
	connect(transpSlider, SIGNAL(valueChanged(int)), this, SLOT(setTranspSlider (int)));
 barpButtonGroup->setLayout(layoutbp);

/*
  barpButtonGroup = new QGroupBox( tr("Draw Pos"));
	QHBoxLayout *layoutbp=new QHBoxLayout;
		  	layoutbp->setContentsMargins(2,2,2,2);
    barpyesButton = new QRadioButton(tr("Yes"),barpButtonGroup, tr("main locked"));
	barpnoButton = new QRadioButton(tr("No"),barpButtonGroup, tr("main not locked"));
	layoutbp->addWidget(barpyesButton);
	layoutbp->addWidget(barpnoButton);
	barpButtonGroup->setLayout(layoutbp);
  barpButtonGroup->setMinimumSize(QSize(95,20));
	if (!barpyesButton->isOn()) barpnoButton->toggle();
	QToolTip::add(barpyesButton, tr("Yes- Draw position"));
	QToolTip::add(barpnoButton, tr("NO -- Don't draw position"));
    connect(barpnoButton, SIGNAL(clicked()), this, SLOT(barpNoButtonClicked()));
	connect(barpyesButton, SIGNAL(clicked()), this, SLOT(barpYesButtonClicked()));
*/
   layoutvs->addWidget(barpButtonGroup);

   
   
   
   
   
	barkButtonGroup = new QGroupBox( tr("Draw Picks"));
	QHBoxLayout *layoutk=new QHBoxLayout;
		  	layoutk->setContentsMargins(2,2,2,2);

    barkyesButton = new QRadioButton(tr("Yes"),barkButtonGroup, tr("main locked"));
	barknoButton = new QRadioButton(tr("No"),barkButtonGroup, tr("main not locked"));
	if (!barknoButton->isOn()) barknoButton->toggle();
	QToolTip::add(barkyesButton, tr("Yes- Draw picks"));
	QToolTip::add(barknoButton, tr("NO -- Don't draw picks"));
	
    connect(barknoButton, SIGNAL(clicked()), this, SLOT(barkNoButtonClicked()));
	connect(barkyesButton, SIGNAL(clicked()), this, SLOT(barkYesButtonClicked()));
  layoutk->addWidget(barkyesButton);
  layoutk->addWidget(barknoButton);
  barkButtonGroup->setLayout(layoutk);
  barkButtonGroup->setMinimumSize(QSize(95,20));

  layoutvs->addWidget(barkButtonGroup);
	
	bartButtonGroup = new QGroupBox( tr("Draw Title"));
		QHBoxLayout *layoutt=new QHBoxLayout;
	  	layoutt->setContentsMargins(2,2,2,2);

    bartyesButton = new QRadioButton(tr("Yes"),bartButtonGroup, tr("main locked"));
	bartnoButton = new QRadioButton(tr("No"),bartButtonGroup, tr("main not locked"));
	if (!bartyesButton->isOn()) bartnoButton->toggle();
	QToolTip::add(bartyesButton, tr("Yes- Draw title"));
	QToolTip::add(bartnoButton, tr("NO -- Don't draw title"));
    connect(bartnoButton, SIGNAL(clicked()), this, SLOT(bartNoButtonClicked()));
	connect(bartyesButton, SIGNAL(clicked()), this, SLOT(bartYesButtonClicked()));
	layoutt->addWidget(bartyesButton);
	layoutt->addWidget(bartnoButton);
	bartButtonGroup->setLayout(layoutt);
	    bartButtonGroup->setMinimumSize(QSize(95,20));

	layoutvs->addWidget(bartButtonGroup);
	viewShowBox = new QGroupBox(tr("Show"));
	viewShowBox->setLayout(layoutvs);
	
	 bargButtonGroup = new QGroupBox(tr("Draw Grid"));
	QHBoxLayout *layoutg=new QHBoxLayout;
		  	layoutg->setContentsMargins(2,2,2,2);

    bargyesButton = new QRadioButton(tr("Yes"),bargButtonGroup, tr("main locked"));
	bargnoButton = new QRadioButton(tr("No"),bargButtonGroup, tr("main not locked"));
	if (!bargyesButton->isOn()) bargnoButton->toggle();
	QToolTip::add(bartyesButton, tr("Yes- Draw grid"));
	QToolTip::add(bartnoButton, tr("NO -- Don't draw grid"));
    connect(bargnoButton, SIGNAL(clicked()), this, SLOT(bargNoButtonClicked()));
	connect(bargyesButton, SIGNAL(clicked()), this, SLOT(bargYesButtonClicked()));
	layoutg->addWidget(bargyesButton);
	layoutg->addWidget(bargnoButton);
	bargButtonGroup->setLayout(layoutg);
	    bargButtonGroup->setMinimumSize(QSize(95,20));




	 barrButtonGroup = new QGroupBox(tr("Draw Region pts"));
	QHBoxLayout *layoutr=new QHBoxLayout;
		  	layoutr->setContentsMargins(2,2,2,2);

    barryesButton = new QRadioButton(tr("Yes"),barrButtonGroup, tr("main locked"));
	barrnoButton = new QRadioButton(tr("No"),barrButtonGroup, tr("main not locked"));
	if (!barryesButton->isOn()) barrnoButton->toggle();
	QToolTip::add(barryesButton, tr("Yes- Draw region pts"));
	QToolTip::add(barrnoButton, tr("NO -- Don't draw region pts"));
    connect(barrnoButton, SIGNAL(clicked()), this, SLOT(barrNoButtonClicked()));
	connect(barryesButton, SIGNAL(clicked()), this, SLOT(barrYesButtonClicked()));
	layoutr->addWidget(barryesButton);
	layoutr->addWidget(barrnoButton);
	barrButtonGroup->setLayout(layoutr);
	    barrButtonGroup->setMinimumSize(QSize(95,20));






// The buttons in the save tab.	
	viewSaveBox = new QGroupBox(tr("Save"));	
	 jpegSave = new QPushButton("ppm",viewSaveBox, tr("PPM Save"));
	
	QToolTip::add(jpegSave, tr("Save view as a PPM"));
	connect(jpegSave, SIGNAL(clicked()), this, SLOT(jpegButtonClicked()));
	
	psSSave = new QPushButton("ps-S",viewSaveBox, tr("PS Small Save"));
	QToolTip::add(psSSave, tr("Save view low-fidelity postscript (slides)"));
	connect(psSSave, SIGNAL(clicked()), this, SLOT(psSButtonClicked()));
	psBSave = new QPushButton("ps-B",viewSaveBox, tr("PS Big Save"));
	QToolTip::add(psBSave, tr("Save a high-fidelty postscript (papers)"));
	connect(psBSave, SIGNAL(clicked()), this, SLOT(psBButtonClicked()));
	QHBoxLayout *layoutp=new QHBoxLayout;
		  	layoutp->setContentsMargins(2,2,2,2);

	layoutp->addWidget(jpegSave);
	layoutp->addWidget(psSSave);
	layoutp->addWidget(psBSave);
	psSSave->setMinimumSize(QSize(30,20));
	psBSave->setMinimumSize(QSize(30,20));
	jpegSave->setMinimumSize(QSize(30,20));
    viewSaveBox->setMinimumSize(QSize(100,20));
	viewSaveBox->setLayout(layoutp);

}
// Adds the widgets and layouts into the main/parent window.

void view_menu::color_createit() {
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
    
	colorOverComboBox->setCurrentIndex(cols.loc["black"]);
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
	colorBgComboBox->setCurrentIndex(cols.loc["white"]);


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
	colorWigComboBox->setCurrentIndex(cols.loc["black"]);

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
      
      posWigComboBox->setCurrentIndex(cols.loc["black"]+1);

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
		      	colorContComboBox->setCurrentIndex(cols.loc["black"]);

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
		numberContComboBox->setCurrentIndex(3);
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
	speedSlider = new QSlider(Qt::Horizontal);

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

void view_menu::layout() {

	layMain->addWidget(viewFacesBox);

		layMain->addWidget(viewFontBox);
	laySave->addWidget(viewSaveBox);
	

	layShow->addWidget(barcButtonGroup);
	layShow->addWidget(barpButtonGroup);
	layShow->addWidget(bartButtonGroup);
	
	
	
	
    layShow2->addWidget(barkButtonGroup);
    layShow2->addWidget(barrButtonGroup);
    layShow2->addWidget(bargButtonGroup);

	layMain->addStretch(1);
		laySave->addStretch(1);
   layShow->addStretch(1);
      layShow2->addStretch(1);

	this->addTab(mainWidget,"Main");

//if(datas->return_ndat() >1){
 multi_dat_create();
 //}
	this->addTab(saveWidget,"Save");
		this->addTab(showWidget,"Show");
	this->addTab(show2Widget,"Extras");
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

	this->addTab(colorWidget,"Color");
	this->addTab(wiggleWidget,"Wiggle");
		this->addTab(contourWidget,"Contour");

}
void view_menu::actionRespond(std::vector<QString>coms){
 if(coms[3].contains("multi")) multi_dat_create();


}


void view_menu::multi_dat_create(){
  // overlayWidget=new QWidget(this,"overlay-view");
   h_dataWidget= new QWidget(this,"h_data-view");
    // layOverlay= new QHBoxLayout(overlayWidget);
	  layData=new QHBoxLayout(h_dataWidget);
    QHBoxLayout *layoutv= new QHBoxLayout;
        QHBoxLayout *layoutvo= new QHBoxLayout;

    	  	layoutv->setContentsMargins(2,2,2,2);

	  viewDataBox = new QGroupBox(tr("Data"));
	  viewDataComboBox = new QComboBox(viewDataBox, vnum+tr("-DataComboBox - view_menu"));
	viewDataComboBox->setMinimumSize(QSize(40,20));

	QString temp,temp2;
	  char t[10];
	  for(int i=0; i< (int)datas->return_ndat(); i++){
	    sprintf(t,"%d",i);
	    temp=t;
	    viewDataComboBox->insertItem(temp+":"+datas->return_dat(i)->name);
	  }
	  int inum=vnum.toInt();
	  int nd=datas->return_ndat();
	  if(inum<nd) viewDataComboBox->setCurrentIndex(inum);
	  else viewDataComboBox->setCurrentIndex(0);

	  //viewFacesComboBox->setCurrentItem(3); // default: three faces
	  QToolTip::add(viewDataComboBox, tr("Choose dataset to view"));
	  connect(viewDataComboBox, SIGNAL(activated(const QString&)), this, SLOT(viewDataComboBoxActivated(const QString&)));
    layoutv->addWidget(viewDataComboBox);
    viewDataBox->setLayout(layoutv);
	// round robin
	viewRoundBox = new QGroupBox( tr("Cycle data"));
	QHBoxLayout *layoutr=new QHBoxLayout;
		  	layoutr->setContentsMargins(2,2,2,2);


	viewRoundPrevButton = new QPushButton(QIcon(qPixmapFromMimeSource("back.png")),
		0, viewRoundBox, tr("viewRoundPrevButton - view_menu"));
	viewRoundNextButton = new QPushButton(QIcon(qPixmapFromMimeSource("forward.png")),
		0, viewRoundBox, tr("viewRoundNextButton - view_menu"));
	QToolTip::add(viewRoundPrevButton, tr("PREVIOUS -- Go to previous h_data set in round robin cycle."));
	QToolTip::add(viewRoundNextButton, tr("NEXT -- Go to next h_data set in round robin cycle."));
  layoutr->addWidget(viewRoundPrevButton);
  layoutr->addWidget(viewRoundNextButton);
 	viewRoundPrevButton->setMinimumSize(QSize(20,20));
 	viewRoundNextButton->setMinimumSize(QSize(20,20));

 viewRoundBox->setLayout(layoutr);
        connect(viewRoundNextButton, SIGNAL(clicked()), this, SLOT(viewRoundNextButtonClicked()));
        connect(viewRoundPrevButton, SIGNAL(clicked()), this, SLOT(viewRoundPrevButtonClicked()));

        
   //
    viewDataOBox = new QGroupBox(tr("Overlay"));
	  viewDataOComboBox = new QComboBox(viewDataOBox, vnum+tr("-DataOComboBox - view_menu"));
	viewDataOComboBox->setMinimumSize(QSize(40,20));

	  	    viewDataOComboBox->insertItem("none");

	  for(int i=0; i< (int)datas->return_ndat(); i++){
	    sprintf(t,"%d",i);
	    temp=t;
	    viewDataOComboBox->insertItem(temp+":"+datas->return_dat(i)->name);
	  }

	  //viewFacesComboBox->setCurrentItem(3); // default: three faces
	  QToolTip::add(viewDataOComboBox, tr("Choose the data to overlay"));
	  connect(viewDataOComboBox, SIGNAL(activated(const QString&)), this, SLOT(viewDataOComboBoxActivated(const QString&)));
    layoutvo->addWidget(viewDataOComboBox);
    viewDataOBox->setLayout(layoutvo);
	// round robin
	viewRoundOBox = new QGroupBox( tr("Cycle overlay"));
		QHBoxLayout *layoutroo=new QHBoxLayout;

		  	layoutvo->setContentsMargins(2,2,2,2);


	viewRoundPrevOButton = new QPushButton(QIcon(qPixmapFromMimeSource("back.png")),
		0, viewRoundOBox, tr("viewRoundPrevButton - view_menu"));
	viewRoundNextOButton = new QPushButton(QIcon(qPixmapFromMimeSource("forward.png")),
		0, viewRoundOBox, tr("viewRoundNextButton - view_menu"));
	QToolTip::add(viewRoundPrevOButton, tr("PREVIOUS -- Go to previous dataset in round robin cycle."));
	QToolTip::add(viewRoundNextOButton, tr("NEXT -- Go to next datasetin round robin cycle."));
  layoutroo->addWidget(viewRoundPrevOButton);
  layoutroo->addWidget(viewRoundNextOButton);
		  	layoutroo->setContentsMargins(2,2,2,2);
 	viewRoundPrevOButton->setMinimumSize(QSize(20,20));
 	viewRoundNextOButton->setMinimumSize(QSize(20,20));

 viewRoundOBox->setLayout(layoutroo);
        connect(viewRoundNextOButton, SIGNAL(clicked()), this, SLOT(viewRoundONextButtonClicked()));
        connect(viewRoundPrevOButton, SIGNAL(clicked()), this, SLOT(viewRoundOPrevButtonClicked()));

        
        
        
        //
        
  
	layData->addWidget(viewDataBox);
	//if(datas.size() >1){
	
  	  layData->addWidget(viewRoundBox);
  	  	layData->addWidget(viewDataOBox);
	//if(datas.size() >1){
	
  	  layData->addWidget(viewRoundOBox);

	  //viewBoxLayout->addWidget(viewMultiBox); //multi
	  //layOverlay->addStretch(1);
	  layData->addStretch(1);
	  this->addTab(h_dataWidget,"Data");
	  //this->addTab(overlayWidget,"Overlay");
	//}
}


void view_menu::viewFacesComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("view"));
	com.push_back(tr("display")); com.push_back(text);
		send_message(com);
}
void view_menu::viewFontComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("view"));
	com.push_back(tr("font")); com.push_back(text);
		send_message(com);
}


void view_menu::viewDataComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("view"));
	com.push_back(tr("data")); com.push_back(text);
	send_message(com);
}
 void view_menu::viewDataOComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("view"));
	com.push_back(tr("overlay")); com.push_back(text);
	send_message(com);
}
void view_menu::viewRoundNextButtonClicked() {
  std::vector<QString> com; com.push_back(tr("view"));
	com.push_back(tr("roundrobin")); com.push_back(tr("forward"));
  send_message(com);
}

void view_menu::viewRoundPrevButtonClicked() {
  std::vector<QString> com; com.push_back(tr("view"));
	com.push_back(tr("roundrobin")); com.push_back(tr("back"));
  send_message(com);
}

void view_menu::viewRoundONextButtonClicked() {
  std::vector<QString> com; com.push_back(tr("view"));
	com.push_back(tr("roundorobin")); com.push_back(tr("forward"));
  send_message(com);
}

void view_menu::viewRoundOPrevButtonClicked() {
  std::vector<QString> com; com.push_back(tr("view"));
	com.push_back(tr("roundorobin")); com.push_back(tr("back"));
  send_message(com);
}
void view_menu::psBButtonClicked() {
	QString filename = QFileDialog::getSaveFileName( this );
	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("save")); com.push_back(tr("ps"));
	 	 com.push_back(tr("big"));

	 com.push_back(filename);
	if ( !filename.isNull() )  
	    	send_message(com);
}
void view_menu::pdfButtonClicked() {
	QString filename = QFileDialog::getSaveFileName( this );
	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("save")); com.push_back(tr("pdf"));


	 com.push_back(filename);
	if ( !filename.isNull() )  
	    	send_message(com);
}

void view_menu::barcYesButtonClicked() {

	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("bar")); com.push_back(tr("yes"));
 
	    	send_message(com);
}
void view_menu::barcNoButtonClicked() {

	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("bar")); com.push_back(tr("no"));
 
	    	send_message(com);
}
void view_menu::barrYesButtonClicked() {

	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("region")); com.push_back(tr("yes"));
 
	    	send_message(com);
}
void view_menu::barrNoButtonClicked() {
	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("region")); com.push_back(tr("no"));
 
	    	send_message(com);
}
void view_menu::bargYesButtonClicked() {

	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("grid")); com.push_back(tr("yes"));
 
	    	send_message(com);
}
void view_menu::bargNoButtonClicked() {
	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("grid")); com.push_back(tr("no"));
 
	    	send_message(com);
}

void view_menu::barpYesButtonClicked() {

	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("pos")); com.push_back(tr("yes"));
 
	    	send_message(com);
}
void view_menu::barpNoButtonClicked() {
	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("pos")); com.push_back(tr("no"));
 
	    	send_message(com);
}

void view_menu::barkYesButtonClicked() {

	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("picks")); com.push_back(tr("yes"));
 
	    	send_message(com);
}
void view_menu::barkNoButtonClicked() {

	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("picks")); com.push_back(tr("no"));
 
	    	send_message(com);
}


void view_menu::bartYesButtonClicked() {


	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("title")); com.push_back(tr("yes"));
 
	    	send_message(com);
}
void view_menu::bartNoButtonClicked() {

	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("title")); com.push_back(tr("no"));
 
	    	send_message(com);
}




void view_menu::psSButtonClicked() {
	QString filename = QFileDialog::getSaveFileName(this );
	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("save")); com.push_back(tr("ps"));
	 com.push_back(tr("small"));
	 com.push_back(filename);
	if ( !filename.isNull() )  
	    	send_message(com);
}
void view_menu::jpegButtonClicked() {
	QString filename = QFileDialog::getSaveFileName( this );
	 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("save")); com.push_back(tr("jpeg"));
	 com.push_back(filename);
	if ( !filename.isNull() )  
	    	send_message(com);
}

void view_menu::setTranspSlider(int speed)
{
		 std::vector <QString> com; com.push_back("view");
	 com.push_back(tr("pos_transp")); com.push_back(QString::number(speed));
 
	    	send_message(com);
	
}
void view_menu::colorCubeComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
  com.push_back(tr("colortable")); com.push_back(text);
  	send_message(com);

}

void view_menu::colorOverComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
	com.push_back(tr("line_color")); com.push_back(text);
		send_message(com);
}

void view_menu::colorBgComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
    com.push_back(tr("background")); com.push_back(text);
    	send_message(com);

}
void view_menu::colorWigComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
    com.push_back(tr("wiggle_color")); com.push_back(text);
    	send_message(com);

}
void view_menu::posWigComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
    com.push_back(tr("wiggle_pos")); com.push_back(text);
    	send_message(com);

}
void view_menu::negWigComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
    com.push_back(tr("wiggle_neg")); com.push_back(text);
    	send_message(com);

}

void view_menu::colorContComboBoxActivated(const QString& text) {
	std::vector<QString> com; com.push_back(tr("color"));
	  com.push_back(tr("contour_color")); com.push_back(text);
		send_message(com);
}

void view_menu:: numberContComboBoxActivated(const QString& text) {
	std::vector<QString> com; com.push_back(tr("color"));
	  com.push_back(tr("number of lines")); com.push_back(text);
	    send_message(com);
}
void view_menu:: dcContComboBoxActivated(const QString& text) {
	std::vector<QString> com; com.push_back(tr("color"));
	 bool ok;
	 double d = QString( text ).toDouble(&ok); // ok == false

	  com.push_back(tr("contour_dc")); com.push_back(text);
	if(ok)send_message(com);
			  if(d==0.){}

}
void view_menu:: c0ContComboBoxActivated(const QString& text) {
	std::vector<QString> com; com.push_back(tr("color"));
	bool ok;
	double d = QString( text ).toDouble(&ok); // ok == false
	  com.push_back(tr("contour_c0")); com.push_back(text);
	  if(ok) send_message(com);
	  if(d==0.){}
}
void view_menu::setAlpha(int alpha){
std::vector<QString> com; com.push_back(tr("color"));
  char temp_ch[40];
  sprintf(temp_ch,"%d",alpha);
  QString s=temp_ch;
  com.push_back(tr("alpha")); com.push_back(s);
	send_message(com);


}

void view_menu::colorOverCComboBoxActivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("color"));
  com.push_back(tr("overlay_color")); com.push_back(text);
	send_message(com);
}

 
