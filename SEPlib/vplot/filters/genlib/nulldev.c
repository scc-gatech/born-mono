/*
 * Copyright 1987 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in
 * the documentation. It authorizes you to use this file for any
 * non-commercial purpose, provided that this copyright notice is not
 * removed and that any modifications made to this file are commented
 * and dated in the style of my example below.
 */

/*
 *
 *  source file:   ./filters/genlib/nulldev.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Stewart A. Levin (SEP), February 22, 2015
 *      Created separate versions of nulldev for each entry in struct device.
 */
#include "../../include/vplot.h"
#include "../include/extern.h"
/*
 * All purpose Do-nothing generic subroutine
 */

void nulldev (void)
{
    return;
}
void nullopen(void)
{
    return;
}
void nullreset(void)
{
    return;
}
/*ARGSUSED*/
void nullmessage(int a1, char *a2)
{
    return;
}
/*ARGSUSED*/
void nullerase(int a1)
{
    return;
}
/*ARGSUSED*/
void nullclose(int a1)
{
    return;
}
/*ARGSUSED*/
void nullvector(int a1, int a2, int a3, int a4, int a5, int a6)
{
    return;
}
/*ARGSUSED*/
void nullmarker(int a1, int a2, int a3, int *a4)
{
    return;
}
/*ARGSUSED*/
void nulltext(char *a1, float a2, float a3, float a4, float a5)
{
    return;
}
/*ARGSUSED*/
void nullarea(int a1, struct vertex *a2)
{
    return;
}
/*ARGSUSED*/
void nullraster(int a1, int a2, int a3, int a4, int a5, int a6,
                unsigned short *a7, int a8, int a9, int a10)
{
    return;
}
/*ARGSUSED*/
void nullpoint(int a1, int a2)
{
    return;
}
/*ARGSUSED*/
int nullgetpoint(int *a1, int *a2)
{
    return 0; /* no point retrieved */
}
/*ARGSUSED*/
int nullinteract(int a1, FILE *a2, char *a3)
{
    return 0; /* DOVPLOT_CONTINUE */
}
/*ARGSUSED*/
void nullplot(int a1, int a2, int a3)
{
    return;
}
/*ARGSUSED*/
void nullstartpoly(int a1)
{
    return;
}
/*ARGSUSED*/
void nullmidpoly(int a1, int a2)
{
    return;
}
/*ARGSUSED*/
void nullendpoly(int a1)
{
    return;
}
/*ARGSUSED*/
int nullattributes(int a1,int a2,int a3,int a4,int a5)
{
    return 0;
}
