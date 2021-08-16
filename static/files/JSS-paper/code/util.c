#define _UTIL_C
#include "util.h"

float max_vec(float *x, int n)
{
  int i;
  float max1 = x[0];

  for (i=1; i<n; i++)
    if (x[i] > max1)
      max1 = x[i];

  return(max1);
}

float min_vec(float *x, int n)
{
  int i;
  float min1 = x[0];

  for (i=1; i<n; i++)
    if (x[i] < min1)
      min1 = x[i];

  return(min1);
}

float mean_fn(float *x1, int n, int *rows_in_plot)
{
    int i;
    float tmpf;
    float mean1;

    tmpf = 0.;
    for (i=0; i<n; i++)
        tmpf += (x1[rows_in_plot[i]]-x1[0]);
    mean1 = tmpf / (float)n;
    mean1 += x1[0];

    return(mean1);
}

float mean_fn2(float *x1, float *x2, int n, int *rows_in_plot)
{
    int i;
    float tmean, tmpf1;
    float mean1, mean2;

    tmpf1 = 0.;
    for (i=0; i<n; i++)
        tmpf1 += x1[rows_in_plot[i]];
    mean1 = tmpf1 / (float)n;
    tmpf1 = 0.;
    for (i=0; i<n; i++)
        tmpf1 += x2[rows_in_plot[i]];
    mean2 = tmpf1 / (float)n;
    tmean = 0.;
    for (i=0; i<n; i++) {
        tmean += ((x1[rows_in_plot[i]]-mean1)*(x2[rows_in_plot[i]]-mean2));
    }
    tmean /= ((float)n);
    tmean += (mean1*mean2);

    return(tmean);
}

float mean_fn3(float *x1, float *x2, float *wgts, int n, int *rows_in_plot)
{
    int i;
    float tmean, tmpf1;
    float mean1, mean2, mean3;

    tmpf1 = 0.;
    for (i=0; i<n; i++)
        tmpf1 += x1[rows_in_plot[i]];
    mean1 = tmpf1 / (float)n;
    tmpf1 = 0.;
    for (i=0; i<n; i++)
        tmpf1 += x2[rows_in_plot[i]];
    mean2 = tmpf1 / (float)n;
    tmpf1 = 0.;
    for (i=0; i<n; i++)
        tmpf1 += wgts[rows_in_plot[i]];
    mean3 = tmpf1 / (float)n;
    tmean = 0.;
    for (i=0; i<n; i++) {
        tmean += ((x1[rows_in_plot[i]]-mean1)*(x2[rows_in_plot[i]]-mean2)*
            wgts[rows_in_plot[i]]);
    }
    tmean /= ((float)n);
    tmean += (mean1*mean2);

    return(tmean);
}

float mean_fn5(float *x1, float *x2, float *x3, float *x4, float *wgts,
               int n, int *rows_in_plot)
{
    int i;
    float tmean;

    tmean = 0.;
    for (i=0; i<n; i++) {
        tmean += (x1[rows_in_plot[i]]*x2[rows_in_plot[i]]*
            x3[rows_in_plot[i]]*x4[rows_in_plot[i]]*wgts[rows_in_plot[i]]);
    }
    tmean /= ((float)n);

    return(tmean);
}

float calc_norm(float *x, int n)
{
    int j;
    double xn = 0;

    for (j=0; j<n; j++)
        xn = xn + (double)(x[j]*x[j]);
    xn = sqrt(xn);

    return((float)xn);
}

void norm(float *x, int n)
{
  int j;
  double xn = 0;

  for (j=0; j<n; j++)
    xn = xn + x[j]*x[j];
  xn = sqrt(xn);
  for (j=0; j<n; j++)
    x[j] = x[j]/xn;
}

float inner_prod(float *x1, float *x2, int n)
{
  double xip;
  int j;

  xip = 0.;
  for (j=0; j<n; j++)
    xip = xip + (double)x1[j]*(double)x2[j];
  return((float)xip);
}

void gram_schmidt(float *x1, float *x2, int n)
{
  int j;
  float ip;

  ip = inner_prod(x1, x2, n);
  for (j=0; j<n; j++)
    x2[j] = x2[j] - ip*x1[j];

  norm(x2, n);
}

float sq_inner_prod(float *x1, float *x2, int n)
{
  float xip;
  int j;

  xip = 0;
  for (j=0; j<n; j++)
    xip = xip + x1[j]*x2[j];
  return(xip*xip);
}

void mat_mult(float **a, float **b, float **matprod, int m, int *mv, int n, int p)
{
  int i,j,k;
  float tmpf;

  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
    {
      tmpf = 0.;
      for (k=0; k<p; k++)
        tmpf += (a[k][i]*b[k][j]);
/*        tmpf += (a[k][mv[i]]*b[k][j]);*//*jul 97*/
      matprod[j][i] = tmpf;
    }
}
  
void tmat_mult(float **a, float **b, float **matprod, int m, int n, int p, 
  int *pv)
{
  int i,j,k;
  float tmpf;

  for (i=0; i<m; i++)
  {
    for (j=0; j<n; j++)
    {
      tmpf = 0.;
      for (k=0; k<p; k++)
        tmpf += (a[i][pv[k]]*b[j][pv[k]]);
      matprod[i][j] = tmpf;
    }
  }
}

#undef _UTIL_C
