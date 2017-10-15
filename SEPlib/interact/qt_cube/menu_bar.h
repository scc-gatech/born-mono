#ifndef MENUBAR_H
#define MENUBAR_H
#include "help_menu.h"
#include "navigate_menu.h"
#include "orient_menu.h"
#include "pick_menu.h"
#include "view_menu.h"
#include "main_menu.h"
//#include "clip_menu.h"
#include "autopick_menu.h"
#include <map>
#include <qtabwidget.h>
#include "param_func.h"

class menu_bar : public QTabWidget {
        Q_OBJECT
public:
  menu_bar(param_func *p, datasets *d,int ndims, QString num);
  void actionRespond(std::vector<QString> com);

    menu_bar();
  public slots:
    void receive_menu(std::vector <QString> com);
  signals:
  	void actionDetected(std::vector<QString> com);

  public:
    QString vnum;
    std::map<QString, menu_base *> parts;
    std::vector < QString> order;
    
};
#endif
