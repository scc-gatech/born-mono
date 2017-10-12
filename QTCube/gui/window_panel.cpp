#include "window_panel.h"
using namespace SEP;
window_panel::window_panel(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, std::shared_ptr<panels> my_p, 
  std::shared_ptr<datasets> my_d, std::shared_ptr<pick_draw> pk, std::shared_ptr<slice_types>c,std::shared_ptr<maps> mym){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);
   for(int i=0; i < my_w->nwind(); i++){
      my_tabs[QString::number(i+1)]=new sub_window_panel(p,my_w,my_p,my_d,pk,c,mym,QString::number(i));
      addTab(my_tabs[QString::number(i+1)],QString::number(i+1));
      sub_window_panel *x=(sub_window_panel*)my_tabs[QString::number(i+1)];
      connect(x,
        SIGNAL(actionDetected(std::vector<QString>)),this,SLOT(actionRespond(std::vector<QString>)));
   }
      setMinimumHeight(150);

  
}
void sub_window_panel::update_menu(std::vector<QString> coms){


}
void window_panel::update_menu(std::vector<QString> coms){
 if(coms[1].contains("windows")){
   int inum=coms[3].toInt(), ip=inum+1;
    if(coms[2].contains("add")){
       my_tabs[QString::number(ip)]=
         new sub_window_panel(my_pos,my_wind,my_pan,my_dat,my_pick,my_slices,my_maps,QString::number(inum));
      addTab(my_tabs[QString::number(ip)],QString::number(ip));
          connect(my_tabs[QString::number(ip)], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

    }
   
 }
 
}
sub_window_panel:: sub_window_panel(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, 
std::shared_ptr<panels> my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk,
 std::shared_ptr<slice_types>c,std::shared_ptr<maps> my_m,QString n){
  set_basics(p,my_w,my_p,my_d,pk,c,my_m);
  myw=n;
    
     mainLay=new QHBoxLayout();
     
     
     
     saveBox=new QGroupBox(tr("Save"));
     saveLay=new QHBoxLayout();

    saveLow= new QPushButton(QIcon(QPixmap(":/images/small.png")),tr(""));
    saveLow->setToolTip(tr("Save a small PDF of the current view"));
    saveHigh= new QPushButton(QIcon(QPixmap(":/images/big.png")),tr(""));
    saveHigh->setToolTip(tr("Save a large PDF of the current view"));
    saveLay->addWidget(saveLow);
    saveLay->addWidget(saveHigh);
    saveBox->setLayout(saveLay);

    std::vector<QString> options;
    for(int i=1; i < 11; i++) options.push_back(QString::number(i));

     splitXBox=new basicComboBox("Split X","Number of sections to split in X",options);
     splitYBox=new basicComboBox("Split Y","Number of sections to split in X",options);
     options.clear();
     for(int i=0; i < 11; i++) options.push_back(QString::number(i));
     boundaryBox=new basicComboBox("Boundary","Number of pixels to make the boundary",options);
     boundaryBox->set_current(0,5);

  	closeButton= new QPushButton(QIcon(QPixmap(":/images/stop.png")),tr(""));
    closeButton->setToolTip(tr("Close-Close the window"));
 	connect(saveLow, SIGNAL(clicked()), this, SLOT(save_small()));
 	connect(saveHigh, SIGNAL(clicked()), this, SLOT(save_big()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close_window()));
 	connect(splitXBox->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(split_x(const QString&)));
 	connect(splitYBox->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(split_y(const QString&)));
 	connect(boundaryBox->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(boundary_change(const QString&)));

    mainLay->addWidget(saveBox);
    mainLay->addWidget(splitXBox->group());
        mainLay->addWidget(splitYBox->group());
    mainLay->addWidget(boundaryBox->group());

    mainLay->addWidget(closeButton);
    mainLay->addStretch(1);
    setLayout(mainLay);


}


void sub_window_panel::delete_sub_window_panel(){

  delete mainLay;
  delete saveLay;
  delete splitXBox;
  delete splitYBox;
  delete boundaryBox;
  delete saveBox;
  delete saveLow;
  delete saveHigh;
  delete closeButton;



}

void sub_window_panel::save_big(){
  std::vector<QString> coms;
   QString filename = QFileDialog::getSaveFileName(this );
       if ( !filename.isNull() ){
         coms.push_back(myw);
         coms.push_back("windows");
         coms.push_back("save_big");
         coms.push_back(filename);
                  emit actionDetected(coms);

        }
}
void sub_window_panel::save_small(){
  std::vector<QString> coms;
   QString filename = QFileDialog::getSaveFileName(this );
       if ( !filename.isNull() ){
         coms.push_back(myw);
         coms.push_back("windows");
         coms.push_back("save_small");
         coms.push_back(filename);
         emit actionDetected(coms);
        }
}
void sub_window_panel::split_x(const QString x){
  std::vector<QString> coms;

         coms.push_back(myw);
         coms.push_back("windows");
         coms.push_back("split_x");
         coms.push_back(x);
         emit actionDetected(coms);
}
void sub_window_panel::split_y(const QString x){
  std::vector<QString> coms;

         coms.push_back(myw);
         coms.push_back("windows");
         coms.push_back("split_y");
         coms.push_back(x);
         emit actionDetected(coms);
}
void sub_window_panel::boundary_change(const QString x){
  std::vector<QString> coms;

         coms.push_back(myw);
         coms.push_back("windows");
         coms.push_back("boundary");
         coms.push_back(x);
         emit actionDetected(coms);
}

void sub_window_panel::close_window(){
  std::vector<QString> coms;

         coms.push_back(myw);
         coms.push_back("windows");
         coms.push_back("close");
      exit(0);
         emit actionDetected(coms);
}


  
