/* Copyright (c) Colorado School of Mines, 1995.*/
/* All rights reserved.                       */

/* VALPKGE: $Revision: 1.1.1.1 $ ; $Date: 2004/03/25 06:37:24 $	*/

/*********************** self documentation **********************/
/************************************************************************* 
VALPKGE - routines to handle variables of type Value

************************************************************************** 
Function Prototypes:
vtoi		cast Value variable as an int
vtol		cast Value variable as a long
vtof		cast Value variable as a float
vtod		cast Value variable as a double
atoval		convert ascii to Value
valtoabs	take absolute value of a Value variable
valcmp		compare Value variables
printfval	printf a Value variable
fprintfval	fprintf a Value variable
scanfval	scanf a Value variable
printftype	printf for the type of a segy header word
int vtoi(register cwp_String type, Value val);
long vtol(register cwp_String type, Value val);
float vtof(register cwp_String type, Value val);
double vtod(register cwp_String type, Value val);
void atoval(cwp_String type, cwp_String keyval, Value *valp);
Value valtoabs(cwp_String type, Value val);
int valcmp(register cwp_String type, Value val1, Value val2);
void printfval(register cwp_String type, Value val);
void fprintfval(FILE *stream, register cwp_String type, Value val);
void scanfval(register cwp_String type, Value *valp);

************************************************************************** 
Notes:
A Value is defined by the following in .../su/include/su.h:

typedef union { * storage for arbitrary type *
	char s[8];
	short h;
	unsigned short u;
	long l;
	unsigned long v;
	int i;
	unsigned int p;
	float f;
	double d;
} Value;

The use of the valpkge routines, as well as the hdrpkge routines,
permits the user to change the definition of the types of the 
various fields of the segy data type, without breaking codes that
look at part or all of these fields.

************************************************************************** 
Authors: CWP: Jack K. Cohen, Shuki Ronen
**************************************************************************/
/**************** end self doc ********************************/
/*Robert G. Clapp	12/9/95			*/
/* Revised to make it independent of SU types (except val) */
#include <sepConfig.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
extern void seperr(const char *, ... );
#include <sulib.h> 

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int vtoi(char  *type, Value val)
_XFUNCPROTOEND
#else
int vtoi( type, val)
char *type;
Value val;
#endif
{
	switch(*type) {
		case 's': return (int) val.s[0];
		case 'h': return (int) val.h;
		case 'u': return (int) val.u;
		case 'l': return (int) val.l;
		case 'v': return (int) val.v;
		case 'i': return       val.i;
		case 'p': return (int) val.p;
		case 'f': return (int) val.f;
		default: fprintf(stderr, "vtoi: unknown type %s", type);
			 return 0;	/* for lint */
	}
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
long vtol(char  *type, Value val)
_XFUNCPROTOEND
#else
long vtol(type,val)
char *type;
Value val;
#endif
{
	switch(*type) {
		case 's': return (long) val.s[0];
		case 'h': return (long) val.h;
		case 'u': return (long) val.u;
		case 'l': return        val.l;
		case 'v': return (long) val.v;
		case 'i': return (long) val.i;
		case 'p': return (long) val.p;
		case 'f': return (long) val.f;
		case 'd': return (long) val.d;
		default: fprintf(stderr, "vtol: unknown type %s", type);
			 return 0L;	/* for lint */
	}
}


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
float vtof(char *type, Value val)
_XFUNCPROTOEND
#else
float vtof(type,val)
char *type;
Value val;
#endif
{
	switch(*type) {
		case 's': return (float) val.s[0];
		case 'h': return (float) val.h;
		case 'u': return (float) val.u;
		case 'l': return (float) val.l;
		case 'v': return (float) val.v;
		case 'i': return (float) val.i;
		case 'p': return (float) val.p;
		case 'f': return         val.f;
		case 'd': return (float) val.d;
		default: fprintf(stderr,"vtof: unknown type %s", type);
			 return 0.0;	/* for lint */
	}
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
double vtod(char *type, Value val)
_XFUNCPROTOEND
#else
double vtod(type,val)
char *type;
Value val;
#endif
{
	switch(*type) {
		case 's': return (double) val.s[0];
		case 'h': return (double) val.h;
		case 'u': return (double) val.u;
		case 'l': return (double) val.l;
		case 'v': return (double) val.v;
		case 'i': return (double) val.i;
		case 'p': return (double) val.p;
		case 'f': return (double) val.f;
		case 'd': return          val.d;
		default: fprintf(stderr,"vtod: unknown type %s", type);
			 return 0.0;	/* for lint */
	}
}


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int valcmp(char  *type, Value val1, Value val2)
_XFUNCPROTOEND
#else
int valcmp(type,val1,val2)
char *type;
Value val1,val2;
#endif 
{
	switch(*type) {
		case 's': return strcmp(val1.s, val2.s);
		case 'h':
			if      ( val1.h < val2.h ) return -1;
			else if ( val1.h > val2.h ) return  1;
			else                        return  0;
		case 'u':
			if      ( val1.u < val2.u ) return -1;
			else if ( val1.u > val2.u ) return  1;
			else                        return  0;
		case 'l':
			if      ( val1.l < val2.l ) return -1;
			else if ( val1.l > val2.l ) return  1;
			else                        return  0;
		case 'v':
			if      ( val1.v < val2.v ) return -1;
			else if ( val1.v > val2.v ) return  1;
			else                        return  0;
		case 'i':
			if      ( val1.i < val2.i ) return -1;
			else if ( val1.i > val2.i ) return  1;
			else                        return  0;
		case 'p':
			if      ( val1.p < val2.p ) return -1;
			else if ( val1.p > val2.p ) return  1;
			else                        return  0;
		case 'f':
			if      ( val1.f < val2.f ) return -1;
			else if ( val1.f > val2.f ) return  1;
			else                        return  0;
		case 'd':
			if      ( val1.d < val2.d ) return -1;
			else if ( val1.d > val2.d ) return  1;
			else                        return  0;
		default: seperr("valcmp: unknown type %s", type);
			 return 0;	/* for lint */
	}
}


#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void printfval(char *type, Value val)
_XFUNCPROTOEND
#else
void printfval(type,val)
char *type;
Value val;
#endif
{
	switch(*type) {
	case 's':
		(void) printf("%s", val.s);
	break;
	case 'h':
		(void) printf("%d", val.h);
	break;
	case 'u':
		(void) printf("%d", val.u);
	break;
	case 'i':
		(void) printf("%d", val.i);
	break;
	case 'p':
		(void) printf("%d", val.p);
	break;
	case 'l':
		(void) printf("%ld", val.l);
	break;
	case 'v':
		(void) printf("%ld", val.v);
	break;
	case 'f':
		(void) printf("%f", val.f);
	break;
	case 'd':
		(void) printf("%f", val.d);
	break;
	default:
		fprintf(stderr, "printfval: unknown type %s", type);
	}

	return;
}



#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void scanfval(char  *type, Value *valp)
_XFUNCPROTOEND
#else
void scanfval(type,valp)
char *type;
Value *valp;
#endif
{
    int rc;
	switch(*type) {
	case 's':
		rc = scanf("%s", valp->s);
	break;
	case 'h':
		rc = scanf("%hd", &(valp->h));
	break;
	case 'u':
		rc = scanf("%hu", &(valp->u));
	break;
	case 'i':
		rc = scanf("%d", &(valp->i));
	break;
	case 'p':
	case 'P':
		rc = scanf("%u", &(valp->p));
	break;
	case 'l':
		rc = scanf("%ld", &(valp->l));
	break;
	case 'v':
		rc = scanf("%ld", &(valp->v));
	break;
	case 'f':
		rc = scanf("%f", &(valp->f));
	break;
	case 'd':
		rc = scanf("%lf", &(valp->d));
	break;
	default:
		fprintf(stderr,"scanfval: unknown type %s", type);
	}

        if(rc < 1)
        {
           perror("scanfval()");
        }

	return;
}
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void puthval(unsigned char *suheader, int index, Value *valp)
_XFUNCPROTOEND
#else
void puthval(suheader, index, valp)
unsigned char *suheader;
int index;
Value *valp;
#endif 
{

        switch(*(hdr[index].type)) {
	case 's': (void) strcpy((char *)suheader + hdr[index].offs, valp->s);  break;
        case 'h': *((short*)  (suheader + hdr[index].offs)) = valp->h; break;
        case 'u': *((unsigned short*) (suheader + hdr[index].offs)) = valp->u; break;
        case 'i': *((int*)   (suheader + hdr[index].offs)) = valp->i;break; 
        case 'p': *((unsigned int*)   (suheader + hdr[index].offs)) = valp->p; break;
        case 'l': *((long*)   (suheader + hdr[index].offs)) = valp->l; break;
        case 'v': *((unsigned long*)  (suheader + hdr[index].offs)) = valp->v; break;
        case 'f': *((float*)  (suheader + hdr[index].offs)) = valp->f; break;
        case 'd': *((double*) (suheader + hdr[index].offs)) = valp->d; break;
        default: seperr("%s: %s: mysterious data type", __FILE__,__LINE__); break;
        }


        return;
}
#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void gethval(unsigned char *suheader, int index, Value *valp)
_XFUNCPROTOEND
#else
void gethval(suheader, index, valp)
unsigned char *suheader;
int index;
Value *valp;
#endif 
{
        switch(*(hdr[index].type)) {
        case 's': (void) strcpy(valp->s, (char *) suheader + hdr[index].offs);  break;
        case 'h': valp->h = *((short*)  (suheader + hdr[index].offs)); 
break;
        case 'u': 
			valp->u = *((unsigned short*) (suheader + hdr[index].offs)); 
					break;
        case 'i': valp->i = *((int*)   (suheader + hdr[index].offs)); 
					break;
        case 'p': valp->p = *((unsigned int*)   (suheader + hdr[index].offs)); break;
        case 'l': valp->l = *((long*)   (suheader + hdr[index].offs)); break;
        case 'v': valp->v = *((unsigned long*)  (suheader + hdr[index].offs)); break;
        case 'f': valp->f = *((float*)  (suheader + hdr[index].offs)); break;
        case 'd': valp->d = *((double*) (suheader + hdr[index].offs)); break;
        default: fprintf(stderr,"%s: %lu: mysterious data type", __FILE__,(unsigned long) __LINE__); break;
        }

        return;
}

/*  $Id: valpkge.c,v 1.1.1.1 2004/03/25 06:37:24 cvs Exp $ */
