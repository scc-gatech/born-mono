/* Parameter handling. */
/*
  Copyright (C) 2004 University of Texas at Austin
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <sys/utsname.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h>

#include "sergey_rsf.h"

/*^*/

static sf_simtab pars;
static char *prog = NULL;
static char *user = NULL;
static char *host = NULL;

void sf_init(int argc,char *argv[]) 
/*< initialize parameter table from command-line arguments >*/
{
    int ic;
    struct passwd* pass;
    struct utsname uhost;
    FILE *fp;
    size_t len;
    char *rsf, sfdoc[PATH_MAX];

    pars = sf_simtab_init (argc);

    /* set prog */
    prog = strrchr(argv[0],'/');
    prog = (NULL == prog)? argv[0]:prog+1;

    /* no pars and input from terminal */
    if (1==argc && isatty(fileno(stdin))) {
	/* selfdoc and exit */
	rsf = getenv("RSFROOT");
	if (NULL != rsf) {
	    (void) snprintf(sfdoc,PATH_MAX,"%s/bin/sfdoc",rsf);
	    execl(sfdoc,sfdoc,prog,NULL);
	} else { /* no RSFROOT, try it anyway */
	    execlp("sfdoc","sfdoc",prog,NULL); 
	}
	sf_error("Could not find sfdoc in $RSFROOT/bin or in $PATH");
    }
	
    /* set user */
    user = getlogin();
    if (NULL == user) {
	pass = getpwuid(geteuid());
	if (NULL == pass) {
	    user = sf_charalloc(1);
	    user[0] = '\0';
	} else {
	    user = pass->pw_name;
	}
    }

    /* set host */
    if (0 > uname (&uhost)) sf_error("%s: Cannot get hostname:",__FILE__);

    len = strlen(uhost.nodename)+1;
    host = sf_charalloc(len);
    memcpy(host,uhost.nodename,len);

    for (ic=1; ic < argc; ic++) {
	if (0 == strncmp(argv[ic],"par=",4)) {
	    fp = fopen(argv[ic]+4,"r");
	    if (NULL == fp) sf_error ("%s: Cannot open par file %s:",
				      __FILE__,argv[ic]+4);
	    sf_simtab_input(pars,fp);
	    (void) fclose (fp);
	} else {
	    sf_simtab_put(pars,argv[ic]);
	}
    }
}

void sf_parclose (void)
/*< close parameter table and free space >*/
{
    sf_simtab_close (pars);
}

void sf_parout (FILE *file)
/*< write the parameters to a file >*/
{
    sf_simtab_output (pars,file);
}

char* sf_getprog (void) 
/*< returns name of the running program >*/ 
{
    return prog;
}

char* sf_getuser (void) 
/*< returns user name >*/
{
    return user;
}

char* sf_gethost (void) 
/*< returns host name >*/
{
    return host;
}

bool sf_getint (const char* key,/*@out@*/ int* par) 
/*< get an int parameter from the command line >*/
{
    return sf_simtab_getint (pars,key,par);
}

bool sf_getints (const char* key,/*@out@*/ int* par,size_t n) 
/*< get an int array parameter (comma-separated) from the command line >*/
{
    return sf_simtab_getints(pars,key,par,n);
} 

bool sf_getfloat (const char* key,/*@out@*/ float* par) 
/*< get a float parameter from the command line >*/
{
    return sf_simtab_getfloat (pars,key,par);
}

bool sf_getdouble (const char* key,/*@out@*/ double* par) 
/*< get a double parameter from the command line >*/
{
    return sf_simtab_getdouble (pars,key,par);
}

bool sf_getfloats (const char* key,/*@out@*/ float* par,size_t n) 
/*< get a float array parameter from the command line >*/
{
    return sf_simtab_getfloats (pars,key,par,n);
}

char* sf_getstring (const char* key) 
/*< get a string parameter from the command line >*/
{
    return sf_simtab_getstring(pars,key);
}

bool sf_getstrings (const char* key,/*@out@*/ char** par,size_t n) 
/*< get a string array parameter from the command line >*/
{
    return sf_simtab_getstrings(pars,key,par,n);
}

bool sf_getbool (const char* key,/*@out@*/ bool* par)
/*< get a bool parameter from the command line >*/
{
    return sf_simtab_getbool(pars,key,par);
}

bool sf_getbools (const char* key,/*@out@*/ bool* par,size_t n) 
/*< get a bool array parameter from the command line >*/
{
    return sf_simtab_getbools(pars,key,par,n);
} 

sf_simtab sf_getpars (void)
/*< provide access to the parameter table >*/
{
    return pars;
}
 
/* 	$Id: getpar.c 988 2005-02-05 16:59:58Z savap $	 */
