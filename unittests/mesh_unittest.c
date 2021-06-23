#include <stdio.h>
#include <stdlib.h>
#include "../src/lib/list.h"
#include "../src/mesh.h"


void test_mesh(){
  struct list* nodes = new_list();
  append(nodes, new_node(0.0, 0.0));
  append(nodes, new_node(0.0, 3.0));
  append(nodes, new_node(3.0, 3.0));
  append(nodes, new_node(3.0, 0.0));
  struct list* elements = new_list();
  int* IEN1 = malloc(2*sizeof(int));
  int* IEN2 = malloc(2*sizeof(int));
  int* IEN3 = malloc(2*sizeof(int));
  IEN1[0] = 0;
  IEN1[1] = 1;
  IEN2[0] = 0;
  IEN2[1] = 2;
  IEN3[0] = 0;
  IEN3[1] = 3;
  append(elements, new_element(1, IEN1));
  append(elements, new_element(1, IEN2));
  append(elements, new_element(1, IEN3));
  int* nenodes = malloc(2*sizeof(int));
  nenodes[1] = 2;
  print_mesh(nodes, elements, nenodes);
  free(nenodes);
  free_mesh(nodes, elements);
}


int main(){
  test_mesh();
  return 0;
}
