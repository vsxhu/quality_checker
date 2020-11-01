#include <iostream>
#include <vector>
#include <set>
#include <cassert>
#include <iterator>
#include "shape.h"
#include "debug.h"

using namespace std;

//================== Coordinate ======================
void
Coordinate::print(ostream& out)
{
  out << x << " " << y << " ";
}

//================== Rect ======================
void
Rect::print(ostream& out)
{
  m_ll.print(out);
  out << m_ur.x - m_ll.x << " " << m_ur.y - m_ll.y << endl;
}

//================== Polygon ======================
Polygon::~Polygon()
{
  for (vector<Coordinate*>::iterator it = m_points.begin(); it != m_points.end(); ++it)
    delete (*it);

  //note the deletion of m_convertedRects will be controled by its caller
}

void
Polygon::addPoint(int x, int y)
{
  Coordinate* coord = new Coordinate(x, y);
  m_points.push_back(coord);
}

void
Polygon::print(ostream& out)
{
  for (vector<Coordinate*>::iterator it = m_points.begin(); it != m_points.end(); ++it)
    (*it)->print(out);
  out << endl;
}

vector<Rect*>&
Polygon::convertRect()
{
  set<int> hLevel, vLevel;
  int numPt = m_points.size();

  //find non-duplicated horizontal and vertical coordinate of the polygon
  //and sort in increasing order
  Coordinate* fromPt = m_points[numPt - 1];
  Coordinate* toPt;
  for (int i = 0; i < numPt; ++i)
  {
    toPt = m_points[i];
    if (fromPt->x == toPt->x) //horizontal edge
      vLevel.insert(fromPt->x);
      //inslvl(vlvl, &vcnt, fromPt.x);
    else if (fromPt->y == toPt->y)  //vertical edge
      hLevel.insert(fromPt->y);
      //inslvl(hlvl, &hcnt, fromPt.y);
    else {
//      wmesg("diagonal edge in polygon\n");
    }
    fromPt = toPt;
//    x1 = x2;
//    y1 = y2;
  }
#ifdef _DEBUG
//  for (auto it = vLevel.begin(); it != vLevel.end(); ++it)
//    cout << *it << " ";
//  cout << endl;
//  for (auto it = hLevel.begin(); it != hLevel.end(); ++it)
//    cout << *it << " ";
//  cout << endl;
#endif

  Coordinate corner1, corner2, center;
  for (set<int>::iterator vIt = vLevel.begin(), vNext = ++vLevel.begin();
      vNext != vLevel.end(); ++vIt, ++vNext)
    for (set<int>::iterator hIt = hLevel.begin(), hNext = ++hLevel.begin();
        hNext != hLevel.end(); ++hIt, ++hNext)
//  for(int i = 0; i< vcnt - 1; ++i)
//    for (int j = 0; j < hcnt - 1; ++j)
    {
      corner1.x = (*vIt);
      corner1.y = (*hIt);
      corner2.x = (*vNext);
      corner2.y = (*hNext);
      center.x = (corner1.x + corner2.x) / 2;
      center.y = (corner1.y + corner2.y) / 2;
      bool valid = pointInPolygon(center.x, center.y);
      if (valid)
      {
        assert(corner1.x < corner2.x && corner1.y < corner2.y);
        Rect* rect = new Rect(corner1.x, corner1.y, corner2.x, corner2.y);
        m_convertedRects.push_back(rect);
      }
      //if (p==1) er(l, xp1, yp1, xp2 - xp1, yp2 - yp1);
    }
  return m_convertedRects;
}

//check if point is inside polygon
bool
Polygon::pointInPolygon(int x, int y)
{
  int numPt = m_points.size();
  Coordinate* fromPt = m_points[numPt - 1];
  Coordinate* toPt;
  bool isOddNodes = false;
//  int  i, j=n-1 ;
//  int  oddNodes=0    ;
  for (int i = 0; i < numPt; ++i)
  {
    toPt = m_points[i];
    float fromX = fromPt->x, fromY = fromPt->y;
    float toX = toPt->x, toY = toPt->y;
    if (((fromY < y && toY >= y) || (toY < y && fromY >= y)) //vedge cover point
        && (fromX + (y - fromY) / (toY - fromY) * (toX - fromX) < x)
       )
      isOddNodes = (!isOddNodes);
//    if (Y(i)<F(y) && Y(j)>=F(y) ||  Y(j)<F(y) && Y(i)>=F(y))
//      if (X(i) + (F(y)-Y(i)) / (Y(j)-Y(i)) * (X(j)-X(i)) < F(x) )
//        oddNodes=!oddNodes;

    fromPt = toPt;
  }
  return isOddNodes;
}

