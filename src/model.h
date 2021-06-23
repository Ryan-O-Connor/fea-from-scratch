/*
Functions for interpreter to interface with the finite element model.
Interpreter can only talk to the model.  It cannot access any underlying
FEM data structures (mesh, solver, etc.) directly. So all of these functions
are redundant, but it provides better encapsulation.
Only data the model holds are the global matrices and vectors K, F, and U
*/

struct model{
  int nsd;
  int ndof;
  int free_dof;
  int total_dof;
  struct list* nodes;
  struct list* elements;
  struct list* et_defs;
  struct list* essential_bcs;
  struct list* nodal_forces;
  struct static_soln* solution;
};


// Model interface
struct model* new_model();
void free_model(struct model* running_model);


// Mesh interface
void new_model_node(struct model* running_model, double x, double y);
void new_model_element(struct model* running_model, int et_id, int* IEN);
void print_model_mesh(struct model* running_model);


// Element type definition interface
void new_model_element_type(struct model* running_model, int et_id,
			    char* et_name);
void set_model_et_real_constant(struct model* running_model, int et_id,
				int real_constant, double value);
void set_model_et_matprop(struct model* running_model, int et_id,
			  char* name, double value);
void set_model_et_keyopt(struct model* running_model, int et_id,
			 int key, int option);

// Boundary condition definition interface
void add_model_essential_bc(struct model* running_model,
			    int node_id, char* comp, double value);
void add_model_nodal_force(struct model* running_model,
			   int node_id, char* comp, double value);

// Solver interface
void solve_model(struct model* running_model, int p_type, int s_type);

// Postprocessing interface
void print_model_result(struct model* running_model, char* res_name);
