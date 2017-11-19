#include "Algo.h"
#include "NXLog.h"
namespace wendouzi
{


bool Algo::correct_radio(const Image_u16 & src, Image_f & result, std::vector<float> d_scale, std::vector<float> d_offset)
{
    NXLog("%s\n", __PRETTY_FUNCTION__);
    if (d_scale.size() != src.bandNum || d_offset.size() != src.bandNum)
    {
        NXLog("scale or offset size is not equal bandnum\n");
        return false;
    }

    result.init(src.width, src.height, src.bandNum);
    for(int idx = 0; idx < src.bandNum; ++idx){
        float _scale = d_scale[idx];
        float _offset = d_offset[idx];
        int _width = src.width;
        int _height = src.height;
        uint16_t * srcband = src.getBandPtr(idx);
        float* resband = result.getBandPtr(idx);
        for(int i = 0; i < _height*_width; i++)
        {
            resband[i] = (srcband[i] - _offset) * _scale;
        }
    }
    return true;
}


bool Algo::radio_to_reflectance(const Image_f & src, Image_f & result, std::vector<float> E0)
{
    NXLog("%s\n", __PRETTY_FUNCTION__);
    if (E0.size() != src.bandNum )
    {
        NXLog("E0 size is not equal bandnum\n");
        return false;
    }
    result.init(src.width, src.height, src.bandNum);
    for(int idx = 0; idx < src.bandNum; ++idx) {
        float _E0 = E0[idx];
        int _width = src.width;
        int _height = src.height;
        float * srcband = src.getBandPtr(idx);
        float* resband = result.getBandPtr(idx);
        for(int i = 0; i < _height*_width; i++)
        {
            resband[i] = srcband[i] / _E0;
        }
    }
    return true;
}

bool Algo::cal_NDVI(const Image_f & src, Image_f & result)
{
    NXLog("%s\n", __PRETTY_FUNCTION__);
    result.init(src.width, src.height, src.bandNum);
    int _width = src.width;
    int _height = src.height;
    float * band4 = src.getBandPtr(3);
    float * band3 = src.getBandPtr(2);
    float * ndviband = result.getBandPtr(0);
    if (band4 == NULL || band3 == NULL || ndviband == NULL) {
        NXLog("%s cal_ndvi error\n", __PRETTY_FUNCTION__);
        return false;
    }
    for(int i = 0; i < _width * _height; i++)
    {
        ndviband[i] = (band4[i]-band3[i])/(band4[i]+band3[i]);
        if(ndviband[i] < -2) {
            ndviband[i] = -2.0;
        }
    }
    return true;
}

bool Algo::cal_NDWI(const Image_f & src, Image_f & result)
{
    assert(false);
    return false;
}

bool Algo::cal_SVI(const Image_f & src, Image_f & result)
{
    NXLog("%s\n", __PRETTY_FUNCTION__);
    result.init(src.width, src.height, src.bandNum);
    Image_f ndvi;
    cal_NDVI(src, ndvi);
    int _width = src.width;
    int _height = src.height;
    float * ndviband = ndvi.getBandPtr(0);
    float * band4 = src.getBandPtr(3);
    float * sviband = result.getBandPtr(0);
    if (ndviband == NULL || band4 == NULL || sviband == NULL) {
        NXLog("%s cal_svi error\n", __PRETTY_FUNCTION__);
        return false;
    }
    for(int i = 0; i < _width * _height; i++)
    {
        sviband[i] = ndviband[i] * band4[i];
        if(sviband[i] < -2.0) {
            sviband[i] = -2.0;

        }
    }
    return true;    
}

bool Algo::cal_WI(const Image_f & src, Image_b & result, float threshold)
{
    NXLog("%s\n", __PRETTY_FUNCTION__);
    result.init(src.width, src.height, src.bandNum);
    Image_f ndvi;
    cal_NDVI(src, ndvi);
    int _width = src.width;
    int _height = src.height;
    float * ndviband = ndvi.getBandPtr(0);
    bool * wiband = result.getBandPtr(0);
    if (ndviband == NULL || wiband == NULL) {
        NXLog("%s cal_WI error\n", __PRETTY_FUNCTION__);
        return false;
    }
    for(int i = 0; i < _width * _height; i++)
    {
        if(ndviband[i]> threshold){
            wiband[i] = true;
        }
        else{
            wiband[i] = false;
        }
    }
    return true;
}

bool Algo::cal_KT(const Image_f & src, Image_f & result)
{
    assert(false);
    return false;
}

bool Algo::cal_Shade(const Image_f & src, Image_f & result)
{
    assert(false);
    return false;
}

bool Algo::cal_distance(const Image_f & src, Image_f & result, float fillvalue , DistAlgo dia)
{
    NXLog("%s\n", __PRETTY_FUNCTION__);
    result.init(src.width, src.height, src.bandNum);
    Image_b wiImage;
    if (!cal_WI(src, wiImage)) {
        NXLog("%s cal_distance error\n", __PRETTY_FUNCTION__);
        return false;
    }

    result.fillValue(fillvalue);
    int width = src.width;
    int height = src.height;
    bool * wi = wiImage.getBandPtr(0);
    float * distance = result.getBandPtr(0);
    if (wi == NULL || distance == NULL) {
        NXLog("%s cal_distance error\n", __PRETTY_FUNCTION__);
        return false;
    }

    for (int row = 1; row < height - 1; row++) 
    {
        for (int col = 1; col < width - 1; col++)
        {
            int idx = row * width + col;
            // if not water, set the dist as FILLVALUE and continue
            if(!wi[idx])
            {
                continue;
            }
            // find the neighbour water pixels 
            int up = (row - 1) * width + col;
            int down = (row - 1) * width + col;
            int left = idx - 1;
            int right = idx + 1;
            // if all near pixel are water , then skip
            if (wi[up] && wi[down] && wi[left] && wi[right])
            {
                continue;
            }
            // else if the near pixel all are not water 
            else if (!(wi[up] || wi[down] || wi[left] || wi[right] ))
            {
                // then calculate distance for each pixel
                for ( int ii = -NEAR_POINTS_NUM + 1; ii < NEAR_POINTS_NUM; ii++)
                    for ( int jj = -NEAR_POINTS_NUM + 1; jj < NEAR_POINTS_NUM; jj++)
                    {
                        if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                            continue;
                        int nearidx = (row + ii) * width + col+jj;
                        if(wi[nearidx]) 
                            continue;
                        float dis = sqrt(pow(ii,2)+pow(jj,2));
                        if (dis < distance[nearidx])
                            distance[nearidx] = dis;
                    }
            }
            // else pixel the near pixeles are water
            else{
                // if up and down are both not water
                if(!(wi[up]||wi[down]))
                {
                    // if left is water 
                    if(wi[left])
                    {
                        // calculate the distance for the right part 
                        for ( int ii = -NEAR_POINTS_NUM+1; ii < NEAR_POINTS_NUM; ii++)
                        for ( int jj = 0; jj < NEAR_POINTS_NUM; jj++)
                        {
                            if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                                continue;
                            int nearidx = (row + ii) * width + col+jj;
                            if(wi[nearidx]) 
                                continue;
                            float dis = sqrt(pow(ii,2)+pow(jj,2));
                            if (dis < distance[nearidx])
                                distance[nearidx] = dis;
                        }
                    }
                    // else the right pixel is water
                    else if (wi[right])
                    {
                        // calculate the distance for the left part
                        for ( int ii = -NEAR_POINTS_NUM+1; ii < NEAR_POINTS_NUM; ii++)
                        for ( int jj = -NEAR_POINTS_NUM+1; jj <= 0; jj++)
                        {
                            if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                                continue;
                            int nearidx = (row + ii) * width + col+jj;
                            if(wi[nearidx]) 
                                continue;
                            float dis = sqrt(pow(ii,2)+pow(jj,2));
                            if (dis < distance[nearidx])
                                distance[nearidx] = dis;
                        }
                    }
                    // impossible case
                    else
                    {
                        assert(true == false);
                    }    
                }
                // else up and down has water pixel. one or both
                else
                {
                    // if the up pixel is water
                    if(wi[up])
                    {
                        // calculate the distance for the down part
                        for ( int ii = 0; ii < NEAR_POINTS_NUM; ii++)
                        for ( int jj = -NEAR_POINTS_NUM+1; jj < NEAR_POINTS_NUM; jj++)
                        {
                            if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                                continue;
                            int nearidx = (row + ii) * width + col+jj;
                            if(wi[nearidx]) 
                                continue;
                            float dis = sqrt(pow(ii,2)+pow(jj,2));
                            if (dis < distance[nearidx])
                                distance[nearidx] = dis;
                        }
                    }
                    // else if the down pixel is water
                    else if (wi[down])
                    {
                        // calculate the distance for the up part
                        for ( int ii = -NEAR_POINTS_NUM+1; ii <= 0; ii++)
                        for ( int jj = -NEAR_POINTS_NUM+1; jj < NEAR_POINTS_NUM; jj++)
                        {
                            if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                                continue;
                            int nearidx = (row + ii) * width + col+jj;
                            if(wi[nearidx]) 
                                continue;
                            float dis = sqrt(pow(ii,2)+pow(jj,2));
                            if (dis < distance[nearidx])
                                distance[nearidx] = dis;
                        }
                    }else
                    {
                        // impossible case
                        assert(true == false);
                    }
                } // if(!(wi[up]||wi[down]))
            } // if (wi[up] && wi[down] && wi[left] && wi[right])
        }// for
    }
    NXLog("%s, GFimg::getDistance2water():distance calculate  success.\n", __PRETTY_FUNCTION__);

    // set the far pixel as the FILLVALUE the keep the influence range is circle
    for ( int row = 0; row < height; row++)
    {
        for ( int col = 0; col < width; col++)
        {
            int idx = row * width + col;
            if(wi[idx]) {
                distance[idx] = WATERVALUE;
            }
            if(distance[idx] > NEAR_POINTS_NUM)
            {
                distance[idx] = FILLVALUE;
            }
        } 
    }
    
    return true;
    
}

bool Algo::cal_density(const Image_f & src, Image_f & result, Method me)
{
    NXLog("%s\n", __PRETTY_FUNCTION__);
    Image_f svi;
    if (!cal_SVI(src, svi))
    {
        NXLog("%s, %d, cal_SVI error happened\n", __PRETTY_FUNCTION__, __LINE__);
        return false;
    }

    Image_f distance;
    if (!cal_distance(src, distance))
    {
        NXLog("%s, %d, cal_distance error happened\n", __PRETTY_FUNCTION__, __LINE__);
        return false;
    }

    result.init(src.width, src.height, src.bandNum);
    int _width = src.width;
    int _height = src.height;
    float * sviband = svi.getBandPtr(0);
    float * distband = distance.getBandPtr(0);
    float * densband = result.getBandPtr(0);
    if (sviband == NULL || distband == NULL || densband == NULL) {
        NXLog("%s cal_density error\n", __PRETTY_FUNCTION__);
        return false;
    }

    for ( int i = 0; i < _width * _height ; i++)
    {
        densband[i] = 100*sviband[i] - distband[i]/ 200;
    }
    return true;
}

}
