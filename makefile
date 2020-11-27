CFLAGS=-Wall -O2

objects = main.o board.o magic.o

kilapa : $(objects)
	cc -o kilapa $(objects)

main.o : main.c

magic.o : magic.c magic.h
board.o : board.h board.c

.PHONY : clean
clean :
	rm -f $(objects)
