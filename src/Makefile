# -*- Makefile -*-

objects = main.o interpreter.o model.o mesh.o element_types.o bc_data.o solver.o stiffness.o shape.o post.o lib/strfuncs.o lib/list.o lib/linalg.o lib/geom.o

all: myfea

myfea: $(objects)
	gcc -o myfea $(objects) -lm

main.o: main.c model.h interpreter.h
	gcc -c main.c

interpreter.o: interpreter.c interpreter.h model.h lib/strfuncs.h
	gcc -c interpreter.c

model.o: model.c model.h mesh.h element_types.h bc_data.h \
		solver.h post.h lib/list.h
	gcc -c model.c

mesh.o: mesh.c mesh.h lib/list.h
	gcc -c mesh.c

element_types.o: element_types.c element_types.h lib/list.h lib/geom.h
	gcc -c element_types.c

bc_data.o: bc_data.c bc_data.h lib/list.h
	gcc -c bc_data.c

solver.o: solver.c solver.h model.h mesh.h element_types.h bc_data.h \
		stiffness.h lib/list.h lib/linalg.h
	gcc -c solver.c

stiffness.o: stiffness.c stiffness.h element_types.h lib/linalg.h
	gcc -c stiffness.c

shape.o: shape.c shape.h lib/linalg.h lib/geom.h
	gcc -c shape.c

post.o: post.c post.h mesh.h model.h solver.h lib/list.h lib/linalg.h
	gcc -c post.c
