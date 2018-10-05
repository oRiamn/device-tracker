#include <iostream>
#include <string.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#ifndef XEVENTPRINTER_H
#define XEVENTPRINTER_H

class XEventPrinter
{
    private:

    public:
        XEventPrinter() {}
        const char* type_to_name(int evtype);
        void devicechanged(Display *dpy, XIDeviceChangedEvent *event);
        void hierarchychanged(XIHierarchyEvent *event);
        void raw(XIRawEvent *event);
        void enterleave(XILeaveEvent* event);
        void property(Display *display, XIPropertyEvent* event);
        void device(XIDeviceEvent* event);
};

#include "XEventPrinter.cpp"

#endif // XEVENTPRINTER_H