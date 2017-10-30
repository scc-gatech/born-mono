#include<string.h>
#include "sep3dFile.h"
extern "C" {
#include "seplib.h"
#include "sep3d.h"
}
#include <assert.h>
using namespace SEP;

sep3dFile::sep3dFile(const std::string  &tag,const SEP::usage_code usage){
 _tag=tag;
 switch(usage){
    case usageIn:
      if(_tag!="in")
        auxin(_tag.c_str());
    case usageOut:
       if(tag!="out")
          auxout(_tag.c_str());
    case usageInOut:
       auxinout(_tag.c_str());
    case usageScr:
      auxscr(_tag.c_str());
  }
 
}

int sep3dFile::getInt(const std::string &arg)const{
    int x;
    if(0==auxpar(arg.c_str(),"d",&x,_tag.c_str()))
      error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
    return x;
}
int sep3dFile::getInt(const std::string &arg, const int def)const{
   int x=def;
   int i=auxpar(arg.c_str(),"d",&x,_tag.c_str());
   return x;
}
   
float sep3dFile::getFloat(const std::string &arg, const float def)const{
  float x;
  int i=auxpar(arg.c_str(),"f",&x,_tag.c_str());
  return x;

}
float sep3dFile::getFloat(const std::string &arg)const{
  float x;
  if(0==auxpar(arg.c_str(),"f",&x,_tag.c_str()))
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  return x;
}
   
std::string sep3dFile::getString(const std::string &arg)const{
  char buf[10000];
  if(0==auxpar(arg.c_str(),"s",buf,_tag.c_str()))
      error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  return std::string(buf);


}
std::string sep3dFile::getString(const std::string &arg, const std::string &def)const{

  char buf[10000];
  std::copy(def.begin(), def.end(), buf);
  int i=auxpar(arg.c_str(),"s",buf,_tag.c_str());
  return std::string(buf);

}
 
bool sep3dFile::getBool(const std::string &arg, const bool def)const{
  bool x=def;
  int i=auxpar(arg.c_str(),"l",&x,_tag.c_str());
  return x;

}
bool sep3dFile::getBool(const std::string &arg)const{
 bool x;
  if(0==auxpar(arg.c_str(),"l",&x,_tag.c_str())){
    error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  }
  return x;

}
   
   
std::vector<int> sep3dFile::getInts(const std::string &arg,int nval)const{
  int tmp[10000];
  int ierr=auxpar(arg.c_str(),"d",tmp,_tag.c_str());
  if(ierr==0) 
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::vector<int> x;
  for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  return x;
}
std::vector<int> sep3dFile::getInts(const std::string &arg,std::vector<int> &defs)const{
  int tmp[10000];
  for(int i=0; i < defs.size(); i++){
    tmp[i]=defs[i];
  }
  int ierr=auxpar(arg.c_str(),"d",tmp,_tag.c_str());
  if(ierr==0) 
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::vector<int> x;
  if(ierr>0){  
    for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  }
  else{
    for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
  }
  return x;
}
     
std::vector<float> sep3dFile::getFloats(const std::string &arg,int nvals)const{
  float tmp[10000];
  int ierr=auxpar(arg.c_str(),"f",tmp,_tag.c_str());
  if(ierr==0) 
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::vector<float> x;
  for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  return x;


}
std::vector<float> sep3dFile::getFloats(const std::string &arg,std::vector<float>  &defs)const{

  float tmp[10000];
  for(int i=0; i < defs.size(); i++){
    tmp[i]=defs[i];
  }
  int ierr=auxpar(arg.c_str(),"f",tmp,_tag.c_str());
  if(ierr==0) 
     error(std::string("trouble grabbing parameter ")+arg+std::string(" from parameters"));
  std::vector<float> x;
  if(ierr>0){  
    for(int i=0; i < ierr; i++) x.push_back(tmp[i]);
  }
  else{
    for(int i=0; i < defs.size(); i++) x.push_back(defs[i]);
  }
  return x;
}


void sep3dFile::error(const std::string err)const{
   seperr(err.c_str());
}
    
void sep3dFile::putInt(const std::string par, const int val)const{

   auxputch(par.c_str(),"d",&val,_tag.c_str());

}
void sep3dFile::putFloat(const std::string par, const float val)const{

   auxputch(par.c_str(),"f",&val,_tag.c_str());
}
void sep3dFile::putString(const std::string par, const std::string val)const{
  auxputch(par.c_str(),"s",val.c_str(),_tag.c_str());
}

void sep3dFile::putBool(const std::string par, const bool val)const{
   int x=0;
   if(val) x=1;
   auxputch(par.c_str(),"l",&x,_tag.c_str());    

}
void sep3dFile::putInts(const std::string par, const  std::vector<int>& val)const{
   int *tmp=new int[val.size()];
   for(int i=0; i < val.size(); i++) tmp[i]=val[i];
   auxputch(par.c_str(),"d",tmp,_tag.c_str());
   delete [] tmp;
}
void sep3dFile::putFloats(const std::string par, const std::vector<float> &val)const{
   float *tmp=new float[val.size()];
   for(int i=0; i < val.size(); i++) tmp[i]=val[i];
   auxputch(par.c_str(),"f",tmp,_tag.c_str());
   delete [] tmp;

}
void sep3dFile::readFloatStream( float *array,const long long npts){ 
  assert(1==-1);

}
void sep3dFile::readUCharStream( unsigned char *array,const long long npts){ 
  assert(1==-1);

}

void sep3dFile::writeFloatStream( const float *array,const long long npts){
  setDataType(dataFloat);
  assert(1==-1); 

}
 void sep3dFile::readFloatWindow(const std::vector<int> &nw, const std::vector<int> &fw, 
      const std::vector<int>& jw,  float *array){
   assert(1==-1);
    
}

 void sep3dFile::readUCharWindow(const std::vector<int> &nw, const std::vector<int>& fw, 
      const std::vector<int>& jw,  unsigned char *array){
  assert(1==-1);
    
}
 void sep3dFile::writeFloatWindow(const std::vector<int> &nw, const std::vector<int>& fw, 

      const std::vector<int>& jw,const float *array){
        setDataType(dataFloat);

  assert(1==-1);
      
}
void sep3dFile::readDescription(){
  int ndim;
  sep_get_number_data_axes(_tag.c_str(),&ndim);
  std::vector<axis> axes;
  for(int i=1; i <= ndim; i++){
    int n;
    float o,d;
    char label[1024];
    sep_get_data_axis_par(_tag.c_str(),&i,&n,&o,&d,label);
    axes.push_back(axis(n,o,d,std::string(label)));
  }
  std::shared_ptr<hypercube> hyper(new hypercube(axes));
  int esize=getInt("esize",4);
  if(esize==1) setDataType(dataByte);
  else if(esize==4){
     std::string format=getString(std::string("data_format"),
        std::string("xdr_float"));
     if(format==std::string("xdr_float") ||
       format==std::string("native_float")) setDataType(dataFloat);
     else if (format==std::string("xdr_int") ||
       format==std::string("native_int")) setDataType(dataInt);
     else
       error(std::string("Unknown data type "+format));
  }
  else
    error(std::string("Only know about esize=4 or 1"));
     
  setHyper(hyper);
}
void sep3dFile::writeDescription(){
  std::shared_ptr<hypercube> hyper=getHyper();
  std::vector<axis> axes=hyper->returnAxes(hyper->getNdim());
  for(int i=1; i <= axes.size(); i++){
     int n=axes[i].n; float o=axes[i].o; float d=axes[i].d;
     char label[1024];  std::copy(axes[i].label.begin(), axes[i].label.end(), label);
     sep_put_data_axis_par(_tag.c_str(),&i,&n,&o,&d,label);
  }
  for(int i=axes.size()+1; i< 9; i++){
     int n=1; float o=0.,d=1.;
     char label[1024];  
     strcpy(label,"none");
     sep_put_data_axis_par(_tag.c_str(),&i,&n,&o,&d,label);
  }
  dataType fmt=getDataType();
  switch(fmt){
    case dataFloat:
      putInt("esize",4);
      putString("data_format","xdr_float");
    case dataInt:
       putInt("esize",4);
       putString("data_format","xdr_float");
    case dataByte:
       putInt("esize",1);
   }

}
int sep3dFile::getHeaderIndex(const std::string keyname)const{
  assert(1==-1);
  }
std::vector<headerType> sep3dFile::getTraceHeader(const long long index)const{

assert(1==-1);
}
std::vector<std::string> sep3dFile::getHeaderTypes()const{

  assert(1==-1);


}
