#ifndef QT_CUBE_AUTOPICK_H
#define QT_CUBE_AUTOPICK_H
#include<vector>
#include "float_2d.h"
#include "float_1d.h"
#include "view.h"
#include "autopick_2d.h"
#include "pick_draw.h"
class autopick{

  public:
    autopick(pick_draw *_pks);
    
    int pick_2d(view *myv,orient_cube *pos);
    int extend_picks(view *myv, orient_cube *pos);
    int flat_view(view *myv,orient_cube *pos);

    int get_ipick(){return ipick;}
    void set_correlate(){ correlate=true;}
    void set_amplitude(){ correlate=false;}
     float_2d *extract_dat (orient_cube *pos,dataset *dat,int isingle, int isort,
     float_1d *line, int ns,int f);
    void pick_line(view *myv,float *fpos,orient_cube *pos,int iax1,int iax2, int isort,
      int isingle, pairs_new *myp,int iax3);
    void set_nsearch(int ns){nsearch=ns;}
    void set_ncor(int nc){ncor=nc;}
    bool get_correlate(){return correlate;}
    int get_ncor(){return ncor;}
    int get_nsearch(){return nsearch;}
    float_2d *correlate_it(float_2d *ar, int ncor,std::vector <int> close);
    int decrease_auto(){ cur_auto--; return 0;}
    int get_auto(){return cur_auto;}
    float get_error(){return error;}
    void set_error(float er){ error=er;}
    void create_viteribi();
    void create_brown();
    QString get_method(){ return method;}
    void set_method(QString meth){ method=meth;}
    void set_plane(QString pl){ plane=pl;}
    QString get_plane(){return plane;}
    int get_accept(){return accept;}
    void accept_picks(){ accept=cur_auto;}
    void set_accept(int ac){ accept=ac;}
    void decline_picks();
    void init_method();
  private:
    pick_draw *pks;
    int ipick;
    int ncor;
    bool correlate;
    int nsearch;
   float nail,pen_o,pen_e;
   float error;
   int cur_auto;
   int accept;
   float *dip; //temporary
   autopick_2d *auto_2d;
   QString method;
   QString plane;
  
};
  
#endif
