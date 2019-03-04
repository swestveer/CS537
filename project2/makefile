SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

COMPILER = gcc 
CFLAGS = -Wall -Wextra
EXE = prodcomm 
LFLAGS= -lpthread

$(EXE): $(OBJS) 
	$(COMPILER) -o $@ $^ $(CFLAGS) $(LFLAGS) 

%.o: %.c
	$(COMPILER) $(CFLAGS) -c $<

clean:
	-rm -f $(OBJS) $(EXE)

