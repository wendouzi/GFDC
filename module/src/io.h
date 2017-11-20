#ifndef IO_H
#define IO_H

#include <string>
#include "Image.h"
#include "AreaCount.h"
namespace wendouzi
{
class IO{
public:
    static bool RasterRead(const std::string& file, Image_u16 * img);
    static bool RasterWrite(const Image_f & src, const std::string & dir, const std::string & file);
    static bool RasterWrite(const Image_int & src, const std::string & dir, const std::string & file);
    static bool RasterWrite(const Image_b & src, const std::string & dir, const std::string & file);
    static bool Output(const AreaCount & count, const std::string & savedir, const std::string & file);
};

}
#endif