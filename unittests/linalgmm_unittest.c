#include "../src/lib/linalg.h"


struct matrix* test_new_matrix(){
  return new_matrix(4, 4);
}


void test_void_matrix(struct matrix* A){
  A->array[0][0] = 500;
}


int main(){
  struct matrix* A = test_new_matrix();
  test_void_matrix(A);
  print_matrix(A);
  return 0;
}
