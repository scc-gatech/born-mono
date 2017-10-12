#include "pick_panel.h"
using namespace SEP;
pick_panel:: pick_panel(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk, std::shared_ptr<slice_types>c,std::shared_ptr<maps>mym){




  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  layMain.reset( new QVBoxLayout());
  row1=build_row_1();
  row2=build_row_2();
  row3=build_row_3();
    row4=build_row_4();

  layMain->addWidget(row1.get());
  layMain->addWidget(row2.get());
  layMain->addWidget(row3.get());
  layMain->addWidget(row4.get());

  setLayout(layMain.get());
  
  }
void pick_panel::update_menu(std::vector<QString>){


}

std::shared_ptr<QWidget> pick_panel::build_row_1(){
    std::shared_ptr<QWidget> mine(new QWidget());
    lay1.reset(new QHBoxLayout());

     std::vector<QString> c1,c2;
     c1=my_c->return_cvec();
     c2.push_back("all");
    for(int i=0; i <(int)c1.size();i++){
       c2.push_back(c1[i]);
     }
     group.reset(new basicComboBox("Pick group","Current group activated by the mouse",c1));

     display.reset(new basicRadioBox("How to display picks","Points","Display picks as points"));
     display->addButton("line","Display picks as lines");
     display->addButton("text","Display text associated with picks");
     display->set_checked(0);
     std::vector<QString> listg;
     for(int i=1; i <=8; i++) listg.push_back(QString::number(i));
     single.reset(new basicComboBox("Single","Axis which isn't single valued",listg));
     view.reset( new basicComboBox("View/change","Pickset that we will display/change",c2));
     
       connect(group->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(group_change(const QString&)));
       connect(view->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(view_change(const QString&)));
       connect(single->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(single_change(const QString&)));
  	connect(display->my_radios[0], SIGNAL(clicked()), this, SLOT(display_points()));
  	connect(display->my_radios[1], SIGNAL(clicked()), this, SLOT(display_line()));
  	connect(display->my_radios[2], SIGNAL(clicked()), this, SLOT(display_text()));

  

    
     lay1->addWidget(group->group());
     lay1->addWidget(view->group());
     lay1->addWidget(display->group());
     lay1->addWidget(single->group());

    lay1->addStretch(1);
     mine->setLayout(lay1.get());

     return mine;
  }
  
std::shared_ptr<QWidget> pick_panel::build_row_2(){
      std::shared_ptr<QWidget> mine(new QWidget());
      lay2.reset(new QHBoxLayout());
      
      loadsave.reset( new basicButtonBox("Load/save",":/images/save.png","Save picks",true));
      loadsave->addButton(":/images/open.png","Load picks",true);
      distance.reset( new basicSliderBox("Distance","Distance away to display picks",0,20,200));

      std::vector<QString> listg;
      for(int i=1; i <=8; i++) listg.push_back(QString::number(i));
      extra.reset( new basicComboBox("Extra","Extra information about picks",listg));
      
      pksize.reset(new basicButtonBox("Pick size",":/images/picks-large.png","Increase the size of the picks",true));
      pksize->addButton(":/images/picks-small.png","Derease the size of the picks",true);
    
      pkText.reset(new basicLineEditBox("Text",10,"","Pick text"));

  	connect(loadsave->my_radios[0], SIGNAL(clicked()), this, SLOT(load_picks()));
  	connect(loadsave->my_radios[1], SIGNAL(clicked()), this, SLOT(save_picks()));
    connect(distance->my_sliders[0], SIGNAL(valueChanged(int)), this, SLOT(distance_change (int)));
     connect(extra->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(extra_change(const QString&)));
  	connect(pksize->my_radios[0], SIGNAL(clicked()), this, SLOT(increase_size()));
  	connect(pksize->my_radios[1], SIGNAL(clicked()), this, SLOT(decrease_size()));
    connect(pkText->my_edits[0], SIGNAL(textChanged(const QString&)),this, SLOT(change_text(const QString&)));

      
    lay2->addWidget(loadsave->group());
    lay2->addWidget(distance->group());
    lay2->addWidget(extra->group());
    lay2->addWidget(pksize->group());
    lay2->addWidget(pkText->group());

    lay2->addStretch(1);
     mine->setLayout(lay2.get());

     return mine;
  
  }
std::shared_ptr<QWidget>pick_panel::build_row_3(){
      std::shared_ptr<QWidget> mine(new QWidget());
      lay3.reset(new QHBoxLayout());



      auto2d.reset(
        new basicButtonBox("2-D autopicking","pick","Perform autopicking",false));
      auto2d->addButton("accept","Accept the last autopicking",false);
      auto2d->addButton("decline","Decline the last autopicking",false);
      auto2d->addButton("extend","Attempt to extend a series of 2-D picks",false);
      
      std::vector<QString> m; m.push_back("Viterbi"); m.push_back("Brown");
      std::shared_ptr<basicComboBox> method
       (new basicComboBox("Method","Method to use for 2-D autopicking",m));
      
      m.clear(); m.push_back("Left-Right"); m.push_back("In-Out");m.push_back("Top-bottom");
      std::shared_ptr<basicComboBox>  direction
        (new basicComboBox("Plane","Plane to extend picks in",m));
      
      connect(auto2d->my_radios[0], SIGNAL(clicked()), this, SLOT(auto_do()));
      connect(auto2d->my_radios[1], SIGNAL(clicked()), this, SLOT(auto_accept()));
      connect(auto2d->my_radios[2], SIGNAL(clicked()), this, SLOT(auto_decline()));
      connect(auto2d->my_radios[3], SIGNAL(clicked()), this, SLOT(auto_extend()));
      
      connect(method->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(auto_method(const QString&)));
      connect(direction->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(auto_direction(const QString&)));

       lay3->addWidget(auto2d->group());
       lay3->addWidget(method->group());
       lay3->addWidget(direction->group());
       

    lay3->addStretch(1);
     mine->setLayout(lay3.get());
     return mine;
  }
  
  std::shared_ptr<QWidget> pick_panel::build_row_4(){
      std::shared_ptr<QWidget> mine(new QWidget());
      lay4.reset(new QHBoxLayout());


      std::vector<QString> m; m.push_back("correlation"); m.push_back("amplitude");
      autoType.reset(new basicComboBox("Type","What to use as the basis of autopicking",m));
      m.clear(); m.push_back("5"); m.push_back("10");m.push_back("20"); m.push_back("30");
      autoLen.reset(new basicComboBox("Length","Length of window to look at",m));
      autoLen->set_current(0,2);
      m.clear();m.push_back("3");m.push_back("10");m.push_back("20");
      m.push_back("30");m.push_back("50");
      autoMax.reset(new basicComboBox("MaxD","Maximum distance to allow a hop",m));
      autoMax->set_current(0,3);
      m.clear();m.push_back("0"); m.push_back("2");m.push_back("4");
      m.push_back("6"); m.push_back("10");m.push_back("20");
      autoError.reset(new basicComboBox("Error","Allowable error",m));
      
      
      connect(autoType->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(auto_type(const QString&)));
      connect(autoLen->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(auto_len(const QString&)));
      connect(autoMax->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(auto_maxd(const QString&)));
      connect(autoError->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(auto_type(const QString&)));

     
      lay4->addWidget(autoType->group());
      lay4->addWidget(autoLen->group());
      lay4->addWidget(autoMax->group());
      lay4->addWidget(autoError->group());

    lay4->addStretch(1);
     mine->setLayout(lay4.get());
     return mine;
  }
void pick_panel::delete_row_1(){

 
  }
  
void pick_panel::delete_row_2(){

  }    
void pick_panel::delete_row_3(){


  
  }    
void pick_panel::delete_row_4(){


  }    
void pick_panel::change_text(const QString txt){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("text");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void pick_panel::group_change(const QString txt){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("group");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void pick_panel::view_change(const QString txt){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("view");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void pick_panel::single_change(const QString txt){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("single");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void pick_panel::extra_change(const QString txt){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("extra");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void pick_panel::distance_change(int txt){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("distance");
  coms.push_back(QString::number(txt));
  emit actionDetected(coms);
}
void pick_panel::display_points(){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("display");
  coms.push_back("points");
  emit actionDetected(coms);
}
void pick_panel::display_text(){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("display");
  coms.push_back("text");
  emit actionDetected(coms);
}
void pick_panel::display_line(){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("display");
  coms.push_back("line");
  emit actionDetected(coms);
}
void pick_panel::load_picks(){

 QString filename = QFileDialog::getOpenFileName(this );
       if ( !filename.isNull() ){
       std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
       coms.push_back("load");
       coms.push_back(filename);
       emit actionDetected(coms);
       }
}
void pick_panel::save_picks(){

 QString filename = QFileDialog::getSaveFileName(this );
       if ( !filename.isNull() ){
       std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
       coms.push_back("save");
       coms.push_back(filename);
       emit actionDetected(coms);
       }
}
void pick_panel::increase_size(){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("size");
  coms.push_back("increase");
  emit actionDetected(coms);
}
void pick_panel::decrease_size(){
  std::vector<QString> coms;
  coms.push_back("-1"); coms.push_back("pick");
  coms.push_back("size");
  coms.push_back("decrease");
  emit actionDetected(coms);
}


void pick_panel::auto_type(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
  com.push_back(tr("type")); com.push_back(text);
  if(!(text.contains("hoose")>0)) emit actionDetected(com);

}
void pick_panel::auto_len(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("cor")); com.push_back(text);
	if(!(text.contains("hoose")>0)) 	emit actionDetected(com);

}
void pick_panel::auto_maxd(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("dist")); com.push_back(text);
	if(!(text.contains("hoose")>0)) 	emit actionDetected(com);

}
void pick_panel::auto_error(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("error_lloyd")); com.push_back(text);
		emit actionDetected(com);

}
void pick_panel::auto_do() {


	 std::vector <QString> com; com.push_back("auto");
	 com.push_back(tr("pick"));
	  com.push_back(tr("run2d"));
	    com.push_back(tr("do"));
	    	emit actionDetected(com);

}
void pick_panel::auto_accept() {

	 std::vector <QString> com; com.push_back("auto");
	 com.push_back(tr("pick"));
	  com.push_back(tr("accept"));
	    com.push_back(tr("do"));
	    	emit actionDetected(com);

}
void pick_panel::auto_decline() {

	 std::vector <QString> com; com.push_back("auto");
	 com.push_back(tr("pick"));
	  com.push_back(tr("decline"));
	    com.push_back(tr("do"));
	    	emit actionDetected(com);

}
void pick_panel::auto_extend() {

	 std::vector <QString> com; com.push_back("auto");
	 com.push_back(tr("pick"));
	  com.push_back(tr("extend"));
	    com.push_back(tr("do"));
	    	emit actionDetected(com);

}
void pick_panel::auto_method(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("method")); com.push_back(text);
		emit actionDetected(com);

}
void pick_panel::auto_direction(const QString& text) {
  std::vector <QString> com; com.push_back("auto");
   com.push_back(tr("plane")); com.push_back(text);
		emit actionDetected(com);

}
