#ifndef XT_COMMANDS_H
#define XT_COMMANDS_H

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
 *  source file:   ./filters/xtlib/xtbuttons.c
 *
 * Steve Cole (SEP), February 18 1992
 *      Inserted this sample edit history entry.
 *
 */

#include "xtpen.h"


#ifdef XT_SET_TRANSLATIONS
/* 	 <ColormapNotify>:	PenRepaint() \n\
         <ConfigureNotify>:	PenRepaint() \n\ */

/* default translation table for pen_picture widget */
static char trans[] =
	"<Expose>:		PenRepaint() \n\
         <ConfigureNotify>:	PenRepaint() \n\
         <Btn1Down>:            xt_print_coord() \n\
         ~Shift ~Lock<KeyPress>n:       xt_stop() xt_reset_number() xt_next() \n\
         ~Shift ~Lock<KeyPress>m:       xt_stop() xt_reset_number() xt_prev() \n\
         ~Shift ~Lock<KeyPress>r:       xt_run()  \n\
         ~Shift ~Lock<KeyPress>q:       xt_quit()  \n\
         ~Shift ~Lock<KeyPress>.:       xt_stop()  \n\
         ~Shift ~Lock<KeyPress>f:       xt_faster()  \n\
         ~Shift ~Lock<KeyPress>s:       xt_slower()  \n\
         ~Shift ~Lock<KeyPress>t:       xt_stretchy()  \n\
         ~Shift ~Lock<KeyPress>z:       xt_zoom()  \n\
	 ~Shift ~Lock<KeyPress>Escape: 	xt_reset_number()\n\
         ~Shift ~Lock<KeyPress>0: 	xt_number(0)\n\
         ~Shift ~Lock<KeyPress>1: 	xt_number(1)\n\
         ~Shift ~Lock<KeyPress>2: 	xt_number(2)\n\
         ~Shift ~Lock<KeyPress>3: 	xt_number(3)\n\
         ~Shift ~Lock<KeyPress>4: 	xt_number(4)\n\
         ~Shift ~Lock<KeyPress>5: 	xt_number(5)\n\
         ~Shift ~Lock<KeyPress>6: 	xt_number(6)\n\
         ~Shift ~Lock<KeyPress>7: 	xt_number(7)\n\
         ~Shift ~Lock<KeyPress>8: 	xt_number(8)\n\
         ~Shift ~Lock<KeyPress>9: 	xt_number(9)\n\
	 ~Shift ~Lock<KeyPress>Return:	xt_goto_frame() xt_reset_number()";

static XtActionsRec window_actions[] = {
	{"PenRepaint",	      PenRepaint},
	{"xt_quit",           actionQuit},	
	{"xt_next",           actionNext},	
	{"xt_stretchy",       actionStretch},	
	{"xt_zoom",           actionZoom},	
	{"xt_prev",           actionPrev},
        {"xt_run",            actionRun},
        {"xt_stop",           actionStop},
        {"xt_restart",        actionRestart},
	{"xt_faster",         actionFaster},
	{"xt_slower",         actionSlower},
	{"xt_number",         actionNumber},
	{"xt_reset_number",   actionNumReset},
	{"xt_goto_frame",     actionGoto},
	{"xt_print_coord",    actionCoord},
	{"xt_run_mode",       actionRunMode},
};

#endif

extern int xt_next_num;

#endif
