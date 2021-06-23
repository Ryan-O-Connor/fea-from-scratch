#include <stdio.h>
#include "lib/list.h"
#include "lib/linalg.h"
#include "mesh.h"
#include "model.h"
#include "solver.h"


void print_nodal_soln(struct list* nodes, struct static_soln* sol){
  int i, j, P, c;
  for (i=0; i<nodes->nitems; i++){
    for (j=0; j<sol->ndof; j++){
      c = j == 0 ? 'x' : 'y';
      P = sol->ID->array[i][j];
      if (P != -1)
	printf("Node %d: %c deflection: %g \n", i, c, sol->U->array[P]);
      else
        printf("Node %d: %c dof was constrained\n", i, c);
    }
  }
}
