#ifndef HELP_MENU_H
#define HELP_MENU_H 1
#include "menu_base.h"
#include<qtextedit.h>
class help_menu: public menu_base{

  public:
    help_menu(datasets * dats, QString num, int nd);
    void createit();
    void layout();
  private:
    QTextEdit *ModesWidget, *MouseWidget, *CommandWidget, *KeysWidget, *HistWidget, *ClipWidget, *LoadWidget;

        std::vector<QTextEdit *> edits;
};
#endif
