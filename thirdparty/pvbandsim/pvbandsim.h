#ifndef _PVBANDSIM_H_
#define _PVBANDSIM_H_

//below copied from pvbandsim.c
int MAX_FILE_NAME_LENGTH = 80;
//below setting for binary mask
float BACKGROUND_REAL = 0.0;
float BACKGROUND_IMAG = 0.0;
float MASK_REAL = 1.0;
float MASK_IMAG = 0.0;
//below setting for attenuated mask
//float BACKGROUND_REAL = 0.0229;
//float BACKGROUND_IMAG = -0.0217;
//float MASK_REAL = 0.9771;
//float MASK_IMAG = 0.0217;

float TARGET_INTENSITY = 0.039;
float MAX_DOSE = 1.02;
float MIN_DOSE = 0.98;
//===============

#endif

