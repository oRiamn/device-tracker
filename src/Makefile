CC=g++
CFLAGS=-fpermissive
LDFLAGS=-lX11 -lXi -lm -lpthread 
EXEC=/app/build/run
ENTRY=main.cpp

.DEFAULT_GOAL := compile

compile:
	$(CC) $(CFLAGS) $(ENTRY) -o $(EXEC) $(LDFLAGS)
	chmod 777 $(EXEC)