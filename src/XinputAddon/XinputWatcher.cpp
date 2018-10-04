#include "XinputWatcher.h"

Display XinputWatcher::*dpy;
XDeviceInfo *info = NULL;
Window win;

XIEventMask mask[2];
XIEventMask *m;

XinputWatcher::XinputWatcher()
{
	dpy = XOpenDisplay(NULL);
	win = DefaultRootWindow(dpy);
}

void XinputWatcher::watch(std::string inputID)
{
	// cast std:string to char*
	char *inputIdChar = new char[inputID.length() + 1];
	strcpy(inputIdChar, inputID.c_str());

	info = find_device_info(inputIdChar, True);

	if (!info)
	{
		fprintf(stderr, "unable to find device '%s'\n", inputIdChar);
	}
	else
	{
		test_xi2(dpy, 15);
	}
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
XinputWatcher::test_xi2(Display	*display,
         int deviceid)
{
    XIEventMask mask[2];
    XIEventMask *m;
    Window win;

    setvbuf(stdout, NULL, _IOLBF, 0);
	
	win = DefaultRootWindow(display);

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

    XISelectEvents(display, win, &mask[0], 2);
    XSync(display, False);

    free(mask[0].mask);
    free(mask[1].mask);

    /*
    test_sync_grab(display, win);
    */

    while(1)
    {
        XEvent ev;
        XGenericEventCookie *cookie = (XGenericEventCookie*)&ev.xcookie;
        XNextEvent(display, (XEvent*)&ev);

        if (XGetEventData(display, cookie) &&
            cookie->type == GenericEvent)
        {
            printf("EVENT type %d (%s)\n", cookie->evtype, type_to_name(cookie->evtype));
            switch (cookie->evtype)
            {
                case XI_DeviceChanged:
                    print_devicechangedevent(display, cookie->data);
                    break;
                case XI_HierarchyChanged:
                    print_hierarchychangedevent(cookie->data);
                    break;
                case XI_RawKeyPress:
                case XI_RawKeyRelease:
                case XI_RawButtonPress:
                case XI_RawButtonRelease:
                case XI_RawMotion:
                case XI_RawTouchBegin:
                case XI_RawTouchUpdate:
                case XI_RawTouchEnd:
                    print_rawevent(cookie->data);
                    break;
                case XI_Enter:
                case XI_Leave:
                case XI_FocusIn:
                case XI_FocusOut:
                    print_enterleave(cookie->data);
                    break;
                case XI_PropertyEvent:
                    print_propertyevent(display, cookie->data);
                    break;
                default:
                    print_deviceevent(cookie->data);
                    break;
            }
        }

        XFreeEventData(display, cookie);
    }

    XDestroyWindow(display, win);

    return EXIT_SUCCESS;
}

const char* 
XinputWatcher::type_to_name(int evtype)
{
    const char *name;

    switch(evtype) {
        case XI_DeviceChanged:    name = "DeviceChanged";       break;
        case XI_KeyPress:         name = "KeyPress";            break;
        case XI_KeyRelease:       name = "KeyRelease";          break;
        case XI_ButtonPress:      name = "ButtonPress";         break;
        case XI_ButtonRelease:    name = "ButtonRelease";       break;
        case XI_Motion:           name = "Motion";              break;
        case XI_Enter:            name = "Enter";               break;
        case XI_Leave:            name = "Leave";               break;
        case XI_FocusIn:          name = "FocusIn";             break;
        case XI_FocusOut:         name = "FocusOut";            break;
        case XI_HierarchyChanged: name = "HierarchyChanged";    break;
        case XI_PropertyEvent:    name = "PropertyEvent";       break;
        case XI_RawKeyPress:      name = "RawKeyPress";         break;
        case XI_RawKeyRelease:    name = "RawKeyRelease";       break;
        case XI_RawButtonPress:   name = "RawButtonPress";      break;
        case XI_RawButtonRelease: name = "RawButtonRelease";    break;
        case XI_RawMotion:        name = "RawMotion";           break;
        case XI_TouchBegin:       name = "TouchBegin";          break;
        case XI_TouchUpdate:      name = "TouchUpdate";         break;
        case XI_TouchEnd:         name = "TouchEnd";            break;
        case XI_RawTouchBegin:    name = "RawTouchBegin";       break;
        case XI_RawTouchUpdate:   name = "RawTouchUpdate";      break;
        case XI_RawTouchEnd:      name = "RawTouchEnd";         break;
        default:
                                  name = "unknown event type"; break;
    }
    return name;
}

void 
XinputWatcher::print_devicechangedevent(Display *dpy, XIDeviceChangedEvent *event)
{
    printf("    device: %d (%d)\n", event->deviceid, event->sourceid);
    printf("    reason: %s\n", (event->reason == XISlaveSwitch) ? "SlaveSwitch" :
                                "DeviceChanged");
}

void 
XinputWatcher::print_hierarchychangedevent(XIHierarchyEvent *event)
{
    int i;
    printf("    Changes happened: %s %s %s %s %s %s %s %s\n",
            (event->flags & XIMasterAdded) ? "[new master]" : "",
            (event->flags & XIMasterRemoved) ? "[master removed]" : "",
            (event->flags & XISlaveAdded) ? "[new slave]" : "",
            (event->flags & XISlaveRemoved) ? "[slave removed]" : "",
            (event->flags & XISlaveAttached) ? "[slave attached]" : "",
            (event->flags & XISlaveDetached) ? "[slave detached]" : "",
            (event->flags & XIDeviceEnabled) ? "[device enabled]" : "",
            (event->flags & XIDeviceDisabled) ? "[device disabled]" : "");

    for (i = 0; i < event->num_info; i++)
    {
        char *use = "<undefined>";
        switch(event->info[i].use)
        {
            case XIMasterPointer: use = "master pointer"; break;
            case XIMasterKeyboard: use = "master keyboard"; break;
            case XISlavePointer: use = "slave pointer"; break;
            case XISlaveKeyboard: use = "slave keyboard"; break;
            case XIFloatingSlave: use = "floating slave"; break;
                break;
        }

        printf("    device %d [%s (%d)] is %s\n",
                event->info[i].deviceid,
                use,
                event->info[i].attachment,
                (event->info[i].enabled) ? "enabled" : "disabled");
        if (event->info[i].flags)
        {
            printf("    changes: %s %s %s %s %s %s %s %s\n",
                    (event->info[i].flags & XIMasterAdded) ? "[new master]" : "",
                    (event->info[i].flags & XIMasterRemoved) ? "[master removed]" : "",
                    (event->info[i].flags & XISlaveAdded) ? "[new slave]" : "",
                    (event->info[i].flags & XISlaveRemoved) ? "[slave removed]" : "",
                    (event->info[i].flags & XISlaveAttached) ? "[slave attached]" : "",
                    (event->info[i].flags & XISlaveDetached) ? "[slave detached]" : "",
                    (event->info[i].flags & XIDeviceEnabled) ? "[device enabled]" : "",
                    (event->info[i].flags & XIDeviceDisabled) ? "[device disabled]" : "");
        }
    }
}

void
XinputWatcher::print_rawevent(XIRawEvent *event)
{
    int i;
    double *val, *raw_val;

    printf("    device: %d (%d)\n", event->deviceid, event->sourceid);
    printf("    detail: %d\n", event->detail);

    printf("    valuators:\n");
    val = event->valuators.values;
    raw_val = event->raw_values;
    for (i = 0; i < event->valuators.mask_len * 8; i++)
        if (XIMaskIsSet(event->valuators.mask, i))
            printf("         %2d: %.2f (%.2f)\n", i, *val++, *raw_val++);
    printf("\n");
}

void 
XinputWatcher::print_enterleave(XILeaveEvent* event)
{
    char *mode = "<undefined>",
         *detail = "<undefined>";
    int i;

    printf("    device: %d (%d)\n", event->deviceid, event->sourceid);
    printf("    windows: root 0x%lx event 0x%lx child 0x%ld\n",
            event->root, event->event, event->child);
    switch(event->mode)
    {
        case XINotifyNormal:       mode = "NotifyNormal"; break;
        case XINotifyGrab:         mode = "NotifyGrab"; break;
        case XINotifyUngrab:       mode = "NotifyUngrab"; break;
        case XINotifyWhileGrabbed: mode = "NotifyWhileGrabbed"; break;
        case XINotifyPassiveGrab:  mode = "NotifyPassiveGrab"; break;
        case XINotifyPassiveUngrab:mode = "NotifyPassiveUngrab"; break;
    }
    switch (event->detail)
    {
        case XINotifyAncestor: detail = "NotifyAncestor"; break;
        case XINotifyVirtual: detail = "NotifyVirtual"; break;
        case XINotifyInferior: detail = "NotifyInferior"; break;
        case XINotifyNonlinear: detail = "NotifyNonlinear"; break;
        case XINotifyNonlinearVirtual: detail = "NotifyNonlinearVirtual"; break;
        case XINotifyPointer: detail = "NotifyPointer"; break;
        case XINotifyPointerRoot: detail = "NotifyPointerRoot"; break;
        case XINotifyDetailNone: detail = "NotifyDetailNone"; break;
    }
    printf("    mode: %s (detail %s)\n", mode, detail);
    printf("    flags: %s %s\n", event->focus ? "[focus]" : "",
                                 event->same_screen ? "[same screen]" : "");
    printf("    buttons:");
    for (i = 0; i < event->buttons.mask_len * 8; i++)
        if (XIMaskIsSet(event->buttons.mask, i))
            printf(" %d", i);
    printf("\n");

    printf("    modifiers: locked %#x latched %#x base %#x effective: %#x\n",
            event->mods.locked, event->mods.latched,
            event->mods.base, event->mods.effective);
    printf("    group: locked %#x latched %#x base %#x effective: %#x\n",
            event->group.locked, event->group.latched,
            event->group.base, event->group.effective);

    printf("    root x/y:  %.2f / %.2f\n", event->root_x, event->root_y);
    printf("    event x/y: %.2f / %.2f\n", event->event_x, event->event_y);

}

void 
XinputWatcher::print_propertyevent(Display *display, XIPropertyEvent* event)
{
    char *changed;
    char *name;

    if (event->what == XIPropertyDeleted)
        changed = "deleted";
    else if (event->what == XIPropertyCreated)
        changed = "created";
    else
        changed = "modified";
    name = XGetAtomName(display, event->property);
    printf("     property: %ld '%s'\n", event->property, name);
    printf("     changed: %s\n", changed);

    XFree(name);
}

void 
XinputWatcher::print_deviceevent(XIDeviceEvent* event)
{
    double *val;
    int i;

    printf("    device: %d (%d)\n", event->deviceid, event->sourceid);
    printf("    detail: %d\n", event->detail);
    switch(event->evtype) {
        case XI_KeyPress:
        case XI_KeyRelease:
            printf("    flags: %s\n", (event->flags & XIKeyRepeat) ?  "repeat" : "");
            break;
    }

    printf("    root: %.2f/%.2f\n", event->root_x, event->root_y);
    printf("    event: %.2f/%.2f\n", event->event_x, event->event_y);

    printf("    buttons:");
    for (i = 0; i < event->buttons.mask_len * 8; i++)
        if (XIMaskIsSet(event->buttons.mask, i))
            printf(" %d", i);
    printf("\n");

    printf("    modifiers: locked %#x latched %#x base %#x effective: %#x\n",
            event->mods.locked, event->mods.latched,
            event->mods.base, event->mods.effective);
    printf("    group: locked %#x latched %#x base %#x effective: %#x\n",
            event->group.locked, event->group.latched,
            event->group.base, event->group.effective);
    printf("    valuators:\n");

    val = event->valuators.values;
    for (i = 0; i < event->valuators.mask_len * 8; i++)
        if (XIMaskIsSet(event->valuators.mask, i))
            printf("        %i: %.2f\n", i, *val++);

    printf("    windows: root 0x%lx event 0x%lx child 0x%lx\n",
            event->root, event->event, event->child);
}

