#ifndef IO_H
#define IO_H

#include <string>
#include "Image.h"
namespace wendouzi
{
class IO{
public:
    static bool RasterRead(const std::string& file, Image_u16 * img);
    static bool RasterWrite(const Image_f & src, const std::string & dir);
};

}
#endif