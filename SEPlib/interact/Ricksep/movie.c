#include <sepConfig.h>
#if defined(MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
movie object code
one animator shared between all parts of a view
*/
#include <sys/time.h>
#include <stdio.h>
#include "rick.h"

Movie movie = 0;

/* initialize movie object */
void MovieInit ()
	{
	NEW (Movie,movie,1);
	movie->dir = MOVIE_REVERSE;
	movie->run = 0;
	movie->delay = 0;
	movie->cache = 0;
	rick_getch ("dir","d",&movie->dir);
	rick_getch ("run","d",&movie->run);
	rick_getch ("delay","d",&movie->delay);
	rick_getch ("cache","d",&movie->cache);
	}

/* return movie direction */
int MovieDir ()
	{
	if (!movie) return (0);
	return (movie->dir);
	}

/* set movie direction */
void MovieSetDir (int dir)
	{
	if (!movie) return;
	movie->dir = dir > 0 ? MOVIE_FORWARD : MOVIE_REVERSE;
	}

/* toggle movie direction */
void MovieToggleDir ()
	{
	if (!movie) return;
	if (movie->dir == MOVIE_FORWARD) movie->dir = MOVIE_REVERSE;
	else movie->dir = MOVIE_FORWARD;
	}

/* set cache mode */
int MovieToggleCache ()
	{
	if (!movie) return(0);
	movie->cache = 1 - movie->cache;
	return(0);
	}

/* return cache mode */
int MovieCache ()
	{
	if (!movie) return (NO_INDEX);
	return (movie->cache);
	}

/* return delay */
int MovieDelay ()
	{
	if (!movie) return (0);
	return (movie->delay);
	}

/* set run mode */
int MovieSetRun (int run)
	{
	if (!movie) return(0);
	movie->run = run;
  return(0);
	}

/* return run mode */
int MovieRun ()
	{
	if (!movie) return (0);
	return (movie->run);
	}
int MovieMulti()
{
	return(movie->multi);
}

int SetMovieMulti(int multi){

if (!movie) return (0);
movie->multi=multi;
return(0);
	
}
int MovieDeltaSpeed (int delta) {
  
  movie->delay=movie->delay-delta;
  if(movie->delay<=0) movie->delay=0;
	/* reset timer */
	if (MovieRun()) {
		MovieOff();
		MovieOn();
		}
	return(0);
	}


/* set movie speed 0-100 */
int MovieSetSpeed (int speed)
	{
	movie->delay = 100 - speed;
	/* reset timer */
	if (MovieRun()) {
		MovieOff();
		MovieOn();
		}
	return(0);
	}

/* get movie speed 0-100 */
int MovieGetSpeed() {
  return (100 - movie->delay);
}

/* start animation oscillator */
int MovieOn ()
	{
	if (ViewMovieOK() == 0) return(0);
	movie->run = 1;
	movie->multi = 0;
#ifdef XVIEW
	static struct itimerval	timer;
	int value;

	value = movie->delay * 20000 + 1000;
	timer.it_interval.tv_sec = value / 1000000;
	timer.it_interval.tv_usec = value % 1000000;
	timer.it_value.tv_sec = value / 1000000;
	timer.it_value.tv_usec = value % 1000000;
	UITimer (&timer);
#endif
#if defined(HAVE_ATHENA)
	ViewDrawMovie ();
#endif
#if defined(MOTIF_FOUND)
	ViewDrawMovie ();
#endif
  return(0);
	}

/* stop animation oscillator */
int MovieOff ()
	{
	if (!movie->run) return(0);
	UITimer (-1,0);
	movie->run = 0;
  return(0);
	}

/* return movie information */
int MovieInfo ()
	{
	Message message;

	if (!movie) return(0);
	sprintf (message,"Movie: dir=%d run=%d delay=%d cache=%d",
		movie->dir,
		movie->run,
		movie->delay,
		movie->cache);
	UIDelayMessage (message);
	return(0);
	}

/* save movie parameters */
int MovieSavePar ()
	{
	Message message;

	sprintf (message,"Movie: dir=%d run=%d delay=%d cache=%d",
		movie->dir,
		movie->run,
		movie->delay,
		movie->cache);
	UISaveMessage (message);
	return(0);
	}
#endif
