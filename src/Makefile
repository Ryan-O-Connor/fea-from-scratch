# -*- Makefile -*-

objects = main.o interpreter.o model.o mesh.o element_types.o bc_data.o solver.o stiffness.o shape.o post.o lib/strfuncs.o lib/list.o lib/linalg.o lib/geom.o

all: myfea

myfea: $(objects)
	gcc -o myfea $(objects) -lm

main.o: main.c model.h interpreter.h
	gcc -c -g main.c

interpreter.o: interpreter.c interpreter.h model.h lib/strfuncs.h
	gcc -c -g interpreter.c

model.o: model.c model.h mesh.h element_types.h bc_data.h \
		solver.h post.h lib/list.h
	gcc -c -g model.c

mesh.o: mesh.c mesh.h lib/list.h
	gcc -c -g mesh.c

element_types.o: element_types.c element_types.h lib/list.h lib/geom.h
	gcc -c -g element_types.c

bc_data.o: bc_data.c bc_data.h lib/list.h
	gcc -c -g bc_data.c

solver.o: solver.c solver.h model.h mesh.h element_types.h bc_data.h \
		stiffness.h lib/list.h lib/linalg.h shape.h
	gcc -c -g solver.c

stiffness.o: stiffness.c stiffness.h element_types.h \
		lib/linalg.h lib/list.h mesh.h shape.h
	gcc -c -g stiffness.c

shape.o: shape.c shape.h lib/linalg.h lib/geom.h lib/list.h \
		mesh.h element_types.h
	gcc -c -g shape.c

post.o: post.c post.h mesh.h model.h solver.h lib/list.h lib/linalg.h
	gcc -c -g post.c

clean:
	rm -f myfea *.o *~
