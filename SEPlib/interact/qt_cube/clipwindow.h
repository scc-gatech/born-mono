
#ifndef ClipWindow_H
#define ClipWindow_H
#include <qwidget.h>
#include "clip_bar.h"
#include "color_bar.h"
#include "datasets.h"
#include "drawwindow.h"
#include "position.h"

#include<vector>
#include<QTreeWidget>
#include<QListWidget>
#include<vector>
#include<change.h>
#include<qwidget.h>

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
#include "slice_types.h"
#define MINIMUM_HEIGHT_MENU 30

class ClipWindow : public QTabWidget
{
    Q_OBJECT
public:
    ClipWindow( slice_types *colt,position *pos, datasets *ds, int nviews, 
    std::vector<DrawWindow *>winds,QWidget *parent = 0, const char *name = 0 );
        void setValue(int, int, int , int , int);
  
     void create_clips(datasets *data,slice_types *colt);
 
     void createit(QWidget *widget,clip_bar *bar,int i);

     QString return_pos_lab(){return pos_lab;}
     QString return_val_lab(){return val_lab;}
    virtual void actionRespond(std::vector<QString> com);


 signals:
    void actionDetected(std::vector <QString>);
 private slots:
       void set_source(const QString& text);

      void bactivated(const QString& text);
    void eactivated(const QString& text);
      void yesButtonClicked();
      void noButtonClicked();
      void changeContrast(int);
      void clearButtonClicked();
      void picksSaveButtonClicked() ;
void picksLoadButtonClicked() ;
float *second_deriv(int npts, float *x, float *y);
   
    void send_it(std::vector <QString> com);

 private:
	QTreeWidgetItem *list_poss[8];
	QTreeWidgetItem *list_vals[8],*list_views[8];
	QTreeWidgetItem *list_data,*list_pos,*list_axes,*list_view,*list_val;
	QTreeWidgetItem **datsets;
	QTreeWidgetItem ***daxes;
	QListWidget *historyView;
    color_bar *cbar;
	 QTabWidget *clipMain;
	QListWidgetItem *historyItem[20];
	std::vector<QWidget *>clipWidgets,colorWidgets;
	QWidget *d1,*d2;
	clip_bar *b1,*b2;
    QWidget *colorWidget;
    QTabWidget *clipTab;
  	QComboBox* bclipbox,*eclipbox,*mode;
  	QGroupBox *colorBoxb,*colorBoxe,*modeBox;
  
  	QPushButton* resetButton;
  	QHBoxLayout* layc;
  	std::vector<clip_bar *> bars;
	  //std::vector<color_bar *> bars_c;
	  std::vector<QTabWidget*> tabs;
  	QLabel* contrastLabel;
  	QSlider* contrastSlider;
		QLabel* centerLabel;
  	QSlider* centerSlider;

	
	QTreeWidget* statusView;
	
  position *pos;
  datasets *datas;
  int nviews;
  std::vector<DrawWindow *>windows;
  axis axes[8];
  int nhist,ndat;
  QString pos_lab,val_lab;
  bool symetric;
	
	
};

#endif

