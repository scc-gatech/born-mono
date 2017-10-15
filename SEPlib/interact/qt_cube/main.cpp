#include <qlayout.h>
#include <q3listview.h>
#include <qlabel.h>
#include <qapplication.h>
#include <qsplitter.h>
#include <qsettings.h>
#include <qstatusbar.h>
#ifdef CHEVRON
#include "chev_strat_data.h"
#endif
#include "centerwindow.h"
#include "drawwindow.h"
//#include "pick_groups.h"
#include "surface_data.h"
#include<sep_params.h>
#include "incore_data.h"
#include "partial_data.h"
#include "created_data.h"
#include "outcore_data.h"
#include "sep_reg_io.h"
#include "util.h"
#include "su_reg_io.h"
#include "seispak_reg_io.h"
#include "segy_reg_io.h"
#include "sep_reg_mmap_io.h"
#include "nmoed.h"
#include "nmo_semblance.h"
#include "part_semb_data.h"
#include "pick_draw.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);


	sep_params *pars=new sep_params(argc,argv);
	util *pp=new util(pars);
  //pick_maps *mp=new pick_maps();
  //pick_groups *pks= new pick_groups(mp);
  pick_draw *pk=new pick_draw();
  hypercube *grid=new hypercube(); //Grid for implementation
  datasets *datas=new datasets(pars);
        QString red="red", blue="blue";
  std::string title;
  int mmap=pars->get_int("mmap",0);
 io_func *io=0;
  if(1==pars->get_int("basic_sep_io",1)){
    sreg *d=new sreg("in");
    std::vector<axis>  as=d->return_axes(8);
    grid->set_axes(as);
    int esize=d->get_int("esize",4);
   
    if(esize==4) io=new sep_reg_io_float("in",pp);
    else io=new sep_reg_io_byte("in",pp);
    std::string title=d->get_string("title","stdin");
    datas->add_dat(new incore_data_byte(title,title.c_str(),grid,io,pars,0,1));
  }
  else{ //Called from script
  std::vector<axis> axes;
  char tt[20];
   //First set up the grid
   for(int i=0; i < 8; i++){
     axes.push_back(axis(1));
     sprintf(tt,"ng_%d",i+1); axes[i].n=pars->get_int(tt,1);
     sprintf(tt,"og_%d",i+1); axes[i].o=pars->get_float(tt,0.);
     sprintf(tt,"dg_%d",i+1); axes[i].d=pars->get_float(tt,1.);
     sprintf(tt,"labelg_%d",i+1); axes[i].label=pars->get_string(tt," ");
   }

   grid->set_axes(axes);
   //Now set up the datasets
   std::string type,nm,store;
   for(int idat=0; idat< pars->get_int("ndata"); idat++){

     sprintf(tt,"type%d",idat); type=pars->get_string(tt,"NONE");
     if(0==strcmp(type.c_str(),"NONE")) {
        char errmsg[] = "didn't specify1 %s ";
        seperr(errmsg,tt);
     }
     sprintf(tt,"data%d",idat); nm=pars->get_string(tt,"NONE");
     if(0==strcmp(nm.c_str(),"NONE")) {
        char errmsg[] = "didn't specify2 %s ";
        seperr(errmsg,tt);
     }
     sprintf(tt,"store%d",idat); store=pars->get_string(tt,"NONE");
     int it_axis=pars->get_int("it_axis",0);
     int ioff_axis=pars->get_int("off_axis",1);
     int imes_axis=pars->get_int("mes_axis",5);
     int imov=pars->get_int("moveout_data",0);
     if(0==strcmp(store.c_str(),"NONE")) {
        char errmsg[] = "didn't specify3 %s ";
        seperr(errmsg,tt);
     }
     io_func *io = (io_func *) NULL;


     if(0==strcmp(type.c_str(),"SEP")){
       sreg *d=new sreg(nm);
       int esize=d->get_int("esize",4);

       if(mmap==1 && esize==4){
          io=new sep_reg_mmap_io_float(nm.c_str(),pp);

       }
       else{       

         if(esize==4) {
         
         io=new sep_reg_io_float(nm.c_str(),pp);
         }
         else if(esize==1) io=new sep_reg_io_byte(nm.c_str(),pp);
         std::vector<axis> ju=d->return_axes(8);

         for(int i=0; i < 8; i++) axes[i]=ju[i];

       }
       title=d->get_title();
     }
     else{
       for(int i=0; i < 8; i++){
         sprintf(tt,"n%d_%d",idat,i+1); axes[i].n=pars->get_int(tt,1);
         sprintf(tt,"o%d_%d",idat,i+1); axes[i].o=pars->get_float(tt,0.);
         sprintf(tt,"d%d_%d",idat,i+1); axes[i].d=pars->get_float(tt,1.);
         sprintf(tt,"label%d_%d",idat,i+1); axes[i].label=pars->get_string(tt," ");
      }

      sprintf(tt,"swap%d",idat); int s=pars->get_int(tt,0);
      bool swap=false;
      if(s==1) swap=true;
      hypercube *dd=new hypercube();
      dd->set_axes(axes);
      if(0==strcmp(type.c_str(),"SU"))  io=new su_reg_io_float(nm.c_str(),dd,swap,idat,pp);
      else if(0==strcmp(type.c_str(),"SEGY")) io=new segy_reg_io_float(nm.c_str(),dd,swap,idat,pp);
      else if(0==strcmp(type.c_str(),"SEISPAKF")) 
        io=new seispak_reg_io_float(nm.c_str(),dd,swap,idat,pp);
      else if(0==strcmp(type.c_str(),"SEISPAKS")) 
        io=new seispak_reg_io_short(nm.c_str(),dd,swap,idat,pp);
      else if(0==strcmp(type.c_str(),"NMOED")) 
        io=new nmoed(grid,datas->return_dat(imov),it_axis,ioff_axis,5,pp,pk,red,blue);
 //         pp,mp->groups["red"],mp->groups["blue"]);
       
      else if(0==strcmp(type.c_str(),"SEMBLANCE")) 
        io=new nmo_semblance(grid,datas->return_dat(imov),it_axis,ioff_axis,5,pp,pk,
         red,blue);
   //        mp->groups["red"],mp->groups["blue"]);
      else{
  
      
      
        fprintf(stderr,"type%d=%s \n",idat,type.c_str());
        pars->error("Error in type specification\n");
      }
            title=pars->get_string("title",nm);

     }
     
      if(strcmp(store.c_str(),"IN_FLOAT")==0) {
        datas->add_dat(new incore_data_float(title,nm.c_str(),grid,io,pars,idat,mmap,1));
        }
      else if(strcmp(store.c_str(),"IN_BYTE")==0) 
        datas->add_dat(new incore_data_byte(title,nm.c_str(),grid,io,pars,idat,1));
      else if(strcmp(store.c_str(),"SEMB_BYTE")==0) {
        datas->add_dat(new  part_semb_data_byte(title,nm.c_str(),grid,io,
          it_axis,ioff_axis,imes_axis,pk,red,blue,pars,idat,1));
       }
      else if(strcmp(store.c_str(),"OUT_FLOAT")==0) 
        datas->add_dat(new outcore_data_float(title,nm.c_str(),grid,io,pars,idat,3));
      else if(strcmp(store.c_str(),"OUT_BYTE")==0) 
        datas->add_dat(new outcore_data_byte(title,nm.c_str(),grid,io,pars,idat,3));
      else if(strcmp(store.c_str(),"PART_FLOAT")==0) 
        datas->add_dat(new partial_data_float(title,nm.c_str(),grid,io,pars,idat,1));
      else if(strcmp(store.c_str(),"PART_BYTE")==0) 
        datas->add_dat(new partial_data_byte(title,nm.c_str(),grid,io,pars,idat,1));
      else if(strcmp(store.c_str(),"CREATED_FLOAT")==0) 
        datas->add_dat(new created_data_float(title,nm.c_str(),grid,io,pars,idat,1));
      else if(strcmp(store.c_str(),"CREATED_BYTE")==0) 
        datas->add_dat(new created_data_byte(title,nm.c_str(),grid,io,pars,idat,1));
      else{
        fprintf(stderr,"store%d=%s \n",idat,store.c_str());
        pars->error("Undefined storage\n");
      }
     
	}
}

  int ndat=pars->get_int("ndata");
  int surf=0;
  char fmt[] = "d";
  getch("do_surface",fmt,&surf);
    if(surf==1){
    datas->add_dat(new surface_data(grid,"Surface",datas->return_dat(0),
       pars,pk,"red",ndat));
       
}
#ifdef CHEVRON
int doc=0;
getch("do_chevron","d",&doc);
if(doc==1){
    datas->add_dat(new chev_strat_data(grid,"CHEV_STRAT",pk,"red",ndat+1));
}
#endif
	slice_types *ct=new slice_types();


	CenterWindow* window=new CenterWindow(pars,datas,pk,ct);

	app.setMainWidget(window);
	window->show();
	window->display_it();
    QString name=pars->get_string("run_history","NONE").c_str();
    if(name!="NONE") window->up->load_history(name);

    int x=app.exec();
   
  delete datas;
  delete grid;
  delete pp;
  delete pars;
  fprintf(stderr,"about to exit \n");
  return x;
}
