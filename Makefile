CC=g++
CFLAGS=-fpermissive
LDFLAGS=-lX11 -lXi -lm -lpthread 
EXEC=build/run
ENTRY=src/main.cpp

all: ./$(EXEC)

compile:
	$(CC) $(CFLAGS) $(ENTRY) -o $(EXEC) $(LDFLAGS)

run: compile
	./$(EXEC)