#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include "gdal_priv.h"
#include "gdal_alg.h"
#include "cpl_conv.h"
#include <stdint.h>
#include "NXLog.h"
#include <assert.h>

namespace wendouzi{
class Algo;
template<typename T>
class _Image {
    typedef T* Band;
    typedef T  BandType;
    friend class Algo;
    friend class IO;
public:
    _Image(int w = 0, int h = 0, int b = 0): width(w), height(h), bandNum(b){
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
            Band b = (Band) CPLMalloc(sizeof(BandType) * width * height);
            if (b == NULL){
                assert(false);
            }
            memset(b, 0, sizeof(BandType)* height * width);            
            bands.push_back(b);
        }
    }

    void setGeoTransform(const std::vector<double> & g){
        this->adfGeoTransform = g;
    }

    std::vector<double> getGeoTransform() const
    {
        return adfGeoTransform;
    }


    void fillValue(BandType val)
    {
        for (int idx = 0; idx < bandNum; ++idx) {
            Band b = getBandPtr(idx);
            if (!b) {
                NXLog("%s fillValue error\n", __PRETTY_FUNCTION__);
                return;
            }
            memset(b, val, sizeof(BandType)* height * width);
        }
    }

    T* getBandPtr(const int idx) const {
        if (idx >= bandNum) {
            return NULL;
        }
        return bands[idx];
    }

    void clear()
    {
        NXLog("%s\n", __PRETTY_FUNCTION__);
        assert(bandNum == bands.size());        
        for (int idx = 0; idx < bandNum; ++idx) {
            if (bands[idx] != NULL) {
                CPLFree(bands[idx]);
                bands[idx] = NULL;
            }
        }
        bands.clear();
    }

    ~_Image(){ 
        NXLog("%s\n", __PRETTY_FUNCTION__);
        assert(bandNum == bands.size());
        clear();
    }

    // T* operator [](const int idx) {
    //     return getBandPtr(idx);
    // }
protected:

    std::vector<Band> bands;
    int width;
    int height;
    int bandNum;
    std::vector<double> adfGeoTransform;

};

typedef _Image<float> Image_f;
typedef _Image<uint16_t> Image_u16;
typedef _Image<int> Image_int;
typedef _Image<bool> Image_b;

}



#endif