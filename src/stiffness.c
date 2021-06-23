/* 
 * Functions for computing element stiffness matrices (KE) and
 * associated matrices (B, D, R)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "element_types.h"
#include "lib/linalg.h"


/***********************************************************
 * Functions for computing transformation matrices R
 */

static struct matrix* R_bar(double c, double s){
  struct matrix* R = new_matrix(4, 4);
  R->array[0][0] = c;
  R->array[0][1] = s;
  R->array[1][0] = -s;
  R->array[1][1] = c;
  R->array[2][2] = c;
  R->array[2][3] = s;
  R->array[3][2] = -s;
  R->array[3][3] = c;
  return R;
}


/*************************************************************
 * Functions for computing stiffness matrices KE
 */


// 2D structural truss element
static struct matrix* SBar_KE(struct et_def* et, struct matrix* COORDS){
  double x1 = COORDS->array[0][0], x2 = COORDS->array[0][1];
  double y1 = COORDS->array[1][0], y2 = COORDS->array[1][1];
  double E = et->mprops->E;
  double A = et->consts[1];
  double L = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  double k = E*A/L;
  double c = (x2 - x1) / L;
  double s = (y2 - y1) / L;
  struct matrix* R = R_bar(c, s);
  struct matrix* RT = mtranspose(R);
  struct matrix* kp = new_matrix(4, 4);
  kp->array[0][0] = k;
  kp->array[0][2] = -k;
  kp->array[2][0] = -k;
  kp->array[2][2] = k;
  struct matrix* Q = mmmult(kp, R);
  struct matrix* KE = mmmult(RT, Q);
  free_matrix(R), free_matrix(RT), free_matrix(kp), free_matrix(Q);
  return KE;
}



struct matrix* KE_matrix(struct et_def* et, struct matrix* COORDS){
  if (et->lib_id == 1)
    return SBar_KE(et, COORDS);
  else
    return NULL;
}
