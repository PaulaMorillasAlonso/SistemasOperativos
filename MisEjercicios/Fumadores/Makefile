CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: fumadores

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: fumadores.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm fumadores fumadores.o
