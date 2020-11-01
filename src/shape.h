#ifndef _SHAPE_H_
#define _SHAPE_H_
#include <vector>
#include <iostream>

class Coordinate
{
public:
  Coordinate() {}
  Coordinate(int coordX, int coordY) : x(coordX), y(coordY) {}

  //copy constructor
  Coordinate(const Coordinate& that)
  {
    x = that.x;
    y = that.y;
  }

  void print(std::ostream& out = std::cout);

  int x;
  int y;
};

class Rect
{
public:
  Rect(int llx, int lly, int urx, int ury)
    : m_ll(llx, lly)
    , m_ur(urx, ury)
  {}

  //copy constructor
  Rect(const Rect& that)
    : m_ll(that.m_ll)
    , m_ur(that.m_ur)
  {}

  const Coordinate& ll() { return m_ll; }
  const Coordinate& ur() { return m_ur; }

  void print(std::ostream& out = std::cout);

private:
  Coordinate m_ll;  //lowerleft
  Coordinate m_ur;  //upperrightgg
};

class Polygon
{
public:
  Polygon() {}
  ~Polygon();

  const std::vector<Coordinate*>& getPoints() const { return m_points; }

  void addPoint(int x, int y);
  void print(std::ostream& out = std::cout);
  std::vector<Rect*>& convertRect();
  bool pointInPolygon(int x, int y);

private:
  std::vector<Coordinate*> m_points;
  std::vector<Rect*> m_convertedRects;
};

class Grid{
public:
    Grid(int llx, int lly, int urx, int ury, bool isPattern, int id)
            : m_ll(llx, lly)
            , m_ur(urx, ury)
            , isPattern(isPattern)
            , id(id)
    {}
    const Coordinate& ll() { return m_ll; }
    const Coordinate& ur() { return m_ur; }
    const bool getIsPattern() { return isPattern;}
    const int getId() { return  id;}

private:
    Coordinate m_ll;  //lowerleft
    Coordinate m_ur;  //upperrightgg
    bool isPattern;
    int id;
};

#endif

