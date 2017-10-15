#ifndef NFILTER_H
#define NFILTER_H 1
#include "filter.h"
class float_nfilter{
  public:
    float_nfilter(){set_def();};
    virtual float_filter *get_filt(long long iloc){
       fprintf(stderr,"ERROR get_filt not overwritten\n");
       return 0;
    }
    virtual bool check_match(long long n){return false;}

   void set_def(){};
  

};

class basic_float_filter: public float_nfilter{

  public:
    basic_float_filter(){ set_def();}
    basic_float_filter(float_filter *filt);
    basic_float_filter( std::vector<int> lag,std::vector<float> filt);
    float_filter *get_filt(long long iloc){ return &filt;}
    ~basic_float_filter(){}
    virtual bool check_match(long long n){return true;}

    
    float_filter filt;
    

};

class variable_float_filter: public float_nfilter{

  public:
    variable_float_filter(){ set_def();}
    
    variable_float_filter(std::vector<float_filter> fs, std::vector<int> map);
    
    ~variable_float_filter(){ 
      filts.clear();
      filt_map.clear();
    }
    
    float_filter *get_filt(long long iloc){
      return &filts[filt_map[iloc]];
    }
    virtual bool check_match(long long n){
       fprintf(stderr,"CHEXK SIZE %d \n",(int)filt_map.size());
       if(filt_map.size()==n) return true;
       else return false;
    }
    std::vector<float_filter> filts;
    std:: vector<int> filt_map;

};

class float_filt_1d:public float_nfilter{
  public:
    float_filt_1d(){ set_def();}
    float_filt_1d(float_filter f, int n);
//    void float_filt_1d::set_1d(float_filter f, int n);
    void set_1d(float_filter f, int n);
    float_filter *get_filt(long long iloc){
      return &real;
      long long i1=iloc%n1;
      if(i1==n1-1) return &dummy;
      else return &real;
    }
    virtual bool check_match(long long n){
       return true;
    }
   

    long long n1;
    float_filter real,dummy;

};

#endif
