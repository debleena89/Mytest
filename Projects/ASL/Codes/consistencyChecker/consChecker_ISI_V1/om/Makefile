CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I.

SRCS = \
	eeExpr.cpp        \
	eeValue.cpp      \
	eeNamedExpr.cpp   \
        eeBinaryExpr.cpp  \
        eeUniExpr.cpp\
	

OBJS = $(SRCS:%.cpp=%.o)

om.lib: $(OBJS)
	ld -r -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(OBJS) om.lib
