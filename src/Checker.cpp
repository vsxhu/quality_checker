#include "Checker.h"

Checker::Checker(Design &design) : design(design) {
    msk1 = new float[MASK_SIZE];
    msk2 = new float[MASK_SIZE];
    intensity1 = new float[MASK_SIZE];
    intensity2 = new float[MASK_SIZE];
    intensityInner1 = new float[MASK_SIZE];
    intensityInner2 = new float[MASK_SIZE];
    intensityOuter1 = new float[MASK_SIZE];
    intensityOuter2 = new float[MASK_SIZE];
    aerialNorminal =  new float[MASK_SIZE];
    aerialInner = new float[MASK_SIZE];
    aerialOuter = new float[MASK_SIZE];
    memset(intensity1, 0, sizeof(float) * MASK_SIZE);
    memset(intensity2, 0, sizeof(float) * MASK_SIZE);
    memset(intensityInner1, 0, sizeof(float) * MASK_SIZE);
    memset(intensityInner2, 0, sizeof(float) * MASK_SIZE);
    memset(intensityOuter1, 0, sizeof(float) * MASK_SIZE);
    memset(intensityOuter2, 0, sizeof(float) * MASK_SIZE);
    memset(aerialNorminal, 0, sizeof(float) * MASK_SIZE);
    memset(aerialInner, 0, sizeof(float) * MASK_SIZE);
    memset(aerialOuter, 0, sizeof(float) * MASK_SIZE);
    epeChecker.setDesign(design);
}

Checker::~Checker() {
    delete msk1;
    delete msk2;
    delete intensity1;
    delete intensity2;
    delete intensityInner1;
    delete intensityInner2;
    delete intensityOuter1;
    delete intensityOuter2;
    delete aerialNorminal;
    delete aerialInner;
    delete aerialOuter;
}

void Checker::run() {
    lithosim.simulateImageOpt(msk1, intensity1, LithosimWrapper::LITHO_KERNEL_FOCUS, NOMINAL_DOSE);
    lithosim.simulateImageOpt(msk2, intensity2, LithosimWrapper::LITHO_KERNEL_FOCUS, NOMINAL_DOSE);
    mergeImg(intensity1, intensity2, aerialNorminal);
    int epe = get_epe(intensity1, intensity2);

    lithosim.simulateImageOpt(msk1, intensityInner1, LithosimWrapper::LITHO_KERNEL_FOCUS, MIN_DOSE);
    lithosim.simulateImageOpt(msk2, intensityInner2, LithosimWrapper::LITHO_KERNEL_FOCUS, MIN_DOSE);
    mergeImg(intensityInner1, intensityInner2, aerialInner);

    lithosim.simulateImageOpt(msk1, intensityOuter1, LithosimWrapper::LITHO_KERNEL_FOCUS, MAX_DOSE);
    lithosim.simulateImageOpt(msk2, intensityOuter2, LithosimWrapper::LITHO_KERNEL_FOCUS, MAX_DOSE);
    mergeImg(intensityOuter1, intensityOuter2, aerialOuter);

    int pvband = lithosim.calculatePvband(aerialInner, aerialOuter);
    lithosim.writePvband("pvband.png", aerialInner, aerialOuter);
    lithosim.writeImageEpePng("EPE.png", aerialNorminal, NOMINAL_DOSE, design.getLayoutFile());
    std::cout << "EPE:" << epe << std::endl
              << "PV Band: " << pvband << std::endl;
}

/**
 * read image file and assign values to different masks
 * we assume the image size is 2048*2048
 * @param msk_img1: path to the png file
 */
void Checker::read(std::string &msk_img1, std::string &msk_img2) {
    memset(msk1, 0, sizeof(float) * MASK_SIZE);
    memset(msk2, 0, sizeof(float) * MASK_SIZE);
    readMask(msk_img1, msk1);
    readMask(msk_img2, msk2);
}

int Checker::get_epe(float *img1, float *img2) {
    auto *aerial = new float[MASK_SIZE];
    memset(aerial, 0, sizeof(float) * MASK_SIZE);
    mergeImg(img1, img2, aerial);
    int ret = epeChecker.run(aerial);
    delete aerial;
    return ret;
}

void Checker::mergeImg(float *img1, float *img2, float *target) const {
    for (int i = 0; i < MASK_SIZE; i++) {
        if (img1[i] >= TARGET_INTENSITY || img2[i] >= TARGET_INTENSITY) {
            target[i] = 1;
        }
    }
}

void Checker::readMask(string &file_name, float *msk) {
    int width, height, rowbytes, channels;
    png_byte color_type;
    png_byte bit_depth;

    png_structp png_ptr;
    png_infop info_ptr;
    int number_of_passes;
    png_bytep * row_pointers;

    unsigned char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name.c_str(), "rb");
    if (!fp){
        cout << "can't open file" << endl;
        exit(1);
    }

    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)){
        cout << "Not a png file" << endl;
        exit(1);
    }


    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr){
        cout << "create png structure failed" << endl;
        exit(1);
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr){
        cout << "create info structure failed " << endl;
        exit(1);
    }

    if (setjmp(png_jmpbuf(png_ptr))){
        cout << "error in init_io" << endl;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    channels = png_get_channels(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr))){
        cout << "error in read_image" << endl;
    }

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

    if (bit_depth == 16)
        rowbytes = width*8;
    else
        rowbytes = width*4;
    for (int y=0; y<height; y++)
        row_pointers[y] = (png_byte*) malloc(rowbytes);

    png_read_image(png_ptr, row_pointers);

    if(color_type == PNG_COLOR_TYPE_RGB){
        int rgb_width = 3 * width;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < rgb_width; j+= 3){
                if(row_pointers[i][j]  != MASK_BG_COLOR.r || row_pointers[i][j+1] != MASK_BG_COLOR.b || row_pointers[i][j+2] != MASK_BG_COLOR.g)
                    msk[getIndex(i, j/3)] = 1;
            }
        }
    } else{

    }
    png_destroy_read_struct(&png_ptr, &info_ptr, 0);
    fclose(fp);
}

