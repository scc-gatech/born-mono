#ifndef MAIN_MENU_H
#define MAIN_MENU_H 1
#include "menu_base.h"
class main_menu: public menu_base
{
Q_OBJECT
  public:

   main_menu( datasets *dats, QString num,int nd);
    
    virtual void createit();
    virtual void layout();
  signals:
	  void actionDetected(std::vector <QString> text);


  private slots:
    void mainQuitButtonClicked();
    void mainNoButtonClicked();
    void mainYesButtonClicked();
        void attachNoButtonClicked();
    void attachYesButtonClicked();
    void historySaveButtonClicked();
	void historyLoadButtonClicked();
	void loadButtonClicked();
	void enableDataFileLoadButton();
	void mouseBoxActivated(QString com);
		void statusBoxActivated(QString com);

//	void storageComboBoxActivated(const QString&);
	

	
  private:
	QWidget *mainWidget,*loadWidget,*infoWidget;
	QPushButton *mainQuitButton;
	QPushButton *historySaveButton;
	QPushButton *historyLoadButton;
	QPushButton *loadButton;
	QGroupBox *mainButtonGroup;
	QGroupBox *mainMouseGroup,*mainStatusGroup;
		QGroupBox *attachButtonGroup;

	QGroupBox *actionButtonGroup;
	QGroupBox *loadButtonGroup;
	QGroupBox *fileGroup;
	QGroupBox *storageGroup;
	QComboBox *dataTypeComboBox,*mouseBox,*statusBox;
	QComboBox *storageTypeComboBox;
  	QRadioButton* mainyesButton;
  	QRadioButton* mainnoButton;
  	QRadioButton *attachnoButton, *attachyesButton;
  	QGroupBox *mainBox;
    QHBoxLayout *lay2,*layi;
};
	
#endif
