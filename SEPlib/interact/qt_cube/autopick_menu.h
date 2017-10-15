#ifndef AUTOPICK_MENU_H
#define AUTOPICK_MENU_H 1
#include "menu_base.h"
class autopick_menu: public menu_base
{
Q_OBJECT
public:
    autopick_menu();
    autopick_menu(datasets  *dats, QString num,int nd);

    virtual void createit();
    virtual void layout();
  private slots:

	  void extendButtonClicked();

	  void pickButtonClicked();

	  void acceptButtonClicked();
	  void declineButtonClicked();
	  
  void methodActivated(const QString& text);
   void planeActivated(const QString& text);
    void pickTypeBoxActivated(const QString& text);
    void picksCorBoxActivated(const QString& text);
        void picksMaxDistActivated(const QString& text);
        void picksErrorActivated(const QString& text);


 private:
    QWidget *mainWidget;
      QWidget *lloydWidget,*flatWidget,*Widget2d,*Widget3d;
	QGroupBox* single_box;
  	  	QComboBox* single_select;
	QWidget* picksBox;
	QGroupBox *acceptBoxB,*methodTypeBox2d, *extendTypeBox2d,*planeTypeBox2d;
	QPushButton *declineButtonB,*acceptButtonB,*extendButtonB;
	QComboBox *methodBox2d,*extendBox2d,*planeBox2d;
	QGroupBox *pick2dBox;
		QGroupBox* picksFileBox,*picksTypeTBox,*picksCorTBox,*picksDistTBox;
		QGroupBox *picksErrorBox,*flatBox,*picksFileBoxB,*methodTypeBox3d;
		QHBoxLayout* layL,*layF,*layB,*lay2d;
	QHBoxLayout* picksBoxLayout,*picksBoxLayoutB;
    QPushButton* pickButton,*flatButton,*reflatButton;
	QPushButton* repickButton,*repickButtonB,*pickButtonB;
		QComboBox* pickTypeBox;
			QComboBox* picksCorBox;
				QComboBox* picksMaxDist;
					QComboBox* picksErrorPct;
	
	
};

#endif
