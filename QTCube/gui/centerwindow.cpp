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
#include "paramObj.h"
#include "slice_types.h"
#include<iostream>
#include <qregexp.h>
#include "util.h"
#include "mouse_orient.h"
#include "mouse_anno.h"
#include "mouse_zoom.h"
#include "mouse_pick.h"
#include "mouse_region.h"
#include "mouse_overlay.h"
#include "windows.h"
#include "panels.h"


using namespace SEP;

void MainWindow::create_panels(){

  int nviews=pars->getInt("nviews",1);
  int inum=0;
  for(int i=0; i < nviews; i++){
     QString par=tr("npanelx")+QString::number(i+1);
     int npanx=pars->getInt(std::string(par.toAscii().constData()),1);
     par=tr("npanely")+QString::number(i+1);
     int npany=pars->getInt(std::string(par.toAscii().constData()),1);
     int npan=npanx*npany;
     for(int ipan=0; ipan<npan; ipan++,inum++){
       std::shared_ptr<panel> x(new panel(my_pan->get_next_panel_num(), pars,pos,pk,datas,0,colort,cur_func,serv,my_or,my_maps));
       my_pan->add_panel(x);

     }
  }
  stat_view="mouse";
}
void MainWindow::create_windows(){
   int nviews=pars->getInt("nviews",1);
   int ipan=0;
   for(int iv=0; iv < nviews; iv++){     

        QString par=tr("npanelx")+QString::number(iv+1);
        int npanx=pars->getInt(std::string(par.toAscii().constData()),1);
        par=tr("npanely")+QString::number(iv+1);
        int npany=pars->getInt(std::string(par.toAscii().constData()),1);
        int npan=npanx*npany;
        std::vector<std::shared_ptr<panel>> pp;
        for(int i=0; i < npan; i++){
          pp.push_back(my_pan->get_panel(i+ipan));
          my_pan->set_window(i+ipan,i);
        }
        ipan+=npan;
        std::shared_ptr<DrawWindow> d(new DrawWindow(npanx,npany,pp,iv));
        my_wind->add_window(d);

   }
}
void MainWindow::create_mouse_funcs(){
  std::shared_ptr<mouse_pick> mp(new mouse_pick());
  std::shared_ptr<mouse_orient> mn(new mouse_orient());
  std::shared_ptr<mouse_zoom> mz(new mouse_zoom());
  std::shared_ptr<mouse_region> mr(new mouse_region());
  std::shared_ptr<mouse_overlay> mo(new mouse_overlay());
  std::shared_ptr<mouse_anno> ma(new mouse_anno());

   funcs["pick"]=mp;
    funcs["navigate"]=mn;
    funcs["zoom"]=mz;
    funcs["region"]=mr;
    funcs["overlay"]=mo;
    funcs["annotate"]=ma;

	cur_func=funcs["zoom"];

}
MainWindow::MainWindow(std::shared_ptr<genericIO> ios,std::shared_ptr<datasets> d,
 std::shared_ptr<pick_draw> _pks, std::shared_ptr<slice_types> col){

  _io=ios;
  pars=ios->getParamObj();
  std::shared_ptr<util> xutil(new util(pars));
  pp=xutil;
 datas=d;
    std::shared_ptr<orients> o(new orients());
      my_or=o;

    connect(datas.get() , SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

//setStyleSheet("border-radius 0px; font: bold 14px; padding: 0px; border-style: inset; spacing: 0px");
//setStyleSheet("QGroupBox { spacing: 40px }");
setStyleSheet("margin: 3px");
setStyleSheet("QGroupBox { border: 2px solid gray; border-radius: 3px; }");
setStyleSheet("QGroupBox::title { background-color: transparent; subcontrol-position: top left; padding:9 13px;}");
    pk=_pks;
    stat_view="mouse";
	this->colort=col;
    QFont font("Arial", 12, QFont::Normal);
	QApplication::setFont(font);
	std::shared_ptr<maps> mym(new maps(_io)); 
	    my_maps=mym;

    std::shared_ptr<panels> pp(new panels(_io,pk,my_maps,my_or));
    my_pan=pp;
	datas->set_orients(my_or);
	std::shared_ptr<windows> myw(new windows(my_pan));
	my_wind=myw;

	int beg[8],end[8],init[8];
    for(int i=0; i< 8; i++){
       beg[i]=0; end[i]=datas->return_dat(0)->get_grid_axis(i+1).n; init[i]=end[i]/2;
    }
     modeLabel = new QLabel(mouse_lab, this);
  	statusBar()->addWidget(modeLabel);
   create_mouse_funcs();
   std::shared_ptr<position> ps(new position(std::static_pointer_cast<hypercube>(datas->return_dat(0)->return_grid()),init,beg,end,2));
   pos=ps;
    std::shared_ptr<orientation_server> s2(new orientation_server(pos));
    
 	serv=s2;;
        pk->set_server(serv);
  pk->set_position(pos);
 	create_panels();
 	create_windows();


     QWidget *widget = new QWidget;
     setCentralWidget(widget);

     std::shared_ptr<window_panel> myw22(new window_panel(pos,my_wind,my_pan,datas,pk,colort,my_maps));
     my_window_panel = myw22;
     my_window_panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
     my_window_panel->setMaximumHeight(75);


     std::shared_ptr<panel_panel> pp22(new panel_panel(pos,my_wind,my_pan,datas,pk,colort,my_maps));
     my_panel_panel = pp22;
     my_panel_panel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

     QVBoxLayout *layout = new QVBoxLayout;
     layout->setMargin(5);
     layout->addWidget(my_window_panel.get());
     layout->addWidget(my_panel_panel.get());
   //  layout->addWidget(bottomFiller);
     widget->setLayout(layout);
     std::shared_ptr<data_panel> myp(new data_panel(pos,my_wind,my_pan,datas,pk,colort,my_maps));
     
     my_data_panel=myp;

     createActions();
     createMenus();
      
      
     std::shared_ptr<color_panel> cp2(new color_panel(pos,my_wind,my_pan,datas,pk,colort,my_maps));
     my_color_panel=cp2;
      std::shared_ptr<help_panel> hp2 (new help_panel(pos,my_wind,my_pan,datas,pk,colort,my_maps));
       std::shared_ptr<ano_panel> ap2(new ano_panel(pos,my_wind,my_pan,datas,pk,colort,my_maps));
        std::shared_ptr<pick_panel> pp3(new pick_panel(pos,my_wind,my_pan,datas,pk,colort,my_maps));
     my_help_panel=hp2;
     my_help_panel->setMinimumSize(200,200);
     my_ano_panel=ap2;
     my_pick_panel=pp3 ;
     std::shared_ptr<history> hs(new history());
     my_hist=hs;

     mouse_lab=funcs["zoom"]->status;     
     
     setWindowTitle(tr("qt_cube"));
     setMinimumSize(160, 160);
     resize(630,650);
     

     for(int i=0; i < my_wind->nwind(); i++)  {
        my_wind->return_window(i)->resize(600,400);
        my_wind->return_window(i)->update_panel_map();
        my_wind->return_window(i)->show();
      }
      connect_it();
     my_pan->update_all();
    update_status_bar();
  std::vector<QString> coms; coms.push_back("-1"); coms.push_back("redraw");coms.push_back("all"); actionRespond(coms);
   coms[1]="view"; coms[2]="data";
   coms.push_back("");
   for(int ipan=1; ipan < my_pan->size(); ipan++){
    coms[0]=QString::number(ipan);
    
    coms[3]=QString::number(std::min(ipan,datas->return_ndat()-1));
    actionRespond(coms);
   }
 my_pan->update_all();
 my_pan->set_active(0);
 coms[0]="-1";
 coms[1]="redraw";
 coms[2]="all";
 datas->afterInitialization();
 actionRespond(coms);
 }
 void MainWindow::connect_it(){

 
   connect(my_pan.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
   connect(my_panel_panel.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
   connect(my_pan.get(), SIGNAL(windows_update()), my_wind.get(), SLOT(update_all()));
   connect(my_window_panel.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
   connect(my_wind.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
   connect(my_hist.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
    connect(my_ano_panel.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
    connect(my_color_panel.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
     connect(my_pick_panel.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
     connect(my_data_panel.get(), SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

 }
  void MainWindow::secondaryRespond(std::vector<QString> coms){
 
    
 
 
 }
 void MainWindow::actionRespond(std::vector<QString> coms){
 
// for(int i=0; i < coms.size(); i++)  fprintf(stderr,"%s ",coms[i].toAscii().constData());
 // fprintf(stderr,"\n");
  //fflush(stderr);
   if(coms[1].contains("none")) return;
   if(coms[0].contains("annotate")) coms.insert(coms.begin(),QString::number(my_wind->get_active_num()));
      if(coms[0].contains("auto")) coms.insert(coms.begin(),QString::number(my_wind->get_active_num()));
   if(!coms[1].contains("clip")) my_hist->addEvent(coms);
   
   

   
   
   if(coms[1].contains("navigate")) my_pan->perform_navigate(coms);
    else if(coms[1].contains("pick")) my_pan->perform_pick(coms);
    else if(coms[1].contains("auto")) my_pan->perform_auto(coms);
    else if(coms[1].contains("display")) {
        my_pan->perform_display(coms);
        }
        else if(coms[1].contains("navigate")) my_pan->perform_navigate(coms);
        else if(coms[1].contains("orient")) my_pan->perform_orient(coms);
        else if(coms[1].contains("windows")) my_wind->windows_respond(coms);
        else if(coms[1].contains("annotate")) my_wind->annotate_respond(coms);

    else if(coms[1].contains("view")) my_pan->perform_view(coms);
   else if(coms[1].contains("ctable")) colort->update_colortable(coms);
      else if(coms[1].contains("data")) {
         datas->update_data(coms);
            my_wind->update_displays(true);
   }
   else if(coms[1].contains("redraw")){

      if(coms[2].contains("only")) my_pan->update_only(coms[0].toInt());
      else if(coms[2].contains("all")) my_pan->update_all();
      else if(coms[2].contains("limited")) my_pan->update_it(coms[0].toInt());
   }
   if(!coms[1].contains("panels") && !coms[1].contains("windows") && !coms[1].contains("menu") && 
    !coms[1].contains("clip")){
     if(coms[1].contains("ctable"))      my_wind->update_displays(true);
      else my_wind->update_displays();
   }
   my_panel_panel->update_menu(coms);

   my_data_panel->update_menu(coms);

   my_ano_panel->update_menu(coms);

   my_color_panel->update_menu(coms);

   my_pick_panel->update_menu(coms);

   my_window_panel->update_menu(coms);

   bool v; int i=coms[0].toInt(&v);

   if(v){
     if(i!=-1 && !coms[1].contains("menu") &&!coms[1].contains("clip") ){
       if(coms[1].contains("windows") ||coms[1].contains("annotate")||coms[1].contains("data") ){
          my_window_panel->set_current_index(i);        
          my_wind->set_active(i);
       }
       else{
         int iwind=my_pan->get_window(i);
         my_wind->set_active_panel(iwind,i);
          my_window_panel->set_current_index(iwind);
          my_panel_panel->set_current_index(i);
          my_wind->set_active(iwind);
          my_pan->set_active(i);

       }
     }
    }
    update_status_bar();
    coms[0]="none"; coms[1]=="none";
 }
 void MainWindow::update_status_bar(){
 
  if(stat_view.contains("mouse") >0) {
    modeLabel->setText(mouse_lab);
  }
  else if(stat_view.contains("position") >0) {
      modeLabel->setText(my_pan->get_panel(0)->get_orient()->return_pos_label());

  }  
  else if(stat_view.contains("overlay")>0){
      std::shared_ptr<mouse_overlay> x=std::static_pointer_cast<mouse_overlay>(funcs["overlay"]);
      modeLabel->setText(x->get_info());
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
    //  modeLabel->setText(statwindow->return_val_lab());
  }

  }
void MainWindow::newFile(){

  int in=my_pan->get_next_panel_num();

  std::shared_ptr<panel> x=my_pan->get_panel(0)->clone(in);

  my_pan->add_panel(x);

  std::vector<QString> coms;
  coms.push_back("-1");
  coms.push_back("panels");
  coms.push_back("add");
  coms.push_back(QString::number(in));
     my_panel_panel->update_menu(coms);

     std::vector<std::shared_ptr<panel>> pp; pp.push_back(x);


  std::shared_ptr<DrawWindow> dw(new DrawWindow(1,1,pp,my_wind->size()));
     my_wind->add_window(dw);
               my_pan->set_window(in,my_wind->size()-1);


     coms[1]="windows";
     my_window_panel->update_menu(coms);
 
     my_wind->my_winds[my_wind->size()-1]->show();
      my_wind->return_window(my_wind->size()-1)->resize(600,400);
        my_wind->return_window(my_wind->size()-1)->update_panel_map();
        my_wind->return_window(my_wind->size()-1)->show();
 my_wind->update_displays(true);


 }
void MainWindow::lockViews(){
    my_pan->set_locked(true);
 }
void MainWindow::unlockViews(){
    my_pan->set_locked(false);
 }
void MainWindow::open(){
 QString filename = QFileDialog::getOpenFileName(this );
       if ( !filename.isNull() ){
   my_hist->load_history(filename); }
   
   }
void MainWindow::save(){
 QString filename = QFileDialog::getSaveFileName(this );
       if ( !filename.isNull() ){
   my_hist->write_history(filename);
  }

 }
void MainWindow::mouseAnnotate(){\

	cur_func=funcs["annotate"];
	     my_pan->update_mouse(cur_func);
	          mouse_lab=funcs["annotate"]->status;     


}
void MainWindow::mouseZoom(){
	cur_func=funcs["zoom"];
     my_pan->update_mouse(cur_func);
          mouse_lab=funcs["zoom"]->status;     

}
void MainWindow::statusOverlay(){
 stat_view="overlay";
   update_status_bar();

 }
void MainWindow::statusMouse(){
stat_view="mouse";
  update_status_bar();

}
void MainWindow::statusPosition(){
stat_view="position";
  update_status_bar();

}
void MainWindow::statusPick(){
stat_view="pick";
  update_status_bar();

}
void MainWindow::mouseOverlay(){
	cur_func=funcs["overlay"];
     my_pan->update_mouse(cur_func);
     mouse_lab=funcs["overlay"]->status;     


}
void MainWindow::mousePick(){
	cur_func=funcs["pick"];
     my_pan->update_mouse(cur_func);
     mouse_lab=funcs["pick"]->status;     

}
void MainWindow::mouseRegion(){
	cur_func=funcs["region"];
     my_pan->update_mouse(cur_func);
     mouse_lab=funcs["region"]->status;     

}
void MainWindow::mouseNavigate(){
	cur_func=funcs["navigate"];
     my_pan->update_mouse(cur_func);
     mouse_lab=funcs["navigate"]->status;     

}
 void MainWindow::closeAnno(){

}
void MainWindow::openAnno(){
   my_ano_panel->show();

}
void MainWindow::closeData(){

}
void MainWindow::openData(){
  my_data_panel->show();
}
void MainWindow::closeColor(){

}
void MainWindow::openColor(){
   my_color_panel->show();
}
void MainWindow::closePick(){

}
void MainWindow::openPick(){
   my_pick_panel->show();

}
void MainWindow::closeHelp(){

}
void MainWindow::openHelp(){
   my_help_panel->show();

}
 void MainWindow::about(){
     QMessageBox::about(this, tr("About Sep_cube"),
             tr("<b>Sep_cube</b>  is a multi-dimensional viewer of regularly sampled cube.  "
                "If you find bugs please email the author bob@sep.stanford.edu"));
 }
 void MainWindow::aboutQt(){
 }
void MainWindow::createActions() {
     newAct = new QAction(tr("&New Window"), this);
     newAct->setShortcut(tr("Ctrl+W"));
     newAct->setStatusTip(tr("Create a new file"));
     connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

     lockAct = new QAction(tr("Lock views"), this);
     //lockAct->setShortcut(tr("Ctrl+L"));
     lockAct->setStatusTip(tr("Open an existing file"));
     connect(lockAct, SIGNAL(triggered()), this, SLOT(lockViews()));

     unlockAct = new QAction(tr("Unlock views"), this);
     //lockAct->setShortcut(tr("Ctrl+L"));
     unlockAct->setStatusTip(tr("Open an existing file"));
     connect(unlockAct, SIGNAL(triggered()), this, SLOT(unlockViews()));
     
     openAct = new QAction(tr("&Load..."), this);
     openAct->setShortcut(tr("Ctrl+L"));
     connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

     saveAct = new QAction(tr("&Save"), this);
     saveAct->setShortcut((tr("Ctrl+S")));
     connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));


     mNavAct = new QAction(tr("Navigate"), this);
     mNavAct->setShortcut((tr("Ctrl+N")));
     connect(mNavAct, SIGNAL(triggered()), this, SLOT(mouseNavigate()));

     mZoomAct = new QAction(tr("Zoom"), this);
     mZoomAct->setShortcut((tr("Ctrl+Z")));
     connect(mZoomAct, SIGNAL(triggered()), this, SLOT(mouseZoom()));

     mRegionAct = new QAction(tr("Region"), this);
     mRegionAct->setShortcut((tr("Ctrl+R")));
     connect(mRegionAct, SIGNAL(triggered()), this, SLOT(mouseRegion()));
     
     mAnnoAct = new QAction(tr("Annotate"), this);
      mAnnoAct->setShortcut((tr("Ctrl+A")));
     connect(mAnnoAct, SIGNAL(triggered()), this, SLOT(mouseAnnotate()));
     
     mPickAct = new QAction(tr("Pick"), this);
     mPickAct->setShortcut((tr("Ctrl+P")));
     connect(mPickAct, SIGNAL(triggered()), this, SLOT(mousePick()));

     mOverAct = new QAction(tr("Overlay"), this);
    mOverAct->setShortcut((tr("Ctrl+O")));
     connect(mOverAct, SIGNAL(triggered()), this, SLOT(mouseOverlay()));

     sMouseAct = new QAction(tr("Mouse"), this);
    // printAct->setShortcuts(QKeySequence::Print);
     connect(sMouseAct, SIGNAL(triggered()), this, SLOT(statusMouse()));

     sPositionAct = new QAction(tr("Position"), this);
    // printAct->setShortcuts(QKeySequence::Print);
     connect(sPositionAct, SIGNAL(triggered()), this, SLOT(statusPosition()));
     
     sOverlayAct = new QAction(tr("Overlay"), this);
    // printAct->setShortcuts(QKeySequence::Print);
     connect(sOverlayAct, SIGNAL(triggered()), this, SLOT(statusOverlay()));
     
     sPickAct = new QAction(tr("Picking"), this);
    // printAct->setShortcuts(QKeySequence::Print);
     connect(sPickAct, SIGNAL(triggered()), this, SLOT(statusPick()));


    yesAnnoAct = new QAction(tr("Annotate"), this);
     yesAnnoAct->setShortcut(tr("Ctrl+Shift+A"));
  //   yesDataAct->setStatusTip(tr("Show annotation menu"));
     connect(yesAnnoAct, SIGNAL(triggered()), this, SLOT(openAnno()));

        yesColorAct = new QAction(tr("Color"), this);
     yesColorAct->setShortcut(tr("Ctrl+Shift+C"));
  //   yesDataAct->setStatusTip(tr("Show annotation menu"));
     connect(yesColorAct, SIGNAL(triggered()), this, SLOT(openColor()));

        noColorAct = new QAction(tr("Hide"), this);
    // yesDataAct->setShortcuts(QKeySequence::Print);
  //   yesDataAct->setStatusTip(tr("Show annotation menu"));
     connect(noColorAct, SIGNAL(triggered()), this, SLOT(closeColor()));


     noAnnoAct = new QAction(tr("Hide"), this);
    // yesDataAct->setShortcuts(QKeySequence::Print);
     //noDataAct->setStatusTip(tr("Hide annotation menu"));
     connect(noAnnoAct, SIGNAL(triggered()), this, SLOT(closeAnno()));

     yesDataAct = new QAction(tr("Data"), this);
     yesDataAct->setShortcut(tr("Ctrl+Shift+D"));
     connect(yesDataAct, SIGNAL(triggered()), this, SLOT(openData()));

     noDataAct = new QAction(tr("&Hide"), this);
    // yesDataAct->setShortcuts(QKeySequence::Print);
     connect(noDataAct, SIGNAL(triggered()), this, SLOT(closeData()));

     yesPickAct = new QAction(tr("Pick"), this);
     yesPickAct->setShortcut(tr("Ctrl+Shift+P"));
     connect(yesPickAct, SIGNAL(triggered()), this, SLOT(openPick()));

     noPickAct = new QAction(tr("&Hide"), this);
    // yesDataAct->setShortcuts(QKeySequence::Print);
     connect(noPickAct, SIGNAL(triggered()), this, SLOT(closePick()));
     
     yesHelpAct = new QAction(tr("Help"), this);
      yesHelpAct->setShortcut(tr("Ctrl+Shift+H"));
     connect(yesHelpAct, SIGNAL(triggered()), this, SLOT(openHelp()));

     noHelpAct = new QAction(tr("&Hide"), this);
    // noHelpAct->setShortcuts(QKeySequence::Print);
     connect(noHelpAct, SIGNAL(triggered()), this, SLOT(closeHelp()));


     exitAct = new QAction(tr("E&xit"), this);
//     exitAct->setShortcuts(QKeySequence::Quit);
     connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

     

     aboutAct = new QAction(tr("&About"), this);
     aboutAct->setStatusTip(tr("Show the application's About box"));
     connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

     aboutQtAct = new QAction(tr("About &Qt"), this);
     aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
     connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
     connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));

 }
 void MainWindow::createMenus(){
     mainMenu = menuBar()->addMenu(tr("&Main"));
     mainMenu->addAction(newAct);
     mainMenu->addAction(exitAct);
     mainMenu->addAction(lockAct);
     mainMenu->addAction(unlockAct);
     
     
     mouseMenu=menuBar()->addMenu(tr("Mouse"));
     mouseMenu->addAction(mZoomAct);
     mouseMenu->addAction(mPickAct);
     mouseMenu->addAction(mNavAct);
     mouseMenu->addAction(mAnnoAct);
     mouseMenu->addAction(mOverAct);
     mouseMenu->addAction(mRegionAct);

     statusMenu=menuBar()->addMenu(tr("Status"));
     statusMenu->addAction(sMouseAct);
     statusMenu->addAction(sPositionAct);
     statusMenu->addAction(sOverlayAct);
     statusMenu->addAction(sPickAct);

     


     historyMenu = menuBar()->addMenu(tr("&History"));
     historyMenu->addAction(openAct);
     historyMenu->addAction(saveAct);
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
     helpMenu->addAction(aboutQtAct);
     
     panelMenu = menuBar()->addMenu(tr("&Panels"));
    panelMenu->addAction(yesColorAct);
    panelMenu->addAction(yesAnnoAct);
     panelMenu->addAction(yesDataAct);
     panelMenu->addAction(yesPickAct);
     panelMenu->addAction(yesHelpAct);

 }
