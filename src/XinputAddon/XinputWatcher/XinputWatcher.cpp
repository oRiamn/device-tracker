#include "XinputWatcher.h"

Display *dpy;
XDeviceInfo *info;
Window win;

XIEventMask mask[2];
XIEventMask *m;

XinputWatcher::XinputWatcher(void)
{
	info=NULL;
    dpy = XOpenDisplay(NULL);
	win = DefaultRootWindow(dpy);
}

XinputWatcher::~XinputWatcher(void) 
{
    XDestroyWindow(dpy, win);
}

int XinputWatcher::watch(std::string inputID)
{
	// cast std:string to char*
	char *inputIdChar = new char[inputID.length() + 1];
	strcpy(inputIdChar, inputID.c_str());

	info = find_device_info(inputIdChar, True);

	if (!info)
	{
		fprintf(stderr, "unable to find device '%s'\n", inputIdChar);
        return EXIT_FAILURE;
	}
	else
	{
		register_device( info->id);
	}

    return EXIT_SUCCESS;
}

XDeviceInfo *
XinputWatcher::find_device_info(char *name,
								Bool only_extended)
{
	XDeviceInfo *devices;
	XDeviceInfo *found = NULL;
	int loop;
	int num_devices;
	int len = strlen(name);
	Bool is_id = True;
	XID id = (XID)-1;

	for (loop = 0; loop < len; loop++)
	{
		if (!isdigit(name[loop]))
		{
			is_id = False;
			break;
		}
	}

	if (is_id)
	{
		id = atoi(name);
	}

	devices = XListInputDevices(dpy, &num_devices);

	for (loop = 0; loop < num_devices; loop++)
	{
		if ((!only_extended || (devices[loop].use >= IsXExtensionDevice)) &&
			((!is_id && strcmp(devices[loop].name, name) == 0) ||
			 (is_id && devices[loop].id == id)))
		{
			if (found)
			{
				fprintf(stderr,
						"Warning: There are multiple devices named '%s'.\n"
						"To ensure the correct one is selected, please use "
						"the device ID instead.\n\n",
						name);
				return NULL;
			}
			else
			{
				found = &devices[loop];
			}
		}
	}
	return found;
}

int
XinputWatcher::register_device(int deviceid)
{
    XIEventMask mask[2];
    XIEventMask *m;

    setvbuf(stdout, NULL, _IOLBF, 0);
	
    /* Select for motion events */
    m = &mask[0];
    m->deviceid = deviceid;
    m->mask_len = XIMaskLen(XI_LASTEVENT);
    m->mask = calloc(m->mask_len, sizeof(char));
    XISetMask(m->mask, XI_ButtonPress);
    XISetMask(m->mask, XI_ButtonRelease);
    XISetMask(m->mask, XI_KeyPress);
    XISetMask(m->mask, XI_KeyRelease);
    XISetMask(m->mask, XI_Motion);
    XISetMask(m->mask, XI_DeviceChanged);
    XISetMask(m->mask, XI_Enter);
    XISetMask(m->mask, XI_Leave);
    XISetMask(m->mask, XI_FocusIn);
    XISetMask(m->mask, XI_FocusOut);

    if (m->deviceid == XIAllDevices)
        XISetMask(m->mask, XI_HierarchyChanged);
    XISetMask(m->mask, XI_PropertyEvent);

    m = &mask[1];
    m->deviceid = (deviceid == -1) ? XIAllMasterDevices : deviceid;
    m->mask_len = XIMaskLen(XI_LASTEVENT);
    m->mask = calloc(m->mask_len, sizeof(char));
    XISetMask(m->mask, XI_RawKeyPress);
    XISetMask(m->mask, XI_RawKeyRelease);
    XISetMask(m->mask, XI_RawButtonPress);
    XISetMask(m->mask, XI_RawButtonRelease);
    XISetMask(m->mask, XI_RawMotion);

    XISelectEvents(dpy, win, &mask[0], 2);
    XSync(dpy, False);

    free(mask[0].mask);
    free(mask[1].mask);

    return EXIT_SUCCESS;
}

int
XinputWatcher::print_events()
{

    XEvent ev;
    XGenericEventCookie *cookie = (XGenericEventCookie*)&ev.xcookie;
    XNextEvent(dpy, (XEvent*)&ev);

    if (XGetEventData(dpy, cookie) &&
        cookie->type == GenericEvent)
    {
        printf("EVENT type %d (%s)\n", cookie->evtype, XEventPrinter::type_to_name(cookie->evtype));
        switch (cookie->evtype)
        {
            case XI_DeviceChanged:
                XEventPrinter::devicechanged(dpy, cookie->data);
                break;
            case XI_HierarchyChanged:
                XEventPrinter::hierarchychanged(cookie->data);
                break;
            case XI_RawKeyPress:
            case XI_RawKeyRelease:
            case XI_RawButtonPress:
            case XI_RawButtonRelease:
            case XI_RawMotion:
            case XI_RawTouchBegin:
            case XI_RawTouchUpdate:
            case XI_RawTouchEnd:
                XEventPrinter::raw(cookie->data);
                break;
            case XI_Enter:
            case XI_Leave:
            case XI_FocusIn:
            case XI_FocusOut:
                XEventPrinter::enterleave(cookie->data);
                break;
            case XI_PropertyEvent:
                XEventPrinter::property(dpy, cookie->data);
                break;
            default:
                XEventPrinter::device(cookie->data);
                break;
        }
    }

    XFreeEventData(dpy, cookie);

    return EXIT_SUCCESS;
}