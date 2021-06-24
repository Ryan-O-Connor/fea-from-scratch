/* 
 * Functions for computing element stiffness matrices (KE) and
 * associated matrices (B, D, R)
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib/list.h"
#include "lib/linalg.h"
#include "mesh.h"
#include "element_types.h"
#include "shape.h"


/***********************************************************
 * Functions for computing transformation matrices R
 */

static struct matrix* construct_R(double c, double s){
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
 * Functions for computing constitutive matrices D
 */


static struct matrix* D_structural(struct et_def* et){
  double E = et->mprops->E;
  double v = et->mprops->v;
  struct matrix* D = NULL;
  if (et->opts[1] == 0){
    // Plane stress
    D = new_matrix(3, 3);
    D->array[0][0] = 1.0;
    D->array[0][1] = v;
    D->array[1][0] = v;
    D->array[1][1] = 1.0;
    D->array[2][2] = 0.5*(1.0-v);
    cmmult(D, E/(1.0-v*v));
  }
  else if (et->opts[1] == 1){
    // Plane strain
    D = new_matrix(3, 3);
    D->array[0][0] = 1.0-v;
    D->array[0][1] = v;
    D->array[1][0] = v;
    D->array[1][1] = 1.0-v;
    D->array[2][2] = 1.0-v;
    cmmult(D, E/((1.0+v)*(1.0-2.0*v)));
  }
  else{
    printf("KeyError: Bad key option: %d.  Failed to construct D.\n",
	   et->opts[1]);
    exit(1);
  }
  return D;
}


static struct matrix* D_thermal(struct et_def* et){
  double K = et->mprops->K;
  struct matrix* D = new_matrix(2, 2);
  D->array[0][0] = K;
  D->array[1][1] = K;
  return D;
}



struct matrix* construct_D(struct et_def* et){
  
  if (et->lib_id >= 0 && et->lib_id < 10)
    return D_structural(et);
  
  else if (et->lib_id >= 10 && et->lib_id < 20)
    return D_thermal(et);

  printf("Error: Invalid library id\n");
  return NULL;
}


/***********************************************************
 * Functions for computing strain-displacement matrices B
 */


static struct matrix* Bi_thermal(int i, struct matrix* NDERGLB){
  // Construct plane conduction submatrix Bi from the
  // Global shape functions where Bi is the 2x1 matrix:
  // Bi = grad(Ni) = { {Na,x}, {Na,y} }
  struct matrix* Bi = new_matrix(2, 1);
  Bi->array[0][0] = NDERGLB->array[i][0];
  Bi->array[1][0] = NDERGLB->array[i][0];
  return Bi;
}


static struct matrix* Bi_structural(int i, struct matrix* NDERGLB){
  // Construct plane-stress/plane-strain submatrix Bi from the
  // global shape functions where Bi is the 3x2 matrix:
  // Bi = { {Na,x , 0}, {0 , Na,y}, {Na,y , Na,x} }
  struct matrix* Bi = new_matrix(3, 2);
  Bi->array[0][0] = NDERGLB->array[i][0];
  Bi->array[1][1] = NDERGLB->array[i][1];
  Bi->array[2][0] = NDERGLB->array[i][1];
  Bi->array[2][1] = NDERGLB->array[i][0];
  return Bi;
}



/*************************************************************
 * Functions for computing stiffness matrices KE
 */


static void
assemble_subKE(struct matrix* KE, struct matrix* ke,
	       int a, int b, int ndof){
  // Applicable to isoparametric elements
  int i, j;
  int row_offset = ndof*a, col_offset = ndof*b;
  for (i=0; i<ndof; i++){
    for (j=0; j<ndof; j++)
      KE->array[row_offset+i][col_offset+j] += ke->array[i][j];
  }
}


static struct matrix*
SBar_KE(struct et_def* et, struct matrix* COORDS){
  double x1 = COORDS->array[0][0], x2 = COORDS->array[0][1];
  double y1 = COORDS->array[1][0], y2 = COORDS->array[1][1];
  double E = et->mprops->E;
  double A = et->consts[1];
  double L = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
  double k = E*A/L;
  double c = (x2 - x1) / L;
  double s = (y2 - y1) / L;
  struct matrix* R = construct_R(c, s);
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


static struct matrix*
Isoparametric_KE(struct et_def* et, struct matrix* COORDS, int ndof,
		 struct matrix* (*construct_Bi)(int, struct matrix*)){
  struct list* NDERGLBs = construct_NDERGLBs(COORDS, et->sdata->NDERNATs,
					     et->sdata->nint_pts);
  struct matrix* KE = new_matrix(et->ndof*et->nenodes, et->ndof*et->nenodes);
  struct matrix* D = et->sdata->D;
  struct matrix* NDERGLB;
  printf("Constitutive matrix:\n"), print_matrix(D);
  int i, j, k;
  double w, jacob;
  for (k=0; k<et->sdata->nint_pts; k++){
    NDERGLB = NDERGLBs->array[k];
    w = et->sdata->int_wts[k];
    jacob = jacobian(COORDS, et->sdata->NDERNATs->array[k]);
    for (i=0; i<et->nenodes; i++){
      struct matrix* Bi = construct_Bi(i, NDERGLB);
      struct matrix* BiT = mtranspose(Bi);
      struct matrix* BD = mmmult(BiT, D);
      cmmult(BD, w*jacob);
      for (j=0; j<et->nenodes; j++){
	struct matrix* Bj = construct_Bi(j, NDERGLB);
	struct matrix* kij = mmmult(BD, Bj);
	assemble_subKE(KE, kij, i, j, ndof);
	free_matrix(Bj), free_matrix(kij);
      }
      free_matrix(Bi), free_matrix(BiT), free_matrix(BD);
    }
  }
  for (i=0; i<et->sdata->nint_pts; i++)
    free_matrix(NDERGLBs->array[i]);
  free_list(NDERGLBs);
  return KE;
}



struct matrix* construct_KE(struct element* e, struct et_def* et,
			    struct list* nodes){
  struct matrix* KE;
  struct matrix* COORDS = construct_COORDS(nodes, e->IEN, et->nenodes);
  
  if (et->lib_id == 1)
    KE = SBar_KE(et, COORDS);
  
  else if (et->lib_id == 4){
    double t = et->consts[1]; // Thickness
    KE = Isoparametric_KE(et, COORDS, 2, Bi_structural);
    cmmult(KE, t);
  }
  
  else if (et->lib_id == 14){
    double t = et->consts[1]; // Thickness
    KE = Isoparametric_KE(et, COORDS, 1, Bi_thermal);
    cmmult(KE, t);
  }
  
  return KE;
}
