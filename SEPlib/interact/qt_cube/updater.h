#include "change.h"
#include "drawwindow.h"
#include <vector>
#include <string>
#include <qtimer.h>
//Added by qt3to4:
#include <QCloseEvent>
#include "autopick.h"
#include "datasets.h"
#include "pick_draw.h"
#include "slice_types.h"
 #include <qobject.h>
#ifndef UPDATER_H
#define UPDATER_H 1
class updater: public QObject{
	Q_OBJECT

signals:
	void actionDetected(std::vector< QString> text);


  public:
    updater(QObject *parent,int nv,  std::vector<DrawWindow*>ws,datasets *d,
      param_func *p, pick_draw *p_k, slice_types *st,
      const char* name = 0);

    void actionRespond(std::vector<QString> command);
    int get_active(){return iactive;}
     void load_history(QString file);
	 void load_dataset (QString file);
         std::vector<change> hist;
  private slots:
     void update_movie();
         void run_movie();
    void stop_movie();

  private:
    QString color_respond(std::vector<QString> command);
	QString contour_respond(std::vector<QString> command);
    QString main_respond(std::vector<QString> command);
    QString orient_respond(std::vector<QString> command);
    QString navigate_respond(std::vector<QString> command);
    QString view_respond(std::vector<QString> command);
    QString pick_respond(std::vector<QString> command);
    QString history_respond(std::vector<QString> command);
    QString clip_respond(std::vector<QString> command);
    QString auto_respond(std::vector<QString> command);
    QString ctable_respond(std::vector<QString> command);
    #ifdef CHEVRON
    QString chev_respond(std::vector<QString> command);
    #endif
    void write_history(QString file);
   

    void sync_orient();



protected:
	void closeEvent(QCloseEvent* event);
	void load_dataset(std::vector<QString> coms);
  
  private:
    	QTimer *movie_timer;

    std::vector<DrawWindow *>windows;
    int iactive,locked,nviews;
    DrawWindow *active;
    pick_draw *pk;
    int ibeg,iend,movie_delay;
    std::vector<QString> com;
    bool write;
    autopick *autop;
    datasets *datas;
    param_func *pars;
    slice_types *stype;
    QString snap;

};
    
#endif
