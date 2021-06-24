/*
Functions for calculating shape functions and their derivatives
for all elements in the library at specified points.  All results
are returned as vectors of the form:
     N = {N0(pt), N1(pt), ..., Nn-1(pt)}
for all n shape functions of that element.
 */

#include <stdio.h>
#include "lib/geom.h"
#include "lib/list.h"
#include "lib/linalg.h"
#include "mesh.h"
#include "element_types.h"
#include "shape.h"


#define NSD 2


double jacobian(struct matrix* COORDS, struct matrix* NDERNAT){
  struct matrix* J = mmmult(COORDS, NDERNAT);
  double j = determinant2x2(J);
  free_matrix(J);
  return j;
}


struct matrix*
construct_COORDS(struct list* nodes, int IEN[], int nenodes){
  struct matrix* COORDS = new_matrix(NSD, nenodes);
  struct node* n;
  int i;
  for (i=0; i<nenodes; i++){
    n = nodes->array[IEN[i]];
    COORDS->array[0][i] = n->x;
    COORDS->array[1][i] = n->y;
  }
  return COORDS;
}


static struct matrix*
Plane4_NDERNAT(struct point* pt){
  // 4-node linear quadrilateral elements
  double x = pt->x, y = pt->y;
  struct matrix* NDERNAT = new_matrix(4, 2);
  NDERNAT->array[0][0] = -0.25*(1-y);
  NDERNAT->array[1][0] = 0.25*(1-y);
  NDERNAT->array[2][0] = 0.25*(1+y);
  NDERNAT->array[3][0] = -0.25*(1+y);
  NDERNAT->array[0][1] = -0.25*(1-x);
  NDERNAT->array[1][1] = -0.25*(1+x);
  NDERNAT->array[2][1] = 0.25*(1+x);
  NDERNAT->array[3][1] = 0.25*(1-x);
  return NDERNAT;
}


static struct matrix*
Plane8_NDERNAT(struct point* pt){
  // 8-node serendipity quadrilateral elements
  struct matrix* NDERNAT = new_matrix(8, 2);
  double x = pt->x, y = pt->y;
  return NDERNAT;
}


static struct matrix*
construct_NDERNAT(int lib_id, struct point* pt){
  // Get shape function derivative matrix in natural coordinates
  // Need only be computed once for each element family
  if (lib_id == 4 || lib_id == 14)
    return Plane4_NDERNAT(pt);
  
  else if (lib_id == 8 || lib_id == 18)
    return Plane8_NDERNAT(pt);
  
  else{
    printf("Invalid library element id\n");
    return NULL;
  }
}


struct list*
construct_NDERNATs(struct et_def* et){
  struct point* pt;
  struct list* NDERNATs = new_list();
  int i;
  for (i=0; i<et->sdata->nint_pts; i++){
    pt = et->sdata->int_pts->array[i];
    append(NDERNATs, construct_NDERNAT(et->lib_id, pt));
  }
  return NDERNATs;
}


static struct matrix*
construct_NDERGLB(struct matrix* COORDS, struct matrix* NDERNAT){
  struct matrix* J = mmmult(COORDS, NDERNAT);
  struct matrix* Jinv = minverse2x2(J);
  struct matrix* NDERGLB = mmmult(NDERNAT, Jinv);
  free_matrix(J), free_matrix(Jinv);
  return NDERGLB;
}


struct list*
construct_NDERGLBs(struct matrix* COORDS,
		   struct list* NDERNATs, int nint_pts){
  struct matrix* NDERNAT;
  struct list* NDERGLBs = new_list();
  int i;
  for (i=0; i<nint_pts; i++){
    NDERNAT = NDERNATs->array[i];
    append(NDERGLBs, construct_NDERGLB(COORDS, NDERNAT));
  }
  return NDERGLBs;
}



