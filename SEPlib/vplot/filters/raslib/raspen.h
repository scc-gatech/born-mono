extern unsigned char *image;	/* raster image */
extern int rascolor;		/* current color */
extern int dev_xmax, dev_ymax;
extern int      grf_format;
extern int      ppm_format;
extern int      white;
extern int      default_out;
extern char     colfile[];
extern float    o_pixels_per_inch;
#define	Image(IX,IY)		image[(IX)+dev_xmax*(dev_ymax-1-(IY))]
#define	Image3(IX,IY,RGB)	image[(RGB)+(IX)*3+dev_xmax*3*(dev_ymax-1-(IY))]
#define	Min(IX,IY)		((IX) < (IY) ? (IX) : (IY))
#define	Max(IX,IY)		((IX) > (IY) ? (IX) : (IY))
#define NCOLOR 256		/* number of colors */
extern int color_table[NCOLOR][3];
extern void zap(void);
extern void zap_change(int *);
extern void ras_write (void);
