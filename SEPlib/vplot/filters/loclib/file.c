/*
 *	opens and creates file
 *	arguments:
 *		name	character string containing filename
 *		mode	0 open only
 *			1 create only
 *			2 try open, then create
 *	returns file descriptor
 */

#include <fcntl.h>
#include "../include/extern.h"
extern void seperr(const char *, ... );
#include <unistd.h>

int file (char *name,int mode)
{
	int filedes;
	switch (mode)
	{
	case 0:
		if ((filedes = open (name,0)) ==-1)
			{
			 perror("file()");
			 seperr("file() can't open file %s\n",name);
			}
		break;
	case 2:
		if ((filedes = open (name,2)) ==-1)
		case 1:
			if ((filedes = creat (name,0664)) ==-1)
			{
			 perror("file()");
			 seperr("file() can't create file %s\n",name);
			}
			else
			{
				if (-1 == close (filedes))
				 {
				  perror("file()");
				  seperr("file() unable to close file %s\n",name);
				 }
				if (-1 == (filedes = open (name,2)))
				 {
				  perror("file()");
				  seperr("file() unable to open file %s\n",name);
				 }
			}
	}
	return (filedes);
}
