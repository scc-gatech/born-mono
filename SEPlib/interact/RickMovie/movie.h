#ifndef RICKMOVIE_MOVIE_H
#define RICKMOVIE_MOVIE_H

/*
movie description

animation parameters
This version uses XView timers
*/

/* direction options */
#define	MOVIE_REVERSE	-1
#define	MOVIE_FORWARD	1

/* Movie object */
typedef struct {
	int	dir;	/* direction */
	int	run;	/* movie on or off */
	int	delay;	/* delay: 100 = 5 seconds */
	int	cache;	/* save frames */
	} *Movie;

/* API */
extern void MovieInit (void);
extern int MovieDir (void);
extern void MovieSetDir (int dir);
extern void MovieToggleDir (void);
extern void MovieToggleCache (void);
extern int MovieCache (void);
extern int MovieDelay (void);
extern void MovieSetRun (int run);
extern int MovieRun (void);
extern void MovieSetSpeed (int speed);
extern void MovieOn (void);
extern void MovieOff (void);
extern void MovieInfo (void);
extern void MovieSavePar (void);

#endif
