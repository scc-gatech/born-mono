#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_gtext ## SUF (FTYP x, FTYP y, FTYP xpath, FTYP ypath, FTYP xup, \
                     FTYP yup, char *string, char *labelpos)
#else
#define A1(FTYP,SUF) \
int gl_gtext ## SUF (x, y, xpath, ypath, xup, yup, string, labelpos) \
    FTYP            x, y, xpath, ypath, xup, yup; \
    char            *string, *labelpos;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
        gl_tjust ## SUF (labelpos); \
	return(vp_gtext ## SUF  (x, y, xpath, ypath, xup, yup, string)); \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)
