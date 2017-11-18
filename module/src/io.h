#ifndef IO_H
#define IO_H

#include <string>

namespace wendouzi
{
class Image;
class IO{
public:
    static bool RasterRead(const std::string& file, const Image_u16 * img);
};

}
#endif