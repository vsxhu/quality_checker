#include <iostream>
#include <fstream>
#include <cctype>  //isdigit
#include "exception.h"
#include "design.h"
#include "shape.h"
using namespace std;

Design::Design(char *layoutFile)
        : m_layoutFile(layoutFile) {
    parse(layoutFile);
    polygon2rect(); //convert polygon to rect and store in m_rects
}

void
Design::parse(const char *layoutFile) {
#ifndef _DEBUG
    ios_base::sync_with_stdio(false); //to improve efficiency
#endif

    fstream file(layoutFile, fstream::in);
    if (!file.is_open())
        throw EXCEPTION_OPEN_DESIGN_ERROR;

    string token, dummy;
    while (file >> token) {
        if (token == "BEGIN") continue;
        else if (token == "EQUIV") {
            file >> m_scaleNumerator >> m_scaleDenominator >> m_unit >> dummy;
        } else if (token == "CNAME")
            file >> m_cellName;
        else if (token == "LEVEL")
            file >> m_layer;
        else if (token == "CELL") {
            file >> dummy >> dummy;
            string orient, layer;
            int llx, lly, width, height;
            int pointX, pointY;
            char head;
            while (file >> token) {
                if (token == "RECT") {
                    file >> orient >> layer >> llx >> lly >> width >> height;
                    addRect(llx, lly, llx + width, lly + height);
                } else if (token == "PGON") {
                    Polygon *polygon = addPolygon();
                    file >> orient >> layer;
                    while ((head = (file >> ws).peek())) {
                        if (isdigit(head)) {
                            file >> pointX >> pointY;
                            polygon->addPoint(pointX, pointY);
                        } else
                            break;
                    }
                } else if (token == "ENDMSG")
                    break;
            }
        }
    }
}

Design::~Design() {
//    should delete pointers, but it makes the program crashed
//    for (vector<Rect *>::iterator it = m_rects.begin(); it != m_rects.end(); ++it)
//        delete *it;
//    for (vector<Polygon *>::iterator it = m_polygons.begin(); it != m_polygons.end(); ++it)
//        delete *it;
//    for (vector<Rect *>::iterator it = m_maskRects.begin(); it != m_maskRects.end(); ++it)
//        delete *it;
}

void
Design::addRect(int llx, int lly, int urx, int ury) {
    Rect *rect = new Rect(llx, lly, urx, ury);
    m_rects.push_back(rect);
}

Polygon *
Design::addPolygon() {
    Polygon *polygon = new Polygon();
    m_polygons.push_back(polygon);
    return polygon;
}

void
Design::testParser() {
    for (vector<Rect *>::iterator it = m_rects.begin(); it != m_rects.end(); ++it)
        m_maskRects.push_back(*it);
}

void
Design::writeGlp(const char *outputFile) {
    fstream file(outputFile, fstream::out);

    file << "BEGIN" << endl;
    file << "EQUIV  " << m_scaleNumerator << " " << m_scaleDenominator << " "
         << m_unit << " +X,+Y" << endl;
    file << "CNAME " << m_cellName << endl;
    file << "LEVEL M1OPC" << endl << endl;

    file << "CELL " << m_cellName << " PRIME" << endl;
    for (vector<Rect *>::iterator it = m_maskRects.begin(); it != m_maskRects.end(); ++it) {
        file << "   RECT N M1OPC ";
        (*it)->print(file);
    }

    file << "ENDMSG" << endl;
}

//convert all polygons to non-overlapped rectangles and store into m_rects
void
Design::polygon2rect() {
    m_numTrueRects = m_rects.size();
    for (vector<Polygon *>::iterator it = m_polygons.begin(); it != m_polygons.end(); ++it) {
        const vector<Rect *> &rects = (*it)->convertRect();
        for (vector<Rect *>::const_iterator rit = rects.begin(); rit != rects.end(); ++rit)
            addRect(*rit);
    }
}
