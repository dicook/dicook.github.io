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
/* main.c */
#include <sys/types.h>
#include <stdio.h>
#include <math.h>

#include "defs_and_types.h"

/* external variables */
extern int ncols, ncols_used, nrows;
extern float **raw_data;

int main(int argc, char *argv[])
{
  char filenm[100];
  extern int pdim, tourlength, stepscale;

  if (argc < 2)
  {
    printf("Usage: gt datafilename <proj_dim tourlen stepscale>\n\n");
    printf("      proj_dim = Projection Dimension, default is 2\n");
    printf("      tourlen = The number of projections to calculate, default is 150\n");
    printf("      stepscale = The speed factor: 1 is slow, 100 is fast, default is 10\n");
    exit(0);
  }
  else
  {
    (void) strcpy(filenm, argv[1]);

    if (argc > 2)
      pdim = atoi(argv[2]);
    else
      pdim = 2;
  
    if (argc > 3)
      tourlength = atoi(argv[3]);
    else
      tourlength = 150;
  
    if (argc > 4)
      stepscale = atoi(argv[4]);
    else
      stepscale = 10;
  
  
    init_wombat(filenm);
  }
}

