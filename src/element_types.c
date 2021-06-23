#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "lib/list.h"
#include "lib/geom.h"
#include "element_types.h"

#define MAXOPT 10


/* 
 * Element library type ids:
 *  0 = SSPRING (2-node structural spring)
 *  1 = SBAR (2-node structural bar)
 *  2 = SBEAM (2-node structural beam)
 *  3 = SPLANE3 (3-node structural triangular plane element)
 *  4 = SPLANE4 (4-node structural quadrilateral plane element)
 *  5 = Undefined
 *  6 = SPLANE6 (6-node structural triangular plane element)
 *  7 = Undefined 
 *  8 = SPLANE8 (8-node structural quadrilateral plane element)
 *  9 = Undefined
 * 10 = TRESISTANCE
 * 11 = Undefined
 * 12 = Undefined
 * 13 = TPLANE3 (3-node thermal triangular plane element)
 * 14 = TPLANE4 (4-node thermal quadrilateral plane element)
 * 15 = Undefined
 * 16 = TPLANE6 (6-node thermal triangular plane element)
 * 17 = Undefined 
 * 18 = TPLANE8 (8-node thermal quadrilateral plane element)
 * 19 = Undefined
 */


static int get_lib_id(char type_name[]){
  if (strcmp(type_name, "SSPRING") == 0)
    return 0;
  else if (strcmp(type_name, "SBAR") == 0)
    return 1;
  else if (strcmp(type_name, "SBEAM") == 0)
    return 2;
  else if (strcmp(type_name, "SPLANE3") == 0)
    return 3;
  else if (strcmp(type_name, "SPLANE4") == 0)
    return 4;
  else if (strcmp(type_name, "SPLANE5") == 0)
    return 5;
  else if (strcmp(type_name, "SPLANE6") == 0)
    return 6;
  else if (strcmp(type_name, "SPLANE8") == 0)
    return 8;
  else if (strcmp(type_name, "TRESISTANCE") == 0)
    return 10;
  else if (strcmp(type_name, "TPLANE3") == 0)
    return 13;
  else if (strcmp(type_name, "TPLANE4") == 0)
    return 14;
  else if (strcmp(type_name, "TPLANE5") == 0)
    return 15;
  else if (strcmp(type_name, "TPLANE6") == 0)
    return 16;
  else if (strcmp(type_name, "TPLANE8") == 0)
    return 18;
  else{
    printf("Error: Invalid element type name: %s\n", type_name);
    return -1;
  }
}


static int get_nenodes(int lib_id){
  if (lib_id == 0 || lib_id == 1 || lib_id == 2 || lib_id == 10)
    return 2;
  else if (lib_id == 3 || lib_id == 13)
    return 3;
  else if (lib_id == 4 || lib_id == 14)
    return 4;
  else if (lib_id == 5 || lib_id == 15)
    return 5;
  else if (lib_id == 6 || lib_id == 16)
    return 6;
  else if (lib_id == 8 || lib_id == 18)
    return 8;
  printf("Error: Invalid library element id\n");
  return -1;
}


static int get_ndof(int lib_id){
  if (lib_id >= 0 && lib_id < 10)
    return 2;
  else if (lib_id >= 10 && lib_id < 20)
    return 1;
  printf("Error: Invalid library element id\n");
  return -1;
}


static struct matprops* new_matprops(){
  struct matprops* m = malloc(sizeof(struct matprops));
  m->E = 0;
  m->v = 0;
  m->K = 0;
  return m;
}


static struct solver_data* new_sdata(){
  struct solver_data* sdata = malloc(sizeof(struct solver_data));
  sdata->int_wts = NULL;
  sdata->int_pts = NULL;
  sdata->NDERNATs = NULL;
  sdata->D = NULL;
  return sdata;
}


struct et_def* new_et_def(int user_id, char* type_name){
  struct et_def* et = malloc(sizeof(struct et_def));
  et->user_id = user_id;
  et->lib_id = get_lib_id(type_name);
  et->nenodes = get_nenodes(et->lib_id); 
  et->ndof = get_ndof(et->lib_id);
  et->mprops = new_matprops();
  et->sdata = new_sdata();
  // Zero out options and constants
  int i;
  for (i=0; i<MAXOPT; i++){
    et->opts[i] = 0;
    et->consts[i] = 0.0;
  }
  return et;
}


struct et_def* get_et_def(struct list* et_defs, int user_id){
  int i;
  struct et_def* et;
  for (i=0; i<et_defs->nitems; i++){
    et = et_defs->array[i];
    if (user_id == et->user_id)
      return et;
  }
  return NULL;
}


void set_real_constant(struct et_def* et, int const_id, double value){
  assert(const_id>=0 && const_id < 10 && value > 0);
  et->consts[const_id] = value;
}


void set_keyopt(struct et_def* et, int key, int option){
  assert(key>=0 && key < 10);
  et->opts[key] = option;
}


void set_matprop(struct et_def* et, char* prop_name, double value){
  if (strcmp(prop_name, "E") == 0)
    et->mprops->E = value;
  else if (strcmp(prop_name, "V") == 0)
    et->mprops->v = value;
  else if (strcmp(prop_name, "K") == 0)
    et->mprops->K = value;
  else
    printf("Error: Invalid material property name\n");
}


void print_et_def(struct et_def* et){
  int i;
  printf("Element type id: %d\n", et->user_id);
  printf("\tLibrary type id: %d\n", et->lib_id);
  printf("\tElement nodes: %d\n", et->nenodes);
  printf("\tDegrees of freedom: %d\n", et->ndof);
  printf("\tElement key options:");
  for (i=0; i<MAXOPT; i++)
    printf(" %d", et->opts[i]);
  printf("\n");
  printf("\tElement constants:");
  for (i=0; i<MAXOPT; i++)
    printf(" %g", et->consts[i]);
  printf("\n");
  printf("\tMaterial properties:\n");
  printf("\t\tE = %g\n", et->mprops->E);
  printf("\t\tv = %g\n", et->mprops->v);
  printf("\t\tK = %g\n", et->mprops->K);
}


void free_et_def(void* et){
  struct et_def* ET = et;
  free(ET->mprops);
  free(ET->sdata);
  free(ET);
}


/***************************************************************
 * Integration interfaces
 */


int get_nint_pts(int lib_id, int integration){
  // 0 = reduced integration
  // 1 = full integration
  assert(integrated_element(lib_id));
  if (lib_id == 4 || lib_id == 14){
    if (integration == 0)
      return 1;
    else
      return 4;
  }
  else if (lib_id == 8 || lib_id == 18){
    if (integration == 0)
      return 4;
    else
      return 9;
  }
  printf("Error: Invalid library element id\n");
  return -1;
}


struct list* get_int_pts(int lib_id, int integration){
  struct list* int_pts = new_list();
  if (lib_id == 4 || lib_id == 14){
    if (integration == 0)
      append(int_pts, new_point(0.0, 0.0));
    else{
      append(int_pts, new_point(-0.57735, -0.57735));
      append(int_pts, new_point(0.57735, -0.57735));
      append(int_pts, new_point(0.57735, 0.57735));
      append(int_pts, new_point(-0.57735, 0.57735));
    } 
  }
  else if (lib_id == 8 || lib_id == 18){
    if (integration == 0){
      append(int_pts, new_point(-0.57735, -0.57735));
      append(int_pts, new_point(0.57735, -0.57735));
      append(int_pts, new_point(0.57735, 0.57735));
      append(int_pts, new_point(-0.57735, 0.57735));
    }      
    else{
      append(int_pts, new_point(-.77460, -0.77460));
      append(int_pts, new_point(0.0, -0.77460));
      append(int_pts, new_point(.77460, -0.77460));
      append(int_pts, new_point(-.77460, 0.0));
      append(int_pts, new_point(0.0, 0.0));
      append(int_pts, new_point(.77460, 0.0));
      append(int_pts, new_point(-.77460, 0.77460));
      append(int_pts, new_point(0.0, 0.77460));
      append(int_pts, new_point(.77460, 0.77460));
    }
  }
  return int_pts;
}


double* get_int_wts(int lib_id, int integration){
  double* int_wts;
  if (lib_id == 4 || lib_id == 14){
    if (integration == 0){
      int_wts = malloc(sizeof(double));
      int_wts[0] = 4.0;
    }
    else{
      int_wts = malloc(4*sizeof(double));
      int_wts[0] = 1.0;
      int_wts[1] = 1.0;
      int_wts[2] = 1.0;
      int_wts[3] = 1.0;
    } 
  }
  else if (lib_id == 8 || lib_id == 18){
    if (integration == 0){
      int_wts = malloc(4*sizeof(double));
      int_wts[0] = 1.0;
      int_wts[1] = 1.0;
      int_wts[2] = 1.0;
      int_wts[3] = 1.0;
    }      
    else{
      int_wts = malloc(9*sizeof(double));
      int_wts[0] = 0.30864;
      int_wts[1] = 0.49383;
      int_wts[2] = 0.30864;
      int_wts[3] = 0.49383;
      int_wts[4] = 0.79012;
      int_wts[5] = 0.49383;
      int_wts[6] = 0.30864;
      int_wts[7] = 0.49383;
      int_wts[8] = 0.30864;
    }
  }
  return int_wts;
}


int integrated_element(int lib_id){
  if (lib_id == 0 || lib_id == 1 || lib_id == 2 || lib_id == 10)
    return 0;
  else
    return 1;
  printf("Error: Invalid library element id\n");
  return -1;
}




