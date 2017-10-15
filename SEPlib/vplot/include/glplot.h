#include <math.h>
#include <vplot.h>
#include <seplib.h>
#define NPMAX (1*2097151+0*32000) /* that big number came from Galilee */
#define SCREENHT STANDARD_HEIGHT	/* STANDARD_HEIGHT is defined in
					 * vplot.h */
#define SCREENWD SCREENHT/SCREEN_RATIO	/* SCREEN_RATIO is defined in vplot.h */


 /*
  * datainfo contains the information normally associated with seplib
  * programs. 
  */
struct datainfo
{
    int             n1[NPMAX+1];
    int             n2;
    int             n3;
    float           d1[NPMAX+1];
    float           o1[NPMAX+1];
    float           d2;
    float           o2;
    float           d3;
    float           o3;
    int             esize;
};
struct datainfo_g
{
    int             n1[NPMAX+1];
    int             n2;
    int             n3;
    double          d1[NPMAX+1];
    double          o1[NPMAX+1];
    double          d2;
    double          o2;
    double          d3;
    double          o3;
    int             esize;
};
 /*
  * coordinfo defines the users coordinates four corners.  The min's and
  * max's of the data 
  */
struct coordinfo
{
    float           min1;
    float           max1;
    float           min2;
    float           max2;
    int             transp;
    int             xreverse;
    int             yreverse;
    int		    labelrot;
    float           fmin1;
    float           fmin2;
    float           fmax1;
    float           fmax2;
    int             pad;		
    int             npad;		
};
struct coordinfo_g
{
    double          min1;
    double          max1;
    double          min2;
    double          max2;
    int             transp;
    int             xreverse;
    int             yreverse;
    int		    labelrot;
    double          fmin1;
    double          fmin2;
    double          fmax1;
    double          fmax2;
    int             pad;		
    int             npad;		
};
 /*
  * axisinfo  defines what axis is.  Axisinfo is a general structure. an axis
  * is defined by it's origin,  where it is positioned, it's labeling
  * interval, its tic interval, its beginning value for labeling and tics,
  * it's label size labelfat and label position. 
  */
struct axisinfo
{
    float           labelsz;
    int             labelfat;
    int             fat[NPMAX+1];
    int             col[NPMAX+1];
    float           axisor;
    float           inch;
    int             wantaxis;
    char           wheretics[10];
    char           wherelabel[10];
    float           dnum;
    float           dtic;
    float           num0;
    float           tic0;
    int             ntic;
    char           label[280];
};
struct axisinfo_g
{
    double          labelsz;
    int             labelfat;
    int             fat[NPMAX+1];
    int             col[NPMAX+1];
    double          axisor;
    double          inch;
    int             wantaxis;
    char            wheretics[10];
    char            wherelabel[10];
    double          dnum;
    double          dtic;
    double          num0;
    double          tic0;
    int             ntic;
    char            label[280];
};
 /*
  * plotposition  contains all the information about the size and positioning
  * of the plot on the output device 
  */
struct plotposition
{
    float           inch1;
    float           inch2;
    float           xll;
    float           yll;
    float           xur;
    float           yur;
    float           screenht;
    float           screenwd;
    float           screenratio;
};
struct plotposition_g
{
    double          inch1;
    double          inch2;
    double          xll;
    double          yll;
    double          xur;
    double          yur;
    double          screenht;
    double          screenwd;
    double          screenratio;
};
 /*
  * plotinfo contains the information about how the data will be plotted.
  * Type of line or symbol, size of symbol or thickness of line and color of
  * both symbol and line. 
  */
struct plotinfo
{
    int             lineunder;
    char            symbol[NPMAX+1];
    int             symbolsz[NPMAX+1];
    int             col[NPMAX+1];
    int             fat[NPMAX+1];
};
#define plotinfo_g plotinfo
struct dashinfo
{
    float           dashtype[NPMAX+1];
    float           dash[2];
    float           gap[2];
};
struct dashinfo_g
{
    double         dashtype[NPMAX+1];
    double         dash[2];
    double         gap[2];
};
 /*
  * gridinfo contains the information about plotting a grid on the data. the
  * intervals of the grid lines. and which axes to put the grid. 
  */
struct gridinfo
{
    int           grid;
    int           grid1;
    int           grid2;
    int           col[NPMAX+1]; 
    int           fat; 
    float         g1num;
    float         g2num;
};
struct gridinfo_g
{
    int           grid;
    int           grid1;
    int           grid2;
    int           col[NPMAX+1]; 
    int           fat; 
    double        g1num;
    double        g2num;
};
 /*
  * titleinfo   contains information about the plot's title.  the size of the
  * title, fatness of title and which axis it will appear on. 
  */
struct titleinfo
{
    int             titlefat;
    float           titlesz;
    char           title[280];
    char           wheretitle[10];
    int 	   wanttitle;
};
struct titleinfo_g
{
    int            titlefat;
    double         titlesz;
    char           title[280];
    char           wheretitle[10];
    int 	   wanttitle;
};
 /*
  * colorinfo contains information about background  and fill colors of the
  * plot. 
  */
struct colorinfo
{
    float           backcol[3];
    float           fillcol[3];
};
#define colorinfo_g colorinfo

#if defined(DEC3100) || defined(__stdc__ ) || defined(__STDC__) || defined( __cplusplus)

#define USE_PROTO

/* use prototyped function definitions for ansi-C and C++ */
extern int gl_optimal_scale(int n, float min, float max,
                         /*@out@*/ float *onum,float *dnum);
extern int gl_optimal_scale_g(int n, double min, double max,
                         /*@out@*/ double *onum,double *dnum);
extern float gl_nice_number (float d);
extern double gl_nice_number_g (double d);
extern float gl_power (float f, int ie);
extern double gl_power_g (double f, int ie);
extern int gl_arrow(float,float,float,float,float);
extern int gl_arrow_g(double,double,double,double,double);
extern int gl_axisint(struct axisinfo * , struct axisinfo * , struct coordinfo *,struct plotposition *);
extern int gl_axisint_g(struct axisinfo_g * , struct axisinfo_g * , struct coordinfo_g *,struct plotposition_g *);
extern int gl_barint (struct plotposition *, struct axisinfo *, struct plotposition *, struct axisinfo *, float *, float *, char *, int *, int,int);
extern int gl_barint_g (struct plotposition_g *, struct axisinfo_g *, struct plotposition_g *, struct axisinfo_g *, float *, float *, char *, int *, int,int);
extern int gl_barplot (struct plotposition *, struct axisinfo *, float *, float *, char *, int, int);
extern int gl_barplot_g (struct plotposition_g *, struct axisinfo_g *, float *, float *, char *, int, int);
extern int gl_clip(float,float,float,float);
extern int gl_clip_g(double,double,double,double);
extern int gl_color(int); 
extern int gl_color_g(int); 
extern int gl_colorint(struct colorinfo *);
extern int gl_colorint_g(struct colorinfo_g *);
extern int gl_coordinfo(struct plotposition *);
extern int gl_coordinfo_g(struct plotposition_g *);
extern int gl_coordint(struct plotposition *,struct coordinfo *,struct axisinfo *,struct axisinfo *);
extern int gl_coordint_g(struct plotposition_g *,struct coordinfo_g *,struct axisinfo_g *,struct axisinfo_g *);
extern int gl_dash(struct dashinfo *);
extern int gl_dash_g(struct dashinfo_g *);
extern int gl_dashfig(struct dashinfo *, int);
extern int gl_dashfig_g(struct dashinfo_g *, int);
extern int gl_draw(float, float);
extern int gl_draw_g(double, double);
extern int gl_erase(void);
extern int gl_erase_g(void);
extern int gl_fat(int);
extern int gl_fat_g(int);
extern int gl_fillin(struct coordinfo *, struct colorinfo *);
extern int gl_fillin_g(struct coordinfo_g *, struct colorinfo_g *);
extern int gl_framenum(int,float,float,float,float,float);
extern int gl_framenum_g(int,double,double,double,double,double);
extern int gl_getscl( struct coordinfo *, struct axisinfo * );
extern int gl_getscl_g( struct coordinfo_g *, struct axisinfo_g * );
extern int gl_gridint(struct gridinfo *, struct coordinfo *, struct axisinfo * , struct axisinfo * );
extern int gl_gridint_g(struct gridinfo_g *, struct coordinfo_g *, struct axisinfo_g * , struct axisinfo_g * );
extern int gl_gtext(float,float,float,float,float,float,char*,char*);
extern int gl_gtext_g(double,double,double,double,double,double,char*,char*);
extern int gl_invmassage(float*,float*,float,float);
extern int gl_invmassage_g(double*,double*,double,double);
extern int gl_labelaxis(struct coordinfo *, struct axisinfo * );
extern int gl_labelaxis_g(struct coordinfo_g *, struct axisinfo_g * );
extern int gl_labeltic(struct coordinfo *, struct axisinfo *  );
extern int gl_labeltic_g(struct coordinfo_g *, struct axisinfo_g *  );
extern int gl_makelabel(float, float, struct coordinfo *, struct axisinfo * );
extern int gl_makelabel_g(double, double, struct coordinfo_g *, struct axisinfo_g * );
extern int gl_massage(float*,float*,float*,float*);
extern int gl_massage_g(double*,double*,double*,double*);
extern int gl_minmax(struct coordinfo *);
extern int gl_minmax_g(struct coordinfo_g *);
extern int gl_move(float, float); 
extern int gl_move_g(double, double); 
extern int gl_opttic(float , float , float , float , float *, float);
extern int gl_opttic_g(double , double , double , double , double *, double);
extern int gl_padint(struct coordinfo *);
extern int gl_padint_g(struct coordinfo_g *);
extern int gl_penup(void);
extern int gl_penup_g(void);
extern int gl_plotaxis(struct axisinfo * , struct coordinfo *,int);
extern int gl_plotaxis_g(struct axisinfo_g * , struct coordinfo_g *,int);
extern int gl_plotframe(struct coordinfo *,int);
extern int gl_plotframe_g(struct coordinfo_g *,int);
extern int gl_plotgrid(struct coordinfo *,struct axisinfo *,struct gridinfo *,int );
extern int gl_plotgrid_g(struct coordinfo_g *,struct axisinfo_g *,struct gridinfo_g *,int );
extern int gl_plotint( struct plotinfo *, struct dashinfo *);
extern int gl_plotint_g( struct plotinfo_g *, struct dashinfo_g *);
extern int gl_plotpram(struct colorinfo *, struct coordinfo *);
extern int gl_plotpram_g(struct colorinfo_g *, struct coordinfo_g *);
extern int gl_plottic(struct coordinfo *, struct axisinfo * , int );
extern int gl_plottic_g(struct coordinfo_g *, struct axisinfo_g * , int );
extern int gl_plottitle(struct coordinfo *, struct titleinfo *, struct axisinfo *, int );
extern int gl_plottitle_g(struct coordinfo_g *, struct titleinfo_g *, struct axisinfo_g *, int );
extern int gl_purge(void);
extern int gl_purge_g(void);
extern int gl_rotate(float *, float, float, struct datainfo *);
extern int gl_rotate_g(float *, double, double, struct datainfo_g *);
extern int gl_rotate1(float *, float, float);
extern int gl_rotate1_g(double *, double, double);
extern int gl_simpleaxis( float,float,float,float,float,float, 
  float, float , float , char* , int,int,int,int,float,float );
extern int gl_simpleaxis_g( double,double,double,double,double,double, 
  double, double , double , char* , int,int,int,int,double,double );
extern int gl_stdplot(struct datainfo *,struct coordinfo *,struct axisinfo *,struct axisinfo *,struct gridinfo *,struct titleinfo *,int,int,int,int);
extern int gl_stdplot_g(struct datainfo_g *,struct coordinfo_g *,struct axisinfo_g *,struct axisinfo_g *,struct gridinfo_g *,struct titleinfo_g *,int,int,int,int);
extern int gl_tfont(int,int,int);
extern int gl_tfont_g(int,int,int);
extern int gl_titleint(struct titleinfo *);
extern int gl_titleint_g(struct titleinfo_g *);
extern int gl_tjust(char*);
extern int gl_tjust_g(char*);
extern int gl_transp( float*, float*,struct datainfo *); 
extern int gl_transp_g( double*, double*,struct datainfo_g *); 
extern int gl_uarea(float*,float*,int,int,int,int,int);
extern int gl_uarea_g(double*,double*,int,int,int,int,int);
extern int gl_udraw(float,float);
extern int gl_udraw_g(double,double);
extern int gl_uarrow(float,float,float,float,float);
extern int gl_uarrow_g(double,double,double,double,double);
extern int gl_uclip(float,float,float,float);
extern int gl_uclip_g(double,double,double,double);
extern int gl_udraw(float,float);
extern int gl_udraw_g(double,double);
extern int gl_umove(float,float);
extern int gl_umove_g(double,double);
extern int gl_upendn(float,float);
extern int gl_upendn_g(double,double);
extern int gl_upmark(int,int,int,float *,float *);
extern int gl_upmark_g(int,int,int,double *,double *);
extern int gl_vplotint(struct plotposition *, struct coordinfo *,struct axisinfo *,struct axisinfo * );
extern int gl_vplotint_g(struct plotposition_g *, struct coordinfo_g *,struct axisinfo_g *,struct axisinfo_g * );
extern int gl_where(float*,float*);
extern int gl_where_g(double*,double*);
#endif
