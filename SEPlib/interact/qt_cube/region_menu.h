#ifndef region_menu_H
#define region_menu_H 1
#include "menu_base.h"

class region_menu: public menu_base
{
Q_OBJECT
public:
    region_menu();
    region_menu(datasets * dats, QString num,int nd);

    virtual void createit();
    virtual void layout();
  private slots:

	

    void primaryColorBoxActivated(const QString& text);
    void horizdirBoxActivated(const QString& text);
        void vertdirBoxActivated(const QString& text);

    void secondColorBoxActivated(const QString& text);



void regionSaveButtonClicked() ;
 private:
     QHBoxLayout* layMain,*layColor,*laySize,*layDraw;
     QWidget *mainWidget,*colorWidget,*sizeWidget,*drawWidget;

	QGroupBox* single_box;
  	  	QComboBox* single_select;
	QWidget* picksBox;
	QHBoxLayout* picksBoxLayout;
	QVBoxLayout* lineLayout;
	QVBoxLayout* colorLayout;
	QGroupBox* picksVisibleBox;
	QRadioButton* picksShowButton;
	QRadioButton* picksHideButton;
	QGroupBox* picksFileBox;
	QPushButton* regionSaveButton;
	QPushButton* picksLoadButton;
	QGroupBox* picksActionBox;
	QPushButton* picksFixButton;
	QPushButton* picksIncRangeButton;
	QPushButton* picksDecRangeButton;
	QPushButton* picksIncSizeButton;
	QPushButton* picksDecSizeButton;
	QPushButton* picksMenuPopButton;
	QComboBox* primaryColorBox;
	QComboBox* secondColorBox;
	QGroupBox* picksChooseBox,*horizdirBox,*picksChooseBox2;
	QComboBox* horizdir;
			QComboBox* vertdir;

	  QGroupBox* lineptgroup;
	QSlider* rangeSlider;
  	QRadioButton* linebutton;
  	QRadioButton* ptbutton;
};

#endif
