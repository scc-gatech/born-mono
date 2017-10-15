#ifndef PICK_NEW_H
#define PICK_NEW_H 1
#define TYPE_DEFAULT -10
#define EXTRA_DEFAULT -9999
#include <vector>
#include <QString>
#include "position.h"

class pick_new{

 public:
  pick_new(){}
  pick_new(int *iloc,long long p,int te,QString col,int ex);
  inline int *return_loc(){ return iloc;}
  inline long long return_pos(){return pos;}
  void print();
  ~pick_new(){pick_delete();}
  void pick_delete();
  pick_new *clone(){
    pick_new *y=new pick_new(iloc,pos,type,col,extra);
    return y;
  
  }
  long long pos;
  int type;
  QString col;
  int extra;
  int iloc[8];
  int info;
};
class picks_vec{
  public:
    picks_vec(){}
    void add_pick(pick_new *p){ picks.push_back(p);}
    pick_new *return_pick(int i){ return picks[i];}
    int return_size(){return picks.size();}
    std::vector<pick_new*> picks;
    

};
class picks_new{
  public:
  picks_new(){myp=0;  setit=-1; }
  pick_new *get_pick(long long p, QString col);
  picks_new(position *pos);
  pick_new *add_pick(int *iloc,long long p,int te, QString col,int ex);
  void del_pick(long long p, QString col);
  picks_vec *get_parse_picks(QString col="None",int type=TYPE_DEFAULT,int extra=EXTRA_DEFAULT);
  void get_parse_picks(picks_vec *picks, QString col="None",int type=TYPE_DEFAULT,int extra=EXTRA_DEFAULT);
  void delete_pick_type(int typ);
  pick_new *return_pick(int i){ return bucket[i];}
  int return_size(){return bucket.size();}
  void delete_picks();  
  void print(int i);
  ~picks_new(){ delete_picks();}
  private:
     position *myp;
     int setit;
     std::vector<pick_new*> bucket;

};

 




#endif
