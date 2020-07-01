FLAGS = -Wall -Wextra -pedantic -std=c11
CC = gcc

default: libgol.so gameoflife.o 
	${CC} ${FLAGS} libgol.so gameoflife.o -o gameoflife

gameoflife.o: gameoflife.c
	${CC} ${FLAGS} -c -fPIC gameoflife.c

gol.o: gol.c gol.h
	${CC} ${FLAGS} -c -fPIC gol.c

libgol.so: gol.o
	${CC} ${FLAGS} -shared -o libgol.so gol.o -lm

clean:
	-rm *.o gameoflife libgol.so