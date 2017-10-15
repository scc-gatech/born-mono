#ifndef VP_EXTERN_H
#define VP_EXTERN_H
/*
 * defines structures referred to herein
 */
#include <sepConfig.h>
#include <sys/types.h>

#if defined(HAVE_TERMIOS_H) || defined(HAVE_SYS_TERMIOS_H)
#include<termios.h>
#endif
#if defined(HAVE_TERMIO_H)
#include<termio.h>
#endif

#include "../include/prototypes.h"
#include "../../include/vplot.h"
#include "../include/device.h"
#include "../include/vertex.h"
#include "../include/enum.h"
#include "../include/params.h"
#include "../include/pat.h"
/*
 * external variables (quite a few of them)
 */
#if defined(HAVE_TERMIOS_H)
extern struct termios tty_clean_state;
#else
#if defined(HAVE_TERMIO_H)
extern struct termio tty_clean_state;
#else
extern struct sgttyb tty_clean_state;
#endif
#endif

/*
 * these must be DECLARED in dev.conf
 */
extern char *documentation[];
extern int doclength;
extern char name[];

/*
 * these MUST be initialized in either dev.open or dev.reset
 * (Not setting them will cause a fatal error.)
 */
extern int dev_xmax, dev_ymax, dev_xmin, dev_ymin;
extern float pixels_per_inch;
extern float aspect_ratio;
extern int num_col;

/*
 * options and variables that may also need to be reset in dev.open
 * They can't be reset in dev.reset, because the user may override
 * the values set by the device. (Except that mono will not be reset
 * to NO if the device says it is YES.)
 */
extern int mono, invras;
extern float fatmult;
extern float fatmult_orig;
extern float patternmult;
extern int shade;
extern int wantras;
extern int rotate;
extern float  hshift, vshift;
extern int dither;
extern int endpause;
extern int txfont,txprec,txovly;
extern float pixc, greyc;

/*
 * these can also be set in dev.open or dev.reset if dovplot gets them wrong,
 * but can usually be left at their default values.
 *
 * allowecho must only be reset in dev.open because the user can override
 * from the command line to force it to be YES. (But user can't force it NO.)
 */
extern int allowecho;
extern int need_end_erase;
extern int buffer_output;
extern int smart_clip;
extern int smart_raster;
extern int cachepipe;
extern int Allgone;
extern int lost;

/*
 * This might also need to be set if you needed to force allowecho.
 */
extern void (*message)(int,char *);

/*
 * These variables may be useful for referring to in dev.open,
 * so that similar devices can be merged under one pen filter.
 */
extern char wstype[];
extern char callname[];
extern char *orig_argv0;

/*
 * Usual place to read from and place to send the plot to.
 * The device can use these if they are appropriate, or reject
 * these and handle things on its own instead.
 */
extern FILE *pltin;
extern FILE *pltout;
extern FILE *controltty;

/*
 * device structure stuff.
 */
extern struct device dev;

extern int MAIN_(void);
extern int gen_do_dovplot(int,FILE **, char [][MAXFLEN+1]);
extern void genmarker(int,int,int,int *);
extern void gentext (char *,float,float,float,float);
extern void dovplot(void);
extern void proc_vplot(void);
extern void init_vplot(void);
extern void genraster (int,int,int,int,int,int,unsigned short *,int,int,int);
extern void nulldev (void);
extern void nullopen(void);
extern void nullreset(void);
extern void nullmessage(int,char *);
extern void nullerase(int);
extern void nullclose(int);
extern void nullvector(int,int,int,int,int,int);
extern void nullmarker(int,int,int,int *);
extern void nulltext(char *,float,float,float,float);
extern void nullarea(int,struct vertex *);
extern void nullraster(int,int,int,int,int,int,unsigned short *,int,int,int);
extern void nullpoint(int,int);
extern int nullattributes(int,int,int,int,int);
extern int nullgetpoint(int *,int *);
extern int nullinteract(int,FILE *,char *);
extern void nullplot(int,int,int);
extern void nullstartpoly(int);
extern void nullmidpoly(int,int);
extern void nullendpoly(int);
extern void genarea (int,struct vertex *);
extern void xminclip (int,int,int *);
extern void yminclip (int,int,int *);
extern void xmaxclip (int,int,int *);
extern void ymaxclip (int,int,int *);
extern void polyfix (int,int,int *);
extern void polystart (void);
extern void text_marker (char *,int,int,int *);
extern void genmessage (int, char[]);
extern void genraster1 (int,int,int,int,int,int,unsigned short *,int,int,int);
extern void insert(int,int,int,int);
extern void delete(int);
extern void scan(void);
extern int edge(int,int);
extern void genhatch (int,int,float,int *,int *,int *,int *,struct vertex *);
extern void sort(int *,int);
extern int intersect(int,int *,struct vertex *,int);
extern void genpatarea (int npts, struct vertex *head);
extern void mov (double,double);
extern void load_font (int);
extern int inter (int,int,int,int,int);
extern int geninteract (int,FILE *,char *);
extern void genpoint(int,int);
extern void fatvec (int,int,int,int,int,int);
extern void genvector (int,int,int,int,int,int);
extern void dashvec (int,int,int,int,int,int);
extern int clip(int *,int *,int *,int *);
extern void vecarea (int,struct vertex *);
extern void drawpolygon(int,int[],int[]);
extern int solve (int,int,int,int,int);
extern int dupside (register struct vertex *);
extern int greycorr (int);
extern double dashmod (double,double);
extern void removtemp(void);
extern void vecoutline(struct vertex *);
extern void vptodevxy(int,int,int *,int *);
extern void vptodevw (int,int,int,int,int *,int *,int *,int *);
extern void devtovpxy (int,int,int *,int *);
extern void devtovpuxy (int,int,float *,float *);
extern void devtovpw (int,int,int,int,int *,int *,int *,int *);
extern void vptodevxy_text (int,int,int *,int *);
/*extern int seperr(char *, ... );*/
extern int file(char *,int);
extern void bufpar_scan(char *,int,int);
typedef union { int *i; float *f; double *g ; char *s; } MIXED;
extern int getpar(char *, char *, MIXED );
extern int bufpar (char *,char *, MIXED );
extern void bufpar_add_string(char *);
extern void bpscan_ (char *,int *,int *);
extern int bufpar_ (char *,char *, MIXED );
extern void bpadds_ (char *);
extern char *alloc(size_t size);
extern void getm_getsav( MIXED ,char,int);
extern int getparin(char *,char *, MIXED );
extern int getpar_hash(register char *,register int);
extern void getpar_push_input(register char *,register int);
#ifdef notdef
extern int getch(const char *, char *, MIXED);
extern int fetch(const char *, char *, MIXED);
#endif
extern int getch2(char *,char *, ... );
extern int fsize (int);
extern void xtopen(void);
extern int xt_dovplot(int,FILE **,char [][MAXFLEN+1]);
extern void Puthead (char *, ... );
extern int doc(char *);
extern void sepwhere (void);
extern void reset_parameters(void);
extern int xtnumcol(int,int);
extern void xt_set_color(int);
extern void xt_set_color_table(int,int,int,int);
extern int xtinteract(int,FILE *,char *);
extern void xtplot(int,int,int);
extern void xtaddText(char *);
extern void xtmessage (int,char *);
extern void xtpoint(int,int);
extern void xtarea(int,struct vertex *);
extern void xtraster(int,int,int,int,int,int,unsigned short *,int,int,int);
extern void xterase (int);
extern int xtattributes(int,int,int,int,int);
extern int (*genreader) (int,FILE **,char [][MAXFLEN+1]);
extern void psarea(int,struct vertex  *);
extern void startpath (void);
extern void addpath (int,int);
extern void endpath (void);
extern void psreset (void);
extern void ps_fixup_after_grestore(void);
extern void ps_grey_map(int);
extern void ps_set_color(int);
extern void ps_set_dash(int);
extern int psattributes (int,int,int,int,int);
extern void psplot (int,int,int);
extern void pstext (char *,float,float,float,float);
extern void pserase (int);
extern void dumb_psraster(int,int,int,int,int,int,unsigned short *,int,int,int);
extern void smart_psraster(int,int,int,int,int,int,unsigned short *,int,int,int);
extern void psopen(void);
extern void psclose(int);
extern void psvector(int,int,int,int,int,int);
extern void dateget(char *);
extern int rasattr (int,int,int,int,int);
extern void rasopen (void);
extern void rasreset (void);
extern void rasclose (int);
extern void raserase (int);
extern void rasvector(int,int,int,int,int,int);
extern void rasvector2(int,int,int,int,int,int);
extern void rasvector3(int,int,int,int,int,int);
extern void rasvector4(int,int,int,int,int,int);
extern int vpattributes(int,int,int,int,int);
extern void vperase (int);
extern void vpopen (void);
extern void vpclose (int);
extern void vpreset (void);
extern void vpvector(int,int,int,int,int,int);
extern void vpmarker(int,int,int,int *);
extern void vptext(char *,float,float,float,float);
extern void vpraster(int,int,int,int,int,int,unsigned short *,int,int,int);
extern void vpmessage (int,char *);
extern void vpplot (int,int,int);
extern void vpstartpoly(int);
extern void vpmidpoly(int,int);
extern void vpendpoly(int);
extern int vp_do_dovplot (int,FILE **,char [][MAXFLEN + 1]);
extern void vplogvector (int,int,int,int,int,int);
extern void vplogmessage (int,char []);

extern void end_of_file(void);
extern void set_table(void);
extern void reset(void);
extern void reset_windows(void);
extern void outline_window(void);
extern void getvpstring(void);
extern void update_color(void);
extern void getapoint(void);
extern void init_colors(void);
extern void setstyle(int);
extern int getpolygon(int);
extern void add_a_cor(char *,int,int);
extern void reset_parameters(void);
extern void removtemp(void);
FILE* tempcopy( FILE *, char *);

extern int isatty(int);
extern int redin(void);
extern int fullnm(char *,int);
extern int findnm(int, char *,int);
extern char* datapath(char *);
/*
 * options
 */
extern int size;
extern int echo;
extern int  xorig, yorig;
extern int overlay;
extern int nplots;
extern int xcenterflag, ycenterflag;
extern int xcenter, ycenter;
extern float  xscale, yscale, txscale, mkscale, dashscale;
extern float  hdevscale, vdevscale;

/*
 * variables
 */
extern int sepxargc;
extern char **sepxargv;
extern int xold,yold;
extern int xnew,ynew;
extern int xwmin,xwmax,ywmin,ywmax;
extern int xorigin,yorigin;
extern float uxorigin, uyorigin; /* as set by vplot generator */
extern float uxscale, uyscale; /* as set by vplot generator */
extern int fat,fatbase,dashon;
extern int num_col_8;
extern int afat;
extern int ipat;
extern struct pat pat[];
extern int cur_color;
extern int color_set[MAX_COL + 1][_NUM_PRIM];
extern int need_devcolor;
extern struct txalign txalign;
extern float mxx, mxy, myx, myy;
extern float dashes[];
extern float dashpos, dashsum;
extern char pltname[];
extern char group_name[];
extern char interact[];
extern int interactcoords; /* 1=user, 2=vplot 3=inches */
extern int group_number;
extern int ras_allgrey;		/* david,  10/03/94	*/
extern int device_open;
extern int no_stretch_text;
extern int brake;
extern int hclose_done;
extern int group_number;
extern int xt_after_break;
extern float ps_curcolor;
extern int ps_done_clipping_gsave;
extern int default_ps_font;
extern float psscale;
extern float epause;
extern int color_mult;
extern int esize;
extern int allow_pipe;
extern int first_time;
extern int style;
extern int default_style;
extern int erase;
extern int xwmax_last;
extern int xwmin_last;
extern int ywmax_last;
extern int ywmin_last;
extern int ever_called;
extern int framewindows;
extern int xret, yret;
extern char *txbuffer;
extern int txbuflen;
extern int ifat;
extern int pat_color;
extern int next_color;
extern int window;
extern struct vertex *vxbuffer;
extern int vxbuflen;
extern int xWmax, xWmin, yWmax, yWmin;
extern int default_txfont, default_txprec, default_txovly;
extern int      colormask[];
extern float    redmap[4], greenmap[4], bluemap[4];
extern float    redpow, greenpow, bluepow;
extern int      default_overlay;
extern float    user_txscale;
extern float    user_mkscale;
extern float    user_dashscale;
extern int      user_size;
extern int      user_rotate;
extern float    user_scale;
extern float    user_xscale;
extern float    user_yscale;
extern float    user_hshift;
extern float    user_vshift;
extern int      buffer_input;
extern FILE    *pltinarray[];
extern int      infileno;
extern char     pltinname[][MAXFLEN + 1];
extern int      out_isatty;
extern int      pltoutfd;
extern char     sepoutwhere[];
extern char     sepheadwhere[];
extern int	headfd;
extern FILE	*headstream;
extern int 	sepxargc;
extern char   **sepxargv;

#if defined(HAVE_TERMIOS_H)
extern struct termios tty_clean_state;
#else
#if defined(HAVE_TERMIO_H)
extern struct termio tty_clean_state;
#else
extern struct sgttyb tty_clean_state;
#endif
#endif
#endif
