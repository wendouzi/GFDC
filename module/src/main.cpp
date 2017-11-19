
#include "Image.h"
#include "io.h"
#include "Algo.h"
#include <string>
#include <assert.h>
#include "setting.h"
#include "config.h"

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

    wendouzi::Image_f density;
    wendouzi::Algo::cal_density(refImg, density);

    wendouzi::IO::RasterWrite(density, st.destdir);

}