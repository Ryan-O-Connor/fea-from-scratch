#include <stdlib.h>
#include <stdio.h>
#include "../src/lib/llist.h"


void test_sequential_insert_and_print(){
  struct llist* l = new_llist();
  insert(l, 1, 6);
  insert(l, 2, 2);
  insert(l, 3, 10);
  insert(l, 4, 9);
  insert(l, 5, 1);
  print_llist(l);
  free_llist(l);
}


void test_reverse_insert_and_print(){
  struct llist* l = new_llist();
  insert(l, 5, 6);
  insert(l, 4, 2);
  insert(l, 3, 10);
  insert(l, 2, 9);
  insert(l, 1, 1);
  print_llist(l);
  free_llist(l);
}


void test_middle_insert_and_print(){
  struct llist* l = new_llist();
  insert(l, 5, 6);
  insert(l, 1, 2);
  insert(l, 3, 10);
  insert(l, 4, 9);
  insert(l, 2, 1);
  print_llist(l);
  free_llist(l);
}


int main(){
  test_sequential_insert_and_print();
  test_reverse_insert_and_print();
  test_middle_insert_and_print();
  return 0;
}
