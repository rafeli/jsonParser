CC = g++ 
CFLAGS = -Wall -g -std=c++0x
LFLAGS = -L ~/local/lib
INCLUDES = -I ~/local/include


TESTSRCS := $(wildcard test/*.cpp)
TESTOBJS := $(TESTSRCS:.cpp=.o)
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)

all : test/testRunner

install : $(OBJS)
	cp src/*.hpp ~/local/include/momo
	ar rvs ~/local/lib/libjson.a $(OBJS)

test/testRunner : install $(OBJS) $(TESTOBJS)
	$(CC) $(LFLAGS) -o test/testRunner $(TESTOBJS) -lmomoLogging -ljson -lmomoHTTP

test/%.o: test/%.cpp test/%.hpp
	$(CC) -o $@ -c $(INCLUDES) $(CFLAGS) $<

src/%.o: src/%.cpp src/%.hpp
	$(CC) -o $@ -c $(INCLUDES) $(CFLAGS) $<


clean:
	cd src; rm -f *.o;  rm -f *.tab.* *.swp stack.hh location.hh position.hh scanner.cpp;
	cd test; rm -f *.o;
