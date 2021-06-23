# -*- Makefile -*-

all: linalg.o list.o geom.o strfuncs.o

linalg.o: linalg.c linalg.h
	gcc -c linalg.c

list.o: list.c list.h
	gcc -c list.c

geom.o: geom.c geom.h
	gcc -c geom.c

strfuncs.o: strfuncs.c strfuncs.h
	gcc -c strfuncs.c

clean:
	rm -f *.o
