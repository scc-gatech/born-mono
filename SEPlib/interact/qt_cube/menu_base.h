#ifndef MENU_BASE_H
#define MENU_BASE_H 1
#include<qwidget.h>
#include <q3mimefactory.h>

#include <QHBoxLayout>
#include<string.h>
#include<qgroupbox.h>
#include<qcombobox.h>
#include<qobject.h>
#include<qpushbutton.h>
#include<qlabel.h>
#include<qslider.h>
#include<QLineEdit>
#include "datasets.h"

#include<qradiobutton.h>
#include <qtooltip.h>

#include<qfiledialog.h>
#include <qlayout.h>
#include <vector>
#include <qtabwidget.h>
#define BUTTON_SIZE 30
#define TAB_HEIGHT 30
#define LAYOUT_MARGIN_SMALL 2
#define LAYOUT_SPACING_SMALL 5
#define LAYOUT_MARGIN_MED 5
#define LAYOUT_SPACING_MED 5

#define MINIMUM_HEIGHT_MENU 30

class menu_base: public QTabWidget{
  Q_OBJECT
  public:
    menu_base():QTabWidget(){};
    virtual void create_menu(){
        createit();
        layout();
    }
    virtual void createit(){}
    virtual void layout(){}
    void set_basics(datasets *dats, QString num,
    QString sub, int nd);
    virtual void actionRespond(std::vector<QString> com){
      if(com[0]==""){}
      }
        
    
    QHBoxLayout* lay;
  public slots:
    void send_message(std::vector<QString> com);
  signals:
    void action_detected(std::vector<QString> com);


  public:

    datasets *datas;
    QString vnum,sub_name;
    int ndims;
};
#endif
