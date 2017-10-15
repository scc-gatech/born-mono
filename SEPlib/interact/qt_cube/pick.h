#ifndef PICK_H
#define PICK_H 1
#include <map>
#include <string>
#include <vector>
#include "hypercube.h"
#include <QColor>
#define DISPLAY_LINE 0
#define DISPLAY_POINTS 1
#define DISPLAY_EXTRA 2
class pick{

 public:
  pick(){}
  pick(int *is,long long p,int te,int ex);
  int iloc[8];
  long long pos;
  int type;
  int extra;
  int info;
  ~pick(){}


};


class picks{
  public:
    
    picks(QColor c);
    long long form_key(int *iloc);
    void key_to_loc(long long key, int *iloc);
    void delete_pick(long long hash);

    long long add_pick(int *iloc, int tp, int ex, bool *del);
    std::vector<pick*> all_picks(std::vector<long long> *list);
   // std::map<QString,pick> bucket;
    std::map<long long,pick> *return_all(){return &bucket2;}
    std::map<long long,pick> bucket2;
    std::vector<long long> find_picks(int ival);
    void set_ns(int *n);
    int get_single(){return single;}
    void set_single(int sing){ single=sing;}
    int get_display(){return display_type;}
    void set_display(int type){ display_type=type;}
    void increase_size(){sz++;}
    void decrease_size(){sz--;}
    int get_sz(){return sz;}
    void loc_to_pos(int *loc, long long *pos);
    void pos_to_loc(long long pos, int *loc);
    
    QColor get_color(){return col;}
    long long blk[8];
    int single;
    int display_type; 
    int sz;
    QColor col;
  
};


#endif
