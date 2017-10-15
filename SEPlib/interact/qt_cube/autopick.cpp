#include "autopick.h"
#include "auto_viteribi.h"
#include "auto_brown.h"
#include "lloyd_1d.h"
#include "seplib.h"

autopick::autopick(pick_draw *_pks){
  ncor=20;
  nsearch=20;
  correlate=true;
  nail=1000.;
  cur_auto=2;
  accept=1;
  pks=_pks;
  pen_o=-3.;
  pen_e=3;
  error=0.;
  dip=0;
  auto_2d=0;
  method="viterbi";
  plane="left";
}
void autopick::create_viteribi(){
 if(auto_2d==0) delete auto_2d;
 auto_2d=new auto_viteribi(pen_o,pen_e,nail);
}
void autopick::create_brown(){
  if(auto_2d==0) delete auto_2d;
  auto_2d=new auto_brown(pen_o,pen_e,nail);

}
void autopick::init_method(){
  if(method.contains("iter")) create_viteribi();
  if(method.contains("Brown")) create_brown();
}
int autopick::extend_picks(view *myv, orient_cube *pos){
  cur_auto+=1;
  init_method();
  int iax1,iax2,iax3;
  int order[8];
  int isingle=pks->get_single();
  if(pos->get_rotate()) return 0;


  pos->get_orders(order);
  if(plane.contains("top")){
    iax1=order[1];
    iax2=order[2];
    iax3=order[0];
  }
  else if(plane.contains("left")){
   iax1=order[0];
   iax2=order[2];
   iax3=order[1];
  }
  else{
    iax1=order[0];
    iax2=order[1];
    iax3=order[2];
  }

  if(iax1!=isingle && iax2!=isingle) return 0;
  int isort=0;
  if(iax1 == isingle) { isort=iax2;}
  else if(iax2==isingle) { isort=iax1;}
  float fpos[8],os[8],ds[8];
  for(int i=0; i< 8; i++) { fpos[i]=pos->get_pos(i);}
  int ibeg=pos->get_beg(iax3);
  int iend=pos->get_end(iax3);
  pos->get_os(os);
  pos->get_ds(ds);
  int iloc[8];
  pos->get_locs(iloc);
  for(int i3=ibeg; i3 < iend; i3++){
    
    fpos[iax3]=os[iax3]+ds[iax3]*i3;
    iloc[iax3]=i3;
    
    QString col=pks->get_active_col();
    
    pairs_new *myp=pks->get_pts_sort_le(pos,iax1,iax2,accept,isort,col);
    
    
    pick_line(myv,fpos,pos,iax1,iax2,isort,isingle,myp,iax3);
    delete myp;
  }
  cur_auto+=1;
  return 0;
}
int  autopick::pick_2d(view *myv,orient_cube *pos){

  init_method();

    QString col=pks->get_active_col();

  for(int islc=0; islc < (int)myv->slices.size(); islc++){
    int iax1=myv->slices[islc]->get_iax1();
    int iax2=myv->slices[islc]->get_iax2();
    int isingle=pks->get_single();
    

    if(iax1==isingle || iax2==isingle){
      int isort=0;
      if(iax1 == isingle) { isort=iax2;}
      else if(iax2==isingle) { isort=iax1;}
      float fpos[8];
      for(int i=0; i< 8; i++) { fpos[i]=pos->get_pos(i);}
          fprintf(stderr,"the sort is set to %d %d\n",isort,isingle);

      pairs_new *myp=pks->get_pts_sort_le(pos,myv->slices[islc]->get_iax1(),
        myv->slices[islc]->get_iax2(),accept,isort,col);
      
      pick_line(myv,fpos,pos,iax1,iax2,isort,isingle,myp,-1);
      delete myp;
    }
  }
  cur_auto+=1;
  return cur_auto;
}
  //WORK ON THIS
 void autopick::decline_picks(){
  cur_auto--;
      QString col=pks->get_active_col();

  pks->delete_pick_vals(cur_auto,col);
 }
/*ARGSUSED*/
 void autopick::pick_line(view *myv, float *fpos, orient_cube *pos,int iax1, int iax2, int isort,int isingle,pairs_new *myp, int iax3){
(void) iax3; (void) fpos; // quiet unused argument complaints
    QString col=pks->get_active_col();

        if(myp->size()>1){
          int il[8];
          pos->get_locs(il); il[iax1]=il[iax2]=-1;
          pks->clear_picks(pos,il,col);
         axis a=pos->get_orient_axis(isort),b=pos->get_orient_axis(isingle);
         float osort=a.o, dsort=a.d;
         float osingle=b.o,dsingle=b.d;
         osort+=pos->get_beg(isort)*dsort;
         osingle+=pos->get_beg(isingle)*dsingle;
         std::vector<int> loc_p;
         for(int i=0; i < (int)myp->size();i++) {
           loc_p.push_back(myp->vals[i].y);
      fprintf(stderr,"pushing value %d %d\n",myp->vals[i].x,myp->vals[i].y);
      }
         int f=loc_p[0],n=loc_p[loc_p.size()-1]-f+1;
         float_1d *line=new float_1d(n);
         myp->build_int_line(f,n,line->vals);
         float_2d *sim;

         if(correlate) {

           float_2d *ar=extract_dat(pos, myv->cur_dat,isingle, isort,line,
           nsearch+ncor,f);

           std::vector<int> close;
           for(int i=0;i<(int)myp->size(); i++){ 
             close.push_back(myp->vals[i].y-f);
           }

           sim=correlate_it(ar,ncor,close);

           delete ar;
         }
         else{
         sim=extract_dat(pos,myv->cur_dat,isingle,isort,line,nsearch,f);
      }
      //Normalize it to max 1

      sim->normalize(1.);
      std::vector<int> locs;
      for(int i=0; i<(int)myp->size(); i++){
        locs.push_back(myp->vals[i].y-f);
      }
      std::vector<path> mypath=auto_2d->return_path(locs,sim);
      locs.clear();
      delete sim;


 
       std::vector<float> tr;
       std::vector<int> iloc;
      if(error>.01){

        for(int i=0; i < (int)mypath.size(); i++) tr.push_back(mypath[i].ix*1.);
        int nlenc=mypath.size()/3;
        if(nlenc > 40) nlenc=40;
        lloyd_1d_fit *fit=new lloyd_1d_fit(tr,nlenc,error/100.);
        for(int iter=0; iter < 35; iter++)  {
          fit->do_iter(iter);
        }
        for(int i=0; i <(int) fit->center.size(); i++) iloc.push_back((int)fit->center[i]);
         delete fit;
      }
      else{

        for(int i=0; i< (int)mypath.size();i++) iloc.push_back(i);
      }

      
      for(int i=0; i <(int)iloc.size(); i++){
        bool newp=true;
        for(int j=0; j < (int) loc_p.size(); j++) if(loc_p[j]==f+iloc[i]) newp=false;
         fprintf(stderr,"CHECK ME %d %d %d \n",mypath[iloc[i]].ix,nsearch,ncor);
          il[isingle]=mypath[iloc[i]].ix-nsearch+(int) (line->vals[iloc[i]]);
          il[isort]=f+iloc[i];
          fprintf(stderr,"adding point %d %d %d \n",il[0],il[1],il[2]);
           if(newp) pks->add_pt(il,cur_auto);
           else pks->add_pt(il,1);
      //  }
      }

      delete line; 
    
    }
 

}
int  autopick::flat_view(view *myv,orient_cube *pos){
  cur_auto+=1;
  if(myv==0 || pos==0){}

  return 0;
}



float_2d *autopick::correlate_it(float_2d *ar, int ncor, std::vector<int> close){

  axis ax[2],ain1,ain2;;
  ain1=ar->get_axis(1); ax[1]=ain2=ar->get_axis(2);
  ax[0]=ain1; ax[0].n=ax[0].n-2*ncor;
  float_2d *out=new float_2d(ax[0],ax[1]);
  
  //int center=ax[0].n/2;
  float master[ain1.n];
 

  int iout=0,cold=ain1.n/2,imaster,ishift;
  float f;
  int iless=0,iloc=0;
  for(int i2=0; i2 < ain2.n; i2++){
    //construct master
    for(int i=0; i < (int)close.size(); i++){
      if(close[i] <= i2) {iless=close[i]; iloc=i;}
    }
    if(i2==iless || iloc==(int)close.size()-1){ //Right on pick
       for(int i=0; i< ain1.n; i++) master[i]=ar->vals[i+ain1.n*close[iloc]];
      
    }
    else{
      f=((float)(i2-iless)/((float)(close[iloc+1]-close[iloc])));
      for(int i=0; i< ain1.n; i++){
        master[i]=ar->vals[i+ain1.n*close[iloc]]*(1.-f)+f*ar->vals[i+ain1.n*close[iloc+1]];
      }
    }

    for(int i1=0; i1 < ax[0].n; i1++){
      iout=i1+i2*ax[0].n;  out->vals[iout]=0.;
      imaster=cold-ncor;
      ishift=i1+i2*ain1.n;     
      for(int i=0; i< ncor; i++){
        out->vals[iout]+=master[i+imaster]*ar->vals[ishift+i];
        }
    } 
  }
  return out;
}


float_2d *autopick::extract_dat(orient_cube *pos,dataset *cur_dat,int isingle, 
int isort,float_1d *line, int ns,int f2){
  int n1=1,n2=1;
 unsigned char *buf;
 float_2d *dat;

    n1=cur_dat->get_grid_axis(isingle+1).n;
    n2=cur_dat->get_grid_axis(isort+1).n;  
    
    int b1,b2,e1,e2;
    b1=pos->get_beg(isingle); b2=pos->get_beg(isort);
    e1=pos->get_end(isingle); e2=pos->get_end(isort);
    buf=cur_dat->get_char_data(pos,isingle,0,n1,isort,0,n2);
    axis axes[2];
    axes[0]=axis(ns*2+1);
    axes[1]=axis(line->get_axis(1).n);
    dat=new float_2d(axes[0],axes[1]);
    int i;
    float f;
    int ishift;


   for(int i2=0; i2 <  axes[1].n; i2++){
      f=line->vals[i2]+b1*0*e1;
      ishift=(int)f; f=f-ishift;
       
      for(int i1=0; i1 < ns*2+1; i1++){
        i=ishift-ns+i1;
        if(i < 0) dat->vals[i1+i2*(2*ns+1)]=0.;
        else if(i> n1-2) dat->vals[i1+i2*(ns*2+1)]=0.;
        else dat->vals[i1+i2*(ns*2+1)]= (1.-f)*buf[i+(i2+b2+0*e2+f2 )*n1]+
          f*buf[i+1+(i2+b2+0*e2+f2 )*n1]-128;
      }
    }
  delete [] buf;
  return dat;
}
