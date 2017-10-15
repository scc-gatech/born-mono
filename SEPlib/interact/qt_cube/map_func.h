#ifndef MAP_FUNC_H
#define MAP_FUNC_H 1
class map_func{
  public:
  map_func(){}
  virtual void to_grid(int *loc){ assert(1==0);}
  virtual void to_io(int *loc){ assert(1==0);}
};
class ndmap_func: public map_func{
  ndmap_func

#endif
