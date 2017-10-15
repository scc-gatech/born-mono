#include "mouse_func.h"
#include "util.h"

void  mouse_func::do_noth( std::vector<slice*> slices,int islice,int ix, int iy, 
  orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
  if((int)slices.size()==0 && islice==0 && ix==0 && iy==0 && pos==0 && draw_o==0){}
  com->at(1)="none";

}

void mouse_func::del_pt(std::vector <slice*>slices,int islice,int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o,bool move){
    int iloc[8];
(void) pos; // silence unused argument warning
    if(draw_o==0){}
    if(islice<0) {com->at(1)/*=*/="none"; return;}
    int ia1=slices[islice]->get_iax1(), ia2=slices[islice]->get_iax2();
    slices[islice]->get_data_loc(ix,iy,iloc);
    com->at(1)="pick"; com->at(2)="del";
    com->at(3)=util::string_from_int_array(8,iloc)+":1:"+QString::number(ia1)+":"+
      QString::number(ia2);
   ivalid_down=islice+1000;
   down_x=ix;
   down_y=iy;
    TIME.start();
  if(move) com->at(1)="none";
}
void mouse_func::update_pos(std::vector <slice*>slices, int islice,int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
      if(draw_o==0){}
    if(islice<0 && ivalid_down!=-5) {com->at(1)/*=*/="none"; return;}
  if(ivalid_down==-5){
    ivalid_down=-1;
     com->at(1)="orient"; com->at(2)="vsize_end"; 
      com->at(3)=QString::number(ix);
      com->at(4)=QString::number(iy);
  }
  else slices[islice]->update_pos(com,ix,iy,pos);
  
}
void mouse_func::update_range(std::vector<slice*>slices,int islice, int ix,int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
       if(draw_o==0){}
    if(islice<0) {com->at(1)/*=*/="none"; return;}

   com->at(1)="none";
   if(ivalid_down!=islice){
     return;
   }
   if(abs(down_x-ix) >2 && abs(down_y-iy) >2) 
   slices[islice]->update_range(com,down_x,down_y,ix,iy,pos);
}
void mouse_func::range_down(std::vector<slice*>slices,int islice, int ix,int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
 if((int)slices.size()==0 || pos==0 ){}
    if(draw_o==0){}
    if(islice<0) {com->at(1)/*=*/="none"; return;}

 ivalid_down=islice;
  down_x=ix; down_y=iy;
  down_time=clock();    
  TIME.start();
  com->at(1)="none"; 
}
void mouse_func::range_move(std::vector<slice*>slices,int islice, int ix,int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    if(draw_o==0){}
    if(islice<0) {com->at(1)/*=*/="none"; return;}

  if((int)slices.size() ==0 && pos==0 ){}
  com->at(1)="none";
  if(ivalid_down!=islice){ 
    ivalid_down=-1;
    return ;
  }

  if(TIME.elapsed() > 30) {
       TIME.restart();

       draw_o->objs.push_back(new draw_line(down_x,down_y,ix,iy));
     }
  com->at(1)="redraw"; com->at(2)="only";
  
}
/*ARGSUSED*/
void mouse_func::add_pt(std::vector <slice*>slices,int islice,int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
(void) pos; // quiet unused argument complaint
    if(draw_o==0){}

    if(islice<0) {com->at(1)/*=*/="none"; return;}
 int iold[8],iloc[8];

    int ia1=slices[islice]->get_iax1(), ia2=slices[islice]->get_iax2();
    slices[islice]->get_data_loc(ix,iy,iloc); 
   com->at(1)="pick";
   if(ivalid_down-1000==islice){
        com->at(2)="move"; 
      slices[islice]->get_data_loc(ix,iy,iloc); 
      slices[islice]->get_data_loc(down_x,down_y,iold);
    com->at(3)=util::string_from_int_array(8,iloc)+":1:"+
      util::string_from_int_array(8,iold)+":"
      +QString::number(ia1)+":"+QString::number(ia2);
    }
    else{
   com->at(2)="add"; 
      com->at(3)=util::string_from_int_array(8,iloc)+":1:"+QString::number(ia1)+":"+
      QString::number(ia2);
    }
   ivalid_down=-3;
  
}
void mouse_func::reset(std::vector <slice*>slices,int islice,int ix, int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o)
{
    if(draw_o==0 || (ix==0 && iy==0 && islice==0 && ((int)slices.size())==0)){}
    if(islice<0) {com->at(1)/*=*/="none"; return;}

    int beg[8],end[8],order[8];
    pos->get_orders(order);
    for( int it=0; it< 8; it++) {
     // beg[order[it]]=0; end[order[it]]=pos->get_axis(it).n;
     beg[it]=0; end[it]=pos->get_axis(it).n;

    }
//   for(int it=3; it < 8; it++){
  //    beg[order[it]]=pos->get_beg(order[it]); end[order[it]]=pos->get_end(order[it]);
     //    fprintf(stderr,"setting2 end %d %d %d \n",it,order[it],end[order[it]]);
  // }
    com->at(1)="navigate"; com->at(2)="range";
    com->at(3)=util::string_from_int_array(8,beg)+":"+util::string_from_int_array(8,end)+
    ":"+QString::number(slices[islice]->iax1)+":"+QString::number(slices[islice]->iax2);
    
}

void mouse_func::move_pt(std::vector<slice*>slices,int islice, int ix,int iy, orient_cube *pos, std::vector<QString> *com, draw_other *draw_o){
    if(draw_o==0){}
    if(islice<0) {com->at(1)/*=*/="none"; return;}

  if((int)slices.size() ==0 && pos==0 ){}
  com->at(1)="none";
  if(ivalid_down!=islice && ivalid_down!=islice+1000){ 
    ivalid_down=-1;
    return ;
  }

  if(TIME.elapsed() > 30) {
       TIME.restart();

       draw_o->objs.push_back(new draw_line(ix-2,iy-2,ix+2,iy+2));
     
     
  com->at(1)="redraw"; com->at(2)="only";
  }
}
void mouse_func::non_slice_down(int ix,int iy, std::vector<QString> *com){

 ivalid_down=-5;
  down_time=clock();    
  TIME.start();

  com->at(1)="orient"; com->at(2)="vsize_beg"; 
  com->at(3)=QString::number(ix);      com->at(4)=QString::number(iy);

}
void mouse_func::non_slice_move(int ix,int iy,std::vector<QString> *com,draw_other *draw_o){
  
 if(draw_o==0){}
  ivalid_down=-5;

  if(TIME.elapsed() > 30) {
       TIME.restart();
            

      com->at(1)="orient"; com->at(2)="vsize"; 
      com->at(3)=QString::number(ix);      com->at(4)=QString::number(iy);
// draw_o->objs.push_back(new draw_line(ix-2,iy-2,ix+2,iy+2));
  }
  
}


