CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: gasolinera

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: gasolinera.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm gasolinera gasolinera.o
