CFLAGS=-Wall -O2

objects = main.o board.o core.o

kilapa : $(objects)
	cc -o kilapa $(objects)

main.o : main.c

core.o : core.h core.c
board.o : board.h board.c

.PHONY : clean
clean :
	rm -f $(objects)
