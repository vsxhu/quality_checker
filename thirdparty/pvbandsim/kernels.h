#include <stdlib.h>

#ifndef __AGSKRN_H__
#define __AGSKRN_H__

typedef struct agsKernel {
  int nk;
  int nkx;
  int nky;
  float *scale;
  //krn i's (i=0..nk-1) start at krn[i*nkx*nky]
  float *krn;
} agsKernel;

#endif

void swap4(void *x) {
  unsigned char t;
  unsigned char *y = (unsigned char*)x;
  t = y[0];
  y[0] = y[3];
  y[3] = t;
  t = y[1];
  y[1] = y[2];
  y[2] = t;
}

agsKernel *agsReadKernel(char *lyr, int flag_defocus) {

  char fname[1024];
  int i;

  if(!flag_defocus) {
    sprintf(fname, "Kernels/%s/scales.txt", lyr);
  } else {
    sprintf(fname, "Kernels/%s_def/scales.txt", lyr);
  }
  FILE *F;

  F = fopen(fname, "r");

  if (F==NULL) {
    fprintf(stderr, "Kernel %s not found\n", fname);
    return NULL;
  }

  agsKernel *k = (agsKernel*)malloc(sizeof(agsKernel));
  fscanf(F, "%d\n", &(k->nk));

  k->scale = (float*)malloc(k->nk * sizeof(float));
  float d;

  for(i = 0; i<k->nk; i++) {
    fscanf(F, "%f", &d);
    k->scale[i] = d;
    fprintf(stderr,"%s(scale %d) = %f\n", lyr, i, k->scale[i]);
  }
  fclose (F);

  int idata[5];

  for(i=0; i<k->nk; i++) {
    if(!flag_defocus) { sprintf(fname, "Kernels/%s/fh%d.bin", lyr, i); }
    else { sprintf(fname, "Kernels/%s_def/fh%d.bin", lyr, i); }
    F = fopen(fname, "r");
    fread(idata, 5, sizeof(int), F);
    swap4(idata);
    swap4(idata+1);
    swap4(idata+2);
    swap4(idata+3);
    swap4(idata+4);
    fprintf(stderr, "%s (%d %d %d %d %d)\n", fname, idata[0], idata[1], idata[2], idata[3], idata[4]);
    if (idata[2] != 2) fprintf(stderr, "Should be type 2 kernel not %d\n", idata[2]);
    if (idata[3] != i) fprintf(stderr, "Kernel %d should be %d in file also\n", i, idata[3]);
    if (i==0) {
      k->nkx = idata[0];
      k->nky = idata[1];
      k->krn = (float*)malloc(k->nk * k->nkx * k->nky * 2 * sizeof(float));
    } else {
      if(idata[0] != k->nkx || idata[1] != k->nky)
        fprintf(stderr, "Kernel size mismatch in kernel %d (%d,%d) is not (%d %d)\n", i, idata[0], idata[1], k->nkx, k->nky);
    }
    fread(k->krn + i * k->nkx * k->nky * 2, k->nkx * k->nky * 2, sizeof(float), F);
    fclose(F);
  }

  //fprintf(stderr, "Layer %s - kernel data = %d floats\n", k->nk * k->nkx * k->nky * 2);
  for(i = 0; i< k->nk * k->nkx * k->nky * 2; i++) {
    swap4(k->krn+i);
    //fprintf(stderr, "%f ", k->krn[i]);
  }
  //fprintf(stderr,"\n");

  return k;
}



