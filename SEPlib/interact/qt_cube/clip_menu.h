#ifndef CLIP_MENU_H
#define CLIP_MENU_H
#include "menu_base.h"
#include "clip_bar.h"

#include <QMouseEvent>
#include <QLabel>
class clip_menu: public menu_base
{
Q_OBJECT
  public:
    clip_menu();
    
    clip_menu( datasets *dats, QString num,int nd);
    virtual void createit();
    virtual void layout();
    void mouseReleaseEvent(QMouseEvent *);
    virtual void actionRespond(std::vector<QString> com);
  private slots:
  void bactivated(const QString& text);
    void eactivated(const QString& text);
  
   
    void send_it(std::vector <QString> com);

  private:
    QWidget *clipWidget;
    QWidget *colorWidget;

  	QComboBox* bclipbox,*eclipbox;
  	QGroupBox *colorBoxb,*colorBoxe;
  
  	QPushButton* resetButton;
  	QHBoxLayout* layc;
  	clip_bar *bar;
	
  	QLabel* contrastLabel;
  	QSlider* contrastSlider;
		QLabel* centerLabel;
  	QSlider* centerSlider;
};

#endif
