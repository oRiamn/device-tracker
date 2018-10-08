std::stringstream buffer;

XEventPrinter::XEventPrinter()
{  
}

std::string 
XEventPrinter::flush()
{
    std::string value = buffer.str();
    buffer.str("");
    
    return value;
}

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
XEventPrinter::devicechanged(Display *dpy, XIDeviceChangedEvent *event)
{
    buffer << "    device: " << event->deviceid << " (" << event->sourceid << ")" << std::endl;
    buffer << "    reason: ";
        buffer<< (event->reason == XISlaveSwitch ? "SlaveSwitch" : "DeviceChanged");
    buffer << std::endl;
}

void 
XEventPrinter::hierarchychanged(XIHierarchyEvent *event)
{
    int i;

    buffer << "    Changes happened: ";
        buffer << (event->info[i].flags & XIMasterAdded ? "[new master]" : "");
        buffer << (event->info[i].flags & XIMasterRemoved ? "[master removed]" : "");
        buffer << (event->info[i].flags & XISlaveAdded ? "[new slave]" : "");
        buffer << (event->info[i].flags & XISlaveRemoved ? "[slave removed]" : "");
        buffer << (event->info[i].flags & XISlaveAttached ? "[slave attached]" : "");
        buffer << (event->info[i].flags & XISlaveDetached ? "[slave detached]" : "");
        buffer << (event->info[i].flags & XIDeviceEnabled ? "[device enabled]" : "");
        buffer << (event->info[i].flags & XIDeviceDisabled ? "[device disabled]" : "");
    buffer << std::endl;

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
        
        buffer << "    device " << event->info[i].deviceid;
            buffer << "["  << use << " (" << event->info[i].attachment << ")]";
            buffer << " is " << (event->info[i].enabled ? "enabled" : "disabled");
        buffer << std::endl;
        
        if (event->info[i].flags)
        {   
            buffer << "    changes: ";
                buffer << (event->info[i].flags & XIMasterAdded ? "[new master]" : "");
                buffer << (event->info[i].flags & XIMasterRemoved ? "[master removed]" : "");
                buffer << (event->info[i].flags & XISlaveAdded ? "[new slave]" : "");
                buffer << (event->info[i].flags & XISlaveRemoved ? "[slave removed]" : "");
                buffer << (event->info[i].flags & XISlaveAttached ? "[slave attached]" : "");
                buffer << (event->info[i].flags & XISlaveDetached ? "[slave detached]" : "");
                buffer << (event->info[i].flags & XIDeviceEnabled ? "[device enabled]" : "");
                buffer << (event->info[i].flags & XIDeviceDisabled ? "[device disabled]" : "");
            buffer << std::endl;
        }
    }
}

void
XEventPrinter::raw(XIRawEvent *event)
{
    int i;
    double *val, *raw_val;

    buffer << "    device: " << event->deviceid << " " << event->sourceid << std::endl;
    buffer << "    detail: " << event->detail << std::endl;

    buffer << "    valuators:" << std::endl;
    val = event->valuators.values;
    raw_val = event->raw_values;
    for (i = 0; i < event->valuators.mask_len * 8; i++)
        if (XIMaskIsSet(event->valuators.mask, i))
            buffer << "         " << i << ": " << *val++ << "(" << *raw_val++ << ")"<< std::endl;
   
   buffer <<  std::endl;
}

void 
XEventPrinter::enterleave(XILeaveEvent* event)
{
    char *mode = "<undefined>",
         *detail = "<undefined>";
    int i;

    buffer << "    device: " << event->deviceid << " " << event->sourceid << std::endl;

    buffer << "    windows: root" << event->root;
        buffer << " event " << event->event;
        buffer << " child " << event->child;
    buffer << std::endl;


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

    buffer << "    mode: " << mode << "(detail " << detail << ")" << std::endl;

    buffer << "    flags: ";
        buffer << (event->focus ? "[focus]" : "" ) << " ";
        buffer <<  (event->same_screen ? "[same screen]" : "");
    buffer << std::endl;

    buffer << "    buttons:";
    for (i = 0; i < event->buttons.mask_len * 8; i++)
        if (XIMaskIsSet(event->buttons.mask, i))
            buffer << " " << i;

    buffer << std::endl;
    
    buffer << "    modifiers: locked " << event->mods.locked << " latched" << event->mods.latched;
        buffer << " base " << event->mods.locked << " effective: " << event->mods.effective;
    buffer << std::endl;
    

    buffer << "    group: locked " << event->group.locked << " latched" << event->group.latched;
        buffer << " base " << event->group.locked << " effective: " << event->group.effective;
    buffer << std::endl;

    buffer << "    root x/y:  " << event->root_x << "/" <<  event->root_y  << std::endl;
    buffer << "    event x/y:  " << event->event_x << "/" <<  event->event_y  << std::endl;
}

void 
XEventPrinter::property(Display *display, XIPropertyEvent* event)
{
    char *changed;
    char *name;

     switch(event->what) {
        case XIPropertyDeleted: changed = "deleted"; break;
        case XIPropertyCreated: changed = "created"; break;
        default:
                                changed = "modified"; break;
     }
    
    name = XGetAtomName(display, event->property);

    buffer << "     property: " << event->property << " " << name << std::endl;

    buffer << "     changed: " << changed << std::endl;

    XFree(name);
}

void 
XEventPrinter::device(XIDeviceEvent* event)
{
    double *val;
    int i;

    buffer << "    device: " << event->deviceid << " " << event->sourceid << std::endl;
    buffer << "    detail: " << event->detail << std::endl;

    switch(event->evtype) {
        case XI_KeyPress:
        case XI_KeyRelease:
            buffer << "    flags: " << ((event->flags & XIKeyRepeat) ?  "repeat" : "") << std::endl;
            break;
    }

    buffer << "    root: " << event->root_x << " " << event->root_y << std::endl;
    buffer << "    event: " << event->event_x << " " << event->event_y << std::endl;

    buffer << "    buttons:" << std::endl;
    for (i = 0; i < event->buttons.mask_len * 8; i++)
        if (XIMaskIsSet(event->buttons.mask, i))
            buffer << " " << i;
    
    buffer << std::endl;


    buffer << "    modifiers: locked " << event->mods.locked << " latched" << event->mods.latched;
        buffer << " base " << event->mods.locked << " effective: " << event->mods.effective;
    buffer << std::endl;
    

    buffer << "    group: locked " << event->group.locked << " latched" << event->group.latched;
        buffer << " base " << event->group.locked << " effective: " << event->group.effective;
    buffer << std::endl;


    buffer << "    valuators:" << std::endl;

    val = event->valuators.values;
    for (i = 0; i < event->valuators.mask_len * 8; i++)
        if (XIMaskIsSet(event->valuators.mask, i))
            buffer << "        " << i << ": " << *val++ << std::endl;

    buffer << "    windows: root" << event->root;
        buffer << " event " << event->event;
        buffer << " child " << event->child;
    buffer << std::endl;
}

