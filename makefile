objects = main.o board.o

kilapa : $(objects)
	cc -o kilapa $(objects)

main.o : main.c

board.o : board.h board.c

.PHONY : clean
clean :
	rm -f $(objects)
