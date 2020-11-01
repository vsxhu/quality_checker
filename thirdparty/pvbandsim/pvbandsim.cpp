
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "kiss_fft.h"
#include "kiss_fftnd.h"
#include "kernels.h"
#include "layoutrw.h"
#include "pvbandsim.h"
#include "omp.h"

//below copied to pvbandsim.h
//int MAX_FILE_NAME_LENGTH = 80;
//float BACKGROUND_REAL = 0.0229;
//float BACKGROUND_IMAG = -0.0217;
//float MASK_REAL = 0.9771;
//float MASK_IMAG = 0.0217;
//float TARGET_INTENSITY = 0.225;
//float MAX_DOSE = 1.02;
//float MIN_DOSE = 0.98;

#include "write_png.h"

#define myMin(a,b) ((a)<=(b) ? (a) : (b))
#define myMax(a,b) ((a)<=(b) ? (b) : (a))

/*char *fftwf_wisdom[2] = {
//for macbook pro _cf2048x2048_cb2048x2048_ =
"(fftw-3.2.1 fftwf_wisdom\
(fftwf_dft_nop_register 0 #x1040 #x1040 #x0 #x7f6a6432 #x2d6843ab #x2527ce25 #x368a637c)\
(fftwf_rdft_rank0_register 4 #x1040 #x1040 #x0 #x80c09daa #x83ad25dd #x8d7a205b #xc5645b5f)\
(fftwf_codelet_t1_32 0 #x40 #x40 #x0 #xf7704b04 #x5997cad8 #x86de22f7 #xb4be321f)\
(fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #x2ab6aeb6 #xa3c5408d #x0ba6d5d5 #x2fcdec33)\
(fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x3c0aa804 #x09ff3a0e #x3970e0ad #x175c5ea9)\
(fftwf_codelet_n1_4 0 #x40 #x40 #x0 #xca3a4d79 #xa349ccf8 #x0877a3a5 #x62edc76f)\
(fftwf_dft_rank_geq2_register 0 #x1040 #x1040 #x0 #x6d0d3557 #x66c05dd2 #x08c72cca #x96573262)\
(fftwf_codelet_n1_4 0 #x40 #x40 #x0 #x94178bf7 #xf50166b7 #xa507ebc4 #x8495b5ca)\
(fftwf_codelet_t1_64 0 #x1040 #x1040 #x0 #x52c16d5c #x78672292 #xb152d1c2 #x86ccca81)\
(fftwf_dft_buffered_register 1 #x1040 #x1040 #x0 #xef76f989 #xbd4f4561 #x0aeeb443 #xfc90f8c1)\
(fftwf_dft_r2hc_register 0 #x1040 #x1040 #x0 #x4160825d #xd6712fce #x28e48976 #x429f2d9c)\
(fftwf_codelet_t1_64 0 #x1040 #x1040 #x0 #xeab40192 #xd605d0ea #x33acdc5e #xd87ce2ac)\
(fftwf_dft_r2hc_register 0 #x1040 #x1040 #x0 #x41aebf60 #x5f9a94de #xa1db3560 #xaaa35541)\
(fftwf_codelet_t1_16 0 #x40 #x40 #x0 #xe90e5bd7 #x9b96231a #x5ae521e6 #xaf8d9e76)\
(fftwf_codelet_t1_16 0 #x40 #x40 #x0 #xb8ffa816 #x392a7ef1 #xdcd466aa #x45c25d3d)\
(fftwf_codelet_t1_32 0 #x40 #x40 #x0 #x421c52a8 #x16494b63 #xd410b8a8 #x5bdfc4cc)\
(fftwf_dft_rank_geq2_register 0 #x1040 #x1040 #x0 #x499a8235 #x3dd05817 #x554cd266 #xb0cff2bb)\
(fftwf_dft_buffered_register 1 #x1040 #x1040 #x0 #xab1cc9c2 #xeb3c33aa #x71ef6175 #xa0246365)\
(fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x0d9ff864 #xf8defb37 #xfc370118 #x920983f7)\
(fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #xf621dcbf #x0cd08781 #xb1dba09f #xf9e76e38)\
(fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #x3fb8754a #xf5931640 #x74d8ad32 #x7d796fa3)\
(fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #xe4f64e90 #xf2c71347 #x96fbf0a7 #x8bb5233a)\
(fftwf_dft_nop_register 0 #x1040 #x1040 #x0 #x58c24a54 #x34caee9b #x6da274de #x686c1b30)\
(fftwf_codelet_n1_32 0 #x1040 #x1040 #x0 #xf15b330d #xbf11680d #xac2e6b40 #x74efe163)\
(fftwf_codelet_n1_32 0 #x1040 #x1040 #x0 #x68879c73 #x58ad2bd9 #x96b1e709 #xd0eba397)\
)",
// for arlx111 ... arlx118 _cf2048x2048_cb2048x2048_
"(fftw-3.2.1 fftwf_wisdom\
(fftwf_dft_r2hc_register 0 #x1040 #x1040 #x0 #x87d402be #x9edaf97e #x578bd92b #x9b94644d)\
(fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x2f525d92 #x354db20c #x5dc9c81c #xd5262385)\
(fftwf_dft_buffered_register 1 #x1040 #x1040 #x0 #x0551392c #xb8a7f449 #x18f646c0 #xf9608e38)\
(fftwf_codelet_t1_64 0 #x40 #x40 #x0 #x24a3cb08 #x596c2b8b #x72ff3abb #x0d42e591)\
(fftwf_dft_nop_register 0 #x1040 #x1040 #x0 #x7927936d #x770c8ef7 #x7cda0f74 #x0ef3ed58)\
(fftwf_dft_vrank_geq1_register 0 #x440 #x440 #x0 #xf80d33e6 #x7f21d9a0 #xf0163aa7 #x9336d2cf)\
(fftwf_rdft_rank0_register 2 #x40 #x40 #x0 #xfce92b26 #xd61bc939 #x513238d4 #x71cc55f3)\
(fftwf_codelet_t1_32 0 #x40 #x40 #x0 #x9b478eaf #x6b4eb1dd #x2181f1f4 #x5fd7ee9a)\
(fftwf_codelet_n1_32 0 #x1040 #x1040 #x0 #x04ff6da1 #x20ae4760 #xb5314542 #x6e1be611)\
(fftwf_dft_nop_register 0 #x1040 #x1040 #x0 #x9bc41ce1 #x0bcb22c5 #x9c236ad1 #xf034d07b)\
(fftwf_dft_rank_geq2_register 0 #x1040 #x1040 #x0 #x78d97a71 #xecc36c1f #xb05dd254 #x78217136)\
(fftwf_dft_r2hc_register 0 #x40 #x40 #x0 #xd683b9af #x6c47fd8b #x94b22112 #xcb95e9c6)\
(fftwf_dft_buffered_register 1 #x1040 #x1040 #x0 #x44868a42 #x169fd24f #x7ce2cf5b #x399a18f5)\
(fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x9abe6f40 #xa299582f #x7eb66309 #x128ce6c4)\
(fftwf_codelet_t1_64 0 #x1040 #x1040 #x0 #xa5ec3219 #xb6a587c3 #x59ea6ff1 #x344b8c2b)\
(fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #xfa8b7e37 #x32d92aed #xd9a1bd77 #x61fe06a9)\
(fftwf_dft_indirect_register 0 #x40 #x40 #x0 #xbaac3df1 #x08141485 #xeb919b5f #x06e839f3)\
(fftwf_codelet_n1_32 0 #x440 #x440 #x0 #x162d3b85 #xae08a7f3 #x370cad45 #xa2396241)\
(fftwf_dft_vrank_geq1_register 0 #x40 #x40 #x0 #x6a20f1a0 #x6c518403 #x9a36a6c9 #x06655191)\
(fftwf_codelet_n1_32 0 #x1040 #x1040 #x0 #x1ed069aa #x4cdf0b81 #xaba11f5d #x9e76b43f)\
(fftwf_rdft_rank0_register 4 #x1040 #x1040 #x0 #xb2990766 #xa2d3c102 #xce2cca98 #x96e02af4)\
(fftwf_codelet_t1_64 0 #x1040 #x1040 #x0 #x3182f4fb #x0a5541af #xd1943d43 #x87c40628)\
(fftwf_dft_r2hc_register 0 #x1040 #x1040 #x0 #x1ddea6cc #xe6e3df5f #x79956d3f #x21c77e34)\
(fftwf_dft_vrank_geq1_register 0 #x1040 #x1040 #x0 #x3d4161a6 #x33cd4303 #xcfe62f13 #xea549dc6)\
(fftwf_dft_rank_geq2_register 0 #x1040 #x1040 #x0 #x4ef51761 #x7671a8ae #x0344b1b0 #x75952958)\
(fftwf_codelet_n1_64 1 #x40 #x40 #x0 #xd0abe487 #x0ecd413e #xbd08ae7a #x78ddf448)\
)"
};				  */



void mySubBlock(unsigned char *bmsk, int nx, int ny, unsigned char *tiletemp, int tnx, int tny, int x, int y) {
  int i,  j;
  for(i=0; i<tnx; i++) {
    for(j=0; j<tny; j++) {
      if((y+j)<tny && (x+i)<tnx) {	tiletemp[j*tnx+i] = bmsk[(y+j)*ny+(x+i)]; }
      else { tiletemp[j*tnx+i] = 0; }
    }
  }
}

void mySubSample(unsigned char *tile, int tnx, int tny, int sc, unsigned char *out) {
  int i, j;
  int snx = tnx / sc;
  int sny = tny / sc;
  for(i=0; i<snx; i++) {
    for(j=0; j<sny; j++) {
      out[j*snx+i] = tile[j*sc*tnx + i*sc];
    }
  }
}

void myDevLayerDraw(unsigned char *imgpbo, int nx, int ny, int ox, int oy, int sc, rect *devdata, int lvlcnt, int c) {
  //TI_TN;
  int i;
  int x;
  int y;
  for( i = 0; i<lvlcnt; i++) {
    rect r = devdata[i];
    int x0 = (r.x / sc - ox);
    int y0 = (r.y / sc - oy);
    int x1 = x0 + r.w / sc;
    int y1 = y0 + r.h / sc;
    /*fprintf(stderr,"nx=%d, ny=%d\n",nx,ny);
      fprintf(stderr,"Read rectangle (%d,%d) to (%d,%d)\n",x0,y0,x1,y1);*/
    int p = (x1 < 0) || (x0 >= nx) || (y1 < 0) || (y0 >= ny);
    if (!p) {
      for( x = myMax(0,x0) ; x<myMin(nx-1,x1); x++) {
        for( y = myMax(0,y0); y<myMin(ny-1,y1); y++) {
          imgpbo[nx*y + x] = c;
        }
      }
    }
    /*else { fprintf(stderr,"Error: Rectangle (%d, %d) to (%d,%d) out of bounds!\n",x0,y0,x1,y1); exit(0); }*/
  }
}

int knlvl = 1;	// Only one simulation layer for now
agsKernel **krns;

float *d_scale;
float *d_kernels;

//int nk, nkx, nky;

int imx = 2048;
int imy = 2048;
int imxy = 2048 * 2048;

unsigned char *bmsk, *bimg;

kiss_fft_cpx *msk, *mskp, *mskpf, *mskf;
kiss_fft_cpx *km, *kmp, *imgpk, *imgk;
float *img;
//float *inner_img;
//float *outer_img;

kiss_fftnd_cfg fwdplan;
kiss_fftnd_cfg bwdplan;

float *d_work;
unsigned char  *d_tile;
/* unsigned char *d_bmsk; */

// Function to copy image arrays
void myCopyImage(float* src, float* dest, int nx, int ny)
{
  int i,j;
  for(j=0; j<ny; j++) {
    for(i=0; i<nx; i++) {
      dest[j*nx+i] = src[j*nx+i];
    }
  }
}

// Function to calculate the area of the PV band
int calculate_pvband_area(float* inner, float* outer, int sizex, int sizey)
{
  int pvb_area = 0;
  int i,j, index;
  for(j=0; j<sizey; j++)  {
    for(i=0; i<sizex; i++) {
      index = j*sizex + i;
      if(outer[index]>=TARGET_INTENSITY && inner[index]<TARGET_INTENSITY) pvb_area++;
    }
  }
  return pvb_area;
}

void myClearByte(unsigned char *msk) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i] = 0.0;
  }
}

void myClearFloat(float *msk) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i] = 0.0;
  }
}
void myClearComplex(kiss_fft_cpx *msk) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i].r = 0.0;
    msk[i].i = 0.0;
  }
}
void myMaskFloat(unsigned char *bmsk, kiss_fft_cpx *msk, float dose) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i].r = (((float)bmsk[i]) / 255.0 * MASK_REAL + BACKGROUND_REAL)*dose;   /* Enable these lines for Attenuated PSM */
    msk[i].i = (((float)bmsk[i]) / 255.0 * MASK_IMAG + BACKGROUND_IMAG)*dose;
    //msk[i][0] = ((float)bmsk[i]) / 255.0 * dose;    /* Enable these lines for binary masks */
    //msk[i][1] = 0.0;
  }
}

//void myShift_old(kiss_fft_cpx *in, kiss_fft_cpx *out) {
//  int i,j;
//  int x, y;
//  int imxh = imx/2;
//  int imyh = imy/2;
//  for (i=0; i<imx; i++) {
//    for(j=0; j<imy; j++) {
//      if(i<imxh) {
//        x = i+imxh;
//      } else {
//        x = i-imxh;
//      }
//      if(j<imyh) {
//        y = j+imyh;
//      } else {
//        y = j-imyh;
//      }
//      out[y*imx+x].r = in[j*imx+i].r;
//      out[y*imx+x].i = in[j*imx+i].i;
//    }
//  }
//}
void myShift(kiss_fft_cpx *in, kiss_fft_cpx *out) {
  int i,j;
  int x, y;
  int imxh = imx/2;
  int imyh = imy/2;

  for(j=0; j<imy; j++) {
    for (i=0; i<imx; i++) {
      if(i<imxh) {
        x = i+imxh;
      } else {
        x = i-imxh;
      }
      if(j<imyh) {
        y = j+imyh;
      } else {
        y = j-imyh;
      }
      out[y*imx+x].r = in[j*imx+i].r;
      out[y*imx+x].i = in[j*imx+i].i;
    }
  }
}

void mySumImg(kiss_fft_cpx *p, float *s, float scale) {
  int i;
  for(i=0; i<imxy; i++) {
    float pr = p[i].r;
    float pi = p[i].i;
    s[i] += scale * (pr*pr + pi*pi);
    /*if(i%100000==0) fprintf(stderr,"sum img %d scale %f real %f imag %f is %f\n", i, scale, pr, pi, s[i]);*/
  }
}
/* int pi = 0; */
/* #define P0 fprintf(stderr,"pi @ %d %d %d\n", pi=0, i, j); */
/* #define PI fprintf(stderr,"pi @ %d %d %d\n", pi++, i, j); */
#define P0
#define PI
void myKernelMult(int knx, int kny, float *krn, kiss_fft_cpx *mskf, kiss_fft_cpx *km ) {
  int i, j, k;
  k = 0;
  int imxh = imx/2;
  int imyh = imy/2;
  int xoff = imxh - knx/2 ;
  int yoff = imyh - kny/2 ;

  for(i=0; i<knx; i++) {
    for(j=0; j<kny; j++) {
      P0 float kr = krn[k++];
      PI float ki = krn[k++];
      //fprintf(stderr, "krn(%d,%d) = %f+%fJ\n", i,j,kr,ki);
      PI int ind = (yoff+j)*imx+(xoff+i);
      PI float mr = mskf[ind].r;
      PI float mi = mskf[ind].i;
      PI km[ind].r = (mr*kr-mi*ki);
      PI km[ind].i = (mi*kr+mr*ki);
      //fprintf(stderr, "km(%d,%d) = %f+%fJ\n", i,j,km[ind][0], km[ind][1]);
    }
  }
}

void myThreshhold(float *img, unsigned char *bimg) {
  int i,j;
  //  for (i=0; i<imxy; i++) bimg[i] = img[i] * 255;
  int cnt = 0;
  float max_intensity = 0.0;
  for(i=0; i<imx; i++) {
    for(j=0; j<imy; j++) {
      if( img[j*imx+i] > max_intensity) { max_intensity = img[j*imx+i]; }
      if ( img[j*imx+i] >= TARGET_INTENSITY)  {
        bimg[(j)*imx+(i)] =  255;
        cnt++;
      } else { bimg[(j)*imx+( i)] =  0; }
    }
  }
  fprintf(stderr, "%d Pixels, Max. Intensity = %f\n",cnt,max_intensity);
}

void myVmul(int n, float *v, float sc) {
  int i;
  for(i=0; i<n; i++) v[i] *= sc;
}

int is_in_tile(int workx,int worky,int x,int y)
{
  if(x>=workx && x<=workx+1024 && y>=worky && y<=worky+1024) return 1;
  return 0;
}

//void myComputeImage(agsKernel *krn, int workx, int worky, float dose) {
void myComputeImage(agsKernel *krn, int workx, int worky, float dose, int kernelLevel) {
  int k;
//  int nk = krn->nk;
  int nk = kernelLevel;

  //Image simulation
//  myMaskFloat(bmsk, msk, dose);
  myShift(msk, mskp);
  //fftwf_execute(fwdplan);
  kiss_fftnd(fwdplan, mskp, mskpf);
  myVmul(2*imxy, (float*)mskpf, 1.0 / ((float)imxy));
  myShift(mskpf, mskf);
  myClearFloat(img);
  myClearComplex(km);

  for(k=0; k<nk; k++) {

    /* Convolve with each kernel */
    fprintf(stderr, "Now convolving with kernel %d weight %f\n", k, krn->scale[k]);
    myKernelMult(krn->nkx, krn->nky, krn->krn + 2*k*krn->nkx*krn->nky, mskf, km);
    myShift(km, kmp);
    //fftwf_execute(bwdplan);
    kiss_fftnd(bwdplan, kmp, imgpk);
    myShift(imgpk, imgk);
    mySumImg(imgk, img, krn->scale[k]);

  }

//  myThreshhold(img, bimg);
  return;
}

#if 0
int oldmain(int argc, char	 **argv) {
  int i,j;
  int dims[] = {2048, 2048};
  int ndims = 2;
  int isinverse = 1;
  char INFILE[80];
  char OUTFILE[80];

  // Read in input and output file names
  if(argc<2) {
    fprintf(stderr,"*Error*: Must specify input GULP file name\n");
    exit(1);
  } else {
    strcpy(INFILE, argv[1]);
    if(argc<3) { fprintf(stderr,"*Error*: Must specify output file name\n"); exit(1); }
    strcpy(OUTFILE, argv[2]);
  }

  bmsk	= (unsigned char*)malloc(2048 * 2048);
  bimg	= (unsigned char*)malloc(2048 * 2048);

  msk	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  mskp	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  mskpf = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  mskf	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );

  km	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  kmp	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  imgpk = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  imgk	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );

  img	= (float*)malloc(2048 * 2048 * sizeof(float));
  outer_img	= (float*)malloc(2048 * 2048 * sizeof(float));
  inner_img	= (float*)malloc(2048 * 2048 * sizeof(float));

  memset(msk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(mskp, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(mskpf, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(mskf, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));

  memset(km, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(kmp, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(imgpk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(imgk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));


  fwdplan = kiss_fftnd_alloc (dims, ndims, 0, 0, 0);//(2048, 2048, mskp, mskpf, -1, FFTW_WISDOM_ONLY);
  bwdplan = kiss_fftnd_alloc (dims, ndims, isinverse, 0, 0); //fftwf_plan_dft_2d(2048, 2048, kmp, imgpk,	1, FFTW_WISDOM_ONLY);

  int workx[100];	// No more than 100 tiles. Start with 1
  int worky[100];
  //	int workcnt = 0;

  int lyr = 0;   // For single layer, it is always 0
  int tnx = 2048;
  int tny = 2048;
  int sc = 1;	// What does this variable do?

  int size;
  int scsize;

  //	int num_target_shapes;
  //	float dose = 1.0;

  unsigned char *tile = NULL;
  unsigned char *tiletemp = NULL;

  //case CMD_DESIGN_LOAD:
  // Parse the input file and assign layer name
  clear_design_space();
  read_layout(INFILE);
  char layername[] = "M1OPC";


  //case CMD_KERNEL_LOAD:
  krns = (agsKernel**)malloc(knlvl * sizeof(agsKernel *));
  for(i=0; i<knlvl; i++) {
    krns[i] = agsReadKernel(layername,0);	//0 => no defocus
    nk = krns[i]->nk;
    nkx = krns[i]->nkx;
    nky = krns[i]->nky;
  }
  fprintf(stderr, "*Message*: received kernel data\n");

  // Process the tile with litho simultion
  fprintf(stderr, "*Message*: Running simulation on tile\n");
  size = tnx * tny;
  scsize = size / (sc * sc);

  if (tile == NULL) {
    tile = (unsigned char*)malloc(size*sizeof(unsigned char));
    tiletemp = (unsigned char*)malloc(size*sizeof(unsigned char));
  }
  // this is real assuming 1024x1024 tile is being asked for (i know! i know! i'll fix it later)
  workx[0] = 0;	// Let's start with just one tile
  worky[0] = 0;
  myClearByte(bmsk);
  myDevLayerDraw(bmsk, imx, imy, workx[0]-512, worky[0]-512, 1, lvldata[lyr], lvlcnt[lyr], 255);

  // ********* Write the mask output to a PNG file ************//
  char maskfile[80]; sprintf(maskfile,"Debug/mask_tile%d.png",i);
  write_mask_file(bmsk, maskfile, 2048, 2048);
  // *********************************************************//

  // Run outer corner simulation
  myComputeImage(krns[lyr], workx[0], worky[0], MAX_DOSE );
  mySubBlock(bimg, imx, imy, tiletemp, tnx, tny, 512, 512);
  mySubSample(tiletemp, tnx, tny, sc, tile);
  myCopyImage(img, outer_img, tnx, tny);

  // ********* Write the Outer image to a PNG file ************//
  //char outerimfile[80]; sprintf(outerimfile,"Debug/outer_image_tile%d.png",i);
  //write_image_file(img, outerimfile, 2048, 2048, MAX_DOSE);
  // ********************************************************* //
  fprintf(stderr,"*Message*: Done computing outer image\n");

  //Load kernels for defocus image
  free(krns);
  krns = (agsKernel**)malloc(knlvl * sizeof(agsKernel *));
  for(i=0; i<knlvl; i++) {
    krns[i] = agsReadKernel(layername,1);	//1=>defocus
    nk = krns[i]->nk;
    nkx = krns[i]->nkx;
    nky = krns[i]->nky;
  }
  fprintf(stderr, "*Message*: received defocus kernel data\n");

  // Run inner corner simulation
  myComputeImage(krns[lyr], workx[0], worky[0], MIN_DOSE );
  mySubBlock(bimg, imx, imy, tiletemp, tnx, tny, 512, 512);
  mySubSample(tiletemp, tnx, tny, sc, tile);
  myCopyImage(img, inner_img, tnx, tny);
  fprintf(stderr,"*Message*: Done computing inner image\n");

  // ********* Write the PV Band to a PNG file ************//
  char imagefile[80]; sprintf(imagefile,"Debug/pvband_image_tile%d.png",i);
  write_pvband(inner_img, outer_img, imagefile, 2048, 2048);
  // ********************************************************* //

  // Dump PV Band area to output file
  int pvband_area = calculate_pvband_area(inner_img, outer_img, tnx, tny);
  FILE* fout = fopen(OUTFILE,"w");
  fprintf(fout,"PV Band Area = %d nm^2\n",pvband_area);
  fclose(fout);

  //Clean up
  for(i=0; i<knlvl; i++)  free(krns[i]);
  for(j=0; j<nlvl; j++)  { free(lvldata[j]); lvlcnt[j]=0; }
  fprintf(stderr,"*Message*: Removed kernel and design data\n");
  exit(0);
}
#endif

//below added============================
namespace pvbandsim
{
unsigned char *tile = NULL;
unsigned char *tiletemp = NULL;
agsKernel **optKernels[4];  //focus, defocus, CT focus, CT defocus
agsKernel **comboOptKernels[6]; //focus, defocus, CT focus, CT defocus, precompute
//agsKernel **krnsFocus;
//agsKernel **krnsDefocus;

int lyr = 0;   // For single layer, it is always 0
int tnx = 2048;
int tny = 2048;
int sc = 1;	// What does this variable do?

//generate conjugate transpose of kernels
void
initialCTKernel()
{
  agsKernel *kernel, *kernelCT;
  float *kernelHead, *kernelCTHead;
  int nk, nx, ny;
  int type, layer, i, j, k;
  int index, indexCT;

  for (type = 0; type < 2; ++type)  //focus, defocus
    for (layer = 0; layer < knlvl; ++layer)
    {
      kernel = optKernels[type][layer];
      //copy to new kernel
      kernelCT = (agsKernel*)malloc(sizeof(agsKernel));
      kernelCT->nk = kernel->nk;
      kernelCT->nkx = kernel->nky; //transpose
      kernelCT->nky = kernel->nkx; //transpose
      kernelCT->scale = (float*)malloc(kernelCT->nk * sizeof(float));
      memcpy(kernelCT->scale, kernel->scale, kernel->nk * sizeof(float));

      //get conjugate transpose
      nk = kernelCT->nk;
      nx = kernelCT->nkx;
      ny = kernelCT->nky;
      kernelCT->krn = (float*)malloc(nk * nx * ny * 2 * sizeof(float));
      kernelHead = kernel->krn;
      kernelCTHead = kernelCT->krn;
      for (k = 0; k < nk; ++k)
      {
        for (i = 0; i < nx; ++i)
          for (j = 0; j < ny; ++j)
          {
            indexCT = (j * nx + i) * 2; //i,j entry
            index = (i * ny + j) * 2; //j,i entry
            kernelCTHead[indexCT] = kernelHead[index];  //r
            kernelCTHead[indexCT + 1] = -kernelHead[index + 1]; //i, conjugate
          }
        kernelHead += (nx * ny * 2);
        kernelCTHead += (nx * ny * 2);
      }

      optKernels[type + 2][layer] = kernelCT;
    }
  //========================= print
//  char fnr[10], fni[10];
//  for (k = 0; k < 2; ++k)
//  {
//    sprintf(fnr, "kernel%dr", k);
//    sprintf(fni, "kernel%di", k);
//    FILE* filekr = fopen(fnr, "w");
//    FILE* fileki = fopen(fni, "w");
//    kernel = optKernels[k][0];
//    printf("output kernel %d scale %f\n", k, kernel->scale[0]);
//    for (j = 0; j < kernel->nky; ++j)
//    {
//      for (i = 0; i < kernel->nkx; ++i)
//      {
//        index = (j*kernel->nkx+i)*2;
//        fprintf(filekr, "%f ", kernel->krn[index]);
//        fprintf(fileki, "%f ", kernel->krn[index + 1]);
//      }
//      fprintf(filekr, "\n");
//      fprintf(fileki, "\n");
//    }
//    fclose(filekr);
//    fclose(fileki);
//  }

//  char fnr[10], fni[10];
//  for (k = 0; k < 2; ++k)
//  {
//    sprintf(fnr, "kernel%dr", k);
//    sprintf(fni, "kernel%di", k);
//    FILE* filekr = fopen(fnr, "w");
//    FILE* fileki = fopen(fni, "w");
//    kernel = optKernels[0][0];
//    for (j = 0; j < kernel->nky; ++j)
//    {
//      for (i = 0; i < kernel->nkx; ++i)
//      {
//        index = kernel->nkx*kernel->nky*2*k + (j*kernel->nkx+i)*2;
//        fprintf(filekr, "%f ", kernel->krn[index]);
//        fprintf(fileki, "%f ", kernel->krn[index + 1]);
//      }
//      fprintf(filekr, "\n");
//      fprintf(fileki, "\n");
//    }
//    fclose(filekr);
//    fclose(fileki);
//  }

//  FILE* filer = fopen("kernelctr", "w");
//  FILE* filei = fopen("kernelcti", "w");
//  for (k = 0; k < 1; ++k)
//  {
//    kernel = optKernels[2][0];
//    for (j = 0; j < kernel->nky; ++j)
//    {
//      for (i = 0; i < kernel->nkx; ++i)
//      {
//        index = (j*kernel->nkx+i)*2;
//        fprintf(filer, "%f ", kernel->krn[index]);
//        fprintf(filei, "%f ", kernel->krn[index + 1]);
//      }
//      fprintf(filer, "\n");
//      fprintf(filei, "\n");
//    }
//  }
//  fclose(filer);
//  fclose(filei);
  //=========================
}

void
initialComboKernel()
{
  agsKernel *kernel, *kernelCombo, *kernelComboCT;
  float *kernelHead, *kernelComboHead, *kernelComboHeadCT;
  int nk, nx, ny;
  int type, layer, i, j, k;
  int index, indexCT;
  float scale;

  for (i = 0; i < 4; ++i)
    comboOptKernels[i] = (agsKernel**)malloc(knlvl * sizeof(agsKernel *));

  for (type = 0; type < 2; ++type)  //focus, defocus
    for (layer = 0; layer < knlvl; ++layer)
    {
      kernel = optKernels[type][layer];
      //accomulate all levels to one combo kernel
      kernelCombo = (agsKernel*)malloc(sizeof(agsKernel));
      kernelCombo->nk = 1;
      kernelCombo->nkx = kernel->nkx;
      kernelCombo->nky = kernel->nky;
      kernelCombo->scale = (float*)malloc(kernelCombo->nk * sizeof(float));
      kernelCombo->scale[0] = 1;

      //nk = kernel->nk;
      nk = 9;
      nx = kernelCombo->nkx;
      ny = kernelCombo->nky;
      kernelCombo->krn = (float*)malloc(nx * ny * 2 * sizeof(float));
      memset(kernelCombo->krn, 0, nx * ny * 2 * sizeof(float));
      kernelHead = kernel->krn;
      kernelComboHead = kernelCombo->krn;
      //kernelHead += (nx * ny * 2);//!!!!!!!!!!!!
      for (k = 0; k < nk; ++k)
      {
        //scale = kernel->scale[k];
        scale = sqrt(kernel->scale[k]);
        //printf("%f\n", scale);
        for (j = 0; j < ny; ++j)
          for (i = 0; i < nx; ++i)
          {
            index = (j * nx + i) * 2;
            kernelComboHead[index] += (scale * kernelHead[index]);  //r
            kernelComboHead[index + 1] += (scale * kernelHead[index + 1]);  //i
          }
        kernelHead += (nx * ny * 2);
      }
      comboOptKernels[type][layer] = kernelCombo;

      //get conjugate transpose
      kernelComboCT = (agsKernel*)malloc(sizeof(agsKernel));
      kernelComboCT->nk = 1;
      kernelComboCT->nkx = kernel->nky; //transpose
      kernelComboCT->nky = kernel->nkx; //transpose
      kernelComboCT->scale = (float*)malloc(kernelComboCT->nk * sizeof(float));
      kernelComboCT->scale[0] = 1;

      kernelComboCT->krn = (float*)malloc(nx * ny * 2 * sizeof(float));
      memset(kernelComboCT->krn, 0, nx * ny * 2 * sizeof(float));
      kernelComboHeadCT = kernelComboCT->krn;
      for (j = 0; j < ny; ++j)
        for (i = 0; i < nx; ++i)
        {
          index = (j * nx + i) * 2; //i,j entry
          indexCT = (i * ny + j) * 2; //j,i entry
          kernelComboHeadCT[indexCT] = kernelComboHead[index];  //r
          kernelComboHeadCT[indexCT + 1] = -kernelComboHead[index + 1];  //conj i
        }
      comboOptKernels[type + 2][layer] = kernelComboCT;
    }
  printf("%d Combo kernel loaded\n", nk);
  //print
//    FILE* filekr = fopen("combokr", "w");
//    FILE* fileki = fopen("comboki", "w");
//    kernel = comboOptKernels[0][0];
//    for (j = 0; j < kernel->nky; ++j)
//    {
//      for (i = 0; i < kernel->nkx; ++i)
//      {
//        index = (j*kernel->nkx+i)*2;
//        fprintf(filekr, "%f ", kernel->krn[index]);
//        fprintf(fileki, "%f ", kernel->krn[index + 1]);
//      }
//      fprintf(filekr, "\n");
//      fprintf(fileki, "\n");
//    }
//    fclose(filekr);
//    fclose(fileki);
}

//precompute the convolution of H * conj_transpose(H)
void
preComputeComboConv()
{
  agsKernel *kernel, *kernelCT, *kernelConv;
  float *kernelHead, *kernelCTHead, *kernelConvHead;
  int nx, ny;
  int type, layer, i, j;
  int index;

  for (i = 4; i < 6; ++i) //focus,defocus
    comboOptKernels[i] = (agsKernel**)malloc(knlvl * sizeof(agsKernel *));

  for (type = 0; type < 2; ++type)  //focus, defocus
    for (layer = 0; layer < knlvl; ++layer)
    {
      kernel = optKernels[type][layer];
      kernelCT = optKernels[type + 2][layer]; //conj transpose
      //accomulate all levels to one combo kernel
      kernelConv = (agsKernel*)malloc(sizeof(agsKernel));
      kernelConv->nk = 1;
      kernelConv->nkx = kernel->nkx;
      kernelConv->nky = kernel->nky;
      kernelConv->scale = (float*)malloc(kernelConv->nk * sizeof(float));
      kernelConv->scale[0] = 1;
      kernelConv->krn = (float*)malloc(nx * ny * 2 * sizeof(float));

      nx = kernelConv->nkx;
      ny = kernelConv->nky;
      kernelHead = kernel->krn;
      kernelCTHead = kernelCT->krn;
      kernelConvHead = kernelConv->krn;
      for (j = 0; j < ny; ++j)
        for (i = 0; i < nx; ++i)
        {
          index = (j * nx + i) * 2;
          //complex multiplication
          kernelConvHead[index] = kernelHead[index] * kernelCTHead[index]
            - kernelHead[index + 1] * kernelCTHead[index + 1];  //r
          kernelConvHead[index + 1] = kernelHead[index + 1] * kernelCTHead[index]
            + kernelHead[index] * kernelCTHead[index + 1];  //i
        }
      comboOptKernels[type + 4][layer] = kernelConv;  //precomput HH*
    }
//  FILE* filekr = fopen("combokr", "w");
//  FILE* fileki = fopen("comboki", "w");
//  kernel = comboOptKernels[4][0];
//  for (j = 0; j < kernel->nky; ++j)
//  {
//    for (i = 0; i < kernel->nkx; ++i)
//    {
//      index = (j*kernel->nkx+i)*2;
//      fprintf(filekr, "%f ", kernel->krn[index]);
//      fprintf(fileki, "%f ", kernel->krn[index + 1]);
//    }
//    fprintf(filekr, "\n");
//    fprintf(fileki, "\n");
//  }
//  fclose(filekr);
//  fclose(fileki);
//  exit(0);
  printf("Done precompute kernel convolution\n");
}


void
initSimulate()
{
  int i;
  int dims[] = {2048, 2048};
  int ndims = 2;
  int isinverse = 1;

//  bmsk	= (unsigned char*)malloc(2048 * 2048);
  bimg	= (unsigned char*)malloc(2048 * 2048);

  msk	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  mskp	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  mskpf = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  mskf	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );

  km	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  kmp	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  imgpk = (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );
  imgk	= (kiss_fft_cpx*) malloc( dims[0] * dims[1] * sizeof(kiss_fft_cpx) );

  img	= (float*)malloc(2048 * 2048 * sizeof(float));

  memset(msk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(mskp, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(mskpf, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(mskf, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));

  memset(km, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(kmp, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(imgpk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));
  memset(imgk, 0, dims[0] * dims[1] * sizeof(kiss_fft_cpx));

  for (i = 0; i < 4; ++i)
    optKernels[i] = (agsKernel**)malloc(knlvl * sizeof(agsKernel *));

  fwdplan = kiss_fftnd_alloc (dims, ndims, 0, 0, 0);//(2048, 2048, mskp, mskpf, -1, FFTW_WISDOM_ONLY);
  bwdplan = kiss_fftnd_alloc (dims, ndims, isinverse, 0, 0); //fftwf_plan_dft_2d(2048, 2048, kmp, imgpk,	1, FFTW_WISDOM_ONLY);

//  int workx[100];	// No more than 100 tiles. Start with 1
//  int worky[100];

//  int lyr = 0;   // For single layer, it is always 0
  int tnx = 2048;
  int tny = 2048;
  int sc = 1;	// What does this variable do?

  int size;
  int scsize;

//  //case CMD_DESIGN_LOAD:
//  // Parse the input file and assign layer name
//  clear_design_space();
//  read_layout(INFILE);
  char layername[] = "M1OPC";

  agsKernel** kernel;
  //Load kernels for focus
  kernel = optKernels[0];
  for(i=0; i<knlvl; i++) {
    kernel[i] = agsReadKernel(layername,0);	//0 => no defocus
//    nk = kernel[i]->nk;
//    nkx = kernel[i]->nkx;
//    nky = kernel[i]->nky;
  }
  fprintf(stderr, "*Message*: received focus kernel data\n");

  //Load kernels for defocus
  kernel = optKernels[1];
  for(i=0; i<knlvl; i++) {
    kernel[i] = agsReadKernel(layername,1);	//1=>defocus
//    nk = kernel[i]->nk;
//    nkx = kernel[i]->nkx;
//    nky = kernel[i]->nky;
  }
  fprintf(stderr, "*Message*: received defocus kernel data\n");

  // Process the tile with litho simultion
  size = tnx * tny;
  scsize = size / (sc * sc);

  if (tile == NULL) {
    tile = (unsigned char*)malloc(size*sizeof(unsigned char));
    tiletemp = (unsigned char*)malloc(size*sizeof(unsigned char));
  }
  // this is real assuming 1024x1024 tile is being asked for (i know! i know! i'll fix it later)
//  workx[0] = 0;	// Let's start with just one tile
//  worky[0] = 0;
//  myClearByte(bmsk);
//  myDevLayerDraw(bmsk, imx, imy, workx[0]-512, worky[0]-512, 1, lvldata[lyr], lvlcnt[lyr], 255);

  initialCTKernel();
  initialComboKernel();
  //preComputeComboConv();
}

void
endSimulate()
{
  //Clean up
  int type, i;
  for (type = 0; type < 4; ++type)
    for(i = 0; i < knlvl; ++i)
      free(optKernels[type][i]);
  //for (type = 0; type < 6; ++type)
  for (type = 0; type < 4; ++type)
    free(comboOptKernels[type][0]);
  fprintf(stderr,"*Message*: Removed kernel and design data\n");
}

void myMaskFloat2(float *mask, kiss_fft_cpx *msk, float dose) {
  int i;
  for(i=0; i<imxy; i++) {
    msk[i].r = (mask[i] * MASK_REAL + BACKGROUND_REAL)*dose;   /* Enable these lines for Attenuated PSM */
    msk[i].i = (mask[i] * MASK_IMAG + BACKGROUND_IMAG)*dose;
    //msk[i][0] = (mask[i] / 255.0 * dose;    /* Enable these lines for binary masks */
    //msk[i][1] = 0.0;
  }
}

//result stored in maskf
kiss_fft_cpx*
maskfft(float *mask, float dose)
{
  myMaskFloat2(mask, msk, dose);
  myShift(msk, mskp);
  //fftwf_execute(fwdplan);
  kiss_fftnd(fwdplan, mskp, mskpf);
  myVmul(2*imxy, (float*)mskpf, 1.0 / ((float)imxy));
  myShift(mskpf, mskf);
  return mskf;
}

kiss_fft_cpx*
cpxfft(kiss_fft_cpx *matrix, float dose)
{
  myShift(matrix, mskp);
//  for (int i = 0; i < imxy; ++i)  //include dose effect
//  {
//    mskp[i].r *= dose;
//    mskp[i].i *= dose;
//  }
  //fftwf_execute(fwdplan);
  kiss_fftnd(fwdplan, mskp, mskpf);
  myVmul(2*imxy, (float*)mskpf, 1.0 / ((float)imxy));
  myShift(mskpf, mskf);
  return mskf;
}

//convolv kernel, but not transform to binary image
void
convolveKernelCpx(kiss_fft_cpx *matrix, kiss_fft_cpx *result,
    int kernelIdx, int kernelLevel)
{
#define COMBO_VERSION
#ifdef COMBO_VERSION
  agsKernel* kernel = comboOptKernels[kernelIdx][lyr];
  //kernel = comboOptKernels[kernelIdx][lyr];
  myClearComplex(km);
  fprintf(stderr, "Now convolving with kernel %d weight %f\n", 0, kernel->scale[0]);
  myKernelMult(kernel->nkx, kernel->nky, kernel->krn, matrix, km);
  myShift(km, kmp);
  kiss_fftnd(bwdplan, kmp, imgpk);
  myShift(imgpk, result);
  //print
//  FILE* file2 = fopen("matrix2", "w");
//  for(int j=0; j<2048; j++) {
//    for(int i=0; i<2048; i++)
//      fprintf(file2, "%f ", result[j*2048+i].r);
//    fprintf(file2,"\n");
//  }
//  fclose(file2);
#else //original version
  agsKernel* kernel = optKernels[kernelIdx][lyr];

  int i, k;
  int nk = kernelLevel;
  float scale;

  myClearComplex(result);
  myClearComplex(km);
  for(k=0; k<nk; k++) {
    /* Convolve with each kernel */
    fprintf(stderr, "Now convolving with kernel %d weight %f\n", k, kernel->scale[k]);
    myKernelMult(kernel->nkx, kernel->nky, kernel->krn + 2*k*kernel->nkx*kernel->nky, matrix, km);
    myShift(km, kmp);
    //fftwf_execute(bwdplan);
    kiss_fftnd(bwdplan, kmp, imgpk);
    //myVmul(2*imxy, (float*)imgpk, ((float)imxy));   //bug in ori?????!!!!!!!
    myShift(imgpk, imgk);
    //mySumImg(imgk, img, kernel->scale[k]);
    scale = kernel->scale[k];
    for(i=0; i<imxy; i++) {
      result[i].r += scale * imgk[i].r;
      result[i].i += scale * imgk[i].i;
    }
  }
//  //print
//  FILE* file1 = fopen("matrix1", "w");
//  for(int j=0; j<2048; j++) {
//    for(int i=0; i<2048; i++)
//      fprintf(file1, "%f ", result[j*2048+i].r);
//    fprintf(file1,"\n");
//  }
//  fclose(file1);
#endif
//  fprintf(stderr,"*Message*: Done convolving image\n");
}

//void
//convolveKernel(unsigned char *mask, kiss_fft_cpx *result, int kernelIdx, float dose)
//{
//  maskfft(mask, dose);  //result in mskf
//  convolveKernelCpx(mskf, result, kernelIdx);
//}

//binary version
// Function to calculate the area of the PV band
//int calculate_pvband_area2(unsigned char* inner, unsigned char* outer, int sizex, int sizey)
//{
//  int pvb_area = 0;
//  int i,j, index;
//  for(j=0; j<sizey; j++)  {
//    for(i=0; i<sizex; i++) {
//      index = j*sizex + i;
//      if(outer[index] != inner[index]) pvb_area++;
//    }
//  }
//  return pvb_area;
//}

//mask is simulated and store the result in image
//isDefocus 1 => defocus, 0 => no defocus
//tileX, tileY: the lowerleft coordinate of current processing tile
//void
//simulateImage(unsigned char *mask, float *image, int kernelIdx, float dose)
//{
//  agsKernel** kernels = optKernels[kernelIdx];
//  bmsk = mask;
//  img = image;
//
//  myComputeImage(kernels[lyr], 0, 0, dose );
////  myThreshhold(img, image);
////  mySubBlock(image, imx, imy, tiletemp, tnx, tny, 512, 512); //needed???
////  mySubSample(tiletemp, tnx, tny, sc, tile);                //needed???
//  fprintf(stderr,"*Message*: Done computing image\n");
//}

//only do optical simulation, no photorisist threshold
void
simulateImageOpt(float *mask, float *image, int kernelIdx, float dose,
    int kernelLevel)
{
  agsKernel** kernels = optKernels[kernelIdx];
  img = image;

  myMaskFloat2(mask, msk, dose);
  myComputeImage(kernels[lyr], 0, 0, dose, kernelLevel);
//  mySubBlock(bimg, imx, imy, tiletemp, tnx, tny, 512, 512); //needed???
//  mySubSample(tiletemp, tnx, tny, sc, tile);                //needed???
  fprintf(stderr,"*Message*: Done computing image\n");

//  FILE* file = fopen("matrix1", "w");
//  for(int j=0; j<2048; j++) {
//    for(int i=0; i<2048; i++)
//      fprintf(file, "%f ", image[j*2048+i]);
//    fprintf(file,"\n");
//  }
//  fclose(file);
}

//binary version
void
simulateImageB(float *mask, unsigned char *image, int kernelIdx, float dose,
    int kernelLevel)
{
  simulateImageOpt(mask, img, kernelIdx, dose, kernelLevel);
  myThreshhold(img, image);
}

////not work: square aboslute value cannot be simplified
//void
//simulateImageCombo(float *mask, float *image, int kernelIdx, float dose)
//{
//  maskfft(mask, dose);  //result in mskf
//  convolveKernelCpx(mskf, imgk, kernelIdx, comboOptKernels[kernelIdx][lyr]->nk);
//  int i;
//  for (i = 0; i < imxy; ++i)
//    image[i] = imgk[i].r * imgk[i].r + imgk[i].i * imgk[i].i;
//
////  FILE* file = fopen("matrix2", "w");
////  for(int j=0; j<2048; j++) {
////    for(int i=0; i<2048; i++)
////      fprintf(file, "%f ", image[j*2048+i]);
////    fprintf(file,"\n");
////  }
////  fclose(file);
////  exit(0);
//}

int
getPvband(float *inner, float *outer)
{
  int pvband_area = calculate_pvband_area(inner, outer, tnx, tny);
  return pvband_area; //unit is nm^2
}

void
writePvband(char* fileName, float* inner, float* outer)
{
//  char imagefile[80];
//  sprintf(imagefile,"Debug/pvband_image_tile1.png");
  write_pvband(inner, outer, fileName, tnx, tny);
}

//pass refernce to kernel: focus, focus_ct, defocus, defocus_ct
//void
//getComboKernels(agsKernel** kernels)
//{
//  for (int kernelIdx = 0; kernelIdx < 4; ++ kernelIdx)
//    kernels[kernelIdx] = comboOptKernels[kernelidx][lyr];
//}

//void
//computeImage(agsKernel *krn, int workx, int worky, float dose) {
//  int k;
//  int nk = krn->nk;
//
//  //Image simulation
//  myMaskFloat(bmsk, msk, dose); //binary to complex
//  myShift(msk, mskp); //replace ll,ur and ul,lr
//  //fftwf_execute(fwdplan);
//  kiss_fftnd(fwdplan, mskp, mskpf);
//  myVmul(2*imxy, (float*)mskpf, 1.0 / ((float)imxy));
//  myShift(mskpf, mskf);
//  myClearFloat(img);
//  myClearComplex(km);
//
//  for(k=0; k<nk; k++) {
//
//    /* Convolve with each kernel */
//    fprintf(stderr, "Now convolving with kernel %d weight %f\n", k, krn->scale[k]);
//    myKernelMult(krn->nkx, krn->nky, krn->krn + 2*k*krn->nkx*krn->nky, mskf, km);
//    myShift(km, kmp);
//    //fftwf_execute(bwdplan);
//    kiss_fftnd(bwdplan, kmp, imgpk);
//    myShift(imgpk, imgk);
//    mySumImg(imgk, img, krn->scale[k]);
//
//  }
//}

} //namespace

