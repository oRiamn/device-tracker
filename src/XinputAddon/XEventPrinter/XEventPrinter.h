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
        static void print_devicechangedevent(Display *dpy, XIDeviceChangedEvent *event);
        static void print_hierarchychangedevent(XIHierarchyEvent *event);
        static void print_rawevent(XIRawEvent *event);
        static void print_enterleave(XILeaveEvent* event);
        static void print_propertyevent(Display *display, XIPropertyEvent* event);
        static void print_deviceevent(XIDeviceEvent* event);

    private:
        XEventPrinter() {}
};

#endif // XEVENTPRINTER_H