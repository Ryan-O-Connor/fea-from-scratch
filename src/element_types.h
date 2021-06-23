// General data structures for holding element data
// Based on ANSYS APDL language constructs


struct matprops{
  double E;
  double v;
  double K;
};


struct solver_data{
  int nint_pts;
  double* int_wts;
  struct list* int_pts;
  struct list* NDERNATs;
  struct matrix* D;
};


struct et_def{
  int user_id;
  int lib_id;
  int nenodes;
  int ndof;
  int opts[10];
  double consts[10];
  struct matprops* mprops;
  struct solver_data* sdata;
};

struct et_def* new_et_def(int user_id, char* type_name);
struct et_def* get_et_def(struct list* et_defs, int user_id);
void set_real_constant(struct et_def* et, int const_id, double value);
void set_matprop(struct et_def* et, char* prop_name, double value);
void set_keyopt(struct et_def* et, int key, int option);
void print_et_def(struct et_def* et);
void free_et_def(void* et);


int get_nint_pts(int lib_id, int integration);
struct list* get_int_pts(int lib_id, int integration);
double* get_int_wts(int lib_id, int integration);
int integrated_element(int lib_id);


