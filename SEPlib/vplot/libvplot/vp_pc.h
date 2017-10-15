#ifndef _VP_PC_H_
#define _VP_PC_H_
#include <stdio.h>

#define A1(FTYP,SUF) \
typedef struct \
{ \
	FTYP _x0, _y0;		/* origin in inches */ \
	FTYP _xu0, _yu0;	/* value in user units corresponding to x0,y0 */ \
	FTYP _xscl, _yscl;	/* scaling from user units to inches */ \
	FTYP _xold, _yold;	/* old pen position (in inches) */ \
	FTYP _dpos;		/* position in dash sequence (in inches) */ \
	FTYP _ddef[4];		/* definition of current dashes (in inches) */ \
	int _dashon;		/* =0 if not dashed line; =1 if dashed line */ \
	int _pendown;		/* is pen up or down */ \
	FILE *_pltout;		/* output file */ \
	int _xjust, _yjust;	/* current x and y text justification */ \
	int _font, _prec, _ovly;/* current text font, precision, and overlay */ \
} PC ## SUF ;

A1(float, )
A1(double,_g)

#undef A1

/*
 * This structure used to be called just plain "pc", but this is disaster
 * on 4.3bsd where the same name is used in the internal FORTRAN routines.
 * You get no warnings about this from the linker, either! You just get
 * random problems all over the place.
 */

#define A2(SUF) \
extern PC ## SUF vp_pc ## SUF ;

A2( )
A2(_g)

#undef A2
#endif /* _VP_PC_H_ */
