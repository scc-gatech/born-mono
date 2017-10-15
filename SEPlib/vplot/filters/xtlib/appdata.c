/*
 * Copyright 1987 the Board of Trustees of the Leland Stanford Junior
 * University. Official permission to use this software is included in
 * the documentation. It authorizes you to use this file for any
 * non-commercial purpose, provided that this copyright notice is not
 * removed and that any modifications made to this file are commented
 * and dated in the style of my eXample below.
 */

/*
 *
 *  source file:   ./filters/xtlib/appdata.c
 *
 * Dave Nichosl (SEP), April 13 1993
 *      Inserted this sample edit history entry.
 * Dave Nichols (SEP), Jan 14 1994
 *      app_data.pause is a float not an int.
 * Dave Nichols (SEP), Nov 16 1999
 *      Add visual depth as a resource
 */


#include "xtpen.h"

AppData app_data;

#define XtNstretchy "stretchy"
#define XtCStretchy "Stretchy"
#define XtNusePixmaps "usePixmaps"
#define XtCUsePixmaps "UsePixmaps"
#define XtNuseImages    "useImages"
#define XtCUseImages    "UseImages"
#define XtNshowButtons "showButtons"
#define XtCShowButtons "ShowButtons"
#define XtNshowLabels "showLabels"
#define XtCShowLabels "ShowLabels"
#define XtNshowText "showText"
#define XtCShowText "ShowText"
#define XtNnumCol "numCol"
#define XtCNumCol "NumCol"
#define XtNpause "pause"
#define XtCPause "Pause"
#define XtNvisDepth "visDepth"
#define XtCVisDepth "VisDepth"

static XtResource resources[] = {
{
    XtNstretchy,
    XtCStretchy,
    XtRBoolean,
    sizeof(Boolean),
    XtOffset( AppDataPtr, stretchy ),
    XtRString,
    "False",
},
{
    XtNusePixmaps,
    XtCUsePixmaps,
    XtRBoolean,
    sizeof(Boolean),
    XtOffset( AppDataPtr, pixmaps ),
    XtRString,
    "False",
},
{
    XtNuseImages,
    XtCUseImages,
    XtRBoolean,
    sizeof(Boolean),
    XtOffset( AppDataPtr, images ),
    XtRString,
    "True",
},
{
    XtNshowButtons,
    XtCShowButtons,
    XtRBoolean,
    sizeof(Boolean),
    XtOffset( AppDataPtr, buttons ),
    XtRString,
    "True",
},
{
    XtNshowLabels,
    XtCShowLabels,
    XtRBoolean,
    sizeof(Boolean),
    XtOffset( AppDataPtr, labels ),
    XtRString,
    "True",
},
{
    XtNshowText,
    XtCShowText,
    XtRBoolean,
    sizeof(Boolean),
    XtOffset( AppDataPtr, textpane ),
    XtRString,
    "True",
},
{
    XtNnumCol,
    XtCNumCol,
    XtRInt,
    sizeof(int),
    XtOffset( AppDataPtr, num_col ),
    XtRString,
    "0",
},
{ 
    XtNvisDepth,
    XtCVisDepth,
    XtRInt,
    sizeof(int),
    XtOffset( AppDataPtr, vis_depth ),
    XtRString,
    "32",
},
{
    XtNpause,
    XtCPause,
    XtRFloat,
    sizeof(float),
    XtOffset( AppDataPtr, pause ),
    XtRString,
    "0.",
}
};

int xt_app_data( Widget app )
{

    XtVaGetApplicationResources( app,
			&app_data,
			resources, XtNumber( resources ),
			NULL );
    return 0;

}
