all: demo test


test: test.o myalloc.o
	gcc test.o myalloc.o -o test

demo: demo.o myalloc.o
	gcc demo.o myalloc.o -o demo

test.o: test.c myalloc.h
	gcc -c test.c

demo.o: demo.c myalloc.h
	gcc -c demo.c

myalloc.o: myalloc.c myalloc.h
	gcc -c myalloc.c

clean:
	rm -f *.o

proper: clean
	rm -rf test demo



