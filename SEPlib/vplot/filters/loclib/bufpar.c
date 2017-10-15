/*
 *
 *  source file:   ./filters/loclib/bufpar.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 * Chuck Karish		April 29, 1989
 *	Cleaned up comments, minor code cleanup.  Fixed indentation.
 */


/* bufpar.c: get and store getpar-style parameters from a text
 *		buffer.
 *	bufpar_scan:	parse the buffer, and make a hashed list
 *	bufpar:			return a value from the list
 *	bufpar_string_store:	add a {tag,value} pair to the list
 *					-- c. r. karish  18 dec 1986
 */

#if defined(__stdc__) || defined(__STDC__)
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#else
#define NULL 0
#endif
#include "../include/extern.h"
#include "fastpar.h"
#define BUF_QUEUE_SIZE	127
static hash_item 	*buf_queue[BUF_QUEUE_SIZE];

void
#if defined(__stdc__) || defined(__STDC__)
bufpar_scan(char *strbuf, int buflen, int reset)
#else
bufpar_scan (strbuf, buflen, reset)
	char	*strbuf;		/* Text buffer to be scanned */
	int	buflen,			/* Size of strbuf, in bytes */
		reset;			/* Flag: If non-zero, re- */
					/*   initialize the hashed list */
#endif
{
	char	*buffer;
	char	*bptr;
	int	ii;

	/* Zero out the hashed list.  Always do this on the first call.*/
	if (reset)
		memset ((char *) buf_queue, '\0',
			BUF_QUEUE_SIZE * sizeof (hash_item *));

	/* set up the input */
	buffer = alloc (buflen + 3);
	bptr = buffer;
	*bptr++ = '\n';
	for (ii = 0; ii < buflen; ii++)
		*bptr++ = *strbuf++;
	*bptr++ = '\n';
	*bptr = '\0';
	getpar_push_input (buffer, 1);

	/* Scan input, and put results into the hashed list. */
	getpar_scan (buf_queue, BUF_QUEUE_SIZE);
}

int
#if defined(__stdc__) || defined(__STDC__)
bufpar (char *tag, char *type, MIXED val)
#else
bufpar (tag, type, val)
	char	*tag,			/* Label for the tuple  */
		*type;			/* Expected data type:  */
					/*	i or d	integer */
					/*	r or f	float   */
					/*	g	double  */
					/*	s	string  */
	MIXED	val;			/* Data for the tuple (array) */
#endif
{
	return (getpar_decode (buf_queue, BUF_QUEUE_SIZE, tag, type, val));
}

void
#if defined(__stdc__) || defined(__STDC__)
bufpar_add_string (char *string)
#else
bufpar_add_string (string)
	char	*string;		/* String (tag=value) to be   */
					/*   added to the hashed list */
#endif
{
	getpar_string_store (buf_queue, BUF_QUEUE_SIZE, string);
}

/* Aliases: for calling from FORTRAN  (UNIX f77 calling conventions) */

void
#if defined(__stdc__) || defined(__STDC__)
bpscan_ (char *strbuf, int *buflen, int *reset)
#else
bpscan_ (strbuf, buflen, reset)
	char	*strbuf;
	int	*buflen,
		*reset;
#endif
{
	bufpar_scan (strbuf, *buflen, *reset);
}

int
#if defined(__stdc__) || defined(__STDC__)
bufpar_ (char *tag, char *type, MIXED val)
#else
bufpar_ (tag, type, val)
	char	*tag, *type;
	MIXED	val;
#endif
{
	return(bufpar (tag, type, val));
}

void
#if defined(__stdc__) || defined(__STDC__)
bpadds_ (char *string)
#else
bpadds_ (string)
	char	*string;
#endif
{
	bufpar_add_string (string);
}

/* End of file bufpar.c */
