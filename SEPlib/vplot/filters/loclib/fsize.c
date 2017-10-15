/* This routine finds the length of a file and returns its size in number
of bytes. filedes is the file descriptor that is returned by a open()
command or a creat() command. */
#include	<sys/types.h>
#include	<sys/stat.h>
#include 	"../include/extern.h"

int
fsize (int filedes)
/*
 *	returns size of file in bytes
 *
 * modified 1/26/83  S. Levin corrected length and simplfied
 * modified 3/27/83  return -1 if fstat fails.
 */
{
	struct stat buf;
	if(0 != fstat (filedes,&buf)) return(-1);
	return ((int) buf.st_size);
}
