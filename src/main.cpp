#include <stdio.h>
#include <stdlib.h>			

#include "XinputAddon/XinputWatcher.cpp"

using namespace std;

int main( int argc, const char* argv[] )
{
    XinputWatcher watcher = XinputWatcher();
	std::string inputId = "15";  
	watcher.watch(inputId);
    return EXIT_SUCCESS;
}