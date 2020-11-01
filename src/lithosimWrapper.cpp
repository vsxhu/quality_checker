#include "lithosimWrapper.h"
#include "kiss_fft.h"

using namespace pvbandsim;

LithosimWrapper::LithosimWrapper()
{
  m_mask = new unsigned char[OPC_TILE_SIZE];
  for (int i = 0; i < NUM_PROCESS_CORNERS; ++i)
    m_image[i] = new float[OPC_TILE_SIZE];
  initSimulate();
}

LithosimWrapper::~LithosimWrapper()
{
  endSimulate();
  delete m_mask;
  for (int i = 0; i < NUM_PROCESS_CORNERS; ++i)
    delete m_image[i];
}

//mask value is either 0 or 255
void
LithosimWrapper::writeMaskPng(char* fileName, float* mask, int xSize, int ySize)
{
  int pixel;
  for (int i = 0; i < OPC_TILE_SIZE; ++i)
  {
    if (mask[i] >= MASK_PRINTABLE_THRESHOLD)
      pixel = 1;
    else
      pixel = 0;
    m_mask[i] = pixel * OPC_PIXEL_VALUE;
  }
  write_mask_file(m_mask, fileName, xSize, ySize);
}

void
LithosimWrapper::writeImagePng(char* fileName, float* image, float dose,
    int xSize, int ySize)
{
  write_image_file(image, fileName, xSize, ySize, dose);
}

void
LithosimWrapper::writeImageEpePng(char* fileName, float* image, float dose,
    char* inLayoutFile, int xSize, int ySize)
{
  if (inLayoutFile)
    read_layout(inLayoutFile);
  write_image_epe_file(image, fileName, xSize, ySize, dose);
}

//image value is a float number after simulation
//void
//LithosimWrapper::writeSimImagePng(char* fileName, float* image,
//    int xSize, int ySize, float dose)
//{
//  write_image_file(float* image, char* fname, int sizex, int sizey, float dose);
//}

void
LithosimWrapper::simulateImage(float* mask, unsigned char* image,
    kernel_t kernelType, float dose, int kernelLevel)
{
  pvbandsim::simulateImageB(mask, image, (int)kernelType, dose, kernelLevel);
}

//only do optical simulation, no photorisist threshold
void
LithosimWrapper::simulateImageOpt(float* mask, float* image,
    kernel_t kernelType, float dose, int kernelLevel)
{
  pvbandsim::simulateImageOpt(mask, image, (int)kernelType, dose, kernelLevel);
}

//only do optical simulation, no photorisist threshold
//void
//LithosimWrapper::simulateImageCombo(float* mask, float* image,
//    kernel_t kernelType, float dose)
//{
//  pvbandsim::simulateImageCombo(mask, image, (int)kernelType, dose);
//}

void
LithosimWrapper::convolveKernel(float* mask, kiss_fft_cpx *result,
    kernel_t kernelType, float dose, int kernelLevel)
{
  kiss_fft_cpx* maskf = pvbandsim::maskfft(mask, dose);
  pvbandsim::convolveKernelCpx(maskf, result, (int)kernelType, kernelLevel);
}

void
LithosimWrapper::convolveKernel(kiss_fft_cpx *matrix, kiss_fft_cpx *result,
    kernel_t kernelType, float dose, int kernelLevel)
{
  kiss_fft_cpx* matrixf = pvbandsim::cpxfft(matrix, dose);
  pvbandsim::convolveKernelCpx(matrixf, result, (int)kernelType, kernelLevel);
}

int
LithosimWrapper::calculatePvband(float* mask)
{
  //outer: focus, maxdose
  pvbandsim::simulateImageOpt(mask, m_image[0], LITHO_KERNEL_FOCUS, MAX_DOSE,
      NUM_LITHO_KERNELS);
  //inner: defocus, mindose
  pvbandsim::simulateImageOpt(mask, m_image[1], LITHO_KERNEL_DEFOCUS, MIN_DOSE,
      NUM_LITHO_KERNELS);
  int pvbandArea = getPvband(m_image[1], m_image[0]);
  return pvbandArea;
}

int
LithosimWrapper::calculatePvband(float* inner, float* outer)
{
  return getPvband(inner, outer);
}

void
LithosimWrapper::writePvband(char* fileName)
{
  pvbandsim::writePvband(fileName, m_image[0], m_image[1]);
}

void
LithosimWrapper::writePvband(char* fileName, float* inner, float* outer)
{
  pvbandsim::writePvband(fileName, inner, outer);
}

