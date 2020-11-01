#include <vector>
#include <algorithm>
#include <cstring>
#include "epeChecker.h"
#include "shape.h"
#include "design.h"
#include "debug.h"
#include "lithosimWrapper.h"
#include "kiss_fft.h"

using namespace std;

EpeChecker::EpeChecker()
  : m_valid(false)
  , m_numEPEIn(0)
  , m_numEPEOut(0)
{
  m_bimage = new bool[EPE_TILE_Y * EPE_TILE_Y];
}

EpeChecker::~EpeChecker()
{
  delete m_bimage;

  //delete only self created polygons
  for (unsigned int i = m_numTruePlgns; i < m_polygons.size(); ++i)
    delete m_polygons[i];

  clear();
}

void
EpeChecker::setDesign(Design& design)
{
  m_valid = true;

  const vector<Polygon*>& polygons = design.getPolygons();
  m_polygons.assign(polygons.begin(), polygons.end());
  m_numTruePlgns = m_polygons.size();

  const vector<Rect*>& rects = design.getRects();
  int numTrueRects = design.getNumTrueRects();
  for (int i = 0; i < numTrueRects; ++i)
  {
    //tranform rectangle into polygon
    const Coordinate& ll = rects[i]->ll();
    const Coordinate& ur = rects[i]->ur();
    Polygon* plgn = new Polygon();
    plgn->addPoint(ll.x, ll.y);
    plgn->addPoint(ur.x, ll.y);
    plgn->addPoint(ur.x, ur.y);
    plgn->addPoint(ll.x, ur.y);
    m_polygons.push_back(plgn);
  }
}

void
EpeChecker::setDesign(Design& design, float* targetImage)
{
  setDesign(design);
  m_targetImage = targetImage;
}

void
EpeChecker::clear()
{
  for (vector<Coordinate*>::iterator it = m_violations.begin();
      it != m_violations.end(); ++it)
    delete (*it);
  m_violations.clear();
  m_numEPEIn = 0;
  m_numEPEOut = 0;
}

//must called after findSamplePoint
//return number of total EPE violations
int
EpeChecker::run(const vector<EpeSample>* samples, const float* image, const float* target)
{
  if (!m_valid)
    emesg("must set design before checking EPE\n");

  clear();
  binaryImage(image);

  int index;
  int sampleX, sampleY; //note they are on 2048x2048 grid

  for (vector<EpeSample>::const_iterator it = samples->begin(); it != samples->end(); ++it)
  {
    sampleX = it->getX();
    sampleY = it->getY();
    if (it->getOrient() == EpeChecker::HORIZONTAL)
    {
      //check vertical direction
      index = getIndex(sampleX, sampleY - EPE_CONSTRAINT);
      if (!equalf((float)m_bimage[index], target[index]))
      {
        if (innerViolate((float)m_bimage[index], target[index]))
          ++m_numEPEIn;
        else
          ++m_numEPEOut;
        Coordinate* coord = new Coordinate(sampleX, sampleY - EPE_CONSTRAINT);
        m_violations.push_back(coord);
      }
      index = getIndex(sampleX, sampleY + EPE_CONSTRAINT);
      if (!equalf((float)m_bimage[index], target[index]))
      {
        if (innerViolate((float)m_bimage[index], target[index]))
          ++m_numEPEIn;
        else
          ++m_numEPEOut;
        Coordinate* coord = new Coordinate(sampleX, sampleY + EPE_CONSTRAINT);
        m_violations.push_back(coord);
      }
    }
    else  //VERTICAL
    {
      //check horizontal direction
      index = getIndex(sampleX - EPE_CONSTRAINT, sampleY);
      if (!equalf((float)m_bimage[index], target[index]))
      {
        if (innerViolate((float)m_bimage[index], target[index]))
          ++m_numEPEIn;
        else
          ++m_numEPEOut;
        Coordinate* coord = new Coordinate(sampleX - EPE_CONSTRAINT, sampleY);
        m_violations.push_back(coord);
      }
      index = getIndex(sampleX + EPE_CONSTRAINT, sampleY);
      if (!equalf((float)m_bimage[index], target[index]))
      {
        if (innerViolate((float)m_bimage[index], target[index]))
          ++m_numEPEIn;
        else
          ++m_numEPEOut;
        Coordinate* coord = new Coordinate(sampleX + EPE_CONSTRAINT, sampleY);
        m_violations.push_back(coord);
      }
    }
  }

  dmesg("Total %d EPE violations: %d inner and %d outer\n", m_violations.size(),
      m_numEPEIn, m_numEPEOut);
//  for (vector<Coordinate*>::iterator it = m_violations.begin();
//      it != m_violations.end(); ++it)
//  {
//    (*it)->print();
//    cout << ", ";
//  }
//  cout << endl;

  return m_violations.size();
}

//must called after setDesign
//return number of total EPE violations
int
EpeChecker::run(const float* image)
{
  if (!m_valid)
    emesg("must set design before checking EPE\n");

  clear();
  binaryImage(image);
  Coordinate pt1;
  Coordinate pt2;
  Polygon* polygon;
  for (vector<Polygon*>::iterator it = m_polygons.begin(); it != m_polygons.end(); ++it)
  {
    polygon = (*it);
    const vector<Coordinate*>& points = polygon->getPoints();
    pt1 = (*points.back());
    for (vector<Coordinate*>::const_iterator ptIt = points.begin();
        ptIt != points.end(); ++ptIt)
    {
      pt2 = (**ptIt);
      int center, start, end;
      if (pt1.y == pt2.y) //horizontal
      {
        start = pt1.x;
        end = pt2.x;
        if (start > end)
          swap(start, end);
        center = (pt1.x + pt2.x) / 2;
        direction_t direction = determineCheckDiretion(polygon, center, pt1.y, HORIZONTAL);
        if (end - start <= MIN_EPE_CHECK_LENGTH)
          check(center, pt1.y, direction);
          //check(center, pt1.y, HORIZONTAL);
        else
        {
          for (int x = start + EPE_CHECK_START_INTERVEL; x <= center; x += EPE_CHECK_INTERVEL)
          {
            check(x, pt1.y, direction);
            //check(x, pt1.y, HORIZONTAL);
          }
          for (int x = end - EPE_CHECK_START_INTERVEL; x > center; x -= EPE_CHECK_INTERVEL)
          {
            check(x, pt1.y, direction);
            //check(x, pt1.y, HORIZONTAL);
          }
        }
      }
      else  //vertical
      {
        start = pt1.y;
        end = pt2.y;
        if (start > end)
          swap(start, end);
        center = (pt1.y + pt2.y) / 2;
        direction_t direction = determineCheckDiretion(polygon, pt1.x, center, VERTICAL);
        if (end - start <= MIN_EPE_CHECK_LENGTH)
          check(pt1.x, center, direction);
          //check(pt1.x, center, VERTICAL);
        else
        {
          for (int y = start + EPE_CHECK_START_INTERVEL; y <= center ; y += EPE_CHECK_INTERVEL)
          {
            check(pt1.x, y, direction);
            //check(pt1.x, y, VERTICAL);
          }
          for (int y = end - EPE_CHECK_START_INTERVEL; y > center; y -= EPE_CHECK_INTERVEL)
          {
            check(pt1.x, y, direction);
            //check(pt1.x, y, VERTICAL);
          }
        }
      }

      pt1 = pt2;
    }
  }

  dmesg("Total %d EPE violations: %d inner and %d outer\n", m_violations.size(),
      m_numEPEIn, m_numEPEOut);
//  for (vector<Coordinate*>::iterator it = m_violations.begin();
//      it != m_violations.end(); ++it)
//  {
//    (*it)->print();
//    cout << ", ";
//  }
//  cout << endl;

  return m_violations.size();
}

//obtain binary mask
void
EpeChecker::binaryImage(const float* image)
{
  memset(m_bimage, 0, sizeof(bool) * EPE_TILE_X * EPE_TILE_Y);

  int index;
  for (int y = EPE_OFFSET_Y; y < EPE_TILE_Y; ++y)
    for (int x = EPE_OFFSET_X; x < EPE_TILE_X; ++x)
    {
      index = getIndex(x, y);
      if (image[index] >= MASK_PRINTABLE_THRESHOLD)
        m_bimage[index] = 1;
    }

//  int imgIdx, bImgIdx;
//  for (int y = 0; y < EPE_TILE_Y; ++y)
//    for (int x = 0; x < EPE_TILE_X; ++x)
//    {
//      bImgIdx = y * EPE_TILE_X + x;
//      imgIdx = getIndex(EPE_OFFSET_X + x, EPE_OFFSET_Y + y);
//      if (image[imgIdx] >= TARGET_INTENSITY)
//        m_bimage[bImgIdx] = 1;
//    }
}

//return the EPE internal check direction from x, y
EpeChecker::direction_t
EpeChecker::determineCheckDiretion(Polygon* polygon, int x, int y, orient_t edgeOrient)
{
  if (edgeOrient == HORIZONTAL)
  {
    if (polygon->pointInPolygon (x, y - 1))
      return DOWN;
    else
      return UP;
  }
  else  //VERTICAL
  {
    if (polygon->pointInPolygon(x - 1, y))
      return LEFT;
    else
      return RIGHT;
  }
}

//only check EPE for image inside the desired pattern
//direction points to internal of the pattern
//void
//EpeChecker::checkInside(int x, int y, direction_t direction)
//{
//  Coordinate checkPt; //the point inside the polygon
//  if ((direction == LEFT) || (direction == RIGHT))
//  {
//    if (direction == LEFT)
//      checkPt.x = x - EPE_CONSTRAINT;
//    else  //RIGHT
//      checkPt.x = x + EPE_CONSTRAINT;
//    checkPt.y = y;
//  }
//  else
//  {
//    if (direction == DOWN)
//      checkPt.y = y - EPE_CONSTRAINT;
//    else  //UP
//      checkPt.y = y + EPE_CONSTRAINT;
//    checkPt.x = x;
//  }
//
//  if (m_bimage[checkPt.y * EPE_TILE_X + checkPt.x] != 1)
//  {
//    Coordinate* coord = new Coordinate(x, y);
//    m_violations.push_back(coord);
//  }
//}

//direction points to internal of the pattern
void
EpeChecker::check(int x, int y, direction_t direction)
{
  //apply offset
  x = x + EPE_OFFSET_X;
  y = y + EPE_OFFSET_Y;

  Coordinate checkPtIn, checkPtOut; //the point inside/outside the polygon
  if ((direction == LEFT) || (direction == RIGHT))
  {
    if (direction == LEFT)
    {
      checkPtIn.x = x - EPE_CONSTRAINT;
      checkPtOut.x = x + EPE_CONSTRAINT;
    }
    else  //RIGHT
    {
      checkPtIn.x = x + EPE_CONSTRAINT;
      checkPtOut.x = x - EPE_CONSTRAINT;
    }
    checkPtIn.y = checkPtOut.y = y;
  }
  else
  {
    if (direction == DOWN)
    {
      checkPtIn.y = y - EPE_CONSTRAINT;
      checkPtOut.y = y + EPE_CONSTRAINT;
    }
    else  //UP
    {
      checkPtIn.y = y + EPE_CONSTRAINT;
      checkPtOut.y = y - EPE_CONSTRAINT;
    }
    checkPtIn.x = checkPtOut.x = x;
  }

  //inside point should be 1 to avoid shortening
  if (m_bimage[checkPtIn.y * EPE_TILE_X + checkPtIn.x] != 1)
  {
    Coordinate* coord = new Coordinate(x, y);
    m_violations.push_back(coord);
    ++m_numEPEIn;
  }
  //outside point should be 0 to avoid bridging
  if (m_bimage[checkPtOut.y * EPE_TILE_X + checkPtOut.x] != 0)
  {
    Coordinate* coord = new Coordinate(x, y);
    m_violations.push_back(coord);
    ++m_numEPEOut;
  }
}

//bug
//void
//EpeChecker::check(int x, int y, orient_t orient)
//{
//  Coordinate pt1, pt2;
//  if (orient == HORIZONTAL)
//  {
//    //check vertical
//    pt1.x = pt2.x = x;
//    pt1.y = y - EPE_CONSTRAINT;
//    pt2.y = y + EPE_CONSTRAINT;
//  }
//  else  //VERTICAL
//  {
//    //check horizontal
//    pt1.y = pt2.y = y;
//    pt1.x = x - EPE_CONSTRAINT;
//    pt2.x = x + EPE_CONSTRAINT;
//  }
//
//  int bIndex, tIndex;
//  bIndex = getEpeTileIndex(pt1.x, pt1.y);
//  tIndex = getIndex(pt1.x, pt1.y);
//  //dmesg("%d,%d:%d-%f ", pt1.x, pt1.y, m_bimage[index], m_targetImage[index]);
//  if ((m_bimage[bIndex] > 0.5) ^ (m_targetImage[tIndex] > 0.5))
//  {
//    Coordinate* coord = new Coordinate(x, y);
//    m_violations.push_back(coord);
//  }
//  bIndex = getEpeTileIndex(pt2.x, pt2.y);
//  tIndex = getIndex(pt2.x, pt2.y);
//  //dmesg("%d,%d:%d-%f ", pt2.x, pt2.y, m_bimage[index], m_targetImage[index]);
//  if ((m_bimage[bIndex] > 0.5) ^ (m_targetImage[tIndex] > 0.5))
//  {
//    Coordinate* coord = new Coordinate(x, y);
//    m_violations.push_back(coord);
//  }
//}

//epe safe region will be set as 1, otherwise 0
void
EpeChecker::setEPESafeRegion(float* matrix, int constraint)
{
  if (!m_valid)
    emesg("must set design before checking EPE\n");

  memset(matrix, 0, sizeof(float) * OPC_TILE_SIZE);
  Coordinate pt1;
  Coordinate pt2;
  Polygon* polygon;
  for (vector<Polygon*>::iterator it = m_polygons.begin(); it != m_polygons.end(); ++it)
  {
    polygon = (*it);
    const vector<Coordinate*>& points = polygon->getPoints();
    pt1 = (*points.back());
    for (vector<Coordinate*>::const_iterator ptIt = points.begin();
        ptIt != points.end(); ++ptIt)
    {
      pt2 = (**ptIt);
      int start, end;
      if (pt1.y == pt2.y) //horizontal
      {
        start = pt1.x;
        end = pt2.x;
        if (start > end)
          swap(start, end);
        for (int y = pt1.y - constraint; y <= pt1.y + constraint; ++y)
          for (int x = start - constraint; x <= end + constraint; ++x)
            matrix[getIndex(EPE_OFFSET_X + x, EPE_OFFSET_Y + y)] = 1;
      }
      else  //vertical
      {
        start = pt1.y;
        end = pt2.y;
        if (start > end)
          swap(start, end);
        for (int y = start - constraint; y <= end + constraint; ++y)
          for (int x = pt1.x - constraint; x <= pt1.x + constraint; ++x)
            matrix[getIndex(EPE_OFFSET_X + x, EPE_OFFSET_Y + y)] = 1;
      }

      pt1 = pt2;
    }
  }
}

//epe boundary will be set as 1, otherwise 0
void
EpeChecker::setEPEBoundary(float* matrix, int constraint)
{
  if (!m_valid)
    emesg("must set design before checking EPE\n");

  memset(matrix, 0, sizeof(float) * OPC_TILE_SIZE);
  Coordinate pt1;
  Coordinate pt2;
  Polygon* polygon;
  for (vector<Polygon*>::iterator it = m_polygons.begin(); it != m_polygons.end(); ++it)
  {
    polygon = (*it);
    const vector<Coordinate*>& points = polygon->getPoints();
    pt1 = (*points.back());
    for (vector<Coordinate*>::const_iterator ptIt = points.begin();
        ptIt != points.end(); ++ptIt)
    {
      pt2 = (**ptIt);
      int start, end;
      if (pt1.y == pt2.y) //horizontal
      {
        start = pt1.x;
        end = pt2.x;
        if (start > end)
          swap(start, end);
        for (int x = start - constraint; x <= end + constraint; ++x)
        {
          matrix[getIndex(EPE_OFFSET_X + x, EPE_OFFSET_Y + pt1.y - constraint)] = 1;
          matrix[getIndex(EPE_OFFSET_X + x, EPE_OFFSET_Y + pt1.y + constraint)] = 1;
        }
      }
      else  //vertical
      {
        start = pt1.y;
        end = pt2.y;
        if (start > end)
          swap(start, end);
        for (int y = start - constraint; y <= end + constraint; ++y)
        {
          matrix[getIndex(EPE_OFFSET_X + pt1.x - constraint, EPE_OFFSET_Y + y)] = 1;
          matrix[getIndex(EPE_OFFSET_X + pt1.x + constraint, EPE_OFFSET_Y + y)] = 1;
        }
      }

      pt1 = pt2;
    }
  }
}

vector<EpeSample>*
EpeChecker::findSamplePoint()
{
  Coordinate pt1;
  Coordinate pt2;
  Polygon* polygon;
  for (vector<Polygon*>::iterator it = m_polygons.begin(); it != m_polygons.end(); ++it)
  {
    polygon = (*it);
    const vector<Coordinate*>& points = polygon->getPoints();
    pt1 = (*points.back());
    for (vector<Coordinate*>::const_iterator ptIt = points.begin();
        ptIt != points.end(); ++ptIt)
    {
      pt2 = (**ptIt);
      int center, start, end;
      if (pt1.y == pt2.y) //horizontal
      {
        start = pt1.x;
        end = pt2.x;
        if (start > end)
          swap(start, end);
        center = (pt1.x + pt2.x) / 2;
        if (end - start <= MIN_EPE_CHECK_LENGTH)
          m_samples.push_back(EpeSample(center, pt1.y, HORIZONTAL));
        else
        {
          for (int x = start + EPE_CHECK_START_INTERVEL; x <= center; x += EPE_CHECK_INTERVEL)
          {
            m_samples.push_back(EpeSample(x, pt1.y, HORIZONTAL));
          }
          for (int x = end - EPE_CHECK_START_INTERVEL; x > center; x -= EPE_CHECK_INTERVEL)
          {
            m_samples.push_back(EpeSample(x, pt1.y, HORIZONTAL));
          }
        }
      }
      else  //vertical
      {
        start = pt1.y;
        end = pt2.y;
        if (start > end)
          swap(start, end);
        center = (pt1.y + pt2.y) / 2;
        if (end - start <= MIN_EPE_CHECK_LENGTH)
          m_samples.push_back(EpeSample(pt1.x, center, VERTICAL));
        else
        {
          for (int y = start + EPE_CHECK_START_INTERVEL; y <= center; y += EPE_CHECK_INTERVEL)
          {
            m_samples.push_back(EpeSample(pt1.x, y, VERTICAL));
          }
          for (int y = end - EPE_CHECK_START_INTERVEL; y > center; y -= EPE_CHECK_INTERVEL)
          {
            m_samples.push_back(EpeSample(pt1.x, y, VERTICAL));
          }
        }
      }

      pt1 = pt2;
    }
  }
  dmesg("Total number of EPE samples: %d\n", m_samples.size());

//  for (vector<EpeSample>::iterator it = m_samples.begin(); it != m_samples.end(); ++it)
//    cout << "(" << it->getX() - EPE_OFFSET_X << ", " << it->getY() - EPE_OFFSET_Y << ") ";
//  cout << endl;

  return &m_samples;
}

void
EpeSample::setPosition(kiss_fft_cpx* from, kiss_fft_cpx* to) const
{
  int index;
  if (m_orient == EpeChecker::HORIZONTAL)
  {
    //for (int y = m_y - EPE_CONSTRAINT; y <= m_y + EPE_CONSTRAINT; ++y)  //count epe range
    //count only 2 points
    for (int y = m_y - EPE_CONSTRAINT; y <= m_y + EPE_CONSTRAINT; y += 2 * EPE_CONSTRAINT)
    {
      index = EpeChecker::getIndex(m_x, y);
      to[index].r = from[index].r;
      to[index].i = from[index].i;
    }
  }
  else  //VERTICAL
  {
    //for (int x = m_x - EPE_CONSTRAINT; x <= m_x + EPE_CONSTRAINT; ++x)  //count epe range
    //count only 2 points
    for (int x = m_x - EPE_CONSTRAINT; x <= m_x + EPE_CONSTRAINT; x += 2 * EPE_CONSTRAINT)
    {
      index = EpeChecker::getIndex(x, m_y);
      to[index].r = from[index].r;
      to[index].i = from[index].i;
    }
  }
}

void
EpeSample::resetPosition(kiss_fft_cpx* matrix) const
{
  int index;
  if (m_orient == EpeChecker::HORIZONTAL)
  {
    //for (int y = m_y - EPE_CONSTRAINT; y <= m_y + EPE_CONSTRAINT; ++y)  //count epe range
    //count only 2 points
    for (int y = m_y - EPE_CONSTRAINT; y <= m_y + EPE_CONSTRAINT; y += 2 * EPE_CONSTRAINT)
    {
      index = EpeChecker::getIndex(m_x, y);
      matrix[index].r = matrix[index].i = 0;
    }
  }
  else  //VERTICAL
  {
    //for (int x = m_x - EPE_CONSTRAINT; x <= m_x + EPE_CONSTRAINT; ++x)  //count epe range
    //count only 2 points
    for (int x = m_x - EPE_CONSTRAINT; x <= m_x + EPE_CONSTRAINT; x += 2 * EPE_CONSTRAINT)
    {
      index = EpeChecker::getIndex(x, m_y);
      matrix[index].r = matrix[index].i = 0;
    }
  }
}

