#include "../src/bc_data.h"


void test_essential_bc(){
  struct essential_bc* ebcX = new_essential_bc(1, 0, 0.3);
  struct essential_bc* ebcY = new_essential_bc(1, 0, 0.5);
  print_essential_bc(ebcX), print_essential_bc(ebcY);
  free_essential_bc(ebcX), free_essential_bc(ebcY);
}


void test_nodal_force(){
  struct nodal_force* ndfX = new_nodal_force(1, 0, 0.3);
  struct nodal_force* ndfY = new_nodal_force(1, 0, 0.5);
  print_nodal_force(ndfX), print_nodal_force(ndfY);
  free_nodal_force(ndfX), free_nodal_force(ndfY);
}


int main(){
  test_essential_bc();
  test_nodal_force();
  return 0;
}
