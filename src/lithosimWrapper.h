#ifndef _LITHOSIMWRAPPER_H_
#define _LITHOSIMWRAPPER_H_
#include "kiss_fft.h"
class Rect;

//below defined in pvbandsim=============
extern float MIN_DOSE;
extern float MAX_DOSE;
extern float TARGET_INTENSITY;
extern void write_mask_file(unsigned char* wmsk, char* fname, int imx, int imy);
extern void write_image_file(float* image, char* fname, int sizex, int sizey, float dose);
extern void write_image_epe_file(float* image, char* fname, int sizex, int sizey, float dose);
extern void read_layout(char* fname);

namespace pvbandsim
{
  extern void initSimulate();
  extern void endSimulate();
//  extern void simulateImage(unsigned char *mask, float* image,
//      int kernelIdx, float dose);
  extern void simulateImageB(float *mask, unsigned char* image,
      int kernelIdx, float dose, int kernelLevel);
  extern void simulateImageOpt(float *mask, float *image,
      int kernelIdx, float dose, int kernelLevel);
//  extern void simulateImageCombo(float *mask, float *image,
//      int kernelIdx, float dose);
  extern kiss_fft_cpx* maskfft(float *mask, float dose);
  extern kiss_fft_cpx* cpxfft(kiss_fft_cpx *matrix, float dose);
  extern void convolveKernelCpx(kiss_fft_cpx *matrix, kiss_fft_cpx *result,
      int kernelIdx, int kernelLevel);
  extern int getPvband(float* inner, float* outer);
  extern void writePvband(char* fileName, float* inner, float* outer);
}
//================

const int OPC_PIXEL_VALUE = 255;
const float MASK_PRINTABLE_THRESHOLD = 0.5; // >=50% pxl value
const int OPC_TILE_X = (1 << 11);
const int OPC_TILE_Y = (1 << 11);
const int OPC_TILE_SIZE = (1 << 22);  //2048x2048
const int NUM_PROCESS_CORNERS = 3;
const int NUM_LITHO_KERNELS = 24; //according to kernel data
//const int NUM_KERNEL_USED = NUM_LITHO_KERNELS;
const int NUM_KERNEL_USED = 15; //9

class LithosimWrapper
{
public:
  typedef enum {LITHO_KERNEL_FOCUS, LITHO_KERNEL_DEFOCUS,
    LITHO_KERNEL_FOCUS_CT, LITHO_KERNEL_DEFOCUS_CT,
    LITHO_KERNEL_FOCUS_PC, LITHO_KERNEL_DEFOCUS_PC} kernel_t;
  LithosimWrapper();
  ~LithosimWrapper();

  void writeMaskPng(char* fileName, float* mask, int xSize = 2048, int ySize = 2048);
  void writeImagePng(char* fileName, float* image, float dose, int xSize = 2048, int ySize = 2048);
  void writeImageEpePng(char* fileName, float* image, float dose,
      char* inLayoutFile = NULL, int xSize = 2048, int ySize = 2048);

  void simulateImage(float* mask, unsigned char* image, kernel_t kernelType,
      float dose, int kernelLevel = NUM_KERNEL_USED);
  void simulateImageOpt(float* mask, float* image, kernel_t kernelType,
      float dose, int kernelLevel = NUM_KERNEL_USED);
//  void simulateImageCombo(float* mask, float* image, kernel_t kernelType,
//      float dose);
  void convolveKernel(float* mask, kiss_fft_cpx *result, kernel_t kernelType,
      float dose, int kernelLevel = NUM_KERNEL_USED);
  void convolveKernel(kiss_fft_cpx *matrix, kiss_fft_cpx *result,
      kernel_t kernelType, float dose, int kernelLevel = NUM_KERNEL_USED);
  int calculatePvband(float* mask);
  int calculatePvband(float* inner, float* outer);
  void writePvband(char* fileName);
  void writePvband(char* fileName, float* inner, float* outer);

private:
  unsigned char *m_mask;
  float *m_image[NUM_PROCESS_CORNERS];
};

#endif

