#include <string>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <math.h>
#include "tinyxml.h"
#include "gdal_priv.h"
#include "gdal_alg.h"
#include "cpl_conv.h"
#include "GFimg.h"
#include "config.h"

GFimg::GFimg(const std::string  _filename,
            const std::string _xmlfile,
             const std::string _rpbfile,
             const std::string _savedir,
             const int  _SensorType)
            : filename(_filename)
            , xmlfile(_xmlfile)
            , rpbfile(_rpbfile)
            , savedir(_savedir)
            , SensorType(_SensorType)
            , band1(NULL)
            , band2(NULL)
            , band3(NULL)
            , band4(NULL)
            , ndvi(NULL)
            , ndwi(NULL)
            , wi(NULL)
            , svi(NULL)
            , bright(NULL)
            , green(NULL)
            , wet(NULL)
            , dis(NULL)
            , isShade(NULL)
            , distance(NULL)
            , density(NULL)
            , latitude(NULL)
            , longitude(NULL)
            , width(-1)
            , height(-1)
            , _leftup(0,0)
            , _rightdown(0,0)
{
    printf("GFimg::GFimg(), constructor");
}

void GFimg::init()
{
    printf("reading GF img  %s  .......\n", filename.c_str());
   GDALAllRegister();
   GDALDataset * pDataset = NULL;
   pDataset = (GDALDataset *)GDALOpen(filename.c_str(),GA_ReadOnly);
   if(pDataset == NULL)
   {
        printf("File to open file: %s\n", filename.c_str());
       // GDALCose(pDataset);
        return ;
   }

//   double adfGeoTransform[6];
//   pDataset->GetGeoTransform(adfGeoTransform);
//   printf("GeoTransfrom:%f,%f,%f,%f,%f,%f\n",adfGeoTransform[0],adfGeoTransform[1],adfGeoTransform[2],adfGeoTransform[3],adfGeoTransform[4],adfGeoTransform[5]);
   int nRasterCount = GDALGetRasterCount(pDataset);
   width = GDALGetRasterXSize(pDataset);
   height = GDALGetRasterYSize(pDataset);
   printf("nRasterCount:%d,width:%d,height:%d\n",nRasterCount,width,height);
   assert(nRasterCount == 4);

   GDALRasterBand * pBand1 = pDataset->GetRasterBand(1);
   GDALRasterBand * pBand2 = pDataset->GetRasterBand(2);
   GDALRasterBand * pBand3 = pDataset->GetRasterBand(3);
   GDALRasterBand * pBand4 = pDataset->GetRasterBand(4);
   uint16_t * pMemData = (uint16_t *) CPLMalloc(sizeof(uint16_t)* width * height);
   // the first band
    pBand1->RasterIO(GF_Read,0,0,
            width,height,pMemData,
            width,height,GDT_UInt16,
            0,0);
    float * _band1 = (float *) CPLMalloc(sizeof(float)* width * height);
    float  _offset = offset[SensorType * 4];
    float  _scale = scale[SensorType * 4];
    for(int i = 0; i < height*width; i++)
        {
            _band1[i] = (pMemData[i] - _offset)/ _scale/E0[0];
        }
    band1 = _band1;
    // the second band
    pBand2->RasterIO(GF_Read,0,0,
            width,height,pMemData,
            width,height,GDT_UInt16,
            0,0);
    float * _band2 = (float *) CPLMalloc(sizeof(float)* width * height);
    _offset = offset[SensorType * 4+1];
    _scale = scale[SensorType * 4+1];
    for(int i = 0; i < height*width; i++)
        {
            _band2[i] = (pMemData[i] - _offset)/ _scale/E0[1];
        }
    band2 = _band2;
    // the third band
    pBand3->RasterIO(GF_Read,0,0,
            width,height,pMemData,
            width,height,GDT_UInt16,
            0,0);
    float * _band3 = (float *) CPLMalloc(sizeof(float)* width * height);
    _offset = offset[SensorType * 4+2];
    _scale = scale[SensorType * 4+2];
    for(int i = 0; i < height*width; i++)
        {
            _band3[i] = (pMemData[i] - _offset)/ _scale/E0[2];
        }
    band3 = _band3;
    // the fourth  band 
    pBand4->RasterIO(GF_Read,0,0,
            width,height,pMemData,
            width,height,GDT_UInt16,
            0,0);
    float * _band4 = (float *) CPLMalloc(sizeof(float)* width * height);
    _offset = offset[SensorType * 4+3];
    _scale = scale[SensorType * 4+3];
    size_t iLimit = height*width;
    for(size_t i = 0; i < iLimit; i++)
        {
            _band4[i] = (pMemData[i] - _offset)/ _scale/E0[3];
        }
    band4 = _band4;
    CPLFree(pMemData); pMemData = NULL;
    printf("read done!\n");
    GDALClose(pDataset);
}

GFimg::~GFimg()
{
    printf("GFimg release...\n");
    if(band1!=NULL){
        CPLFree(band1); band1 = NULL;
    }
    if(band2!=NULL){
        CPLFree(band2); band2 = NULL;
    }
    if(band3!=NULL){
        CPLFree(band3); band3 = NULL;
    }
    if(band4!=NULL){
        CPLFree(band4); band4 = NULL;
    }
    if(ndvi!=NULL){
        CPLFree(ndvi); ndvi = NULL;
    }
    if(ndwi!=NULL)
    {
        CPLFree(ndwi);ndwi = NULL;
    }
    if(svi!=NULL)
    {
        CPLFree(svi);svi = NULL;
    }
    if(isShade !=NULL)
    {
        CPLFree(isShade); isShade = NULL;
    }
    if(wi!=NULL){
        CPLFree(wi); wi = NULL;
    }
    if(bright!=NULL){
        CPLFree(bright); bright = NULL;
        CPLFree(green); green = NULL;
        CPLFree(wet); wet = NULL;
    }
    if(dis!=NULL)
    {
        CPLFree(dis);dis = NULL;
    }
    if(density!=NULL)
    {
        CPLFree(density);density = NULL;
    }
    if(latitude!=NULL)
    {
        CPLFree(latitude);latitude = NULL;
    }
    if(longitude!=NULL)
    {
        CPLFree(longitude);longitude = NULL;
    }
    if(distance!=NULL)
    {
        CPLFree(distance);distance = NULL;
    }
    if(urban!=NULL)
    {
        CPLFree(urban);urban = NULL;
    }
    if(risk!=NULL)
    {
        CPLFree(risk);risk = NULL;
    }
}

void GFimg::print(print_var var)
{
    if(print_reflectance == var) {
        if(band1!=NULL)
        {
            for(int i =0; i < width * height;i+=100000)
            {
                printf("%f,%f,%f,%f", band1[i],band2[i],band3[i], band4[i]);
            }
        }
        else
        {
            printf("band1 is NULL");
        }
    }
    else if (print_ndvi == var) {
        if(ndvi != NULL)
        {
            for(int i =0; i < width * height;i+=100000)
            {
                printf("ndvi: %f", ndvi[i]);
            }
        }
        else
        {
            printf("ndvi is NULL");
        }
    }
    else if (print_distance == var) {
        if(distance != NULL)
        {
            for(int i =0; i < width * height;i+=100000)
            {
                printf("distance: %f", distance[i]);
            }
        }
        else
        {
            printf("distance is NULL");
        }
    }
    else if (print_svi == var) {
        if(svi != NULL)
        {
            for(int i =0; i < width * height;i+=100000)
            {
                printf("svi: %f", svi[i]);
            }
        }
        else
        {
            printf("svi is NULL");
        }
    }
    else if (print_density == var) {
        if(density != NULL)
        {
            for(int i =0; i < width * height;i+=100000)
            {
                printf("density: %f", density[i]);
            }
        }
        else
        {
            printf("density is NULL");
        }
    }
    else {
        printf("other print type , undefined");
    }
}
void GFimg::setPOI(std::pair<int, int> leftup, std::pair<int, int> rightdown) {
    if(leftup.first == 0 && leftup.second == 0 && rightdown.first == 0 && rightdown.second == 0) {
        printf("POI invalid input!\n");
        return;
    }
    if(leftup.first < 0 || leftup.second < 0 || rightdown.first < 0 || rightdown.second < 0) {
        printf("POI invalid input!\n");
        return;
    }
    if(leftup.first > height || leftup.second > width || rightdown.first > height || rightdown.second > width) {
        printf("POI invalid input!\n");
        return;
    }
    _leftup = leftup;
    _rightdown = rightdown;
    printf("GFimg::setPOI<%d,%d>,<%d,%d>\n",leftup.first,leftup.second,rightdown.first,rightdown.second);

}
int GFimg::getPOIArea(std::pair<int, int> leftup, std::pair<int, int> rightdown) {
    printf("GFimg::getPOIArea()");
    if(density == NULL) {
      printf("density is NULL");
      return -1;
    }
    if(leftup.first == 0 && leftup.second == 0 && rightdown.first == 0 && rightdown.second == 0) {
        printf("POI invalid input!\n");
        return -1;
    }
    if(leftup.first < 0 || leftup.second < 0 || rightdown.first < 0 || rightdown.second < 0) {
        printf("POI invalid input!\n");
        return -1;
    }
    if(leftup.first > height || leftup.second > width || rightdown.first > height || rightdown.second > width) {
        printf("POI invalid input!\n");
        return -1;
    }
    int count = 0;
    for(int row = leftup.first; row < rightdown.first; row++) {
        for(int col = leftup.second; col < rightdown.second; col++) {
            int idx = row*width + col;
            if(density[idx]>0)
                count++;
        }
    }
    return count;
}

void GFimg::getNDVI()
{
    printf("getNDVI()...\n");
    ndvi = (float *) CPLMalloc(sizeof(float)* width * height);
    size_t iLimit = height*width;
    for(size_t i = 0; i < iLimit; i++)
        {
            ndvi[i] = (band4[i]-band3[i])/(band4[i]+band3[i]);
            if(ndvi[i] < -2) 
                ndvi[i] = -2.0;
        }
}

void GFimg::getNDWI()
{
    printf("getNDWI()...\n");
    ndwi = (float *) CPLMalloc(sizeof(float) * width * height);
    size_t iLimit = height * width;
    for(size_t i = 0; i < iLimit; i++)
    {
        ndwi[i] = (band2[i] - band4[i])/(band2[i] + band4[i]);
        if(ndwi[i] < NDWI_MIN)
            ndwi[i] = NDWI_MIN;
        else if (ndwi[i] > NDWI_MAX)
            ndwi[i] = NDWI_MAX;
        else
        {;
        }
    }
}
void GFimg::getIsShade()
{
    printf("getIsShade()...");
    isShade = (float *) CPLMalloc(sizeof(float) * width * height);
 //   ndwi = (float * ) CPLMalloc(sizeof(float) * width * height);
    size_t iLimit = height*width;
    
    for(size_t i = 0; i < iLimit; i++)
        {
            float ratio = band4[i]/band3[i];
            if(ratio > RATIO43_WATERINDEX)
                isShade[i] = 0;
            else
                isShade[i] = 1;
            /*
            if(ratio> RATIO43_MAX)
                isShade[i] = RATIO43_MAX;
            else if (ratio < RATIO43_MIN)
                isShade[i] = RATIO43_MIN;
            else
            {
                isShade[i] = ratio;
            } */
        }
}
void GFimg::getWI()
{
    printf("GFimg::getWI()...\n");
    if (ndwi==NULL)
    {
        printf("GFimg::getWI(), do getNDWI() !\n");
        getNDWI();
    }
    
/*    if (ndwi == NULL)
    {
        printf("ndwi is not init before\n");
        exit(1);
    }
    */
    wi = (bool *) CPLMalloc(sizeof(bool) * width * height);
 //   ndwi = (float * ) CPLMalloc(sizeof(float) * width * height);
    size_t iLimit = height*width;
    
    for(size_t i = 0; i < iLimit; i++)
        {
            if(ndwi[i]> NDWI_WATERINDEX)
                wi[i] = true;
            else
                wi[i] = false;
        }
    // for each pixel
    for (int row = 0; row < height; row++)
    {    
        for (int col = 0; col < width; col++)
        {
            int idx = row * width + col;
           // if the pixel is not water
           if(!wi[idx])
           {
           // then continue;
                continue;
           }
           // else 
           else 
           {
               // define one set for save all close water pixel location, (x, y), add this pixel into set
               // if the up is also water
               //add the up pixel to set
               // if the down is also water
               //add the down pixel to set
               // if the left is also water
               //add the left pixel to set
               // if the right is also water
               //add the right pixel to set
               // if the upleft is also water
               //add the upleft pixel to set
                // .....
                // .....
               // add the downright pixel to set
               
               //

           }// end of else 
        }// end of col
    }// end of row
}
void GFimg::getKTtransform()
{
    printf("getKTtransform...\n");
    bright = (float *) CPLMalloc(sizeof(float)* width * height);
    green = (float *) CPLMalloc(sizeof(float)* width * height);
    wet = (float *) CPLMalloc(sizeof(float)* width * height);
    size_t iLimit = height*width;
    for(size_t i = 0; i < iLimit; i++)
        {
            bright[i] = band1[i] * 0.326 + band2[i] * 0.509 + band3[i] * 0.560 + band4[i] * 0.567;
            green[i] = band1[i] * (-0.311) + band2[i] * (-0.356) + band3[i] * (-0.325) + band4[i] * 0.819;
            wet[i] = band1[i] * (-0.612) + band2[i] * (-0.312) + band3[i] * 0.722 + band4[i] * (-0.081);
        }

}

void GFimg::checkNearWater(NearBody * nb, int row, int col)
{
    assert(true == false);
}

void GFimg::getDistance2water(DistAlgo da)
{
    printf("GFimg::getDistance2water():get distance\n");
    // if algorithm is for each image pixel
    if (Pixel2Water == da) {
        assert(true == false);
    }
    
    // else if algorithm is for each water pixel
    else if (Water2Pixel == da) {
        printf("No POI input, calculate distance for the whole image!\n");
        if(wi == NULL) {
            getWI();
        }
        // malloc the mat of distance
        distance = (float *) CPLMalloc(sizeof(float) * width * height);
        if(distance == NULL) {
            printf("malloc the distance failed\n");
            return;
        }
        printf("malloc the distance sucess\n");
        // init the disint 
        for (int row = 0; row < height; row++)
            for (int col = 0; col < width; col++)
            {
                int idx = row * width + col;

                distance[idx] = FILLVALUE;
            }
        printf("GFimg::getDistance2water():distance init success.\n");
        // for each pixel
        
   //     NearBody * nb = new NearBody();
        for (int row = 1; row < height-1; row++) {
          //  printf("row: %d", row);
            for (int col = 1; col < width-1; col++)
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
                    for ( int ii = -NEAR_POINTS_NUM+1; ii < NEAR_POINTS_NUM; ii++)
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
        printf("GFimg::getDistance2water():distance calculate  success.\n");

        // set the far pixel as the FILLVALUE the keep the influence range is circle
        for ( int row = 0; row < height; row++)
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
    }// method
    else if (POI_SVI_DIST == da ) {
        printf("There are valid POI input, calculate POI distance!\n");
        if(wi == NULL) {
            getWI();
        }
        // malloc the mat of distance
        distance = (float *) CPLMalloc(sizeof(float) * width * height);
        if(distance == NULL) {
            printf("malloc the distance failed\n");
            return;
        }
        printf("malloc the distance sucess\n");
        // init the disint
        for (int row = 0; row < height; row++)
            for (int col = 0; col < width; col++)
            {
                int idx = row * width + col;
                distance[idx] = FILLVALUE;
            }
        printf("GFimg::getDistance2water():distance init success.\n");
        // for each pixel

   //     NearBody * nb = new NearBody();
        for (int row = _leftup.second; row < _rightdown.second-1; row++) {
            //printf("row :%d \n", row);
            for (int col = _leftup.first; col < _rightdown.first-1; col++)
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
                    for ( int ii = -NEAR_POINTS_NUM+1; ii < NEAR_POINTS_NUM; ii++)
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
        printf("GFimg::getDistance2water():distance calculate  success.\n");

        // set the far pixel as the FILLVALUE the keep the influence range is circle
        for (int row = _leftup.second; row < _rightdown.second-1; row++)
            for (int col = _leftup.first; col < _rightdown.first-1; col++)
            {
               int idx = row * width + col;
                if(distance[idx] > NEAR_POINTS_NUM)
                {
                    distance[idx] = FILLVALUE;
                }
           }
    }
}// EOF




void GFimg::getDistance2water()
{
    
    printf("get distance\n");
    assert(wi != NULL);
    dis = (float *) CPLMalloc(sizeof(float)* width * height);
    for(int row = 1; row < height; row++)
        for(int col =1; col < width; col++)
        {
            printf("r %d\n", row);
            int idx = row * width +col;
            if(wi[idx]) /* water*/
            {
                dis[idx] = 0;    
                continue;
            }else
            {
                int rn = 1;
                while(true)
                {
                    if ((row <= rn)|| (col <=rn) || (row+rn >= width)||(col +rn >= height))
                    {
                        dis[idx] = rn;break;
                    }
                   // int directions = 0;
                    if(wi[(row+rn)*width+col] || wi[(row-rn)*width+col] ||wi[row*width+col+rn]||wi[row*width+col-rn])
                    {
                        dis[idx] = rn * GRes; break;
                    }
                    for(int directions = 1; directions < rn; directions++)
                    {
                        if(wi[(row+rn)*width+col+directions] || wi[(row-rn)*width+col+directions] ||wi[(row+directions)*width+col+rn] || wi[(row+directions)*width+col-rn] || wi[(row+rn)*width+col-directions] || wi[(row-rn)*width+col-directions] ||wi[(row-directions)*width+col+rn]||wi[(row-directions)*width+col-rn])
                        {
                            dis[idx] = sqrt(rn^2+directions^2)* GRes; break;
                        }
                    } 
                   if (rn++ >= NEAR_POINTS_NUM )
                   {
                       dis[idx] = FILLVALUE  * GRes;
                        break;
                   }
                    
                }// while
            }//if wi[idx]  else

        }
    printf("get distance done\n");
}

void GFimg::getSVI()
{
    printf("GFimg::getSVI()...\n");
    svi = (float *) CPLMalloc(sizeof(float)* width * height);
    if(ndvi == NULL){getNDVI();}
    size_t iLimit = height*width;
    for(size_t i = 0; i < iLimit; i++)
        {
            svi[i] = ndvi[i] * band4[i];
            if(svi[i] < -2.0) 
                svi[i] = -2.0;
        }

    printf("getSVI() end\n");
}

void GFimg::caldensity()
{
    caldensity(TEST_CASE);
}

void GFimg::caldensity(Method me)
{
    
    /*
    if(NULL== dis){getWI();getDistance2water();}
    
    if(wet==NULL){getKTtransform();} */

     density = (float *) CPLMalloc(sizeof(float)* width * height);
    if(me == withoutDist)
    { 
        if(ndwi==NULL){getNDWI();}
        getWI();
        if(distance == NULL) {
            getDistance2water(Water2Pixel);
            if(distance == NULL) {
                return;
            }
            write(var_dist, "distance.tiff");
        }
        assert(distance != NULL);
        for (int i = 0; i < width * height ; i++)
        {
            float ndvi  =  (band4[i]-band3[i])/(band4[i]+band3[i]);
            float wet = band1[i] * (-0.612) + band2[i] * (-0.312) + band3[i] * 0.722 + band4[i] * (-0.081);
            float bright = band1[i] * 0.326 + band2[i] * 0.509 + band3[i] * 0.560 + band4[i] * 0.567;
            density[i] = 1.04525 - 8.41716* wet + 1.919549 * ndvi + 0.014895 * bright - 0.00146*4*distance[i];
      //  density[i] = 1.04525 - 8.41716* wet[i] + 1.919549 * ndvi[i] + 0.014895 * bright[i] - 0.00146 * dis[i];
        }
        write(var_density,"density_bobo.tiff");
    }else if (me == withDist_1)
    {
        
        if(ndvi==NULL){getNDVI();}
        if(ndwi==NULL){getNDWI();}
        getWI();
        if (distance == NULL) {
            getDistance2water(Water2Pixel);
            write(var_dist, "distance.tiff");
        }
        getIsShade();
        
        for ( int i = 0; i < width * height ; i++)
        {
            float res = 0.65 - 10*fabs( ndvi[i] - 0.515) 
                + 0.35 - 10*fabs(ndwi[i] + 0.41) ;
            if (distance[i] >= 300 || isShade[i] == 0 || res < 0)
                density[i] = 0;
            else
                density[i] = res;
       }
                
     for (int i =0; i < width * height; i=i+1000000)
       {
            printf("%f,%f,%f     ",  distance[i], ndvi[i], density[i]);
        }
//        if(wet == NULL){getKTtransform();}
//        write(var_wet,"wet.tiff");
        write(var_isShade,"isShade.tiff");
        write(var_ndvi,"ndvi.tiff");
        write(var_dist,"dist.tiff");
        write(var_ndwi,"ndwi.tiff");
        write(var_density,"density2.tiff"); 
     //   getSVI();
     //   write(var_svi,"svi.tiff");
     //   write(var_reflectance,"reflectance.tiff");
    }
    else if (me == SVI_DIST) {
        if(svi == NULL) {
            getSVI();
          //  write(var_svi,"svi.tiff");
        }
        if (distance == NULL ) {
            getDistance2water(Water2Pixel); 
          //  write(var_dist, "distance.tiff");
        }

        for ( int i = 0; i < width * height ; i++)
        {
            density[i] = 100*svi[i] - distance[i]/ 200;
        }
        //write(var_density,"density_svi_dist.tiff");
    }
    else if (me == TEST_CASE) {

        if(svi == NULL) {
            getSVI();
            printf("caldensity(TEST_CASE)\n");
            write(var_svi, "svi.tiff");
        }
    }
}

void GFimg::areacount(std::pair<int,int> leftup, std::pair<int, int> rightdown)
{
    if(density == NULL) {
        printf("please caldensity first!\n");
        return;
    }
    if(leftup.first == 0 && leftup.second == 0 && rightdown.first == 0 && rightdown.second == 0) {
        printf("please invalid input!\n");
        return;
    }
    int result[8] = {0,0,0,0,0,0};
    for ( int i = leftup.first; i < rightdown.first ; i++) {
        for(int j = leftup.second; j < rightdown.second; j++) {
            int num = i*j;
            if(density[num] > 0) {
                result[0] = result[0] + 1;
                if(density[num] > 0.5) {
                    result[1] = result[1] +1;
                    if(density[num] > 1) {
                        result[2] = result[2] +1;
                        if(density[num] > 1.5) {
                            result[3] = result[3] +1;
                            if(density[num] > 2) {
                                result[4] = result[4] +1;
                                if(density[num] > 2.5) {
                                    result[5] = result[5] +1;
                                    if(density[num] > 3) {
                                        result[6] = result[6] +1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    std::string tempsfile = this->savedir + DIR_SEPERATOR + "areacounter.txt";
    std::ofstream file(tempsfile.c_str());
    if(!file.is_open()) {
        printf("failed to creat areacounter.txt");
        for(int idx = 0; idx < 7; idx++) {
            printf("area > %f, counter is :%d", idx/2.0, result[idx]);
        }
    }
    for(int idx = 0 ; idx < 7 ; idx++) {
        file << "area > " << idx/ 2.0 << ", counter is " << result[idx] <<std::endl;
    }
    printf("counter finished!\n");
}


void GFimg::getCorner()
{
    printf("GFimg::getCorner()...\n");
    TiXmlDocument doc(xmlfile.c_str());
    bool loadOkay = doc.LoadFile();  
    if (!loadOkay) {      
        printf( "Could not load test file %s. Error='%s'. Exiting.\n", xmlfile.c_str(),doc.ErrorDesc() );
        return;
    }
    TiXmlElement* root = doc.RootElement();  
    TiXmlNode * topleftlat = root->FirstChild("TopLeftLatitude");
    tllat =atof((char *)topleftlat->ToElement()->GetText());
    TiXmlNode * topleftlong = root->FirstChild("TopLeftLongitude");
    tllong =atof((char *)topleftlong->ToElement()->GetText());
    TiXmlNode * toprightlat = root->FirstChild("TopRightLatitude");
    trlat =atof((char *)toprightlat->ToElement()->GetText());
    TiXmlNode * toprightlong = root->FirstChild("TopRightLongitude");
    trlong =atof((char *)toprightlong->ToElement()->GetText());
    TiXmlNode * bottomleftlat = root->FirstChild("BottomLeftLatitude");
    bllat =atof((char *)bottomleftlat->ToElement()->GetText());
    TiXmlNode * bottomleftlong = root->FirstChild("BottomLeftLongitude");
    bllong =atof((char *)bottomleftlong->ToElement()->GetText());
    TiXmlNode * bottomrightlat = root->FirstChild("BottomRightLatitude");
    brlat =atof((char *)bottomrightlat->ToElement()->GetText());
    TiXmlNode * bottomrightlong = root->FirstChild("BottomRightLongitude");
    brlong =atof((char *)bottomrightlong->ToElement()->GetText());
    printf("topleftlatitude, topleftlongitude:(%f,%f),  toprightlatitude,toprightlongitude:(%f,%f),\n bottomleftlatitude,bottomleftlongitude:(%f,%f),  bottomrightlatitude,bottomrightlongitude:(%f,%f)\n",tllat,tllong,trlat,trlong,bllat,bllong,brlat,brlong);

}

void GFimg::generate_Lat_Long()
{
    assert(!xmlfile.empty());
    printf("begin to generate Latitude and Longitude...\n");
    latitude = (float *) CPLMalloc(sizeof(float) * width * height);
    longitude = (float * ) CPLMalloc(sizeof(float) * width * height);
    for(int row = 0; row < height; row++)
    {
            float leftlat = (-tllat + bllat)/(height-1) * row + tllat;
            float rightlat = (-trlat + brlat)/(height-1) * row + trlat;
            float leftlong = (-tllong + bllong)/(height-1) * row + tllong;
            float rightlong = (-trlong + brlong)/(height-1) * row + trlong;
        for (int col = 0; col < width ; col++)
        {
            latitude[row*width + col] = (leftlat-rightlat)/(width-1)*col+rightlat;
            longitude[row*width + col] = (leftlong-rightlong)/(width-1)* col + rightlong;
        }
    }
    printf(" generate Latitude and Longitude Done\n");
}

void GFimg::getValueByPoints(float * li,  std::vector<int> * colrowtemp)
{
    printf("begin to getValueByPoints near<%f,%f>\n", li[0],li[1]);
 //   std::vector<int> colrowtemp;
    for(int row = 0; row < height; row = row +1)
    {
        for(int col = 0; col < width; col = col+1)
        {
        int index = row * width + col;
        float input[2]={longitude[index],latitude[index]};
      /*  printf("<%f,%f>--<%f,%f> = %f\n", input[0],input[1],li[0],li[1],point2point(input,li));
        if(fabs(input[0] - li[0]) > DIST_LIMIT_DEGREE || fabs(input[1] - li[1]) > DIST_LIMIT_DEGREE)
        {
            col = col + JUMP_NUM;
            continue;
        }else
        {
            printf("<%f,%f>--<%f,%f> = %f\n", input[0],input[1],li[0],li[1],point2point(input,li));
            
        }
        */
        
        if(fabs(input[0] - li[0]) < FIRST_DEGREE && fabs(input[1] - li[1]) < FIRST_DEGREE)
        {
            if(point2point(input,li)<DIST_LIMIT_DEGREE)
            {
                colrowtemp->push_back(index);
            }else
                continue;
           // printf("Xdif:%f,Ydif:%f,Dis:%f\n",fabs(input[0]-li[0]),fabs(input[1]-li[1]),point2point(input,li));
        }else
        {
            col=col+ JUMP_NUM;
            continue;
        }
       /*
        if(fabs(input[0] - li[0]) > DIST_LIMIT_DEGREE || fabs(input[1] - li[1]) > DIST_LIMIT_DEGREE)
        {
            col = col + JUMP_NUM;
            continue;
        }else if(point2point(input, li) < DIST_LIMIT_DEGREE)
        {
            printf("<%f,%f>--<%f,%f> = %f\n", input[0],input[1],li[0],li[1],point2point(input,li));
            colrowtemp->push_back(index);
        }
*/
        }
    }
    printf("Point <%f,%f> extract Values done!\n", li[0],li[1]);

}

void GFimg::print_lat_long()
{
    for(size_t i = 0; i < width*height; i=i+width)
    {
        printf("latitude,longitude:%f,%f\n", latitude[i],longitude[i]);
    }
}

void GFimg::getAllValueNearLine(float *li, float * restmp)
{
    float diftoplat = fabs(tllat- trlat); 
    float difbottomlat = fabs(bllat-brlat);
    float diflat = diftoplat > difbottomlat? diftoplat : difbottomlat;
    float difleftlong = fabs(bllong - tllong);
    float difrightlong = fabs(brlong - trlong);
    float diflong = difleftlong > difrightlong ? difleftlong : difrightlong;
    
    // estimate the range of interested region.
    float longmin =( li[0]>li[2] ? li[2]: li[0]) - diflong;
    float longmax =( li[0] > li[2] ? li[0] : li[2])+ diflong;
    float latmax = (li[1] > li[3] ? li[1] : li[3]) + diflat;
    float latmin = (li[1] > li[3] ? li[3] : li[1]) - diflong;
    assert(longmin < longmax);
    assert(latmin < latmax);
    printf("longmin,longmax,latmin,latmax:%f,%f,%f,%f\n",longmin,longmax,latmin,latmax);
    printf("topleftlatitude, topleftlongitude:(%f,%f),  toprightlatitude,toprightlongitude:(%f,%f),\n bottomleftlatitude,bottomleftlongitude:(%f,%f),  bottomrightlatitude,bottomrightlongitude:(%f,%f)\n",tllat,tllong,trlat,trlong,bllat,bllong,brlat,brlong);
    // estimate the row and lines
    int rowmin = geo2num(tllat, bllat, height, latmax);
    int rowmax = geo2num(tllat, bllat, height, latmin);
    int colmin = geo2num(tllong, trlong,width, longmin);
    int colmax = geo2num(tllong, trlong, width, longmax);
    printf("rowmin,rowmax,colmin,colmax:%d,%d,%d,%d\n",rowmin,rowmax,colmin,colmax);
    if (rowmin < 0) rowmin = 0;
    if (rowmax < 0) rowmax = 0;
    if (colmin < 0) colmin = 0;
    if (colmax < 0) colmax = 0;
    if(rowmin ==rowmax || colmin == colmax )
    {
        restmp[0] = 0;
        restmp[1] = 0;
        restmp[2] = 0;
        restmp[3] = 0;
        return;
    }
    printf("range of rows :[%d,%d], of cols:[%d,%d]\n",rowmin,rowmax,colmin,colmax);
    assert(rowmin < rowmax);
    assert(colmin < colmax);
    
    // calculate the distance to line 
    float* res = (float *)malloc(sizeof(float)*2);
    i_colrow.clear();
  //  i_row.clear();
    for (int row = rowmin; row <= rowmax; row++)
    {
        for(int col = colmin; col <= colmax;
                col++)
        {
            num2geo(col, row, res);
            if(dis2point(li, res)< DIST_LIMIT_DEGREE)
            {
                i_colrow.push_back(col+ row*width);
            //    i_row.push_back(row);
            }
        }
    }
    free(res);
    assert(i_colrow.size()==0);
    float av_ndvi, av_bright, av_green, av_wet, av_dis, av_band1, av_band2, av_band3, av_band4, av_wi;
    if(WANTED_PARAM==10)
    {
    for(int i = 0; i < i_colrow.size(); i++)
    {
       av_band1 = av_band1 + band1[i_colrow[i]];
       av_band2 = av_band2 + band2[i_colrow[i]];
       av_band3 = av_band3 + band3[i_colrow[i]];
       av_band4 = av_band4 + band4[i_colrow[i]];
       av_ndvi = av_ndvi + ndvi[i_colrow[i]];
       av_dis = av_dis + dis[i_colrow[i]];
       av_wi = av_wi + wi[i_colrow[i]];
       av_bright = av_bright + bright[i_colrow[i]];
       av_green = av_green + green[i_colrow[i]];
       av_wet = av_wet + wet[i_colrow[i]];
    }
    restmp[0] = av_band1/i_colrow.size();
    restmp[1] = av_band2/i_colrow.size();
    restmp[2] = av_band3/i_colrow.size();
    restmp[3] = av_band4/i_colrow.size();
    restmp[4] = av_ndvi/i_colrow.size();
    restmp[5] = av_wi/i_colrow.size();
    restmp[6] = av_dis/i_colrow.size();
    restmp[7] = av_bright/i_colrow.size();
    restmp[8] = av_green/i_colrow.size();
    restmp[9] = av_wet/i_colrow.size();
    }else if (WANTED_PARAM == 4)
    {
    for(int i = 0; i < i_colrow.size(); i++)
    {
       av_band1 = av_band1 + band1[i_colrow[i]];
       av_band2 = av_band2 + band2[i_colrow[i]];
       av_band3 = av_band3 + band3[i_colrow[i]];
       av_band4 = av_band4 + band4[i_colrow[i]];
    }
    restmp[0] = av_band1/i_colrow.size();
    restmp[1] = av_band2/i_colrow.size();
    restmp[2] = av_band3/i_colrow.size();
    restmp[3] = av_band4/i_colrow.size();
    
    }else
    {
        assert(true == false);
    }
}

void GFimg::readGeoinfo(std::string geofile)
{
    printf("readGeoinfo...\n");
    std::ifstream ifile;
    ifile.open(geofile.c_str(),std::ios::in);
    if(!ifile.is_open())
    {
        printf("geofile file open failed!\n");
        return;
    }
    int count = 0;
    char buf[1024];
    while(ifile.getline(buf,sizeof(buf)))
    {
        count++;
    }
    this->insituPoints = count;
    printf("insituPoints:%d\n",count);
//    ifile.seekg(0,std::ios::beg);
    ifile.close();
    ifile.open(geofile.c_str(),std::ios::in);
    if(!ifile.is_open())
    {
        printf("geofile file open failed!\n");
        return;
    }
    geoinfo =(float *) malloc(sizeof(float)*2 * insituPoints);
    count = 0;
    while(ifile.getline(buf,sizeof(buf)))
    {
 //       printf("%s\n",buf);
      //  sscanf(buf,"%f %f %f %f",&ap[0],&ap[1],&ap[2],&ap[3]);
     //   printf("%f, %f, %f, %f\n",ap[0],ap[1],ap[2],ap[3]);
        sscanf(buf,"%f %f",&geoinfo[2* count],&geoinfo[2* count+1]);
//        printf("geoinfo:%d,(%f,%f),(%f,%f)\n",count, geoinfo[4*count],geoinfo[4*count+1],geoinfo[4*count+2],geoinfo[4*count+3]);
        count++;
    }
    printf("readGeoinfo done!\n");
}

void GFimg::writePOIs(std::string poifile, std::string savefile) {
    printf("GFimg::writePOIs, poi input file: %s, save output file :%s", poifile.c_str(), savefile.c_str());

}

float GFimg::getDistance2water(DistAlgo da, int row, int col) {
   // printf("get distance\n");
    if(wi == NULL) {
        getWI();
    }
    if(col < 2 || col > width-2 || row < 2 || row > height-2) {
        return -1;
    }
    printf("row:%d,col:%d",row,col);
    float d = FILLVALUE;
    if(da == Pixel2Water) {
           // printf("r %d\n", row);
            int idx = row * width +col;
            if(wi[idx]) /* water*/
            {
                return WATERVALUE;
            }else
            {
                int rn = 1;
                while(true)
                {

                    if ((row <= rn)|| (col <=rn) || (row+rn >= height)||(col +rn >= width))
                    {
                        return  FILLVALUE;
                    }
                   // int directions = 0;
                    if(wi[(row+rn)*width+col] || wi[(row-rn)*width+col] ||wi[row*width+col+rn]||wi[row*width+col-rn])
                    {
                        d= rn ; return d;
                    }
                    for(int directions = 1; directions <= rn; directions++)
                    {
                        if(wi[(row+rn)*width+col+directions] || wi[(row-rn)*width+col+directions] ||wi[(row+directions)*width+col+rn] || wi[(row+directions)*width+col-rn] || wi[(row+rn)*width+col-directions] || wi[(row-rn)*width+col-directions] ||wi[(row-directions)*width+col+rn]||wi[(row-directions)*width+col-rn])
                        {
                            printf("rn:%d,direction:%d",rn,directions);
                            d = sqrt(pow(rn,2)+pow(directions,2)); return d;
                        }
                    }
                   if (rn++ >= FILLVALUE )
                   {
                       d = FILLVALUE ;
                        return d;
                   }

                }// while
            }//if wi[idx]  else
    }
}

void GFimg::readRpbCoefs(const std::string rf) {
    printf("GFimg::readRpbCoefs()");
    std::string Rwanted;
    if(rf.compare(FILE_EMPTY) == 0) {
        Rwanted = rpbfile;
    }
    else {
        Rwanted = rf;
    }
/*    std::fstream ifile;
    ifile.open(Rwanted,std::ios::in);
    if(!ifile.is_open())
    {
        printf("RPB file open error");
        return;
    }
    assert(true == false);
*/
}

void GFimg::doextract(std::string savefile)
{
    printf("do extract begin\n");
    assert(geoinfo != NULL);
    if(latitude == NULL || longitude == NULL) {
        generate_Lat_Long();
    }
    std::vector<int>  colrowtemp;
    std::ofstream ofile;
    ofile.open(savefile.c_str(),std::ios::out);
    if(!ofile.is_open())
    {
        printf("savefile file open failed!:%s\n", savefile.c_str());
        return;
    }

    if(density == NULL) {
        caldensity(SVI_DIST);
    }
    ofile<<"Num latitude longitude band1 band2 band3 band4 svi distance density"<<std::endl;
    for (int idx = 0; idx < insituPoints; idx ++)
    {
        colrowtemp.clear();
        float input[2] = {geoinfo[idx*2],geoinfo[idx*2+1]};
        getValueByPoints(input,&colrowtemp);
        //assert(true==false);
        ofile << idx <<" " <<input[0] <<" "<< input[1]<< " ";
        std::cout <<"size of colrowtemp: "<< colrowtemp.size()<<std::endl;
        for(int i =0; i < colrowtemp.size(); i++)
        {
            ofile << band1[colrowtemp[i]] << " " << band2[colrowtemp[i]] << " " 
                << band3[colrowtemp[i]] << " "<< band4[colrowtemp[i]] <<" "
                << svi[colrowtemp[i]] << " "<< distance[colrowtemp[i]] <<" "
                << density[colrowtemp[i]] <<" ";
        }
        ofile<<std::endl;
/////////////////////////////////////////////////////////////

        colrowtemp.clear();
        /*
        float input2[2] = {geoinfo[idx*4+2],geoinfo[idx*4+3]};
        getValueByPoints(input2,&colrowtemp);
        //assert(true==false);
        ofile << input2[0] <<" "<< input2[1]<< " ";
        std::cout <<"size of colrowtemp: "<< colrowtemp.size()<<std::endl;
        for(int i =0; i < colrowtemp.size(); i++)
        {
            ofile << band1[colrowtemp[i]] << " " << band2[colrowtemp[i]] << " " 
                << band3[colrowtemp[i]] << " "<< band4[colrowtemp[i]]<<" ";
        }
        ofile<<std::endl;
        */
    }
    printf("do extract done\n");
}
void GFimg::extractAll(std::string savefile)
{
    printf("do extract begin\n");
    assert(geoinfo != NULL);
    assert(latitude != NULL || longitude != NULL);
    std::ofstream ofile;
    ofile.open(savefile.c_str(),std::ios::out);
    if(!ofile.is_open())
    {
        printf("savefile file open failed!:%s\n", savefile.c_str());
        return;
    }
//    for (int idx = 0; idx < width*height; idx ++)
    for(int row =0; row < height; row=row+STEP)
    {
        for(int col=0; col< width; col = col + STEP)
        {
            int idx = row * width + col;
            printf("band1:%f,band2:%f,band3:%f,band4:%f\n",band1[idx],band2[idx],band3[idx],band4[idx]);
            ofile<< band1[idx] << " " << band2[idx] << " " 
                << band3[idx] << " "<< band4[idx]<< std::endl;
        }
    }
    /*
    {
        colrowtemp.clear();
        float input[2] = {geoinfo[idx*4],geoinfo[idx*4+1]};
        getValueByPoints(input,&colrowtemp);
        //assert(true==false);
        ofile << input[0] <<" "<< input[1]<< " ";
        std::cout <<"size of colrowtemp: "<< colrowtemp.size()<<std::endl;
        for(int i =0; i < colrowtemp.size(); i++)
        {
            ofile << band1[colrowtemp[i]] << " " << band2[colrowtemp[i]] << " " 
                << band3[colrowtemp[i]] << " "<< band4[colrowtemp[i]]<<" ";
        }
        ofile<<std::endl;
    }
    */
    printf("do extract done\n");
}

void GFimg::writeDen(std::string sfile)
{
    //assert(NULL==sfile);
  //  assert(true == false);a
    GDALAllRegister();
    GDALDriver * poDriver;
    char ** papszMetadata;
    poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
    if( poDriver == NULL )
    {
        printf("GTiff is not supported.\n");
        return;
    }
    char **papszOptions = NULL; 
    GDALDataset *WriteDataSet = poDriver->Create(sfile.c_str(), width,height,1,GDT_Float32,papszOptions);
    int poBandMap[1] ={1};
    if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,density,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
    {
            printf("i_write failed.\n");
      //      break;
    }
   // delete WriteDataSet;
    printf("write to tiff done\n");
    GDALClose((GDALDatasetH)WriteDataSet);
}

void GFimg::num2geo(int col, int row, float * res)
{
    float leftlat = tllat + (bllat-tllat)/height* row;
    float leftlong = tllong + (bllong-tllat)/height* row;
    float rightlat = trlat + (brlat-trlat)/height* row;
    float rightlong = trlong + (brlong-trlat)/height* row;
    res[0] = leftlat + (rightlat-leftlat)/width * col;
    res[1] = leftlong + (rightlong - leftlong)/ width * col;
    
}
int GFimg::geo2num(float num1, float num2, int hw, float num3)
{
    if(num3 > num2)
        return hw;
    float dif = (num2- num1)/hw;
    return floor((num3- num1)/dif);
}

bool GFimg::copyrpb( std::string dest)
{
    std::string tempsfile = this->savedir + DIR_SEPERATOR + dest;
    std::ofstream out;
    std::ifstream in;
    in.open(this->rpbfile.c_str(),std::ios::in);
    if(!in.is_open()) {
        printf("%s open failed",this->rpbfile.c_str());
        return false;
    }
    out.open(tempsfile.c_str());
    if(!out.is_open()) {
        printf("%s create failed",tempsfile.c_str());
        return false;
    }
    in.seekg(0,in.end);
    long size = in.tellg();
    in.seekg(0);
    char* buffer = new char[size];
    in.read(buffer, size);
    out.write(buffer,size);
    delete[] buffer;
    out.close();
    in.close();
    return true;
}


void GFimg::write(write_var var,std::string sfile)
{
   std::string tempsfile = this->savedir + DIR_SEPERATOR + sfile;
   printf("Save full path: %s", tempsfile.c_str());
    // save the ndvi variable
    if ( var == var_ndvi )
    {
        assert(ndvi != NULL);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL;
        printf("write width:%d, height:%d",width, height);
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,ndvi,width,height,GDT_Float32,1,poBandMap,0,0,0) != CE_None)
        {
            printf("write ndvi failed.\n");
            return;
        //      break;
        }
        // delete WriteDataSet;

        printf("write ndvi done\n");
        GDALClose(WriteDataSet);
   //     GDALDestroyDriverManager();

    }
    // save the distance variable
    else if ( var == var_dist )
    {
        assert(distance != NULL);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL; 
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,distance,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write distance failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        printf("write distance done\n");
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    
    }else if ( var == var_density)
    {
        assert( NULL != density);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL; 
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,density,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write density failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        printf("write density done\n");
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }else if (var == var_ndwi)
    {
        assert( NULL != ndwi);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL; 
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,ndwi,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write ndwi failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    
    }else if (var == var_wet)
    {
        assert( NULL != wet);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL; 
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,wet,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write wet failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }else if (var == var_svi)
    {
        assert( NULL != svi);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL; 
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,svi,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write wet failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }else if (var == var_reflectance)
    {
        
        assert( NULL != band1);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL; 
        std::string b1file = this->savedir + DIR_SEPERATOR+ "band1"+sfile;
        GDALDataset *WriteDataSet1 = poDriver->Create(b1file.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet1->RasterIO(GF_Write,0,0,width,height,band1,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write band1 failed.\n");
        }
        std::string b2file =this->savedir + DIR_SEPERATOR+ "band2"+sfile;
        GDALDataset *WriteDataSet2 = poDriver->Create(b2file.c_str(), width,height,1,GDT_Float32,papszOptions);
        if(WriteDataSet2->RasterIO(GF_Write,0,0,width,height,band2,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write band2 failed.\n");
        }
        std::string b3file =this->savedir + DIR_SEPERATOR+ "band3"+sfile;
        GDALDataset *WriteDataSet3 = poDriver->Create(b3file.c_str(), width,height,1,GDT_Float32,papszOptions);
        if(WriteDataSet3->RasterIO(GF_Write,0,0,width,height,band3,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write band3 failed.\n");
        }
        std::string b4file = this->savedir + DIR_SEPERATOR+"band4"+sfile;
        GDALDataset *WriteDataSet4 = poDriver->Create(b4file.c_str(), width,height,1,GDT_Float32,papszOptions);
        if(WriteDataSet4->RasterIO(GF_Write,0,0,width,height,band4,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write band4 failed.\n");
        }
        WriteDataSet1->FlushCache();
        WriteDataSet2->FlushCache();
        WriteDataSet3->FlushCache();
        WriteDataSet4->FlushCache();

        GDALClose((GDALDatasetH)WriteDataSet1);
        GDALClose((GDALDatasetH)WriteDataSet2);
        GDALClose((GDALDatasetH)WriteDataSet3);
        GDALClose((GDALDatasetH)WriteDataSet4);

        // delete WriteDataSet;
        
    }else if (var == var_isShade)
    {
        assert( NULL != isShade);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL; 
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,isShade,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write isShade failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }else if(var == var_bright) {
        assert( NULL != bright);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL;
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,bright,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write isShade failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }else if (var == var_wet) {
        assert( NULL != wet);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL;
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,wet,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write isShade failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }
    else if (var == var_green) {
        assert( NULL != green);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL;
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,green,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write isShade failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }
    else if(var == var_urban) {
        assert(false);
        assert(urban != NULL);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        char **papszOptions = NULL;
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,urban,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write isShade failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }
    else if(var == var_risk) {
        assert(risk != NULL);
        GDALDriver * poDriver;
        char ** papszMetadata;
        poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
        char **papszOptions = NULL;
        GDALDataset *WriteDataSet = poDriver->Create(tempsfile.c_str(), width,height,1,GDT_Float32,papszOptions);
        int poBandMap[1] ={1};
        if(WriteDataSet->RasterIO(GF_Write,0,0,width,height,risk,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write isShade failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        WriteDataSet->FlushCache();
        GDALClose((GDALDatasetH)WriteDataSet);
    }
    else
    {
        printf("unknown save type");
    }
   // copyrpb(sfile);
}

void GFimg::extractValueByFile(std::string geofile, std::string savefile)
{
    std::string tempsavefile = this->savedir + DIR_SEPERATOR + savefile;
    printf("extractValueByFile...\n");
    readGeoinfo(geofile);
    std::ofstream ofile(tempsavefile.c_str());
    if(!ofile.is_open())
    {
        printf("failed to open the savefile:%s\n",tempsavefile.c_str());
        return;
    }
    for(int count = 0; count < insituPoints ; count++)
    {
        float li[4];float res[WANTED_PARAM]={0};
        li[0] = geoinfo[count * 4 ];
        li[1] = geoinfo[count * 4 + 1];
        li[2] = geoinfo[count * 4 + 2];
        li[3] = geoinfo[count * 4 + 3];
        printf("line:(%f,%f)-> (%f,%f)\n", li[0],li[1],li[2],li[3]);
        getAllValueNearLine(li, res);
        printf("res:(%f,%f,%f,%f)\n", res[0],res[1],res[2],res[3]);
        for(int i = 0; i < WANTED_PARAM; i++)
        {
            ofile<< res[i] << " ";
        }   
        ofile << std::endl;
    }
  //  free(geoinfo); 
}

void GFimg::extractPOIs( Product pro, std::string pois, std::string out) {
    printf("GFimg::extractPOIs");
    if(pois.compare(INVILIDPOIS) == 0) {
        printf("There are no valid input poi file");\
        return;
    }
    std::ifstream ifile;
    ifile.open(pois.c_str(), std::ios::in);
    if(!ifile.is_open()) {
        printf("%s open failed", pois.c_str());
        return;
    }

    std::ofstream ofile;
    ofile.open(out.c_str(), std::ios::out);
    if(!ofile.is_open()) {
        printf("extract file : %s failed to open", out.c_str());
        return;
    }

    int count = 0;
    char buf[1024];
    while(ifile.getline(buf,sizeof(buf))) {
        count++;
    }
    ifile.close();
    ifile.open(pois.c_str(), std::ios::in);
    //ifile.seekg(0,std::ios_base::beg);
    double * lat_t = new double[count];
    double * lon_t = new double[count];
    double * height_t = new double[count];
    float *lat_bak = new float[count];
    float *lon_bak = new float[count];
    float * height_bak = new float[count]; // transfer to double for save row and col
    int *nSuccess = new int[count];
    int c = 0;
    while(ifile.getline(buf,sizeof(buf))) {
        sscanf(buf,"%f %f %f", &lon_bak[c], &lat_bak[c], &height_bak[c]);
        ++c;
    }


    for(int idx = 0; idx < count; idx++) {
        lat_t[idx] =  lat_bak[idx];
        lon_t[idx] = lon_bak[idx];
        height_t[idx] = height_bak[idx];
    }
  //  for(int idx = 0; idx < count; idx++) {
  //      printf("Latitude,Longitude:[%f,%f]",
  //             lat_t[idx], lon_t[idx]);
  //  }
    GDALAllRegister();
    GDALDataset * pDS = (GDALDataset *) GDALOpen(filename.c_str(), GA_ReadOnly);
    char ** papszRPC = pDS->GetMetadata("RPC");
    GDALRPCInfo oInfo;
    GDALExtractRPCInfo(papszRPC, &oInfo);

    GDALClose((GDALDatasetH)pDS);

    char ** papszTransOption = NULL;
    void * pRPCTransform = GDALCreateRPCTransformer(&oInfo, FALSE,0, papszTransOption);
    // input longitude && latitude
    // output column && rows
    GDALRPCTransform(pRPCTransform, TRUE, count, lon_t, lat_t, height_t, nSuccess);
    int* latidx = new int[count];
    int *lonidx = new int[count];
//    for(int idx = 0; idx < count; idx++) {
//        printf("Latitude,Longitude:[%f,%f],Row,Col:[%d,%d]",
//               lat_bak[idx], lon_bak[idx], latidx[idx]=round(lat_t[idx]), lonidx[idx]=round(lon_t[idx]));
//    }

    for(int idx = 0; idx < count; idx++) {
        latidx[idx]=round(lat_t[idx]);lonidx[idx]=round(lon_t[idx]);
    }
    printf("begin to write");
    ofile << "Lat" << " " << "Long" << " " <<"Row" <<" " << "Col" << " "<< "Band1" << " " << "Band2" << " " << "Band3" << " "
          << "Band4";

    if(pro.ndvi) {
        ofile << " " << "ndvi";
    }
    if(pro.ndwi) {
        ofile << " " << "ndwi";
    }
    if(pro.svi) {
        ofile << " " << "svi";
    }
    if(pro.distance) {
        ofile << " " << "distance";
    }
    if(pro.kt) {
        ofile << " " << "bright" << " " << "wet" << " " << "green";
    }
    if(pro.density) {
        ofile << " " << "density";
    }
    ofile<< std::endl;
    for(int idx = 0; idx < count; idx++) {
        ofile << lat_bak[idx] << " " << lon_bak[idx];
        ofile<<" " << latidx[idx] << " " << lonidx[idx];

        if(latidx[idx] > (height-1) || lonidx[idx] > (width-1) || latidx[idx] < 0 || lonidx[idx] < 0) {
            ofile<<" -1 -1 -1 -1"<<std::endl;
            continue;
        }
        int num = latidx[idx] * width + lonidx[idx];

        ofile <<" "<< band1[num] << " " << band2[num] << " "
                                      << band3[num] << " "<< band4[num];
        if(pro.ndvi) {
            ofile << " " << ndvi[num];
        }
        if(pro.ndwi) {
            ofile << " " << ndwi[num];
        }
        if(pro.svi) {
            ofile << " " << svi[num];
        }
        if(pro.distance) {
            ofile << " " << getDistance2water(Pixel2Water,latidx[idx],lonidx[idx]);// row col
        }
        if(pro.kt) {
            ofile << " " << bright[num] << " " << wet[num] << " " << green[num];
        }
        if(pro.density) {
            ofile << " " << density[num];
        }
        ofile<<std::endl;
    }

    ifile.close();
    ofile.close();
    delete latidx, lat_bak, lat_t;
    delete lonidx, lon_bak, lon_t;
    delete height_bak, height_t;
    delete nSuccess;
    printf("GFimg::extractPOIs done");
}

void GFimg::extractUrban(const Product pro, std::string savefile) {
    if(urban == NULL) {
        getUrban();
    }
    std::ofstream ofile;
    ofile.open(savefile.c_str(), std::ios::out);
    if(!ofile.is_open()) {
        printf("extract file : %s failed to open", savefile.c_str());
        return;
    }

    GDALAllRegister();
    GDALDataset * pDS = (GDALDataset *) GDALOpen(filename.c_str(), GA_ReadOnly);
    char ** papszRPC = pDS->GetMetadata("RPC");
    GDALRPCInfo oInfo;
    GDALExtractRPCInfo(papszRPC, &oInfo);

    GDALClose((GDALDatasetH)pDS);

    char ** papszTransOption = NULL;
    void * pRPCTransform = GDALCreateRPCTransformer(&oInfo, FALSE,0, papszTransOption);
    int nSuccess;
    double lon_t;
    double lat_t;
    double h;
    printf("begin to write");
    ofile << "Lat" << " " << "Long" << " " <<"Row" <<" " << "Col" << " "<< "Band1" << " " << "Band2" << " " << "Band3" << " "
          << "Band4";

    if(pro.ndvi) {
        ofile << " " << "ndvi";
    }
    if(pro.ndwi) {
        ofile << " " << "ndwi";
    }
    if(pro.svi) {
        ofile << " " << "svi";
    }
    if(pro.distance) {
        ofile << " " << "distance";
    }
    if(pro.kt) {
        ofile << " " << "bright" << " " << "wet" << " " << "green";
    }
    if(pro.density) {
        ofile << " " << "density";
    }
    ofile<< std::endl;
    for(int row = 0; row < height; row+=10) {
        for(int col =0; col < width; col+=10) {
            int num = row * width + col;
            if(urban[num]) {
               // input longitude && latitude
                // output column && rows
                lon_t = col;
                lat_t = row;
                GDALRPCTransform(pRPCTransform, FALSE, 1, &lon_t, &lat_t, &h, &nSuccess);
                ofile << lat_t << " " << lon_t;
                ofile<<" " << row << " " << col;


                ofile <<" "<< band1[num] << " " << band2[num] << " "
                                              << band3[num] << " "<< band4[num];
                if(pro.ndvi) {
                    ofile << " " << ndvi[num];
                }
                if(pro.ndwi) {
                    ofile << " " << ndwi[num];
                }
                if(pro.svi) {
                    ofile << " " << svi[num];
                }
                if(pro.distance) {
                    ofile << " " << getDistance2water(Pixel2Water,row,col);// row col
                }
                if(pro.kt) {
                    ofile << " " << bright[num] << " " << wet[num] << " " << green[num];
                }
                if(pro.density) {
                    ofile << " " << density[num];
                }
                if(pro.risk) {
                    ofile<<" " << risk[num];
                }
                ofile<<std::endl;
            }
        }
    }
    ofile.close();
}





void GFimg::getUrban() {
    printf("getUrban()...\n");
    urban = (bool *) CPLMalloc(sizeof(bool)* width * height);
    assert(urban != NULL);
    size_t iLimit = height*width;
    for(size_t i = 0; i < iLimit; i++)
        {
            urban[i] = (band1[i]+band2[i]+band3[i]) > URBANINDEX ;
        }
}

void GFimg::CalRisk() {
    printf("GFimg::CalRisk...");
    size_t iLimit = height*width;
    if(density == NULL) {
        caldensity(SVI_DIST);
    }
    bool * isThere =  (bool *) CPLMalloc(sizeof(bool)* width * height);
    for(size_t i = 0; i < iLimit; i++) {
        isThere[i] = (density[i] >= 0); // > URBANINDEX ? true: false;
    }
    // malloc the mat of distance
    risk = (float *) CPLMalloc(sizeof(float) * width * height);
    if(risk == NULL) {
        printf("malloc the risk space failed\n");
        return;
    }
    printf("malloc the distance sucess\n");
    // init the disint
    for (int row = 0; row < height; row++)
        for (int col = 0; col < width; col++)
        {
            int idx = row * width + col;

            risk[idx] = RISKMINMUM;
        }
    printf("GFimg::calRisk():distance init success.\n");
    // for each pixel

//     NearBody * nb = new NearBody();
    for (int row = 1; row < height-1; row++) {
      //  printf("row: %d", row);
        for (int col = 1; col < width-1; col++)
        {
            int idx = row * width + col;
            // if  density false,  continue
            if(!isThere[idx])
            {
                risk[idx] = density[idx];
                continue;
            }
            // find the neighbour water pixels
            int up = (row - 1) * width + col;
            int down = (row - 1) * width + col;
            int left = idx - 1;
            int right = idx + 1;
            // if all near pixel are water , then skip
            if (isThere[up] && isThere[down] && isThere[left] && isThere[right])
            {
                risk[idx] = density[idx];
                continue;
            }
            // else if the near pixel all are not water
            else if (!(isThere[up] || isThere[down] || isThere[left] || isThere[right] ))
            {
                // then calculate distance for each pixel
                for ( int ii = -NEAR_POINTS_NUM+1; ii < NEAR_POINTS_NUM; ii++)
                    for ( int jj = -NEAR_POINTS_NUM+1; jj < NEAR_POINTS_NUM; jj++)
                    {
                        if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                            continue;
                        int nearidx = (row + ii) * width + col+jj;
                        if(isThere[nearidx])
                            continue;
                        float d = sqrt(pow(ii,2)+pow(jj,2));
                        float ri = density[nearidx]/d;
                        if (ri > risk[nearidx])
                            risk[nearidx] = ri;
                    }
            }
            // else pixel the near pixeles are water
            else{
                // if up and down are both not water
                if(!(isThere[up]||isThere[down]))
                {
                    // if left is water
                    if(isThere[left])
                    {
                        // calculate the distance for the right part
                        for ( int ii = -NEAR_POINTS_NUM+1; ii < NEAR_POINTS_NUM; ii++)
                        for ( int jj = 0; jj < NEAR_POINTS_NUM; jj++)
                        {
                            if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                                continue;
                            int nearidx = (row + ii) * width + col+jj;
                            if(isThere[nearidx])
                                continue;
                            float d = sqrt(pow(ii,2)+pow(jj,2));
                            float ri = density[nearidx]/d;
                            if (ri > risk[nearidx])
                                risk[nearidx] = ri;
                        }
                    }
                    // else the right pixel is water
                    else if (isThere[right])
                    {
                        // calculate the distance for the left part
                        for ( int ii = -NEAR_POINTS_NUM+1; ii < NEAR_POINTS_NUM; ii++)
                        for ( int jj = -NEAR_POINTS_NUM+1; jj <= 0; jj++)
                        {
                            if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                                continue;
                            int nearidx = (row + ii) * width + col+jj;
                            if(isThere[nearidx])
                                continue;
                            float d = sqrt(pow(ii,2)+pow(jj,2));
                            float ri = density[nearidx]/d;
                            if (ri > risk[nearidx])
                                risk[nearidx] = ri;
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
                    if(isThere[up])
                    {
                        // calculate the distance for the down part
                        for ( int ii = 0; ii < NEAR_POINTS_NUM; ii++)
                        for ( int jj = -NEAR_POINTS_NUM+1; jj < NEAR_POINTS_NUM; jj++)
                        {
                            if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                                continue;
                            int nearidx = (row + ii) * width + col+jj;
                            if(isThere[nearidx])
                                continue;
                            float d = sqrt(pow(ii,2)+pow(jj,2));
                            float ri = density[nearidx]/d;
                            if (ri > risk[nearidx])
                                risk[nearidx] = ri;
                        }
                    }
                    // else if the down pixel is water
                    else if (isThere[down])
                    {
                        // calculate the distance for the up part
                        for ( int ii = -NEAR_POINTS_NUM+1; ii <= 0; ii++)
                        for ( int jj = -NEAR_POINTS_NUM+1; jj < NEAR_POINTS_NUM; jj++)
                        {
                            if (row+ii < 0 || row + ii > height-1 || col +jj < 0 || col + jj > width-1)
                                continue;
                            int nearidx = (row + ii) * width + col+jj;
                            if(isThere[nearidx])
                                continue;
                            float d = sqrt(pow(ii,2)+pow(jj,2));
                            float ri = density[nearidx]/d;
                            if (ri > risk[nearidx])
                                risk[nearidx] = ri;
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
    CPLFree(isThere); isThere = NULL;
    printf("GFimg::getDistance2water():distance calculate  success.\n");
    if(urban == NULL) {
        getUrban();
    }
    // set the far pixel as the FILLVALUE the keep the influence range is circle
    for ( int row = 0; row < height; row++)
       for ( int col = 0; col < width; col++)
       {
           int idx = row * width + col;
           if(urban[idx]) {
   //             risk[idx] = URBANRISKVALUE;
           }
       }
}

void GFimg::loadData(std::string file, write_var var) {
    printf("GFimg::loadData begin");
    if(var == var_dist) {
        if(distance == NULL) {
            distance = (float *) CPLMalloc(sizeof(float) * width * height);
        }
        assert(distance != NULL);
        GDALDriver * poDriver;
        char ** papszMetadata;

        if( poDriver == NULL )
        {
            printf("GTiff is not supported.\n");
            return;
        }
        char **papszOptions = NULL;
        GDALDataset *pDataset = (GDALDataset *)GDALOpen(filename.c_str(),GA_ReadOnly);
        int poBandMap[1] ={1};
        if(pDataset->RasterIO(GF_Read,0,0,width,height,distance,width,height,GDT_Float32,1,poBandMap,0,0,0)==CE_Failure)
        {
            printf("write distance failed.\n");
        //      break;
        }
        // delete WriteDataSet;
        printf("write distance done\n");
        GDALClose((GDALDatasetH)pDataset);
    }
    printf("GFimg::loadData done");
}
// EOF
