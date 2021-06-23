#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lib/list.h"
#include "mesh.h"
#include "element_types.h"
#include "bc_data.h"
#include "model.h"
#include "solver.h"
#include "post.h"


struct model* new_model(){
  printf("**********************************************\n");
  printf("*****Creating new model***********************\n");
  printf("**********************************************\n");
  struct model* new_model = malloc(sizeof(struct model));
  new_model->free_dof = 0;
  new_model->total_dof = 0;
  new_model->nodes = new_list();
  new_model->elements = new_list();
  new_model->et_defs = new_list();
  new_model->essential_bcs = new_list();
  new_model->nodal_forces = new_list();
  new_model->solution = NULL;
  return new_model;
}


// Mesh functions

void new_model_node(struct model* running_model, double x, double y){
  printf("Creating new node at (%g, %g)\n", x, y);
  struct node* n = new_node(x, y);
  append(running_model->nodes, n);
}



void new_model_element(struct model* running_model, int et_id, int* IEN){
  printf("Creating new element of type %d\n", et_id);
  struct et_def* et = get_et_def(running_model->et_defs, et_id);
  assert(et != NULL);
  struct element* e = new_element(et_id, IEN);
  append(running_model->elements, e);
  print_element(e, et->nenodes);
}


// Element type definition functions

void new_model_element_type(struct model* running_model,
		      int et_id, char* type_name){
  printf("Creating new element type %s with id %d\n", type_name, et_id);
  struct et_def* et = new_et_def(et_id, type_name);
  print_et_def(et);
  append(running_model->et_defs, et);
}


void set_model_et_real_constant(struct model* running_model,
			     int et_id, int const_id, double value){
  struct et_def* et = get_et_def(running_model->et_defs, et_id);
  assert(et != NULL);
  set_real_constant(et, const_id, value);
  print_et_def(et);
}


void set_model_et_keyopt(struct model* running_model,
		      int et_id, int key, int option){
  struct et_def* et = get_et_def(running_model->et_defs, et_id);
  assert(et != NULL);
  set_keyopt(et, key, option);
  print_et_def(et);
}


void set_model_et_matprop(struct model* running_model,
		       int et_id, char* prop_name, double value){
  struct et_def* et = get_et_def(running_model->et_defs, et_id);
  assert(et != NULL);
  set_matprop(et, prop_name, value);
  print_et_def(et);
}


// Boundary condition functions

void add_model_essential_bc(struct model* running_model,
			    int node_id, char* comp, double value){
  if (strcmp(comp, "ALL") == 0){
    struct essential_bc* ebcX = new_essential_bc(node_id, 0, value);
    struct essential_bc* ebcY = new_essential_bc(node_id, 1, value);
    append(running_model->essential_bcs, ebcX);
    append(running_model->essential_bcs, ebcY);
    print_essential_bc(ebcX), print_essential_bc(ebcY);
  }
  else if (strcmp(comp, "Y") == 0) {
    struct essential_bc* ebcY = new_essential_bc(node_id, 1, value);
    append(running_model->essential_bcs, ebcY);
    print_essential_bc(ebcY);
  }
  else {
    struct essential_bc* ebcX = new_essential_bc(node_id, 0, value);
    append(running_model->essential_bcs, ebcX);
    print_essential_bc(ebcX);
  }
}


void add_model_nodal_force(struct model* running_model,
			   int node_id, char* comp, double value){
  struct nodal_force* ndf;
  if (strcmp(comp, "X") == 0)
    ndf = new_nodal_force(node_id, 0, value);
  else if (strcmp(comp, "Y") == 0)
    ndf = new_nodal_force(node_id, 1, value);
  else
    ndf = NULL;
  append(running_model->nodal_forces, ndf);
  print_nodal_force(ndf);
}


// Other functions


static void setup_model_for_solve(struct model* running_model){
  struct list* nodes = running_model->nodes;
  struct list* essential_bcs = running_model->essential_bcs;
  struct et_def* et = running_model->et_defs->array[0];
  running_model->nsd = 2;
  running_model->ndof = et->ndof;
  running_model->total_dof = et->ndof*nodes->nitems;
  running_model->free_dof = running_model->total_dof - essential_bcs->nitems;
}


/*
 * p_type = physics type
 * s_type = solver type
 * When p_type = 0 (Static analysis)
 *   s_type = 0 (Dense, direct solver)
 *   s_type = 1 (Sparse, direct solver)
 * When p_type = 1 (Modal analysis)
 *   s_type = 0 (Dense, QR solver)
 */
void solve_model(struct model* running_model, int p_type, int s_type){
  printf("**********************************************\n");
  printf("*****Solving model****************************\n");
  printf("**********************************************\n");
  setup_model_for_solve(running_model);
  if (p_type == 0)
    if (s_type == 0)
      running_model->solution = dense_static_solver(running_model);
  printf("**********************************************\n");
  printf("*****Finished solving*************************\n");
  printf("**********************************************\n");
}


void print_model_mesh(struct model* running_model){
  int i, nenodes[100];
  struct et_def* et;
  for (i=0; i<running_model->et_defs->nitems; i++){
    et = get(running_model->et_defs, i);
    nenodes[et->user_id] = et->nenodes;
  }
  print_mesh(running_model->nodes, running_model->elements, nenodes);
}


void print_model_result(struct model* running_model, char* res_name){
  if (running_model->solution != NULL)
    print_nodal_soln(running_model->nodes, running_model->solution);
  else
    printf("Error: No solution to print\n");
}


void free_model(struct model* running_model){
  free_mesh(running_model->nodes, running_model->elements);
  free_items(running_model->et_defs, free_et_def);
  free_list(running_model->et_defs);
  free_items(running_model->essential_bcs, free_essential_bc);
  free_list(running_model->essential_bcs);
  free_items(running_model->nodal_forces, free_nodal_force);
  free_list(running_model->nodal_forces);
  free_static_soln(running_model->solution);
  free(running_model);
}
