#ifdef SEP_MPI
/*<

mpi_pass

Usage:
ierr=mpi_pass( arg1, arg2, arg3 )

Return Values




Input Parameters:


Output Parameters:



Description:



>*/ 
/*
-------------------------------------------------

Author: Robert Clapp, ESMB 463, 7230253

Date Created:Mon Dec 11  9:32:54 PST 2000

Purpose: 

*/	 

int max_mem_sep_mpi;
int nproc_sep_mpi;
int mpi_job_num;
int mpi_verb;
int mpi_ndead;
float *mem_buf_mpi;
int *mpi_dead_list;
int mpi_sep_ngroup;
#define MAX_SEP_COM 10


#define DEFAULT_JOB_NUM 1001

#include<sep_par.h>
extern char **sepxargv;
extern int sepxargc;
#include<mpi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

MPI_Comm sep_mpi_comm[MAX_SEP_COM]; 
MPI_Group sep_mpi_group[MAX_SEP_COM];

int mpi_sep_tag_sum_core(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs, int nb, int add,MPI_Comm comm);
int mpi_sep_tag_bcast_core(int ithread_from,char *tag_from, int ithread, char *tag_to, int bs, int nb, MPI_Comm comm);
int mpi_sep_tag_combine_core(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs1, int bs2, int nb, int *send_from,int add,MPI_Comm comm);

#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int mpi_sep_set_dead(int nproc, int *dead_list)
_XFUNCPROTOEND
#else
int mpi_sep_set_dead(nproc, int *dead_list)
int nproc,*dead_list;
#endif
{
int i;
mpi_ndead=nproc;
mpi_dead_list=(int*)malloc(mpi_ndead*sizeof(int));
for (i=0; i < nproc; i++) mpi_dead_list[i]=dead_list[i];
return(0);
}





#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int mpi_sep_send_args(int nproc, int max_mem,int verb) 
_XFUNCPROTOEND
#else
int mpi_sep_send_args(nproc, int max_mem,int verb) 
int nproc;
#endif 
{ 
int iproc,iarg,tempi,ierr;
MPI_Status status;
char stdintag[2048];

mpi_verb=verb;
mpi_job_num=DEFAULT_JOB_NUM;
mpi_ndead=0;
mpi_sep_ngroup=0;


ierr=getch("stdintag","s",stdintag);
if(ierr==1){ 
	 if (freopen(stdintag,"r",stdin)==NULL) 
			seperr("trouble opening stdintag %s \n",stdintag);
}
if(0==getch("head","s",stdintag)){ /*assume stdout isn't being redirected*/
	getch_add_string("noheader=y");
	getch_add_string("head=/dev/null");
	getch_add_string("out=/dev/null");
}

max_mem_sep_mpi=max_mem*1000*1000;
nproc_sep_mpi=nproc;
mem_buf_mpi=(float*) malloc(sizeof(float)*max_mem_sep_mpi);
if(mem_buf_mpi==NULL)
	seperr("trouble allocating local memory\n");


for(iproc=1; iproc < nproc; iproc++){
  MPI_Send( &mpi_verb, 1, MPI_INT, iproc, 0,MPI_COMM_WORLD);
  MPI_Send( &max_mem_sep_mpi, 1, MPI_INT, iproc, 0,MPI_COMM_WORLD);
  MPI_Send( &nproc_sep_mpi, 1, MPI_INT, iproc, 0,MPI_COMM_WORLD);
	MPI_Send( &sepxargc, 1, MPI_INT, iproc, 0,MPI_COMM_WORLD);
	for(iarg=0; iarg< sepxargc; iarg++){
		tempi=1+(int)strlen(sepxargv[iarg]);
		MPI_Send(&tempi,1,MPI_INT, iproc,iarg*2, MPI_COMM_WORLD);
		MPI_Send( sepxargv[iarg],tempi,MPI_CHAR, iproc,
     iarg*2+1, MPI_COMM_WORLD);
	}
}
return (0);
} 
/*copy from tag_from to tag_to nb blocks of size bs */
int mpi_sep_tag_sum_index(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs, int nb, int add,int index){
  return(mpi_sep_tag_sum_core(ithread,tag_from,ithread_to,tag_to,bs,nb,add,sep_mpi_comm[index]));
}



int mpi_sep_tag_sum(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs, int nb, int add){
  return(mpi_sep_tag_sum_core(ithread,tag_from,ithread_to,tag_to,bs,nb,add,MPI_COMM_WORLD));
}



int mpi_sep_tag_sum_core(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs, int nb, int add,MPI_Comm comm){
double tot_copy;
double done;
double temp_sz,temp_b;
int block,ierr;
int j,k,i,i_am_dead;
float *temp_buf,*temp2_buf;
MPI_Status stat;


if(ithread==ithread_to){
	temp_buf=(float*) malloc(sizeof(float)*max_mem_sep_mpi);
	if(temp_buf==NULL) {
		fprintf(stderr,"trouble allocating temporary memory\n");
		return(-1);
	}
}
i_am_dead=0;
for(j=0; j < mpi_ndead;j++){
	if(ithread==mpi_dead_list[j]){
     i_am_dead=1;
     for(i=0; i < max_mem_sep_mpi;i++) mem_buf_mpi[i]=0.;
  }
}


tot_copy=(double)bs*(double)nb;
done=0;
temp_sz=max_mem_sep_mpi;
while(tot_copy > done){
  temp_b=MIN(tot_copy-done,temp_sz);
  block=temp_b;
  if(ithread==ithread_to && mpi_verb==1)
     fprintf(stderr,"working on %d MB to %d MB of %d MB \n",
   (int)(done/256./1024.),(int)((done+block)/256./1024.),
    (int)(tot_copy/256./1024.));
  if(i_am_dead==0){
   ierr=sreed(tag_from,mem_buf_mpi,block*4);
  }
  MPI_Barrier(comm);
  MPI_Reduce(mem_buf_mpi,temp_buf,block,MPI_FLOAT,MPI_SUM,ithread_to,
    comm);

    
   if(add==1 && ithread_to==ithread){
   ierr=sreed(tag_to,mem_buf_mpi,block*4);
   if(block*4!=ierr){
	  fprintf(stderr,"trouble reading from thread %d , tag=%s (read=%d of %d) \n",
     ithread,tag_to,ierr,block*4); 
     return(-1);
	 }
   ierr=sseek(tag_to,-block*4,1);
   for(i=0; i < block; i++) temp_buf[i]+=mem_buf_mpi[i];
   }
  if(ithread==ithread_to){
  	if(block*4!=srite(tag_to,temp_buf,block*4)){
			fprintf(stderr,"trouble writing from thread %d , tag=%s \n",
     	ithread,tag_to); 
     	return(-1);
		}
	}
  done+=(double)block;
}
if(ithread==ithread_to) {free(temp_buf);}

return(0);

}

int mpi_sep_clean()
{
if(mpi_ndead>0) free(mpi_dead_list);
free(mem_buf_mpi);
return(0);
}

int mpi_sep_tag_bcast_index(int ithread_from,char *tag_from, int ithread, char *tag_to, int bs, int nb,int index){

return(mpi_sep_tag_bcast_core(ithread_from,tag_from,ithread,tag_to,bs,nb,sep_mpi_comm[index]));

}
int mpi_sep_tag_bcast(int ithread_from,char *tag_from, int ithread, char *tag_to, int bs, int nb){
return(mpi_sep_tag_bcast_core(ithread_from,tag_from,ithread,tag_to,bs,nb,MPI_COMM_WORLD));
}
int mpi_sep_tag_bcast_core(int ithread_from,char *tag_from, int ithread, char *tag_to, int bs, int nb, MPI_Comm comm){
double tot_copy;
double done;
int block,ierr;
double temp_b,temp_sz;

tot_copy=(double)bs*(double)nb;
done=0;
temp_sz=max_mem_sep_mpi;
while(tot_copy > done){
  
  temp_b=MIN(tot_copy-done,temp_sz);
  block=temp_b;
  if(mpi_verb==1 && ithread_from==ithread) 
     fprintf(stderr,"working on %d MB to %d MB of %d MB \n",
   (int)(done/256./1024.),(int)((done+block)/256./1024.),
    (int)(tot_copy/256./1024.));
  if(ithread==ithread_from) {
   ierr=sreed(tag_from,mem_buf_mpi,block*4);
   if(block*4!=ierr){
			fprintf(stderr,"trouble reading from thread %d , tag=%s. Read=%d of %d\n",
      ithread,tag_from,ierr,block*4); 
      return(-1);
		}
	}
  MPI_Bcast(mem_buf_mpi,block,MPI_FLOAT, ithread_from, comm);
  MPI_Barrier(comm);
  if(block*4!=srite(tag_to,mem_buf_mpi,block*4)){
		fprintf(stderr,"trouble writing from thread %d , tag=%s \n",
     ithread,tag_to); 
     return(-1);
	}
  done+=(double)block;
}

return(0);

}
int mpi_sep_tag_distribute(int ithread_from,char *tag_from, int ithread, char *tag_to, int bs, int nb, int *send_to){
int i,ierr;
int ia, ib;
int block,done;
MPI_Status stat;




/* loop over parts (e.g. frequencies*/
for(i=0; i <  nb; i++){
	done=0;
  if(mpi_verb==1 && ithread_from==ithread) {
     fprintf(stderr,"sending from=%d to=%d part=%d \n",
       ithread_from,send_to[i],i);
     fflush(stderr);
   } 
	while(done< bs){
		block=MIN(bs-done,max_mem_sep_mpi);
		if(ithread==ithread_from){
   		ierr=sreed(tag_from,mem_buf_mpi,block*4);
   		if(block*4!=ierr){
				fprintf(stderr,"trouble reading from thread %d , tag=%s ierr=%d (%d) \n",
      	ithread,tag_from,ierr,block); 
      	return(-1);
			}
		}
		if(send_to[i]!=ithread_from){
       if(ithread==ithread_from){
         MPI_Send(mem_buf_mpi,block,MPI_FLOAT, 
         send_to[i],i+mpi_job_num,MPI_COMM_WORLD);
       }
       else if(send_to[i]==ithread){
         MPI_Recv(mem_buf_mpi,block,MPI_FLOAT,
         ithread_from,i+mpi_job_num,MPI_COMM_WORLD,&stat);
       }
    }
		if(send_to[i]==ithread){
  		if(block*4!=srite(tag_to,mem_buf_mpi,block*4)){
				fprintf(stderr,"trouble writing to thread %d , tag=%s \n",
   		  ithread,tag_to); 
     		return(-1);
      }
    }
    done+=block;
	}
  mpi_job_num+=nb;
}
return(0);
}

int mpi_sep_tag_combine_index(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs, int nb, int *send_from,int add,int index){
return(mpi_sep_tag_combine_core(ithread,tag_from,ithread_to,tag_to,bs,nb,1,send_from,add,sep_mpi_comm[index]));
}

int mpi_sep_tag_combine_big(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs, int bs2, int nb, int *send_from,int add){
return(mpi_sep_tag_combine_core(ithread,tag_from,ithread_to,tag_to,bs,bs2,nb,send_from,add,MPI_COMM_WORLD));
}

int mpi_sep_tag_combine(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs, int nb, int *send_from,int add){
return(mpi_sep_tag_combine_core(ithread,tag_from,ithread_to,tag_to,bs,1,nb,send_from,add,MPI_COMM_WORLD));
}

int mpi_sep_tag_combine_core(int ithread,char *tag_from, int ithread_to, char *tag_to, int bs,int bs2, int nb, int *send_from,int add,MPI_Comm comm){
int i,valid,j,ierr;
int block;
MPI_Status stat;
double  big_num,done,blockd;
float *temp_buf;

big_num=(double)bs*(double)bs2;


if(ithread==ithread_to)
	temp_buf=(float*) malloc(sizeof(float)*max_mem_sep_mpi);

/* loop over parts (e.g. frequencies*/
for(i=0; i <  nb; i++){
	done=0;
  valid=1;for(j=0; j < mpi_ndead;j++){if(send_from[i]==mpi_dead_list[j]) valid=0;}
	while(done< big_num){
		blockd=MIN(big_num-done,(double)max_mem_sep_mpi);
    block=(int)blockd;
    if(valid==1){
		if(send_from[i]==ithread){
  		if(block*4!=sreed(tag_from,mem_buf_mpi,block*4)){
		  fprintf(stderr,"trouble reading from tag thread %d , tag=%s \n",
   		  ithread,tag_to); 
     		return(-1);
      }
		}
		if(send_from[i]!=ithread_to){
       if(ithread!=ithread_to && send_from[i]==ithread) {
         MPI_Send(mem_buf_mpi,block,MPI_FLOAT, 
         ithread_to,i+mpi_job_num,comm);
        }
       else if(ithread==ithread_to){
           if(mpi_verb==1) fprintf(stderr,"receiving %d from %d (%g of %g) \n",i,send_from[i],done,big_num);
          MPI_Recv(mem_buf_mpi,block,MPI_FLOAT,
         send_from[i],i+mpi_job_num,comm,&stat);
        }
    }
    }
    else { /*dead node zero it */
      for(j=0; j < block; j++) mem_buf_mpi[j]=0.;
    }
		if(ithread==ithread_to){
     if(add==1){
   		if(block*4!=sreed(tag_to,temp_buf,block*4)){
				fprintf(stderr,"trouble writing to thread %d , tag=%s \n",
      	ithread,tag_to); 
      	return(-1);
			}
      ierr=sseek(tag_to,-block*4,1);
      for(j=0; j < block; j++) mem_buf_mpi[j]+=temp_buf[j];
    }
   		if(block*4!=srite(tag_to,mem_buf_mpi,block*4)){
				fprintf(stderr,"trouble writing to thread %d , tag=%s \n",
      	ithread,tag_to); 
      	return(-1);
			}
		}
    done+=block;
  mpi_job_num+=1;
	}
}
if(ithread==ithread_to) free(temp_buf);
return(0);
}



#if NeedFunctionPrototypes
_XFUNCPROTOBEGIN
int mpi_sep_receive_args()
_XFUNCPROTOEND
#else
int mpi_sep_receive_args()    
#endif
{
int iproc,iarg;
MPI_Status status;
int narg,nlen,ierr;
char temp_ch[2048],temp2_ch[2049];
char stdintag[2048];

  mpi_sep_ngroup=0;
	MPI_Recv( &mpi_verb, 1, MPI_INT, 0, 0,MPI_COMM_WORLD,&status);
	MPI_Recv( &max_mem_sep_mpi, 1, MPI_INT, 0, 0,MPI_COMM_WORLD,&status);
	MPI_Recv( &nproc_sep_mpi, 1, MPI_INT, 0, 0,MPI_COMM_WORLD,&status);
	MPI_Recv( &narg, 1, MPI_INT, 0, 0,MPI_COMM_WORLD,&status);
	for(iarg=0; iarg< narg; iarg++){
		MPI_Recv( &nlen,1,MPI_INT, 0,iarg*2, MPI_COMM_WORLD,&status);
		MPI_Recv(temp_ch,nlen,MPI_CHAR,0,iarg*2+1,MPI_COMM_WORLD,&status);
		getch_add_string(temp_ch);
	}

mem_buf_mpi=(float*) malloc(sizeof(float)*max_mem_sep_mpi);
if(mem_buf_mpi==NULL)
	seperr("trouble allocating local memory\n");
	
	if(1==getch("stdin","s",stdintag)){
		if (freopen(stdintag,"r",stdin)==NULL) 
			seperr("trouble opening stdintag %s \n",stdintag);
	}

	if(0==getch("head","s",stdintag)){ /*assume stdout isn't being redirected*/
		getch_add_string("noheader=y");
		getch_add_string("head=/dev/null");
		getch_add_string("out=/dev/null");
	}
mpi_job_num=DEFAULT_JOB_NUM;

}





int mpi_sep_open_lock(char *fname){
int fd;

   fd= open(fname,O_EXCL|O_CREAT);
   while(fd==-1){
      sleep(1);
      fd= open(fname,O_EXCL|O_CREAT);
   }
   return(fd);
}
int mpi_sep_check_lock(char *fname){
return(open(fname,O_EXCL|O_CREAT));
}


int mpi_sep_close_lock(char *fname,int fd){
    if(fd!=-1) close(fd);
   unlink(fname);
   return(0);
}
int mpi_sep_create_group(int ngrp, int *use){
  int i;
  int *list;
  int n;
  MPI_Group base_grp;


  /*construct a new communicator*/
  n=0;for(i=0; i <  ngrp; i++){
    if(use[i]==1) n++;
  }
  list=(int*) malloc(sizeof(int)*n);
  n=0;for(i=0; i <  ngrp; i++) {
    if(use[i]==1){   
       list[n]=i;
       n++;
    }
  }
  i=mpi_sep_ngroup;
  if(i==MAX_SEP_COM) 
   seperr("only allow a maximium of %d objects \n",
    (int)MAX_SEP_COM);
  mpi_sep_ngroup++;

  MPI_Comm_group(MPI_COMM_WORLD,&base_grp);
  MPI_Group_incl(base_grp,n,list,&sep_mpi_group[i]);
  MPI_Comm_create(MPI_COMM_WORLD,sep_mpi_group[i],&sep_mpi_comm[i]);
  free(list);
  return(i);
}
MPI_Comm *sep_mpi_return_com(int index){
return(&sep_mpi_comm[index]);
}

#else
void solaris_requires_def(){
}
#endif
