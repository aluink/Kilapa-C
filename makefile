objects = main.o

kilapa : $(objects)
	cc -o kilapa $(objects)

main.o : main.c

.PHONY : clean
clean :
	rm -f $(objects)
