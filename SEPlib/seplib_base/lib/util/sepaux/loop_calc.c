#include<sepConfig.h>
#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <sepaux.h>

extern int sepwarn(int, const char *, ...);

struct _sep_loop;  /* declare it as a type that can be pointed at */
                                                                                                                     
#define H2C(index,block,n) (((int)(index/block))%n)

                                                                                                                     
struct _sep_loop{
struct _sep_loop  *next; /*next sep3d structure */
struct _sep_loop  *prev; /*previous sep3d structure*/
char name[255]; /*description */
int ndims,iaxis;
int *n,*block;
int nblock;
int loop_n,loop_d;
int nouter,iouter;
};
typedef struct _sep_loop sep_loop;

static sep_loop *seplooplist=0;
static sep_loop *seplooptail=0;

sep_loop *SEPloop_head(void);
void SEPloop_del( sep_loop *curr );
void SEPloop_addend( sep_loop *curr );


_XFUNCPROTOBEGIN
int init_loop_calc(int ndims,int *n,char *string,int max_size)
_XFUNCPROTOEND
{
sep_loop *curr;
double big1,big2,tol;
int iaxis,ifound=0,iblock,i,j;

    curr =SEPloop_head();
    while( curr != 0 ) curr = curr->next;
    curr = (sep_loop*) calloc(1,sizeof(sep_loop));
    strcpy(curr->name,string);
    SEPloop_addend(curr);
    
     if(n[0]>max_size) return(sepwarn(-1,"can't hold the first axis in memory \n"));

    /*find the maximum axis we can hold in memory*/
    iaxis=1; big1=n[0];ifound=0;
    while(iaxis< ndims && ifound==0){
       big1=big1*n[iaxis];
       if(big1 > (double) max_size){ /*can't hold this entire subcube in memory*/
          ifound =1;
          big1=big1/n[iaxis];
          iblock=(int)((double)max_size /big1);
       }
       else iaxis++;
    }
    curr->ndims=ndims;
    curr->iaxis=iaxis;
    curr->n=(int*) malloc(sizeof(int)*ndims);
    curr->block=(int*) malloc(sizeof(int)*ndims);
    curr->loop_d=0;
    curr->loop_n=1;
    curr->nblock=1;
    curr->iouter=0;
    for(i=0;i < ndims; i++) curr->block[i]=1;
    memcpy((void*) curr->n,(const void*) n, sizeof(int)*ndims);
    if(iaxis<ndims){
       curr->loop_n=n[iaxis]; /*size of shared axis*/
       curr->loop_d=0;  /*number we have done*/
       curr->nblock=-1;
       i=iblock;
      
       tol=(double)n[iaxis]*.1;
       while(curr->nblock==-1){
         j=n[iaxis]/i;
         if(n[iaxis]-i*j<=tol) curr->nblock=i;
         i--;
       }
      if(iaxis<ndims-1){ /* we  have outer loops */
        curr->nouter=n[iaxis+1];
        for(i=iaxis+2; i< ndims; i++){
          curr->nouter=curr->nouter*n[i];
          curr->block[i]=curr->block[i-1]*n[i-1];
        }
      }
   } 

  return(0);

}
_XFUNCPROTOBEGIN
int do_sep_loop(char *name, int *nwind, int *fwind)
_XFUNCPROTOEND
{
sep_loop *info;
int found,i;

    info =SEPloop_head(); found=0;
    while( info != 0 && found==0 ){
       if(0==strcmp(info->name,name)) found=1;
         else info = info->next;
    }
    if(found==0) return(-1,sepwarn(-1,"can't find loop %s requested \n",name));

  for(i=0; i < info->ndims;i++){
    fwind[i]=0; nwind[i]=info->n[i];
  }


if(info->iaxis==info->ndims){ /* we can hold  */
  if(info->loop_d==1) return(1) ; /*done*/
  for(i=0; i < info->ndims;i++){
    fwind[i]=0; nwind[i]=info->n[i];
  }
  info->loop_d++;
}
else if(info->iaxis==info->ndims-1){  /*only outer axis*/
   if(info->loop_d==info->loop_n) return(1);
   fwind[info->iaxis]=info->loop_d;
   nwind[info->iaxis]=info->n[info->iaxis]-info->loop_d;
   if(nwind[info->iaxis] > info->nblock) nwind[info->iaxis]=info->nblock;
   info->loop_d+=nwind[info->iaxis];
}
else{
   if(info->loop_d==info->loop_n){
     if (info->iouter==info->nouter-1 ) return(1);
     info->loop_d=0;
     info->iouter++;
   }
   fwind[info->iaxis]=info->loop_d;
   nwind[info->iaxis]=info->n[info->iaxis]-info->loop_d;
   if(nwind[info->iaxis] > info->nblock) nwind[info->iaxis]=info->nblock;
   for(i=info->iaxis+1; i< info->ndims; i++){
     nwind[i]=1;
     fwind[i]=H2C(info->iouter,info->block[i],info->n[i]);
   }
   info->loop_d+=nwind[info->iaxis];
}
return(0);
}


_XFUNCPROTOBEGIN
sep_loop * SEPloop_head(void)
_XFUNCPROTOEND
{ return(seplooplist); }

_XFUNCPROTOBEGIN
void SEPloop_del( sep_loop *curr )
_XFUNCPROTOEND
{
    sep_loop *tmp1,*tmp2;
                                                                                                                     
    tmp1=curr->prev;
    tmp2=curr->next;
                                                                                                                     
    if( curr == seplooplist ) seplooplist = tmp2;
    if( curr == seplooptail ) seplooptail = tmp1;
                                                                                                                     
    if( tmp1 != 0 ) tmp1->next = tmp2;
    if( tmp2 != 0 ) tmp2->prev = tmp1;
                                                                                                                     
}
_XFUNCPROTOBEGIN
void SEPloop_addend( sep_loop *curr )
_XFUNCPROTOEND
{
    if( curr == seplooptail )  return;
                                                                                                                     
    curr->prev=seplooptail;
    if( seplooptail != 0 ) seplooptail->next = curr;
    curr-> next = 0;
    seplooptail=curr;
    if( seplooplist == 0 ) seplooplist = curr;
}
