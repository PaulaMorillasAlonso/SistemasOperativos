CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: filosofos

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: filosofos.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm filosofos filosofos.o
