#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "list.h"


static void resize(struct list* l, int new_size){
  l->array = realloc(l->array, new_size*sizeof(void*));
  l->array_size = new_size;
}


struct list* new_list(){
  struct list* l = malloc(sizeof(struct list));
  void** array = malloc(sizeof(void*));
  l->array = array;
  l->nitems = 0;
  l->array_size = 1;
  return l;
}


void append(struct list* l, void* item){
  if (l->nitems == l->array_size){
    resize(l, 2*l->array_size);
  }
  l->array[l->nitems++] = item;
}


void* get(struct list* l, int i){
  assert(i < l->nitems);
  return l->array[i];
}


void free_items(struct list* l, void (*free_item)(void*)){
  int i;
  for (i=0; i<l->nitems; i++)
    free_item(l->array[i]);
}


void free_list(struct list* l){
  free(l->array);
  free(l);
}
