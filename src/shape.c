/*
Functions for calculating shape functions and their derivatives
for all elements in the library at specified points.  All results
are returned as vectors of the form:
     N = {N0(pt), N1(pt), ..., Nn-1(pt)}
for all n shape functions of that element.
 */

#include <stdio.h>
#include "lib/geom.h"
#include "lib/linalg.h"
#include "shape.h"


static struct matrix* Plane4_NDERNAT(struct point* pt){
  // Applies to all 4-node bilinear elements 
  struct matrix* NDERNAT = new_matrix(4, 2);
  NDERNAT->array[0][0] = -0.25*(1-(pt->y));
  NDERNAT->array[1][0] = 0.25*(1-(pt->y));
  NDERNAT->array[2][0] = 0.25*(1+(pt->y));
  NDERNAT->array[3][0] = -0.25*(1+(pt->y));
  NDERNAT->array[0][1] = -0.25*(1-(pt->x));
  NDERNAT->array[1][1] = -0.25*(1+(pt->x));
  NDERNAT->array[2][1] = 0.25*(1+(pt->x));
  NDERNAT->array[3][1] = 0.25*(1-(pt->x));
  return NDERNAT;
}


struct matrix* get_NDERNAT(int lib_id, struct point* pt){
  // Get shape function derivative matrix in natural coordinates
  // Need only be computed once for each element family
  if (lib_id == 4 || lib_id == 14)
    return Plane4_NDERNAT(pt);
  else{
    printf("Invalid library element id\n");
    return NULL;
  }
}


struct matrix* get_NDERGLB(struct matrix* COORDS, struct matrix* NDERNAT){
  struct matrix* J = mmmult(COORDS, NDERNAT);
  struct matrix* Jinv = minverse2x2(J);
  struct matrix* NDERGLB = mmmult(NDERNAT, Jinv);
  free_matrix(J);
  free_matrix(Jinv);
  return NDERGLB;
}



