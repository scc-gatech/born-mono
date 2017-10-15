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
stew		5-4-84  fixed bug that "grepped" for parameters within
			quoted strings in par files.  Pointer was not
			being pushed in parser.  Also doubled MAXPARLEVEL
			at pete's request.
joe		9-18-96 Added new field '1' for yes-no getpars
Dave Nichols    6-26-89 pull in fastpar.h to get correct definition of MIXED
Dave Nichols    8-22-89 always pass getp_getval unions not particular types
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "fastpar.h"
extern void seperr(const char *, ... );
/*#define MIXED    union { char *s; int *i; float *f; double *g }*/
#define MAXLINE 256
#define MAXNAMES 20
#define MAXLENGTH 30
#define MAXPARLEVEL 6

static int ac;
static char **av;
static char name_list[MAXNAMES][MAXLENGTH];
static char saved_param[MAXLINE];
static char par_level = 0;

static int break_names(char *);
static void save_get(char *, int);
static int is_empty(char *);
static int getp_same(char *,char *);
static int getp_getval(char *str,MIXED ptr,char type,int INPAR);
static int getp_neq(char,char *);
static int getm_same(char *,int);
static int count_found(int);
static int parser(char *,MIXED,char[MAXLINE],int,int);

int getparin(char *name, char *type, MIXED ptr)
{
	int INPAR, numnames;
	char line[MAXLINE], fname[64];
	FILE *file;
	MIXED un_fname;

	if( (ac= sepxargc)==0 || (av= sepxargv)== NULL)
		seperr("getpar() sepxargc or sepxargv not initialized\n");
	INPAR = 0;
	/* break the parameter names in name into a list */
	numnames = break_names(name);
	while(--ac>0) {
		av++;
		if(getm_same(*av, numnames)) {
			if (is_empty(*av)) {
				count_found(0);
			}
			else {
				count_found(1);
				save_get(*av, INPAR);
			}
		}
		else if (getp_same("par",*av)) {
			INPAR = 1;
			un_fname.s = fname;
			getp_getval(*av,un_fname,'s',INPAR);
			if( (file=fopen(fname,"r"))==NULL) {
				perror("getpar()");
				seperr("getpar() cannot open parameter file %s\n",fname);
			}
			while( fgets(line,MAXLINE,file) != NULL ) {
				count_found(parser(type, ptr, line, numnames, INPAR));
			}

			fclose(file);
			INPAR = 0;
		}
	}
	if (count_found(-2)) {
		getm_getsav(ptr,*type,INPAR);
	}
	return(count_found(-1));
}

void getm_getsav(MIXED ptr, char type, int INPAR)
{
	getp_getval(saved_param, ptr, type, INPAR);
}

static int parser(char *type,MIXED ptr,char line[MAXLINE],int numnames,int INPAR)
{
	char *pl, t1, t2, fname[64];
	FILE *file;
	MIXED un_fname;

	++par_level;
	pl= line;
	if(*pl=='#') goto bottom; /* comment line */
	/* loop over entries on each line */
loop:	
	while(*pl==' ' || *pl=='\t') pl++;
	if(*pl=='\0'|| *pl=='\n') goto bottom;
	if (getm_same(pl, numnames)) {
		if (is_empty(pl)) {
			count_found(0);
		}
		else {
			count_found(1);
			save_get(pl, INPAR);
		}
	}
	else if (getp_same("par",pl)) {
		if (par_level < MAXPARLEVEL) {
			un_fname.s = fname;
			getp_getval(pl,un_fname,'s',INPAR);
			if((file=fopen(fname,"r"))==NULL) {
				perror("getpar()");
				seperr("getpar() can't open parameter file %s\n",fname);
			}
			while( fgets(line,MAXLINE,file) != NULL ) {
				count_found(parser(type, ptr, line, numnames, INPAR));
			}

			fclose(file);
		}
		else {
			seperr("getpar() can't handle the recursive par file in %s\n",fname);
		}
	}
/*****************
	if(*pl=='"' || *pl=='\'') { 
		t1= t2= *pl++; 
	}
	else { 
		t1= ' '; 
		t2= '\t'; 
	}
	while(*pl!=t1 && *pl!=t2 && *pl!='\n'
	    && *pl!='\0') pl++;
	if(*pl=='"' || *pl=='\'') pl++;
*******************/
	t1 = ' '; t2= '\t';
	while(*pl!=t1 && *pl!=t2 && *pl!='\n' && *pl!='\0') {
		if((*pl) == '"' || (*pl) == '\'') { t1= t2= *pl; }
		if((*pl) == '\\') pl++;
		pl++; 
		}
	if(*pl=='"' || *pl=='\'') pl++;
	goto loop;
bottom:	
	--par_level;
	return(count_found(-1));
}
/* This will be the end of parser. */


static int getp_same(char *s1,char *s2)
{
	while(*s1) if((*(s1++)) != (*(s2++))) return(0);
	if(*s2=='=' || *s2=='[' || *s2=='(') return(1);
	return(0);
}

static int getm_same(char *s1, int numnames)
{
	int ii;

	ii = 0;
	while(getp_same(name_list[ii], s1) == 0 && ++ii <= numnames);
	return(numnames + 1 - ii);
}

static int getp_getval(char *str,MIXED ptr,char type,int INPAR)
{
	register char *sptr;
	register int index, endindex;
	char t1, t2;
	float flt;
	double dubble;
	int integer, count;

	while(getp_neq(*str,"=[(")) str++;
	if (!isalnum(*(str + 1)) && !ispunct(*(str + 1))) {
		return(0);
	}
	index=0;
	if(*str=='(' || *str=='[') {
		str++;
		index= atoi(str);
		while(getp_neq(*str,")]")) str++;
		if(*str=='\0') return(0);
		if(*str==')') index--;
		if(index<0) seperr("getpar() invalid array index %d\n",
			(*str==')'?index+1:index));
		str++;
	}
	if(*str=='=') str++; 
	else return(0);
	if (!isalnum(*str) && !ispunct(*str)) return(0);
loop:
	endindex= index+1;
	if(type != 's') {
		sptr= str;
		while(getp_neq(*sptr,"*x\t ,")) sptr++;
		if(*sptr=='*' || *sptr=='x') {
			count= atoi(str);
			endindex= index+count;
			str= sptr+1;
		}
	}
	switch(type) {
	case 'd':
	case 'i':
		integer= atoi(str);
		while(index<endindex) ptr.i[index++]= integer;
		break;
	case '1':
		if (str[0] == 'y' || str[0] == '1' || str[0] == 'Y')
			integer = 1;
		else
			integer = 0;
		while(index<endindex) ptr.i[index++]= integer;
		break;
	case 'f':
	case 'r':
		flt= (float) atof(str);
		while(index<endindex) ptr.f[index++]= flt;
		break;
	case 'g':
		dubble= atof(str);
		while(index<endindex) ptr.g[index++]= dubble;
		break;
	case 's':
		sptr= ptr.s;
		if(*str=='"' || *str=='\'') { 
			t1= t2= *str++; 
		}
		else { 
			t1= ' '; 
			t2= '\t'; 
		}
		if (!INPAR)
			t1 = t2 = '\0';
		while(*str!=t1 && *str!=t2 && *str!='\0' && *str!='\n')
			*sptr++ = *str++;
		*sptr= '\0';
		break;
	default:
		seperr("getpar() unknown conversion type %c\n",type);
	}
	while(getp_neq(*str,"\t\n ,")) str++;
	if(*str==',')
	{
		str++; 
		goto loop;
	}
	return(1);
}

static int getp_neq(char c,char *s)
{
	do {
		if(*s == c) {
			return(0); 
		}
	} 
	while(*s++);
	return(1);
}

static int break_names(char *names)
{
	char *namp;
	int kk, num;

	namp = names;
	kk = 0;
	num = 0;
	while (*namp != '\0' && num < MAXNAMES) {
		switch (*namp) {
		case ' ':
		case ',':
		case ';':
		case ':':
			if (kk) {
				++num;
				kk = 0;
			}
			break;
		default:
			if (kk < (MAXLENGTH - 1)) {
				name_list[num][kk++] = *namp;
				name_list[num][kk] = '\0';
			}
			break;
		}
		++namp;
	}
	return(num);
}

/* The next routine is the counting routine for hetch and
getch.  When the argument is positive, found is incremented
by the value in which.  When the argument is zero, found
is set to zero.  If the argument is -2, found is returned
unchanged. If the argument is -1, found is returned and also
set to zero.  In each case, the value of found is returned.
*/
static int count_found(int which)
{
	static int found;
	int temp;

	if (which > 0) {
		return(found += which);
	}
	else if (which == 0) {
		return(found = 0);
	}
	else if (which == -1) {
		temp = found;
		found = 0;
		return(temp);
	}
	else {
		return(found);
	}
}

/* This routine saves the argument stored in the array str into
the array saved_param.
*/
static void save_get(char *str, int INPAR)
{
	char *ps, t1, t2;

	ps = saved_param;
	t1 = ' ';
	t2 = '\t';
	while (*str != t1 && *str != t2 && *str != '\n' && *str != '\0') {
		if (*str == '"' || *str == '\'') {
			t1 = t2 = *str;
		}
		if (!INPAR)
			t1 = t2 = '\0';
		*ps++ = *str++;
	}
	if (*str == '"' || *str == '\'') {
		*ps = *str;
	}
	*ps = '\0';
}

/* The next routine determines whether the next argument is empty or not.
If the argument is empty, it returns a 1. If the argument is present, it
returns a 0. str is the pointer to the argument.
*/
static int is_empty(char *str)
{
	while (*str++ != '=');
	if (!isalnum(*str) && !ispunct(*str)) {
		return(1);
	}
	return(0);
}
