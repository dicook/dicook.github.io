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
 *  This routine is adapted from code available in XGobi.   *
 *                                                          *
 ************************************************************/
/* read.c */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "defs_and_types.h"

/* defines for reads */
#define NCOLS 1500
#define INITSTRSIZE 1500
#define COLLABLEN 25
#define ROWLABLEN 50
#define NGLYPHTYPES 7
#define NGLYPHSIZES 5
#define NGLYPHS ((NGLYPHTYPES-1)*NGLYPHSIZES + 1)

/* data variables */
float **raw_data;
int ncols, ncols_used, nrows, nrows_used;

/* variable groups */
int *vgroup_ids;

void  *gt_2_malloc(size_t our_size)
{
    void *rpt=NULL;
    if((rpt=malloc(our_size))==NULL)
    {
	fprintf(stderr, "Could not allocate memory (gt_2_malloc in read.c)\n");
	exit(1);
    }   
    return(rpt);   
}

void *gt_realloc(void *p, size_t our_size)
{
    void *rpt=NULL;
    if((rpt=realloc(p, our_size))==NULL)
    {
	fprintf(stderr, "Could not allocate memory (gt_realloc in read.c)\n");
	exit(1);
    } 
    return(rpt);        
}

int read_array(char *data_in)
{
  register int ch;
  int i, j, jrows, jcols, fs;
  int nitems;
  float row1[NCOLS];
  int nblocks;
  char fname[100];
  FILE *fp;

  if (strcmp(data_in,"stdin") == 0) 
    fp = stdin;
  else
  {

    if ((fp = fopen(data_in, "r")) == NULL)
    {
      strcpy(fname, data_in);
      strcat(fname, ".dat");
      if ((fp = fopen(fname, "r")) == NULL)
      {
        (void) fprintf(stderr,
          "Neither the file %s nor %s exists\n", data_in, fname);
        return(0);
      }
    }
  }
/*
 * Read in the first row of the data file and calculate ncols.
*/
  ncols_used = 0;
  while ( (ch = getc(fp)) != '\n')
  {
    if (ch == '\t' || ch == ' ')
      ;
    else
    {
      if ( ungetc(ch, fp) == EOF ||
         fscanf(fp, "%g", &row1[ncols_used++]) < 0 )
      {
        fprintf(stderr, "error in ungetc or fscanf");
        return(0);
      }
    }
  }

  ncols = ncols_used;
/*
 * Allocate space for 500 rows, leaving two extra slots for the
 * extra variables; one sometimes mapped, one never mapped.
*/
  raw_data = (float **) gt_2_malloc(
    (unsigned int) 500 * sizeof(float *));
  for (i=0; i<500; i++)
    raw_data[i] = (float *) gt_2_malloc(
      (unsigned int) ncols * sizeof(float));

/*
 * Fill in the first row
*/
  for (j=0; j<ncols_used; j++)
  {
    raw_data[0][j] = row1[j];
   }

/*
 * Read data, gt_reallocating whenever 500 more rows are read in.
 * Determine nrows.
*/
  nblocks = 1;
  nitems = ncols_used;
  nrows = jrows = 1;
  jcols = 0;
  while (1)
  {
    fs = fscanf(fp, "%g", &raw_data[nrows][jcols]);
    jcols++;

    if (fs == EOF)
      break;
    else if (fs < 0)
    {
      fprintf(stderr, "error in fscanf in read_array\n");
      return(0);
    }
    else
    {
      nitems++;

      if (jcols == ncols_used)
      {
        jcols = 0;
        nrows++;
        jrows++;
      }
      if (jrows == 500)
      {
        jrows = 0;
        nblocks++;

        raw_data = (float **) gt_realloc((char *)raw_data,
          (unsigned int) (nblocks * 500) * sizeof(float *));
        for (i=500*(nblocks-1); i<500*nblocks; i++)
          raw_data[i] = (float *) gt_2_malloc(
            (unsigned int) ncols * sizeof(float));
      }
    }
  }

/*
 * Close the data file
*/
  if (fclose(fp) == EOF)
    fprintf(stderr, "error in fclose in read_array");

  if ( nitems != nrows * ncols_used )
  {
    (void) fprintf(stderr, "read_array: nrows*ncols != nitems\n");
    return(0);
  }
  else
  {
     /*
     * One last free and gt_realloc to make raw_data take up exactly
     * the amount of space it needs.
    */
    for (i=nrows; i<500*nblocks; i++)
      free((void*) raw_data[i]);
    raw_data = (float **) gt_realloc((void*) raw_data,
      (unsigned int) nrows * sizeof(float *));

    /*
     * If the data contains only one column, add a second,
     * the numbers 1:nrows.
    */
    if (ncols_used == 1)
    {
      ncols_used = 2;
      ncols = 3;
      for (i=0; i<nrows; i++)
      {
        raw_data[i] = (float *) gt_realloc(
          (void*) raw_data[i],
          (unsigned int) 3 * sizeof(float));
        raw_data[i][1] = (float) (i+1) ;
      }
    }
    return(1);
  }
}

int
read_vgroups(char *data_in)
/*
 * Read in the grouping numbers for joint scaling of variables
 */
{
  char lab_file[115], suffix[15];
  int itmp, i, j, found = 0;
  int id, newid, maxid;
  FILE *fp;

  vgroup_ids = (int *) gt_2_malloc(
    (unsigned int) (ncols+1) * sizeof(int));

  (void) strcpy(suffix, ".vgroups");

  if (data_in != NULL && strcmp(data_in, "stdin") != 0)
  {
    i = 0;
    (void) strcpy(lab_file, data_in);
    (void) strcat(lab_file, suffix);
    if ( (fp = fopen(lab_file,"r")) != NULL)
      found = 1;
  }

  if (found)
  {
    i = 0;
    while ((fscanf(fp, "%d", &itmp) != EOF) && (i < ncols))
      vgroup_ids[i++] = itmp;

    /*
     * Add the vgroup_id value for the extra column.
    */
    if (i == ncols)
    {
      vgroup_ids[i] = i;
    }

    if (i < ncols)
    {
      (void) fprintf(stderr,
        "Number of variables and number of group types do not match.\n");
      (void) fprintf(stderr,
        "Creating extra generic groups.\n");
      for (j=i; j<ncols; j++)
        vgroup_ids[j] = j;
    }

/*
 * Find maximum vgroup id.
*/
    maxid = 0;
    for (i=1; i<ncols; i++)
      if (vgroup_ids[i] > maxid)
        maxid = vgroup_ids[i];
/*
 * Find minimum vgroup id, set it to 0.  Find next, set it to 1; etc.
*/
    id = 0;
    newid = -1;
    while (id <= maxid)
    {
      found = 0;
      for (j=0; j<ncols; j++)
      {
        if (vgroup_ids[j] == id)
        {
          newid++;
          found = 1;
          break;
        }
      }
      if (found)
        for (j=0; j<ncols; j++)
          if (vgroup_ids[j] == id)
            vgroup_ids[j] = newid;
      id++;
    }
  }

  else
  {
    for (i=0; i<ncols; i++)
      vgroup_ids[i] = i;
  }
  return(1);
}

	  
