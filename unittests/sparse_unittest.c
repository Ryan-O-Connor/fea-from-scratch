#include "../src/lib/sparse_linalg.h"


void test_aol_matrix(){
  struct aol_matrix* A = new_aol_matrix(5, 5);
  add_aol_element(A, 0, 2, 3);
  add_aol_element(A, 0, 4, 4);
  add_aol_element(A, 1, 2, 5);
  add_aol_element(A, 1, 3, 6);
  add_aol_element(A, 2, 0, 9);
  add_aol_element(A, 4, 1, 2);
  add_aol_element(A, 4, 2, 6);
  print_aol_matrix(A, 's');
  print_aol_matrix(A, 'f');
  free_aol_matrix(A);
}


int main(){
  test_aol_matrix();
  return 0;
}
