#ifndef EDIT_RICK
#include "main.h"
#include "map.h"
#define EDIT_RICK 12
extern int EditFrame (Buffer data,Map map1,Map map2,Map map3,int frame,float*);
extern int EditBox (Buffer data,Map map1,Map map2,Map map3,float *dist);
extern int EditStats (int n,float *dist,int *low,int *median,int *high) ;
extern int EditCube (Buffer data,Map map1,Map map2,Map map3,float *dist);
extern int EditGrade (Buffer data,Map map1,Map map2,Map map3);
extern int EditUndo (Buffer data);
#endif

