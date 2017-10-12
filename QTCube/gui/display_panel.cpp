#include "display_panel.h"
using namespace SEP;
display_panel:: display_panel(std::shared_ptr<position>p,std::shared_ptr<windows> my_w, 
std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk,
std::shared_ptr< slice_types>c,std::shared_ptr<maps> mym){




  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  layMain=new QVBoxLayout;
  row1=build_row_1();
  row2=build_row_2();
  row3=build_row_3();
  layMain->addWidget(row1);
  layMain->addWidget(row2);
  layMain->addWidget(row3);
  setLayout(layMain);
  
  }

void display_panel::update_menu(std::vector<QString> coms){


}
QWidget *display_panel::build_row_1(){
    QWidget *mine=new QWidget();
    lay1=new QHBoxLayout();
    
    fontBox=new basicComboBox("Font","Font to display",my_f->font_list);
    
    std::vector<QString> cvec=my_c->return_cvec();
  
    colorBox=new basicComboBox("Line Color","Color of lines/axes",cvec);
    colorBox->set_current(0,8);
    title= new basicRadioBox("Title","Yes","Display title");
    title->addButton("No","Don't display title");
    title->set_checked(0);    
    posSlide=new basicSliderBox("Draw Pos","Transparency of position",0,100,100);


     connect(fontBox->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_font(const QString&)));
          connect(colorBox->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_color(const QString&)));

 	connect(title->my_radios[0], SIGNAL(clicked()), this, SLOT(yes_title()));
 	connect(title->my_radios[1], SIGNAL(clicked()), this, SLOT(no_title()));
     	connect(posSlide->my_sliders[0], SIGNAL(valueChanged(int)), this, SLOT(draw_pos (int)));




    lay1->addWidget(fontBox->group());
        lay1->addWidget(colorBox->group());

    lay1->addWidget(title->group());
    lay1->addWidget(posSlide->group());
    lay1->addStretch(1);
     mine->setLayout(lay1);

     return mine;
  }
  
QWidget * display_panel::build_row_2(){
      QWidget *mine=new QWidget();
      lay2=new QHBoxLayout();
  
    axes1= new basicRadioBox("Axis 1","Yes","Display axis 1");
    axes1->addButton("No","Don't display axis 1");
    axes1->set_checked(0);  
  
    
    axes2= new basicRadioBox("Axis 2","Yes","Display axis 2");
    axes2->addButton("No","Don't display axis 2");
    axes2->set_checked(0);  
  
    
    axes3= new basicRadioBox("Axis 3","Yes","Display axis 3");
    axes3->addButton("No","Don't display axis 3");
    axes3->set_checked(0);  
  
  
   	connect(axes1->my_radios[0], SIGNAL(clicked()), this, SLOT(yes_axis1()));
  	connect(axes1->my_radios[1], SIGNAL(clicked()), this, SLOT(no_axis1()));
  	 	connect(axes2->my_radios[0], SIGNAL(clicked()), this, SLOT(yes_axis2()));
  	connect(axes2->my_radios[1], SIGNAL(clicked()), this, SLOT(no_axis2()));
  	  	 	connect(axes3->my_radios[0], SIGNAL(clicked()), this, SLOT(yes_axis3()));
  	connect(axes3->my_radios[1], SIGNAL(clicked()), this, SLOT(no_axis3()));
  	

    lay2->addWidget(axes1->group());
    lay2->addWidget(axes2->group());
    lay2->addWidget(axes3->group());
    lay2->addStretch(1);
     mine->setLayout(lay2);

     return mine;
  
  }
QWidget * display_panel::build_row_3(){
      QWidget *mine=new QWidget();
      lay3=new QHBoxLayout();
  
    picks= new basicRadioBox("Picks","Yes","Display picks");
    picks->addButton("No","Don't display picks");
    picks->set_checked(0);  
    
    grid= new basicRadioBox("Grid","Yes","Display grid");
    grid->addButton("No","Don't display grid");
    grid->set_checked(1);  
  
    colorbar= new basicRadioBox("Colorbar","Yes","Display colorbar");
    colorbar->addButton("No","Don't display color bar");
    colorbar->set_checked(1);
  
    connect(picks->my_radios[0], SIGNAL(clicked()), this, SLOT(yes_picks()));
  	connect(picks->my_radios[1], SIGNAL(clicked()), this, SLOT(no_picks()));
  	connect(grid->my_radios[0], SIGNAL(clicked()), this, SLOT(yes_grid()));
  	connect(grid->my_radios[1], SIGNAL(clicked()), this, SLOT(no_grid()));
  	connect(colorbar->my_radios[0], SIGNAL(clicked()), this, SLOT(yes_colorbar()));
 	connect(colorbar->my_radios[1], SIGNAL(clicked()), this, SLOT(no_colorbar()));
  	

  
    lay3->addWidget(picks->group());
    lay3->addWidget(grid->group());
        lay1->addWidget(colorbar->group());

    lay3->addStretch(1);
    
     mine->setLayout(lay3);

     return mine;
  }
void display_panel::delete_row_1(){
    delete row1;
    delete lay1;
    delete fontBox;
    delete title;
    delete posSlide;
  }
  
void display_panel::delete_row_2(){
    delete axes2;
    delete axes1;
    delete axes3;
    delete row2;
    delete lay2;
  
  }    
void display_panel::delete_row_3(){
      delete grid;
      delete picks;
          delete colorbar;

      delete row3;
    delete lay3;
  
  }    

void display_panel::change_font(const QString fnt){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("font");
  coms.push_back(fnt);
  emit actionDetected(coms);
}
void display_panel::change_color(const QString fnt){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("color");
  coms.push_back(fnt);
  emit actionDetected(coms);
}

void display_panel::yes_colorbar(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("colorbar");
  coms.push_back("yes");
  emit actionDetected(coms);
}
void display_panel::no_colorbar(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("colorbar");
  coms.push_back("no");
  emit actionDetected(coms);
}
void display_panel::yes_title(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("title");
  coms.push_back("yes");
  emit actionDetected(coms);
}
void display_panel::no_title(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("title");
  coms.push_back("no");
  emit actionDetected(coms);
}

void display_panel::draw_pos(int alpha){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("pos_alpha");
  coms.push_back(QString::number(alpha));
  emit actionDetected(coms);
}
void display_panel::yes_axis1(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("axis1");
  coms.push_back("yes");
  emit actionDetected(coms);
}
void display_panel::no_axis1(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("axis1");
  coms.push_back("no");
  emit actionDetected(coms);
}

void display_panel::yes_axis2(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("axis2");
  coms.push_back("yes");
  emit actionDetected(coms);
}
void display_panel::no_axis2(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("axis2");
  coms.push_back("no");
  emit actionDetected(coms);
}
void display_panel::yes_axis3(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("axis3");
  coms.push_back("yes");
  emit actionDetected(coms);
}
void display_panel::no_axis3(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("axis3");
  coms.push_back("no");
  emit actionDetected(coms);
}

void display_panel::yes_picks(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("picks");
  coms.push_back("yes");
  emit actionDetected(coms);
}
void display_panel::no_picks(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("picks");
  coms.push_back("no");
  emit actionDetected(coms);
}
void display_panel::yes_grid(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("grid");
  coms.push_back("yes");
  emit actionDetected(coms);
}
void display_panel::no_grid(){
  std::vector<QString> coms;
  coms.push_back("display");
  coms.push_back("grid");
  coms.push_back("no");
  emit actionDetected(coms);
}
