
region_1d::region_1d(int ib, int ie, hypercube_float *ar){

 std::vector<float> vals;
 float f,b=ar->vals[ib], e=ar->vals[ie];
 float min=99999;max=-99999.;sum=0;
 var=0.;
 for(int i=ib; i < ie; i++) {
   f=(float)(ib-i)/(float)(ie-ib);
   vals.push_back((1.-f)*b+f*e);
   if(vals[i] < min) min=vals[i];
   if(vals[i]> max) max=vals[i];
   sum+=vals[i];
   var=var+vals[i]*vals[i];
 }
 if(sum <0){
   for(int i=0; i < (int) vals.size(); i++) vals[i]=max-vals[i];
 }
 else{
   for(int i=0; i < (int) vals.size(); i++) vals[i]=vals[i]-min;
 }
 i_e=ie; i_b=ib;
}
void lloyd_map_1d::lloyd_map_1d(hypercube_float *v, int nreg)
{
  if(v->get_ndim()!=1) fprintf(stderr,"ERROR expecting 1d function \n");
  int ib=0;
  float dreg=(float)v->get_n123()/(float)nreg;
  for(int i=0; i < nreg; i++){
    int ie=(int)dreg*(i+1);
    if(i==nreg-1) ie=v->get_n123(); //rounding error issues
     regions.push_back(region_1d(ib,ie,v));
     ib=ie;
  }
  trace=ar;
  var_split.push_back(99999.); //By default don't split any regions
}
void lloyd_map_1d::update_regions(int iter){

 std::vector<float>> centers;
  var;
 for(int i=0; i < (int)regions.size(); i++){
   centers[i].push_back(regions[i]->return_center());
   var[i].push_back(regions[i]->var);
 }
 remove_close_centers(iter,centers,var);
 split_big_var(iter,centers,var);
 
 int ib=0,ibo=0;
 regions.clear();
 for (int iter=0; iter < (int)centers.size(); iter++){
   regions
   regions.push_back(region_1d(ib,ie,tr))
   ibo=ib;
   ib=ie;
}
void lloyd_map::split_big_var(int iter, std::map<int,std::vector<float>> centers,
  std::map<int,float> var){
  
   flaot var_s;
   if(iter <= (int) var_split.size()) var_s=var_split[var_split.size()-1];
   else var_s=var_split[iter];



}


void lloyd_map_1d::find_bound(){

  for(int i=0; i < center.size()-1; i++) bound[i]=(center[i]+center[i+1])/2.;
  bound[center.size()-1]=max_v+5;
}
void lloyd_map_1d::partition_space(){
 int iloc=0;
 float end=bound[iloc];
 for(int i=0; i < region.size();i++){
   if( i >end) {iloc++;end=bound[iloc];}
   region[i]=iloc;
 }
}
void lloyd_map_1d::compute_center(){
  this->compute_error
  this->compute_back();
  for(int i=0; i < wt.size(); i++){ wt[i]=sum[i]=0.;}
    for(int id=0; id < region.size(); id++){
      wt[region[id]]+=(range+back[id]);
      sum[region[id]]+=(float)id*(range+back[id]);
   
   
  }
  for(int i=0; i < wt.size(); i++){
    if(wt[i]==0.) center[i]=0.;
    else center[i]=sum[i]/wt[i];
  }

}