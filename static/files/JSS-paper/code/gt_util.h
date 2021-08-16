#ifdef _GT_UTIL_C

int gen_norm_variates(int n, int p, float *vars);
void new_random_basis(float **basis, int ncols, int ntour_vars,int *tour_vars,
     int pdim);
void path(float **u0, float **u1, float **v0, float **v1, float **b, 
     float **w0, float **w1, int ncols, int ntour_vars, int *tour_vars, 
     int pdim, float *tau, float **tv, float *pdv, 
     float *pdelta, float *step, int *pspandim, int *pds);
extern int dsvd(float **a, int m, int n, float *w, float **v);
int reached_target_plane(int ntour_vars, float *tinc, float *tau, 
                         float delta, int spandim, int got_new_target_plane);
void increment_tour();
void finishing_step(float *tinc, float *tau, int spandim);
void u0_gets_current_pos(float **u0, float **u,int ncols, int ntour_vars, 
  int pdim);

#else

extern int gen_norm_variates(int n, int p, float *vars);
extern void new_random_basis(float **basis, int ncols, int ntour_vars, 
  int *tour_vars, int pdim);
extern void path(float **u0, float **u1, float **v0, float **v1, float **b, 
                 float **w0, float **w1, int ncols, int ntour_vars, 
                 int *tour_vars, int pdim, float *tau, float **tv, float *pdv, 
		 float *pdelta, float *step, int *pspandim, int *pds);
extern int dsvd(float **a, int m, int n, float *w, float **v);
extern int reached_target_plane(int ntour_vars, float *tinc, float *tau, 
                                float delta, int spandim, 
                                int got_new_target_plane);
extern void increment_tour();
extern void finishing_step(float *tinc, float *tau, int spandim);
extern void u0_gets_current_pos(float **u0, float **u,int ncols, 
  int ntour_vars, int pdim);

#endif
