#include "XEventPrinter.h"

const char* 
XEventPrinter::type_to_name(int evtype)
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
XEventPrinter::print_devicechangedevent(Display *dpy, XIDeviceChangedEvent *event)
{
    printf("    device: %d (%d)\n", event->deviceid, event->sourceid);
    printf("    reason: %s\n", (event->reason == XISlaveSwitch) ? "SlaveSwitch" :
                                "DeviceChanged");
}

void 
XEventPrinter::print_hierarchychangedevent(XIHierarchyEvent *event)
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
XEventPrinter::print_rawevent(XIRawEvent *event)
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
XEventPrinter::print_enterleave(XILeaveEvent* event)
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
XEventPrinter::print_propertyevent(Display *display, XIPropertyEvent* event)
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
XEventPrinter::print_deviceevent(XIDeviceEvent* event)
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

