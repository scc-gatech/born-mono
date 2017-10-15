#include <sepConfig.h>
#if defined (MOTIF_FOUND) || defined(HAVE_ATHENA)
/*
help object
called for command line selfdoc or
portions for menu help
*/
#include <stdio.h>
#include <string.h>
#include "rick.h"

char* help = "START\
DATA ARGUMENTS: \n\
   in=datafile n1= n2= n3= \n\
      bytes format (SEPlib) \n\
      byte array without header \n\
   \n\
   in=datafile n1= n2= n3= esize=4 \n\
      float format \n\
      float array without header \n\
   \n\
   \n\
DATA PARAMETERS: \n\
   in=stdin                  \t name of input file \n\
   n1= n2= n3= n4=1 n5=1     \t length of three dimensions \n\
                             \t n1 is fastest, e.g. time \n\
   o1=0 o2=0 ...             \t first sample value in each dimension \n\
   d1=1 d2=1 ...             \t sample increment in each dimension \n\
   label1=n1 ...             \t label for each dimension \n\
   title=in                  \t dataset title \n\
   value=sample              \t name for values on colorbar \n\
   esize=1                   \t =1 for bytes or =4 for floats \n\
   \n\
   for esize=1 data \n\
      pclip=255              \t positive clip value; synonym high= \n\
      nclip=1                \t negative clip value; synonym low= \n\
   \n\
   for esize=4 data \n\
      tpow, gpow, pclip, clip, min, max     clipping parameters \n\
   \n\
   \n\
PICK PARAMETERS: \n\
   For multiple datasets:   <param> for first view, <param>1 for second view, ... \n\
   \n\
   npick=25000               \t maximum number of picks used \n\
   pick=file                 \t file containing picks \n\
   picksize=5                \t size of pick mark to display \n\
   run_cor=0                 \t (1) run correlation when doing auto-picking \n\
   search_radius=5           \t radius to search arround when doing auto-picking \n\
   npaths=3                  \t number of paths to search when doing VIT path \n\
   j_display=8               \t sampling of picks to display when doing auto-picking \n\
   nwind_cor=8               \t half witdth of correlation window when doing auto-picking \n\
   ind_axis=1                \t independent axis when doing auto-picking  \n\
   max_tol=1.02              \t first tolerance when doing auto-picking by growing \n\
   min_tol=.96               \t minimum tolerance when doing auto-picking by growing \n\
   dtol=.022                 \t sampling of tolerance when doing auto-picking by growing \n\
   showypicks=1              \t (1) show picks, (2) hide picks \n\
   display_method=0          \t (0) puts marks at each location, \n\
                             \t (1) lines drawn between picks along dependent axis \n\
   pickrange=5               \t range in which we can see nearby picks \n\
   \n\
   \n\
DISPLAY PARAMETERS: \n\
   For multiple views: <param> for first view, <param>1 for second view, ... \n\
   \n\
   ncolor=128                \t for now we can only display up to 128 \n\
   width=600 height=600      \t pixel dimension (> 64) or fraction of screen (<= 1.0) \n\
   style=cube                \t view is front, side, top, plan, array, picks, cube, fence, or transparent \n\
   orient=front              \t orientation is front, side, or top \n\
   origin=minimum            \t frames set to middle or minimum (origin) \n\
   transp=0                  \t (1) transpose down and across \n\
   shape=fit                 \t shape is fit (screen), true (size), or pixel \n\
   movie=off                 \t run movie in up, down, left, right, in, or out direction \n\
   color=gray                \t color is gray, straw, flag, tiger, blue, \n\
                             \t rainbow, AVO, velocity \n\
   contrast=50               \t contrast is between 0 and 100\n\
   norder=1,2,3,4,5          \t data axis corresponding to each view axis \n\
                             \t view axes are DOWN, ACROSS, DEEP, AXIS4, AXIS5 \n\
   font=bold-courier-20      \t alternative XWindows font \n\
   \n\
   \n\
MULTIPLE VIEW PARAMETERS: \n\
   nview=1                    \t number of different views (view[0], ..., view[nview-1]) \n\
   dataX =                    \t tag for dataset beyond the first (e.g. data1=comparison.H) \n\
   nview_dim=[nview,1]        \t orientation of the different views (across,down) \n\
   view_ratio_x=[1./nview[0]] \t amount of space in x for each view \n\
   view_ratio_y=[1./nview[1]] \t amount of space in y for each view \n\
   viewX_data = [in]          \t data to be used in a given view \n\
   \n\
   \n\
VELOCITY ANALYSIS PARAMETERS: \n\
   Set mode=velan. Make sure to have vscan and nmoed set in viewx_data. \n\
   Right mouse click with 'l' key is useful. \n\
   \n\
   oversample=10             \t oversample rate for velocity analysis \n\
   ignore=0.                 \t how much of eary times to ignore \n\
   smute=1.5                 \t stretch factor for muting \n\
   nsmooth=oversample*2+1    \t amount to smooth semblance \n\
   no_sem=0                  \t (1) calculate semblance \n\
   v0=1.5                    \t initial velocity to scan over \n\
   nv=50                     \t number of velocities to scan over \n\
   dv=(3.5-v0)/(nv-1)        \t sampling rate of velocity \n\
   \n\
   \n\
FILE FORMATS: \n\
   seplib, bytes input data set: (user supplied) \n\
      2-D of 3-D array of unsigned byte integers 0-255.\n\
      Use segy2movie to convert segy. \n\
      Use Byte to convert seplib floating point. \n\
   \n\
   seplib, float input data set: (user supplied) \n\
      2-D of 3-D array of float numbers. \n\
   \n\
   segy, su input data set: (user supplied) \n\
       Two dataset headers. \n\
       n2 x n3 segy traces. \n\
       Each trace 240 bytes + n1 IEEE floating point samples. \n\
       Use suread to convert tape dataset. \n\
   \n\
   vgrid: (user supplied).\n\
       cubic array of data. \n\
       100 byte tail describing dimensions. \n\
   \n\
   script file: (user supplied) \n\
       Text file. \n\
       Each line of text annotates an n3 direction frame. \n\
   \n\
   pick file: (generated by program or user supplied) \n\
	First line is dataset name. \n\
	Second line is dataset dimensions. \n\
	Third line are axes labels. \n\
	Followed by pick-lines: \n\
	   First line is ID and frame.\n\
	   Following lines are pick points: three coordinates and data value. \n\
   \n\
   par file: (user supplied) \n\
	List of parameters in name=value form. Free format. \n\
	Last of duplicates used. \n\
   \n\
   \n\
WINDOWS: \n\
   (1) Menubar on top.\n\
   (2) Message window below menubar.\n\
   (3) Control panel below message window.\n\
   (4) Color spectrum below control panel.\n\
       Line shows relative data sample distribution.\n\
       Bar shows last pick value or range of values.\n\
       Mouse click-drag-up specifies a value range.\n\
   (5) Resizable drawing canvas. \n\
       For mouse controls, see: \n\
          Help > Navigation Mouse Usage. \n\
          Help > Picking Mouse Usage. \n\
          Help > Annotate Mouse Usage. \n\
          Help > Other Mouse Usage. \n\
   \n\
   \n\
NAVIGATION MOUSE USAGE: \n\
   LEFT zoom; MIDDLE navigate; RIGHT pick. \n\
   \n\
   LEFT click-drag-up            \t zoom window \n\
   LEFT click-drag-up + 'h' key  \t zoom horizontal only \n\
   LEFT click-drag-up + 'v' key  \t zoom vertical only \n\
   'x' key                       \t move left one frame \n\
   'X' key                       \t move right one frame \n\
   'y' key                       \t move up one frame \n\
   'Y' key                       \t move down one frame \n\
   'z' key                       \t move backward one frame \n\
   'Z' key                       \t move forward one frame \n\
   MIDDLE click                  \t select cross frames \n\
   MIDDLE click-drag-up          \t select a movie range \n\
   \n\
   \n\
PICKING MOUSE USAGE: \n\
   RIGHT click                   \t pick a point on the image \n\
   RIGHT click + 'a' key         \t add a point to end of pick \n\
   RIGHT click + 'c' key         \t bring up pick parameters panel \n\
   RIGHT click + 'd' key         \t delete nearest point in pick. \n\
   RIGHT click + 'q' key         \t print information about nearest pick \n\
   RIGHT click + '?' key         \t print information about nearest pick \n\
   RIGHT click + 'l' key         \t display a line across all panels at selected \n\
   RIGHT drag  + 'e' key         \t delete a window of picks \n\
   RIGHT drag  + 'g' key         \t do a 2-D growing within selected window\n\
   RIGHT drag  + 'p' key         \t do a line between two points that has the best correlation \n\
   RIGHT click + 'f' key         \t perform region growing in the currently viewable cube \n\
   RIGHT click + 'b' key         \t snap all points with current symbol to best correlation \n\
   \n\
   \n\
ANNOTATE MOUSE USAGE:\n\
   All actions only available when annotation option selected (in control panel). \n\
   \n\
   RIGHT click + 'q' key         \t modify nearest annotation object \n\
   LEFT  drag                    \t create an oval at the given location \n\
   Middle drag                   \t create a rectangle at the given location \n\
   RIGHT click + 'a' key         \t add point to polyline object \n\
   RIGHT click + 'f' key         \t finish a polyline object \n\
   \n\
   \n\
OTHER MOUSE USAGE:\n\
   RIGHT click + 's' key         \t create sub-volume \n\
   COLORBAR ANY click-drag-up    \t replace sub-volume range with this new range \n\
   \n\
   \n\
MAIN FUNCTIONS: \n\
   Redraw                          \t refresh screen \n\
   Fix Picking                     \t fix pick changes \n\
   Write Vgrid File (floats)       \t save data files as floats in \n\
   Write Vgrid File (bytes)        \t save data files as bytes in \n\
   Write Parameter Restart File    \t create a parameter restart file \n\
   Write History File              \t save history list \n\
   Load History File               \t load history list \n\
   Quit                            \t exit program \n\
   \n\
   \n\
VIEW FUNCTIONS: select a style and set attributes \n\
   Front Face (2D)                 \t front face of data cube \n\
   Side Face (2D)                  \t side face of data cube \n\
   Top Face (2D)                   \t top face of data cube \n\
   Three Faces (2D)                \t all three cube faces \n\
   Cube (3D)                       \t cube view \n\
   Array (3D)                      \t array of front faces- up to a hundred \n\
   Fence (3D)                      \t show intersecting faces \n\
   Transparent Cube (3D)           \t transparent volume \n\
   Round Robin On/Off              \t collapse multiple views into one view \n\
   Next View                       \t show next view in round robin \n\
   Previous View                   \t show previous view in round robin \n\
   KEY 'Shift' + 'r'               \t turn feature on/off \n\
   KEY '>'                         \t show next view \n\
   KEY '<'                         \t show previous view \n\
   \n\
   \n\
ORIENT FUNCTIONS: change way axes point; 2-D are in plane \n\
   Swap Side & Top Faces(12)       \t rotate axes \n\
   Swap Front & Side Faces(23)     \t rotate axes \n\
   Swap Top & Front Faces(13)      \t rotate axes \n\
   Swap Deep & 4th Axes(14)        \t rotate axes \n\
   Swap Across & 4th Axes(24)      \t rotate axes \n\
   Swap Down & 4th Axes(34)        \t rotate axes \n\
   Swap Deep & 5th Axes(15)        \t rotate axes \n\
   Swap Across & 5th Axes(25)      \t rotate axes \n\
   Swap Down & 5th Axes(35)        \t rotate axes \n\
   Orientation Menu                \t launch orientation panel \n\
   Flip Down Direction             \t reversal \n\
   Flip Across Direction           \t reversal \n\
   Flip Deep Direction             \t reversal \n\
   Move Frames to Middle           \t cross frames to middle of each axis \n\
   Move Frames to Corner           \t cross frames to origin of each axis \n\
   Initial Orientation             \t revert \n\
   Adjust Labels ...               \t change axes' labels \n\
   \n\
   \n\
SIZE FUNCTIONS: set size and shape policy \n\
   Even Cube                       \t front fills 0.67 screen, sides 0.33 \n\
   True Proportions                \t show cube with true dimensions \n\
   Sample Per Pixel                \t guarantee one sample per pixel \n\
   Interpolate                     \t improves large magnifications \n\
   Fine Control ...                \t launch size parameters panel \n\
   Initial Size                    \t revert to initialize size \n\
   \n\
   LEFT MOUSE BOX ZOOMS            \t interactive magnification \n\
   + 'h' KEY ONLY HORZ             \t constrain to horizontal \n\
   + 'v' KEY ONLY VERT             \t constrain to vertical \n\
   LEFT MOUSE CLICK ONLY           \t initial size \n\
   \n\
   \n\
NAVIGATE FUNCTIONS: go to part of the cube or run movies \n\
   Movie to Boundaries             \t movie loop traverses full cross face \n\
   High Speed                      \t frames are stored in display terminal \n\
   Undo Move                       \t undo last command \n\
   Redo Move                       \t redo last undone command \n\
   Undo to Start                   \t undo to start of history list \n\
   Redo to End                     \t redo to end of history list \n\
   Animate Undo to Start           \t play movie undo sequence \n\
   Animate Redo to End             \t play movie redo sequence \n\
   Pause Animation                 \t stop movie \n\
   BUTTON 'GO'                     \t start movie \n\
   BUTTON 'NO'                     \t stop movie \n\
   BUTTON 'z'                      \t run/step out direction \n\
   BUTTON 'Z'                      \t run/step in direction \n\
   BUTTON 'x'                      \t run/step left direction \n\
   BUTTON 'X'                      \t run/step right direction \n\
   BUTTON 'y'                      \t run/step up direction \n\
   BUTTON 'Y'                      \t run/step down direction \n\
   SLIDER 'Speed'                  \t animation delay (right=faster) \n\
   MIDDLE MOUSE CLICK XFRAMES      \t moves cross frames \n\
   MIDDLE MOUSE DRAG MOVIE BOUNDS  \t set movie range \n\
   KEY 'Ctrl' + 'u'                \t undo \n\
   KEY 'Ctrl' + 'r'                \t redo \n\
   KEY 'Ctrl' + 'p'                \t pause \n\
   \n\
   \n\
COLOR FUNCTIONS: set color, contrast, and transparency \n\
   Gray                          \t grayscale \n\
   Straw                         \t blue and yellow \n\
   Flag                          \t red, white and blue \n\
   Tiger                         \t red, white and black \n\
   Blue                          \t blue and white \n\
   Rainbow                       \t multi-colored \n\
   AVO                           \t amplitude variation with offset \n\
   Veldata                       \t velocity analysis \n\
   Overlay' <color-list>         \t overlay lines and text \n\
   Mark' <color-list>            \t pick color \n\
   Background' <color-list>      \t screen background color \n\
   Flip Polarity                 \t flips data color \n\
   SLIDER 'CONTRAST              \t shift color table skew \n\
   SLIDER 'CENTER                \t shift color table zero point \n\
   BUTTON 'RESET                 \t reset initial contrast \n\
   BUTTON 'STEP                  \t increment contrast \n\
   \n\
   \n\
PICK FUNCTIONS: set picking behavior \n\
   Show/Hide Pick                \t make picks visible/invisible \n\
   Change Pick Properties        \t launch pick parameters panel \n\
   Fix Picking                   \t fix pick changes \n\
   Write to File                 \t save picks \n\
   Read from File                \t read picks \n\
   Increase Range                \t increase range for correlation \n\
   Decrease Range                \t decrease range for correlation \n\
   Increase Draw Size            \t make picks bigger \n\
   Decrease Draw Size            \t make picks smaller \n\
   \n\
   RIGHT MOUSE MAKES PICK' \n\
   + 'a' KEY                     \t add point \n\
   + 'm' KEY                     \t move nearest point \n\
   + 'd' KEY                     \t delete nearest point \n\
   + 'q' or '?' KEY              \t inquire nearest point \n\
   + 's' KEY                     \t select smooth region \n\
   \n\
   \n\
EDIT FUNCTIONS: edit grid sub-volume \n\
   Undo Blob Fill                   \t reverse fill of blob\n\
   Interpolate Current Edge Values  \t fill cube with edge values\n\
   Undo Cube Edge Interpolation     \t reverse fill of cube\n\
   's' KEY + RIGHT MOUSE DRAG       \t picks subvolume \n\
   COLORBAR                         \t set smoothing value \n\
   \n\
   \n\
PATH VIEW FUNCTIONS: display path through cube \n\
   Path View On/Off              \t toggle path projection on/off \n\
   Path Start                    \t go to start of path \n\
   Path Middle                   \t go to middle of path \n\
   Path End                      \t go to end of path \n\
   Animate Path Forward          \t play movie along path forward \n\
   Animate Path Backward         \t play movie along path backward \n\
   Pause Animation               \t stop movie \n\
   KEY '+                        \t moves forward on path \n\
   KEY '-                        \t moves backward on path \n\
   KEY 'Ctrl' + 'p               \t stop movie \n\
   \n\
   \n\
SECTION FUNCTIONS: plot various sections through the data \n\
   On Screen Wiggle Plot \n\
   On Screen Contour Plot \n\
   On Screen Greyscale Plot \n\
   Output View \n\
   Cubeplot \n\
   Print Wiggle Plot \n\
   Print Contour Plot \n\
   Save Section in File ... \n\
   Save Profile in File ... \n\
   CROSS-HAIRS SET PLANES & PROFILES \n\
   \n\
   \n\
VELOCITY VIEW FUNCTIONS: merge physical and velocity cubes \n\
   Separate Cubes                 \t distinct cubes \n\
   Merge by Adjusting Intensities \t merged cube in place of physical \n\
   Merge by Copying Differences   \t merged cube in place of physical \n\
   SLIDER 'VEL VIEW'              \t left=physical, right=velocity, middle=merge \n\
   \n\
   \n\
STATUS FUNCTIONS: print parameters and state variables \n\
   Dataset                       \t dataset parameters \n\
   Data Values                   \t data value parameters \n\
   Data Axis0                    \t value/color axis \n\
   Data Axis1                    \t fast axis, usually time \n\
   Data Axis2                    \t second data axis, usually CDP \n\
   Data Axis3                    \t slow axis, usually section \n\
   Data Axis4                    \t slow axis, usually offset \n\
   Data Axis5                    \t slow axis, unused \n\
   View                          \t view parameters \n\
   Down Axis                     \t view down axis parameters \n\
   Across Axis                   \t view across axis parameters \n\
   Deep Axis                     \t view deep axis parameters \n\
   4D Axis                       \t view extra axis parameters \n\
   5D Axis                       \t view extra axis parameters \n\
   Color Axis                    \t colorbar axis parameters \n\
   Color                         \t color and contrast parameters \n\
   Render                        \t rendering parameters \n\
   Graphics                      \t draw screen parameters \n\
   Mouse Buttons                 \t mouse button functions \n\
   Navigate                      \t movie parameters \n\
   Pick                          \t pick parameters \n\
   Pick List                     \t current pick line parameters \n\
   Edit                          \t current sub-volume edit parameters \n\
   Frame List                    \t list of screen frames (stdout) \n\
   \n\
   \n\
HELP FUNCTIONS: print self documentation from various places \n\
   Data Parameters \n\
   Pick Parameters \n\
   Display Parameters \n\
   Multiple View Parameters \n\
   Velocity Analysis Parameters \n\
   File Formats \n\
   Windows \n\
   Navigation Mouse Usage \n\
   Picking Mouse Usage \n\
   Annotate Mouse Usage \n\
   Other Mouse Usage \n\
   Main Functions \n\
   View Functions \n\
   Orient Functions \n\
   Size Functions \n\
   Navigate Functions \n\
   Color Functions \n\
   Pick Functions \n\
   Edit Functions \n\
   Path View Functions \n\
   Velocity View Functions \n\
   Status Functions \n\
   Help Functions \n\
   Examples \n\
   \n\
   \n\
EXAMPLES \n\
   Two cubes: \n\
      Ricksep < in.H data1=comparison.H nview=2 view1_data=data1 \n\
   \n\
   Velocity anlaysis: \n\
      Ricksep < cmps.H mode=velan view1_data=vscan \n\
      view2_data=nmoed nviews=3 nview=3 v0=1.5 dv=.025 \n\
   \n\
   4-D view: \n\
      Ricksep < data.H nview=2 norder=1,3,4,5,2 norder1=1,2,3,4,5 \n\
   \n\
   \n\
END";

/* print text on screen */
int HelpPrint (char *start,char *finish)
	{
	char *startp=NULL, *finishp=NULL;
	extern char *help;

	startp = strstr (help,start);
	finishp = strstr (help,finish);
	if (startp == NULL || finishp == NULL) return(0);
	for (;startp<finishp; startp++) fputc (*startp,stderr);
	fputc ('\n',stderr);
	fflush (stderr);
 	return(0);
	}

/* pipe text through more */
int HelpMore (char *start,char *finish)
	{
	FILE *pfile, *popen();
	char *startp=NULL, *finishp=NULL;
	extern char *help;

	startp = strstr (help,start);
	finishp = strstr (help,finish);
	if (startp == 0 || finishp == 0) return(0);
	pfile = popen ("more","w");
	for (;startp<finishp; startp++) fputc (*startp,pfile);
	fputc ('\n',pfile);
	fflush (pfile);
	pclose (pfile);
  return(0);
	}
#endif
