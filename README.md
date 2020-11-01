## Dependencies ##
- BLAS
- zlib
- libpng

## Build ##
Under the project root directory
```bash
mkdir build
cd build
cmake ..
make
```

## Run ##
Before running the binary in runtime directory, please specify the following parameters:
 1. **EPE_OFFSET_X** in src/epeChecker.h, default value is 512
 2. **EPE_OFFSET_Y** in src/epeChecker.h, default value is 512
 3. **MASK_BG_COLOR** in src/Checker.h , which is corresponding to the mask background RGB value, default value is (0,0,0)
 4. **EPEConstraint** in thirdparty/pvbandsim/writer.h if you want to draw an EPE image, default value is 10
 5. **EPE_CONSTRAINT** in src/epeChecker.h, which determines the EPE violation threshold and it should be consistent with EPEConstraint

Under the project root directory
```bash
cd runtime
ln -s ../res/Kernels/
./quality_checker -g <path to mask image1> -b <path to mask image2>-g <path to glp file>
```