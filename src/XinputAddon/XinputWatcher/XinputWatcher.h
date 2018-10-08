#include <iostream>
#include <string.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#include "../XEventPrinter/XEventPrinter.h"

#ifndef XINPUTWATCHER_H
#define XINPUTWATCHER_H

using namespace std;

class XinputWatcher
{
    private:
        Display *dpy;        
        Window win;
        XIEventMask mask[2];
        XIEventMask *m;
        XDeviceInfo * find_device_info(char *name, Bool only_extended);        
        int register_device(int deviceid);
    
    protected:
    
    public:
        XinputWatcher();
        virtual ~XinputWatcher();
        int watch(std::string inputID);
        int print_events(XEventPrinter &printer);
};

#include "XinputWatcher.cpp"

#endif // XINPUTWATCHER_H
