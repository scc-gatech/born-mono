#ifndef VIEW_MENU_H
#define VIEW_MENU_H 1
#include "menu_base.h"

class view_menu: public menu_base
{
 Q_OBJECT
  public:
    view_menu();
    view_menu( datasets * dats, QString num,int nd);
    virtual void createit();
    void view_createit();
    void color_createit();
    virtual void layout();
    
signals:
    void alphaChanged(int alpha);

  private slots:
    void viewRoundONextButtonClicked();
    void viewRoundNextButtonClicked();
    void jpegButtonClicked();
    void psBButtonClicked();
    void pdfButtonClicked();
        void psSButtonClicked();
    void viewRoundPrevButtonClicked();
        void viewRoundOPrevButtonClicked();
	void setTranspSlider(int);

 	void viewFacesComboBoxActivated(const QString& text);
 	 	void viewFontComboBoxActivated(const QString& text);
 	void viewDataComboBoxActivated(const QString& text);
 	 	void viewDataOComboBoxActivated(const QString& text);
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

    void barrNoButtonClicked();
    void barrYesButtonClicked();
    void bargNoButtonClicked();
    void bargYesButtonClicked();
    void barcNoButtonClicked();
    void barcYesButtonClicked();
    void barpNoButtonClicked();
    void barpYesButtonClicked();
    void barkNoButtonClicked();
    void barkYesButtonClicked();
    void bartNoButtonClicked();
    void bartYesButtonClicked();
    void actionRespond(std::vector<QString>);	

     
  private:
      void multi_dat_create();

  	QWidget* viewBox,*mainWidget,*overlayWidget,*h_dataWidget,*saveWidget,*showWidget,*show2Widget;
  	    QHBoxLayout* layMain,*layOverlay,*layData,*laySave,*layShow,*layShow2;
  	    	  QGroupBox *barcButtonGroup,*barkButtonGroup,*barrButtonGroup;
  	    	  QGroupBox *bargButtonGroup,*barpButtonGroup,*bartButtonGroup,*viewRoundBox,*viewRoundOBox;
  	QRadioButton* bartyesButton;
  	QRadioButton* bartnoButton;
  	QRadioButton* barcyesButton;
  	QRadioButton* barcnoButton;  	
  	QRadioButton* barpyesButton;
  	QRadioButton* barpnoButton;  	
  	QRadioButton* barkyesButton;
  	QRadioButton* barknoButton;
  	QRadioButton* bargyesButton;
  	QRadioButton* bargnoButton;
  	QRadioButton* barryesButton;
  	QRadioButton* barrnoButton;
  	QGroupBox* viewFacesBox;
  	QComboBox* viewFacesComboBox;
  	 	QGroupBox* viewFontBox;
  	QComboBox* viewFontComboBox;
		QGroupBox* viewDataBox;
  	QComboBox* viewDataComboBox;
		QGroupBox* viewDataOBox;
  	QComboBox* viewDataOComboBox;
  	QGroupBox* viewSaveBox;
  	QGroupBox *viewShowBox;
  	QRadioButton* viewRoundOnButton;
  	QRadioButton* viewRoundOffButton;
  	QPushButton* viewRoundNextButton;
  	QPushButton* viewRoundPrevButton;
  	  	QPushButton* viewRoundNextOButton;
  	QPushButton* viewRoundPrevOButton;
  	QPushButton* jpegSave;
  	QPushButton* psBSave;
  	QPushButton* pdfSave;
  	QPushButton* psSSave;
  			QSlider* transpSlider;
QHBoxLayout *lay_con,*lay_wig,*lay_col;
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



  	QRadioButton* viewMultiOnButton;
  	QRadioButton* viewMultiOffButton;
};
#endif
