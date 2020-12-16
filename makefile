CFLAGS=-Wall -O2

objects = main.o board.o core.o kilapa.o

kilapa : $(objects)
	cc -o kilapa $(objects)

main.o : main.c

core.o : core.h core.c
board.o : board.h board.c
kilapa.o : kilapa.h kilapa.c

.PHONY : clean
clean :
	rm -f $(objects)
