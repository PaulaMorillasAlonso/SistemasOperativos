CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: baloncesto

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: baloncesto.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm baloncesto baloncesto.o
