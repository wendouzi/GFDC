
#ifndef GFIMG_H
#define GFIMG_H
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "geoshape.h"
#include <utility>
#include "CCBaseTypeDefine.h"
#include "products.h"
#define ST_WFV1 0 // SensorType WFV1
#define ST_WFV2 1 // SensorType WFV2
#define ST_WFV3 2 // SensorType WFV3
#define ST_WFV4 3 // SensorType WFV4
#define RESOLUTION 8
#define NEAR_POINTS_NUM 600
#define FILLVALUE 1000
#define WATERFILLVALUE 0
#define NDVI_WATERINDEX (-0.1)
#define NDVI_CENTER (0.5)
#define NDWI_MIN (-5)
#define NDWI_MAX (5)
#define NDWI_WATERINDEX (0)
#define URBANINDEX 0.10
#define WATERVALUE   (-1)
#define RATIO43_WATERINDEX (3.5)
#define RATIO43_MIN (-5)
#define RATIO43_MAX (5)
#define INVILIDPOIS  "invalid.poi.file"
#define URBANRISKVALUE (0)
#define RISKMINMUM   (0.0)

#define GETGFIMG (GFimg::getInstance())

#ifdef __WINDOWS_
#define DIR_SEPERATOR "\\"
#else
#define DIR_SEPERATOR "/"
#endif
    enum Method 
    {
        withoutDist = 0,
        withDist_1,
        SVI_DIST,
        TEST_CASE
    };
    enum write_var
    {
        var_ndvi = 0,
        var_ndwi,
        var_svi,
        var_dist,
        var_bright,
        var_wet,
        var_green,
        var_isShade,
        var_reflectance,
        var_density,
        var_urban,
        var_risk
    };
    enum DistAlgo
    {
        Pixel2Water = 0,
        Water2Pixel,
        POI_SVI_DIST
    };
    enum print_var
    {
        print_reflectance = 0,
        print_ndvi,
        print_nd,
        print_ndwi,
        print_svi,
        print_distance,
        print_density
    };

struct NearBody {
    bool up;
    bool down;
    bool left;
    bool right;
};

class GFimg
{
    private:
        std::string filename;
        int SensorType;
        std::string xmlfile;
        std::string rpbfile;
        std::string savedir;
        float tllat,tllong,trlat,trlong,bllat,bllong,brlat,brlong;

        float * latitude;
        float * longitude;
        float * ndvi;
        bool * wi;
        bool * urban;
        float * svi;
        float * ndwi;
        float * bright;
        float * green;
        float * wet;
        float * dis;
        float * distance;
        float * density;
        float * risk;
        float * isShade;
        int insituPoints;
        float * geoinfo; // latitude and longitude  of in situ points
     //   float * latitude;
     //   float * longitude;

//        std::vector<vector<int>> colrow;
        std::vector<int> i_colrow;
        std::pair<int, int> _leftup;
        std::pair<int, int> _rightdown;
     //   bool wantedProducts[TOTAL_PRODUCTS];
       // std::vector<int> i_row;
     //   std::vector<float> i_lat;
     //   std::vector<float> i_long;
    public:
        float * band1;
        float * band2;
        float * band3;
        float * band4;
        int width;
        int height;
    private:
        static GFimg * m_pGF;
    public:
        GFimg(const std::string  _filename,
                const  std::string _xmlfile,
                const std::string _rpbfile,
                const std::string savedir,
                const int  _SensorType = 0);
        ~GFimg();
        void init();
        void getNDVI();
        void getNDWI();
        void getSVI();
        void getWI();
        void getUrban();
        void getIsShade();
        void getKTtransform();
        void getDistance2water(DistAlgo da);
        float getDistance2water(DistAlgo da, int row, int col);
        void getDistance2water();
        void caldensity();
        void caldensity(Method me = TEST_CASE);
        void generate_Lat_Long();
        void getAllValueNearLine(float * li, float * res);
        void getValueByPoints(float * li, std::vector<int> * colrowtemp);
        void extractValueByFile(std::string geofile,std::string savefile);
        void writePOIs(std::string poifile, std::string savefile);
        void readRpbCoefs(const std::string rf = FILE_EMPTY);
        void write(write_var var, std::string sfile);
        void print(print_var var = print_reflectance);
        int geo2num(float num1, float num2, int hw, float num3);
        void num2geo(int col, int row, float * res);
        void writeDen(std::string sfile);
        void print_lat_long();
        void doextract(std::string savefile);
        void extractAll(std::string savefile);
        void readGeoinfo(std::string geofile  = FILE_EMPTY);
        void getCorner(); // extract the latitude and longitude corner
        void checkNearWater(NearBody * nb , int row, int col);
        // pair<列号，行号> // //
        void areacount(std::pair<int,int> leftup, std::pair<int, int> rightdown );
        void setPOI(std::pair<int,int> leftup, std::pair<int, int> rightdown );
        int getPOIArea(std::pair<int,int> leftup, std::pair<int, int> rightdown );
        bool copyrpb(std::string dest);
        void extractPOIs( const  Product pro, const std::string pois = INVILIDPOIS,const std::string out = INVILIDPOIS);
    //    void setWantedProducts(std::string prods);
        void CalRisk();
        void extractUrban(const Product pro, std::string savefile);
        void loadData(std::string file, write_var var);
public:
        static GFimg* getInstance();
};

#endif /* GFIMG_H*/
