
#include"genericC.h"
#include "ioModes.h"
#include <string.h>
using namespace SEP;
void initializeIO(const int nargs, const int *nlens,const char *ar){

  std::vector<int> lens;
  for(int i=0;i < nargs; i++) lens.push_back(nlens[i]);
  std::vector<std::string> aout=fromChar1D(lens,ar);
  char **argv=new char* [nargs];
  for(int i=0; i < nargs; i++){
    argv[i]=new char[aout[i].length()+1];
    std::copy(aout[i].begin(),aout[i].end(),argv[i]);
  }
 std::shared_ptr<ioModesFortran> x=ioModesFortran::getInstance();
 x->setup(nargs,argv);
 /* SEPlib acts stupid
 for(int i=0; i < nargs; i++){
    delete [] argv[i];
  }
  delete [] argv;
  */
}
void openRegFile(const char *ioName, const char *name,const  char *usg){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    usage_code x;
    if(strcmp("INOUT",usg)==0) x=usageInOut;
    else if(strcmp("OUT",usg)==0) x=usageOut;
    else if(strcmp("IN",usg)==0) x=usageIn;
    else if(strcmp("SCRATCH",usg)==0) x=usageScr;
    std::shared_ptr<genericRegFile> fle=io->getRegFile(std::string(name),x);
}
void openIrregFile(const char *ioName, const char *name, const char *usg){
    std::shared_ptr<genericIO>io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    usage_code x;
    if(strcmp("INOUT",usg)==0) x=usageInOut;
    else if(strcmp("OUT",usg)==0) x=usageOut;
    else if(strcmp("IN",usg)==0) x=usageIn;
    else if(strcmp("SCRATCH",usg)==0) x=usageScr;
    std::shared_ptr<genericIrregFile> fle=io->getIrregFile(std::string(name),x);
}
void closeIO(const char *ioName){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    io->close();
}
void getParamInt(const char *ioName, const char *name, int *val,const  int def){
    std::shared_ptr<paramObj> par=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName))->getParamObj();
    if(def) *val=par->getInt(std::string(name),*val);
    else *val=par->getInt(std::string(name));
}
void getParamFloat(const char *ioName, const char *name, float *val,const  int def){

    std::shared_ptr<paramObj> par=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName))->getParamObj();
    if(def) *val=par->getFloat(std::string(name),*val);
    else *val=par->getFloat(std::string(name));
}
void getParamString(const char *ioName, const char *name, char *val, const int def){
    std::shared_ptr<paramObj> par=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName))->getParamObj();
    std::string x;
    if(def) x=par->getString(std::string(name),std::string(val));
    else x=par->getString(std::string(name));
    std::copy(x.begin(), x.end(), val);
}
void getParamBool(const char *ioName, const char *name, int *val, const int def){

    std::shared_ptr<paramObj> par=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName))->getParamObj();
    bool x=false;
    if(def==1){
       if(*val==1) x=true;
       x=par->getBool(std::string(name),x);
    }
    else x=par->getBool(std::string(name));
    if(x==1) *val=true;
    else *val=false;
}
void getParamInts(const char *ioName, const char *name, const int num, int *val, const int def){
    std::shared_ptr<paramObj> par=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName))->getParamObj();

   if(def){
      std::vector<int> x(val,val+num);
      std::vector<int> y=par->getInts(std::string(name),x);
      for(int i=0; i < std::min(y.size(),x.size()); i++) val[i]=y[i];
   }
   else{
     std::vector<int> y=par->getInts(std::string(name),num);
      for(int i=0; i < std::min((int)y.size(),num); i++) val[i]=y[i];
   }

}
void getParamFloats(const char *ioName, const char *name,const int num, float *val,const  int def){

    std::shared_ptr<paramObj> par=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName))->getParamObj();

   if(def){
      std::vector<float> x(val,val+num);
      std::vector<float> y=par->getFloats(std::string(name),x);
      for(int i=0; i < std::min(y.size(),x.size()); i++) val[i]=y[i];
   }
   else{
     std::vector<float> y=par->getFloats(std::string(name),num);
      for(int i=0; i < std::min((int)y.size(),num); i++) val[i]=y[i];
   }
}
void getFileInt(const char *ioName, const char *tag, const char *name, int *val,const  int def){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      if(def) *val=par->getInt(std::string(name),*val);
      else *val=par->getInt(std::string(name));
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      if(def) *val=par->getInt(std::string(name),*val);
      else *val=par->getInt(std::string(name));
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));

}
void getFileFloat(const char *ioName, const char *tag,const  char *name, float *val, const int def){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      if(def) *val=par->getFloat(std::string(name),*val);
      else *val=par->getInt(std::string(name));
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      if(def) *val=par->getFloat(std::string(name),*val);
      else *val=par->getInt(std::string(name));
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));


}
void getFileString(const char *ioName, const char *tag,  const char *name, char *val,const  int def){

    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      std::string x;
      if(def) x=par->getString(std::string(name),std::string(val));
      else x=par->getString(std::string(name));
      std::copy(x.begin(), x.end(), val);
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      std::string x;
      if(def) x=par->getString(std::string(name),std::string(val));
      else x=par->getString(std::string(name));
      std::copy(x.begin(), x.end(), val);
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));



}
void getDefaultIOName(char *ioName){
  std::string nm=ioModesFortran::getInstance()->getIOModes()->getDefaultType();
  std::copy(nm.begin(),nm.end(),ioName);
}
void getFileBool(const char *ioName, const char *tag,  const char *name, int *val, const int def){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
      int x=0;
      if(*val==1) x=true;

    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      if(def) x=par->getBool(std::string(name),x);
      else x=par->getBool(std::string(name));
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      if(def) x=par->getBool(std::string(name),x);
      else x=par->getBool(std::string(name));
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
   if(x) *val=1;
   else *val=0;


}
void getFileInts(const char *ioName, const char *tag,  const char *name, const int num, int *val, const int def){

    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
       std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
       if(def){
         std::vector<int> x(val,val+num);
         std::vector<int> y=par->getInts(std::string(name),x);
         for(int i=0; i < std::min(y.size(),x.size()); i++) val[i]=y[i];
       }
       else{
         std::vector<int> y=par->getInts(std::string(name),num);
         for(int i=0; i < std::min((int)y.size(),num); i++) val[i]=y[i];
       }
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
       if(def){
         std::vector<int> x(val,val+num);
         std::vector<int> y=par->getInts(std::string(name),x);
         for(int i=0; i < std::min(y.size(),x.size()); i++) val[i]=y[i];
       }
       else{
         std::vector<int> y=par->getInts(std::string(name),num);
         for(int i=0; i < std::min((int)y.size(),num); i++) val[i]=y[i];
       }
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));

}
void getFileFloats(const char *ioName, const char *file, const char *name, const int num, float *val,const  int def){

    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
       std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
       if(def){
         std::vector<float> x(val,val+num);
         std::vector<float> y=par->getFloats(std::string(name),x);
         for(int i=0; i < std::min(y.size(),x.size()); i++) val[i]=y[i];
       }
       else{
         std::vector<float> y=par->getFloats(std::string(name),num);
         for(int i=0; i < std::min((int)y.size(),num); i++) val[i]=y[i];
       }
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
       if(def){
         std::vector<float> x(val,val+num);
         std::vector<float> y=par->getFloats(std::string(name),x);
         for(int i=0; i < std::min(y.size(),x.size()); i++) val[i]=y[i];
       }
       else{
         std::vector<float> y=par->getFloats(std::string(name),num);
         for(int i=0; i < std::min((int)y.size(),num); i++) val[i]=y[i];
       }
   }
   else io->getParamObj()->error(std::string(file)+std::string(" has not been initialized"));
}
void putFileInt(const char *ioName, const char *tag,const  char *name, const int val){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      par->putInt(std::string(name),val);
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      par->putInt(std::string(name),val);

   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));




}
void putFileFloat(const char *ioName, const char *tag, const char *name,const  float val){

    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      par->putFloat(std::string(name),val);
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      par->putFloat(std::string(name),val);

   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));





}
void putFileString(const char *ioName, const char *tag,const  char *name,const  char *val){

    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      par->putString(std::string(name),std::string(val));
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      par->putString(std::string(name),std::string(val));

   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
void putFileBool(const char *ioName, const char *tag,const  char *name, int val){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    bool x=false;
    if(val==1) x=true;
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      par->putBool(std::string(name),x);
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      par->putBool(std::string(name),x);

   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));



}
void putFileInts(const char *ioName, const char *tag,const  char *name, const int num, const int *val){

    std::vector<int> v(val,val+num);
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      par->putInts(std::string(name),v);
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      par->putInts(std::string(name),v);

   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));

}
void putFileFloats(const char *ioName, const char *file, const char *name,const int num, const float *val){

    std::vector<float> v(val,val+num);
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(name))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(name));
      par->putFloats(std::string(name),v);
    }
    else if(io->irregFileExists(std::string(name))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(name));
      par->putFloats(std::string(name),v);

   }
   else io->getParamObj()->error(std::string(file)+std::string(" has not been initialized"));

}

void readFloatStream(const char *ioName, const char *tag,const  long long npts, float *array){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));

    if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->readFloatStream(array,npts);
            par->setDataType(dataFloat);

   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      par->readFloatStream(array,npts);
            par->setDataType(dataFloat);

  }
  else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
void writeFloatStream(const char *ioName, const char *tag,const  long long npts, const float *array){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));

    if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->writeFloatStream(array,npts);
            par->setDataType(dataFloat);

   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      
      par->writeFloatStream(array,npts);
            par->setDataType(dataFloat);

  }
  else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
void readFloatWindow(const char *ioName, const char *tag,const  int ndim,const  int *nw,const  int *fw, const int *jw, float *ar){
  std::vector<int> n_w(nw,nw+ndim),f_w(fw,fw+ndim);
  std::vector<int>  j_w(jw,jw+ndim);

     std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->readFloatWindow(n_w,f_w,j_w,ar);
            par->setDataType(dataFloat);

   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      par->readFloatWindow(n_w,f_w,j_w,ar);
            par->setDataType(dataFloat);

   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));

}
void writeFloatWindow(const char *ioName, const char *tag, const int ndim,const  int *nw,const  int *fw, const int *jw, const float *ar){

  std::vector<int> n_w(nw,nw+ndim),f_w(fw,fw+ndim), j_w(jw,jw+ndim);
     std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->writeFloatWindow(n_w,f_w,j_w,ar);
       par->setDataType(dataFloat);
   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      par->writeFloatWindow(n_w,f_w,j_w,ar);   
       par->setDataType(dataFloat);
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
void readDescription(const char *ioName, const char *tag){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));

   if(io->regFileExists(std::string(tag))){
      io->getRegFile(std::string(tag))->readDescription();
      
   }
   else if(io->irregFileExists(std::string(tag))){
      io->getIrregFile(std::string(tag))->readDescription();
  }
  else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
void writeDescription(const char *ioName, const char *tag){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));

   if(io->regFileExists(std::string(tag))){
      io->getRegFile(std::string(tag))->writeDescription();
      
   }
   else if(io->irregFileExists(std::string(tag))){
      io->getIrregFile(std::string(tag))->writeDescription();
  }
  else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
void getDataType(const char *ioName, const char *tag, char *typ){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));

   dataType x;
   if(io->regFileExists(std::string(tag))){
      x=io->getRegFile(std::string(tag))->getDataType();
      
   }
   else if(io->irregFileExists(std::string(tag))){
      x=io->getIrregFile(std::string(tag))->getDataType();
  }
  else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
  switch(x){
    case dataFloat:
      strcpy(typ,"Float");
      break;
     case dataComplex:
      strcpy(typ,"Complex");
      break;
    case dataByte:
          strcpy(typ,"Byte");
      break;
    case dataShort:
          strcpy(typ,"Short");
      break;
    case dataInt:
          strcpy(typ,"Int");
      break;
    case dataUndefined:
          strcpy(typ,"Undefined");
      break;
      
  }

}
void setDataType(const char *ioName, const char *tag, const char *typ){
     std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));

  dataType t;
  if(strcmp(typ,"Float")==0)  t=dataFloat;
  else if(strcmp(typ,"Byte")==0)  t=dataByte;
  else if(strcmp(typ,"Complex")==0)  t=dataComplex;
  else if(strcmp(typ,"Short")==0)  t=dataShort;
  else if(strcmp(typ,"Int")==0)  t=dataInt;
  else if(strcmp(typ,"Undefined")==0)  t=dataUndefined;
   if(io->regFileExists(std::string(tag))){
     io->getRegFile(std::string(tag))->setDataType(t);
      
   }
   else if(io->irregFileExists(std::string(tag))){
     io->getIrregFile(std::string(tag))->setDataType(t);
  }
  else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
      
  }
void cError(const char *ioName,  const char *err){
     ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName))->getParamObj()->error(std::string(err));


  }

void getHyper(const char *ioName, const char *tag,const int ndimin, int *ndimout, int *n, float *o, float *d,  int *nlens, char *big){

     std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
     std::shared_ptr<hypercube> h;
    if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      h=par->getHyper();
   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      h=par->getHyper();
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));

   int ndim=h->getNdim();
   std::vector<std::string> labels;
   if(ndim > ndimin) io->getParamObj()->error(std::string(tag)+std::string(" has ")+std::to_string(ndim)+" only provided space for "+std::to_string(ndimin));
   for(int i=0; i < ndim; i++){
     axis a=h->getAxis(i);
     n[i]=a.n; o[i]=a.o; d[i]=a.d;
     labels.push_back(a.label);
   }
   
   std::vector<int> lens;
   for(int i=0; i< ndim; i++) lens.push_back(nlens[i]);
   toChar1D(labels,ndim,lens,big);
   *ndimout=ndim;
}

void setHyper(const char *ioName, const char *tag,const int ndim,const  int *n,const  float *o, const float *d,const int *nlens, 
  const char *lin){
   std::vector<axis> axes;
   std::vector<int> lens;
   for(int i=0;i <ndim; i++) lens.push_back(nlens[i]);
   std::vector<std::string> label=fromChar1D(lens,lin);
   
   for(int i=0; i < ndim; i++){
      axes.push_back(axis(n[i],o[i],d[i],std::string(label[i])));
      }
   std::shared_ptr<hypercube> h(new hypercube(axes));
   std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
   if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->setHyper(h);
   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      par->setHyper(h);
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
std::vector<std::string> fromChar1D(const std::vector<int> &lens,const  char *val){
   std::vector<std::string> out;
   
   int ipos=0;
   for(int i2=0; i2< lens.size();i2++){
     out.push_back(std::string(val+ipos,val+ipos+lens[i2]));
     ipos+=lens[i2];
   }
   return out;

}

void toChar1D(const std::vector<std::string> in,const  int nargs, std::vector<int> &lens, char *val){
   int ipos=0;
   for(int i2=0; i2 < nargs;i2++){
      std::copy(in[i2].begin(),in[i2].end(),&val[ipos]);
      lens[i2]=in[i2].length();
      ipos+=lens[i2];
    }

}

void readComplexStream(const char *ioName, const char *tag,const  long long npts, float  _Complex *array){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
            

    if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->readComplexStream(array,npts);
      par->setDataType(dataComplex);
   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      par->readComplexStream(array,npts);
        par->setDataType(dataComplex);
  }
  else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
void writeComplexStream(const char *ioName, const char *tag,const  long long npts, const float  _Complex *array){
    std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(tag))){
           

      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->writeComplexStream(array,npts);
      par->setDataType(dataComplex);
   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      par->writeComplexStream(array,npts);
      par->setDataType(dataComplex);
  }
  else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}
void readComplexWindow(const char *ioName, const char *tag,const  int ndim,const  int *nw,const  int *fw, const int *jw, float  _Complex *ar){
  std::vector<int> n_w(nw,nw+ndim),f_w(fw,fw+ndim);
  std::vector<int>  j_w(jw,jw+ndim);

     std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->readComplexWindow(n_w,f_w,j_w,ar);
      par->setDataType(dataComplex);

   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      par->readComplexWindow(n_w,f_w,j_w,ar); 
            par->setDataType(dataComplex);
  
   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));

}
void writeComplexWindow(const char *ioName, const char *tag, const int ndim,const  int *nw,const  int *fw, const int *jw, const float _Complex *ar){
  std::vector<int> n_w(nw,nw+ndim),f_w(fw,fw+ndim), j_w(jw,jw+ndim);

     std::shared_ptr<genericIO> io=ioModesFortran::getInstance()->getIOModes()->getIO(std::string(ioName));
    if(io->regFileExists(std::string(tag))){
      std::shared_ptr<genericRegFile> par=io->getRegFile(std::string(tag));
      par->writeComplexWindow(n_w,f_w,j_w,ar);
            par->setDataType(dataComplex);

   }
    else if(io->irregFileExists(std::string(tag))){
      std::shared_ptr<genericIrregFile> par=io->getIrregFile(std::string(tag));
      par->writeComplexWindow(n_w,f_w,j_w,ar);   
            par->setDataType(dataComplex);

   }
   else io->getParamObj()->error(std::string(tag)+std::string(" has not been initialized"));
}

