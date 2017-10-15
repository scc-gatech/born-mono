#ifndef RICK_MAIN
#define RICK_MAIN 2
/*
basic definitions
*/
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

/* allocate memory for count objects of a class */
#define NEW(class,var,count)	{\
	extern int _alloc;\
	var=(class)malloc((count)*sizeof(var[0]));\
	_alloc += (count)*sizeof(var[0]);\
	if (var == 0) err ("cant allocate %d bytes for var; %d already allocated",(count)*sizeof(var[0]),_alloc);\
	if (memwatch) printf ("malloc %p=%ld\n",var,(long) ((count)*(int)sizeof(var[0])));\
	}\
/* clear object memory */
#define	FILL(var,size,val)		{register byte bp, be; for (bp=(byte)var, be=bp+(size)*sizeof(var[0]); bp<be;) *bp++ = val;}
/* release object storage */
#define	FREE(var)		if (var) {free(var); var=0; if (memwatch) printf ("free %s\n","var");}

/* alias message to user interface message */
/* draw or erase choice */
#define	SEP_COMMANDS	2
#define	DRAW	1
#define	ERASE	0
#define	ERASE_ALL	-1
#define	NO_INDEX	-1

/* byte type */
typedef	unsigned char* byte;
typedef byte Buffer;
/* string type */
typedef char string[80];
typedef char Message[256];

/* getpar */
#ifdef SU_DATA
#include<su.h>
#define	GETPARSTRING(name,type,var)	getparstring (name,var)
#define	GETPARFLOAT(name,type,var)	getparfloat (name,var)
#define	GETPARINT(name,type,var)	getparint (name,var)
#define	GetparInit	initargs
#define err             err
#endif
/*#ifdef SEPLIB*/
#include<sepcube.h>
#define GETPARSTRING    getch
#define GETPARFLOAT     getch
#define GETPARINT       getch
#define err		seperr
/*#endif*/

extern int memwatch;

/* maximum number of views */
#define NUM_VIEWS 12

#define DATA_NAXIS  6


/* API */
void MainFirst (void);







#endif
