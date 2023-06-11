CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: ordenados

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: ordenados.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm ordenados ordenados.o
