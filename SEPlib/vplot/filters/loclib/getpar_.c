/*
 *
 *  source file:   ./filters/loclib/getpar_.c
 *
 * Joe Dellinger (SEP), June 11 1987
 *	Inserted this sample edit history entry.
 *	Please log any further modifications made to this file:
 */

/*
clayton		1981	wrote getpar
hale		1982	?  INPAR?
JFC		1-22-83	stdin copied to stdout and parsed first.
ron		1-28-83 allow for multiple names of a variable
ron		2-1-83  does not count entries where there is no
			argument as being found
ron		2-2-83  changed the stdin read from character by character
			to line by line
ron		2-4-83	altered the stdin read to not keep lines starting
			with '#'
JFC		2-7-83  hetch introduced
ron		2-8-83  split up the text file of getpar and fetch
ron		2-15-83 added count_found so that the routines in fetch and
			getpar would use the same found.
ron		2-23-83 added save_get and getm_getsav so that only the last
			parameter is used.
ron		2-25-83 fixed getpar so that it can do recursive par= in
			par files. The limit in depth of the calls is
			MAXPARLEVEL.
stew		3-5-83  defined getpar() as alias for getpar_()
stew		7-14-83 added err and perror diagnostics
stew		1-8-86  compatibility with fast getpar
joe		2-25-88 Don't repeat code between here and getpar
*/

#define GETPARSYNONYM getpar_
#include "getpar.c"
