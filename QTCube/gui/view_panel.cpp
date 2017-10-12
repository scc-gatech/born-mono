#include "view_panel.h"
using namespace SEP;
view_panel:: view_panel(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, 
std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk, 
std::shared_ptr<slice_types>c,std::shared_ptr<maps> mym){




  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  layMain=new QVBoxLayout;
  
  	std::shared_ptr<hypercube> h=my_dat->return_dat(0)->return_grid();
	for(int i=0; i < 8; i++) {
	   if(h->getAxis(i+1).n >1) validax[i]=true;
	   else validax[i]=false;
	}
  
  row1=build_row_1();
  row2=build_row_2();
  row3=build_row_3();
  layMain->addWidget(row1);
  layMain->addWidget(row2);
  layMain->addWidget(row3);
  setLayout(layMain);
  
  }
  void view_panel::update_menu(std::vector<QString> coms){

    if(coms[1].contains("view")){
     if(coms[3].contains("odata")){
         over->set_current(0,coms[3].toInt()-1);
      }
      else if(coms[2].contains("data")){
         data->set_current(0,coms[3].toInt());
      }
    }
    else if(coms[1].contains("data") && coms[2].contains("new")){
      int idat=my_dat->return_ndat()-1;
      data->my_combos[0]->addItem(my_dat->return_dat(idat)->get_title_short());
      over->my_combos[0]->addItem(my_dat->return_dat(idat)->get_title_short());
    }
    else if(coms[1].contains("view")){
      if(coms[2].contains("map1") && coms[3].contains("load")){
         usingMap1->my_combos[0]->addItem(my_maps->return_map(my_maps->size()-1)->return_name());
      }
   }

  }
QWidget *view_panel::build_row_1(){
    QWidget *mine=new QWidget();
    lay1=new QHBoxLayout();
    
    std::map<QString,bool> *typ;
    std::vector<QString> types;
    typ=my_v->return_views();
    for(std::map<QString,bool>::iterator i=typ->begin(); i!=typ->end(); i++) types.push_back(i->first);
    
        
    perspective=new basicComboBox("Perspective","Perspective to use",types);
    cycleData=new basicButtonBox("Cycle data",":/images/back.png","Switch back one dataset",true);
    cycleData->addButton(":/images/forward.png","Switch back one dataset",true);
    perspective->set_current(0,4);
    std::vector<QString> dlist;
    for(int i=0; i< my_dat->return_ndat(); i++) dlist.push_back(my_dat->return_dat(i)->get_name());
    data=new basicComboBox("Data","Data to display",dlist);
    
    std::vector<QString> dislist=my_c->return_cvec();
    mainType=new basicComboBox("Display","Type of display",my_slices->names);
    

	connect(perspective->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(perspective_change(const QString&)));
    connect(cycleData->my_radios[1], SIGNAL(clicked()), this, SLOT(roundRobinForward()));
    connect(cycleData->my_radios[0], SIGNAL(clicked()), this, SLOT(roundRobinBack()));
	connect(mainType->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(display_change(const QString&)));
	connect(data->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(data_change(const QString&)));

    
    lay1->addWidget(perspective->group());
    lay1->addWidget(cycleData->group());
    lay1->addWidget(data->group());
    lay1->addWidget(mainType->group());
 
    
    lay1->addStretch(1);

     mine->setLayout(lay1);

     return mine;
  }

QWidget * view_panel::build_row_2(){
      QWidget *mine=new QWidget();
      lay2=new QHBoxLayout();
      
          std::vector<QString> dlist;
    for(int i=0; i< my_dat->return_ndat(); i++) dlist.push_back(my_dat->return_dat(i)->get_name());
    std::vector<QString> dislist=my_c->return_cvec();


    opacity=new basicSliderBox("Opacity","Relative of opacity of overlay",0,128,256);
    cycleOver=new basicButtonBox("Cycle overlay data",":/images/back.png","Switch back one dataset",true);
    cycleOver->addButton(":/images/forward.png","Switch back one dataset",true);
    overType=new basicComboBox("Overlay Display","Type of display for overlay",my_slices->names);
    dlist.insert(dlist.begin(),"None");
    over=new basicComboBox("Overlay Data","Dataset to use for overlay",dlist);
    overType->set_current(0,4);
    connect(cycleOver->my_radios[1], SIGNAL(clicked()), this, SLOT(oRoundRobinForward()));
    connect(cycleOver->my_radios[0], SIGNAL(clicked()), this, SLOT(oRoundRobinBack()));
	connect(overType->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(odisplay_change(const QString&)));
	connect(over->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(odata_change(const QString&)));
    connect(opacity->my_sliders[0],SIGNAL(valueChanged(int)),this, SLOT(update_opactiy(int)));
   lay2->addWidget(opacity->group());
   lay2->addWidget(cycleOver->group());
    lay2->addWidget(overType->group());
    lay2->addWidget(over->group());
       lay2->addStretch(1);
     mine->setLayout(lay2);

     return mine;
  
  }
QTabWidget * view_panel::build_row_3(){
      QTabWidget *mine=new QTabWidget();
      wiggle=wiggle_tab();
      contour=contour_tab();
      grid=grid_tab();
      map1=map1_tab();
      mine->addTab(wiggle,"Wiggle");
      mine->addTab(contour,"Contour");
      mine->addTab(grid,"Grid");
#ifdef CHEVRON
      mine->addTab(map1,"Flatten map");
#endif




     return mine;
  }
void view_panel::delete_row_1(){

    delete cycleData;
    delete data;
    delete perspective;
    delete mainType;

    delete row1;
    delete lay1;
    


  }
void view_panel::delete_row_2(){

    delete overType;
    delete over;
    delete opacity;
    delete cycleOver;
    delete row2;
    delete lay2;
    
  
  }    
void view_panel::delete_row_3(){


   delete_wiggle_tab();
  delete_grid_tab();
   delete_contour_tab();
   delete map1_tab();
   delete wiggle;
  delete contour;
  delete grid;
  delete map1;
   
  
  }   
  QWidget * view_panel::map1_tab(){
     QWidget *mine=new QWidget();
     layMap1=new QHBoxLayout();
     
     std::vector<QString> mp=my_maps->return_names();
     mp.insert(mp.begin(),"None");
     usingMap1=new basicComboBox("Map","Map distorting to use",mp);
     overSamp=new basicLineEditBox("Oversample",7,"1.","Oversample factor of map compared to data being displayed");
     loadMap1=new basicButtonBox("Load", ":/images/open.png","Load new map",true);
    
      usingMap1->set_current(0,0);
     
     connect(usingMap1->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_map1(const QString&)));
     connect(loadMap1->my_radios[0], SIGNAL(clicked()),this, SLOT(load_map()));

     layMap1->addWidget(usingMap1->group());
          layMap1->addWidget(overSamp->group());
     layMap1->addWidget(loadMap1->group());

     layMap1->addStretch(1);
     mine->setLayout(layMap1);
     return mine;
     }
void view_panel::delete_map1_tab(){
      delete usingMap1;
      delete overSamp;
      delete loadMap1;

       delete layMap1;
     }
  
QWidget * view_panel::wiggle_tab(){
     QWidget *mine=new QWidget();
     layWiggle=new QHBoxLayout();
     
     std::vector<QString>  mp=my_c->return_cvec();
     std::vector<QString> color;
     color.push_back("none");
     for(int i=0; i < (int) mp.size(); i++) color.push_back(mp[i]);
     
     lineColor=new basicComboBox("Line color","Color of wiggle lines",mp);
     posFill=new basicComboBox("Positive fill","Fill color for positive values",color);
     negFill=new basicComboBox("Negative fill","Fill color for negative values",color);
     lineColor->set_current(0,8);
     posFill->set_current(0,9);
     negFill->set_current(0,0);
     connect(lineColor->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(wiggle_color_change(const QString&)));
     connect(posFill->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(wiggle_pos_change(const QString&)));
     connect(negFill->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(wiggle_neg_change(const QString&)));

     layWiggle->addWidget(lineColor->group());
          layWiggle->addWidget(posFill->group());
     layWiggle->addWidget(negFill->group());

     layWiggle->addStretch(1);
     mine->setLayout(layWiggle);
     return mine;
     }
void view_panel::delete_wiggle_tab(){
     

      delete lineColor; delete posFill; delete negFill;
       delete layWiggle;
     }
QWidget *  view_panel::grid_tab(){
     
    QWidget *mine=new QWidget();
     layGrid=new QHBoxLayout();

     std::vector<QString> lst;
     for(int i=1; i <=30; i++) lst.push_back(QString::number(i));
     
     grid1=new basicComboBox("Grid x","Number of samples in X",lst);
     grid2=new basicComboBox("Grid y","Number of samples in Y",lst);
     
     sampleGrid=new basicLineEditBox("Sampling",7,"1","Increment in third axis to display");
     	connect(sampleGrid->my_edits[0], SIGNAL(textChanged(const QString&)),this, SLOT(change_jplane(const QString&)));
     	 connect(grid1->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_grid1(const QString&)));
     	connect(grid2->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_grid2(const QString&)));

     layGrid->addWidget(grid1->group());
     layGrid->addWidget(grid2->group());
     layGrid->addWidget(sampleGrid->group());
     layGrid->addStretch(1);
     mine->setLayout(layGrid);
     return mine;
     }
void view_panel::delete_grid_tab(){
     
            delete grid1;
            delete grid2;
            delete sampleGrid;
            delete layGrid;

     }
QWidget *  view_panel::contour_tab(){
     QWidget *mine=new QWidget();
     layContour=new QHBoxLayout();

     std::vector<QString>  mp=my_c->return_cvec(); 
     contourColor=new basicComboBox("Color","Color of the contour lines",mp);
    contourColor->set_current(0,8);
     std::vector<QString> num;
     num.push_back("2"); num.push_back("3"); num.push_back("4"); num.push_back("5"); 
     num.push_back("6"); num.push_back("7"); num.push_back("8"); num.push_back("10"); 
     num.push_back("15"); num.push_back("20"); num.push_back("25");
     numLines=new basicComboBox("Number","Number of contour lines",num);
     numLines->set_current(0,3);
     firstContour=new basicLineEditBox("First",7,"","First contour value");
     sampleContour=new basicLineEditBox("sampling",7,"","Sampling of contours");


    connect(contourColor->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(contour_color_change(const QString&)));
     connect(numLines->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(num_lines_change(const QString&)));
     connect(firstContour->my_edits[0], SIGNAL(textChanged(const QString&)),this, SLOT(first_contour_change(const QString&)));
     connect(sampleContour->my_edits[0], SIGNAL(textChanged(const QString&)),this, SLOT(sample_contour_change(const QString&)));

     layContour->addWidget(contourColor->group());
     layContour->addWidget(numLines->group());
     layContour->addWidget(firstContour->group());
     layContour->addWidget(sampleContour->group());
    
     layContour->addStretch(1);
     mine->setLayout(layContour);
     return mine;
     }
void view_panel::perspective_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("type"); coms.push_back(val);
  emit actionDetected(coms);
}
void view_panel::display_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("colortable"); coms.push_back(val);
  emit actionDetected(coms);
}
void view_panel::change_grid1(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("grid_grid1"); coms.push_back(val);
  emit actionDetected(coms);
}
void view_panel::change_grid2(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("grid_grid2"); coms.push_back(val);
  emit actionDetected(coms);
}


void view_panel::load_map(){
  QString filename = QFileDialog::getOpenFileName(this );
  if ( !filename.isNull() ){
    std::vector<QString> coms;
    coms.push_back("view"); coms.push_back("map1"); coms.push_back("load");
    QString a= overSamp->my_edits[0]->displayText();
    bool v; float x= a.toFloat(&v); if(!v) a="1.0"; coms.push_back(a);
    coms.push_back(filename);
    emit actionDetected(coms);
  }

}
void view_panel::change_map1(const QString vals){

 std::vector<QString> coms;
 coms.push_back("view"); coms.push_back("map1"); coms.push_back("change");
 coms.push_back(vals);
 emit actionDetected(coms);



}

void view_panel::change_jplane(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("grid_jplane"); coms.push_back(val);
  emit actionDetected(coms);
}
void view_panel::odisplay_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("ocolortable"); coms.push_back(val);
  emit actionDetected(coms);
}

void view_panel::odata_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("odata"); 
  if(val.contains("None")) coms.push_back("-1");
  else
    coms.push_back(QString::number(my_dat->dataNumFromName(val)));
  emit actionDetected(coms);
}
void view_panel::wiggle_color_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("wiggle_color"); 
  coms.push_back(val);
  emit actionDetected(coms);
}
void view_panel::wiggle_pos_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("wiggle_pos"); 
  coms.push_back(val);
  emit actionDetected(coms);
}
void view_panel::wiggle_neg_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("wiggle_neg"); 
  coms.push_back(val);
  emit actionDetected(coms);
}

void view_panel::update_opactiy(int val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("opacity"); 
  coms.push_back(QString::number(val));
  emit actionDetected(coms);
}
void view_panel::data_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("data"); 
  coms.push_back(QString::number(my_dat->dataNumFromName(val)));
  emit actionDetected(coms);
}
void view_panel::contour_color_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("contour_color"); 
  coms.push_back(val);
  emit actionDetected(coms);
}
void view_panel::sample_contour_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("contour_dc"); 
  coms.push_back(val);
  emit actionDetected(coms);
}

void view_panel::num_lines_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("contour_number"); 
  coms.push_back(val);
  emit actionDetected(coms);
}

void view_panel::first_contour_change(const QString val){

  std::vector<QString> coms;
  coms.push_back("view"); coms.push_back("contour_c0"); 
  coms.push_back(QString::number(my_dat->dataNumFromName(val)));
  emit actionDetected(coms);
}


void view_panel::delete_contour_tab(){
     
       delete contourColor;
       delete numLines;
       delete firstContour;
       delete sampleContour;
       delete layContour;
     }
  void view_panel::roundRobinForward(){
    std::vector<QString> coms;
    coms.push_back("view");
    coms.push_back("roundrobin");
    coms.push_back("forward");
    emit actionDetected(coms);
  }
    void view_panel::roundRobinBack(){
    std::vector<QString> coms;
    coms.push_back("view");
    coms.push_back("roundrobin");
    coms.push_back("back");
    emit actionDetected(coms);
  }
  
   void view_panel::oRoundRobinForward(){
    std::vector<QString> coms;
    coms.push_back("view");
    coms.push_back("roundorobin");
    coms.push_back("forward");
    emit actionDetected(coms);
  }
    void view_panel::oRoundRobinBack(){
    std::vector<QString> coms;
    coms.push_back("view");
    coms.push_back("roundorobin");
    coms.push_back("back");
    emit actionDetected(coms);
  }
