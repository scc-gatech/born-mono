/*<
fixnull

DESCRIPTION
Adds '\0' at end of literal strings inside subroutine/function calls 

USAGE
fixnull <input  >output

CATEGORY
Tools

Compile Level
Local
>*/

/*
KEYWORDS
null string fortran C


*/
/* ***************************************************************** */
/*                          Fixnull                                  */
/*	Pgmr: W. Bauske			Date: March 19, 1991	     */
/*								     */
/*	Purpose: Reads stdin and adds '\0' to the end of all 	     */
/*	literal strings that occur inside of subroutine or function  */
/*	calls							     */
/*								     */
/*	Modified:						     */
/*	03-30-91  W. Bauske IBM					     */
/*	Added ability to toss out lines that start with a '#'        */
/*	Added ability to handle comments that start with a '!'       */
/*	Added ability to strip off inline comments starting with '!' */
/*	This has a flaw in that if a literal contains a '!' the      */
/*	code lops off the line in the wrong place.                   */
/*	Allow lines gt 80 characters long for input                  */
/*	Bypass checking of lines starting with 'char' since data     */
/*	statements for character variables can screw up the code     */
/*	04-19-91 W. Bauske IBM					     */
/*	plugged the in-line comment loop hole so it checks if the    */
/*	'!' is in quotes					     */
/*								     */
/*								     */
/* ***************************************************************** */

#include <stdio.h>
#include <string.h>
#include <macros.h>
#define MAXCCS 800
#define TABINC 8

char nxchar(char * card);

main(argc,argv)
int argc;
char * argv[];
{
	char card[MAXCCS+1], token[MAXCCS+1],
	     ocard[2*MAXCCS+1], lastchar;
	char *cformat="format", *cread="read", *cwrite="write",
	     *cchar="char";
	int  i, c, icol, pflag, qflag, skpflag, flag, cloc, oloc;
/*
 *				Initialize our state
 *				pflag tracks paren sets
 *				qflag tracks quote sets
 *				skpflag tracks continues on
 *				non-processible statements
*/
	pflag=0;
	qflag=0;
	skpflag=0;

/*
 *				scan each card
 */
	while(fgets(card,MAXCCS,stdin)!=NULL)
	{
/*
 *				strip off ending inline comment, if any
 */
		flag=0;
		for(i=strlen(card)-1;i>0;i--)
		{
			if(card[i] == '\'')
				flag++;
			if(card[i] == '!' && flag%2==0)
			{
				card[i]='\n';
				card[i+1]='\0';
				i=0;
			}
		}
/*
 *				check for non-processible
 *				statements using FORTRAN
 *				conventions
 */
		if(card[0] == '#')	/* pre-processor junk */
			;
		else if(lower(card[0]) == 'c' || card[0] == '!')
		{
			card[0]='c';
			fputs(card,stdout);
		}
		else if(card[0] != '\t' &&
			card[5] != ' ' && skpflag)
			cardout(card,stdout);
		else
		{		/* check first non-white space */
			icol=gettok(&card[6],token);
			if(!strncmp(token,cread,4) ||
	 		   !strncmp(token,cchar,4) ||
	 		   !strncmp(token,cwrite,5) ||
		   	   !strncmp(token,cformat,6) )
		   	{
				cardout(card,stdout);
				skpflag=1;
			}
			else
			{
/*
 *				couldn't skip it, scan for literals
 */
			oloc=0;
			skpflag=0;
			lastchar=' ';
			for(cloc=0;(c=card[cloc++])!='\0';)
			switch(c)
			{
			case '(':	/* count if not in a literal */ 
				   if(!qflag)pflag++;
				   ocard[oloc++]=c;
				   lastchar=c;
				   break;
			case '\n':
				   pflag=qflag=0;
				   ocard[oloc++]=c; 
				   lastchar=c;
				   break;
			case ')':	/* decrement if not in literal */ 
				   if(!qflag)pflag--; 
				   ocard[oloc++]=c; 
				   lastchar=c;
				   break;
			case '\t': 
				   ocard[oloc++]=' ';
				   for(;oloc%TABINC!=0;oloc++)
					ocard[oloc]=' ';
				   break;
			case '\'':
				   if(card[cloc] == '\'') /* skip '' */ 
				   {
					ocard[oloc++]='\'';
					ocard[oloc++]='\'';
					cloc++;
				   }
/*
 *					add NULL only if not there yet!
 */
				   else if(pflag && qflag &&
					   (nxchar(&card[cloc])==',' ||
					    nxchar(&card[cloc])==')'))
					{
					 qflag=0;
					 if(card[cloc-3] != '\\'
					    || card[cloc-2] != '0')
					   {
					     ocard[oloc++]='\\';
					     ocard[oloc++]='0';
					   }
					   ocard[oloc++]=c; 
					}
				   else if(pflag &&
					   (lastchar==',' ||
					    lastchar=='('))
					{
					 qflag=1;
					 ocard[oloc++]=c;
					}
				   else
					ocard[oloc++]=c;
				   lastchar=c;
				   break;
			case ' ':  ocard[oloc++]=c;
				   break;
			default:   ocard[oloc++]=c; 
				   lastchar=c;
			}	
			ocard[oloc]='\0';
			cardout(ocard,stdout);
			}
		}
	}
}
/*       1         2         3         4         5         6         7
1234567890123456789012345678901234567890123456789012345678901234567890*/

char nxchar(char *card)
{
	char *cc;
/*
 *					find next non-whitespace character
 */
        cc=card;
	while(*cc == ' ' || *cc == '\t') cc++;
	return(*cc);
}
 
int gettok(char *card, char *token)
{
	int icol;
	char *cc, *tt;
/*
 *					find first non-whitespace string
 */
	tt=token;
        cc=card;
	icol=0;
	while(*cc == ' ' || *cc == '\t')
	{
		icol++;
		cc++;
	}
	while(*cc != ' ' && *cc != '\t' && *cc != '\0')
	{
		*tt++ = *cc++;
	}
	*tt='\0';
	return(icol);
}

int lower(int c) /* convert to lower case for ASCII only */
{
	if(c >= 'A' && c <= 'Z')
		return(c + 'a' - 'A');
	else
		return(c);
}

int cardout(char *card, FILE *fdout)
{
/*
 *					Output modified source code
 *					Split lines gt 72 chars
 */
	int idx,lentot,lencpy;
	char ocard[81];
	char *cont="     *", *cblanks="        ";

	lentot=strlen(card);
	if(lentot <= 73)
	   fputs(card,fdout);
	else if(lentot < 81 &&
		!strncmp(&card[72],cblanks,8))
		fputs(card,fdout);
	else 
	{
		strncpy(ocard,card,72);
		ocard[72]='\n';
		ocard[73]='\0';
		fputs(ocard,fdout);
		lentot=lentot-72;
		idx=72;
		while(lentot > 0)
		{
			strncpy(ocard,cont,6);
			lencpy=min(lentot,66);
			strncpy(&ocard[6],&card[idx],lencpy);
			idx=idx+lencpy;
			lentot=lentot-66;
			if(lentot > 0)
			{
				strncpy(&ocard[72],cblanks,8);
				ocard[80]='\n';
				ocard[81]='\0';
			}
			else
				ocard[6+lencpy]='\0';
			fputs(ocard,fdout);
		}
	}
}
