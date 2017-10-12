#ifndef PANELS_H
#define PANELS_H 1
#include "panel.h"
#include <map>
#include "mouse_func.h"
#include "util.h"
#include "pick_draw.h"
#include "autopick.h"
#include "maps.h"
#include "orient_cube.h"
#include <QObject>
namespace SEP{
class panels: public QObject{
 Q_OBJECT

 public:
    panels(std::shared_ptr<SEP::genericIO> io,std::shared_ptr<pick_draw> p,std::shared_ptr<maps>my_maps,std::shared_ptr<orients> my_o);
     signals:
    void actionDetected(std::vector<QString> text);
    void secondaryDetected(std::vector<QString> text);
    void windows_update();
  public:
    void add_panel(std::shared_ptr<panel> um);
    void delete_panel(int ipan){ 
      auto it=my_pan.find(ipan); 
       assert(my_pan.count(ipan)==1);      
       disconnect_it(ipan);
      
      my_pan.erase(it);
    }
    void set_active(int iact);
    void sync_panel(std::shared_ptr<panel> x);
    std::shared_ptr<panel> get_panel(int ipan){ 
       return my_pan[ipan];}
    void sync_panel(int ipan){sync_panel(my_pan[ipan]);}
    int get_next_panel_num(){pan_num++; return pan_num-1;}
    void perform_pick(std::vector<QString> coms);
    void perform_display(std::vector<QString> coms);
    void perform_auto(std::vector<QString> coms);

    void perform_orient(std::vector<QString> coms);
        void perform_view(std::vector<QString> coms);
    void perform_navigate(std::vector<QString> coms);
    std::shared_ptr<position> get_position(){ return my_pan[0]->get_orient();}
    ~panels(){delete_panels();}
    void update_mouse(std::shared_ptr<mouse_func> f);
    void connect_all();
    void connect_it(int i);
    void disconnect_it(int i);
    void set_range(int num, int *ibeg, int *iend);
    void perform_navigation(std::vector<QString> coms);
    int size(){ return my_pan.size();}
    void update_all();
    void set_locked(bool x){ locked=x;}
    void update_it(std::shared_ptr<panel >active){ if(locked) update_all(); else update_only(active);}
    void update_it(int iact){ update_it(my_pan[iact]);}
    void update_only(std::shared_ptr<panel > active){ active->set_update_it(true);}
    void update_only(int iact){ update_only(my_pan[iact]);}
    void update_what(int ival, QString what);
    void run_movie();
    void stop_movie();
   void delete_panels();
    void set_window(int ipan,int iwind){ panel_to_window[ipan]=iwind;
    }
    int get_window(int ipan){ return panel_to_window[ipan];}
  private slots:
    void actionRespond(std::vector< QString> text);
    void update_movie();
  
    private:
    std::shared_ptr<maps> my_maps;
    QTimer *movie_timer;
    std::shared_ptr<autopick> autop;
    std::shared_ptr<SEP::genericIO> _io;
    std::map<int,int> panel_to_window;
  int movie_delay;
    int active_num;
    bool locked;
    std::shared_ptr<pick_draw>pk;
    std::shared_ptr<orients>my_or;
   std::map<int,std::shared_ptr<panel>> my_pan;
   int pan_num;
};
}
#endif
