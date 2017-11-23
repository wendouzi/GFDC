#ifndef ALGO_H
#define ALGO_H

#include "Image.h"
#include "AreaCount.h"
namespace wendouzi {


// (DN-offset)/scale;    
class Image;
class Algo {
public:
// as the equation radio =  (input - offset) * scale 
static bool correct_radio(const Image_u16 & src, Image_f & result, std::vector<float> d_scale, std::vector<float> d_offset);
// as the equation reflectance = radio / E0;
static bool radio_to_reflectance(const Image_f & src, Image_f & result, std::vector<float> d_E0);


enum Method 
{
    withoutDist = 0,
    withDist,
    SVI_DIST,
};

enum DistAlgo
{
    Pixel2Water = 0,
    Water2Pixel,
    POI_SVI_DIST
};

static bool cal_NDVI(const Image_f & src, Image_f & result);
static bool cal_NDWI(const Image_f & src, Image_f & result);
//! threshold is required for identify the water pixel
#define NDWI_WATERINDEX (0)
static bool cal_WI(const Image_f & src, Image_b & result, float threshold = NDWI_WATERINDEX);

#define RATIO43_WATERINDEX (3.5)
static bool cal_Mask(const Image_f & src, Image_b & result);

static bool cal_KT(const Image_f & src, Image_f & result);
static bool cal_SVI(const Image_f & src, Image_f & result);
// static bool cal_Shade(const Image_f & src, Image_f & result);

#define FILLVALUE 1000
#define DIST_FILLVALUE 1000
#define NEAR_POINTS_NUM 300
#define WATERVALUE   (-1)

static bool cal_distance(const Image_f & src, Image_f & result, const  Image_b & mask, bool isAddMask = false,  float fillvalue = DIST_FILLVALUE, DistAlgo dia = Water2Pixel);

static bool cal_density(const Image_f & src, Image_f & result, Method me = withDist);
static bool cal_density(const Image_f & src, Image_f & result, const Image_f & distance);
static bool cal_density(const Image_f & src, Image_f & result, const Image_b & mask, const Image_f & distance, const Image_f & svi, float fillvalue = -1.0f);
#define DENSITY_MINI   float(-2)
#define DENSITY_MAXI   float(8)
static bool cal_density(const Image_f & src, Image_f & result, const Image_b & mask, const Image_f & distance, const Image_f & ndvi, const Image_f & kt, 
    float fillvalue = DENSITY_MINI, float minvalue = DENSITY_MINI, float maxvalue = DENSITY_MAXI);

#define LEVEL_FILLVALUE int(-4)
static bool cal_level(const Image_f & density, Image_int & result, const Image_b & mask, int levelNum = 4, float minvalue = DENSITY_MINI, float maxvalue = DENSITY_MAXI, int fillvalue = LEVEL_FILLVALUE);

// static bool area_count(const Image_int & level, AreaCount & count, const Image_b & mask, int levelNum = 4, float minvalue = DENSITY_MINI, float maxvalue = DENSITY_MAXI, int fillvalue = LEVEL_FILLVALUE);
static bool area_count(const Image_int & level, AreaCount & count, const Image_b & mask);

};


}


#endif