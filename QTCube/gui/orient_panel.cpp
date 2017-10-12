#include "orient_panel.h"
using namespace SEP;

orient_panel:: orient_panel(std::shared_ptr<position>p,
std::shared_ptr<windows>my_w, std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, 
std::shared_ptr<pick_draw> pk, std::shared_ptr<slice_types>c,std::shared_ptr<maps> mym){




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
  layMain->addWidget(row1);
  layMain->addWidget(row2);
  layMain->addWidget(row3);
  setLayout(layMain);
  
  }
  void orient_panel::update_menu(std::vector<QString>){


}
QWidget *orient_panel::build_row_1(){
    QWidget *mine=new QWidget();
    lay1=new QHBoxLayout();
    

    flipAxes=new basicButtonBox("Flip axes","Reset","Reset orientation",false);
        flipAxes->addButton(":/images/down-arrow.png","Flip axis 1",true);
    flipAxes->addButton(":/images/across-arrow.png","Flip axis 2",true);
    flipAxes->addButton(":/images/deep-arrow.png","Flip axis 3",true);

    frameOrient=new basicButtonBox("Frame Orient",":/images/middle.png","Move to center",true);
    frameOrient->addButton(":/images/corner.png","Go to the first edge",true);

   std::vector<QString> options;
    for(int i=0; i < 8; i++){
      if(validax[i]){
        for(int j=i+1; j <8; j++){
           if(validax[j]){
            if(i==0 && j==1) options.push_back("12: Side and top");
            else if(i==0 && j==2) options.push_back("13: Top and front");
            else if(i==1 && j==2) options.push_back("23: Side and front");
            else        options.push_back(QString::number(i+1)+QString::number(j+1));
          }
        }
    }
  }
   transpAxes= new basicComboBox("Transpose","Transpose plane",options);


  	connect(flipAxes->my_radios[0], SIGNAL(clicked()), this, SLOT(reset_orientation()));

  	connect(flipAxes->my_radios[1], SIGNAL(clicked()), this, SLOT(flip_1()));
  	connect(flipAxes->my_radios[2], SIGNAL(clicked()), this, SLOT(flip_2()));
  	connect(flipAxes->my_radios[3], SIGNAL(clicked()), this, SLOT(flip_3()));
  	
  	connect(frameOrient->my_radios[0], SIGNAL(clicked()), this, SLOT(center()));
  	connect(frameOrient->my_radios[1], SIGNAL(clicked()), this, SLOT(edge()));
     connect(transpAxes->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(transpose(const QString&)));

  
    lay1->addWidget(flipAxes->group());
    lay1->addWidget(frameOrient->group());
    lay1->addWidget(transpAxes->group());
     mine->setLayout(lay1);

     return mine;
  }
  
QWidget * orient_panel::build_row_2(){
      QWidget *mine=new QWidget();
      lay2=new QHBoxLayout();
      angle2=new basicSliderBox("-44 Angle 44","Set the angle to rotate",-44,0,44);
      angle1=new basicLineEditBox("Angle", 8,"0","Set the angle to rotate");
      std::vector<QString> options; options.push_back("12"); options.push_back("13"); options.push_back("23");
     rotateAxes=new basicComboBox("Rotate axes","Which axes to rotate",options);
     rotation= new basicButtonBox("Rotation","Reset","Reset rotation",false);
     rotation->addButton("Set Pt","Set the rotation point",false);
    


	connect(angle2->my_sliders[0], SIGNAL(valueChanged(int)), this, SLOT(angle_int (int)));
    connect(angle1->my_edits[0], SIGNAL(textChanged(const QString&)),this, SLOT(angle_str(const QString&)));
     connect(rotateAxes->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(rotate_axes(const QString&)));

  	connect(rotation->my_radios[0], SIGNAL(clicked()), this, SLOT(reset_rotation()));
  	connect(rotation->my_radios[1], SIGNAL(clicked()), this, SLOT(set_rotation_pt()));


  
     lay2->addWidget(angle2->group());
     lay2->addWidget(angle1->group());
     lay2->addWidget(rotateAxes->group());
     lay2->addWidget(rotation->group());
     mine->setLayout(lay2);

     return mine;
  
  }
QWidget * orient_panel::build_row_3(){
      QWidget *mine=new QWidget();
      lay3=new QHBoxLayout();
      
      proportions = new basicRadioBox("Proportions","True","Display the size based on the values to the right");
      proportions->addButton("False","Display proportions to maximize display space");
      proportions->set_checked(1);

      
      std::shared_ptr<hypercube> h=my_dat->return_dat(0)->return_grid();
      for(int i=0; i < 8; i++) {
         axis a=h->getAxis(i+1);
         axes.push_back(
           new basicLineEditBox(tr("Axis ")+QString::number(i+1),7,QString::number(a.d*a.n),
             tr("Set the relative proportion of axis ")+QString::number(i+1)));
     }
     lay3->addWidget(proportions->group());

     for(int i=0; i < 8; i++){
       if(validax[i]) lay3->addWidget(axes[i]->group());
     }

  	connect(proportions->my_radios[0], SIGNAL(clicked()), this, SLOT(true_prop()));
  	connect(proportions->my_radios[1], SIGNAL(clicked()), this, SLOT(false_prop()));
  



     mine->setLayout(lay3);



     return mine;
  }
void orient_panel::delete_row_1(){
    delete row1;
    delete flipAxes;
    delete transpAxes;
    delete frameOrient;
    delete lay1;
    

  }
  
void orient_panel::delete_row_2(){
    delete angle1;
    delete angle2;
    delete rotateAxes;
    delete rotation;
    delete row2;
    delete lay2;
    
  
  }    
void orient_panel::delete_row_3(){
    for(int i=0; i <8; i++) delete axes[i];
    delete proportions;
      delete row3;
    delete lay3;
  
  }    
 void orient_panel::flip_1(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("flip1");
    emit actionDetected(coms); 
 }
  void orient_panel::reset_orientation(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("reset");
    emit actionDetected(coms); 
 }
 void orient_panel::flip_2(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("flip2");
    emit actionDetected(coms); 
 }
 void orient_panel::flip_3(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("flip3");
    emit actionDetected(coms); 
 }
 void orient_panel::center(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("center");
    emit actionDetected(coms); 
 }
 void orient_panel::edge(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("edge");
    emit actionDetected(coms); 
 }
 void orient_panel::transpose(const QString x){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("transpose");
    coms.push_back(x);
    emit actionDetected(coms); 
 }
  void orient_panel::angle_int(int x){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("angle");
    angle1->set_value(0,QString::number(x));
    coms.push_back(QString::number(x));
    emit actionDetected(coms); 
 }
   void orient_panel::angle_str(const QString x){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("angle");
    bool v;
    int y=x.toInt(&v); 
    if(v){
      angle2->my_sliders[0]->setValue(y);
      coms.push_back(x);
      emit actionDetected(coms); 
    }
 }
  void orient_panel::rotate_axes(const QString x){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("rotate_axes");
    coms.push_back(x);
    emit actionDetected(coms); 
 }
   void orient_panel::reset_rotation(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("reset");
    emit actionDetected(coms); 
 }
    void orient_panel::set_rotation_pt(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("rotation_pt");
    emit actionDetected(coms); 
 }
     void orient_panel::true_prop(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("true_prop");
    bool v=true;
    int i=0;

    while(i < 8 && v){
      if(validax[i]){
        QString x=axes[i]->my_edits[0]->displayText();
        coms.push_back(x);
      }
      else coms.push_back("1");
      i++;
    }
    if(i==8) emit actionDetected(coms);
    

 }
     void orient_panel::false_prop(){
    std::vector<QString> coms;
    coms.push_back("orient");
    coms.push_back("false_prop");
    emit actionDetected(coms); 
 }
  
