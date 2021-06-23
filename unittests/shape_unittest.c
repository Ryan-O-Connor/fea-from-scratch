#include <stdio.h>
#include "../src/lib/geom.h"
#include "../src/lib/linalg.h"
#include "../src/shape.h"

void test_NDERNAT(){
  int lib_id = 4;
  struct point* pt = new_point(-.57735, -.57735);
  struct matrix* NDERNAT = get_NDERNAT(4, pt);
  print_matrix(NDERNAT);
  free_matrix(NDERNAT);
}

int main(){
  test_NDERNAT();
  return 0;
}
