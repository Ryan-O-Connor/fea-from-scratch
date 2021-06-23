struct essential_bc{
  int node_id;
  int dof;
  double value;
};

struct nodal_force{
  int node_id;
  int dof;
  double value;
};


struct essential_bc* new_essential_bc(int node_id, int dof, double value);
struct nodal_force* new_nodal_force(int node_id, int dof, double value);
double get_essential_bc(struct list* essential_bcs, int node_id, int dof);
double get_nodal_force(struct list* nodal_forces, int node_id, int dof);
void print_essential_bc(struct essential_bc* ebc);
void print_nodal_force(struct nodal_force* ndf);
void free_essential_bc(void* ebc);
void free_nodal_force(void* ndf);
int is_constrained(struct list* essential_bcs, int node_id, int dof);
