CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: poker

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: poker.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm poker poker.o
