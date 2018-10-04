CC=g++
CFLAGS=-fpermissive 
LDFLAGS=-fpermissive -lX11 -lXi -lm -lpthread
EXEC=build/run
ENTRY=src/main.cpp

all: ./$(EXEC)

compile:
	$(CC) $(ENTRY) -o $(EXEC) $(LDFLAGS)

run: compile
	./$(EXEC)