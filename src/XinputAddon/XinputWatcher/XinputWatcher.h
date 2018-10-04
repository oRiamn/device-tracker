#include <iostream>
#include <string.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#include "../XEventPrinter/XEventPrinter.cpp"

#ifndef XINPUTWATCHER_H
#define XINPUTWATCHER_H

using namespace std;

class XinputWatcher
{
    public:
        Display *dpy;
        XinputWatcher();
        virtual ~XinputWatcher();
        int watch(std::string inputID);
        int print_events(Display *display);

    protected:

    private:        
        Window win;
        XIEventMask mask[2];
        XIEventMask *m;
        XDeviceInfo * find_device_info(char *name, Bool only_extended);        
        int register_device(Display	*display,int deviceid);
};

#endif // XINPUTWATCHER_H
