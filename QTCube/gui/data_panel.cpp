#include "data_panel.h"
#include "display_panel.h"
#include "orient_panel.h"
#include "view_panel.h"
#include "navigate_panel.h"
using namespace SEP;
data_panel:: data_panel(std::shared_ptr<position >p,std::shared_ptr<windows>my_w, 
std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk, 
std::shared_ptr<slice_types> c,std::shared_ptr<maps>mym){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);

  for(int i=0; i < my_dat->return_ndat(); i++){
    QString tit=my_dat->return_dat(i)->get_title_short();
    my_tabs[tit]=new data_sub_panel(p,my_w,my_p,my_d,pk,c,mym,i);
    connect(my_tabs[tit], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
    addTab(my_tabs[tit],tit);
 }
   
  my_tabs["add"]=new add_dataset(p,my_w,my_p,my_d,pk,c,mym);
  connect(my_tabs["add"], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
  addTab(my_tabs["add"],"+");
  
}
add_dataset::add_dataset(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, std::shared_ptr<panels>my_p, 
std::shared_ptr<datasets> my_d, std::shared_ptr<pick_draw> pk, std::shared_ptr<slice_types>c,std::shared_ptr<maps>mym){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  create_sep_tab();
  create_seispak_tab();

  create_surface_tab();


}
void add_dataset::create_nmo_tab(){



}
void add_dataset::delete_nmo_tab(){

}
void add_dataset::create_sep(){
  QString filename = QFileDialog::getOpenFileName(this );
  if ( !filename.isNull() ){
    std::vector<QString> coms;
    coms.push_back("-1");
    coms.push_back("data");
    coms.push_back("new");
        coms.push_back("SEP");

    coms.push_back(filename);
    if(storeSEP->what_checked()==0) coms.push_back("BYTE");
    else coms.push_back("FLOAT");
    emit actionDetected(coms);
   
  }
}
void add_dataset::create_flatten(){

    std::vector<QString> coms;
    coms.push_back("-1");
    coms.push_back("data");
    coms.push_back("new");
    coms.push_back("flatten");
    std::vector<QString> x=my_c->return_cvec();
    coms.push_back(x[colorFlatten->get_index(0)]);
        emit actionDetected(coms);

}
void add_dataset::create_seispak(){
  QString filename = QFileDialog::getOpenFileName(this );
  if ( !filename.isNull() ){
    std::vector<QString> coms;
    coms.push_back("-1");
    coms.push_back("data");
    coms.push_back("new");
        coms.push_back("SEISPAK");

    coms.push_back(filename);
    if(storeSEIS->what_checked()==0) coms.push_back("BYTE");
    else coms.push_back("FLOAT");
        if(esizeSEIS->what_checked()==0) coms.push_back("SHORT");
    else coms.push_back("FLOAT");
    emit actionDetected(coms);
   
  }
}
void data_sub_panel::update_surface(){
  std::vector<QString> coms;
    coms.push_back(QString::number(idat));
    coms.push_back("data");
    coms.push_back("surface");
    coms.push_back("update");
    emit actionDetected(coms);

}
void add_dataset::create_surface(){
  std::vector<QString> coms;
    coms.push_back("-1");
    coms.push_back("data");
    coms.push_back("new");
    coms.push_back("surface");
    coms.push_back(QString::number(dataSurface->get_index(0)));
    std::vector<QString> x=my_c->return_cvec();
    coms.push_back(x[colorSurface->get_index(0)]);
    if(typeSURF->what_checked()==0) coms.push_back("amplitude");
    else coms.push_back("single");
    emit actionDetected(coms);

}

void add_dataset::create_semblance_tab(){

}
void add_dataset::delete_semblance_tab(){


}
void add_dataset::create_sep_tab(){
  sepTab=new QWidget();
  sepLay=new QHBoxLayout();
  storeSEP=new basicRadioBox("Storage","Byte","Store internally as a float file");
  storeSEP->addButton("Float","Store internally as a float file");
  loadSEP=new basicButtonBox("","Load","Load SEP file",false);
  storeSEP->set_checked(0);
     connect(loadSEP->my_radios[0], SIGNAL(clicked()), this, SLOT(create_sep()));

  sepLay->addWidget(storeSEP->group());
  sepLay->addWidget(loadSEP->group());
  sepTab->setLayout(sepLay)	;
  addTab(sepTab,"SEP");
  sepTab->setMaximumHeight(90);
    
}
void add_dataset::delete_sep_tab(){
  delete storeSEP;
  delete loadSEP;

  delete sepTab;
  delete sepLay;
}
void add_dataset::create_seispak_tab(){
  seispakTab=new QWidget();
  seispakLay=new QHBoxLayout();
  esizeSEIS=new basicRadioBox("esize","2","Short File");
  esizeSEIS->addButton("4","Float file");
  loadSEIS=new basicButtonBox("","Load","Load SEISPAK file",false);
    storeSEIS=new basicRadioBox("Storage","Byte","Store internally as a float file");
  storeSEIS->addButton("Float","Store internally as a float file");
  esizeSEIS->set_checked(1);
  storeSEIS->set_checked(1);
   connect(loadSEIS->my_radios[0], SIGNAL(clicked()), this, SLOT(create_seispak()));
  seispakLay->addWidget(storeSEIS->group());
  seispakLay->addWidget(esizeSEIS->group());
  seispakLay->addWidget(loadSEIS->group());
  seispakLay->addWidget(esizeSEIS->group());
  seispakLay->addWidget(loadSEIS->group());
  seispakTab->setLayout(seispakLay)	;
  addTab(seispakTab,"SEISPAK");
  seispakTab->setMaximumHeight(90);
    
}
void add_dataset::delete_seispak_tab(){
  delete esizeSEIS;
  delete loadSEIS;

  delete seispakTab;
  delete seispakLay;
}
void add_dataset::create_surface_tab(){
  surfaceTab=new QWidget();
  surfaceLay=new QHBoxLayout();
  std::vector<QString> dlist;
  for(int i=0; i< my_dat->return_ndat(); i++) dlist.push_back(my_dat->return_dat(i)->get_name());
  std::vector<QString>  mp=my_c->return_cvec();

  colorSurface= new basicComboBox("Pickset","Pickset to use for surface",mp);
  dataSurface= new basicComboBox("Dataset","Dataset to use for surface",dlist);
  createSurface=new basicButtonBox("","Create","Create surface datasest",false);
    connect(createSurface->my_radios[0], SIGNAL(clicked()), this, SLOT(create_surface()));
  typeSURF =new basicRadioBox("TYPE","amplitude","Make dataset out of amplitudes");
  typeSURF->addButton("single","Make dataset out of single axis value");
  typeSURF->set_checked(1);
  surfaceLay->addWidget(dataSurface->group());
    surfaceLay->addWidget(colorSurface->group());
    surfaceLay->addWidget(typeSURF->group());
  surfaceLay->addWidget(createSurface->group());

  surfaceTab->setLayout(surfaceLay)	;
  addTab(surfaceTab,"Surface");
  surfaceTab->setMaximumHeight(90);
}
void add_dataset::delete_surface_tab(){
  delete createSurface;
  delete colorSurface;
  delete typeSURF;
  delete seispakTab;
  delete seispakLay;
}
void add_dataset::delete_add_dataset(){
  delete_sep_tab();
  delete_seispak_tab();
  delete_surface_tab();
  delete_nmo_tab();
  delete_semblance_tab();
#ifdef CHEVRON
  delete_flattening_tab();
#endif
}
void data_panel::actionRespond(std::vector<QString> coms){
  emit actionDetected(coms);
}
void data_panel::update_menu(std::vector<QString> coms){
int idat=coms[0].toInt();
if(coms[1].contains("data")){
    if(coms[2].contains("new")){
      int i=my_dat->return_ndat()-1;
      QString tit=my_dat->return_dat(i)->get_title_short();
      my_tabs[tit]=new data_sub_panel(my_pos,my_wind,my_pan,my_dat,my_pick,my_slices,my_maps,i);
      connect(my_tabs[tit], SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));
      insertTab(i,my_tabs[tit],tit);
    idat=i;

    }
  }
 else if(coms[1].contains("clip")){
   if(idat!=-1){
    QString tg=my_dat->return_dat(idat)->get_title_short();
    ((data_sub_panel*)my_tabs[tg])->update_menu(coms);
  }
 }
 else{
   for(std::map<QString,QWidget*>::iterator i=my_tabs.begin(); i!=my_tabs.end(); i++) {
     if(!i->first.contains("add"))
       ((data_sub_panel*)i->second)->update_menu(coms);
  }
}
}
void data_sub_panel::update_menu(std::vector<QString> coms){

  if(coms[1].contains("clip")) {
    if(coms[2].contains("set_histo")){
      clip->set_histo(coms[3]);
    }
     else   if(coms[2].contains("set_pts")){
      clip->set_pts(coms[3]);
    }
  }
  
     char temp_ch[1024];
     sprintf(temp_ch,"%f",my_dat->return_dat(idat)->get_value(my_pan->get_panel(0)->return_orient()));
     val->setText(1,tr(temp_ch));
 
   
    std::vector<QString> h=my_dat->return_dat(idat)->get_header_vals(my_pan->get_panel(0)->return_orient());
    for(int ih=0; ih< (int)h.size(); ih++){
      headers[ih]->setText(1,h[ih]);
    }
#ifdef CHEVRON

 QString dat_type=my_dat->return_dat(idat)->get_type();
  if(dat_type.contains("FLATTEN")){
    int myv=(int)my_dat->return_dat(idat)->get_value(my_pan->get_panel(0)->return_orient());
    flatChoose->set_checked(flat->button_from_value(myv));
  }
#endif
}
data_sub_panel:: data_sub_panel(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, 
std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw>pk, 
std::shared_ptr<slice_types>c, std::shared_ptr<maps>mym,int id){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  idat=id;
#ifdef CHEVRON
flat=new flatten_types();
#endif
  clipWidget=create_clip_window();
   infoWidget=create_info_window();

  addTab(clipWidget,"Clip");
  
  addTab(infoWidget,"Info");
    connect(clip, SIGNAL(actionDetected(std::vector<QString>)), this, SLOT(actionRespond(std::vector<QString>)));

}
QWidget *data_sub_panel::create_clip_window(){
  QWidget *mine= new QWidget();
  boxB=new QGroupBox();
  changeBox=new QHBoxLayout();
  clipBox=new QVBoxLayout();
  std::vector<QString> cc; cc.push_back("black"); cc.push_back("red"); cc.push_back("blue");
  cc.push_back("green"); cc.push_back("white"); cc.push_back("transp");
  begin=new basicComboBox("Beg. clip","Set the begining clip color",cc);
  end=new basicComboBox("End clip","Set the ending clip color",cc);
  symetric=new basicRadioBox("Symmetric","Yes","Make the clip symmetric");
  symetric->addButton("No","Make the clip unsymmetric");
  symetric->set_checked(0);
  begin->set_current(0,1);
  end->set_current(0,1);
  contrast=new basicSliderBox("Contrast","Adjust the contrast of the dataset",0,0,100);
  clear=new basicButtonBox("","Clear","Clear the current clip",false);
  changeBox->addWidget(begin->group());
  changeBox->addWidget(end->group());
  changeBox->addWidget(symetric->group());
  changeBox->addWidget(contrast->group());
  changeBox->addWidget(clear->group());
  boxB->setLayout(changeBox);
  clip=new clip_bar(idat);
  clip->setMinimumSize(300,220);
  
  
  connect(begin->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_beg(const QString&)));
    connect(end->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_end(const QString&)));
    connect(contrast->my_sliders[0], SIGNAL(valueChanged(int)),this, SLOT(change_contrast(int)));
   connect(symetric->my_radios[0], SIGNAL(clicked()), this, SLOT(symetric_yes()));
   connect(symetric->my_radios[1], SIGNAL(clicked()), this, SLOT(symetric_no()));
   connect(clear->my_radios[0], SIGNAL(clicked()), this, SLOT(clip_clear()));

  



/*
connect(frameOrient->my_radios[1], SIGNAL(clicked()), this, SLOT(edge()));
     connect(transpAxes->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(transpose(const QString&)));
    connect(angle1->my_edits[0], SIGNAL(textChanged(const QString&)),this, SLOT(angle_str(const QString&)));
    	connect(angle2->my_sliders[0], SIGNAL(valueChanged(int)), this, SLOT(angle_int (int)));

*/
  clipBox->addWidget(clip);
  clipBox->addWidget(boxB);
  mine->setLayout(clipBox);
  return mine;
}
void data_sub_panel::delete_clip_window(){
   delete clip;
   delete begin;
   delete end;
   delete symetric;
   delete clear;
   delete contrast;
   delete boxB;
   delete changeBox;
   delete clipBox;

}
void data_sub_panel::actionRespond(std::vector<QString> coms){
  emit actionDetected(coms);
}
QWidget *data_sub_panel::create_info_window(){
  char temp_ch[128],temp2_ch[129];
  QWidget *mine=new QWidget();
  infoLay=new QHBoxLayout();
  statusView=new QTreeWidget();
 QStringList list;
  list.append("Properties");
  list.append("Values");
  statusView->setHeaderLabels(list);
  std::vector<axis> axes=my_dat->return_dat(idat)->return_io_hyper()->getAxes();
  for(int i=axes.size(); i < 8; i++) axes.push_back(axis(1));
  QStringList mm;
 mm.append("Axes"); mm.append(" ");
  dataInfo=new QTreeWidgetItem*[2];
  dataInfo[0]=new QTreeWidgetItem(statusView,mm);
  mm.clear(); mm.append("Headers"); mm.append(" ");
  dataInfo[1]=new QTreeWidgetItem(statusView,mm);
  daxes=new QTreeWidgetItem*[8];
  for(int iax=0; iax < 8; iax++){
    QStringList beta;
    sprintf(temp_ch,"%d",iax+1);
	sprintf(temp2_ch,"n=%d o=%f d=%f label=%s",axes[iax].n,axes[iax].o,axes[iax].d,axes[iax].label.c_str());
	 beta.append(tr(temp_ch)); beta.append(tr(temp2_ch));
	 daxes[iax]=new QTreeWidgetItem(dataInfo[0], beta);
  }
  mm.clear();
  mm.append("Value");
  mm.append("0");
  
  
  
     std::vector<QStringList> h=my_dat->return_dat(idat)->get_header(my_pan->get_panel(0)->return_orient());

     headers=new QTreeWidgetItem*[h.size()];

     for(int ih=0; ih < (int)h.size();ih++){
       QStringList x;
       headers[ih]=new QTreeWidgetItem(dataInfo[1],h[ih]);
     }
   
  
  
  val=new QTreeWidgetItem(statusView,mm);
  
  infoLay->addWidget(statusView);
  QString dat_type=my_dat->return_dat(idat)->get_type();
  if(dat_type.contains("SEISPAK")){
     add_seispak_info();
  }
  #ifdef CHEVRON
  else if(dat_type.contains("FLATTEN")){
     add_flatten_info();
  }
  #endif
  else if(dat_type.contains("SURFACE")){
    add_surface_info();
  }
  mine->setLayout(infoLay);
  return mine;
}
void data_sub_panel::add_surface_info(){
   surfUpdate=new basicButtonBox("Surface","UPDATE", "Update the surface values with new picks",false);
   infoLay->addWidget(surfUpdate->group());
    connect(surfUpdate->my_radios[0], SIGNAL(clicked()), this, SLOT(update_surface()));

 
}
#ifdef CHEVRON
flatten_types::flatten_types(){
  s_from_v[3]="Proportional"; b_from_v[3]=0; v_from_b[0]=3; types.push_back("Proportional");
  s_from_v[44]="Base Parallel"; b_from_v[44]=1; v_from_b[1]=44;types.push_back("Base Parallel");
  s_from_v[85]="Top Parallel"; b_from_v[85]=2; v_from_b[2]=85;types.push_back("Top Parallel");
  s_from_v[126]="Preserve"; b_from_v[126]=3; v_from_b[3]=126;types.push_back("Preserve");
  s_from_v[167]="Progradational"; b_from_v[167]=4; v_from_b[4]=167;types.push_back("Progradational");
  s_from_v[208]="Vflat"; b_from_v[208]=5; v_from_b[5]=208;types.push_back("Vflat");
  s_from_v[250]="Best guess"; b_from_v[250]=6; v_from_b[6]=250;types.push_back("Best guess");

}
void data_sub_panel::add_flatten_info(){

   flatBox=new QGroupBox();
   flatLay=new QVBoxLayout();
  // flatCurrent=new basicLineEditBox("Current type",14,"Base Parallel","Type at current location");

  flatChoose=new basicVRadioBox("Type",flat->types[0],"Set the current location to "+flat->types[0]);
 for(int i=1; i <(int) flat->types.size();i++){
     flatChoose->addButton(flat->types[i],"Set the current location to "+flat->types[i]);
   }
     stratSave=new basicButtonBox("","Save","Save the strat collumn",false);

     connect(flatChoose->my_radios[0], SIGNAL(clicked()), this, SLOT(set_strat_prop()));
     connect(flatChoose->my_radios[1], SIGNAL(clicked()), this, SLOT(set_strat_base()));
     connect(flatChoose->my_radios[2], SIGNAL(clicked()), this, SLOT(set_strat_top()));
     connect(flatChoose->my_radios[3], SIGNAL(clicked()), this, SLOT(set_strat_pres()));
     connect(flatChoose->my_radios[4], SIGNAL(clicked()), this, SLOT(set_strat_prog()));
     connect(flatChoose->my_radios[5], SIGNAL(clicked()), this, SLOT(set_strat_vflat()));
     connect(flatChoose->my_radios[6], SIGNAL(clicked()), this, SLOT(set_strat_best()));

     connect(stratSave->my_radios[0], SIGNAL(clicked()), this, SLOT(save_strat()));
  // flatLay->addWidget(flatCurrent->group());
   flatLay->addWidget(flatChoose->group());
 //      flatCurrent->set_value(0,"Preserve");
   flatLay->addWidget(stratSave->group());
    flatChoose->set_checked(3);
   flatBox->setLayout(flatLay);
   infoLay->addWidget(flatBox);
   
}
#endif
void data_sub_panel::add_seispak_info(){


  std::vector<QString> keys=my_dat->return_dat(idat)->return_header_keys();
  keys.insert(keys.begin(),"grid");
  extraInfoBox=new QGroupBox("SEISPAK");
  extraL=new QVBoxLayout();
  for(int i=1; i < 8; i++){
    seispak.push_back(new basicComboBox("AXIS "+QString::number(i+1),"Set header key to display as axis",keys));
  }
  connect(seispak[0]->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_axis2(const QString&)));
  connect(seispak[1]->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_axis3(const QString&)));
  connect(seispak[2]->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_axis4(const QString&)));
  connect(seispak[3]->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_axis5(const QString&)));
  connect(seispak[4]->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_axis6(const QString&)));
  connect(seispak[5]->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_axis7(const QString&)));
  connect(seispak[6]->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(change_axis8(const QString&)));

  for(int i=1; i < 8; i++){
    axis a=my_dat->return_dat(idat)->get_io_axis(i+1);
    if(a.n>1){
      extraL->addWidget(seispak[i-1]->group());
    }
  }
  extraInfoBox->setLayout(extraL);
  infoLay->addWidget(extraInfoBox);

}
void data_sub_panel::delete_info_window(){
    QString dat_type=my_dat->return_dat(idat)->get_type();
  if(dat_type.contains("SEISPAK")){
    delete extraInfoBox;
    delete extraL;
    for(int i=0; i < 7; i++) delete seispak[i];
    delete extraInfoBox;
        QGroupBox *extraInfoBox;
    QVBoxLayout *extraL;
  }

  delete infoLay;
  delete dataInfo[0];
  delete dataInfo[1];
  delete dataInfo;
  for(int i=0; i < 8; i++) delete daxes[i];
  delete daxes;
  delete statusView;
  delete val;


}
void data_sub_panel::save_strat(){
   QString filename = QFileDialog::getSaveFileName(this );
    if ( !filename.isNull() ){
      std::vector<QString> coms;
      coms.push_back(QString::number(idat));
      coms.push_back("data");
      coms.push_back("strat");
      coms.push_back("save");
      coms.push_back(filename);
      emit actionDetected(coms);
   }
}
void data_sub_panel::set_strat(int i){
   std::vector<QString> coms;
  coms.push_back(QString::number(idat));
   coms.push_back("data");
      coms.push_back("strat");

   coms.push_back("set");
#ifdef CHEVRON
   coms.push_back(QString::number(flat->value_from_button(i)));
#endif
   emit actionDetected(coms);

}
void data_sub_panel::change_beg(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("beg_clip");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_axis2(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("axis");
  coms.push_back("2");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_axis3(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("axis");
  coms.push_back("3");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_axis4(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("axis");
  coms.push_back("4");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_axis5(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("axis");
  coms.push_back("5");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_axis6(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("axis");
  coms.push_back("6");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_axis7(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("axis");
  coms.push_back("7");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_axis8(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("axis");
  coms.push_back("8");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_end(const QString txt){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("end_clip");
  coms.push_back(txt);
  emit actionDetected(coms);
}
void data_sub_panel::change_contrast(int contrast){

    float x[4],y[4];
  float pc=(float)contrast/400.;
  x[0]=0; y[0]=0.031;
  x[3]=1.; y[3]=1.-y[0];
  x[1]=.25+pc; y[1]=.25-pc;
  x[2]=1.-x[1]; y[2]=1.-y[1];
  float *second=second_deriv(4,x,y);
  float vals[256];
  for(int i=0; i < 256; i++){
     float px=(float)i/255.;
     int klo,khi;
     if(px<x[1]){klo=0; khi=1;}
     else if(px<x[2]) {klo=1 ;khi=2;}
     else{ klo=2; khi=3;}
   
     float h=x[khi]-x[klo];
     float a=(x[khi]-px)/h;
     float b=(px-x[klo])/h;
     vals[i]=a*y[klo]+b*y[khi]+((a*a*a-a)*second[klo]+
       (b*b*b-b)*second[khi])*(h*h)/6.;
     if(vals[i] < .03) vals[i]=.031;
     if(vals[i]>.97) vals[i]=.969;
  }
  clip->set_pcts(vals);
  delete [] second;
}
void data_sub_panel::symetric_yes(){
  clip->symmetric=true;

}
void data_sub_panel::symetric_no(){
  clip->symmetric=false;
}
void data_sub_panel::clip_clear(){
  std::vector<QString> coms;
  coms.push_back(QString::number(idat));
  coms.push_back("data");
  coms.push_back("clear");
  emit actionDetected(coms);
}
/* Conversion from Numerical recipes*/
float *data_sub_panel::second_deriv(int npts, float *x, float *y){
  float *second=new float[npts];
  float  *u=new float[npts];

  second[0]=0; u[0]=0;
  for(int i=1; i < npts-1; i++){
    float sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
    float p=sig*second[i-1]+2;
    second[i]=(sig-1.)/p;
    u[i]=(6.*((y[i+1]-y[i])/(x[i+1]-x[i])-(y[i]-y[i-1])/
       (x[i]-x[i-1]))/(x[i+1]-x[i-1])-sig*u[i-1])/p;
  }
  second[npts-1]=0.;
  for(int i=npts-2; i >0 ;i--){
    second[i]=second[i]*second[i+1]+u[i];
  }
  delete [] u;
  return second;
}
