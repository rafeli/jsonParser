CC = g++
CFLAGS = -Wall -g -std=c++0x
LFLAGS = -L ~/local/lib
INCLUDES = -I ~/local/include


TESTSRCS := $(wildcard test/*.cpp)
TESTOBJS := $(TESTSRCS:.cpp=.o)
OBJS     := src/jsValue.o src/jsObject.o src/scanner.o src/json.o src/parser.o

all : test/testRunner

install : $(OBJS)
	cp src/*.hpp src/*.hh ~/local/include/momo
	ar rvs ~/local/lib/libjson.a $(OBJS)

test/testRunner : install $(OBJS) $(TESTOBJS)
	$(CC) $(LFLAGS) -o test/testRunner $(TESTOBJS) -lmomoLogging -ljson

test/%.o: test/%.cpp test/%.hpp
	$(CC) -c $(INCLUDES) $(CFLAGS) $<
	mv *.o test

src/scanner.o : src/scanner.cpp src/json.hpp src/parser.tab.hh
	cd src; $(CC) $(CFLAGS) -c scanner.cpp

src/json.o : src/json.cpp src/parser.tab.hh src/json.hpp
	cd src; $(CC) $(CFLAGS) -c json.cpp

src/parser.o : src/parser.tab.cc
	cd src; $(CC) $(CFLAGS) -c -o parser.o parser.tab.cc

src/parser.tab.cc src/parser.tab.hh src/position.hh src/location.hh src/stack.hh : src/parser.yy
	cd src; bison parser.yy

src/scanner.cpp: src/scanner.l src/json.hpp
	flex -o src/scanner.cpp src/scanner.l

src/jsValue.o: src/jsValue.cpp src/jsValue.hpp
	cd src; $(CC) $(CFLAGS) -c jsValue.cpp

src/jsObject.o: src/jsObject.cpp src/jsObject.hpp
	cd src; $(CC) $(CFLAGS) -c jsObject.cpp


clean:
	cd src; rm *.o;  rm *.tab.* stack.hh location.hh position.hh scanner.cpp
