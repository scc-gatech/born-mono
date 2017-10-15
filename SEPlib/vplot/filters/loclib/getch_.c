/*
 *
 *  source file:   ./filters/loclib/getch.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 *
 * Joe Dellinger Feb 25 1988
 *	No reason to repeat code that getpar already has.
 */

/*
getch_  --   a synonym for getpar
call getch ("variables","s","A variable") # search        command line & par=
call getpar("variables","s","A variable") # search        command line & par=
call fetch ("variables","s","A variable") # search stdin, command line & par=
call hetch ("variables","s","A variable") # search stdin

Version #1 -- (no raw data on pipes)
if ( stdin != keyboard)
	if ( first invocation )
		cp stdin stdout;
		cp stdin stdin_buf
parse stuff (old getpar);

Some examples of how this routine works:
Command			Input		Result
fetch_("nx","d",&nx)	nx=100		nx is set to 100, 1 argument found
fetch_("nx","d",&nx)	nx=		0 arguments found
fetch_("nx nxx","d",&nx) nxx=30		nx is set to 30, 1 argument found
fetch_("nx","d",&nx)	nx=50 nx=60	nx is set to 60, 2 arguments found
fetch_("nt","d",&nx)	nx=80		0 arguments found
fetch_("nt ntt","d", &nt) ntt=30 nt=40	nt is set to 40, 2 arguments found
fetch_("nt ntt","d", &nt) nt=30 ntt=40	nt is set to 40, 2 arguments found
???? Ron- I thought it would be 30 -Jon ???? It ain't.

The input above can be from standard input, the command line, and the
par file.
*/

/* edit history
clayton		1981	wrote getpar
hale		1982	?  INPAR?
JFC		1-22-83	stdin copied to stdout and parsed first.
ron		1-28-83 allow for multiple names of a variable
ron		2-1-83  does not count entries where there is no
			argument as being found
ron		2-2-83  changed the stdin read from character by character
			to line by line
ron		2-4-83	altered the stdin read to not keep lines starting with '#'
JFC		2-7-83  hetch introduced
stew            2-8-83  changed fetch_'s copy operation to write to
			FILE returned by head()
stew		2-9-83  hetch looks for noheader=y keyword and returns if there.
			don't copy past CTL D in hetch.
ron		2-15-83 added count_found so that the routines in fetch and
			getpar would use the same found.
stew		2-17-83 forced hetch_ to unbuffer stdin before copying
ron		2-23-83 added getm_getsav so that fetch call will not affect
			value until the last parameter is found
stew		3-1-83  defined time() in terms of time_t typedef in sys/types.h
stew		3-26-83 call noheader()
stew		7-14-83 added err and perror calls for improved diagnostics
stew		8-8-83  split fortran versions to separate files for linkage
*/

#define GETPARSYNONYM	getch_
#include "getpar.c"
