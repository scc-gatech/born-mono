#include<string.h>
#include "glplot.h"
#ifdef USE_PROTO
#define A1(FTYP,SUF) \
int gl_titleint ## SUF (struct titleinfo ## SUF  *title)
#else
#define A1(FTYP,SUF) \
int gl_titleint ## SUF (title) \
struct titleinfo ## SUF  *title;
#endif
#define A2(FTYP,FMT,SUF) \
{ \
getch ("wheretitle", "s", title->wheretitle); \
title->wanttitle = 1; \
getch ("wanttitle", "1", &title->wanttitle); \
title->titlefat = 0; \
fetch ("titlefat", "d", &title->titlefat); \
title->titlesz = 10; \
fetch ("titlesz", "f", &title->titlesz); \
strcpy (title->title, " "); \
if (!fetch ("title", "s", title->title)) \
   fetch ("in", "s", title->title); \
	return 0; \
}

A1(float, )
A2(float,"f", )

A1(double,_g)
A2(double,"g",_g)
