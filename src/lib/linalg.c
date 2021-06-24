/*
 * Basic linear algebra functions needed for 
 * Constructing element matrices.  Global matrices
 * use the sparse linear algebra library.
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "linalg.h"


#define EPSILON 1e-6


/***************************************************
 * General math functions
 */


static int close_floats(double x, double y){
  return fabs((x/y)-1) < EPSILON ? 1 : 0;
}


/*****************************************************
 * Constructors and destructors
 */


struct matrix* new_matrix(int nrows, int ncols){
  struct matrix* A = malloc(sizeof(struct matrix));
  double** array = malloc(nrows*sizeof(double*));
  int i;
  for (i=0; i<nrows; i++)
    array[i] = calloc(ncols, sizeof(double));
  A->array = array;
  A->nrows = nrows;
  A->ncols = ncols;
  return A;
}


struct matrix* new_triangular_matrix(int n, int is_upper){
  struct matrix* A = malloc(sizeof(struct matrix));
  double** array = malloc(n*sizeof(double*));
  int i;
  if (is_upper){
    for (i=0; i<n; i++)
      array[i] = calloc(n-i, sizeof(double));
  }
  else{
    for (i=0; i<n; i++)
      array[i] = calloc(i, sizeof(double));
  }
  A->array = array;
  A->nrows = n;
  A->ncols = n;
  return A;
}


struct matrix* new_identity_matrix(int n){
  struct matrix* eye = new_matrix(n, n);
  int i;
  for (i=0; i<n; i++)
    eye->array[i][i] = 1;
  return eye;
}


struct vector* new_vector(int n){
  struct vector* v = malloc(sizeof(struct vector));
  double* array = calloc(n, sizeof(double));
  v->array = array;
  v->n = n;
  return v;
}


struct matrix* copy_matrix(struct matrix* A){
  struct matrix* B = new_matrix(A->nrows, A->ncols);
  int i, j;
  for (i=0; i<A->nrows; i++){
    for (j=0; j<A->ncols; j++)
      B->array[i][j] = A->array[i][j];
  }
  return B;
}


struct vector* copy_vector(struct vector* u){
  struct vector* v = new_vector(u->n);
  int i;
  for (i=0; i<u->n; i++)
    v->array[i] = u->array[i];
  return v;
}


void free_matrix(struct matrix* A){
  int i;
  for (i=0; i<A->nrows; i++)
    free(A->array[i]);
  free(A->array);
  free(A);
}


void free_vector(struct vector* u){
  free(u->array);
  free(u);
}


/****************************************************
 * Output and checking functions
 */


int mequal(struct matrix* A, struct matrix* B){
  if (A->nrows != B->nrows || A->ncols != B->ncols)
    return 0;
  int i, j;
  for (i=0; i<A->nrows; i++){
    for (j=0; j<A->ncols; j++){
      if (!close_floats(A->array[i][j], B->array[i][j]))
	return 0;
    }
  }
  return 1;
}


int vequal(struct vector* u, struct vector* v){
  if (u->n != v->n)
    return 0;
  int i;
  for (i=0; i<u->n; i++){
    if (!close_floats(u->array[i], v->array[i]))
      return 0;
  }
  return 1;
}


void print_matrix(struct matrix* A){
  int i, j;
  printf("Matrix\n");
  for (i=0; i<A->nrows; i++){
    for (j=0; j<A->ncols; j++)
      printf(" %8.3g ", A->array[i][j]);
    printf("\n");
  }
}


void print_vector(struct vector* u){
  int i;
  printf("Vector:\n");
  for (i=0; i<u->n; i++)
    printf(" %8.3g ", u->array[i]);
  printf("\n");
}


/****************************************************
 * Basic row and matrix manipulations
 */


struct matrix* mtranspose(struct matrix* A){
  struct matrix* AT = new_matrix(A->ncols, A->nrows);
  int i, j;
  for (i=0; i<A->nrows; i++){
    for (j=0; j<A->ncols; j++)
      AT->array[j][i] = A->array[i][j];
  }
  return AT;
}


static void row_swap(struct matrix* A, int i, int j){
  // Swap row i and row j
  double* tmp = A->array[i];
  A->array[i] = A->array[j];
  A->array[j] = tmp;
}

static void row_multiply(struct matrix* A, int i, double c){
  // Multiply row i by c
  int j;
  for (j=0; j<A->ncols; j++)
    A->array[i][j] *= c;
}

static void row_addition(struct matrix* A, int i, int j, int start, double c){
  // Multiply row i by c and add to row j, starting at index start
  int k;
  for (k=start; k<A->ncols; k++)
    A->array[j][k] += c*A->array[i][k];
}


double determinant2x2(struct matrix* A){
  return A->array[0][0]*A->array[1][1]-A->array[1][0]*A->array[0][1];
}


struct matrix* minverse2x2(struct matrix* A){
  struct matrix* Ainv = new_matrix(2, 2);
  double det = determinant2x2(A);
  Ainv->array[0][0] = A->array[1][1];
  Ainv->array[0][1] = -A->array[0][1];
  Ainv->array[1][0] = -A->array[1][0];
  Ainv->array[1][1] = A->array[0][0];
  cmmult(Ainv, 1/det);
  return Ainv;
}


/****************************************************
 * Basic addition and multiplication
 */


void cmmult(struct matrix* A, double c){
  int i, j;
  for (i=0; i<A->nrows; i++){
    for(j=0; j<A->ncols; j++)
      A->array[i][j] *= c;
  }
}


struct matrix* mmadd(struct matrix* A, struct matrix* B){
  assert(A->nrows == B->nrows);
  assert(A->ncols == B->ncols);
  struct matrix* C = new_matrix(A->nrows, A->ncols);
  int i, j;
  for (i=0; i<A->nrows; i++){
    for (j=0; i<A->ncols; j++)
      C->array[i][j] = A->array[i][j] + B->array[i][j];
  }
  return C;
}


struct vector* vvadd(struct vector* u, struct vector* v){
  assert(u->n == v->n);
  struct vector* w = new_vector(u->n);
  int i;
  for (i=0; i<u->n; i++)
    w->array[i] = u->array[i] + v->array[i];
  return w;
}


struct matrix* mmmult(struct matrix* A, struct matrix* B){
  assert(A->ncols == B->nrows);
  struct matrix* C = new_matrix(A->nrows, B->ncols);
  int i, j, k;
  double sum;
  for (i=0; i<A->nrows; i++){
    for (j=0; j<B->ncols; j++){
      sum = 0;
      for (k=0; k<A->ncols; k++)
	sum += A->array[i][k]*B->array[k][j];
      C->array[i][j] = sum;
    }
  }
  return C;
}


struct vector* mvmult(struct matrix* A, struct vector* x){
  assert(A->ncols == x->n);
  struct vector* b = new_vector(A->nrows);
  int i, j;
  double sum;
  for (i=0; i<A->nrows; i++){
    sum = 0;
    for (j=0; j<x->n; j++)
      sum += A->array[i][j]*x->array[j];
    b->array[i] = sum;
  }
  return b;
}


/***********************************************
 * Matrix factoring algorithms
 */


void cholMFA(struct matrix* A){
  // Overwrites A with it's lower trangular cholesky factor
  // Cholesky-Banachiewicz algorithm
  assert(A->nrows == A->ncols);
  int n = A->nrows;
  int i, j, k;
  double sum;
  for (i=0; i<n; i++){
    for (j=0; j<=i; j++){      
      sum = 0.0;
      for (k=0; k<j; k++)
	sum += A->array[i][k] * A->array[j][k];
      if (i == j)
	A->array[i][j] = sqrt(A->array[i][i] - sum);
      else
	A->array[i][j] = (1.0 / A->array[j][j]) * (A->array[i][j] - sum);
    }
  }
}


/****************************************************
 * Basic linear system solver
 */


void forward_elimination(struct matrix* A, struct vector* b){
  // Eliminates all elements below each pivot
  // Normalizes all pivots to 1
  int n = b->n;
  int i, j, k;
  double pivot, sub_pivot;
  for (i=0; i<n; i++){
    pivot = A->array[i][i];
    assert(pivot != 0.0);
    for (j=i+1; j<n; j++){
      sub_pivot = A->array[j][i];
      for (k=i; k<n; k++)
	A->array[j][k] += (-sub_pivot/pivot)*A->array[i][k]; 
      b->array[j] += (-sub_pivot/pivot)*b->array[i];
    }
  }
}


void back_substitution(struct matrix* A, struct vector* b){
  int n = b->n;
  int i, j;
  for (i=n-1; i>=0; i--){
    for (j=i+1; j<n; j++)
      b->array[i] -= A->array[i][j]*b->array[j];
    b->array[i] /= A->array[i][i];
  }
}


void gaussLSS(struct matrix* A, struct vector* b){
  // In-place reduction of A to U and b to the solution x
  assert(A->nrows == A->ncols);
  assert(A->ncols == b->n);
  forward_elimination(A, b);
  back_substitution(A, b);
}




