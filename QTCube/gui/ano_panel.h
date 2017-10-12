#ifndef ano_panel_H
#define ano_panel_H 1
#include "qcubewidgetbasics.h"
#include "widget_tools.h"
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QObject>

namespace SEP{
class ano_panel: public qcubeWidgetBasic{
  Q_OBJECT
  public:
    ano_panel(std::shared_ptr<SEP::position> p,std::shared_ptr<SEP::windows> my_w, std::shared_ptr<SEP::panels> my_p, 
     std::shared_ptr<SEP::datasets> my_d, std::shared_ptr<SEP::pick_draw> pk, std::shared_ptr<SEP::slice_types> c,std::shared_ptr<SEP::maps> mym);

    ~ano_panel(){delete_ano_panel();}
         void update_menu(std::vector<QString>);
  signals:
    void actionDetected(std::vector<QString> coms);
  private slots:
    void sbox_change();
    void stext_change();
    void sarrow_change();
    void sline_change();
    void selipse_change();
    void color_change(const QString txt);
    void thick_change(const QString txt);
    void text_change(const QString txt);
        void font_change(const QString txt);

  private:
    void delete_ano_panel();
    basicRadioBox *shape;
    std::shared_ptr<QHBoxLayout> layBot,layTop;
    std::shared_ptr<QVBoxLayout> layMain;
    std::shared_ptr<QGroupBox> gbox,gbox2;
    std::shared_ptr<basicComboBox> thick,colorB,font;
    basicLineEditBox *txt;    
};
}
#endif
