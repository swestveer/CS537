SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,%.o,$(SRCS))

COMPILER = gcc 
CFLAGS = -Wall -Wextra
EXE = 537ps 

$(EXE): $(OBJS) 
	$(COMPILER) $(CFLAGS) $(LFLAGS) -o $@ $^

%.o: %.c
	$(COMPILER) $(CFLAGS) $(LFLAGS) -c $<

clean:
	-rm -f $(OBJS) $(EXE)

