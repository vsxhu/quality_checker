#ifndef _WRITE_PNG_H_
#define _WRITE_PNG_H_
#include <stdio.h>
//#include "png.h"
#include "lodepng.h"

typedef struct {
  unsigned char r;
  unsigned char g;
  unsigned char b;
  unsigned char a;
} ucharac4;

/* This function writes the mask as a PNG file */
void write_mask_file(unsigned char* wmsk, char* fname, int imx, int imy)
{
  ucharac4 *pngmask;
  pngmask = (ucharac4*)malloc(imx * imy * sizeof(ucharac4));
  int ii, jj, mskval;
  for(jj=0; jj<imy; jj++) {
    for(ii=0; ii<imx; ii++) {
      mskval = (int)wmsk[imx*jj + ii]/255;
      pngmask[imx*jj + ii].g = 0*mskval;
      pngmask[imx*jj + ii].r = 255*mskval;
      pngmask[imx*jj + ii].b = 255*mskval;
      pngmask[imx*jj + ii].a = 255;
    }
  }
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(fname, (const unsigned char*)pngmask, imx, imy);

  /*if there's an error, display it*/
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  free(pngmask);
}

void write_image_file(float* image, char* fname, int sizex, int sizey, float dose)
{
  /* This section dumps out the image */
  ucharac4 *pngimg;
  pngimg = (ucharac4*)malloc(sizex * sizey * sizeof(ucharac4));
  float imgfloat;
  int imgval, j, k;
  for(k=0; k<sizey; k++) {
    for(j=0; j<sizex; j++) {
      imgfloat = image[sizex*k + j];
      if(imgfloat>=TARGET_INTENSITY) { imgval = 252; }
      else { imgval = 0; }
      if(dose==1.0) { pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = pngimg[sizex*k + j].b =  imgval; }
      else if(dose==MAX_DOSE) { pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = pngimg[sizex*k + j].b =  imgval/2; }
      else if(dose==MIN_DOSE) { pngimg[sizex*k + j].g = 0; pngimg[sizex*k + j].r = pngimg[sizex*k + j].b =  imgval/2; }
      pngimg[sizex*k + j].a = 127;
    }
  }
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(fname, (const unsigned char*)pngimg, sizex, sizey);

  /*if there's an error, display it*/
  if(error) fprintf(stderr, "*Error* %u: %s\n", error, lodepng_error_text(error));

  free(pngimg);
}

void write_pvband(float* inner_img, float* outer_img, char* fname, int sizex, int sizey)
{
  /* This section dumps out the PV Band */
  ucharac4 *pngimg;
  pngimg = (ucharac4*)malloc(sizex * sizey * sizeof(ucharac4));
  float in_img, out_img;
  int imgval, j, k;
  for(k=0; k<sizey; k++) {
    for(j=0; j<sizex; j++) {
      in_img = inner_img[sizex*k + j];
      out_img = outer_img[sizex*k + j];
      if(out_img>=TARGET_INTENSITY && in_img<TARGET_INTENSITY) { imgval = 255; }
      else { imgval = 0; }
      pngimg[sizex*k + j].r = imgval/2; pngimg[sizex*k + j].g = imgval; pngimg[sizex*k + j].b = 0;
      pngimg[sizex*k + j].a = 255;
    }
  }
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(fname, (const unsigned char*)pngimg, sizex, sizey);

  /*if there's an error, display it*/
  if(error) fprintf(stderr, "*Error* %u: %s\n", error, lodepng_error_text(error));

  free(pngimg);
}

//below added
void write_image_epe_file(float* image, char* fname, int sizex, int sizey, float dose)
{
  /* This section dumps out the image */
  ucharac4 *pngimg;
  pngimg = (ucharac4*)malloc(sizex * sizey * sizeof(ucharac4));
  float imgfloat;
  int imgval, j, k;
  for(k=0; k<sizey; k++) {
    for(j=0; j<sizex; j++) {
      imgfloat = image[sizex*k + j];
      if(imgfloat>=TARGET_INTENSITY) { imgval = 252; }
      else { imgval = 0; }
      if(dose==1.0) { pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = pngimg[sizex*k + j].b =  imgval; }
      else if(dose==MAX_DOSE) { pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = pngimg[sizex*k + j].b =  imgval/2; }
      else if(dose==MIN_DOSE) { pngimg[sizex*k + j].g = 0; pngimg[sizex*k + j].r = pngimg[sizex*k + j].b =  imgval/2; }
      pngimg[sizex*k + j].a = 127;
    }
  }

  //============================== add boundary for EPE check
  int rectCount = lvlcnt[0];
  unsigned int pxlR = 0;
  unsigned int pxlG = 153;
  unsigned int pxlB = 0;
  int i;
  int x, y;
  int EPEConstraint=10;
  for( i = 0; i<rectCount; i++) {
    rect r = lvldata[0][i];
    int x0 = r.x - ef_xoff + 512;
    int y0 = r.y - ef_yoff + 512;
    int x1 = x0 + r.w + 1;
    int y1 = y0 + r.h + 1;
    //inside
    //draw horizontal line
    for( x = x0 + EPEConstraint ; x<x1 - EPEConstraint; ++x)
    {
      y = y0 + EPEConstraint;
      pngimg[sizex*y + x].r = pxlR;
      pngimg[sizex*y + x].g = pxlG;
      pngimg[sizex*y + x].b = pxlB;
      y = y1 - EPEConstraint;
      pngimg[sizex*y + x].r = pxlR;
      pngimg[sizex*y + x].g = pxlG;
      pngimg[sizex*y + x].b = pxlB;
    }
    //draw vertical line
    for( y = y0 + EPEConstraint ; y<y1 - EPEConstraint; ++y)
    {
      x = x0 + EPEConstraint;
      pngimg[sizex*y + x].r = pxlR;
      pngimg[sizex*y + x].g = pxlG;
      pngimg[sizex*y + x].b = pxlB;
      x = x1 - EPEConstraint;
      pngimg[sizex*y + x].r = pxlR;
      pngimg[sizex*y + x].g = pxlG;
      pngimg[sizex*y + x].b = pxlB;
    }
    //outside
    //draw horizontal line
    for( x = x0 - EPEConstraint ; x<x1 + EPEConstraint; ++x)
    {
      y = y0 - EPEConstraint;
      pngimg[sizex*y + x].r = pxlR;
      pngimg[sizex*y + x].g = pxlG;
      pngimg[sizex*y + x].b = pxlB;
      y = y1 + EPEConstraint;
      pngimg[sizex*y + x].r = pxlR;
      pngimg[sizex*y + x].g = pxlG;
      pngimg[sizex*y + x].b = pxlB;
    }
    //draw vertical line
    for( y = y0 - EPEConstraint ; y<y1 + EPEConstraint; ++y)
    {
      x = x0 - EPEConstraint;
      pngimg[sizex*y + x].r = pxlR;
      pngimg[sizex*y + x].g = pxlG;
      pngimg[sizex*y + x].b = pxlB;
      x = x1 + EPEConstraint;
      pngimg[sizex*y + x].r = pxlR;
      pngimg[sizex*y + x].g = pxlG;
      pngimg[sizex*y + x].b = pxlB;
    }
  }
  //==============================
  /*Encode the image*/
  unsigned error = lodepng_encode32_file(fname, (const unsigned char*)pngimg, sizex, sizey);

  /*if there's an error, display it*/
  if(error) fprintf(stderr, "*Error* %u: %s\n", error, lodepng_error_text(error));

  free(pngimg);
}


//namespace pvbandsim
//{
////binary version
//void write_pvband2(unsigned char* inner_img, unsigned char* outer_img, char* fname, int sizex, int sizey)
//{
//  /* This section dumps out the PV Band */
//  ucharac4 *pngimg;
//  pngimg = (ucharac4*)malloc(sizex * sizey * sizeof(ucharac4));
//  unsigned char in_img, out_img;
//  int imgval, j, k;
//  for(k=0; k<sizey; k++) {
//    for(j=0; j<sizex; j++) {
//      in_img = inner_img[sizex*k + j];
//      out_img = outer_img[sizex*k + j];
//      if(out_img != in_img) { imgval = 255; }
//      else { imgval = 0; }
//      pngimg[sizex*k + j].r = 0; pngimg[sizex*k + j].g = 0; pngimg[sizex*k + j].b =  imgval;
//      pngimg[sizex*k + j].a = 255;
//    }
//  }
//  /*Encode the image*/
//  unsigned error = lodepng_encode32_file(fname, (const unsigned char*)pngimg, sizex, sizey);
//
//  /*if there's an error, display it*/
//  if(error) fprintf(stderr, "*Error* %u: %s\n", error, lodepng_error_text(error));
//
//  free(pngimg);
//}
//} //namespace

#endif

