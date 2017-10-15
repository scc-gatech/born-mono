#include <sepConfig.h>
#if defined(HAVE_MOTIF) || defined(HAVE_ATHENA)
/*
movie object code
one animator shared between all parts of a view
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <unistd.h>
#include <fcntl.h>
#endif
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "axis.h"
#include "data.h"
#include "map.h"
#include "view.h"
#include "movie.h"
#include "ui.h"

Movie movie = 0;

/* initialize movie object */
void MovieInit (void)
	{
	NEW (Movie,movie,1);
	movie->dir = MOVIE_REVERSE;
	movie->run = 0;
	movie->delay = 0;
	movie->cache = 0;
	GETPARINT ("dir","d",&movie->dir);
	GETPARINT ("run","d",&movie->run);
	GETPARINT ("delay","d",&movie->delay);
	GETPARINT ("cache","d",&movie->cache);
	}

/* return movie direction */
int MovieDir (void)
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
void MovieToggleDir (void)
	{
	if (!movie) return;
	if (movie->dir == MOVIE_FORWARD) movie->dir = MOVIE_REVERSE;
	else movie->dir = MOVIE_FORWARD;
	}

/* set cache mode */
void MovieToggleCache (void)
	{
	if (!movie) return;
	movie->cache = 1 - movie->cache;
	}

/* return cache mode */
int MovieCache (void)
	{
	if (!movie) return (NO_INDEX);
	return (movie->cache);
	}

/* return delay */
int MovieDelay (void)
	{
	if (!movie) return (0);
	return (movie->delay);
	}

/* set run mode */
void MovieSetRun (int run)
	{
	if (!movie) return;
	movie->run = run;
	}

/* return run mode */
int MovieRun (void)
	{
	if (!movie) return (0);
	return (movie->run);
	}

/* set movie speed 0-100 */
void MovieSetSpeed (int speed)
	{
	movie->delay = 100 - speed;
	/* reset timer */
	if (MovieRun()) {
		MovieOff();
		MovieOn();
		}
	}

/* start animation oscillator */
void MovieOn (void)
	{
	if (ViewMovieOK() == 0) return;
	movie->run = 1;
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
	ViewDrawMovie (NULL, NULL);
#endif
#if defined(HAVE_MOTIF)
	ViewDrawMovie (NULL, NULL);
#endif
	}

/* stop animation oscillator */
void MovieOff (void)
	{
	if (!movie->run) return;
	UITimer (-1,0);
	movie->run = 0;
	}

/* return movie information */
void MovieInfo (void)
	{
	Message message;
	extern Movie movie;

	if (!movie) return;
	sprintf (message,"Movie: dir=%d run=%d delay=%d cache=%d",
		movie->dir,
		movie->run,
		movie->delay,
		movie->cache);
	UIMessage (message);
	}

/* save movie parameters */
void MovieSavePar (void)
	{
	Message message;
	extern Movie movie;

	sprintf (message,"Movie: dir=%d run=%d delay=%d cache=%d",
		movie->dir,
		movie->run,
		movie->delay,
		movie->cache);
	UISaveMessage (message);
	}
#endif
