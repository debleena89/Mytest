CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I. -I../om

LEX = flex
YACC = bison

SRCS = \
	eeLex.l  \
	eeYacc.y \

OBJS := $(SRCS:%.l=%.o)
OBJS := $(OBJS:%.y=%.o)

parser.lib: $(OBJS)
	ld -r -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.cpp: %.l
	$(LEX) -o$@ $<

%.cpp: %.y
	$(YACC) -dvt -o $@ $<

eeLex.cpp: eeYacc.cpp

clean:
	rm -f $(OBJS) parser.lib eeLex.cpp eeYacc.cpp eeYacc.hpp eeYacc.output

.SECONDARY: eeLex.cpp eeYacc.cpp eeYacc.hpp eeYacc.output
