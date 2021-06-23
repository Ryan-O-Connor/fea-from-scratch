#include <stdlib.h>
#include <stdio.h>
#include "lib/list.h"
#include "bc_data.h"


struct essential_bc* new_essential_bc(int node_id, int dof, double value){
  struct essential_bc* ebc = malloc(sizeof(struct essential_bc));
  ebc->node_id = node_id;
  ebc->dof = dof;
  ebc->value = value;
  return ebc;
}


struct nodal_force* new_nodal_force(int node_id, int dof, double value){
  struct nodal_force* ndf = malloc(sizeof(struct nodal_force));
  ndf->node_id = node_id;
  ndf->dof = dof;
  ndf->value = value;
  return ndf;
}


double get_essential_bc(struct list* essential_bcs, int node_id, int dof){
  int i;
  struct essential_bc* ebc;
  for (i=0; i<essential_bcs->nitems; i++){
    ebc = essential_bcs->array[i];
    if (ebc->node_id == node_id && ebc->dof == dof)
      return ebc->value;
  }
  printf("Error: No boundary condition found for node %d at dof %d\n",
	 node_id, dof);
  exit(1);
}


double get_nodal_force(struct list* nodal_forces, int node_id, int dof){
  int i;
  struct nodal_force* ndf;
  for (i=0; i<nodal_forces->nitems; i++){
    ndf = nodal_forces->array[i];
    if (ndf->node_id == node_id && ndf->dof == dof)
      return ndf->value;
  }
  return 0.0;
}


void print_essential_bc(struct essential_bc* ebc){
  printf("Essential boundary condition: Node=%d, Dof=%d, Value=%g\n",
	 ebc->node_id, ebc->dof, ebc->value);
}


void print_nodal_force(struct nodal_force* ndf){
  printf("Nodal force: Node=%d, Dof=%d, Value=%g\n",
	 ndf->node_id, ndf->dof, ndf->value);
}


void free_essential_bc(void* ebc){
  struct essential_bc* EBC = ebc;
  free(EBC);
}


void free_nodal_force(void* ndf){
  struct nodal_force* NDF = ndf;
  free(NDF);
}


int is_constrained(struct list* essential_bcs, int node_id, int dof){
  int i;
  struct essential_bc* ebc;
  for (i=0; i<essential_bcs->nitems; i++){
    ebc = essential_bcs->array[i];
    if (ebc->node_id == node_id && ebc->dof == dof)
      return 1;
  }
  return 0;
}
