#include "navigate_panel.h"
using namespace SEP;
navigate_panel:: navigate_panel(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, 
std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk, 
std::shared_ptr<slice_types>c,std::shared_ptr<maps>mym){




  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  layMain=new QVBoxLayout;
  
  	std::shared_ptr<hypercube> h=my_dat->return_dat(0)->return_grid();
	for(int i=0; i < 8; i++) {
	   if(h->getAxis(i+1).n >1) validax[i]=true;
	   else validax[i]=false;
	}
  
  row1=build_row_1();
  row2=build_row_2();
  row3=build_row_3();
  layMain->setMargin(0);
  layMain->setSpacing(0);
  layMain->addWidget(row1);
  layMain->addWidget(row2);
  layMain->addWidget(row3);
  setLayout(layMain);
  
  }
  
void navigate_panel::update_menu(std::vector<QString>){
   std::shared_ptr<position> myp=my_pan->get_position();
   int iloc[8]; myp->get_locs(iloc);
   
   for(int i=0;i < 8; i++) {
      axis a=myp->getAxis(i);
      pos[i]->set_value(0,QString::number(a.o+a.d*iloc[i]));
  }

}
QWidget *navigate_panel::build_row_1(){
    QWidget *mine=new QWidget();
    lay1=new QHBoxLayout();
    direction = new basicRadioBox("Direction","x","Move in the positive x direction");
    direction->addButton("X","Move in the negative x direction");
    direction->addButton("y","Move in the positive y direction");
    direction->addButton("Y","Move in the negative y direction");
    direction->addButton("z","Move in the positive z direction");
    direction->addButton("Z","Move in the negative z direction");
    direction->set_checked(2);
    increment=new basicLineEditBox("Increment",3,"1","Number of samples to move with each step");
    
    lay1->addWidget(direction->group());
    lay1->addWidget(increment->group());
    lay1->addStretch(1);
     mine->setLayout(lay1);
    connect(direction->my_radios[0], SIGNAL(clicked()), this, SLOT(x_pos_clicked()));
    connect(direction->my_radios[1], SIGNAL(clicked()), this, SLOT(x_neg_clicked()));
    connect(direction->my_radios[2], SIGNAL(clicked()), this, SLOT(y_pos_clicked()));
    connect(direction->my_radios[3], SIGNAL(clicked()), this, SLOT(y_neg_clicked()));
    connect(direction->my_radios[4], SIGNAL(clicked()), this, SLOT(z_pos_clicked()));
    connect(direction->my_radios[5], SIGNAL(clicked()), this, SLOT(z_neg_clicked()));
    connect(increment->my_edits[0], SIGNAL(textChanged(const QString&)),this, SLOT(inc_changed(const QString&)));

     return mine;
  }
  QWidget * navigate_panel::build_row_2(){
      QWidget *mine=new QWidget();
      lay2=new QHBoxLayout();

    movieGo=new basicRadioBox("Movie","Go","Start movie");
    movieGo->addButton("Stop","Stop Movie");
    movieGo->set_checked(1);
    speed=new basicSliderBox("Speed","Speed of the movie",10,100,1000);
    step= new basicButtonBox("Step view","Step","Take a step in the current movie direction",false);


    connect(movieGo->my_radios[0], SIGNAL(clicked()), this, SLOT(go_clicked()));
        connect(movieGo->my_radios[1], SIGNAL(clicked()), this, SLOT(stop_clicked()));
	connect(step->my_radios[0], SIGNAL(clicked()), this, SLOT(step_clicked()));
	connect(speed->my_sliders[0], SIGNAL(valueChanged(int)), this, SLOT(speed_changed (int)));

    

   lay2->addWidget(movieGo->group());
   lay2->addWidget(speed->group());
   lay2->addWidget(step->group());

     mine->setLayout(lay2);
    lay2->addStretch(1);

     return mine;
  
  }
QWidget * navigate_panel::build_row_3(){
      QWidget *mine=new QWidget();
      lay3=new QHBoxLayout();
      
     std::shared_ptr<hypercube> h=my_dat->return_dat(0)->return_grid();
      for(int i=0; i< 8; i++){
        axis a=h->getAxis(i+1);
        pos.push_back(new basicLineEditBox(tr("Axis ")+QString::number(i+1),10,
          QString::number(a.o+a.d*a.n/2.),"Current position"));
     }
     update=new basicButtonBox("Position","Update","Update the current position being displayed",false);
     
    lay3->addWidget(update->group());
    for(int i=0; i< 8; i++){
      if(validax[i]) lay3->addWidget(pos[i]->group());
   }
      
connect(update->my_radios[0], SIGNAL(clicked()),this, SLOT(update_clicked()));


      void go_clicked();
      void stop_clicked();
      void update_clicked();
      void speed_changed(int);
    lay3->addStretch(1);

     mine->setLayout(lay3);

     return mine;
  }
void navigate_panel::delete_row_1(){

    delete direction;
    delete increment;
    delete row1;
    delete lay1;
    

  }
  
void navigate_panel::delete_row_2(){

  delete step;
  delete speed;
  delete movieGo;
    delete row2;
    delete lay2;
    
  
  }    
void navigate_panel::delete_row_3(){
   delete update;
   for(int i=0; i < 8; i++) delete pos[i];
      delete row3;
    delete lay3;
  }    


        void navigate_panel::x_neg_clicked(){
        
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("direction"); coms.push_back("X");
         QString a= increment->my_edits[0]->displayText();
    bool v;  if(!v) a="1"; coms.push_back(a);
    emit actionDetected(coms);
     
        }
      void navigate_panel::x_pos_clicked(){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("direction"); coms.push_back("x");
         QString a= increment->my_edits[0]->displayText();
    bool v; int x= a.toInt(&v); if(!v) a="1"; coms.push_back(a);
    emit actionDetected(coms);
     
    
      }
      void navigate_panel::y_neg_clicked(){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("direction"); coms.push_back("Y");
         QString a= increment->my_edits[0]->displayText();
    bool v;  if(!v) a="1"; coms.push_back(a);
    emit actionDetected(coms);
     
    }
      void navigate_panel::y_pos_clicked(){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("direction"); coms.push_back("y");
         QString a= increment->my_edits[0]->displayText();
    bool v; if(!v) a="1"; coms.push_back(a);
    emit actionDetected(coms);
     
    }
      void navigate_panel::z_neg_clicked(){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("direction"); coms.push_back("Z");
         QString a= increment->my_edits[0]->displayText();
    bool v;  if(!v) a="1"; coms.push_back(a);
    emit actionDetected(coms);
     
    }
      void navigate_panel::z_pos_clicked(){
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("direction"); coms.push_back("z");
           QString a= increment->my_edits[0]->displayText();
    bool v;  if(!v) a="1"; coms.push_back(a);
    emit actionDetected(coms);
      }
      void navigate_panel::go_clicked(){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("go");
    emit actionDetected(coms);
      
      }
           void navigate_panel::step_clicked(){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("step");
             QString a= increment->my_edits[0]->displayText();

      bool v; int x= a.toInt(&v); if(!v) a="1"; coms.push_back(a);

    emit actionDetected(coms);
      
      }
      void navigate_panel::stop_clicked(){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("stop");
    emit actionDetected(coms);
      
      }
      void navigate_panel::update_clicked(){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("update");
    float poz[8];
  int iloc[8];
  bool v=true;
  QString a=pos[0]->my_edits[0]->displayText();  
  if(validax[0]) poz[0]=a.toFloat(&v);
  else  poz[0]=0.;
  if(v) {  if(validax[1]){a=pos[1]->my_edits[0]->displayText();  poz[1]=a.toFloat(&v);}else poz[1]=0.;
  if(v) {  if(validax[2]){a=pos[2]->my_edits[0]->displayText();  poz[2]=a.toFloat(&v);}else poz[2]=0.;
  if(v) { if(validax[3]){ a=pos[3]->my_edits[0]->displayText();  poz[3]=a.toFloat(&v);}else poz[3]=0.;
  if(v) { if(validax[4]){ a=pos[4]->my_edits[0]->displayText();  poz[4]=a.toFloat(&v);}else poz[4]=0.;
  if(v) {  if(validax[5]){a=pos[5]->my_edits[0]->displayText();  poz[5]=a.toFloat(&v);}else poz[5]=0.;
  if(v) { if(validax[6]){ a=pos[6]->my_edits[0]->displayText();  poz[6]=a.toFloat(&v);}else poz[6]=0.;
  if(v) { if(validax[7]){ a=pos[7]->my_edits[0]->displayText();  poz[7]=a.toFloat(&v);} else poz[7]=0.;
  //  window->return_grid_loc(pos,iloc);
    a=util::string_from_float_array(8,poz);
    coms.push_back(a);
        emit actionDetected(coms);

 }}}}}}}
  
      }
      void navigate_panel::inc_changed(const QString&txt){
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("increment"); 
    bool v;
    txt.toFloat(&v);
    if(v){coms.push_back(txt);
    emit actionDetected(coms);
      }
      }
      void navigate_panel::speed_changed(int val){
      
              
         std::vector<QString> coms;
  coms.push_back("navigate"); coms.push_back("speed"); coms.push_back(QString::number(val));
    emit actionDetected(coms);
      }
