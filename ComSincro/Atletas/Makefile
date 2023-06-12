CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: atletas

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

disco: atletas.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm atletas atletas.o
