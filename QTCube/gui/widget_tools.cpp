#include "widget_tools.h"
using namespace SEP;
basicComboBox::basicComboBox(QString name, QString tool, std::vector<QString> options){

 my_box=new QGroupBox(name);
 my_lay=new QHBoxLayout();
 
 addWidget(name,tool,options);
 my_box->setLayout(my_lay);
 
}
void basicComboBox::addWidget(QString name, QString tool, std::vector<QString> options){

my_combos.push_back(new QComboBox());
int i=my_combos.size()-1;
 my_combos[i]->setToolTip(tool);
 for(int ii=0; ii< (int)options.size(); ii++) {
   my_combos[i]->addItem(options[ii]);
   
   }
 my_lay->addWidget(my_combos[i]);

}
void basicComboBox::delete_basic_combo_box(){

  for(int i=0; i <(int)my_combos.size(); i++) delete my_combos[i];
  delete my_lay;
  delete my_box;
  my_combos.clear();
  
}



basicLineEditBox::basicLineEditBox(QString box, int length, QString def, QString tool){


 

 my_box=new QGroupBox(box);
 my_lay=new QHBoxLayout();
 
 addLineEdit(length,def,tool);
 my_box->setLayout(my_lay);
 
}
void basicLineEditBox::addLineEdit(int length,QString def, QString tool){

my_edits.push_back(new QLineEdit());
int i=my_edits.size()-1;
 my_edits[i]->setToolTip(tool);
 my_edits[i]->setMaxLength(length);
 my_edits[i]->setText(def); 
 my_lay->addWidget(my_edits[i]);

}
void basicLineEditBox::delete_basic_line_edit_box(){

  for(int i=0; i <(int)my_edits.size(); i++) delete my_edits[i];
  delete my_lay;
  delete my_box;
  my_edits.clear();
  
}

basicRadioBox::basicRadioBox(QString box, QString name, QString tool){

 my_box=new QGroupBox(box);
 my_lay=new QHBoxLayout();
 
 addButton(name,tool);
 my_box->setLayout(my_lay);
 
}
void basicRadioBox::set_checked(int i){


my_radios[i]->setChecked(true);

}
void basicRadioBox::addButton(QString name, QString tool){

my_radios.push_back(new QRadioButton(name));
int i=my_radios.size()-1;
 my_radios[i]->setToolTip(tool);
 my_lay->addWidget(my_radios[i]);
}
void basicRadioBox::delete_basic_radio_box(){

  for(int i=0; i <(int)my_radios.size(); i++) delete my_radios[i];
  delete my_lay;
  delete my_box;
  my_radios.clear();
  
}
int basicRadioBox::what_checked(){
  for(int i=0; i < (int) my_radios.size(); i++){
    if(my_radios[i]->isChecked()) return i;
  }
  return 0;
}
basicVRadioBox::basicVRadioBox(QString box, QString name, QString tool){

 my_box=new QGroupBox(box);
 my_lay=new QVBoxLayout();
 
 addButton(name,tool);
 my_box->setLayout(my_lay);
 
}

void basicVRadioBox::set_checked(int i){


my_radios[i]->setChecked(true);

}
void basicVRadioBox::addButton(QString name, QString tool){

my_radios.push_back(new QRadioButton(name));
int i=my_radios.size()-1;
 my_radios[i]->setToolTip(tool);
 my_lay->addWidget(my_radios[i]);
}
void basicVRadioBox::delete_basic_radio_box(){

  for(int i=0; i <(int)my_radios.size(); i++) delete my_radios[i];
  delete my_lay;
  delete my_box;
  my_radios.clear();
  
}





basicButtonBox::basicButtonBox(QString box, QString name, QString tool,bool icon){

 my_box=new QGroupBox(box);
 my_lay=new QHBoxLayout();
 
 addButton(name,tool,icon);
 my_box->setLayout(my_lay);
 
}



void basicButtonBox::addButton(QString name, QString tool,bool icon){

if(!icon){
  my_radios.push_back(new QPushButton(name));
}
else{
    my_radios.push_back(new QPushButton(QIcon(QPixmap(name)),""));
}
int i=my_radios.size()-1;
// my_radios[i]->setToolTip(tool);
 my_lay->addWidget(my_radios[i]);
}
void basicButtonBox::delete_basic_button_box(){

  for(int i=0; i <(int)my_radios.size(); i++) delete my_radios[i];
  delete my_lay;
  delete my_box;
  my_radios.clear();
  
}



basicSliderBox::basicSliderBox(QString box,  QString tool,int mn, int def, int mx){

 my_box=new QGroupBox(box);
 my_lay=new QHBoxLayout();
 
 addSlider(tool,mn,def,mx);
 my_box->setLayout(my_lay);
 
}

void basicSliderBox::addSlider(QString tool,int mn, int def, int mx){

my_sliders.push_back(new QSlider(Qt::Horizontal));
int i=my_sliders.size()-1;
 my_sliders[i]->setToolTip(tool);
 my_sliders[i]->setRange(mn,mx);
 my_sliders[i]->setValue(def);
 my_lay->addWidget(my_sliders[i]);
}
void basicSliderBox::delete_basic_slider_box(){

  for(int i=0; i <(int)my_sliders.size(); i++) delete my_sliders[i];
  delete my_lay;
  delete my_box;
  my_sliders.clear();
  
}
