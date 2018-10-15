#include <stdio.h>
#include <stdlib.h>		

#include "XinputAddon/XinputWatcher/XinputWatcher.h"

using namespace std;

int main( int argc, const char* argv[] )
{
    XinputWatcher watcher = XinputWatcher();

    // define id. 
	watcher.watch("18");
    // watcher.watch("18"); you can define many ids
    
    XEventPrinter printer = XEventPrinter();
    int i = 0;
	while(true){
        watcher.print_events(printer);
        cout << printer.flush();
        i++;
    }

    return EXIT_SUCCESS;
}