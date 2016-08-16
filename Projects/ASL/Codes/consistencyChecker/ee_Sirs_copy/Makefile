CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I.

SRCS = \
	main/main.cpp  \

OBJS = $(SRCS:%.cpp=%.o)

SUBDIRS = \
	om     \
	parser \

LIBS = om/om.lib parser/parser.lib

ee.exe: libs $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS) -lstdc++

libs:
	$(MAKE) -C om
	$(MAKE) -C parser

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	$(MAKE) -C om clean
	$(MAKE) -C parser clean
	rm -f $(OBJS) ee.exe
	rm -f cscope.out
