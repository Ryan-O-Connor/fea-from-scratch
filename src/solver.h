struct static_soln{
  int ndof;
  struct matrix* ID;
  struct vector* U;
};

struct static_soln* dense_static_solver(struct model* running_model);
void free_static_soln(struct static_soln* sol);
