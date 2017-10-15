#include <qlayout.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qsplitter.h>
#include <qsettings.h>
#include <qstatusbar.h>

#include <QCloseEvent>
#include <QKeyEvent>


#include "centerwindow.h"
#include "drawwindow.h"
#include "menu_bar.h"
#include "param_func.h"
#include "slice_types.h"
#include<iostream>
#include <qregexp.h>
#include "util.h"
#include "mouse_orient.h"
#include "mouse_zoom.h"
#include "mouse_pick.h"


CenterWindow::CenterWindow(param_func *par,datasets *d, pick_draw *_pks,  slice_types *col,QWidget* parent): QMainWindow(parent) {
	
    viewnumber = 1; //TEMP
    datas=d;
    pk=_pks;
    stat_view="mouse";
    pars=par;
	this->colort=col;
  QFont font("Arial", 12, QFont::Normal);
	QApplication::setFont(font);

  createComponents(pars);

  set_active(0);
  itour=0;
        pk->set_server(serv);
  pk->set_position(pos);

  up=new updater(this,nviews,windows,datas,pars,pk,col);
 // fprintf(stderr,"BEFORE CREATE PLANE \n");
//  pk->create_planes();
  //fprintf(stderr,"AFTER CREATE PLANE \n");
connect(statwindow, SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
 connect(up,         SIGNAL(actionDetected(std::vector<QString>)), 
    this, SLOT(actionRespond(std::vector<QString>)));
connect(clipwindow, SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

 for(int i=0; i < (int)windows.size();i++){

    /*
    connect(menuBars[i], SIGNAL(actionDetected(const QString&)), 
    this, SLOT(actionRespond(const QString&)));
    */
    connect(menuBars[i], SIGNAL(actionDetected(std::vector<QString>)), 
    this, SLOT(actionRespond(std::vector<QString>)));
    windows[i]->set_windows(nviews,windows);
    /*
        connect(windows[i], SIGNAL(actionDetected(const QString&)), 
    this, SLOT(actionRespond(const QString&)));
    */
     connect(windows[i], SIGNAL(actionDetected(std::vector<QString>)), 
    this, SLOT(actionRespond(std::vector<QString>)));
  }
  for(int i=0; i <datas->return_ndat(); i++){
       connect(datas->return_dat(i), SIGNAL(actionDetected(std::vector<QString>)), 
    this, SLOT(actionRespond(std::vector<QString>)));
  
  }

    movie_delay=0;
	//this->setWindowTitle(tr("CubeT"));
	setCentralWidget(mainBox);

  curw=width(); curh=height();
  QString tname=pars->get_string("run_tour","NONE").c_str();
  if(tname!="NONE") this->load_tour(tname);
  independent=false;

}
void CenterWindow::actionRespond(std::vector <QString> com){

  bool doit=true;
  if(com[1].contains("none")>0) return;
  if(com[2].contains("quit")){
    emit close();
    exit(0);
  }

//for(int i=0;i < (int)com.size();i++) fprintf(stderr,"%s ",com[i].ascii());
//fprintf(stderr,"\n");
  if(com[1].contains("menu")){
    doit=false;
    if(com[2].contains("Clip")){
      int id=com[0].toInt();
      for(int i=0; i < (int)windows.size(); i++){
        if(windows[i]->get_idat()==id){
    //       menuBars[i]->actionRespond(com);
        }
      } 
      clipwindow->actionRespond(com);
    }
    else if(com[2].contains("Orient")){
      int id=com[0].toInt();
      for(int i=0; i < (int)windows.size(); i++){
        if(windows[i]->get_idat()==id){  
           menuBars[i]->actionRespond(com);
        }
      }
    }
   else if(com[2].contains("main")>0 && com[3].contains("mouse") >0){
      change_mouse(com[4]);
      doit=true;
    }   
    else if(com[2].contains("main")>0 && com[3].contains("status") >0){
      stat_view=com[4];
      doit=true;
    }
    else{
      int iv=com[0].toInt(); 
      if(iv>0) { //Only one
        menuBars[iv]->actionRespond(com);
      }
      for(int i=0; i < (int)menuBars.size(); i++){
        menuBars[i]->actionRespond(com);
      }
    }
  }
  else {
    if(com[2].contains("resize")>0){
       int *pp=util::int_array_from_string(com[3]);
       //fprintf(stderr,"IN RESIZE %d %d %d %d \n",pp[0],pp[1],pp[2],pp[3]);

         this->resize(pp[3],pp[4]);

       if(pp[0]==1) doit=false;
       delete [] pp;
    }
    if(com[2].contains("repaint")>0){
       this->repaint();
    }
    if(com[2].contains("attach")>0){
      if(com[3].contains("true")>0){
       if(independent) WindowHandler(false);
       else doit=false;
      }
      else if(com[3].contains("false")>0){
          if(!independent) WindowHandler(true);
          else doit=false;
        
      }
    }
   }
    if(doit){
      up->actionRespond(com);
      statwindow->update_status(up->hist);
      update_status_bar();
      
    }
  }
void CenterWindow::update_status_bar(){
//  fprintf(stderr,"XXX %s --%s \n",stat_view.ascii(),mouse_lab.ascii());
  if(stat_view.contains("mouse") >0) {
    modeLabel->setText(mouse_lab);
  }
  else if(stat_view.contains("position") >0) {
      modeLabel->setText(statwindow->return_pos_lab());

  }  
    else if(stat_view.contains("pick") >0) {
      QString color="Active:"+pk->get_active_name();
      QString drawing="Drawing:"+pk->get_draw_what();
      QString dis;
      int what=pk->get_display();
      if(what==DISPLAY_EXTRA) dis="Display: Extra";
      else if(what==DISPLAY_POINTS) dis="Display: Points";
      else if(what==DISPLAY_LINE) dis="Display: Line";
      QString ex="Extra:"+QString::number(pk->get_extra());
      QString dist="Distance:"+QString::number(pk->get_dist());
      QString move="Move:Single";
    //  if(pk->get_move()==1) move="Move:Multi";
      
      modeLabel->setText(color+" "+drawing+" "+ex+" "+dist+" "+dis+" "+move);

  } 
  else if(stat_view.contains("value") >0) {
      modeLabel->setText(statwindow->return_val_lab());
  }
}
void CenterWindow::set_active(int iact){
 active=windows[iact];
}
void CenterWindow::closeEvent(QCloseEvent* event) {
	//writeSettings();
	if(event==0){}
	clipwindow->close();

	statwindow->close();
//	event->accept();
}
void CenterWindow::display_it(){
  for(int i=0; i < (int)windows.size(); i++)  {
    windows[i]->initial_view();
  }
}
void CenterWindow::createComponents(param_func *pars) {
	// layout managers
	mainBox = new QSplitter(Qt::Horizontal, this);
	leftBox = new QSplitter(Qt::Vertical, mainBox);
	//rightBox = new QSplitter(Qt::Vertical, mainBox, tr("rightBox - CenterWindow"));

	    int beg[8],end[8],init[8];
    for(int i=0; i< 8; i++){
       beg[i]=0; end[i]=datas->return_dat(0)->get_grid_axis(i+1).n; init[i]=end[i]/2;
    }

    pos=new position((hypercube*)datas->return_dat(0)->return_grid(),init,beg,end,2);

 	serv=new orientation_server(pos);


   
   nviews=pars->get_int("nviews",1);

  middleBox = new QSplitter( Qt::Horizontal,leftBox);
			// top menu area
  
  std::vector<QString> data_names;
  for(int i=0; i < (int)datas->return_ndat(); i++) data_names.push_back(datas->return_dat(i)->get_name());
  mym=new mode("navigate");

//Makes all views
	MakeWindows();
    mouse_lab=funcs["zoom"]->status;
	  modeLabel = new QLabel(mouse_lab, this);
        //statusLabel = new QLabel(tr("status: fine"), this);

//	modeLabel = new QLabel(tr("Mode: Navigate"), this);
	windowString = tr("Windows: Attached");



	statusBar()->addWidget(modeLabel);
//	statusBar()->addWidget(statusLabel);

	//statusBar()->addWidget(windowLabel);

  statwindow=new StatusWindow(pos,datas,nviews,windows,0);
  clipwindow=new ClipWindow(colort,pos,datas,nviews,windows,0);

  //statwindow->update_status();

  statwindow->resize(400,600);

  statwindow->show();

  clipwindow->resize(600,350);

  clipwindow->show();

}
/*
	Sets the views to the right size when the window is resized.
*/

void CenterWindow::resizeEvent( QResizeEvent * ){
		 
		  std::vector<QString> com;
  com.push_back(QString::number(50)); com.push_back("main");
 int wold=curw, hold=curh;
 int wnew=width(), hnew=height();
 QString sizec=QString::number(0)+":"+QString::number(wold)+":"+
    QString::number(hold)+":"+QString::number(wnew)+":"+
    QString::number(hnew);
  com.push_back("resize"); com.push_back(sizec);
  curw=wnew; curh=hnew;

		 up->actionRespond(com);
    statwindow->update_status(up->hist);
		 
		}

void CenterWindow::help_action(const QString&text){
  if(text==""){}
}

/*
	Hyperview has some keyboard shortcuts for a wide array of actions. When one such key is pressed and released,
	this method deals with informing the Updater. As elsewhere, a vector is sent with key words as its elements.
*/
void CenterWindow::keyReleaseEvent( QKeyEvent *e){
  std::vector<QString> com;
  for(int i=0; i< 8; i++) com.push_back("");
  iactive=up->get_active();
  active=windows[iactive];
  com[0]=QString::number(iactive);
   com[1]="navigate";com[2]="movie";com[3]="stop";
 int nelem;
  switch( e->key() )
  {
  case Qt::Key_G:
      com[3]="go";
       up->actionRespond(com);
     break;
  case Qt::Key_P:
  if ( e->modifiers() & Qt::ControlModifier) {
      mouse_lab=funcs["pick"]->status;
                  change_mouse("pick");

      break;
    }
    else active->keyReleaseEvent(e);
    break;
  case Qt::Key_N:
    if ( e->modifiers() & Qt::ControlModifier) {
      mouse_lab=funcs["navigate"]->status;
            change_mouse("navigate");

      break;
    }
    else active->keyReleaseEvent(e);
    break;
  case Qt::Key_I:   
    if(stat_view.contains("mouse")>0){
      stat_view="pick";
    }
    else if(stat_view.contains("pick")>0){
      stat_view="value";
    }
    else if(stat_view.contains("value")>0){
      stat_view="position";
    }
    else if(stat_view.contains("position")>0){
      stat_view="mouse";
    }
    update_status_bar();
    break;
  
  case Qt::Key_Z:
    if ( e->modifiers() & Qt::ControlModifier) {
     mouse_lab=funcs["zoom"]->status;
      change_mouse("zoom");
      break;
    }
    else active->keyReleaseEvent(e);
  case Qt::Key_T:
    com.clear();
    if(itour>= (int)tour.size()) break;
      nelem=tour[itour].count("-");
      for(int i=0; i < nelem+1; i++) com.push_back(tour[itour].section("-",i,i));
      itour+=1;
      
      up->actionRespond(com);
      break;
  case Qt::Key_S:
	  up->actionRespond(com);
     break;
  default:
       up->actionRespond(com);
       active->keyReleaseEvent(e);
  }
}



void CenterWindow::load_tour(QString text){
    FILE *fd;
  char line[2048];
  QString ln;
  QString col;
  std::vector<QString> com2;
  com2.push_back("50");
  com2.push_back(" ");
 if ((fd=fopen(text.toAscii().constData(),"r")) == NULL) fprintf(stderr,"PROBLEM OPENING \n");

 while (!feof(fd)){
    if(NULL == fgets(line, 1000, fd)) break;
    if (0!=strncmp (line,"#",1)) {
      ln=line;
      (void) ln.trimmed();
      ln.remove(ln.length()-1,ln.length());
      tour.push_back(ln);
    }
  }

  fclose(fd);
 }

/*
	Deals with the creation of all the views in the program. The different views are divided by splitters, stored in the vector mainViews.
	The index 0 of windows is the "main" view which has certain menu options the other views do not.
*/
void CenterWindow::MakeWindows(){
	char temp_ch[128];
	joined.push_back(false);
	int idat;
    funcs["pick"]=new mouse_pick();
    funcs["navigate"]=new mouse_orient();
    funcs["zoom"]=new mouse_zoom();
	cur_func=funcs["zoom"];

	for(int i=0; i < nviews; i++){
		sprintf(temp_ch,"%d",i);
		mainViews.push_back(new QSplitter(Qt::Vertical,middleBox));
			
		menuBars.push_back(new menu_bar(pars,datas,8, temp_ch));
		idat=i;
		if(idat >= datas->return_ndat()) idat=datas->return_ndat()-1;
		windows.push_back(new DrawWindow(i,pars,pos,mym,pk,datas,idat,colort,cur_func,serv));
		mainViews[i]->setMinimumHeight(400);
		menuBars[i]->setFixedHeight(150);
		mainViews[i]->addWidget(menuBars[i]);
		mainViews[i]->addWidget(windows[i]);
	
	}

	for(int i= nviews; i < 2 * nviews-1; i++){
		sprintf(temp_ch,"%d",i);
		newWindow = new QWidget;
		indepChildWindows.push_back(newWindow);
		lay = new QVBoxLayout;
		mainViews.push_back(new QSplitter(Qt::Vertical, newWindow));
		menuBars.push_back(new menu_bar(pars,datas,8, temp_ch));
		mainViews[i]->setMinimumHeight(400);
		menuBars[i]->setFixedHeight(150);
		idat=i + 1 - nviews;
		if(idat >= datas->return_ndat()) idat=datas->return_ndat()-1;
		windows.push_back(new DrawWindow(i,pars,pos,mym,pk,datas,idat,colort,cur_func,serv));
		lay->addWidget(mainViews[i]);
		newWindow->setLayout(lay);
	    mainViews[i]->addWidget(menuBars[i]);
		mainViews[i]->addWidget(windows[i]);
	}
}

/*
	Changes the function of the mouse.
*/
void CenterWindow::change_mouse(QString text){
  cur_func=funcs[text];
   mouse_lab=funcs[text]->status;
  for(int i=0; i < (int) windows.size(); i++)
    windows[i]->set_mouse_func(funcs[text]);

}

/*
	Hides a  group of views. Depending on the bool passed in,
	this method will either hide the independent windows or the attached views.
*/
void CenterWindow::DestroyWindow(bool indep){
	
	if (!indep){
		for (int i = 0; i < nviews -1 ; i++){
			indepChildWindows[i]->hide();
		}
	}
	else{
		for (int i = 1; i < nviews; i++){
			mainViews[i]->hide();
		}
	}

}
/*
	Handles hiding and showing all the secondary windows.
*/
void CenterWindow::WindowHandler(bool indep){
    independent=indep;
	DestroyWindow(indep);
	if (indep){
		for (int i = 0; i < nviews - 1; i++)
		{
			indepChildWindows[i]->show();
		}
	}
	else{
		for (int i = 1; i < nviews; i++)
		{
			mainViews[i]->show();
		}
	}
}
