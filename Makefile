CC=g++
LFLAGS=-lsqlite3
CFLAGS=-I .
OBJ_SRC=$(wildcard src/lib/*.cpp)
RM=rm
VERSION=0.2.0.1
OBJ=$(OBJ_SRC:src/lib/%.cpp=bin/lib%.o)
.PHONY: all
all: bin/main
bin/main: src/main.cpp $(OBJ) |include/version.h
	$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)
	$(RM) include/version.h
bin/lib%.o: src/lib/%.cpp |bin
	$(CC) -o $@ -c $< $(CFLAGS)
include/version.h:
	echo "const char VERSION[]=\"$(VERSION)\";">$@
.PHONY:print
print:
	echo "$(OBJ_SRC)\n$(OBJ)\n"
bin:
	mkdir bin
.PHONY: run
run:
	cd bin;./main
