/*
 *
 * Copyright 1985, 1998  The Open Group
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of The Open Group shall not be
 * used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization from The Open Group.
 *
 * */
/* $XFree86: xc/programs/xset/xset.c,v 3.32 2003/09/24 02:43:39 dawes Exp $ */
/* Modified by Stephen so keyboard rate is set using XKB extensions */

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xmu/Error.h>
#include <X11/extensions/dpms.h>

int error_status = 0;

static int local_xerror(Display *dpy, XErrorEvent *rep);

    int
main(int argc, char *argv[])
{
    register char *arg;
    register int i;

    CARD16 standby_timeout, suspend_timeout, off_timeout;
    char *disp = NULL;
    Display *dpy;
    Bool hasargs = False;

    for (i = 1; i < argc; i++) {
	arg = argv[i];
	if (strcmp (arg, "-display") == 0 || strcmp (arg, "-d") == 0) {
	    disp = argv[i];
	} else {
	    hasargs = True;
	}
    }
    if (!hasargs) {
	printf("error!\n");
	exit(1);
    }

    dpy = XOpenDisplay(disp);  /*  Open display and check for success */
    if (dpy == NULL) {
	fprintf(stderr, "%s:  unable to open display \"%s\"\n",
		argv[0], XDisplayName (disp));
	exit(1);
    }
    XSetErrorHandler (local_xerror);
    for (i = 1; i < argc; ) {
	arg = argv[i++];
	int dummy;
	if (DPMSQueryExtension(dpy, &dummy, &dummy)) 
	{
	    DPMSGetTimeouts(dpy, &standby_timeout, &suspend_timeout,
		    &off_timeout);

	    if (strcmp(arg, "on") == 0) {
		DPMSEnable(dpy);
		DPMSForceLevel(dpy, DPMSModeOn);
		i++;
	    }
	    else if (strcmp(arg, "standby") == 0) {
		DPMSEnable(dpy);
		usleep(100000);
		DPMSForceLevel(dpy, DPMSModeStandby);
		i++;
	    }
	    else if (strcmp(arg, "suspend") == 0) {		  
		DPMSEnable(dpy);
		usleep(100000);
		DPMSForceLevel(dpy, DPMSModeSuspend);
		i++;
	    }
	    else if (strcmp(arg, "off") == 0) {  
		DPMSEnable(dpy);
		usleep(100000);
		printf("goto off");
		DPMSForceLevel(dpy, DPMSModeOff);
		i++;
	    }
	    else { 
		fprintf(stderr, "bad parameter %s\n", arg);
		i++;
	    }    
	}
	else {
	    fprintf(stderr, "server does not have extension for dpms option\n");
	}
    }
    XCloseDisplay (dpy);

    exit(error_status);    /*  Done.  We can go home now.  */
}




/*  This is the information-getting function for telling the user what the
 *   *  current "xsettings" are.
 *    */
    static void
query(Display *dpy)
{
    int dummy;
    
    DefaultScreen (dpy);
    CARD16 standby, suspend, off;
    BOOL onoff;
    CARD16 state;

    printf("DPMS (Energy Star):\n");
    if (DPMSQueryExtension(dpy, &dummy, &dummy)) {
	if (DPMSCapable(dpy)) {
	    DPMSGetTimeouts(dpy, &standby, &suspend, &off);
	    printf ("  Standby: %d    Suspend: %d    Off: %d\n",
		    standby, suspend, off);
	    DPMSInfo(dpy, &state, &onoff);
	    if (onoff) {
		printf("  DPMS is Enabled\n");
		switch (state) {
		    case DPMSModeOn:
			printf("  Monitor is On\n");
			break;
		    case DPMSModeStandby:
			printf("  Monitor is in Standby\n");
			break;
		    case DPMSModeSuspend:
			printf("  Monitor is in Suspend\n");
			break;
		    case DPMSModeOff:
			printf("  Monitor is Off\n");
			break;
		    default:
			printf("  Unrecognized response from server\n");
		}
	    }
	    else
		printf("  DPMS is Disabled\n");
	}
	else
	    printf ("  Display is not capable of DPMS\n");  
    }
    else {
	printf ("  Server does not have the DPMS Extension\n");
    }
    return;
}

    static int 
local_xerror(Display *dpy, XErrorEvent *rep)
{
    XmuPrintDefaultErrorMessage (dpy, rep, stderr);
    error_status = -1;

    return (0);
}
