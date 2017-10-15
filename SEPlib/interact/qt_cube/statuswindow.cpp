#include "statuswindow.h"
#include "my_colors.h"
#include <qlayout.h>
#include <QVBoxLayout>
StatusWindow::StatusWindow( position *p,datasets *d, int nv, std::vector<DrawWindow *>ws, QWidget *parent, const char *name )
        : QWidget( parent, name ){
  pos=p;
  datas=d;
  nviews=nv;
  windows=ws;
  QVBoxLayout *box1 = new QVBoxLayout;
    clipMain=new QTabWidget(this);

  create_info();
  create_history();
  box1->addWidget(statusView);
  
  box1->addWidget(historyView);




#ifdef CHEVRON
  layergroup=new QGroupBox(tr("Layer"));
  convert_s_n["Proportional"]=PROPORTIONAL;
  convert_s_n["Base Parallel"]=BASE_PARALLEL;
  convert_s_n["Top Parallel"]=TOP_PARALLEL;
  convert_s_n["Progradtional"]=PROGRADATIONAL;
  convert_s_n["Vflat"]=VFLAT;
  convert_s_n["Best guess"]=BEST_GUESS;
  int i=0;
  for(std::map<QString,int>::iterator conv=convert_s_n.begin(); conv!=convert_s_n.end(); conv++){
    convert_n_n[conv->second]=i;
    convert_s_s[conv->first]=QString::number(conv->second);
    i++;
  }
  layoutlayer=new QVBoxLayout;
  layerbox=new QComboBox();
  
  for(std::map<QString,int>::iterator p=convert_s_n.begin(); p!=convert_s_n.end(); p++)
	layerbox->insertItem(p->first);
	
  layerbox->setMinimumSize(QSize(40,20));
   QToolTip::add(layergroup, tr("Set the layer type"));
	connect(layerbox, SIGNAL(activated(const QString&)), this, SLOT(layerBoxActivated(const QString&)));
	
	layoutlayer->setContentsMargins(2,2,2,2);
//	layoutlayer->addWidget(layerbox);
//	layergroup->setLayout(layoutlayer);
 // box1->addWidget(layergroup);
  
  
  
  colorgroup=new QGroupBox(tr("Color"));

 // layoutcolor=new QVBoxLayout;
  colorbox=new QComboBox();
  
  my_colors my=my_colors();
  std::vector<QString> col=my.return_cvec();
  
  for(int i=0; i < (int) col.size(); i++) colorbox->insertItem(col[i]);
	
  colorbox->setMinimumSize(QSize(40,20));
   QToolTip::add(colorgroup, tr("Set the boundary color"));
	connect(colorbox, SIGNAL(activated(const QString&)), this, SLOT(colorBoxActivated(const QString&)));
	
	//layoutcolor->setContentsMargins(2,2,2,2);
	
	
		layoutlayer->addWidget(layerbox);
	layergroup->setLayout(layoutlayer);
layoutlayer->addWidget(colorbox);

	  box1->addWidget(layergroup);
//	colorgroup->setLayout(layoutlayer);
//  box1->addWidget(colorgroup);
  
  
  
#endif


  this->setLayout(box1);
}
 
 
void StatusWindow::create_history(){

 historyView = new QListWidget(this);
 for(int i=0; i< 20; i++){
   historyItem[i]=new QListWidgetItem(tr(""),historyView);
 }
 
 connect(historyView, SIGNAL(itemClicked(QListWidgetItem * )), this, 
   SLOT(history_sel(QListWidgetItem * )));
}
 
 
void StatusWindow::create_info(){
   statusView = new QTreeWidget(this);
   statusView->setColumnCount(2);
   QStringList list;
   list.append("Properties");
   list.append("Values");
   statusView->setHeaderLabels(list);
/*
	statusView->setShowToolTips(true);
	statusView->setAllColumnsShowFocus(true);
	statusView->setRootIsDecorated(true);
*/

	char temp_ch[15];
	char temp2_ch[100];

 QStringList alpha; alpha.append(tr("Data Sets"));alpha.append(tr(""));
    alpha[0]=tr("Grid axes");
	list_axes= new QTreeWidgetItem(statusView, alpha);
	for(int i=0;i < 8; i++){
	    QStringList beta;
	   sprintf(temp_ch,"Axis %d",i+1);
	   axes[i]=datas->return_dat(0)->get_grid_axis(i+1);
	   sprintf(temp2_ch,"n=%d o=%f d=%f label=%s",axes[i].n,axes[i].
	     o,axes[i].d,axes[i].label.c_str());
	   beta.append(tr(temp_ch)); beta.append(tr(temp2_ch));
	   (void*) new QTreeWidgetItem(list_axes, beta);
	}

  
   




    alpha[0]=tr("Position");
	list_pos= new QTreeWidgetItem(statusView, alpha);
	for(int i=0; i < 8; i++){
	  
	  sprintf(temp_ch,"Axis %d",i+1);
	  sprintf(temp2_ch,"%d (%f)",this->pos->get_loc(i),
	    this->pos->get_loc(i)*axes[i].d+axes[i].o);
	  QStringList beta; beta.append(tr(temp_ch)); beta.append(tr(temp2_ch));
	  list_poss[i]= new QTreeWidgetItem(list_pos, beta);
	}

    alpha[0]=tr("View");
	list_view= new QTreeWidgetItem(statusView,alpha);
    for(int i=0; i < nviews; i++){
	  sprintf(temp_ch,"%d",i);
	  sprintf(temp2_ch,"%s",datas->return_dat(windows[i]->get_idat())->get_name().ascii());
	  QStringList beta; beta.append(tr(temp_ch)); beta.append(tr(temp2_ch));
	list_views[i]=new QTreeWidgetItem(list_view, beta);
	}
    create_datas();

	
	nhist=0;
}

void StatusWindow::delete_datas(){
  for(int idat=0; idat < ndat; idat++){
    for(int iax=0; iax < 8; iax++){
      delete daxes[idat][iax];
    }
    delete datsets[idat];
    delete [] daxes[idat];
    delete list_vals[idat];
  }
  delete [] daxes;
  delete [] datsets;
  delete list_val;

}
#ifdef CHEVRON
void StatusWindow::layerBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back(0); com.push_back("chev");
  com.push_back(tr("layer")); 
  com.push_back(convert_s_s[text]);
    emit actionDetected(com);


}
void StatusWindow::colorBoxActivated(const QString& text) {
  std::vector <QString> com; com.push_back(0);com.push_back("chev");
  com.push_back(tr("color")); 
  com.push_back(text);
    emit actionDetected(com);

}
#endif

void StatusWindow::create_datas(){
   QStringList alpha; alpha.append(tr("Data Sets"));alpha.append(tr(""));

    datsets= new QTreeWidgetItem*[datas->return_ndat()];
    daxes=new QTreeWidgetItem**[datas->return_ndat()];
    headers=new QTreeWidgetItem**[datas->return_ndat()];
    data_info=new QTreeWidgetItem**[datas->return_ndat()];
    char temp_ch[99],temp2_ch[98];

   for(int i=0; i < datas->return_ndat();i++){
     alpha[0]=datas->return_dat(i)->title_short;
     std::vector<axis> axes=datas->return_dat(i)->return_io_hyper()->return_axes(8);
     datsets[i]=new QTreeWidgetItem(statusView,alpha);
     data_info[i]=new QTreeWidgetItem*[2];  
     QStringList mm;
     mm.append("Axes"); mm.append(" ");
     data_info[i][0]=new QTreeWidgetItem(datsets[i],mm);
     mm.clear();mm.append("Headers"); mm.append(" ");
     data_info[i][1]=new QTreeWidgetItem(datsets[i],mm);
     daxes[i]=new QTreeWidgetItem*[8];
     for(int iax=0; iax < 8; iax++){
     	    QStringList beta;
     
       sprintf(temp_ch,"%d",iax+1);
	   sprintf(temp2_ch,"n=%d o=%f d=%f label=%s",axes[iax].n,axes[iax].o,axes[iax].d,axes[iax].label.c_str());
	    beta.append(tr(temp_ch)); beta.append(tr(temp2_ch));
	   daxes[i][iax]=new QTreeWidgetItem(data_info[i][0], beta);
     }

     std::vector<QStringList> h=datas->return_dat(i)->get_header(windows[0]->pos);

     headers[i]=new QTreeWidgetItem*[h.size()];

     for(int ih=0; ih < (int)h.size();ih++){
       QStringList x;
       headers[i][ih]=new QTreeWidgetItem(data_info[i][1],h[ih]);
     }
     axes.clear();
   }

    alpha[0]=tr("Value");
	list_val = new QTreeWidgetItem(statusView, alpha);
    for(int i=0; i < datas->return_ndat(); i++){
	  	  strcpy(temp_ch,datas->return_dat(i)->title_short.ascii());

	  sprintf(temp2_ch,"0");
	  QStringList beta; beta.append(tr(temp_ch)); beta.append(tr(temp2_ch));
	  list_vals[i]=new QTreeWidgetItem(list_val,beta);
	}

	ndat=datas->return_ndat();

}     	 

void StatusWindow::update_status(std::vector<change> hist){
  char temp_ch[64];
  int p;
  
  if(ndat!=datas->return_ndat()) {
    if(ndat>0) delete_datas();
    create_datas();
  }
  pos_lab="";
  for(int i=0; i < 8; i++){
    p=windows[0]->pos->get_loc(i);
    sprintf(temp_ch,"%d (%f)",p,p*axes[i].d+axes[i].o);
    list_poss[i]->setText(1,tr(temp_ch));
    if(axes[i].n!=1){
      char temp_ch[1024];
      sprintf(temp_ch,"axis%d=%f ",i+1,p*axes[i].d+axes[i].o);
      pos_lab+=temp_ch;
    }
  }
  val_lab="";
  for(int i=0; i<datas->return_ndat(); i++){
     sprintf(temp_ch,"%f",datas->return_dat(i)->get_value(
       windows[0]->pos));
     list_vals[i]->setText(1,tr(temp_ch));
     char temp_ch[1024];
     sprintf(temp_ch,"%s:%f ",datas->return_dat(i)->title_short.ascii(),
       datas->return_dat(i)->get_value( windows[0]->pos));
#ifdef CHEVRON
      if(datas->return_dat(i)->title_short.contains("V_COL")){
         int iv=(int)(0.5+datas->return_dat(i)->get_value( windows[0]->pos));
         layerbox->setCurrentIndex(convert_n_n[iv]);
      }
#endif
     val_lab+=temp_ch;
  
   
    std::vector<QString> h=datas->return_dat(i)->get_header_vals(windows[0]->pos);
    for(int ih=0; ih< (int)h.size(); ih++){
      headers[i][ih]->setText(1,h[ih]);
    }
  
  }
  
  
  for(int i=0; i < nviews;i++){
   int orders[8];
   windows[i]->pos->get_orders(orders);
   char temp4_ch[30];
     sprintf(temp4_ch,"%d,%d,%d,%d,%d,%d,%d,%d",orders[0]+1,orders[1]+1,orders[2]+1,orders[3]+1,
    orders[4]+1,orders[5]+1,orders[6]+1,orders[7]+1);
	sprintf(temp_ch,"%s order=%s",datas->return_dat(windows[i]->get_idat())->title_short.ascii(),temp4_ch);
    list_views[i]->setText(1,tr(temp_ch));
  }
  for(int i=0; i < 20; i++) delete historyItem[i];
  int nsz=hist.size();
  if(nsz >20) nsz=20;
   nhist=nsz;
  for(int j=0; j < nsz; j++) {
   historyItem[j]=new  QListWidgetItem(hist[hist.size()-1-j].output_for(),historyView);
  }
  for(int j=nsz; j < 20; j++) historyItem[j]=new QListWidgetItem(tr(""),historyView);
}
void StatusWindow::history_sel(QListWidgetItem * item){
  std::vector<QString>  com;
  int dd=20;
  if(nhist < 20) dd=nhist;
  bool found=false;
  for(int index=0; index <dd; index++){
    if(historyItem[index]==item){
      com.push_back("-1"); 
      com.push_back("history");
      com.push_back(QString::number(index));
      found=true;
    }
    
  }
   for(int j=0; j < 20; j++) historyView->setItemSelected(historyItem[j],false);
    if(found) emit actionDetected(com);
}




