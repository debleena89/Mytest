CC = gcc
CFLAGS = -g -Wall
INCLUDES = -I.

SRCS = \
	eeExpr.cpp        \
	eeConstExpr.cpp   \
	eeNamedExpr.cpp   \
	eeUnaryExpr.cpp   \
	eeBinaryExpr.cpp  \
	eeTernaryExpr.cpp \

OBJS = $(SRCS:%.cpp=%.o)

om.lib: $(OBJS)
	ld -r -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

clean:
	rm -f $(OBJS) om.lib
