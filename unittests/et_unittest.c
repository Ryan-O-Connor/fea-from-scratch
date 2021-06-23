#include <stdio.h>
#include <stdlib.h>
#include "../src/element_types.h"


void test_et_def(){
  struct et_def* et = new_et_def(1, "SBAR");
  set_real_constant(et, 3, 0.3);
  set_keyopt(et, 5, 3);
  set_matprop(et, "E", 30e6);
  print_et_def(et);
  free_et_def(et);
}


int main(){
  test_et_def();
  return 0;
}
