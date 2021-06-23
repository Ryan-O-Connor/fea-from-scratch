#include <stdlib.h>
#include <stdio.h>
#include "geom.h"

struct point* new_point(double x, double y){
  struct point* pt = malloc(sizeof(struct point));
  pt->x = x;
  pt->y = y;
  return pt;
}

void print_point(struct point* pt){
  printf("Point = (x=%f, y=%f)\n", pt->x, pt->y);
}
