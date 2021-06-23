struct matrix{
  double** array;
  int nrows;
  int ncols;
};


struct vector{
  double* array;
  int n;
};


// Constructors and destructors
struct matrix* new_matrix(int nrows, int ncols);
struct matrix* new_triangular_matrix(int n, int is_upper);
struct matrix* new_identity_matrix(int n);
struct vector* new_vector(int n);
struct matrix* copy_matrix(struct matrix* A);
struct vector* copy_vector(struct vector* u);
void free_matrix(struct matrix* A);
void free_vector(struct vector* u);

// Output and checking
int mequal(struct matrix* A, struct matrix* B);
int vequal(struct vector* u, struct vector* v);
void print_matrix(struct matrix* A);
void print_vector(struct vector* u);

// Row and matrix manipulations
struct matrix* mtranspose(struct matrix* A);
double determinant2x2(struct matrix* A);
struct matrix* minverse2x2(struct matrix* A);

// Basic addition, multiplication, and manipulation
void cmmult(struct matrix* A, double c);
struct matrix* mmadd(struct matrix* A, struct matrix* B);
struct vector* vvadd(struct vector* u, struct vector* v);
struct matrix* mmmult(struct matrix* A, struct matrix* B);
struct vector* mvmult(struct matrix* A, struct vector* x);


// Linear system solvers (LSS)
void forward_elimination(struct matrix* A, struct vector* b);
void back_substitution(struct matrix* A, struct vector* b);
void gaussLSS(struct matrix* A, struct vector* b);

// Eigenvalue solvers
