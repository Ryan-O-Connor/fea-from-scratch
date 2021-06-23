#include <stdio.h>
#include "../src/lib/list.h"


void test_str_list(){
  struct list* l = new_list();
  append(l, "Hello");
  append(l, "Beautiful");
  append(l, "World");
  append(l, "You");
  int i;
  for (i=0; i<l->nitems; i++)
    printf("%s\n", (char*) l->array[i]);
  free_list(l);
}


void test_dbl_list(){
  struct list* l = new_list();
  int *a, *b, *c, *d;
}


int main(){
  test_str_list();
  return 0;
}
