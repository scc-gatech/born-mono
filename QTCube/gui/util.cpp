#include<util.h>
#include<stdio.h>
#include<vector>
#include<string>
#include "percentile.h"
using namespace SEP;
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
std::vector<int> util::int_vector_from_string(QString str){
    int nelem=str.count(":");
    std::vector<int> buf;
    for(int i=0; i < nelem+1; i++) buf.push_back(str.section(":",i,i).toInt());

  return buf;
}
float *util::float_array_from_string(QString str){
    int nelem=str.count(":");
    float *buf=new float[nelem+1];
    for(int i=0; i < nelem+1; i++) buf[i]=str.section(":",i,i).toFloat();

  return buf;
}




 void util::set_float_clip( float *buf, int inum,long long nelem, float *bc, float *ec, int swap){

 float bclip,eclip,clip,bpclip,epclip,pclip,minv,maxv;

  bclip=par->getFloat(std::string("bclip")+std::to_string(inum),-98765.);
  eclip=par->getFloat(std::string("eclip")+std::to_string(inum),-98765.);
  clip=par->getFloat(std::string("clip")+std::to_string(inum),-98765.);
  bpclip=par->getFloat(std::string("bpclip")+std::to_string(inum),-1.);
  epclip=par->getFloat(std::string("epclip")+std::to_string(inum),-1.);
  pclip=par->getFloat(std::string("pclip")+std::to_string(inum),-1.);
  if(bclip!=-98765){
    if(eclip==-98765) par->error("If you provide bclip you must also provide eclip");
    minv=bclip; maxv=eclip;
  }
  else if(clip!=-98765){
    minv=-clip; maxv=clip;
  }
  else{
   
    percentile p=percentile();
    int j=1;
    int nsmall;
    int sub=1;
    j=sub;
    nsmall=nelem/sub;
    while(nsmall>10000000) {
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
      minv=p.find(tempf,nelem,((int)(nsmall*(float)(bpclip/100.))));
      maxv=p.find(tempf,nelem,((int)(nsmall*(float)(epclip/100.))));
    }
    else{
      if(pclip==-1.) pclip=99;
      int pos=(int)nsmall*(float)(pclip/100.);
      if(pos>=nsmall) pos=nsmall-1;
      maxv=p.find_abs(tempf,nsmall,pos);
      minv=-maxv;
    }
    delete []tempf;
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
  }
  
