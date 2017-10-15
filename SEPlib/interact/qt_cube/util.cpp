#include <stdio.h>
#include<util.h>
#include<vector>
#include<string>
#include "percentile.h"

QString util::string_from_int_array(int n,int *ar){
   QString st=QString::number(ar[0]);
   for(int i=1; i<n;i++) st=st+":"+QString::number(ar[i]);
  return st;
}
QString util::string_from_float_array(int n,float *ar){
   QString st=QString::number(ar[0]);
   for(int i=1; i<n;i++) st=st+":"+QString::number(ar[i]);
  return st;
}
int *util::int_array_from_string(QString str){
    int nelem=str.count(":");
    int *buf=new int[nelem+1];
    for(int i=0; i < nelem+1; i++) buf[i]=str.section(":",i,i).toInt();

  return buf;
}
float *util::float_array_from_string(QString str){
    int nelem=str.count(":");
    float *buf=new float[nelem+1];
    for(int i=0; i < nelem+1; i++) buf[i]=str.section(":",i,i).toFloat();

  return buf;
}
  QString util::param_string(QString p, int inum){
     QString get=param_string(p,inum,"NaNa");
     char buf[1024];
     if(get=="NaNa") {
       sprintf(buf,"Couldn't read %s \n",p.ascii());
       par->error(buf);
        
      }
     return get;
  }
  QString util::param_string(QString para, int inum, QString def){
    std::string dumb="NaNa";
    QString tmp=para+QString::number(inum);
    std::string dumb2=par->get_string(
    tmp.ascii(),dumb);
    std::string defs=def.ascii();
    if(dumb2==dumb) return par->get_string(para.ascii(),defs).c_str();
    return dumb2.c_str();
  
  }
  int util::param_int(QString para, int inum){
    int get=param_int(para,inum,-931246);
    
      char buf[1024];
       sprintf(buf,"Couldn't read %s \n",para.ascii());
     
    if(get==-931246) par->error(buf);
    return get;
  }
  int util::param_int(QString para, int inum, int def){
    int dumb=-9761234;
    QString tmp=para+QString::number(inum);
    int dumb2=par->get_int(tmp.ascii(),dumb);
    if(dumb2==dumb)
      return par->get_int(para.ascii(),def);
    return dumb2;
    
  }
    float util::param_float(QString para, int inum){
    float get=param_float(para,inum,-931246);
       char buf[1024];
       sprintf(buf,"Couldn't read %s \n",para.ascii());
    if(get==-931246) par->error(buf);
    return get;
  }
  float util::param_float(QString para, int inum, float def){
    float dumb=-9761234;
    QString tmp=para+QString::number(inum);
    float dumb2=par->get_float(tmp.ascii(),dumb);
    if(dumb2==dumb)
      return par->get_float(para.ascii(),def);
    return dumb2;
    
  }
  std::vector<int> util::param_ints(QString para, int inum, std::vector<int> def){
    int dumb[9999];
    for(int i=0; i < 9999; i++) dumb[i]=-9761234;
    QString tmp=para+QString::number(inum);
    int *dumb2=par->get_ints(tmp.ascii(),9999,dumb);
    if(dumb2[0]==-9761234){
       delete [] dumb2;
       dumb2=par->get_ints(para.ascii(),9999,dumb);
    }
    std::vector<int> back;
    if(dumb2[0]!=-9761234){
      int i=0;
      
      while(dumb2[i]!=-9761234){
         back.push_back(dumb2[i]);
         i+=1;
      }
      delete [] dumb2;
      return back;
    }
    delete [] dumb2;
    for(int i=0; i< (int)def.size(); i++){
      def.push_back(def[i]);
    }
    return def;
  }
  std::vector<int> util::param_ints(QString para, int inum){
    std::vector<int> a;
    std::vector<int> get=param_ints(para,inum,a);
    char buf[1024];
       sprintf(buf,"Couldn't read %s \n",para.ascii());
    if((int)a.size()==0) par->error(buf);
    return get;
  }
  
 void util::set_float_clip( float *buf, int inum,long long nelem, float *bc, float *ec, int swap){

 float bclip,eclip,clip,bpclip,epclip,pclip,minv,maxv;

  bclip=param_float("bclip",inum,-98765.);
  eclip=param_float("eclip",inum,-98765.);
  clip=param_float("clip",inum,-98765.);
  bpclip=param_float("bpclip",inum,-1.);
  epclip=param_float("epclip",inum,-1.);
  pclip=param_float("pclip",inum,-1.);
  if(bclip!=-98765){
    if(eclip==-98765) par->error("If you provide bclip you must also provide eclip");
    minv=bclip; maxv=eclip;
  }
  else if(clip!=-98765){
    minv=-clip; maxv=clip;
  }
  else{
   
    percentile *p=new percentile();
    int j=1;
    int nsmall;
    int sub=7;
    j=sub;
    nsmall=nelem/sub;
    while(nsmall>1000000) {
      sub=sub+2;
      j=sub*j;
      nsmall=nsmall/sub;
    }
  
    

    float *tempf=new float[nsmall];
     int ii=0,i=0;
     for(i=0; i< nelem && ii < nsmall; i+=j,ii++) tempf[ii]=buf[i];
//    memcpy((void*)tempf,(const void*) buf,sizeof(float)*nelem);
          
    if(swap==1) swap_float_bytes(nsmall,tempf);
    if(bpclip!=-1 || epclip!=-1){
      if(bpclip==-1) bpclip=0;
      if(epclip==-1) epclip=100;
      minv=p->find(tempf,nelem,((int)(nsmall*(float)(bpclip/100.))));
      maxv=p->find(tempf,nelem,((int)(nsmall*(float)(epclip/100.))));
    }
    else{
      if(pclip==-1.) pclip=99;
      int pos=(int)(nsmall*(float)(pclip/100.));
      if(pos>=nsmall) pos=nsmall-1;
      maxv=p->find_abs(tempf,nsmall,pos);
      minv=-maxv;
    }
    delete []tempf;
    delete p;
   }
   *bc=minv;
   *ec=maxv;
}


void util::convert_to_byte(float *fbuf, long long foff, unsigned char *cbuf, long long coff,
    long long nelem, float bclip, float eclip){
  int j;
  float range=eclip-bclip;
  for(long long i=0; i < nelem; i++){
    j=(int)(255*(fbuf[i+foff]-bclip)/range);
    if(j<0) j=0;
    if(j>255) j=255;
    cbuf[i+coff]=j;
  }
 }
 void util::swap_float_bytes(int n, float *buf){
// int *tni4;
 for(int i=0; i< n; i++){
   float *tnf4=&buf[i];
  int *tni4=(int *)tnf4;
   *tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
            ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));

  }
}
void util::convert_to_float(float *fbuf, long long foff, unsigned char *cbuf, long long coff,
    long long nelem, float bclip, float eclip){

    float o=bclip;
    float d=(eclip-bclip)/256;

    for(long long i=0; i< nelem; i++){
      fbuf[i+foff]=cbuf[i+coff]*d+o;
    }
    //fprintf(stderr,"READ CONVERSIOn %d %f \n",(int)cbuf[100+coff],fbuf[100+foff]);
  }
  
