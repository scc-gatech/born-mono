#ifndef COLOR_MENU_H
#define COLOR_MENU_H
#include "menu_base.h"
//Added by qt3to4:
#include <QLabel>
#include <QLineEdit>

class color_menu: public menu_base
{
Q_OBJECT
  public:
    color_menu();
    
    color_menu( datasets *dats, QString num,int nd);
    virtual void createit();
    virtual void layout();

  private slots:
  
  
    void colorCubeComboBoxActivated(const QString& text);
    void colorOverCComboBoxActivated(const QString& text);
    void colorBgComboBoxActivated(const QString& text);
    void colorWigComboBoxActivated(const QString& text);
    void posWigComboBoxActivated(const QString& text);
    void negWigComboBoxActivated(const QString& text);
	void colorContComboBoxActivated(const QString& text);
	void numberContComboBoxActivated(const QString& text);
	void c0ContComboBoxActivated(const QString& text);
	void dcContComboBoxActivated(const QString& text);
	 void setAlpha(int transp);
    void colorOverComboBoxActivated(const QString& text);

  private:
	QHBoxLayout *lay_con,*lay_wig;
     QWidget *colorWidget;
	 QWidget *wiggleWidget;
	 QWidget *contourWidget;
	QGroupBox* conNumberBox;
	QComboBox *numberContComboBox;
	  QGroupBox* colorBox2;
  	QGroupBox* colorCubeBox;
	  QComboBox* colorCubeComboBox,*colorCCubeComboBox;
  	QGroupBox* colorOverBox;
  	QComboBox* colorOverComboBox;
  	QGroupBox* colorBgBox;
  	  	QComboBox* colorBgComboBox;
  	  	  	QGroupBox* colorWigBox;
  	  	  	  	  	  	QGroupBox* posWigBox;
  	  	  	  	  	  	  	  	  	QGroupBox* negWigBox;
  	  	QComboBox* colorWigComboBox;
  	  	  	  	QComboBox* posWigComboBox;
  	  	  	  	  	  	QComboBox* negWigComboBox;
		QGroupBox *colorContourBox;
  	QGroupBox* colorMarkBox;
  	QComboBox* colorMarkComboBox;
  	QLineEdit* c0Box,*dcBox;
  	QGroupBox* dcNumberBox;
  	  	QGroupBox* c0NumberBox;
  	  	QGroupBox* colorOverCBox;
	  QComboBox* colorOverCComboBox;
	  QSlider* speedSlider;
	  QGroupBox *speedBox;

  	//QPushButton* resetButton;
	QComboBox* colorContComboBox;
};

#endif
