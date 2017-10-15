#ifndef RICKMOVIE_MAIN_H
#define RICKMOVIE_MAIN_H
/*
basic definitions
*/
#include <stdlib.h>
/* allocate memory for count objects of a class */
#define NEW(class,var,count)	{\
	extern int _alloc;\
	var=(class)malloc((count)*sizeof(var[0]));\
	_alloc += (count)*sizeof(var[0]);\
	if (var == 0) err ("cant allocate %d bytes for var; %d already allocated",(count)*sizeof(var[0]),_alloc);\
	if (memwatch) printf ("malloc %s=%lu\n","var",(unsigned long) ((count)*sizeof(var[0])));\
	}\
/* clear object memory */
#define	FILL(var,size,val)		{register byte bp, be; for (bp=(byte)var, be=bp+(size)*sizeof(var[0]); bp<be;) *bp++ = val;}
/* release object storage */
#define	FREE(var)		if (var) {free(var); var=0; if (memwatch) printf ("free %s\n","var");}

/* alias message to user interface message */
/* draw or erase choice */
#define	DRAW	1
#define	ERASE	0
#define	NO_INDEX	-1

/* byte type */
typedef	unsigned char* byte;
typedef byte Buffer;
/* string type */
typedef char string[80];
typedef char Message[256];

/* getpar */
#ifdef SU
#define	GETPARFLOAT(name,type,var)	getparfloat (name,var)
#define	GETPARINT(name,type,var)	getparint (name,var)
#define	GetparInit	initargs
#endif
#ifdef SEPLIB
extern int fetch(char *, char *, void *);
extern int seperr(char *, ... );
#define GETPARSTRING    fetch
#define GETPARFLOAT     fetch
#define GETPARINT       fetch
#define err		seperr
#endif

extern int memwatch;

/* API */
void MainFirst (void);

#endif
