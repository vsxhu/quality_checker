#ifndef _EPE_CHECKER_H_
#define _EPE_CHECKER_H_
#include <vector>
#include "kiss_fft.h"

class Coordinate;
class Rect;
class Polygon;
class Design;
class EpeSample;

extern float TARGET_INTENSITY;  //defined in pvbandsim

const int EPE_CONSTRAINT = 10;  //unit:nm  15
const int EPE_CHECK_START_INTERVEL = 40;
const int EPE_CHECK_INTERVEL = 40;
const int MIN_EPE_CHECK_LENGTH = 80;
const int EPE_OFFSET_X = 512;
const int EPE_OFFSET_Y = 512;
//const int EPE_TILE_X = 1024;
//const int EPE_TILE_Y = 1024;
const int EPE_TILE_X = 2048;
const int EPE_TILE_Y = 2048;
const int EPE_TILE_END_X = EPE_OFFSET_X + 1280;
const int EPE_TILE_END_Y = EPE_OFFSET_Y + 1280;

class EpeChecker
{
public:
  typedef enum {HORIZONTAL, VERTICAL} orient_t;
  typedef enum {UP, DOWN, LEFT, RIGHT} direction_t;

  //EpeChecker(Design& design);
  EpeChecker();
  ~EpeChecker();

  const std::vector<Coordinate*>& getViolations() const { return m_violations; }

  void setDesign(Design& design);
  void setDesign(Design& design, float* targetImage);
  void clear();
  int run(const float* image);
  int run(const std::vector<EpeSample>* samples, const float* image, const float* target);
  void setEPESafeRegion(float* matrix, int constraint = EPE_CONSTRAINT);
  void setEPEBoundary(float* matrix, int constraint = EPE_CONSTRAINT);
  std::vector<EpeSample>* findSamplePoint();
  bool equalf(float num1, float num2) const { return fabs(num1 - num2) < 0.000001; }
  bool innerViolate(float target, float image) const { return (target - image > 1e-6); }
  bool outerViolate(float target, float image) const { return (image - target > 1e-6); }

  static int getIndex(int x, int y) { return (y << 11) + x; }

private:
  void binaryImage(const float* image);
  direction_t determineCheckDiretion(Polygon* polygon, int x, int y, orient_t edgeOrient);
  void check(int x, int y, direction_t direction);
//  void check(int x, int y, orient_t orient);
  void checkInside(int x, int y, direction_t direction);
  int getEpeTileIndex(int x, int y) const { return (y << 10) + x; }

  std::vector<Polygon*> m_polygons;
  std::vector<Coordinate*> m_violations;
  std::vector<EpeSample> m_samples;
  bool* m_bimage;
  float* m_targetImage;
  int m_numTruePlgns;
  bool m_valid;
  int m_numEPEIn;
  int m_numEPEOut;
};

class EpeSample
{
public:
  EpeSample(int x, int y, EpeChecker::orient_t orient) : m_orient(orient)
  {
    m_x = EPE_OFFSET_X + x;
    m_y = EPE_OFFSET_Y + y;
    m_numViolation = 0;
  }

  int getX() const { return m_x; }
  int getY() const { return m_y; }
  EpeChecker::orient_t getOrient() const { return m_orient; }

  void setSigProduct(float value) { m_sigProduct = value; }
  float getSigProduct() const { return m_sigProduct; }

  void setPosition(kiss_fft_cpx* from, kiss_fft_cpx* to) const;
  void resetPosition(kiss_fft_cpx* matrix) const;

private:
  int m_x;  //offset x on opc grid
  int m_y;  //offset y on opc grid
  EpeChecker::orient_t m_orient;
//  float m_sumGradientDiff; //summation of d(Z-Z0) near the sample point
  float m_sigProduct; //sig(sumDiffPxl)*(1-sumDiffPxl)
  int m_numViolation;
};

#endif

