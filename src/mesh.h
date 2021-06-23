/*
Functions and data structures for creating mesh geometry
Does not hold any element type, option, or material property data
*/


struct node{
  double x;
  double y;
};


struct element{
  int et_id;
  int* IEN;
};


struct node* new_node(double x, double y);
struct element* new_element(int et_id, int* IEN);
void print_node(struct node* n);
void print_element(struct element* e, int nenodes);
void print_mesh(struct list* nodes, struct list* elements, int* nenodes);
void free_mesh(struct list* nodes, struct list* elements);
