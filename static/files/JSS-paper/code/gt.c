 /************************************************************
 *                                                          *
 *  Permission is hereby granted  to  any  individual   or  *
 *  institution   for  use,  copying, or redistribution of  *
 *  the xgobi code and associated documentation,  provided  *
 *  that   such  code  and documentation are not sold  for  *
 *  profit and the  following copyright notice is retained  *
 *  in the code and documentation:                          *
 *     Copyright (c) held by Dianne Cook                    *
 *  All Rights Reserved.                                    *
 *                                                          *
 *  We welcome your questions and comments, and request     *
 *  that you share any modifications with us.               *
 *                                                          *
 *                   Dianne Cook                            *
 *                dicook@iastate.edu                        *
 *                                                          *
 ************************************************************/
/* gt.c */
#define _GT_C
#define WINDOWSIZE 1.0

#include <sys/types.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs_and_types.h"
#include "util.h"
#include "gt.h"
#include "gt_util.h"

/* local variables */
static float **gt_world;
static float **gt_proj;
static float **gt_plot;
static float *shift;
static float **xi, **xi0, **xi1;
float **u, **u_init;
static float **u0, **u1;
static float **v0, **v1;
static float **b;
static float **w, **w0, **w1;
static lims *lim;
int *tour_vars, *active;
int ntour_vars;
static float **tv, **tmat;
static float s[PROJ_DIM], coss[PROJ_DIM], sins[PROJ_DIM];
static float *tau, *tinc;
static float step, dv, delta;
static int spandim;
static int ds;
static int generate_new_path = 0;
int got_new_target_plane;
static int count=0;
int stepscale;
int pdim; 
int tourlength;

/* external variables */
extern int ncols, ncols_used, nrows;
extern char **collab;
extern char **color_name;
extern float **raw_data;
extern int *vgroup_ids;

void  *gt_malloc(size_t our_size)
{
    void *rpt=NULL;
    if((rpt=malloc(our_size))==NULL)
    {
	fprintf(stderr, "Could not allocate memory (gt_malloc in gt.c)\n");
	exit(1);
    }   
    return(rpt);        
}

/*
 * This only gives the correct result if the
 * vgroups vector is of the form {0,1,2,...,ngroups-1}
*/
int
numvargroups()
{
  int j, nvgroups = 0;

  for (j=0; j<ncols; j++)
    if (vgroup_ids[j] > nvgroups)
      nvgroups = vgroup_ids[j];
  nvgroups++;

  return(nvgroups);
}

/*
 * Find the minimum and maximum values of each column or variable
 * group scaling by mean and std_width standard deviations.
*/
void mean_lgdist(float **data, float *min, float *max)
{
  int i, j, k, n;
  double dx;
  double sumxi;
  double sumdist = 0.0;
  double *mean, lgdist = 0.0;
  int nvgroups = numvargroups();
  
  mean = (double *) gt_malloc((unsigned int) ncols * sizeof(double));
 
  for (j=0; j<ntour_vars; j++)
  {
    sumxi = 0.0;
    for (i=0; i<nrows; i++)
    {
      dx = (double) data[i][tour_vars[j]];
      sumxi += dx;
    }
    mean[tour_vars[j]] = sumxi / nrows;
  }

  for (k=0; k<nvgroups; k++)
  { 
    for (i=0; i<nrows; i++)
    {
      sumdist = 0.0;
      for (j=0; j<ntour_vars; j++)
      {
        if (vgroup_ids[tour_vars[j]] == k)
	{
          dx = (data[i][tour_vars[j]]-mean[tour_vars[j]]);
          dx *= dx;
          sumdist += dx;
	}
      }
      if (sumdist > lgdist)
        lgdist = sumdist;
    }

    lgdist = sqrt(lgdist);
    for (j=0; j<ntour_vars; j++)
    {
      if (vgroup_ids[tour_vars[j]] == k)
      {
        *min = mean[tour_vars[j]] - lgdist;
        *max = mean[tour_vars[j]] + lgdist;
        lim[tour_vars[j]].min = *min;
        lim[tour_vars[j]].max = *max;
       }
    }
  }

  free((void*) mean);
}

/* This routine preprojects the data into the subspace spanned by 
 * the starting and ending basis to speed computation during the 
 * interpolation stage of the tour.
*/
void preproject_data()
{
  int i, k;
  float inner_prod(float *x1, float *x2, int n);
  static int firsttime = 1;

  for (k=0; k<ds; k++)
    for (i=0; i<nrows; i++)
    {
      xi[k][i] = inner_prod(gt_world[i], b[k], ncols);
      
      if (i==8) firsttime = 0;
    }
}

void tour_reproject()
/*
 * This routine uses the data projected into the span of
 * the starting basis and ending basis, and then rotates in this
 * space.
*/
{
  float tmpf;
  int i, j, k;
  float costf[PROJ_DIM], sintf[PROJ_DIM];
  static int firsttime = 1; 
  int firstvar[PROJ_DIM];

  if (ntour_vars < pdim + 1) /* Do projection for the case when the number
                              * of active variables is <= the projection 
                              * dimension. */
  {
    /* find first variable */
    for (j=0; j<ntour_vars; j++)
    {
      for (k=0; k<pdim; k++)
        if (u[k][tour_vars[j]] == 1.0)
         firstvar[k] = tour_vars[j];
    }

    /* generate data projection */
    for (i=0; i<nrows; i++)
    {
      for (j=0; j<pdim; j++)
        gt_proj[i][j] = gt_world[i][firstvar[j]];
    }

  }
  else /* The general case when the number of variables is > the 
        * projection dimension. */
  {
    for (i=0; i<spandim; i++)
    {
      costf[i] = cosf(tinc[i]); 
      sintf[i] = sinf(tinc[i]);
    }
  
    /* basically do calculations ready for use in drawing
     * segments in variable spheres */

    for  (i=0; i<spandim; i++)
      for  (k=0; k<pdim; k++)
      { 
        w[2*i][k] = costf[i]*w0[2*i][k]-sintf[i]*w0[2*i+1][k];
        w[2*i+1][k] = sintf[i]*w0[2*i][k]+costf[i]*w0[2*i+1][k];
      }
    for (i=2*spandim; i<ds; i++)
      for  (k=0; k<pdim; k++)
      {
        w[i][k] = w0[i][k];
      }

  /* generate data projection */
    for (i=0; i<nrows; i++)
    {
      for (j=0; j<pdim; j++)
      {
        tmpf = 0.;
        for (k=0; k<ds; k++)
          tmpf += (w[k][j]*xi[k][i]);
        gt_proj[i][j] = tmpf;
      }
    }
  
  /* generate projection coordinates */
    for (j=0; j<ncols; j++)
      for (k=0; k<pdim; k++)
      {
        tmpf = 0.;
        for (i=0; i<ds; i++)
          tmpf += (b[i][j]*w[i][k]);
        u[k][j] = tmpf;
      }
  }
}

/* Generate the data values to plot */
void scale_into_window()
{
  int i, j;

  for (i=0; i<nrows; i++)
  {
    for (j=0; j<pdim; j++)
      gt_plot[i][j] = gt_proj[i][j]*WINDOWSIZE+shift[j];
  }
}

/* This routine take the raw data and scales it into a 
 * -1 to +1 p-dimensional box.
*/
void gt_pipeline()
{
  int i, j;
  float min, max;
  float rdiff;
  double pow();

  mean_lgdist(raw_data, &min, &max);

  for (j=0; j<ntour_vars; j++)
  {
    rdiff = lim[tour_vars[j]].max - lim[tour_vars[j]].min;
    for (i=0; i<nrows; i++)
    {
      gt_world[i][tour_vars[j]] = -1.0 + 2.0*(raw_data[i][tour_vars[j]] - 
        lim[tour_vars[j]].min)/rdiff;
    }
  }
  
}

void zero_tau()
{
  tau[0] = tau[1] = tau[2] = 0.0;
}

void zero_tinc()
{
  tinc[0] = tinc[1] = tinc[2] = 0.0;
}


void d_alloc_gt()
{
  int i, k;
    
  gt_world = (float **) gt_malloc((unsigned int) nrows * sizeof(float *));
  for (i=0; i<nrows; i++)
    gt_world[i] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  gt_proj = (float **) gt_malloc((unsigned int) nrows * sizeof(float *));
  for (i=0; i<nrows; i++)
    gt_proj[i] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  gt_plot = (float **) gt_malloc((unsigned int) nrows * sizeof(float *));
  for (i=0; i<nrows; i++)
    gt_plot[i] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  shift = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  lim = (lims *) gt_malloc((unsigned int) ncols * sizeof(lims));
  tour_vars = (int *) gt_malloc((unsigned int) ncols * sizeof(int));
  active = (int *) gt_malloc((unsigned int) ncols * sizeof(int));
  xi = (float **) gt_malloc((unsigned int) (2*PROJ_DIM) * sizeof(float *));
  for (k=0; k<2*PROJ_DIM; k++)
    xi[k] = (float *) gt_malloc((unsigned int) nrows * sizeof(float));
  xi0 = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    xi0[k] = (float *) gt_malloc((unsigned int) nrows * sizeof(float));
  xi1 = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    xi1[k] = (float *) gt_malloc((unsigned int) nrows * sizeof(float));
  u = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    u[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  u_init = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    u_init[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  u0 = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    u0[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  u1 = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    u1[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  v0 = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    v0[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  v1 = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    v1[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  b = (float **) gt_malloc((unsigned int) (2*PROJ_DIM) * sizeof(float *));
  for (k=0; k<(2*PROJ_DIM); k++)
    b[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  w = (float **) gt_malloc((unsigned int) (2*PROJ_DIM) * sizeof(float *));
  for (k=0; k<(2*PROJ_DIM); k++)
    w[k] = (float *) gt_malloc((unsigned int) PROJ_DIM * sizeof(float));
  w0 = (float **) gt_malloc((unsigned int) (2*PROJ_DIM) * sizeof(float *));
  for (k=0; k<(2*PROJ_DIM); k++)
    w0[k] = (float *) gt_malloc((unsigned int) PROJ_DIM * sizeof(float));
  w1 = (float **) gt_malloc((unsigned int) (2*PROJ_DIM) * sizeof(float *));
  for (k=0; k<(2*PROJ_DIM); k++)
    w1[k] = (float *) gt_malloc((unsigned int) PROJ_DIM * sizeof(float));
  tv = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    tv[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  tmat = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
  for (k=0; k<PROJ_DIM; k++)
    tmat[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));
  tau = (float*) gt_malloc((unsigned int) PROJ_DIM * sizeof(float));
  tinc = (float*) gt_malloc((unsigned int) PROJ_DIM * sizeof(float));

}

void free_gt()
{
  int i, k;

  for (i=0; i<nrows; i++)
    free((void*) gt_world[i]);
  free((void*) gt_world);
  for (i=0; i<nrows; i++)
    free((void*) gt_proj[i]);
  free((void*) gt_proj);
  for (i=0; i<nrows; i++)
    free((void*) gt_plot[i]);
  free((void*) gt_plot);
  free((void*) shift);
  free((void*) lim);
  free((void*) tour_vars);
  free((void*) active);
  for (k=0; k<2*PROJ_DIM; k++)
    free((void*) xi[k]);
  free((void*) xi);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) xi0[k]);
  free((void*) xi0);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) xi1[k]);
  free((void*) xi1);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) u[k]);
  free((void*) u);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) u_init[k]);
  free((void*) u_init);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) u0[k]);
  free((void*) u0);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) u1[k]);
  free((void*) u1);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) v0[k]);
  free((void*) v0);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) v1[k]);
  free((void*) v1);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) b[k]);
  free((void*) b);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) w[k]);
  free((void*) w);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) w0[k]);
  free((void*) w0);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) w1[k]);
  free((void*) w1);
  for (k=0; k<PROJ_DIM; k++)
    free((void*) tv[k]);
  free((void*) tv);
}

/* Read initial configuration of variables: which ones are active and which
 * order of plotting is used, for example, var 1, var 3, var 2, ...
*/
void
read_config(char *data_in)
{
  char fname[100];
  FILE *fp;
  int fnd, i, j, k, fs;

  fnd = 0;
  if (data_in != NULL && strcmp(data_in,"stdin") != 0)
  {
    (void) strcpy(fname, data_in);
    (void) strcat(fname, ".config");
    if ( (fp = fopen(fname,"r")) != NULL)
      fnd = 1;
  }

  if (fnd)
  {
    for (j=0; j<ncols; j++)
    {
      fs = fscanf(fp, "%d", &active[j]);
      for (k=0; k<pdim; k++)
        fs = fscanf(fp, "%g", &u_init[k][j]);
    }
  }
  else
  {
    for (j=0; j<ncols; j++)
    {
      active[j] = 1;
      for (k=0; k<pdim; k++)
        u_init[k][j] = 0.;
    }
    for (k=0; k<pdim; k++)
      u_init[k][k] = 1.;
  }

}

void init_gt(char *data_in)
{
  int j, k;

  if (pdim > ncols)
  {
    printf("The projection dimension exceeds the number of variables in the data.\n");
    exit(1);
  }

  read_config(data_in);/* first projection and active variables */

  u0_gets_current_pos(u, u_init, ncols, ntour_vars, pdim); 
  
  /* set tour_vars vector */
  ntour_vars = 0;
  for (j=0; j<ncols; j++)
  {
    if (active[j] == 1)
    {
      tour_vars[ntour_vars] = j;
      ntour_vars++;
    }
  }

  delta = 0.0;
  dv = 1.0;
  step = STEP0*stepscale; /* Set the speed parameter, you need to change 
                           * this if you want the tour to run faster or 
                           * slower. */
  for (k=0; k<pdim; k++)
    tau[k] = tinc[k] = 0.0;
  clear_vars();

  spandim = MIN(ncols,pdim);
  for (k=0; k<pdim; k++)
    shift[k] = 0.;

  gt_pipeline();
  run_tour();
  got_new_target_plane = 1;

}

void clear_vars()
{
  int j, k;

  for (k=0; k<pdim; k++)
    for (j=0; j<ncols; j++)
    {
      v0[k][j] = 0.;
      v1[k][j] = 0.;
      b[2*k][j] = 0.;
      b[2*k+1][j] = 0.;
    }
  for (k=0; k<pdim; k++)
    for (j=0; j<pdim; j++)
    {
      w0[2*k][j] = 0.;
      w1[2*k][j] = 0.;
      w0[2*k+1][j] = 0.;
      w1[2*k+1][j] = 0.;
    }
}

/* This is the engine which drives the tour. */
void run_tour()
{
  int i, j, k;
  float tmpf;
  static float **uold;
  static int firsttime = 1;

  count++;

  if (firsttime==1)
  {
    uold = (float **) gt_malloc((unsigned int) PROJ_DIM * sizeof(float *));
    for (k=0; k<PROJ_DIM; k++)
      uold[k] = (float *) gt_malloc((unsigned int) ncols * sizeof(float));

    for (i=0; i<ncols; i++)
    {  
      for (j=0; j<pdim; j++)
        uold[j][i]=u[j][i];
    }
    firsttime = 0;
  }

  if (!reached_target_plane(ntour_vars, tinc, tau, delta, spandim, 
    got_new_target_plane))
  {
    tour_reproject();
    scale_into_window();
    for (i=0; i<ncols; i++)
    {
      /* Print projection vectors to standard output, change this if you 
       * have a plot driver working to plot gt_plot variable. */
      for (j=0; j<pdim; j++)
        printf("%f ",u[j][i]);
      printf("\n");
    }
    for (i=0; i<ncols; i++)
    {  
      for (j=0; j<pdim; j++)
        uold[j][i]=u[j][i];
    }
  }
  else
  {
    finishing_step(tinc, tau, spandim);
    u0_gets_current_pos(u0, u, ncols, ntour_vars, pdim);
    new_random_basis(u1, ncols, ntour_vars, tour_vars, pdim);
        
    clear_vars();
    zero_tinc();
    zero_tau();
    if (ntour_vars > pdim)
    {
      path(u0, u1, v0, v1, b, w0, w1, ncols, ntour_vars, tour_vars, pdim, 
        tau, tv, &dv, &delta, &step, &spandim, &ds);
      preproject_data();
    }
    
    for (i=0; i<ncols; i++)
    {
      /* Print projection vectors to standard output, change this if you 
       * have a plot driver working to plot gt_plot variable. */
      for (j=0; j<pdim; j++)
        printf("%f ",u[j][i]);
      printf("\n");
    }
    for (i=0; i<ncols; i++)
    {  
      for (j=0; j<pdim; j++)
        uold[j][i]=u[j][i];
    }
    tour_reproject();
    scale_into_window();
    generate_new_path = 0;
    got_new_target_plane = 1;
  }

}

void create_gt(char *data_in)
{
  d_alloc_gt();

  init_gt(data_in);
}

void init_wombat(char *filepar)
{
  extern int read_array(char *data_in);
  extern int read_vgroups(char *data_in);

  if (!read_array(filepar))
  {
    printf("Problem in reading data \n");
    exit(0);
  }
  else
  {
    if (!read_vgroups(filepar))
      printf("Problem in reading variable groups \n");
  }

  
  { 
    long seed;
    seed=(long) time((long *) 0);
    seed=845657246;
    srandom(seed);
  }
  
  create_gt(filepar);

  while (count<tourlength)
    run_tour();
}

#undef _GT_C

