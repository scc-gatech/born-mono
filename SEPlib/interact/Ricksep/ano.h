#ifndef ANO_INC 
#define ANO_INC 4
#include<plane.h>








/*
point pick object
*/
#include "pik.h"
#define N_ANO		250
#define MAX_ANO_PTS 50
#define ANO_UNDEFINE 0
#define ANO_TEXT 1
#define ANO_OVAL 2
#define ANO_POLY 3
#define ANO_CURVE 4
#define ANO_BOX 5
#define MAX_STRING 512
#define ANO_ADD_PT 1
#define ANO_DELETE_PT 2
#define ANO_FINISHED 0


typedef struct {
	int n_ano;
	int type[N_ANO];
	int npts[N_ANO]; 
	int rotate[N_ANO];
	int fat[N_ANO];
	int last_op;
	int arrow[N_ANO];
	int size[N_ANO];
	char text[N_ANO][MAX_STRING];
	short my_loc[N_ANO][MAX_ANO_PTS][DATA_NAXIS];
	int do_anno;
	int my_fat;
	int do_close_poly;
	int cur_anno;
	} *Anolist;

typedef struct {
  float x_ll,x_ur,y_ll,y_ur;
	float x_sep,y_sep;
	char commands[N_ANO][MAX_STRING];
  int ncom,fat;
} Vplot2sep;


typedef struct {
  Anolist ano[NUM_VIEWS];
} *Anoviews;




#endif
