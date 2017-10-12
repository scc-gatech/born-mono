#ifndef AXIS_H
#define AXIS_H 1
#include<iostream>
#include<string>
#include <stdio.h>
#include<sstream>

namespace SEP{

class axis{
  public:
    axis(){}  //Default constructor does nothing
    axis(const int n, float o, float d,const  std::string &label, const std::string& unit);
    axis(const int n, float o, float d,const  std::string& label);
    axis(const int n, float o, float d);
    axis(const int n);
    void infoStream(std::stringstream &x);
  
    bool same_axis(const axis &ax)const ;
    int n;
    float o,d;
    std::string label,unit;
    
  ~axis(){}
  private:
    void basic_set(const int n,const float o,const  float d,const  std::string &label, const std::string &unit);
    
    };
    
}
#endif



