CC = g++
CFLAGS = -Wall -g -std=c++0x
LFLAGS = -L ~/local/lib
INCLUDES = -I ~/local/include


TESTSRCS := $(wildcard test/*.cpp)
TESTOBJS := $(TESTSRCS:.cpp=.o) 

all : src/parser test/testRunner

test : test/testRunner

test/testRunner : src/parser $(TESTOBJS)
	$(CC) $(LFLAGS) -o test/testRunner src/jsValue.o src/jsObject.o src/scanner.o src/driver.o src/parser.o $(TESTOBJS) -lmomoLogging

test/%.o: test/%.cpp test/%.hpp
	$(CC) -c $(INCLUDES) $(CFLAGS) $<
	mv *.o test

src/parser : src/scanner.o src/main.o src/driver.o src/parser.o src/jsValue.o src/jsObject.o
	cd src; $(CC) -o parser main.o parser.o scanner.o driver.o

src/scanner.o : src/scanner.cpp src/driver.hpp src/parser.tab.hh
	cd src; $(CC) $(CFLAGS) -c scanner.cpp

src/main.o : src/main.cpp src/driver.hpp
	cd src; $(CC) $(CFLAGS) -c main.cpp

src/driver.o : src/driver.cpp src/parser.tab.hh src/driver.hpp
	cd src; $(CC) $(CFLAGS) -c driver.cpp

src/parser.o : src/parser.tab.cc
	cd src; $(CC) $(CFLAGS) -c -o parser.o parser.tab.cc

src/parser.tab.cc src/parser.tab.hh src/position.hh src/location.hh src/stack.hh : src/parser.yy
	cd src; bison parser.yy

src/scanner.cpp: src/scanner.l
	flex -o src/scanner.cpp src/scanner.l

src/jsValue.o: src/jsValue.cpp src/jsValue.hpp
	cd src; $(CC) $(CFLAGS) -c jsValue.cpp

src/jsObject.o: src/jsObject.cpp src/jsObject.hpp
	cd src; $(CC) $(CFLAGS) -c jsObject.cpp


clean:
	cd src; rm *.o;  rm *.tab.* stack.hh location.hh position.hh parser scanner.cpp
