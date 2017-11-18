#ifndef ALGO_H
#define ALGO_H

#include "image.h"

namespace wendouzi {
class Algo {
class Image;
bool cal_density(const Image & src, Image & result);
bool cal_density(const Image & src, Image & result, const Image & distance);
};


}


#endif