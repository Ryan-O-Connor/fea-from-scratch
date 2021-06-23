/*
Functions for creating mesh geometry
Agnostic about element or analysis types
 */


#include <stdlib.h>
#include <stdio.h>
#include "lib/list.h"
#include "mesh.h"


struct node* new_node(double x, double y){
  struct node* n = malloc(sizeof(struct node));
  n->x = x;
  n->y = y;
  return n;
}


struct element* new_element(int et_id, int* IEN){
  struct element* e = malloc(sizeof(struct element));
  e->et_id = et_id;
  e->IEN = IEN;
  return e;
}


void print_node(struct node* n){
  printf("Node:\n");
  printf("\tX Coor.: %.4g\n", n->x);
  printf("\tY Coor.: %.4g\n", n->y);
}


void print_element(struct element* e, int nenodes){
  int i;
  printf("Element , Type #%d\n", e->et_id);
  for (i=0; i<nenodes; i++){
    printf("Local Node #%d: Global Node #%d\n", i+1, e->IEN[i]+1);
  }
}


void print_mesh(struct list* nodes, struct list* elements, int* nenodes){
  printf("Mesh: %d nodes, %d elements\n",
	 nodes->nitems, elements->nitems);
  int i;
  printf("Nodes:\n");
  for (i=0; i<nodes->nitems; i++){
    struct node* n = nodes->array[i];
    print_node(n);
  }
  printf("Elements:\n");
  for (i=0; i<elements->nitems; i++){
    struct element* e = elements->array[i];
    print_element(e, nenodes[e->et_id]);
  }
}


void free_node(void* n){
  struct node* N = n;
  free(N);
}


void free_element(void* e){
  struct element* E = e;
  free(E->IEN);
  free(E);
}


void free_mesh(struct list* nodes, struct list* elements){
  int i;
  struct element* e;
  free_items(nodes, free_node);
  free_list(nodes);
  free_items(elements, free_element);
  free_list(elements);
}
