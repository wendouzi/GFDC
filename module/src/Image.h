#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include "gdal_priv.h"
#include "gdal_alg.h"
#include "cpl_conv.h"

namespace wendouzi{

class Algo;
template<typename T>
class Image {
    typedef T* Band;
    typedef T  BandType;
    friend class Algo;
public:
    Image(int w = 0, int h = 0, int b = 0): width(w), height(h), bandsNum(b){
        NXLog("%s\n", __PRETTY_FUNCTION__);
        assert(bandNum == bands.size());
        for (int idx = 0; idx < bandNum; idx++) {
            Band b = (Band) CPLMalloc(sizeof(BandType)* width * height);
            if (b == NULL){
                assert(false);
            }
            memset(bands[idx], 0, height * width);            
            bands.push_back(b);
        }
    }

    void init(int w, int h, int b) {
        NXLog("%s\n", __PRETTY_FUNCTION__);
        if (b == bandNum && width == w && height == h) {
            NXLog("%s\n same size with old", __PRETTY_FUNCTION__);
            for(int idx = 0; idx < bandNum; ++idx)
            {
                memset(bands[idx], 0, height * width);
            }
            return;
        }
        clear();
        width = w;
        height = h;
        bandNum = b;
        for (int idx = 0; idx < bandNum; idx++) {
            Band b = (Band) CPLMalloc(sizeof(BandType)* width * height);
            if (b == NULL){
                assert(false);
            }
            memset(bands[idx], 0, height * width);            
            bands.push_back(b);
        }
    }

    T* getBandPtr(const int idx) {
        if (idx >= bandsNum) {
            return NULL;
        }
        return bands[idx];
    }

    void clear()
    {
        NXLog("%s", __PRETTY_FUNCTION__);
        for (int idx = 0; idx < bandNuml; ++idx) {
            if (bands[idx] != NULL) {
                CPLFree(bands[idx]);
                bands[idx] = NULL;
            }
        }
        bands.clear();
    }

    ~Image(){ 
        NXLog("%s", __PRETTY_FUNCTION__);
        assert(bandNum == bands.size());
        clear();
    }
protected:

    std::vector<Band> bands;
    int width;
    int height;
    int bandsNum;

};

typedef Image<float> Image_f;
typedef Image<uint16_t> Image_u16;
typedef Image<int> Image_int;

}



#endif