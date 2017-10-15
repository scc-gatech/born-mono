#ifndef NAVIGATE_MENU_H
#define NAVIGATE_MENU_H 1
#include "menu_base.h"

class navigate_menu: public menu_base
{
Q_OBJECT
  public:

    navigate_menu( datasets * dats, QString num,int nd);
    
    virtual void createit();
    virtual void layout();
  signals:
	  void actionDetected(std::vector<QString> com);
		void movieSpeedChanged(int);

  private slots:

		void movieGoButtonClicked();
	  void movieNoButtonClicked();
	  void stepButtonClicked();
	  void largeZButtonClicked();
	  void smallZButtonClicked();
	  void largeXButtonClicked();
	  void smallXButtonClicked();
	  void largeYButtonClicked();
	  void smallYButtonClicked();
  	void setMovieSpeed(int);
	
  private:
      QHBoxLayout* layMovie,*layDir;
	  QWidget *movieWidget,*dirWidget;
	  QGroupBox* navigateMovieBoundBox;
	  QRadioButton* navigateMovieToEdgeButton;
	  QRadioButton* navigateMovieToClipButton;
	  QPushButton* navigateMovieHighSpeedButton;
	  QGroupBox* navigateHistoryBox;
	  QPushButton* navigateUndoButton;
	  QPushButton* navigateRedoButton;
	  QPushButton* navigateUndoStartButton;
	  QPushButton* navigateRedoEndButton;
	  QPushButton* navigateAnimUndoButton;
	  QPushButton* navigateAnimRedoButton;
	  QGroupBox* navigateHistoryFileBox;
	  QPushButton* navigateHistorySaveButton;
	  QPushButton* navigateHistoryLoadButton;
	  QGroupBox *movieButtonGroup;
	  QPushButton *stepButton;
	  QGroupBox *speedBox;
	  QGroupBox *movieDirButtonGroup;
		QRadioButton* smallZButton;
  	QRadioButton* largeZButton;
  	QRadioButton* smallXButton;
  	QRadioButton* largeXButton;
  	QRadioButton* smallYButton;
  	QRadioButton* largeYButton;
  	QRadioButton* movieGoButton;
  	QRadioButton* movieNoButton;
		QSlider* speedSlider;
		int movieSpeed;
};
	
#endif
