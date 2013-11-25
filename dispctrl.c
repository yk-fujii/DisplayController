#include "dispctrl.h"

int error_status = 0;
static int local_xerror(Display *dpy, XErrorEvent *rep);

int disp_init(int argc, char *argv[])
{
    register char *arg;
    register int i;

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
    if (argc!=1 && !hasargs) {
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

    return 0;
}

int disp_ctrl(Display *dpy, int flag)
{
    CARD16 standby_timeout, suspend_timeout, off_timeout;
    int dummy;
    if (DPMSQueryExtension(dpy, &dummy, &dummy)) 
    {
	DPMSGetTimeouts(dpy, &standby_timeout, &suspend_timeout,
		&off_timeout);

	DPMSEnable(dpy);
	switch(flag){
	    case OFF:
		usleep(100000);
		DPMSForceLevel(dpy, DPMSModeOff);
		break;
	    case ON:
		DPMSForceLevel(dpy, DPMSModeOn);
		break;
	    case STANDBY:
		usleep(100000);
		DPMSForceLevel(dpy, DPMSModeStandby);
		break;
	    case SUSPEND:
		usleep(100000);
		DPMSForceLevel(dpy, DPMSModeSuspend);
		break;
	    default:
		fprintf(stderr,"Invalid Flag\n");
		error_status=-1;
		break;
	}
    }
    return error_status;
}

int disp_close(Display *dpy)
{
    XCloseDisplay(dpy);
    return 0;
}

static int 
local_xerror(Display *dpy, XErrorEvent *rep)
{
    XmuPrintDefaultErrorMessage (dpy, rep, stderr);
    error_status = -1;

    return (0);
}
