#include <stdio.h>
#include <stdlib.h>		

#include "XinputAddon/XinputWatcher/XinputWatcher.cpp"

using namespace std;

int main( int argc, const char* argv[] )
{
    XinputWatcher watcher = XinputWatcher();
	std::string inputId = "15";  
	watcher.watch(inputId);

    int i = 0;
	while(true){
        watcher.print_events(watcher.dpy);
        i++;
    }

    return EXIT_SUCCESS;
}