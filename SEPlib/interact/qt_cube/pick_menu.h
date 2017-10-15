#ifndef PICK_MENU_H
#define PICK_MENU_H 1
#include "menu_base.h"

class pick_menu: public menu_base
{
Q_OBJECT
public:
    pick_menu();
    pick_menu(datasets * dats, QString num,int nd);

    void create_menu();
    void create_file_menu();
    void create_size_dist_menu();
    void create_color_menu();
    void create_surface_menu();
  private slots:

	void bulk_on();
	void bulk_off();
	void setRangeSlider(int ido);
	void set_extra_choice(const QString&text);
	void picksIncSizeButtonClicked();
	void picksDecSizeButtonClicked();
    void picksActiveBoxActivated(const QString& text);
        void picksDataBoxActivated(const QString& text);
    void picksSnapBoxActivated(const QString& text);
    void picksViewBoxActivated(const QString& text);
    void pickSurfaceColor(const QString&text);
    void display_changed(const QString&text);

    void updateButtonClicked();
        void depth_button_clicked();
    void amp_button_clicked();
   void overlay_change(const QString& text);

void picksSaveButtonClicked() ;
void picksLoadButtonClicked() ;
 private:
     QHBoxLayout* layFile,*layColor,*laySize,*layDraw,*laySurface;
     QWidget *fileWidget,*colorWidget,*sizeWidget,*drawWidget,*surfaceWidget;

QGroupBox* extra_box;
  	  	QComboBox* extra_select;
	QGroupBox* single_box;
  	QComboBox* single_select;
	QWidget* picksBox;
	QHBoxLayout* picksBoxLayout;
	QVBoxLayout* lineLayout;
	QVBoxLayout* colorLayout;
	QGroupBox* picksVisibleBox,*picksDataBox;
	QRadioButton* picksShowButton;
	QRadioButton* picksHideButton;
	QGroupBox* picksFileBox,*surfcolorgroup,*updateSurfaceBox;
	QPushButton* picksSaveButton;
	QPushButton* picksLoadButton;
	QGroupBox* picksActionBox;
	QPushButton* picksFixButton;
	 QPushButton *updateButton;
	QPushButton* picksIncRangeButton;
	QPushButton* picksDecRangeButton;
	QPushButton* picksIncSizeButton;
	QPushButton* picksDecSizeButton;
	QPushButton* picksMenuPopButton;
	QComboBox* picksActiveBox;
	QComboBox* picksViewBox,*picksdata;
	QGroupBox* picksChooseBox,*picksSnapBox,*picksChooseBox2;
	QComboBox* picksSnap,*surfaceColor;
		
	  QGroupBox* lineptgroup,*depthampgroup;
	QSlider* rangeSlider;
  	QRadioButton*ampbutton,*depthbutton;
};

#endif
