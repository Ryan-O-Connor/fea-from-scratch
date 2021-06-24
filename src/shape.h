double jacobian(struct matrix* COORDS, struct matrix* NDERNAT);
struct matrix* construct_COORDS(struct list* nodes, int IEN[], int nenodes);
struct list* construct_NDERNATs(struct et_def* et);
struct list* construct_NDERGLBs(struct matrix* COORDS,
				struct list* NDERNATs, int nint_pts);
