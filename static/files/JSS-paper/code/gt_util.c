#define _GT_UTIL_C

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs_and_types.h"
#include "util.h"
#include "gt_util.h"
#include "gt.h"

/* external variables */
extern int ntour_vars, *tour_vars, *tour_vars_bool;
extern int pdim;

int gen_norm_variates(int n, int p, float *vars)
{
  int i,  check=1;
  long runif[2];
  double frunif[2];
  double r, fac, frnorm[2];

  for (i=0; i<(n*p+1)/2; i++)
  {
    while (check)
    {
      runif[1] =  (int) random();
      runif[0] =  (int) random();
      frunif[0] = (double)(2.0 * ((float)(runif[0])/(float)(MAXINT)) -
        1.0);
      frunif[1] = (double)(2.0 * ((float)(runif[1])/(float)(MAXINT)) -
        1.0);
      r = frunif[0] * frunif[0] + frunif[1] * frunif[1];
      if (r < 1)
      {
        check = 0;
        fac = sqrt(-2. * log(r) / r);
        frnorm[0] = frunif[0] * fac;
        frnorm[1] = frunif[1] * fac;
      }
    }
    check = 1;
    vars[2*i] = (float)(frnorm[0]);
    vars[2*i+1] = (float)(frnorm[1]);
  }
  return(1);
}

/*
 * Generate two random p dimensional vectors to form new ending basis
*/
void new_random_basis(float **basis, int ncols, int ntour_vars, int *tour_vars, int pdim)
{
  int j, k;
  float *tmp_basis;

  tmp_basis = (float *) malloc((unsigned int) (pdim*ncols)*sizeof(float));
  gen_norm_variates(pdim, ntour_vars, tmp_basis);
 
  /* zero basis before filling */
  for (k=0; k<pdim; k++)
    for (j=0; j<ncols; j++)
      basis[k][j] = 0.0;

  /* fill in with new values and orthonormalize */
  for (k=0; k<pdim; k++)
    for (j=0; j<ntour_vars; j++)
      basis[k][tour_vars[j]] = tmp_basis[k*ntour_vars+j];
  for (k=0; k<pdim; k++)
    norm(basis[k], ncols);
  for (k=0; k<(pdim-1); k++)
    for (j=(k+1); j<pdim; j++)
      gram_schmidt(basis[k], basis[j], ncols);

  free((void*)tmp_basis);
}

/* Generates the new direction variables for the tour to go from 
 * the starting plane to the ending plane. The interpolation
 * is set up here.
*/
void path(float **u0, float **u1, float **v0, float **v1, float **b, 
     float **w0, float **w1, int ncols, int ntour_vars, int *tour_vars, 
     int pdim, float *tau, float **tv, 
     float *pdv, float *pdelta, float *step, int *pspandim, int *pds)
{
  int i, j, k;
  float tmpf, tol=0.00001;
  float costf[PROJ_DIM], sintf[PROJ_DIM];
  float dv, delta;
  float sorttau[3];
  int tauindx[3];
  int spandim, ds;

  spandim = pdim;

  dv = *pdv;
  delta = *pdelta;

/* Form u0'u1 */
  for (i=0; i<pdim; i++)
  {
    for (j=0; j<pdim; j++)
    {
      tmpf = 0.;
      for (k=0; k<ncols; k++)
        tmpf += (u0[i][k]*u1[j][k]);
      tv[i][j] = tmpf;
    }
  }
  
/* find principal directions and angles */
  if (!dsvd(tv,pdim,pdim,tau,v1))
    printf(" Singular Value Decomposition bombed \n");

/* sort singular values and tv, v1 */
  for (i=0; i<pdim-1; i++)
  {
    tmpf = tau[k=i];
    for (j=i+1; j<pdim; j++)
      if (tau[j] >= tmpf) 
        tmpf = tau[k=j];
    if (k != i)
    {
      tau[k] = tau[i];
      tau[i] = tmpf;
      for (j=0; j<pdim; j++)
      {
        tmpf = tv[j][i];
        tv[j][i] = tv[j][k];
        tv[j][k] = tmpf;
      }
      for (j=0; j<pdim; j++)
      {
        tmpf = v1[j][i];
        v1[j][i] = v1[j][k];
        v1[j][k] = tmpf;
      }
    }
  }
  
/* check if any singular values too close to 1 */
  for (k=0; k<pdim; k++)
    if (tau[k]+tol>1.0)
    {
      spandim -= 1;
    }

/* generate principal directions */
  mat_mult(u0, tv, v0, ncols, tour_vars, pdim, pdim);
  for (k=0; k<pdim; k++)
    for (i=0; i<pdim; i++)
    {
      tv[i][k] = v1[i][k];
      v1[i][k] = 0.;
    }
  mat_mult(u1, tv, v1, ncols, tour_vars, pdim, pdim);

  for (k=pdim-1; k>pdim-spandim-1; k--)
    gram_schmidt(v0[k],v1[k],ncols);

/* compute principal angles */
  for (k=0; k<spandim; k++) 
    tv[0][k] = (float)(acosf(tau[pdim-k-1])); 
  for (k=spandim; k<pdim; k++)
    tv[0][k] = 0.0;
  for (k=0; k<pdim; k++)
    tau[k] = tv[0][k];

/* construct preprojection basis */
  for (k=0; k<spandim; k++)
  {
    for (j=0; j<ncols; j++)
    {
      b[2*k][j] = v0[pdim-k-1][j]; 
      b[2*k+1][j] = v1[pdim-k-1][j]; 
    }
  }
  for (k=spandim; k<pdim; k++)
    for (j=0; j<ncols; j++)
    {
      b[spandim+k][j] = v0[pdim-k-1][j]; 
    }

  ds = spandim+pdim;

/* construct frames */
  for (i=0; i<pdim; i++)
    for (k=0; k<ds; k++)
    {
      tmpf=0.;
      for (j=0; j<ncols; j++)
        tmpf += (b[k][j]*u0[i][j]);
      w0[k][i] = tmpf;
      tmpf=0.;
      for (j=0; j<ncols; j++)
        tmpf += (b[k][j]*u1[i][j]);
      w1[k][i] = tmpf;
    }

  for (i=0; i<pdim; i++)
  {
    costf[i] = cosf(tau[i]); 
    sintf[i] = sinf(tau[i]);
  }

  tmpf = 0.;   
  for (k=0; k<spandim; k++)
    tmpf += (tau[k]*tau[k]);
  dv = sqrt(tmpf) ;
  delta = (*step)/dv;
  *pdv = dv;
  *pdelta = delta;
  *pspandim = spandim;
  *pds = ds;
    
}

/* Checks if the current position is within an increment of the
 * target plane. */
int reached_target_plane(int ntour_vars, float *tinc, float *tau, 
  float delta, int spandim, int got_new_target_plane)
{
  int ret_val = 0, k;
  int all_equal = 1;
  float tol = 0.000001;

  for (k=0; k<spandim; k++)
    if (fabs(tinc[k]-tau[k]) > tol)
      all_equal = 0;

  if (all_equal)
  {
    ret_val = 1;
  }
  else
  {
    for (k=0; k<spandim; k++)
      tinc[k] += (delta * tau[k]);
  
    for (k=0; k<spandim; k++)
      if (tinc[k] > tau[k])
        ret_val = 1;
  }

  return(ret_val);
}

void increment_tour()
{
  tour_reproject();
  scale_into_window();
  /* Put a plot function here if you have a graphics driver. */
}

/* If the last increment doesn't match up with the requested tau's
 * do one last small increment to get to the target basis. */
void finishing_step(float *tinc, float *tau, int spandim)
{
  int k, do_final_step = 0;
  
  for (k=0; k<spandim; k++)
    if (tinc[k] != tau[k])
    {
      tinc[k] = tau[k];
      do_final_step = 1;
    }

  if (do_final_step)
  {
    tour_reproject();
    scale_into_window();
  }
}

/* Fill u0 basis. */
void 
u0_gets_current_pos(float **u0, float **u, int ncols, int ntour_vars, int pdim)
{
  int j, k;

  for (k=0; k<pdim; k++)
    for (j=0; j<ncols; j++)
      u0[k][j] = u[k][j];

  for (k=0; k<pdim; k++)
    norm(u0[k], ncols);
  for (k=0; k<(pdim-1); k++)
    for (j=k+1; j<pdim; j++)
      gram_schmidt(u0[k], u0[j], ncols);
}

#undef _GT_UTIL_C

