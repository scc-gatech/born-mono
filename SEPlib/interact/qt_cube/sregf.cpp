#include<sregf.h>
using std::string;
using std::vector;

void sregf::read_all(string tag,hypercube_float *cube){

   if(this->get_n123() !=cube->get_n123()) {
     char errmsg[] = "Dataset(%d) and sepcube(%d) not the same size\n";
     seperr(errmsg, this->get_n123(),cube->get_n123());
   }
   if(4*this->get_n123()!=(long long) sreed_big(tag.c_str(),cube->vals,this->get_n123()*4)) {
     char errmsg[] = "Trouble reading data from tag %s\n";
     seperr(errmsg,tag.c_str());
   }
}
void sregf::read_next(string tag, hypercube_float *cube){
  if(this->get_n123() < cube->get_n123()) {
    char errmsg[] = "Datasize smaller than hypercube\n";
    seperr(errmsg);
  }
  if(4*cube->get_n123()!=sreed(tag.c_str(),cube->vals,cube->get_n123()*4)) {
    char errmsg[]="Trouble reading data from tag %s\n";
    seperr(errmsg,tag.c_str());
  }
}
void sregf::write_all(string tag,hypercube_float *cube){
   if(this->get_n123() !=cube->get_n123()) {
     char errmsg[] = "Dataset (%d) and sepcube (%d) not the same size\n";
     seperr(errmsg,
     this->get_n123(),cube->get_n123());
   }
   if(4*cube->get_n123()!=srite(tag.c_str(),cube->vals,cube->get_n123()*4)) {
     char errmsg[] = "Trouble reading data from tag %s\n";
     seperr(errmsg,tag.c_str());
   }
}
void sregf::write_next(string tag, hypercube_float *cube){
  if(4*cube->get_n123()!=srite(tag.c_str(),cube->vals,cube->get_n123()*4))  {
    char errmsg[] = "Trouble reading data from tag %s\n";
    seperr(errmsg,tag.c_str());
    }
}
void sregf::read_slice(string tag, vector<int> nwind,hypercube_float *cube){
  vector<int> fwind(nwind),jwind(nwind);
  fwind.assign(jwind.size(),0);
  jwind.assign(jwind.size(),1);
  this->read_subset(tag,nwind,fwind,jwind,cube);
}
void sregf::read_slice(string tag, vector<int> nwind, vector<int> fwind, hypercube_float *cube){
  vector<int> jwind(nwind);
  jwind.assign(jwind.size(),1);
  this->read_subset(tag,nwind,fwind,jwind,cube);
}
void sregf::read_slice(string tag, vector<int> nwind, vector<int>fwind, vector<int> jwind, hypercube_float *cube){

  this->read_subset(tag,nwind,fwind,jwind,cube);
}
void sregf::write_slice(string tag, vector<int> nwind, hypercube_float *cube){
  vector<int> fwind(nwind),jwind(nwind);
  fwind.assign(jwind.size(),0);
  jwind.assign(jwind.size(),1);
  this->write_subset(tag,nwind,fwind,jwind,cube);
}
void sregf::write_slice(string tag, vector<int> nwind, vector<int> fwind, hypercube_float *cube){
  vector<int> jwind(nwind);
  jwind.assign(jwind.size(),1);
  this->write_subset(tag,nwind,fwind,jwind,cube);
}
void sregf::write_slice(string tag, vector<int> nwind, vector<int> fwind, vector<int> jwind, hypercube_float *cube){

  this->write_subset(tag,nwind,fwind,jwind,cube);
}
void sregf::read_subset(string tag, vector<int> nwind, vector<int> fwind, vector<int> jwind, hypercube_float *cube){
  if(nwind.size() !=jwind.size()
  || nwind.size()!=jwind.size() 
  || ((int) (nwind.size()))!=this->get_ndim()) {
    char errmsg[] = "Size of windowing params and data dimensions not all equiv\n";
    seperr(errmsg);
  }
  // int idim;
  int size=4,ndims=this->get_ndim();
  int ng[ndims],nw[ndims],fw[ndims],jw[ndims];


  axis ax;
  for(int i=0 ;i < ndims;i++){ 
    ax=this->get_axis(i+1); 
    ng[i]=ax.n;
    nw[i]=nwind[i]; jw[i]=jwind[i]; fw[i]=fwind[i];
  }
  const char *c_str = tag.c_str ( );
 // fprintf(stderr,"VV %d %d %d - %d %d %d -%d %d %d    \n",
  // fw[0],fw[2],fw[4],nw[0],nw[2],nw[4],ng[0],ng[2],ng[4]);
  int ierr=sreed_window(c_str,&ndims,ng,nw,fw,jw,size,cube->vals);
 
  if(ierr!=0){
    for(int i=0; i < ((int)(nwind.size())); i++){
      fprintf(stderr,"Window parameters (tag=%s) iax=%d ng=%d nw=%d fw=%d jw=%d \n",
        tag.c_str(),i+1,this->get_axis(i+1).n,nwind[i],fwind[i],jwind[i]);
    }
    char errmsg[] = "trouble reading in window\n";
    seperr(errmsg);
   }
  
}
void sregf::write_subset(string tag, vector<int> nwind, vector<int> fwind, vector<int> jwind, hypercube_float *cube){
  if(nwind.size() !=jwind.size() || nwind.size()!=jwind.size() || ((int) (nwind.size()))!=this->get_ndim()) {
    char errmsg[] = "Size of windowing params and data dimensions not all equiv\n";
    seperr(errmsg);
    }
  // int idim;
  int size=4,ndims=this->get_ndim();
  int ng[ndims],nw[ndims],fw[ndims],jw[ndims];
  for(int i=0 ; i < ndims;i++){ 
    ng[i]=this->get_axis(i+1).n; nw[i]=nwind[i]; jw[i]=jwind[i]; fw[i]=fwind[i];
  }
  int ierr=srite_window(tag.c_str(),&ndims,ng,nw,fw,jw,size,cube->vals);
  
  if(ierr!=0){
    for(int i=0; i < ((int)(nwind.size())); i++){
      fprintf(stderr,"Window parameters (tag=%s) iax=%d ng=%d nw=%d fw=%d jw=%d \n",
        tag.c_str(),i+1,this->get_axis(i+1).n,nwind[i],fwind[i],jwind[i]);
    }
    char errmsg[] = "trouble writing out window\n";
    seperr(errmsg);
  }
}





  

