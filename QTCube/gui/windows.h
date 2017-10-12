#ifndef WINDOWS_H
#define WINDOWS_H 1
#include "drawwindow.h"
#include "panels.h"
#include <map>
#include <QObject>
namespace SEP{
class windows:public QObject{
  Q_OBJECT
  public:
    windows(std::shared_ptr<SEP::panels >m){ my_pan=m; active_num=0;}
         signals:
    void actionDetected(std::vector<QString> text);
    public:
    void add_window(std::shared_ptr<DrawWindow>um);
    void delete_window(std::shared_ptr<DrawWindow>wind);
    void update_displays(bool tr=false);
    int size(){return (int)my_winds.size();}
    void set_active_panel(int iw, int ip){ my_winds[iw]->set_active_panel(ip);}
    void windows_respond(std::vector<QString> coms);
        void annotate_respond(std::vector<QString> coms);

    ~windows(){delete_windows();}
    int nwind(){ return my_winds.size();}
    std::shared_ptr<DrawWindow> return_window(int i){return my_winds[i];}
        void connect_all();
    void connect_it(int i);
    void disconnect_it(int i);
    void update_grid(std::shared_ptr<DrawWindow>active,int ioldx,int ioldy, int inewx, int inewy);
       void set_active(int ia){ 
       active_num=ia;}
    int get_active_num(){ return active_num;}
   std::vector<std::shared_ptr<DrawWindow>> my_winds;

  private:
   void delete_windows();
   std::shared_ptr<SEP::panels> my_pan;
   int active_num;
     private slots:
    void actionRespond(std::vector< QString> text);
    void update_all();

};
}
   
    
  
#endif
