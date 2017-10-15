/* Function to organize opening of multiple seplib datasets
 *
 * int init_multihdr( argc, argv, pinfo)
 *     int            argc;
 *     char         **argv;
 *     struct _sep **pinfo;
 *
 * Pass in the argument list, you are returned a list of structures
 * as defined in multihdr.h
 *
 * This routine is used in Merge, Add etc.
 *
 * keyword: multiple header merge utility routine
 *
 */
/*
 * Modified: Dave Nichols (Geco)  10/21/94
 *       Changed test for valid stdin to use input() and added call to
 *       clean up if it isn't valid.
 *
 * Modified: Stew Levin (Mobil)  05/08/96
 *       Inlined strdup() to shut up LINUX gcc.
 * Modified: Bob Clapp 5/25/98
 *             Added n4 support
 */

#include<string.h>
#include "sepaux.h"
#include <seplib.h>

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void struct_dump (struct _sep *a)
_XFUNCPROTOEND
#else
void struct_dump (a)
    struct _sep  *a;
#endif
{
int             i;

    if (a[0].standard[0].n == -1)
    {
        fprintf (stderr, "\nname   %s\n", a[0].header);
        fprintf (stderr, "in     %s\n", a[0].in);
        fprintf (stderr, "n1=-1     n3=%d\n", a[0].standard[2].n);

    }else {
        fprintf (stderr, "\nname   %s\n", a[0].header);
        fprintf (stderr, "in     %s\n", a[0].in);
        fprintf (stderr, "esize  %d\n", a[0].esize);
        
        for (i = 0; i < 4; ++i)
            fprintf (stderr, "n%d     %d     o%d   %f      d%d   %f\n",
                     i + 1, a[0].standard[i].n,
                     i + 1, a[0].standard[i].o,
                     i + 1, a[0].standard[i].d);
    }

    return;
}


void process_one();

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int init_multihdr( int argc, char **argv, struct _sep **pinfo)
_XFUNCPROTOEND
#else
int init_multihdr( argc, argv, pinfo)
    int             argc;
    char          **argv ;
    struct _sep  **pinfo;
#endif
{
int  i, nhead, ihead, ierr, ierrnow, stdinok ;
struct _sep  *info;
int n1,n2,n3,n4;
float o1,o2,o3,o4,d1,d2,d3,d4;

    n1 = 0;
    n2 = 0;
    n3 = 0;
    n4 = 0;
    o1 = 0.0;
    o2 = 0.0;
    o3 = 0.0;
    o4 = 0.0;
    d1 = 0.0;
    d2 = 0.0;
    d3 = 0.0;
    d4 = 0.0;

    ihead = 0;
    nhead = 0;
    stdinok = 0;
    
    /* check if stdin points at a regular file */
    if( input() != 0 ) {
        stdinok=1;
        nhead=1;
    } else {
      auxclose("in");
    }

    /* count the number of headers in the argument list */

    for (i = 1; i < argc; ++i){
	if (0 == strchr (argv[i], '=')){
	    nhead++;
	}
    }

    if (nhead == 0) return 0;

    if (nhead > MAXHEAD)
	seperr ("%d %d Sorry, too many input headers to open all at once!\n",nhead,MAXHEAD);

    info = (struct _sep *) alloc (nhead * sizeof (struct _sep));
    *pinfo = info;

    /* extract info from each header */
    ihead = 0;

    if( stdinok ){
	info[ihead].header = (char *) malloc(1 + strlen( "stdin" ));
	if( ((char *) 0) !=	info[ihead].header )
		strcpy(info[ihead].header, "stdin" );
	sprintf(info[ihead].tag,"in");
	process_one( &info[ihead] );
	ihead++;
    }

    for (i = 1; i < argc; ++i)
    {
	if (0 == strchr(argv[i], '='))
	{
            static char  tmpparam[1024];

	    info[ihead].header = argv[i];

	    if( ihead == 0 ){
		/* the first header should replace stdin ( tag == in )*/
	        sprintf(info[ihead].tag,"in");
		sprintf(tmpparam,"stdin=%s",argv[i]);
            }else{
	       /* the rest have tags in2, in3.. */
	       sprintf(info[ihead].tag,"in%d",ihead+1);
	       sprintf(tmpparam,"in%d=%s",ihead+1,argv[i]);
	    }
	    getch_add_string( tmpparam );

	    process_one( &info[ihead] );
            ihead++;
       }

    }


/* Find default values; just take first one that is set. */
/* n1 starts out -2 so error checking can be done; n2, n3 default 1 */

    n1 = -2;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[0].n)
	{
	    n1 = info[ihead].standard[0].n;
	    break;
	}
    }

    n2 = 1;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[1].n)
	{
	    n2 = info[ihead].standard[1].n;
	    break;
	}
    }

    n3 = 1;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[2].n)
	{
	    n3 = info[ihead].standard[2].n;
	    break;
	}
    }
    n4 = 1;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[3].n)
	{
	    n4 = info[ihead].standard[3].n;
	    break;
	}
    }


   /* d's default 1. */
    d1 = 1.;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[0].d)
	{
	    d1 = info[ihead].standard[0].d;
	    break;
	}
    }

    d2 = 1.;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[1].d)
	{
	    d2 = info[ihead].standard[1].d;
	    break;
	}
    }

    d3 = 1.;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[2].d)
	{
	    d3 = info[ihead].standard[2].d;
	    break;
	}
    }
    d4 = 1.;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[3].d)
	{
	    d4 = info[ihead].standard[3].d;
	    break;
	}
    }




   /* o's default 0. */
    o1 = 0.;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[0].o)
	{
	    o1 = info[ihead].standard[0].o;
	    break;
	}
    }
    o2 = 0.;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[1].o)
	{
	    o2 = info[ihead].standard[1].o;
	    break;
	}
    }
    o3 = 0.;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[2].o)
	{
	    o3 = info[ihead].standard[2].o;
	    break;
	}
    }

    o4 = 0.;
    for (ihead = 0; ihead < nhead; ihead++)
    {
	if (info[ihead].set[3].o)
	{
	    o4 = info[ihead].standard[3].o;
	    break;
	}
    }


    if (n1 < 0 || n2 < 0 || n3 < 0 || n4 < 0)
	seperr ("Need to know n1, n2, n3, and n4 (or at least just n1)\n");

    /* fill in default values for blank headers */
    for (ihead = 0; ihead < nhead; ihead++)
    {
	/* If n1 not given, take default but complain */
	if (0 == info[ihead].set[0].n)
	{
	    info[ihead].standard[0].n = n1;
	    fprintf (stderr, "%s: Guessing n1=%d for header %s\n", argv[0], n1, info[ihead].header);
	}
	/* If n2 not given, take default but complain unless n2=1 */
	if (0 == info[ihead].set[1].n)
	{
	    info[ihead].standard[1].n = n2;
	    if (n2 != 1)
		fprintf (stderr, "%s: Guessing n2=%d for header %s\n", argv[0], n2, info[ihead].header);
	}

	/* If n3 not given, take default but complain unless n3=1 */
	if (0 == info[ihead].set[2].n)
	{
	    info[ihead].standard[2].n = n3;
	    if (n3 != 1)
		fprintf (stderr, "%s: Guessing n3=%d for header %s\n", argv[0], n3, info[ihead].header);
	}

	/* If n4 not given, take default but complain unless n4=1 */
	if (0 == info[ihead].set[3].n)
	{
	    info[ihead].standard[3].n = n4;
	    if (n4 != 1)
		fprintf (stderr, "%s: Guessing n4=%d for header %s\n", argv[0], n4, info[ihead].header);
	}

	if (0 == info[ihead].set[0].d)
	    info[ihead].standard[0].d = d1;
	if (0 == info[ihead].set[1].d)
	    info[ihead].standard[1].d = d2;
	if (0 == info[ihead].set[2].d)
	    info[ihead].standard[2].d = d3;
	if (0 == info[ihead].set[3].d)
	    info[ihead].standard[3].d = d4;

	if (0 == info[ihead].set[0].o)
	    info[ihead].standard[0].o = o1;
	if (0 == info[ihead].set[1].o)
	    info[ihead].standard[1].o = o2;
	if (0 == info[ihead].set[2].o)
	    info[ihead].standard[2].o = o3;
	if (0 == info[ihead].set[3].o)
	    info[ihead].standard[3].o = o4;


    }

    return nhead;
}

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
void process_one( struct _sep *one_info  )
_XFUNCPROTOEND
#else
void process_one( one_info  )
struct _sep *one_info;
#endif
{

    one_info->esize = 4;
    one_info->transp = 0;
    (void) strcpy (one_info->in, "");
    one_info->standard[0].n = 0;
    one_info->standard[1].n = 0;
    one_info->standard[2].n = 0;
    one_info->standard[3].n = 0;
    one_info->standard[0].o = 0.0;
    one_info->standard[1].o = 0.0;
    one_info->standard[2].o = 0.0;
    one_info->standard[3].o = 0.0;
    one_info->standard[0].d = 0.0;
    one_info->standard[1].d = 0.0;
    one_info->standard[2].d = 0.0;
    one_info->standard[3].d = 0.0;

    auxpar ("esize", "d", &(one_info->esize), one_info->tag);

    if (!auxpar ("in", "s", one_info->in, one_info->tag))
	seperr ("%s is not a header file!\n", one_info->header);

/*
 * Read in all the n's, d's, o's, and record whether they were set or not.
 */
    one_info->set[0].n = auxpar ("n1", "d", &(one_info->standard[0].n), one_info->tag);

    one_info->set[1].n = auxpar ("n2", "d", &(one_info->standard[1].n), one_info->tag);

    one_info->set[2].n = auxpar ("n3", "d", &(one_info->standard[2].n), one_info->tag);
    one_info->set[3].n = auxpar ("n4", "d", &(one_info->standard[3].n), one_info->tag);

    one_info->set[0].d = auxpar ("d1", "f", &(one_info->standard[0].d), one_info->tag);

    one_info->set[1].d = auxpar ("d2", "f", &(one_info->standard[1].d), one_info->tag);

    one_info->set[2].d = auxpar ("d3", "f", &(one_info->standard[2].d), one_info->tag);
    one_info->set[3].d = auxpar ("d4", "f", &(one_info->standard[3].d), one_info->tag);


    one_info->set[0].o = auxpar ("o1", "f", &(one_info->standard[0].o), one_info->tag);

    one_info->set[1].o = auxpar ("o2", "f", &(one_info->standard[1].o), one_info->tag);

    one_info->set[2].o = auxpar ("o3", "f", &(one_info->standard[2].o), one_info->tag);
    one_info->set[3].o = auxpar ("o4", "f", &(one_info->standard[3].o), one_info->tag);

}
/*  $Id: multihdr.c,v 1.1.1.1 2004/03/25 06:37:24 cvs Exp $ */
