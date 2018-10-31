#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <getopt.h>

#include "XinputAddon/XinputWatcher/XinputWatcher.h"

using namespace std;

int
main (int argc, char **argv)
{
  int c;
  opterr = 0;

  XinputWatcher watcher = XinputWatcher();
  XEventPrinter printer = XEventPrinter();

  while( ( c = getopt (argc, argv, "lvw:") ) != -1 ) 
    switch (c)
    {
      case 'l':
        cout << "list " <<  endl;
        return EXIT_SUCCESS;
        break;
      case 'v':
        cout << "version " <<  endl;
        return EXIT_SUCCESS;
        break;
      case 'w':
        if(optarg) {
          cout << "watch " << optarg << endl;
          watcher.watch(optarg);
        } else {
          cerr << "Need identifier" << endl;
        }        
        break;
    }

  int i = 0;
	while(true){
      watcher.print_events(printer);
      cout << printer.flush();
      i++;
  }

  return EXIT_SUCCESS;
}
