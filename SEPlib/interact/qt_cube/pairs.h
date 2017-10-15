#include <vector>
#ifndef PAIR_H
#define PAIR_H 1
class pair{
  public:
   pair(float ix,float iy,int typ, int ex );
   float x,y;
  int type, extra;
};

class pairs{
  public:
  pairs(){}
  void add_point(float x, float y, int typ, int ex ){ vals.push_back(pair(x,y,typ,ex));}
  void build_int_line(int f, int n, float *ar, float osingle, float dsingle,float osort, 
  float dsort);
  void sort1();
  void sort2();
  void clear(){ vals.clear();}
  void info(char *str,int n);
  int size(){return (int) vals.size();}
  
   std::vector<pair> vals;
};
bool comp1(pair x, pair y);
bool comp2(pair x, pair y);

#endif
