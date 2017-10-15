/*
 * colname= one of {I,i,F,R,W,G} just like in Movie. You may also append
 * a "C" to make clipped values show up Red (ie colname=IC is like colname=I
 * but with clipped values flagged).
 * If "colname" is 3 characters or longer, it is assumed to give the name
 * of a color-table file.
 *
 * The float arrays red, green, and blue will be filled up with a raster
 * color table that is nocol elements long, in elements 0 through nocol-1.
 * The arrays should be declared 256 long in any case, however, so that
 * user-defined color tables are guaranteed to fit. The values in the
 * arrays range between 0. (full off) and 1. (full on).
 *
 * keywords: raster color-table vplot movie example utility

10/96 - Biondo :  Added V(elocity) color table
 */
#include <sepConfig.h>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <math.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#include <assert.h>
#include "../include/vplot.h"

#ifdef FORTRAN
#define NAME_TO_COLTAB nametocoltab_
#else
#define NAME_TO_COLTAB name_to_coltab
#endif

static int     *cola;
static int      cola2[9];
static float rd_color (int x, int ncol);
static float gr_color (int x, int ncol);
static float bl_color (int x, int ncol);
static void hue2rgb (float hue,float *red, float *green, float *blue);

#define NONE	0
#define HALF	128
#define FULL	255
#define NEARFULL 200

#ifndef MIN
#define	MIN(x,y) ((x) < (y) ? (x) : (y))
#endif

static int      cintensityp[9] =
{
  NONE, HALF, FULL, NONE, HALF, FULL, NONE, HALF, FULL
};
static int      cintensityn[9] =
{
  FULL, HALF, NONE, FULL, HALF, NONE, FULL, HALF, NONE
};
static int      cflag[9] =
{
  FULL, FULL, NONE, NONE, FULL, NONE, NONE, FULL, FULL
};
static int      cbi[9] =
{
  FULL, NONE, NONE, NONE, NONE, NONE, NONE, NONE, FULL
};
static int      csurf[9] =
{
  NONE, NONE, FULL, NONE, NONE, FULL, NONE, NEARFULL, FULL
};
static int      cgsi[9] =
{
  NONE, FULL, FULL, NONE, FULL, NONE, NONE, FULL, NONE
};

int NAME_TO_COLTAB (char *colname, int nocol, float *red, float *green, float *blue)
{
  FILE           *fdc;
  int             ii,ic,ih,hnocol,ncuse;

  int             nmax;
  float           gray, redsave;

  
    nmax=512;
  
  if (strlen (colname) <= 2)
    {
    nmax=512;
/*
  if (nocol < 2 || nocol > 256)
    {
      fprintf (stderr, "nocol=%d in name_to_coltab is illegal\n", nocol);
      exit(-1);
    }
*/
      switch (*colname)
	{
	case 'i':
	  cola = cintensityn;
    nmax=256;
	  break;
	case 'F':
	  cola = cflag;
	  break;
	case 'R':
	  cola = cbi;
	  break;
	case 'W':
	  cola = csurf;
	  break;
	case 'G':
	  cola = cgsi;
	  break;
	case 'I':
    nmax=256;
	case 'j':
    nmax=512;
	default:
	  cola = cintensityp;
	  break;
	}
    ncuse=MIN(nmax,nocol);
      
      for (ii = 0; ii < 9; ii++)
	{
	  switch (cola[ii])
	    {
	    case FULL:
	      cola2[ii] = ncuse - 1;
	      break;
	    case NEARFULL:
	      cola2[ii] = 200 * ncuse / 256;
	      break;
	    case HALF:
	      cola2[ii] = ncuse / 2;
	      break;
	    case NONE:
	    default:
	      cola2[ii] = 0;
	      break;
	    }
 	}
  
      switch (*colname) {  
      case 'a': case 'A': 
      case 'h': case 'H':
      case 'p': case 'P':
      case 'b': case 'B':
      case 'c': case 'C':
      case 'l': case 'L':
      case 'f': 
      case 'j': case 'J':
      case 't': case 'T':
	if (*colname == 'j' || 
	    *colname == 't') hnocol = (int) (0.25  * ncuse); 
	else                 hnocol = (int) (3./8. * ncuse);
	
	for (ii = 0; ii < ncuse; ii++) {
	  switch (*colname) {
	  case 'A': case 'H': case 'P': case 'B':
	  case 'C': case 'L': case 'J': case 'T':
	    ic = ncuse - 1 - ii;
	    break;
	  default:
	    ic = ii;
	    break;
	  }
	  gray = ((float) ii)/((float) ncuse - 1.0f);
	  switch (*colname)
	    {
	    case 'a': /* rainbow - HSV */
	      hue2rgb (gray, &red[ic], &green[ic], &blue[ic]);
	      break;
	    case 'h': case 'p': case 'b': /* hot, pink, bone */
	      if (ii < hnocol) {
		red[ic] = ((float) ii + 1) / ((float) hnocol);
		green[ic] = 0.0f;
		blue[ic] = 0.0f;
	      } else if (ii < 2*hnocol) { /* ii >= hnocol */
		ih = ii - hnocol;
		red[ic] = 1.0f;
		green[ic] = ((float) ih + 1) / ((float) hnocol);
		blue[ic] = 0.0f;
	      } else {                   /* ii >= 2*hnocol */
		ih = ii - 2*hnocol;
		red[ic] = 1.0f;
		green[ic] = 1.0f;
		blue[ic] = ((float) (ih + 1)) / ((float) (ncuse - 2*hnocol));
	      }
	      if (*colname == 'p') { /* pink */
		red[ic] = (float) sqrt( (double) (2.*gray + red[ic])/3.);
		green[ic] = (float) sqrt( (double) (2.*gray + green[ic])/3.);
		blue[ic] = (float) sqrt( (double) (2.*gray + blue[ic])/3.);
	      }
	      if (*colname == 'b') { /* bone */
		redsave = red[ic];
		red[ic] = (7.0f*gray + blue[ic])*0.125f;
		green[ic] = (7.0f*gray + green[ic])*0.125f;
		blue[ic] = (7.0f*gray + redsave)*0.125f;
	      }	  
	      break;
	    case 'c': /* cool */
	      red[ic] = gray;
	      green[ic] = 1.0f - gray;
	      blue[ic] = 1.0f;
	      break;
	    case 'l': /* linear = COPPER */
	      red[ic] = MIN (1.0f, 1.25f * gray);
	      green[ic] = 0.7812f * gray;
	      blue[ic] = 0.4975f * gray;
	      break;
	    case 'f': /* flag */
	      ih = ii%4;
	      switch (ih) {
	      case 0: /* red */
		red[ic] = 1.0f; green[ic] = 0.0f; blue[ic] = 0.0f; break;
	      case 1: /* white */
		red[ic] = 1.0f; green[ic] = 1.0f; blue[ic] = 1.0f; break;
	      case 2: /* blue */
		red[ic] = 0.0f; green[ic] = 0.0f; blue[ic] = 1.0f; break;
	      case 3: /* black */
		red[ic] = 0.0f; green[ic] = 0.0f; blue[ic] = 0.0f; break;
	      default:
		break;
	      }
	      break;
	    case 'j': /* jet */
	      if (ii <= hnocol/2) {
		red[ic] = 0.0f;
		green[ic] = 0.0f;
		blue[ic] = ((float) (ii + hnocol/2))/ ((float) hnocol);
	      } else if (ii < 3*hnocol/2) {  /* ii > hnocol/2 */
		ih = ii - hnocol/2;
		red[ic] = 0.0f;
		green[ic] = ((float) (ih + 1)) / ((float) hnocol);
		blue[ic] = 1.0f;
	      } else if (ii < 5*hnocol/2) { /* ii >= 3*hnocol/2 */
		ih = ii - 3*hnocol/2;
		red[ic] = ((float) (ih + 1)) / ((float) hnocol);
		green[ic] = 1.0f;
		blue[ic] = 1.0f - red[ic];
	      } else if (ii < 7*hnocol/2) { /* ii >= 5*hnocol/2 */
		ih = ii - 5*hnocol/2;
		red[ic] = 1;
		green[ic] = 1.0f- ((float) (ih + 1)) / ((float) hnocol);
		blue[ic] = 0.0f;	
	      } else {                     /* ii >= 7*hnocol/2 */
		ih = ii - 7*hnocol/2;
		red[ic] = ((float) (hnocol - ih))/ ((float) hnocol);
		green[ic] = 0.0f;
		blue[ic] = 0.0f;
	      }
	      break;
	    case 't': /* traffic */
	      blue[ic] = 0.0f;
	      if (ii <= hnocol/2) { /* green up */
		red[ic] = 0.0f; 
		green[ic] = ((float) (ii + hnocol/2))/ ((float) hnocol); 
	      } else if (ii < 3*hnocol/2) { /* ii > hnocol/2 */
		ih = ii - hnocol/2;
		red[ic] = ((float) (ih + 1)) / ((float) hnocol);  /* red up */
		green[ic] = 1.0f;
	      } else if (ii < 5*hnocol/2) {                /* steady yellow */
		red[ic] = 1.0f;
		green[ic] = 1.0f;
	      } else if (ii < 7*hnocol/2) {              /* green down */
		ih = ii - 5*hnocol/2;;
		red[ic] = 1.0f;
		green[ic] = 1.0f - ((float) (ih + 1)) / ((float) hnocol);
		blue[ic] = 0.0f;	
	      } else {                     /* ii >= 7*hnocol/2 */
		ih = ii - 7*hnocol/2;   /* red down */
		red[ic] = ((float) (hnocol - ih))/ ((float) hnocol); 
		green[ic] = 0.0f;
		blue[ic] = 0.0f;
	      }
	      break;
	    default:
	      break;
	    }
	}		
	break;

 case 'V':

           hnocol=(nocol-1)/3+1;

/* M-R-Y */

     cola2[0] = hnocol - 1;
     cola2[1] = hnocol - 1;
     cola2[2] = hnocol - 1;

     cola2[3] = 0;
     cola2[4] = 0;
     cola2[5] = hnocol - 1;

     cola2[6] = hnocol - 1;
     cola2[7] = 0;
     cola2[8] = 0;


     for (ii = 0; ii < hnocol; ii++)
     {
         red[ii] = rd_color (ii, hnocol);
         green[ii] = gr_color (ii, hnocol);
         blue[ii] = bl_color (ii, hnocol);
/*  fprintf(stderr,"i=%d - R = %f, G= %f, B=%f \n",ii,red[ii],green[ii],blue[ii]);*/
     }

/* Y-W-B */


           hnocol=(ncuse-1)/3+1;

     cola2[0] = hnocol - 1;
     cola2[1] = hnocol - 1;
 cola2[2] = 0;

     cola2[3] = hnocol - 1;
     cola2[4] = hnocol - 1;
     cola2[5] = 0;

     cola2[6] = 0;
     cola2[7] = hnocol - 1;
     cola2[8] = hnocol - 1;


     for (ii = 0; ii < hnocol; ii++)
     {
         red[ii+hnocol-1] = rd_color (ii, hnocol);
         green[ii+hnocol-1] = gr_color (ii, hnocol);
         blue[ii+hnocol-1] = bl_color (ii, hnocol);
/*  fprintf(stderr,"i=%d - R = %f, G= %f, B=%f \n",ii+hnocol-1,red[ii+hnocol-1],green[ii+hnocol-1],blue[ii+hnocol-1]);*/

     }

/* B-C-G*/

           hnocol=(ncuse-1)/3+1;


     cola2[0] = 0;
     cola2[1] = 0;
     cola2[2] = 0;

     cola2[3] = 0;
     cola2[4] = hnocol - 1;
     cola2[5] = hnocol - 1;

     cola2[6] = hnocol - 1;
     cola2[7] = hnocol - 1;


     for (ii = 0; ii < hnocol; ii++)
     {
         red[ii+2*hnocol-1] = rd_color (ii, hnocol);
         green[ii+2*hnocol-1] = gr_color (ii, hnocol);
         blue[ii+2*hnocol-1] = bl_color (ii, hnocol);
/*  fprintf(stderr,"i=%d - R = %f, G= %f, B=%f \n",ii+2*hnocol-1,red[ii+2*hnocol-1],green[ii+2*hnocol-1],blue[ii+2*hnocol-1]);*/
     }

      break;

  case 'v':
/* rainbow with primary colors' */

           hnocol=(ncuse-1)/3+1;

/* G-C-B */

     cola2[0] = 0;
     cola2[1] = 0;
     cola2[2] = 0;

     cola2[3] = hnocol - 1;
     cola2[4] = hnocol - 1;
     cola2[5] = 0;

     cola2[6] = 0;
     cola2[7] = hnocol - 1;
     cola2[8] = hnocol - 1;


     for (ii = 0; ii < hnocol; ii++)
     {
     red[ii] = rd_color (ii, hnocol);
         green[ii] = gr_color (ii, hnocol);
         blue[ii] = bl_color (ii, hnocol);
/*  fprintf(stderr,"i=%d - R = %f, G= %f, B=%f \n",ii,red[ii],green[ii],blue[ii]);*/
     }

/* B-W-Y */


           hnocol=(ncuse-1)/3+1;

     cola2[0] = 0;
     cola2[1] = hnocol - 1;
     cola2[2] = hnocol - 1;

     cola2[3] = 0;
     cola2[4] = hnocol - 1;
     cola2[5] = hnocol - 1;

     cola2[6] = hnocol - 1;
     cola2[7] = hnocol - 1;
     cola2[8] = 0;


     for (ii = 0; ii < hnocol; ii++)
     {
         red[ii+hnocol-1] = rd_color (ii, hnocol);
         green[ii+hnocol-1] = gr_color (ii, hnocol);
         blue[ii+hnocol-1] = bl_color (ii, hnocol);
/*  fprintf(stderr,"i=%d - R = %f, G= %f, B=%f \n",ii+hnocol-1,red[ii+hnocol-1],green[ii+hnocol-1],blue[ii+hnocol-1]);*/

     }
           hnocol=(ncuse-1)/3+1;


     cola2[0] = hnocol - 1;
     cola2[1] = hnocol - 1;
     cola2[2] = hnocol - 1;

     cola2[3] = hnocol - 1;
     cola2[4] = 0;
     cola2[5] = 0;

     cola2[6] = 0;
     cola2[7] = 0;
     cola2[8] = hnocol - 1;


     for (ii = 0; ii < hnocol; ii++)
     {
         red[ii+2*hnocol-1] = rd_color (ii, hnocol);
         green[ii+2*hnocol-1] = gr_color (ii, hnocol);
         blue[ii+2*hnocol-1] = bl_color (ii, hnocol);
/*  fprintf(stderr,"i=%d - R = %f, G= %f, B=%f \n",ii+2*hnocol-1,red[ii+2*hnocol-1],green[ii+2*hnocol-1],blue[ii+2*hnocol-1]);*/
     }

      break;

      default:
     
	for (ii = 0; ii < ncuse; ii++)
	  {
	    red[ii] = rd_color (ii, ncuse);
	    green[ii] = gr_color (ii, ncuse);
	    blue[ii] = bl_color (ii, ncuse);
	  }
	break;
      }
      
      /*
       * If clipping flagged, change the 2 values at either end to red
       * (Why 2? Remember that Movie throws half the colors away, so
       * this is what we're used to!)
       */
      if (colname[0] != '\0' && colname[1] == 'C')
	{
	  red[0] = red[1] = red[ncuse-2] = red[ncuse-1] = 1.0f;
	  green[0] = green[1] = green[ncuse-2] = green[ncuse-1] = 0.0f;
	  blue[0] = blue[1] = blue[ncuse-2] = blue[ncuse-1] = 0.0f;
	}
    } 
  else
    {
      /*
       * Get the colors out of a file. Unset colors are left black.
       */
      fdc = fopen (colname, "r");
      if (fdc == NULL)
	{
	  fprintf (stderr, "can't open colfile %s\n", colname);
	  exit (-1);
	}
      
      for (ii = 0; ii < nocol; ii++)
	{
/*fprintf(stderr,"going %d %d  \n",ii,ncuse);*/
	  red[ii] = 0.0f;
	  green[ii] = 0.0f;
	  blue[ii] = 0.0f;
	}
      
      while (fscanf (fdc, "%d ", &ii) != EOF)
	{
/*	  if (ii >= 256)*/
/*	    {*/
/*	      fprintf (stderr, "Maximum allowed color in colfile is 255\n");*/
/*	      exit (-1);*/
/*	    }*/
      if(ii>nmax) nmax=ii;
	  if (ii < 0)
	    {
	      fprintf (stderr, "Minimum allowed color in colfile is 0\n");
	      exit (-1);
	    }
	  if(3 != fscanf (fdc, "%f %f %f\n", red + ii, green + ii, blue + ii))
            {
              perror("name_to_coltab(): reading colfile");
              exit (-1);
            }
	  if (red[ii] < 0.0f || green[ii] < 0.0f || blue[ii] < 0.0f)
	    {
	      fprintf (stderr, "Minimum allowed rgb value in colfile is 0.\n");
	      exit (-1);
	    }
	  if (red[ii] > 1.0f || green[ii] > 1.0f || blue[ii] > 1.0f)
	    {
	      fprintf (stderr, "Maximum allowed rgb value in colfile is 1.\n");
	      exit (-1);
	    }
	}
    }
   return(nmax);
}
  
static float rd_color (int x, int ncol)
{
  int             ncolo2;
  ncolo2 = ncol / 2;
  
  if (x < ncolo2)
    return (((ncolo2 - x) * cola2[0] / (float) ncolo2) + (x * cola2[1] / (float) ncolo2)) / (float) (ncol - 1);
  else
    return ((((ncol - 1) - x) * cola2[1] / (float) (ncolo2 - 1)) + ((x - ncolo2) * cola2[2] / (float) (ncolo2 - 1))) / (float) (ncol - 1);
}

static float gr_color (int x, int ncol)
{
  int             ncolo2;
  ncolo2 = ncol / 2;
  
  if (x < ncolo2)
    return (((ncolo2 - x) * cola2[3] / (float) ncolo2) + (x * cola2[4] / (float) ncolo2)) / (float) (ncol - 1);
  else
    return ((((ncol - 1) - x) * cola2[4] / (float) (ncolo2 - 1)) + ((x - ncolo2) * cola2[5] / (float) (ncolo2 - 1))) / (float) (ncol - 1);
}


static float bl_color (int x, int ncol)
{
  int             ncolo2;
  ncolo2 = ncol / 2;
  
  if (x < ncolo2)
    return (((ncolo2 - x) * cola2[6] / (float) ncolo2) + (x * cola2[7] / (float) ncolo2)) / (float) (ncol - 1);
  else
    return ((((ncol - 1) - x) * cola2[7] / (float) (ncolo2 - 1)) + ((x - ncolo2) * cola2[8] / (float) (ncolo2 - 1))) / (float) (ncol - 1);
}


static void hue2rgb (float hue,float *red, float *green, float *blue)
{
  float df, dg;
  int i;
  
  hue = hue*360.0f; 
  if (hue == 360.0f) hue = 0.0f; 
  hue /= 60.0f;
        
  i = (int) hue;
  df = hue-i;
  dg = 1.0f - df;
  
  switch (i) {
  case 0: 
    *red = 1.0f;  *green = df;  *blue = 0.0f; break;
  case 1: 
    *red = dg;  *green = 1.0f;  *blue = 0.0f; break;
  case 2: 
    *red = 0.0f;  *green = 1.0f;  *blue = df; break;
  case 3: 
    *red = 0.0f;  *green = dg;  *blue = 1.0f; break;
  case 4: 
    *red = df;  *green = 0.0f;  *blue = 1.0f; break;
  case 5: 
    *red = 1.0f;  *green = 0.0f;  *blue = dg; break;
  default:
    assert (0); break;
  }
}
