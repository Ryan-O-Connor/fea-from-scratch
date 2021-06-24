#include <stdio.h>
#include <stdlib.h>
#include "lib/list.h"
#include "lib/geom.h"
#include "lib/linalg.h"
#include "model.h"
#include "mesh.h"
#include "element_types.h"
#include "bc_data.h"
#include "stiffness.h"
#include "solver.h"
#include "shape.h"


static void precomputations(struct list* et_defs){
  // Perform any computations that apply to all elements of the same type
  // and store them in the type definition's solver data
  int i, j, lib_id, integration;
  struct et_def* et;
  for (i=0; i<et_defs->nitems; i++){
    et = et_defs->array[i];
    lib_id = et->lib_id;
    integration = et->opts[0];
    if (integrated_element(lib_id)){
      printf("Computing integration values\n");
      et->sdata->nint_pts = get_nint_pts(lib_id, integration);
      et->sdata->int_pts = get_int_pts(lib_id, integration);
      et->sdata->int_wts = get_int_wts(lib_id, integration);
      et->sdata->D = construct_D(et);
      // List of shape function derivatives in natural coordinates for each
      // integration point (e, n).  Stored in convenient array.
      et->sdata->NDERNATs = construct_NDERNATs(et);
    }
  }
}


static void construct_ID(struct list* nodes, int ndof,
			 struct list* essential_bcs, struct matrix* ID){
  printf("Constructing ID matrix\n");
  int i, j, eqn = 0, nnodes = nodes->nitems;
  for (i=0; i<nnodes; i++){
    for (j=0; j<ndof; j++){
      if (is_constrained(essential_bcs, i, j))
	ID->array[i][j] = -1;
      else
	ID->array[i][j] = eqn++;
    }
  }
}


static void assemble_KE(struct matrix* K, struct vector* F, struct matrix* KE,
			struct matrix* ID, int IEN[],
			struct list* essential_bcs, int nenodes, int ndof){
  // IEN maps local node numbers (starting at 0) to global node numbers
  // ID maps global node numbers and dof to equation numbers
  int i, j, k, l, p, q, P, Q;
  double g;
  for (i=0; i<nenodes; i++){
    for (j=0; j<ndof; j++){
      p = ndof*i+j;                     // Local row number
      P = ID->array[IEN[i]][j];         // Global row number
      if (P != -1){
	for (k=0; k<nenodes; k++){
	  for (l=0; l<ndof; l++){
	    q = ndof*k+l;               // Local col number
	    Q = ID->array[IEN[k]][l];   // Global col number
	    if (Q != -1)
	      K->array[P][Q] += KE->array[p][q];
	    else{
	      g = get_essential_bc(essential_bcs, IEN[k], l);
	      F->array[P] -= KE->array[p][q]*g;
	    }
	  }
	}
      }
    }
  }
}


static void construct_K(struct list* nodes, struct list* elements,
			struct list* et_defs, struct matrix* ID,
			struct matrix* K, struct vector* F, int free_dof,
			struct list* essential_bcs){
  struct element* e;
  struct et_def* et;
  struct matrix *KE;
  int i;
  for (i=0; i<elements->nitems; i++){
    printf("Assembling stiffness matrix for element %d\n", i);
    e = elements->array[i];
    et = get_et_def(et_defs, e->et_id);
    KE = construct_KE(e, et, nodes);
    print_matrix(KE);
    assemble_KE(K, F, KE, ID, e->IEN, essential_bcs, et->nenodes, et->ndof);
    free_matrix(KE);
  }
}


static void construct_F(struct list* nodes, struct list* nodal_forces,
			struct matrix* ID, struct vector* F, int ndof){
  int i, j, P;
  for (i=0; i<nodes->nitems; i++){
    for (j=0; j<ndof; j++){
      P = ID->array[i][j];
      if (P != -1)
	F->array[P] += get_nodal_force(nodal_forces, i, j);
    }
  }
}


static struct static_soln* new_static_soln(int ndof, struct matrix* ID,
					   struct vector* U){
  struct static_soln* sol = malloc(sizeof(struct static_soln));
  sol->ndof = ndof;
  sol->ID = ID;
  sol->U = U;
  return sol;
}


void free_static_soln(struct static_soln* sol){
  free_matrix(sol->ID);
  free_vector(sol->U);
  free(sol);
}


struct static_soln* dense_static_solver(struct model* running_model){
  struct matrix* ID = new_matrix(running_model->nodes->nitems,
				running_model->ndof);
  struct matrix* K = new_matrix(running_model->free_dof,
				running_model->free_dof);
  struct vector* F = new_vector(running_model->free_dof);
  precomputations(running_model->et_defs);
  construct_ID(running_model->nodes, running_model->ndof,
	       running_model->essential_bcs, ID);
  printf("ID Matrix\n"), print_matrix(ID);
  construct_K(running_model->nodes, running_model->elements,
	      running_model->et_defs, ID, K, F, running_model->free_dof,
	      running_model->essential_bcs);
  printf("Stiffness matrix:\n"), print_matrix(K);
  construct_F(running_model->nodes, running_model->nodal_forces,
	      ID, F, running_model->ndof);
  printf("Force vector:\n"), print_vector(F);
  gaussLSS(K, F);  // Reduces F to U
  free_matrix(K);
  printf("Solution vector:\n"), print_vector(F);
  return new_static_soln(running_model->ndof, ID, F);
}
