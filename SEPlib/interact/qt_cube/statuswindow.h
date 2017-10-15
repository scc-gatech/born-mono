
#ifndef STATUSWINDOW_H
#define STATUSWINDOW_H
#include <qwidget.h>
#include "clip_bar.h"

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
#define PROPORTIONAL 2
#define BASE_PARALLEL 51
#define TOP_PARALLEL  102
#define PROGRADATIONAL 153
#define VFLAT 204
#define BEST_GUESS 253
#define MINIMUM_HEIGHT_MENU 30

class StatusWindow : public QWidget
{
    Q_OBJECT

public:
    StatusWindow( position *pos, datasets *ds, int nviews, std::vector<DrawWindow *>winds,QWidget *parent = 0, const char *name = 0 );
        void setValue(int, int, int , int , int);
     void update_status(std::vector<change> changes);
     void create_history();
     void create_info();
     void delete_datas();
     void create_datas();
     QString return_pos_lab(){return pos_lab;}
     QString return_val_lab(){return val_lab;}

    
 signals:
    void actionDetected(std::vector <QString>);
 private slots:
    void history_sel(QListWidgetItem * item);
#ifdef CHEVRON
    void colorBoxActivated(const QString& text);
    void layerBoxActivated(const QString& text);
#endif

 private:
	QTreeWidgetItem *list_poss[8];
	QTreeWidgetItem *list_vals[8],*list_views[8];
	QTreeWidgetItem *list_data,*list_pos,*list_axes,*list_view,*list_val;
	QTreeWidgetItem **datsets;
	QTreeWidgetItem ***data_info;
	QTreeWidgetItem ***headers;
	QTreeWidgetItem ***daxes;
	QListWidget *historyView;
	QComboBox *layerbox,*colorbox;
	QGroupBox *layergroup,*colorgroup;
	QVBoxLayout *layoutcolor;
	QVBoxLayout *layoutlayer;

	 QTabWidget *clipMain;
	QListWidgetItem *historyItem[20];
	std::vector<QWidget *>clipWidget;
	QWidget *d1,*d2;
	clip_bar *b1,*b2;
    QWidget *colorWidget;

  	QComboBox* bclipbox,*eclipbox;
  	QGroupBox *colorBoxb,*colorBoxe;
  
  	QPushButton* resetButton;
  	QHBoxLayout* layc;
  	std::vector<clip_bar *> bars;
	
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
  std::map<QString,int> convert_s_n;
  std::map<QString,QString> convert_s_s;
  std::map<int,int> convert_n_n;
	
	
};

#endif

