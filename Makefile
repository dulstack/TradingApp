CC=g++
LFLAGS=-lsqlite3
CFLAGS=-I .
OBJ_SRC=$(wildcard src/lib/*.cpp)
RM=rm
VERSION=0.2.1
OBJ=$(OBJ_SRC:src/lib/%.cpp=bin/lib%.o)
DEBUG_OBJ=$(OBJ_SRC:src/lib/%.cpp=bin/lib%-debug.o)
.PHONY: all
all: bin/main
.PHONY: debug
debug: src/main.cpp $(DEBUG_OBJ) | include/version.h
	$(CC) -o bin/main -g $(CFLAGS) -DDEBUG $^ -lsqlite3
	$(RM) include/version.h
bin/main: src/main.cpp $(OBJ) |include/version.h
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)
	$(RM) include/version.h
bin/lib%.o: src/lib/%.cpp |bin
	$(CC) -o $@ -c $< $(CFLAGS)
bin/lib%-debug.o: src/lib/%.cpp |bin
	$(CC) -c -o $@ -g $(CFLAGS) -DDEBUG $<
include/version.h:
	echo "const char VERSION[]=\"$(VERSION)\";">$@
.PHONY:print
bin:
	mkdir bin
.PHONY: run
run:
	cd bin;./main
