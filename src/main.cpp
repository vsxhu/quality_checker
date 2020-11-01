#include <iostream>
#include <unistd.h>
#include "lodepng.h"
#include "Checker.h"

using namespace std;
void printUsage();
/**
 * Receives two mask images and outputs the pvband (image) and epe (image)
 *
 * Befor running, please specify the following parameters:
 * 1. EPE_OFFSET_X in src/epeChecker.h, default value is 512
 * 2. EPE_OFFSET_Y in src/epeChecker.h, default value is 512
 * 3. MASK_BG_COLOR in src/Checker.h , which is corresponding to the mask background RGB value, default value is (0,0,0)
 * 4. EPEConstraint in thirdparty/pvbandsim/writer.h if you want to draw an EPE image, default value is 10
 * 5. EPE_CONSTRAINT in src/epeChecker.h, which determines the EPE violation threshold and it should be consistent with EPEConstraint
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char* argv[]) {
    int ch;
    string img_msk1;
    string img_msk2;
    string glp_file;
    while ((ch = getopt(argc, argv, "a:b:g:")) != -1){
        switch (ch){
            case 'a':
                img_msk1 = optarg;
                break;
            case 'b':
                img_msk2 = optarg;
                break;
            case 'g':
                glp_file = optarg;
                break;
            default:
                break;
        }
    }
    if(img_msk1.empty() || img_msk2.empty() || glp_file.empty()){
        printUsage();
        return 1;
    }
    Design design((char*) glp_file.c_str());
    Checker checker(design);
    checker.read(img_msk1, img_msk2);
    checker.run();
    return 0;
}


void printUsage(){
    cout << "----- Quality Checker -----\n"
         << "Usage: \n"
         << "-a <path to mask image1>\n"
         << "-b <path to mask image2>\n"
         << "-g <path to glp file>"
         << endl;
}