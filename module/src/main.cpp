
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

    std::string srcfile = st.srcfile;
    boost::filesystem::path savedir(st.destdir);

    if (!st.shapefile.empty()) {
        boost::filesystem::path tempfile = savedir;
        tempfile /= boost::filesystem::path(st.srcfile).stem();
        tempfile = boost::filesystem::path(tempfile.string() + "_shp_cut.tiff");
        std::string cmd = "";
#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS)
        cmd += ".\gdalwarp ";
#else 
        cmd += "./gdalwarp ";
#endif
        boost::filesystem::path projfile = boost::filesystem::path(st.shapefile).parent_path();
        projfile /= boost::filesystem::path(st.shapefile).stem();
        projfile = boost::filesystem::path(projfile.string() + ".prj");
        NXLog("projfile :%s\n", projfile.c_str());
        if (boost::filesystem::exists(projfile)) {
            cmd += " -t_srs ";
            cmd += projfile.string();
        }
        cmd += " -cutline ";
        cmd += st.shapefile;
        cmd += " -crop_to_cutline ";
        cmd += " -of GTiff ";
        cmd += srcfile;
        cmd += " ";
        cmd += tempfile.string();
        NXLog("shapefile cutline:%s\n", cmd.c_str());
        system(cmd.c_str());
        fflush(stdout);
        srcfile = tempfile.string();
    }

    if (st.m_range.size() == 4) {
        boost::filesystem::path tempfile = savedir;
        tempfile /= boost::filesystem::path(srcfile).stem();
        tempfile = boost::filesystem::path(tempfile.string() + "_rect_cut.tiff");
        std::string cmd = "";
#if defined(WIN32) || defined(_WIN32) || defined(_WINDOWS)
        cmd += ".\gdalwarp ";
#else 
        cmd += "./gdalwarp ";
#endif
        cmd += " -te ";
        for (int idx = 0; idx < 4; ++idx) {
            cmd += std::to_string(st.m_range[idx]);
            cmd += " ";
        }
        cmd += " -of GTiff ";
        cmd += srcfile;
        cmd += " ";
        cmd += tempfile.string();
        system(cmd.c_str());
        fflush(stdout);
        srcfile = tempfile.string(); 
        NXLog("range cut:%s\n", cmd.c_str());
        
    }

    wendouzi::Image_u16 originImg ;
    wendouzi::IO::RasterRead(srcfile, &originImg);
    std::vector<float> _scale = {config::gfscale[0], config::gfscale[1], config::gfscale[2], config::gfscale[3]};
    std::vector<float> _offset = {config::gfoffset[0], config::gfoffset[1], config::gfoffset[2], config::gfoffset[3]};
    std::vector<float> _E0 = {config::gfE0[0], config::gfE0[1], config::gfE0[2], config::gfE0[3]};

    wendouzi::Image_f radioImg;
    wendouzi::Algo::correct_radio(originImg, radioImg, _scale, _offset);

    wendouzi::Image_f refImg;
    wendouzi::Algo::radio_to_reflectance(radioImg, refImg, _E0);
    // wendouzi::IO::RasterWrite(refImg, st.destdir, "ref.tiff");

    wendouzi::Image_b maskImg;
    wendouzi::Algo::cal_Mask(refImg, maskImg);

    wendouzi::Image_b wiImg;
    wendouzi::Algo::cal_WI(refImg, wiImg);
    // wendouzi::IO::RasterWrite(wiImg, st.destdir, "wi.tiff");

    wendouzi::Image_f ndviImg;
    wendouzi::Algo::cal_NDVI(refImg, ndviImg);
    if (st.m_products[int(wendouzi::setting::ndvi)]) {
        wendouzi::IO::RasterWrite(ndviImg, st.destdir, "ndvi.tiff");
    }

    wendouzi::Image_f ndwiImg;
    wendouzi::Algo::cal_NDWI(refImg, ndwiImg);
    if (st.m_products[int(wendouzi::setting::ndwi)]) {
        wendouzi::IO::RasterWrite(ndwiImg, st.destdir, "ndwi.tiff");
    }

    wendouzi::Image_f sviImg;
    wendouzi::Algo::cal_SVI(refImg, sviImg);
    if (st.m_products[int(wendouzi::setting::svi)]) {
        wendouzi::IO::RasterWrite(sviImg, st.destdir, "svi.tiff");
    }
 
    wendouzi::Image_f distanceImg;
    wendouzi::Algo::cal_distance(refImg, distanceImg, maskImg, false, DIST_FILLVALUE);
    if (st.m_products[int(wendouzi::setting::distance)]) {
        wendouzi::IO::RasterWrite(distanceImg, st.destdir, "distance.tiff");
    }

    wendouzi::IO::RasterWrite(maskImg, st.destdir, "mask.tiff");

    wendouzi::Image_f ktImg;
    wendouzi::Algo::cal_KT(refImg, ktImg);

    wendouzi::Image_f densityImg;
    // cal_density(const Image_f & src, Image_f & result, const Image_b & mask, const Image_f & distance, const Image_f & ndvi, const Image_f & kt);
    // wendouzi::Algo::cal_density(refImg, densityImg, maskImg, distanceImg, ndviImg, ktImg);
    wendouzi::Algo::cal_density(refImg, densityImg, maskImg, distanceImg, sviImg, -1.0f);
    if (st.m_products[int(wendouzi::setting::density)]) {
        wendouzi::IO::RasterWrite(densityImg, st.destdir, "density.tiff");
    }


    wendouzi::Image_int level;
    // static bool cal_level(const Image_f & density, Image_int & result, const Image_b & mask, int levelNum = 4, float minvalue = DENSITY_MINI, float maxvalue = DENSITY_MAXI, int fillvalue = LEVEL_FILLVALUE);
    wendouzi::Algo::cal_level(densityImg, level, maskImg, 3, 0, 3, -1);
    wendouzi::IO::RasterWrite(level, st.destdir, "level.tiff");

    if (st.m_products[int(wendouzi::setting::level)]) {
        wendouzi::IO::RasterWrite(level, st.destdir, "level.tiff");
    }

    wendouzi::AreaCount count;
    count.setPixSize(16* 16.0f);
    wendouzi::Algo::area_count(level, count, maskImg);
    wendouzi::IO::Output(count, st.destdir, "areaCount.txt");
}