#ifdef _GT_C

void mean_lgdist(float **data, float *min, float *max);
void preproject_data();
void tour_reproject();
void scale_into_window();
void gt_pipeline();
void zero_tau();
void zero_tinc();
void d_alloc_gt();
void free_gt();
void init_gt();
void clear_vars();
void run_tour();
void create_gt();
void init_wombat(char *filepar);
#else

extern void mean_lgdist(float **data, float *min, float *max);
extern void preproject_data();
extern void tour_reproject();
extern void scale_into_window();
extern void gt_pipeline();
extern void zero_tau();
extern void zero_tinc();
extern void d_alloc_gt();
extern void free_gt();
extern void init_gt();
extern void clear_vars();
extern void run_tour();
extern void create_gt();
extern void init_wombat(char *filepar);
#endif
