
#include "Image.h"
#include "io.h"
#include "Algo.h"
#include <string>
#include <assert.h>
#include "setting.h"
#include "config.h"
#include "AreaCount.h"
// using namespace wendouzi;

int main(int argc, char * argv[])
{
    wendouzi::setting st;
    st.config(argc, argv);
    st.check();
    wendouzi::Image_u16 originImg ;
    wendouzi::IO::RasterRead(st.srcfile, &originImg);
    std::vector<float> _scale = {config::gfscale[0], config::gfscale[1], config::gfscale[2], config::gfscale[3]};
    std::vector<float> _offset = {config::gfoffset[0], config::gfoffset[1], config::gfoffset[2], config::gfoffset[3]};
    std::vector<float> _E0 = {config::gfE0[0], config::gfE0[1], config::gfE0[2], config::gfE0[3]};

    wendouzi::Image_f radioImg;
    wendouzi::Algo::correct_radio(originImg, radioImg, _scale, _offset);

    wendouzi::Image_f refImg;
    wendouzi::Algo::radio_to_reflectance(radioImg, refImg, _E0);
    wendouzi::IO::RasterWrite(refImg, st.destdir, "ref.tiff");    

    wendouzi::Image_b maskImg;
    wendouzi::Algo::cal_Mask(refImg, maskImg);

    wendouzi::Image_b wiImg;
    wendouzi::Algo::cal_WI(refImg, wiImg);
    wendouzi::IO::RasterWrite(wiImg, st.destdir, "wi.tiff");

    wendouzi::Image_f ktImg;
    wendouzi::Algo::cal_KT(refImg, ktImg);

    wendouzi::Image_f sviImg;
    wendouzi::Algo::cal_SVI(refImg, sviImg);

    wendouzi::Image_f ndviImg;
    wendouzi::Algo::cal_NDVI(refImg, ndviImg);

    wendouzi::Image_f distanceImg;
    wendouzi::Algo::cal_distance(refImg, distanceImg, maskImg, false, DIST_FILLVALUE);

    wendouzi::IO::RasterWrite(maskImg, st.destdir, "mask.tiff");
    wendouzi::IO::RasterWrite(distanceImg, st.destdir, "distance.tiff");

    wendouzi::Image_f densityImg;
    // cal_density(const Image_f & src, Image_f & result, const Image_b & mask, const Image_f & distance, const Image_f & ndvi, const Image_f & kt);
    // wendouzi::Algo::cal_density(refImg, densityImg, maskImg, distanceImg, ndviImg, ktImg);
    wendouzi::Algo::cal_density(refImg, densityImg, maskImg, distanceImg, sviImg);
    wendouzi::IO::RasterWrite(densityImg, st.destdir, "density.tiff");

    wendouzi::Image_int level;
    // static bool cal_level(const Image_f & density, Image_int & result, const Image_b & mask, int levelNum = 4, float minvalue = DENSITY_MINI, float maxvalue = DENSITY_MAXI, int fillvalue = LEVEL_FILLVALUE);
    wendouzi::Algo::cal_level(densityImg, level, maskImg, 3, 0, 3, -1);
    wendouzi::IO::RasterWrite(level, st.destdir, "level.tiff");

    wendouzi::AreaCount count;
    count.setPixSize(100.0f);
    wendouzi::Algo::area_count(level, count, maskImg);
    wendouzi::IO::Output(count, st.destdir, "areaCount.txt");
}