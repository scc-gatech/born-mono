#ifndef WIDGET_TOOLS_H
#define WIDGET_TOOLS_H 1
#include <QHBoxLayout>
#include <QComboBox>
#include <QGroupBox>
#include <QRadioButton>
#include <vector>
#include<QSlider>
#include <QLineEdit>
#include <QPushButton>
#include <QObject>
namespace SEP{
class basicComboBox{
  public:
    basicComboBox(QString name, QString tool, std::vector<QString>);
    void addWidget(QString name, QString tool, std::vector<QString>);
    QComboBox *combo(int i){ return my_combos[i];}
    QGroupBox *group(){return my_box;}
    void set_current(int which,int value) { my_combos[which]->setCurrentIndex(value);}
        std::vector<QComboBox *>my_combos;

    int get_index(int com){return my_combos[com]->currentIndex();}
 

 private:
    void delete_basic_combo_box();
    std::vector<QString> command;
    QGroupBox *my_box;
    QHBoxLayout *my_lay;

};

class basicButtonBox{
  public:
    basicButtonBox(QString box, QString name, QString tool,bool icon);
  void addButton(QString name,QString tool, bool icon);

    QPushButton *radio(int i){ return my_radios[i];}
    QGroupBox *group(){return my_box;}
 
    ~basicButtonBox(){ delete_basic_button_box();}
 
    std::vector<QPushButton *>my_radios;

 private:
    void delete_basic_button_box();
    QGroupBox *my_box;
    QHBoxLayout *my_lay;

};

class basicLineEditBox{
  public:
    basicLineEditBox(QString box,int length,QString def, QString tool);
  void addLineEdit(int length, QString def,QString tool);
   void set_value(int ib,QString val){ my_edits[ib]->setText(val);}
    QLineEdit *edit(int i){ return my_edits[i];}
    QGroupBox *group(){return my_box;}
 
    ~basicLineEditBox(){ delete_basic_line_edit_box();}
 
    std::vector<QLineEdit *>my_edits;

 private:
    void delete_basic_line_edit_box();
    QGroupBox *my_box;
    QHBoxLayout *my_lay;

};

class basicRadioBox{
  public:
    basicRadioBox(QString box, QString name, QString tool);
    void addButton(QString name, QString tool);
    QRadioButton *radio(int i){ return my_radios[i];}
    QGroupBox *group(){return my_box;}
    void set_checked(int i);
     int what_checked();

    ~basicRadioBox(){ delete_basic_radio_box();}
     std::vector<QRadioButton *>my_radios;


 private:
    void delete_basic_radio_box();
    QGroupBox *my_box;
    QHBoxLayout *my_lay;

};
class basicVRadioBox{
  public:
    basicVRadioBox(QString box, QString name, QString tool);
    void addButton(QString name, QString tool);
    QRadioButton *radio(int i){ return my_radios[i];}
    QGroupBox *group(){return my_box;}
    void set_checked(int i);
 
    ~basicVRadioBox(){ delete_basic_radio_box();}
     std::vector<QRadioButton *>my_radios;


 private:
    void delete_basic_radio_box();
    QGroupBox *my_box;
    QVBoxLayout *my_lay;

};

class basicSliderBox{
  public:
    basicSliderBox(QString box, QString tool,int mn, int def, int mx);
    void addSlider( QString tool,int mn, int def, int mx);
    QSlider *slider(int i){ return my_sliders[i];}
    QGroupBox *group(){return my_box;}
 
    ~basicSliderBox(){ delete_basic_slider_box();}
 
    std::vector<QSlider *>my_sliders;

 private:
    void delete_basic_slider_box();
    QGroupBox *my_box;
    QHBoxLayout *my_lay;

};
}
#endif






