#ifndef AREACOUNT_H
#define AREACOUNT_H
#include <map>
namespace wendouzi
{
class IO;
class AreaCount {
    friend class IO;
public:
    std::map<int, int> result;
    float pixelArea = 16.0f * 16.0f;
    void setPixSize(float ar)
    {
        pixelArea = ar;
    }
};
}


#endif