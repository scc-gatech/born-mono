#ifndef data_panel_H
#define data_panel_H 1
#include "qcubetabbasics.h"
#include "widget_tools.h"
#include "clip_bar.h"
#include <vector>
namespace SEP{
#ifdef CHEVRON
class flatten_types{ 
  public:
  flatten_types();
  int button_from_value(int val){return b_from_v[val];}
  int value_from_button(int val){return v_from_b[val];}
  QString type_from_value(int val){return s_from_v[val];}
  std::vector<QString> types;
private:
   std::map<int,int> b_from_v,v_from_b;
   std::map<int,QString> s_from_v;

};
#endif
class data_panel: public qcubeTabBasic{
  Q_OBJECT
  public: 
    data_panel(std::shared_ptr<SEP::position> p,std::shared_ptr<SEP::windows> my_w, std::shared_ptr<SEP::panels>my_p, 
    std::shared_ptr<SEP::datasets>my_d, std::shared_ptr<pick_draw>pk, std::shared_ptr<SEP::slice_types> c,
    std::shared_ptr<SEP::maps> mym);
    signals:
    void actionDetected(std::vector<QString> text);
    public:
    void update_menu(std::vector<QString>);

   private slots:
    void actionRespond(std::vector< QString> text);
 
};

class add_dataset: public qcubeTabBasic{
  Q_OBJECT
  public: 
    add_dataset(std::shared_ptr<position> p,
    std::shared_ptr<SEP::windows> my_w, std::shared_ptr<SEP::panels> my_p, std::shared_ptr<SEP::datasets>my_d,
     std::shared_ptr<SEP::pick_draw> pk, std::shared_ptr<SEP::slice_types>c,std::shared_ptr<SEP::maps> mym);


 signals:
    void actionDetected(std::vector<QString> text);
    
  public:
    ~add_dataset(){ delete_add_dataset();}

  private:
    void delete_add_dataset();
    void create_sep_tab();
        void create_seispak_tab();
        void delete_seispak_tab();
        void create_nmo_tab();
        void delete_nmo_tab();
        void create_semblance_tab();
        void delete_semblance_tab();
        void create_flattening_tab();
        void delete_flattening_tab();
        void create_surface_tab();
        void delete_surface_tab();

    void delete_sep_tab();
    QWidget *sepTab,*seispakTab,*nmoTab,*semblanceTab,*flattenTab,*surfaceTab;
    QHBoxLayout *sepLay,*seispakLay,*nmoLay,*semblanceLay,*flattenLay,*surfaceLay;
    basicRadioBox *storeSEP,*storeSEIS,*esizeSEIS,*typeSURF;
    basicButtonBox *loadSEP,*loadSEIS,*createSurface,*createFlatten,*surfUpdate;;
    basicComboBox *colorFlatten,*dataSurface,*colorSurface,*dataOffset,*dataMeas;   
    basicComboBox *timeAxis,*offsetAxis,*measAxis;

    
      private slots:
     void create_sep();
     void create_seispak();
     void create_surface();       
     void create_flatten();
 
};

class data_sub_panel: public qcubeTabBasic{
  Q_OBJECT
  public: 
    data_sub_panel(std::shared_ptr<position> p,std::shared_ptr<windows>my_w, 
    std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk,
     std::shared_ptr<slice_types> c,std::shared_ptr<maps> mym,int idat);
  
     void update_menu(std::vector<QString>);
  signals:
    void actionDetected(std::vector<QString> text);
  private:
  float  *second_deriv(int npts, float *x, float *y);
    void delete_clip_window();
    QWidget *create_clip_window();
    QWidget *create_info_window();
    void delete_info_window();
    void add_seispak_info();
    void add_flatten_info();
    void add_surface_info();
    
    

    

    int idat;
    clip_bar *clip;
    basicComboBox *begin,*end;
    basicRadioBox *symetric,*surfType;
    basicButtonBox *clear,*surfUpdate,*stratSave;
    basicSliderBox  *contrast;
    std::vector<basicComboBox *>seispak;
    QGroupBox *boxB;
    QGroupBox *extraB;
    QGroupBox *extraInfoBox;
    QVBoxLayout *extraL;
    QHBoxLayout *extraH;
    QHBoxLayout *changeBox;
    QVBoxLayout *clipBox;
    QWidget *clipWidget,*infoWidget;
    QHBoxLayout *infoLay;
    QTreeWidget *statusView;
    QTreeWidgetItem **dataInfo;
    	QTreeWidgetItem **headers;
	QTreeWidgetItem **daxes;
   QTreeWidgetItem *val;
       void set_strat(int i);

      private slots:
    void actionRespond(std::vector< QString> text);
    void change_beg(const QString txt);
    void change_end(const QString txt);
    void change_contrast(int contrast);
    void symetric_yes();
    void symetric_no();
    void clip_clear();
    void set_strat_prop(){set_strat(0);}
    void set_strat_base(){set_strat(1);}
    void set_strat_top(){set_strat(2);}
    void set_strat_pres(){set_strat(3);}
    void set_strat_prog(){set_strat(4);}
    void set_strat_vflat(){set_strat(5);}
    void set_strat_best(){set_strat(6);}
    void change_axis2(const QString txt);
    void change_axis3(const QString txt);
    void change_axis4(const QString txt);
    void change_axis5(const QString txt);
    void change_axis6(const QString txt);
    void change_axis7(const QString txt);
    void change_axis8(const QString txt);
     void update_surface();
     void save_strat();

 
};

}
#endif
