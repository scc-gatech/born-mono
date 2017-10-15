/*
 * Author: Dan Rothman  and Stewart A. Levin   10/10/83
 * Revised: Stewart A. Levin   10/27/83    Moved par=xxx to second field
 *					   of fake argv.
 * Revised: Stewart A. Levin   1/11/86     Temporary fixup prior to installing
 *					   new fast getpar.
 * Revised: Stewart A. Levin   9/7/87	   <varargs> for portability
 * Modified 6/26/89  Dave Nichols 
 *	   pull in fastpar.h to get correct definition of MIXED union
 * revised  9-16-90  dave   made ansi-c and posix compatible
 * Revised: dave 9/17/90  Use stdarg for ANSI-C compilers
 */
#include <stdio.h>

#include <string.h>
#include "fastpar.h"
#include "../include/prototypes.h"
#include "../include/extern.h"


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
#include <stdarg.h>
_XFUNCPROTOEND
#else
#include <varargs.h>
#endif

/*VARARGS2*/
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int getch2( char *str1, char *str2, ... )
_XFUNCPROTOEND
#else
/*VARARGS0*/
int getch2( va_alist /*str1,str2,var,headname*/)
va_dcl
#endif
{
	static char **oldxargv;
	static int count, oldxargc;
	static char parline[256];
	static char *parlist[3];
#if NeedFunctionPrototypes
#else
  char *str1,*str2;
#endif
	char *headname;
	va_list apptr;
/*	union { double *g; float *f; int *i; char *s; } var;*/
        MIXED var;

#if NeedFunctionPrototypes
 	va_start(apptr,str2);
#else
	va_start(apptr);
	str1 = va_arg(apptr,char *);
	str2 = va_arg(apptr,char *);
#endif
	switch(str2[0]) {
	case 'g': var.g = va_arg(apptr,double *); break;
	case 's': var.s = va_arg(apptr,char *); break;
	case 'f': case 'r': var.f = va_arg(apptr,float *); break;
	default: var.i = va_arg(apptr,int *); break;
	}
	headname = va_arg(apptr,char *);
	va_end(apptr);

	oldxargv = sepxargv; oldxargc = sepxargc;

	parlist[0]=sepxargv[0]; parlist[1]= &(parline[0]); parlist[2]=(char *)NULL;
	sepxargv = &(parlist[0]);
	sepxargc = 2;
	(void) sprintf(parline,"par=%s",headname);
	count = getparin( str1, str2, var);

	sepxargv = oldxargv;
	sepxargc = oldxargc;
	return (count);
}
