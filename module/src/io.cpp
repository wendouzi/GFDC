#include "io.h"
#include "Image.h"
#include "NXLog.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include <iterator>
#include <fstream>
#include <iostream>
namespace wendouzi{

bool IO::RasterRead(const std::string& filename, Image_u16 * img)
{
    GDALAllRegister();
    GDALDataset * pDataset = NULL;
    pDataset = (GDALDataset *)GDALOpen(filename.c_str(),GA_ReadOnly);
    if(pDataset == NULL)
    {
            NXLog("File to open file: %s\n", filename.c_str());
            return false;
    }
    
    double adfGeoTransform[6];
    pDataset->GetGeoTransform(adfGeoTransform);
    std::vector<double> adf(std::begin(adfGeoTransform), std::end(adfGeoTransform));
    img->setGeoTransform(adf);

    int nRasterCount = GDALGetRasterCount(pDataset);
    int width = GDALGetRasterXSize(pDataset);
    int height = GDALGetRasterYSize(pDataset);
    img->init(width, height, nRasterCount);
    NXLog("%s, nRasterCount:%d,width:%d,height:%d\n",__PRETTY_FUNCTION__, nRasterCount,width,height);

    for (int idx = 1; idx <= nRasterCount; ++idx){
        GDALRasterBand * pBand = pDataset->GetRasterBand(idx);
        pBand->RasterIO(GF_Read,0,0, width,height,img->getBandPtr(idx-1),
            width,height,GDT_UInt16,0,0);
    }
    GDALClose(pDataset);
    NXLog("%s read done\n", __PRETTY_FUNCTION__);
    return true;
}

bool IO::RasterWrite(const Image_f & src, const std::string & dir, const std::string & file)
{
    boost::filesystem::path spath(dir);
    spath /= file;
    if (boost::filesystem::exists(spath)){
        NXLog("%s, file is exists\n", __PRETTY_FUNCTION__);

        return false;       
    }
    NXLog("Save full path: %s", spath.string().c_str());

    GDALDriver * poDriver;
    char ** papszMetadata;
    poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
    if( poDriver == NULL )
    {
        NXLog("GTiff is not supported.\n");
        return false;
    }
    char **papszOptions = NULL;
    NXLog("write width:%d, height:%d",src.width, src.height);
    GDALDataset *WriteDataSet = poDriver->Create(spath.string().c_str(), src.width,  src.height,1,GDT_Float32, papszOptions);

    double adfGeoTransform[6];
    for (int idx = 0; idx < 6; ++idx) {
        adfGeoTransform[idx] = src.adfGeoTransform[idx];
    }
    //! TO DO add geo transform
    WriteDataSet->SetGeoTransform(adfGeoTransform);
    int poBandMap[1] ={1};
    if(WriteDataSet->RasterIO(GF_Write,0,0,src.width,src.height,src.getBandPtr(0),src.width,src.height,GDT_Float32,1,poBandMap,0,0,0) != CE_None)
    {
        NXLog("write % failed.\n", spath.string().c_str());
        return false;
    }

    NXLog("write ndvi done\n");
    GDALClose(WriteDataSet);
    return true;
}

bool IO::RasterWrite(const Image_int & src, const std::string & dir, const std::string & file)
{
    boost::filesystem::path spath(dir);
    spath /= file;
    if (boost::filesystem::exists(spath)){
        NXLog("%s, file is exists\n", __PRETTY_FUNCTION__);

        return false;       
    }
    NXLog("Save full path: %s\n", spath.string().c_str());

    GDALDriver * poDriver;
    char ** papszMetadata;
    poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
    if( poDriver == NULL )
    {
        NXLog("GTiff is not supported.\n");
        return false;
    }
    char **papszOptions = NULL;
    NXLog("write width:%d, height:%d\n",src.width, src.height);
    GDALDataset *WriteDataSet = poDriver->Create(spath.string().c_str(), src.width,  src.height,1,GDT_Int32, papszOptions);

    double adfGeoTransform[6];
    for (int idx = 0; idx < 6; ++idx) {
        adfGeoTransform[idx] = src.adfGeoTransform[idx];
    }
    //! TO DO add geo transform
    WriteDataSet->SetGeoTransform(adfGeoTransform);
    int poBandMap[1] ={1};
    if(WriteDataSet->RasterIO(GF_Write,0,0,src.width,src.height,src.getBandPtr(0),src.width,src.height,GDT_Int32,1,poBandMap,0,0,0) != CE_None)
    {
        NXLog("write % failed.\n", spath.string().c_str());
        return false;
    }

    NXLog("write  done\n");
    GDALClose(WriteDataSet);
    return true;
}

bool IO::RasterWrite(const Image_b & src, const std::string & dir, const std::string & file)
{
    boost::filesystem::path spath(dir);
    spath /= file;
    if (boost::filesystem::exists(spath)){
        NXLog("%s, file is exists\n", __PRETTY_FUNCTION__);

        return false;       
    }
    NXLog("Save full path: %s\n", spath.string().c_str());

    GDALDriver * poDriver;
    char ** papszMetadata;
    poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
    if( poDriver == NULL )
    {
        NXLog("GTiff is not supported.\n");
        return false;
    }
    char **papszOptions = NULL;
    NXLog("write width:%d, height:%d\n",src.width, src.height);
    GDALDataset *WriteDataSet = poDriver->Create(spath.string().c_str(), src.width,  src.height,1,GDT_Byte, papszOptions);

    double adfGeoTransform[6];
    for (int idx = 0; idx < 6; ++idx) {
        adfGeoTransform[idx] = src.adfGeoTransform[idx];
    }
    //! TO DO add geo transform
    WriteDataSet->SetGeoTransform(adfGeoTransform);
    int poBandMap[1] ={1};
    if(WriteDataSet->RasterIO(GF_Write,0,0,src.width,src.height,src.getBandPtr(0),src.width,src.height,GDT_Byte,1,poBandMap,0,0,0) != CE_None)
    {
        NXLog("write % failed.\n", spath.string().c_str());
        return false;
    }

    NXLog("write  done\n");
    GDALClose(WriteDataSet);
    return true;
}

bool IO::Output(const AreaCount & count, const std::string & savedir, const std::string & file)
{
    NXLog("%s\n", __PRETTY_FUNCTION__);
    boost::filesystem::path spath(savedir);
    spath /= file;
    if (boost::filesystem::exists(spath)){
        NXLog("%s, file is exists\n", __PRETTY_FUNCTION__);
        return false;       
    }
    NXLog("Save full path: %s\n", spath.string().c_str());
    std::ofstream outfile;
    outfile.open(spath.string().c_str(), std::ios::out | std::ios::app);
    if (!outfile.is_open()){
        NXLog("%s, write file is not opended\n", __PRETTY_FUNCTION__);
        return false;
    }
    for (auto it = count.result.begin(); it != count.result.end(); ++it) {
        std::cout <<"level : " << it->first << " , area is : " << it->second * count.pixelArea << "  m*m" << std::endl;
        outfile <<"level : " << it->first << " , area is : " << it->second * count.pixelArea << "  m*m" << std::endl;
    }

    outfile.close();
    return true;
}


}