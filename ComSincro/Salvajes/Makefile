CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: salvajes

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: salvajes.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm salvajes salvajes.o
