#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_tjust ## SUF ( char* where)
#else
#define A1(FTYP,SUF) \
int gl_tjust ## SUF (where) \
    char            *where;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
    switch (*where) \
    { \
    case 'l': \
	vp_tjust ## SUF  (TH_CENTER, TV_BOTTOM); \
	break; \
    case 'r': \
	vp_tjust ## SUF  (TH_CENTER, TV_TOP); \
	break; \
    case 't': \
	vp_tjust ## SUF  (TH_CENTER, TV_BOTTOM); \
	break; \
    case 'b': \
	vp_tjust ## SUF  (TH_CENTER, TV_TOP); \
	break; \
    case 's': \
	vp_tjust ## SUF  (TH_SYMBOL, TV_SYMBOL); \
	break; \
    } \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)
