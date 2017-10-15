#include "stdafx.h"
#include <stdarg.h>
#include <signal.h>

//Define some global parameters
bool gVerb = true; 
int  gVerbLevel = 1; //denotes the verb level, the higher the more verby
bool gIsLogging = true;

std::vector<std::string> g_szVecCallStack;
char g_szCallStackTrc[4096];


//
bool equalf(const float& x,const float& y) {
  bool bdivx = fabs(x)>fabs(y)?true:false;
  if (bdivx) {
  if (fabs(x)>0.0f) {
    if (fabs(x)<epsilon) {
      prt("\n The x,y value is very small in equalf, %f,%f! \n",x,y);
    }
    return (fabs((y/x-1))<10*epsilon);
  } else {
    return fabs(x-y)<epsilon;
  }
  } else {
  if (fabs(y)>0.0f) {
    if (fabs(y)<epsilon) {
      prt("\n The x,y value is very small in equalf, %f,%f! \n",x,y);
    }
    return (fabs((x/y-1))<10*epsilon);
  } else {
    return fabs(x-y)<epsilon;
  }
  }
}

float rad2deg(float rad){
  static float ratio = 180.0f / PI;
  return rad*ratio;
}

float deg2rad(float angle){
  static float ratio = PI / 180.0f;
  return angle*ratio;
}

//Print for program output
void verbPrt(const char* format, ...)
{
   if (gVerb) {
	  //fprintf(stderr,"%s:%d;",__FILE__,__LINE__); USELESS, marco __FILE__ needs to expand on-situ
      va_list args;
      va_start (args,format);
      vfprintf (stderr, format, args);
      va_end(args);
   }
   //else do nothing
}

void verbPrt(int verblevel, const char* format, ...)
{
   if (verblevel <= gVerbLevel) { //otherwise not prt it out.
	  //fprintf(stderr,"%s:%d;",__FILE__,__LINE__); USELESS, marco __FILE__ needs to expand on-situ
      va_list args;
      va_start (args,format);
      vfprintf (stderr, format, args);
      va_end(args);
   }
}

void prt(const char* format, ...)
{
	  //fprintf(stderr,"%s:%d;",__FILE__,__LINE__);
      va_list args;
      va_start (args,format);
      vfprintf (stderr, format, args);
      va_end(args); 
}

void errPrt(const char* format, ...)
{
	  //fprintf(stderr,"%s:%d;",__FILE__,__LINE__);
      va_list args;
      va_start (args,format);
      vfprintf (stderr, format, args);
      va_end(args); 
	  	raise(SIGSEGV);
      exit(-1);
}
void dbPrt(const char* format,...){
#ifndef NDEBUG
   //fprintf(stderr,"%s:%d;",__FILE__,__LINE__);
   va_list args;
   va_start (args,format);
   vfprintf (stderr, format, args);
   va_end(args);
#endif
}

//Display the progress bar
void loadProgBar(int x, int n, int r, int w) {
  if (x %(n/r) ) return;
  float ratio = float(x)/(float)n;
  int   c     = int(ratio * float(w));
  
  //show the percentage complete.
  printf("%3d%% [", int(ratio*100.0f));
  //show the load bar
  for (int x=0; x<c; x++) prt("=");
  for (int x=c; x<w; x++) prt(" ");
  //ANSI Control codes to go bavck to the previous line and clear it
  prt("]\n \033[F \033[J"); //ASCII, Escape code.
}

