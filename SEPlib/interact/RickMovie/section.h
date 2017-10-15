#ifndef RICKMOVIE_SECTION_H
#define RICKMOVIE_SECTION_H
/*
section object definition
*/
#ifndef MOVIE_SECTION
#include "main.h"
#include "data.h"
#include "map.h"

#ifdef __cplusplus /* if C++, specify external linkage to C functions */
extern "C" {
#define EGSTERN
#else
#define EGSTERN extern
#endif
EGSTERN void PlotFrontContour (void);
EGSTERN void PlotSideContour (void);
EGSTERN void PlotTopContour (void);
EGSTERN void PlotFrontWiggle (void);
EGSTERN void PlotSideWiggle (void);
EGSTERN void PlotTopWiggle (void);
EGSTERN void PlotDeepProfile (void);
EGSTERN void PlotAcrossProfile (void);
EGSTERN void PlotDownProfile (void);
EGSTERN void PrintFrontContour (void);
EGSTERN void PrintSideContour (void);
EGSTERN void PrintTopContour (void);
EGSTERN void PrintFrontWiggle (void);
EGSTERN void PrintSideWiggle (void);
EGSTERN void PrintTopWiggle (void);
EGSTERN void PrintDeepProfile (void);
EGSTERN void PrintAcrossProfile (void);
EGSTERN void PrintDownProfile (void);
EGSTERN void SaveFront (char *filename);
EGSTERN void SaveSide (char *filename);
EGSTERN void SaveTop (char *filename);
EGSTERN void SaveTop (char *filename);
EGSTERN void SaveAcross (char *filename);
EGSTERN void SaveDown (char *filename);
EGSTERN void SaveDeep (char *filename);
EGSTERN void PlotPlane (int across,int down,int deep,char *program,char *file);
EGSTERN void PlotProfile (int across,int down,int deep,char *program,char *file);
EGSTERN void PlotExtractPlane (Map hmap,Map vmap,Map zmap,Map map4,Map map5,float *buffer,int n1,int n2,int hdir,int vdir);
EGSTERN void PlotExtractProfile (Map hmap,Map vmap,Map zmap,Map map4,Map map5,float *buffer,int n3,int zdir);
EGSTERN void	SectionPlane (int across , int down , int deep , string program , string file);
EGSTERN void	SectionProfile (int across , int down , int deep , string program , string file);
EGSTERN void	SectionExtractCube (Map hmap , Map vmap , Map zmap , Map map4 , Map map5 , Buffer buffer , int n1 , int n2 , int n3 , int hdir , int vdir , int zdir);
EGSTERN void	SectionExtractPlane (Map hmap , Map vmap , Map zmap , Map map4 , Map map5 , Buffer buffer , int n1 , int n2 , int hdir , int vdir);
EGSTERN void	SectionExtractProfile (Map hmap , Map vmap , Map zmap , Map map4 , Map map5 , Buffer buffer , int n3 , int zdir);
#ifdef __cplusplus /* if C++, end external linkage specification */
}
#endif
#define MOVIE_SECTION
#undef EGSTERN
#endif
#endif
