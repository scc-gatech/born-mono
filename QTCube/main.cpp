#include "centerwindow.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qsplitter.h>
#include <qsettings.h>
#include <qstatusbar.h>
#include <fileIO.h>
#include "drawwindow.h"
//#include "pick_groups.h"
#include "surface_data.h"
#include "incore_data.h"
#include "partial_data.h"
#include "created_data.h"
#include "outcore_data.h"
#include "util.h"
#include "nmoed.h"
#include "nmo_semblance.h"
#include "part_semb_data.h"
#include "pick_draw.h"
#include<ioModes.h>
using namespace SEP;
int main(int argc, char** argv) {
	
  std::shared_ptr<ioModes> modes(new ioModes(argc,argv));

  std::shared_ptr<genericIO> defaultIO=modes->getDefaultIO();
  std::string defaultType=modes->getDefaultType();


  QApplication app(argc, argv);

  std::shared_ptr<paramObj> pars(defaultIO->getParamObj());

  std::shared_ptr<pick_draw> pk(new pick_draw());
  std::shared_ptr<datasets> datas(new datasets(pars,pk));
  QString red="red", blue="blue";
  /*pars if we are doing NMO*/
  int it_axis=pars->getInt("it_axis",0);
  int ioff_axis=pars->getInt("off_axis",1);
  int imes_axis=pars->getInt("mes_axis",5);
  int imov=pars->getInt("moveout_data",0);
  std::shared_ptr<hypercube> grid;
  for(int i=0; i < pars->getInt("ndata"); i++){
    std::string type=pars->getString(std::string("type")+std::to_string(i),"FILE");
    std::string name=pars->getString(std::string("data")+std::to_string(i));
    std::string storage=pars->getString(std::string("storage")+std::to_string(i),"IN_BYTE");
    std::string title=pars->getString(std::string("title")+std::to_string(i),name);
    std::shared_ptr<io_func> iof;
    QString nameq=QString(name.c_str());
    if(i==0 && type!=std::string("FILE")) pars->error("First type specified must be a file");
    if(type==std::string("FILE")){
      std::string fileType=pars->getString(std::string("fileType")+std::to_string(i+1),defaultType);
        std::shared_ptr<fileIO> fileI(new fileIO(name,modes,fileType));
        iof=fileI;
        if(i==0)  grid=iof->return_hyper();
    }
    else if(type==std::string("SEMBLANCE")){
        std::shared_ptr<nmo_semblance> se(new nmo_semblance(grid,datas->return_dat(imov),it_axis,ioff_axis,5,pars,pk,red,blue));
        iof=se;
    }
     else if(type== std::string("NMOED")){
        std::shared_ptr<nmoed>  nmo(new nmoed(grid,datas->return_dat(imov),it_axis,ioff_axis,5,pars,pk,red,blue));
        iof=nmo;
   }
   else pars->error(std::string("Unknown storage type :")+type);
    

  
    
   if(storage==std::string("IN_FLOAT")){
        std::shared_ptr<incore_data_float> icf(new incore_data_float(title,nameq,grid,iof,pars,i,1));
        datas->add_dat(icf);
   }
   else if(storage==std::string("IN_BYTE")){
        std::shared_ptr<incore_data_byte> icb(new incore_data_byte(title,nameq,grid,iof,pars,i,1));
        datas->add_dat(icb);
   }
   else if(storage== std::string("PART_FLOAT")){
        std::shared_ptr<partial_data_float> icb(new partial_data_float(title,nameq,grid,iof,pars,i,1));
        datas->add_dat(icb);
  }
   else if(storage==std::string("PART_BYTE")){
        std::shared_ptr<partial_data_byte> icb(new partial_data_byte(title,nameq,grid,iof,pars,i,1));
        datas->add_dat(icb);
 }
   else if(storage== std::string("OUT_FLOAT")){
        std::shared_ptr<outcore_data_float> icb(new outcore_data_float(title,nameq,grid,iof,pars,i,1));
        datas->add_dat(icb);
     }
   else if(storage==std::string("OUT_BYTE")){
        std::shared_ptr<outcore_data_byte> icb(new outcore_data_byte(title,nameq,grid,iof,pars,i,1));
        datas->add_dat(icb);
    }
   else if(storage== std::string("CREATED_FLOAT")){
        std::shared_ptr<created_data_float> icb(new created_data_float(title,nameq,grid,iof,pars,i,1));
        datas->add_dat(icb);
  }
   else if(storage== std::string("CREATED_BYTE")){
        std::shared_ptr<created_data_byte> icb(new created_data_byte(title,nameq,grid,iof,pars,i,1));
        datas->add_dat(icb);
 }
   else
        pars->error("unknown storage type");
    
  }
  int surf=pars->getInt("do_surface",0);
  if(surf==1){
     std::shared_ptr<surface_data> sd(new surface_data(grid,"Surface","single",datas->return_dat(0),
       pars,pk,"red",pars->getInt("ndata")));
  }
  std::shared_ptr<slice_types> ct(new slice_types());

  MainWindow *window=new MainWindow(defaultIO,datas,pk,ct);
  window->show();
  QString name=pars->getString("run_history","NONE").c_str();
  if(name!="NONE")  window->my_hist->load_history(name); 
  app.setWindowIcon(QIcon(QPixmap(":/images/qt_cube.icns")));
  return app.exec();

}



