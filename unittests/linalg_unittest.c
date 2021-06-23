#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/lib/linalg.h"


double random_float(){
  return ((double)rand())/((double)RAND_MAX);
}


void test_identity(){
  printf("***Testing identity\n");
  struct matrix* I = new_identity_matrix(3);
  print_matrix(I);
  free_matrix(I);
}


void test_transpose(){
  printf("***Testing transpose\n");
  struct matrix* B = new_matrix(2, 3);
  B->array[0][0] = 1;
  B->array[0][1] = 2;
  B->array[0][2] = 3;
  B->array[1][0] = 4;
  B->array[1][1] = 5;
  B->array[1][2] = 6;
  print_matrix(B);
  struct matrix* BT = mtranspose(B);
  print_matrix(BT);
  free_matrix(B);
  free_matrix(BT);
}


void test_constant_mult(){
  printf("***Testing constant multiply\n");
  struct matrix* B = new_matrix(2, 3);
  B->array[0][0] = 1;
  B->array[0][1] = 2;
  B->array[0][2] = 3;
  B->array[1][0] = 4;
  B->array[1][1] = 5;
  B->array[1][2] = 6;
  print_matrix(B);
  double pi = 3.14;
  cmmult(B, pi);
  print_matrix(B);
  free_matrix(B);
}


void test_matrix_multiply(){
  printf("***Testing matrix multiplication\n");
  struct matrix *A, *B;
  A = new_matrix(2, 3);
  A->array[0][0] = 1;
  A->array[0][1] = 2;
  A->array[0][2] = 3;
  A->array[1][0] = 4;
  A->array[1][1] = 5;
  A->array[1][2] = 6;
  print_matrix(A);
  B = new_matrix(3, 2);
  B->array[0][0] = 10;
  B->array[0][1] = 11;
  B->array[1][0] = 20;
  B->array[1][1] = 21;
  B->array[2][0] = 30;
  B->array[2][1] = 31;
  print_matrix(B);
  struct matrix* C = mmmult(A, B);
  print_matrix(C);
  free_matrix(A), free_matrix(B), free_matrix(C);
}


void test_vector_add(){
  printf("***Testing vector addition\n");
  struct vector *u, *v;
  u = new_vector(3);
  v = new_vector(3);
  u->array[0] = 1;
  u->array[1] = 2;
  u->array[2] = 3;
  v->array[0] = 6;
  v->array[1] = 5;
  v->array[2] = 4;
  print_vector(u), print_vector(v);
  struct vector* w = vvadd(u, v);
  print_vector(w);
  free_vector(u), free_vector(v), free_vector(w);
}


void test_forward_elimination(){
  printf("***Testing forward elimination\n");
  struct matrix* A = new_matrix(3, 3);
  struct vector* b = new_vector(3);
  A->array[0][0] = 2;
  A->array[0][1] = 1;
  A->array[0][2] = -1;
  A->array[1][0] = -3;
  A->array[1][1] = -1;
  A->array[1][2] = 2;
  A->array[2][0] = -2;
  A->array[2][1] = 1;
  A->array[2][2] = 2;
  b->array[0] = 8;
  b->array[1] = -11;
  b->array[2] = -3;
  print_matrix(A), print_vector(b);
  forward_elimination(A, b);
  print_matrix(A), print_vector(b);
  free_matrix(A), free_vector(b);
}


void test_back_substitution(){
  printf("***Testing back substitution\n");
  struct matrix* A = new_matrix(3, 3);
  struct vector* b = new_vector(3);
  A->array[0][0] = 2.0;
  A->array[0][1] = 1.0;
  A->array[0][2] = -1.0;
  A->array[1][0] = 0.0;
  A->array[1][1] = 0.5;
  A->array[1][2] = 0.5;
  A->array[2][0] = 0.0;
  A->array[2][1] = 0.0;
  A->array[2][2] = -1.0;
  b->array[0] = 8.0;
  b->array[1] = 1.0;
  b->array[2] = 1.0;
  print_matrix(A), print_vector(b);
  back_substitution(A, b);
  print_matrix(A), print_vector(b);
  free_matrix(A), free_vector(b);  
}


void test_gaussian_elimination(){
  printf("***Testing gaussian elimination\n");
  struct matrix* A = new_matrix(3, 3);
  struct vector* b = new_vector(3);
  A->array[0][0] = 2;
  A->array[0][1] = 1;
  A->array[0][2] = -1;
  A->array[1][0] = -3;
  A->array[1][1] = -1;
  A->array[1][2] = 2;
  A->array[2][0] = -2;
  A->array[2][1] = 1;
  A->array[2][2] = 2;
  b->array[0] = 8;
  b->array[1] = -11;
  b->array[2] = -3;
  print_matrix(A), print_vector(b);
  gaussLSS(A, b);
  print_matrix(A), print_vector(b);
  free_matrix(A), free_vector(b);
}


void test_random_gaussian_elimination(){
  printf("***Testing gaussian elimination on random matrix\n");
  struct matrix* A = new_matrix(10, 10);
  struct vector* b = new_vector(10);
  int i, j;
  for (i=0; i<A->nrows; i++){
    b->array[i] = random_float();
    for (j=0; j<A->ncols; j++)
      A->array[i][j] = random_float();
  }
  print_vector(b);
  struct matrix* Ac = copy_matrix(A);
  struct vector* x = copy_vector(b);
  gaussLSS(Ac, x);
  struct vector* bc = mvmult(A, x);
  print_vector(bc);
  printf("%s\n", vequal(b, bc) ? "true" : "false");
  free_matrix(A), free_matrix(Ac);
  free_vector(b), free_vector(bc), free_vector(x);
}


void test_matrix_vector_multiply(){
  printf("***Testing matrix vector multiplication\n");
  struct matrix* A = new_matrix(3, 3);
  struct vector* x = new_vector(3);
  A->array[0][0] = 2;
  A->array[0][1] = 1;
  A->array[0][2] = -1;
  A->array[1][0] = -3;
  A->array[1][1] = -1;
  A->array[1][2] = 2;
  A->array[2][0] = -2;
  A->array[2][1] = 1;
  A->array[2][2] = 2;
  x->array[0] = 2;
  x->array[1] = 3.0;
  x->array[2] = -1;
  struct vector* b = mvmult(A, x);
  struct vector* ans = new_vector(3);
  ans->array[0] = 8.0;
  ans->array[1] = -11.0;
  ans->array[2] = -3;
  print_vector(b);
  printf("%s\n", vequal(b, ans) ? "true" : "false");
  free_matrix(A), free_vector(x), free_vector(b), free_vector(ans);
}


int main(){
  test_identity();
  test_transpose();
  test_constant_mult();
  test_matrix_multiply();
  test_vector_add();
  test_forward_elimination();
  test_back_substitution();
  test_gaussian_elimination();
  test_matrix_vector_multiply();
  test_random_gaussian_elimination();
  return 0;
}
