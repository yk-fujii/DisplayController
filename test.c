#include "dispctrl.h"


int main(int argc, char *argv[])
{
    Display *dpy;

    disp_init(argc, argv);
    disp_ctrl(dpy, OFF);
    disp_close(dpy);
    
    return 0;
}
