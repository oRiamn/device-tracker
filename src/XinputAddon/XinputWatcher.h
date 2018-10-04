#include <iostream>
#include <string.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>

#ifndef XINPUTWATCHER_H
#define XINPUTWATCHER_H

using namespace std;

class XinputWatcher
{
    public:
        XinputWatcher();
        void watch(std::string inputID);

    protected:

    private:
        Display *dpy;
        Window win;

        XIEventMask mask[2];
        XIEventMask *m;

        XDeviceInfo * find_device_info(char *name, Bool only_extended);
        
        int test_xi2(Display	*display,int deviceid);
        
        const char* type_to_name(int evtype);
        void print_devicechangedevent(Display *dpy, XIDeviceChangedEvent *event);
        void print_hierarchychangedevent(XIHierarchyEvent *event);
        void print_rawevent(XIRawEvent *event);
        void print_enterleave(XILeaveEvent* event);
        void print_propertyevent(Display *display, XIPropertyEvent* event);
        void print_deviceevent(XIDeviceEvent* event);
};

#endif // XINPUTWATCHER_H
