#include "clip_menu.h"

#include <QMouseEvent>

clip_menu::clip_menu(datasets *dats, QString num,int nd)
{
  this->set_basics(dats,num,"Color",nd);
  clipWidget=new QWidget(this);
  colorWidget=new QWidget(this);

}
void clip_menu::createit() {
	lay = new QHBoxLayout;
//Layout for the color tab.
	layc =new QHBoxLayout;

  bar=new clip_bar(clipWidget);
  connect(bar, SIGNAL(emitit(std::vector<QString>)), this, 
    SLOT(send_it(std::vector<QString>)));
 
  colorBoxb = new QGroupBox( tr("Begin Clip"));

  QHBoxLayout  *layoutcb=new QHBoxLayout;
	layoutcb->setContentsMargins(2,2,2,2);

  bclipbox = new QComboBox();
	bclipbox->addItem(tr("black"));
	bclipbox->addItem(tr("red"));
	bclipbox->addItem(tr("blue"));
  bclipbox->addItem(tr("green"));
	bclipbox->addItem(tr("white"));
					connect(bclipbox, SIGNAL(activated(const QString&)),this, SLOT(bactivated(const QString&)));

	bclipbox->addItem(tr("transparent"));
	bclipbox->setToolTip(tr("Set color for bclip value"));
	layoutcb->addWidget(bclipbox);
  	colorBoxb->setLayout(layoutcb);

    colorBoxe = new QGroupBox( tr("End clip"));

   QHBoxLayout  *layoutce=new QHBoxLayout;
	layoutce->setContentsMargins(2,2,2,2);
  eclipbox = new QComboBox();
	eclipbox->addItem(tr("black"));
	eclipbox->addItem(tr("red"));
	eclipbox->addItem(tr("blue"));
		eclipbox->addItem(tr("green"));
				eclipbox->addItem(tr("white"));
					eclipbox->addItem(tr("transparent"));
	connect(eclipbox, SIGNAL(activated(const QString&)),this, SLOT(eactivated(const QString&)));
	eclipbox->setToolTip( tr("Set color for eclip value"));
	layoutce->addWidget(eclipbox);
	colorBoxe->setLayout(layoutce);

}
void clip_menu::send_it(std::vector<QString> com){
  send_message(com);

}
void clip_menu::mouseReleaseEvent(QMouseEvent *e){
  bar->mouseReleaseEvent(e);

}
void clip_menu::layout() {

	lay->addWidget(bar);
	clipWidget->setLayout(lay);
//	lay->setContentsMargins(2,2,2,2);
  layc->addWidget(colorBoxb);
    layc->addWidget(colorBoxe);

  colorWidget->setLayout(layc);
   layc->addStretch(1);
	this->addTab(clipWidget,"Histo/Clip");
	this->addTab(colorWidget,"Color");


//	lay->addStretch(1);
}

void clip_menu::actionRespond(std::vector<QString> com){
  if(com[3].contains("set_pt")) bar->set_pts(com[4]);
  if(com[3].contains("set_histo")) bar->set_histo(com[4]);
}

void clip_menu::bactivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("clip"));
    com.push_back(tr("bcolor")); com.push_back(text);
    	send_message(com);

}
void clip_menu::eactivated(const QString& text) {
  std::vector<QString> com; com.push_back(tr("clip"));
    com.push_back(tr("ecolor")); com.push_back(text);
    	send_message(com);

}
