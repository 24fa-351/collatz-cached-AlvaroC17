CC=gcc
CFLAGS=-Wall -g
OBJ=collatz.o cache.o

collatz: $(OBJ)
	$(CC) -o $@ $(OBJ)

collatz.o: collatz.c cache.h
cache.o: cache.c cache.h

clean:
	rm -f *.o collatz
