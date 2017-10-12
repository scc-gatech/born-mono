 #pragma once 
#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H
#include <qmainwindow.h>
#include <QMenuBar>
#include <QLabel>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QAction>
#include "position.h"
#include <genericIO.h>
#include "slice_types.h"
//#include "pick_groups.h"
#include "pick_draw.h"
#include "qsplitter.h"
#include "datasets.h"
#include "drawwindow.h"
#include "maps.h"
#include "mode.h"
#include "slice_types.h"
#include "util.h"
#include <map>
#include "draw_other.h"
#include "windows.h"
#include "panels.h"
#include <QMessageBox>
#include "window_panel.h"
#include "panel_panel.h"
#include "color_panel.h"
#include "help_panel.h"
#include "ano_panel.h"
#include "data_panel.h"
#include "history.h"
#include "orient_cube.h"
#include "pick_panel.h"


 #include <QMainWindow>

 class QAction;
 class QActionGroup;
 class QLabel;
 class QMenu;
namespace SEP{
 class MainWindow : public QMainWindow
 {
     Q_OBJECT

 public:
     MainWindow(std::shared_ptr<SEP::genericIO>io ,std::shared_ptr<SEP::datasets>d,std::shared_ptr<SEP::pick_draw> _pk,
	  std::shared_ptr<SEP::slice_types> ct);
 std::shared_ptr<SEP::history> my_hist;


 private slots:
     void newFile();
     void open();
     void save();
     void openPick();
     void closePick();
     void openData();
     void closeData();
    void closeHelp();
    void openHelp();
    void openAnno();
    void closeAnno();
    void openColor();
    void closeColor();

    void statusMouse();
    void statusPosition();
    void statusOverlay();
    void statusPick();
    
    void mousePick();
    void mouseNavigate();
    void mouseRegion();
    void mouseOverlay();
    void mouseAnnotate();
         void mouseZoom();
   void lockViews();
   void unlockViews();
   void create_mouse_funcs();
   void create_panels();
   void create_windows();
   void actionRespond(std::vector<QString>);
      void secondaryRespond(std::vector<QString>);

   void connect_it();

     void about();
     void aboutQt();

 private:
 void update_status_bar();
  std::shared_ptr<SEP::window_panel>my_window_panel;
 std::shared_ptr<SEP::orientation_server>serv;
 std::shared_ptr<SEP::panel_panel>my_panel_panel;
 std::shared_ptr<SEP::data_panel>my_data_panel;
 std::shared_ptr<SEP::help_panel>my_help_panel;
 std::shared_ptr<SEP::ano_panel>my_ano_panel;
 std::shared_ptr<SEP::color_panel>my_color_panel;
 std::shared_ptr<SEP::pick_panel>my_pick_panel;
 std::shared_ptr<SEP::genericIO> _io;
  std::shared_ptr<SEP::panels>my_pan;
  std::shared_ptr<SEP::windows>my_wind;
  std::shared_ptr<SEP::position>pos;
  std::shared_ptr<SEP::maps>my_maps;
	std::shared_ptr<SEP::pick_draw>pk;
	     std::shared_ptr<SEP::paramObj>pars;
     std::shared_ptr<SEP::util> pp;
     std::shared_ptr<SEP::datasets>datas;
     QString stat_view;
     std::shared_ptr<SEP::slice_types>colort;
     	std::shared_ptr<SEP::orients>my_or;

     void createActions();
     void createMenus();
     QMenu *mainMenu;
     QMenu *mouseMenu;
          QMenu *statusMenu;
    std::map < QString, std::shared_ptr<mouse_func>> funcs;
    std::shared_ptr<mouse_func>cur_func;
     QMenu *historyMenu;
     QMenu *panelMenu,*helpMenu,*help2Menu;
     QMenu *mouseFuncMenu;
     QMenu *dataMenu,*pickMenu,*annoMenu;
     QMenu *formatMenu,*colorMenu;
     QActionGroup *alignmentGroup;
     QAction *newAct;
     QAction *yesColorAct,*noColorAct;
     QAction *yesDataAct,*noDataAct;
   QAction *yesPickAct,*noPickAct;
   QAction *yesHelpAct,*noHelpAct;
   QAction *yesAnnoAct,*noAnnoAct;
   QAction *mAnnoAct,*mPickAct,*mNavAct,*mRegionAct,*mOverAct,*mZoomAct;
   QAction *sMouseAct,*sPositionAct,*sOverlayAct,*sPickAct;
   QAction *lockAct,*unlockAct;
     QAction *openAct;
     QAction *saveAct;
     QAction *exitAct;
     QAction *aboutAct;
     QAction *aboutQtAct;
     QLabel *modeLabel;
     QString mouse_lab;
 };
}
 #endif



