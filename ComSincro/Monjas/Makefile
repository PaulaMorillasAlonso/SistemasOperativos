CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: monjas

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: monjas.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm monjas monjas.o
