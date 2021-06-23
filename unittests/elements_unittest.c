#include <string.h>
#include <stdio.h>
#include "../src/headers/elements.h"

int main(){
  char* test = strdup("TSolid4");
  printf("%s\n", test);
  int i = lookup_element_type(test);
  printf("%d\n", i);
  return 0;
}
