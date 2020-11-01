
#ifndef QUALITY_CHECKER_CHECKER_H
#define QUALITY_CHECKER_CHECKER_H

#include "design.h"
#include "lodepng.h"
#include "lithosimWrapper.h"
#include "epeChecker.h"
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include <png.h>

using namespace std;

class Checker {
    typedef struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } pngcolor;
public:
    Checker(Design& design);
    ~Checker();
    void run();
    void read(std::string & msk_img1, std::string &msk_img2);
    int get_epe(float* img1, float* img2);

private:
    int getIndex(int x, int y) { return (x << 11) + y; }
    void mergeImg(float* img1, float* img2, float* target) const;
    void readMask(string& filename, float* msk);

private:
    Design design;
    const float NOMINAL_DOSE = 1;
    EpeChecker epeChecker;
    LithosimWrapper lithosim;
    float* msk1;
    float* msk2;
    float* intensity1;
    float* intensity2;
    float* intensityInner1;
    float* intensityInner2;
    float* intensityOuter1;
    float* intensityOuter2;
    float* aerialNorminal;
    float* aerialInner;
    float* aerialOuter;
    const int MASK_SIZE = 1 << 22;
    const pngcolor MASK_BG_COLOR = {r:0, g:0, b:0};
};


#endif //QUALITY_CHECKER_CHECKER_H
