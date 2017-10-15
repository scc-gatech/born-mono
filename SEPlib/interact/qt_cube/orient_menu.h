#ifndef ORIENT_MENU_H
#define ORIENT_MENU_H 1
#include "menu_base.h"

class orient_menu: public menu_base
{
Q_OBJECT
  public:
    orient_menu(datasets * dats, QString num,int nd);
    void create_menu();
    void create_main_tab();
    void create_tranpose_tab();
    void create_rotate_2d_tab();
    void create_rotate_3d_tab();
    void create_map_tab();
  private slots:

	void rotateAxis1_1(const QString& text);
		void rotateAxis1_2(const QString& text);

	void rotateAxis2(const QString& text);
  void setRotateSlider(const int& val);
    void setRotateAng(const QString& val);

	void orientSwapComboBoxActivated(const QString& text);
  void rotateAcceptButtonClicked();
  void setptButtonClicked();
	void orientFlipDownButtonClicked();
	void orientFlipAcrossButtonClicked();
	void orientFlipDeepButtonClicked();
	void orientMoveMiddleButtonClicked();
	void orientMoveCornerButtonClicked();
	void orientResetButtonClicked();
	void trueButtonClicked();
	void set_transp(int *ns);
	void rotateResetButtonClicked();
	void actionRespond(std::vector<QString>);	
	
  private:
	int ndim_transp;
	QWidget* orientBox;
	QWidget*mainWidget,*swapWidget,*rot2dWidget,*rot3dWidget,*mapWidget;
	QHBoxLayout*layMain,*laySwap,*layRot2d,*layRot3D,*layMap;
	QGroupBox* orientswapBox,*RotateBox;
	QComboBox* orientSwapComboBox,*axis2dBox1,*axis2dBox2;
	QGroupBox* orientFlipBox;
	QPushButton* orientFlipDownButton;
	QPushButton* orientFlipAcrossButton,*rotate2dButton,*setpt2dButton;
	QPushButton* trueButton,*resetButton;
	QPushButton* orientFlipDeepButton;
	QGroupBox* orientFrameBox;
	QPushButton* orientMoveMiddleButton;
	QPushButton* orientMoveCornerButton;
	QPushButton* orientResetButton;
	QPushButton* orientMenuPopButton,*reset2dButton;
	        QSlider*rotate2dSlider;
  	QLineEdit *angBox;

};

#endif
