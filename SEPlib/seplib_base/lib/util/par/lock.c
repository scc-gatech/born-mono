#define SET_SDOC 1
#include <sepConfig.h>
#include<sep_par.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#if defined(HAVE_SYS_PARAM_H) || defined(__APPLE__)
#include <sys/param.h>
#endif
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <time.h>
#include <errno.h>

#if defined(HAVE_UTIME_H) || defined(__APPLE__)
#include <utime.h>
#endif

sep_file_lock *get_loc(char *name);
static sep_file_lock *sep_file_list=0;
static sep_file_lock *sep_file_tail=0;
int     lockfile_create(const char *lockfile, int retries, int flags);
int     lockfile_remove(const char *lockfile);
int     lockfile_touch(const char *lockfile);
int     lockfile_check(const char *lockfile, int flags);
                                                                                            
/*
 *      Return values for lockfile_create()
 */
#define L_SUCCESS       0       /* Lockfile created                     */
#define L_NAMELEN       1       /* Recipient name too long              */
#define L_TMPLOCK       2       /* Error creating tmp lockfile          */
#define L_TMPWRITE      3       /* Can't write pid int tmp lockfile     */
#define L_MAXTRYS       4       /* Failed after max. number of attempts */
#define L_ERROR         5       /* Unknown error; check errno           */
#define L_MANLOCK       6       /* Cannot set mandatory lock on tempfile */
                                                                                            
/*
 *      Flag values for lockfile_create()
 */
#define L_PID           16      /* Put PID in lockfile                  */
#define L_PPID          32      /* Put PPID in lockfile                 */


_XFUNCPROTOBEGIN
sep_file_lock *get_loc(char *name)
_XFUNCPROTOEND
{ 
 sep_file_lock *curr;
 int my_pid;
 char temp[1024],nm[4096];
 curr=sep_file_list;
 while(curr!=0){
   if(0==strcmp(name,curr->tag)) return(curr);
   curr=curr->next;
 } 

 curr=(sep_file_lock*) alloc(sizeof(sep_file_lock)); 
 curr->tag=(char*)malloc(strlen(name)+1);
 strcpy(curr->tag,name);
 if(0==getch("sep.pid","d",&my_pid)) my_pid=(int)getpid();
 if(0==getch( name,"s",nm)) strcpy(nm,name);

 sprintf(temp,"%s.%d",nm,my_pid);
 curr->lock_file=(char*)malloc(strlen(temp)+1);
 strcpy(curr->lock_file,temp);

 curr->fd=-1;

 if( sep_file_tail != 0 ) sep_file_tail->next = curr;
 curr->next=0;
 sep_file_tail=curr;
 if(sep_file_list==0) sep_file_list=curr;
 return(curr);
}
void sep_open_lock(char *tag){
char temp[256];
int fd;
sep_file_lock *lock;

  lock=get_loc(tag);
  if(L_SUCCESS!=lockfile_create(lock->lock_file, 60*30, 0))
    seperr("trouble opening  lockfile ");
}

void sep_close_lock(char *tag){
sep_file_lock *lock;

  lock=get_loc(tag);
lockfile_remove(lock->lock_file);
}


/*
 * lockfile.c 	Safely creates a lockfile, also over NFS.
 *		This file also holds the implementation for
 *		the Svr4 maillock functions.
 *
 * Version:	@(#)lockfile.c  1.0  05-Jun-1999  miquels@cistron.nl
 *
 *		Copyright (C) Miquel van Smoorenburg 1997,1998,1999.
 *
 *		This library is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU Library General Public
 *		License as published by the Free Software Foundation; either
 *		version 2 of the License, or (at your option) any later version.
 */

int check_sleep(int sleeptime);


/*
 *	Create a lockfile.
 */
int lockfile_create(const char *lockfile, int nsec, int flags)
{
	struct stat	st, st1;
	char		tmplock[MAXPATHLEN];
	char		sysname[256];
	char		buf[8];
	char		*p;
	int		sleeptime = 2;
	int		statfailed = 0;
	int		i, e, len,retries,wat;
	int		fd;



        wat=10000;
        retries=(int)((double)nsec*1000*1000/(double)wat);

	/*
	 *	Safety measure.
	 */
	if (strlen(lockfile) + 32 > MAXPATHLEN) {
		errno = ENAMETOOLONG;
		return L_ERROR;
	}

	/*
	 *	Create a temp lockfile (hopefully unique) and write
	 *	either our pid/ppid in it, or 0\0 for svr4 compatibility.
	 */
	if (gethostname(sysname, sizeof(sysname)) < 0)
		return L_ERROR;
	if ((p = strchr(sysname, '.')) != NULL)
		*p = 0;
	strcpy(tmplock, lockfile);
	if ((p = strrchr(tmplock, '/')) == NULL)
		p = tmplock;
	else
		p++;
	sprintf(p, ".lk%05d%x%s",
		(int)getpid(), (int)time(NULL) & 15, sysname);
	i = umask(022);
	fd = open(tmplock, O_WRONLY|O_CREAT|O_EXCL, 0644);
	e = errno;
	umask(i);
	if (fd < 0) {
		errno = e;
		return L_TMPLOCK;
	}
	if (flags & (L_PID | L_PPID)) {
		sprintf(buf, "%d\n",
			(flags & L_PID) ? (int)getpid() : (int)getppid());
		p = buf;
		len = strlen(buf);
	} else {
		p = "0";
		len = 2;
	}
	i = write(fd, p, len);
	e = errno;
	if (close(fd) != 0) {
		e = errno;
		i = -1;
	}
	if (i != len) {
		unlink(tmplock);
		errno = i < 0 ? e : EAGAIN;
		return L_TMPWRITE;
	}

	/*
	 *	Now try to link the temporary lock to the lock.
	 */
	for (i = 0; i < retries && retries > 0; i++) {

		/*
		 *	Now lock by linking the tempfile to the lock.
		 *
		 *	KLUDGE: some people say the return code of
		 *	link() over NFS can't be trusted.
		 *	EXTRA FIX: the value of the nlink field
		 *	can't be trusted (may be cached).
		 */
		if(-1 == link(tmplock, lockfile))
                {
                   perror("lockfile_create: link() ");
                }

		if (lstat(tmplock, &st1) < 0)
			return L_ERROR; /* Can't happen */

		if (lstat(lockfile, &st) < 0) {
			if (statfailed++ > 5) {
				/*
				 *	Normally, this can't happen; either
				 *	another process holds the lockfile or
				 *	we do. So if this error pops up
				 *	repeatedly, just exit...
				 */
				e = errno;
				(void)unlink(tmplock);
				errno = e;
				return L_MAXTRYS;
			}
			continue;
		}

		/*
		 *	See if we got the lock.
		 */
		if (st.st_rdev == st1.st_rdev &&
		    st.st_ino  == st1.st_ino) {
			(void)unlink(tmplock);
			return L_SUCCESS;
		}
		statfailed = 0;

		/*
		 *	If there is a lockfile and it is invalid,
		 *	remove the lockfile.
		 */
		if (lockfile_check(lockfile, flags) == -1)
			unlink(lockfile);

             if( i%100==99)fprintf(stderr,"SLEEPING\n");
           usleep(wat);
	}
	(void)unlink(tmplock);
	errno = EAGAIN;
	return L_MAXTRYS;
}
int check_sleep(int sleeptime)
{
        int             i;
        static int      ppid = 0;
                                                                                
        if (ppid == 0) ppid = getppid();
                                                                                
        for (i = 0; i < sleeptime; i += 1) {
                sleep(1);
                if (kill(ppid, 0) < 0 && errno == ESRCH)
                        return L_ERROR;
        }
        return 0;
}


/*
 *	See if a valid lockfile is present.
 *	Returns 0 if so, -1 if not.
 */
int lockfile_check(const char *lockfile, int flags)
{
	struct stat	st;
	char		buf[16];
	time_t		now;
	pid_t		pid;
	int		fd, len, r;

	if (stat(lockfile, &st) < 0)
		return -1;

	/*
	 *	Get the contents and mtime of the lockfile.
	 *	Use the time of the file system.
	 */
	time(&now);
	pid = 0;
	if ((fd = open(lockfile, O_RDONLY)) >= 0) {
		if ((len = read(fd, buf, sizeof(buf))) >= 0 &&
		    fstat(fd, &st) == 0)
			now = st.st_atime;
		close(fd);
		if (len > 0 && (flags & (L_PID|L_PPID))) {
		buf[len] = 0;
			pid = atoi(buf);
		}
	}

	if (pid > 0) {
		/*
		 *	If we have a pid, see if the process
		 *	owning the lockfile is still alive.
		 */
		r = kill(pid, 0);
		if (r == 0 || errno == EPERM)
			return 0;
		if (r < 0 && errno == ESRCH)
			return -1;
		/* EINVAL - FALLTHRU */
	}

	/*
	 *	Without a pid in the lockfile, the lock
	 *	is valid if it is newer than 5 mins.
	 */
	if (now < st.st_mtime + 300)
		return 0;
	return -1;
}

/*
 *	Remove a lock.
 */
int lockfile_remove(const char *lockfile)
{
	return (unlink(lockfile) < 0 && errno != ENOENT) ? -1 : 0;
}

