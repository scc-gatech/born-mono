#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
#include <stdio.h>
#include "rick.h"

int rick_getch(char *name, char *type, void* val){
if(type[0]=='d') return(GETPARINT(name,type,val));
if(type[0]=='f') return(GETPARFLOAT(name,type,val));
if(type[0]=='s') return(GETPARSTRING(name,type,val));
}

int rick_auxgetch( char *name, char *type, void* val, char *extra)
{
char temp_ch[10000];
int ierr;
if(type[0]=='d') {
GETPARINT(name,type,val);
sprintf(temp_ch,"%s%s",name,extra);
return(GETPARINT(temp_ch,type,val));
}
if(type[0]=='f') {
GETPARFLOAT(name,type,val);
sprintf(temp_ch,"%s%s",name,extra);
return(GETPARFLOAT(temp_ch,type,val));
}
if(type[0]=='s') {
GETPARSTRING(name,type,val);
sprintf(temp_ch,"%s%s",name,extra);
return(GETPARSTRING(temp_ch,type,val));
}


}

int rick_auxfetch( char *name, char *type, void* val, char* tag, char *extra)
{
char temp_ch[10000];
int ierr;
sprintf(temp_ch,"%s%s",name,extra);
if(type[0]=='d') ierr=GETPARINT(temp_ch,type,val);
if(type[0]=='f') ierr=GETPARFLOAT(temp_ch,type,val);
#ifdef SEPLIB
if(ierr==0) ierr=auxpar(temp_ch,type,val,tag);
#endif
return(ierr);
}



#endif
