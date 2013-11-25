#ifndef __DISP_CTRL__
#define __DISP_CTRL__

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xmu/Error.h>
#include <X11/extensions/dpms.h>

#define OFF 	0x00
#define ON  	0x01
#define STANDBY	0x02
#define SUSPEND	0x03

int disp_init(int argc, char *argv[]);
int disp_ctrl(Display *dpy, int flag);
int disp_close(Display *dpy);

#endif
