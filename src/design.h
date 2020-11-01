#ifndef _DESIGN_H_
#define _DESIGN_H_

#include <vector>
#include <string>

class Rect;

class Polygon;

class Design {
public:
    Design(char *layoutFile);

    ~Design();

    const std::vector<Rect *> &getRects() const { return m_rects; }

    const std::vector<Polygon *> &getPolygons() const { return m_polygons; }

    std::vector<Rect *> &getMaskRects() { return m_maskRects; }

    int getNumTrueRects() const { return m_numTrueRects; }

    char *getLayoutFile() const { return m_layoutFile; }

    void testParser();  //debug only
    void writeGlp(const char *outputFile);

private:
    void parse(const char *layoutFile);

    void polygon2rect();

    void addRect(int llx, int lly, int urx, int ury);

    void addRect(Rect *rect) { m_rects.push_back(rect); }

    void addHananRect(int llx, int lly, int urx, int ury);

    Polygon *addPolygon();

    std::vector<Rect *> m_rects;
    std::vector<Polygon *> m_polygons;
    std::vector<Rect *> m_maskRects;
    char *m_layoutFile;
    int m_numTrueRects;
    int m_scaleNumerator;
    int m_scaleDenominator;
    std::string m_unit;
    std::string m_cellName;
    std::string m_layer;
    int m_minWidth;
};

#endif
