#include "../src/lib/linalg.h"
#include "../src/element_types.h"
#include "../src/stiffness.h"

void test_SBar_KE(){
  struct et_def* et = new_et_def(1, "SBAR");
  set_real_constant(et, 1, 6e-4);
  set_matprop(et, "E", 2e11);
  print_et_def(et);
  struct matrix* COORDS = new_matrix(2, 2);
  COORDS->array[0][0] = 0.0;
  COORDS->array[0][1] = 1.03923;
  COORDS->array[1][0] = 0.0;
  COORDS->array[1][1] = 0.6;
  struct matrix* KE = KE_matrix(et, COORDS);
  print_matrix(KE);
  free_matrix(KE), free_matrix(COORDS), free_et_def(et);
}

int main(){
  test_SBar_KE();
  return 0;
}
