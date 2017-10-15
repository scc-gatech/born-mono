
#ifndef CENTERWINDOW_H
#define CENTERWINDOW_H
#include <qmainwindow.h>
#include <QLabel>
#include <QKeyEvent>
#include <QCloseEvent>
#include "position.h"
#include "slice_types.h"
//#include "pick_groups.h"
#include "pick_draw.h"
#include "menu_bar.h"
#include "qsplitter.h"
#include "datasets.h"
#include "drawwindow.h"
#include "mode.h"
#include "statuswindow.h"
#include "clipwindow.h"
#include "updater.h"
#include "slice_types.h"
#include <map>
#include "draw_other.h"

class CenterWindow : public QMainWindow
{
	Q_OBJECT

public:
   // CenterWindow( QWidget* parent = 0){};
	CenterWindow( param_func *pars,datasets *d,pick_draw *_pk,
	  slice_types *ct,  QWidget* parent = 0);
	void display_it();
	void set_active(int iact);
	updater *up;
protected:
	void closeEvent(QCloseEvent* event);
		void resizeEvent( QResizeEvent * );


private slots:
  void actionRespond(std::vector<QString>);
	

 

private:
  StatusWindow *statwindow;
    ClipWindow *clipwindow;

	QSplitter* mainBox;
	QSplitter* leftBox;
	QSplitter* rightBox;
	QSplitter* topBox;
	QSplitter* midBox;
	std::vector<QSplitter*> mainViews;
	QFontMetrics *fm;
	QVBoxLayout* lay;

	QSplitter* middleBox;
    datasets *datas;
	// top menu area
	menu_bar* menuBar;
	std::vector<menu_bar*>menuBars;

	// center drawing window
	std::vector<DrawWindow*> windows;
	DrawWindow *active;
  void keyReleaseEvent( QKeyEvent *e);
	// bottom control area
	QLabel* statusLabel, *modeLabel, *windowLabel,*posLabel;
	slice_types *colort;
	QString windowString;
	QString holder;
	QTimer *movie_timer;

	//popup array control

	// functions
	void createComponents(param_func *pars);
	void createStatusListView();
	void createHistoryListView();
	void writeSettings();
	void readSettings();
	
  void pick_menu(const QString&text);
  void navigate_action(const QString&text);
  void view_action(const QString&text);
  void help_action(const QString&text);  
  void main_action(const QString&text);  
  void orient_action(const QString&text);
  void color_action(const QString&text);
  void picks_action(const QString&text);
  void overlay_color_action(const QString&text);
  void load_tour(QString text);
  QWidget * newWindow;
  std::vector<bool> joined;
  std::vector <QWidget*> indepChildWindows;
  void MakeWindows();
 // void MakeOtherWindow(param_func *pars);
  void DestroyWindow(bool);
  void WindowHandler(bool);


  void change_mouse(QString);
    void change_status(QString);
   void update_status_bar();

	position *pos;
	pick_draw *pk;
  mode *mym;
	int iactive;
	int nviews;
  std::vector<int> over_c;
  std::vector<int> under_c;
	std::vector<QString> combos;
	QString link_com;
	std::vector<QString> tour;
	int itour;
    std::map < QString, mouse_func *> funcs;
    mouse_func *cur_func;
     param_func *pars;

	int multimode;
	int viewnumber;
	int movie_delay;
	orientation_server *serv;
	int curw,curh;
	bool independent;
  QString mouse_lab;
  QString stat_view;

	
};

#endif


