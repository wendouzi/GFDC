
#include "Image.h"
#include "io.h"
#include "Algo.h"
#include <string>
#include <assert.h>

using namespace wendouzi;

int main(int argc, char * argv[])
{
    std::string filename = "/home/yangxianping/data/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473.tiff";
    wendouzi::Image_u16 img ;
    IO::RasterRead(filename, &img);
}