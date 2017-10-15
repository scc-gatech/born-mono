#ifndef RICK_H_
#define RICK_H_
#include <sepConfig.h>
#define SEPLIB
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#if defined(XAW)
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#endif
#ifdef XVIEW
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>
#endif
#include <stdio.h>
#if defined(XAW)
#include "xaw.h"
#endif
#ifdef XVIEW
#include "xv.h"
#endif
#include <Xm/MainW.h>
#include <Xm/FileSB.h>
#include <Xm/XmAll.h>
#include <Xm/FileSB.h>
#include <Xm/XmAll.h>
#include <Xm/DialogS.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Scale.h>
#include <Xm/Separator.h>
#include <Xm/ScrollBar.h>
#include <Xm/TextF.h>
#include <Xm/Text.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Frame.h>
#include <Xm/Label.h>
#include <Xm/MainW.h>

#include "main.h"
#include "data.h"
#include "ui.h"
#include "pick.h"
#include "pik.h"
#include "view.h"

#include "ano.h"
#include "axis.h"
#include "color.h"
#include "colorbar.h"
#include "draw.h"
#include "edit.h"
#include "gridhd.h"
#include "help.h"
#include "map.h"
#include "movie.h"
#include "plane.h"
#include "render.h"
#include "region.h"
#include "func.h"
#endif
