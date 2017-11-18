#ifndef CONFIG_H
#define CONFIG_H


float scale[] = { 5.851, 7.153, 8.368, 7.474, 6.014 , 6.823, 9.451,   8.996, 5.82, 6.239, 7.010,  7.711, 5.35, 6.235, 6.992, 7.462};
// scale WEV1 band1, band2, band3, band4; WFV2 band1, band 2......
float offset[] = {0.0039, 0.0047, 0.003,   0.0274, 0.0125, 0.0193, 0.0429,  0.011, 0.0071, 0.0334,0.0226, 0.0117,0.0369,   0.0235, 0.0217,  0.005};
// offset WEV1 band1, band2, band3, band4; WFV2 band1, band 2......

float E0[] = { 1966.808, 1822.607, 1523.205, 1066.549};
// (DN-offset)/scale;

// ground resolution
const int GRes = 8;
const float FIRST_DEGREE = 0.001;
const float DIST_LIMIT_DEGREE = 0.0001;
const int STEP = 50;
const int JUMP_NUM = 0;
const int INPUT_COLS = 4;
const int WANTED_PARAM = 4; // band1, band2, band3, band4,ndvi,dis,wi, bright, wet, green
#endif // CONFIG_H
