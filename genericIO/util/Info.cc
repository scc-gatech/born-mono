#include<ioModes.h>
#include<math.h>
using namespace SEP;

int findPow(float x){
  for(int e=31; e >=-32; e--){
    int test=x/powf(10.,(float) e);
    if(test!=0) return e;
  }
}


int findSigDigits(float x){
  float base=powf(10.,findPow(x));
  float test=x/base;
  for(int i=0; i < 6; i++){
    int testi=test;
    if(fabsf(test-testi)<.0000001) return i+1;
    test=test*10.;
  }
  return 7;

}
std::string niceFloat(float x){
  std::string y= std::to_string(x);
  while(y.length() >1 && y.back()=='0') y.pop_back();
  return y;
}

std::string division(int line){
  std::string out;
  for(int i=0; i < line; i++) out+="-";
  return out;
}
std::string center(int line,std::string nm){
  int sz=nm.size();
  int left=(line/2-sz/2);
  std::string out="";
  for(int i=0; i < left; i++) out+=" ";
  out+=nm;
  return out;

}
std::string niceLine(int line,std::vector<std::string> parts){
  int sz=line/parts.size();
  std::string out;
  for(int i=0; i < (int)parts.size(); i++){
    out+=parts[i];
    for(int j=parts[i].size(); j < sz; j++) out+=" ";
   }
   return out;
}


int main(int argc, char **argv){

  ioModes modes(argc,argv);

  std::shared_ptr<genericIO>  io=modes.getDefaultIO();
  
  std::shared_ptr<paramObj> par=io->getParamObj();
  std::string in=par->getString(std::string("in"));
  std::shared_ptr<genericRegFile> inp=io->getRegFile(std::string("in"),usageIn);

  std::shared_ptr<hypercube> hyperIn=inp->getHyper();
  std::vector<int> ng=hyperIn->getNs();
  std::string dataT=inp->getDataTypeString();
  long long n123=hyperIn->getN123();
  
  std::cout<<division(80)<<std::endl;
  std::cout<<center(80,in)<<std::endl;
  std::cout<<division(80)<<std::endl;
  std::vector<std::string> tot;
  tot.push_back(std::string("Axis"));
  tot.push_back(std::string("n"));
  tot.push_back(std::string("o"));
  tot.push_back(std::string("d"));
  tot.push_back(std::string("label"));
  std::cout<<niceLine(80,tot)<<std::endl;
  
  for(int i=0; i < ng.size() ;i++){
     axis a=hyperIn->getAxis(i+1);
     std::vector<std::string> list;
     list.push_back(std::to_string(i+1));
     list.push_back(std::to_string(a.n));
     list.push_back(niceFloat(a.o));
     list.push_back(niceFloat(a.d));
     list.push_back(a.label);
     std::cout<<niceLine(80,list)<<std::endl;
  }
  long long sz=inp->getDataSize();
    std::cout<<division(80)<<std::endl;
  std::vector<std::string> sum;
  sum.push_back(std::string("DataType=")+dataT);
  long long n123E=n123*inp->getDataEsize();
 if(n123E<1024)
  sum.push_back(std::string("Size=")+niceFloat((float)n123E)+std::string("B"));
 else if(n123E<1024*1024)
  sum.push_back(std::string("Size=")+niceFloat((float)n123E/1024.)+std::string("KB"));
 else if(n123E<1024*1024*1024)
  sum.push_back(std::string("Size=")+niceFloat((float)n123E/1024./1024.)+std::string("MB"));
 else if(n123E<1024L*1024L*1024L*1024L)
  sum.push_back(std::string("Size=")+niceFloat((float)n123E/1024./1024./1024.)+std::string("GB")); 
 else
  sum.push_back(std::string("Size=")+niceFloat((float)n123E/1024./1024./1024./1024.)+std::string("TB")); 
 sum.push_back(std::string("PctPresent=")+niceFloat(inp->getDataSize()*100./(float)n123/
    inp->getDataEsize())+std::string("%"));
  std::cout<<niceLine(80,sum)<<std::endl;
  std::cout<<division(80)<<std::endl;

}

