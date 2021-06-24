# -*- Makefile -*-

all: linalg.o list.o geom.o strfuncs.o

linalg.o: linalg.c linalg.h
	gcc -c -g linalg.c

list.o: list.c list.h
	gcc -c -g list.c

geom.o: geom.c geom.h
	gcc -c -g geom.c

strfuncs.o: strfuncs.c strfuncs.h
	gcc -c -g strfuncs.c

clean:
	rm -f *.o *~
