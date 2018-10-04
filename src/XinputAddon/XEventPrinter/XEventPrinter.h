#include <iostream>
#include <string.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#ifndef XEVENTPRINTER_H
#define XEVENTPRINTER_H

class XEventPrinter
{
    public:
        static const char* type_to_name(int evtype);
        static void devicechanged(Display *dpy, XIDeviceChangedEvent *event);
        static void hierarchychanged(XIHierarchyEvent *event);
        static void raw(XIRawEvent *event);
        static void enterleave(XILeaveEvent* event);
        static void property(Display *display, XIPropertyEvent* event);
        static void device(XIDeviceEvent* event);

    private:
        XEventPrinter() {}
};

#endif // XEVENTPRINTER_H