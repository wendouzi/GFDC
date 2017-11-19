#include "io.h"
#include "Image.h"
#include "NXLog.h"
#include <typeinfo> 
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
    if (boost::filesystem::exists(spath){
        NXLog("%s, file is exists\n", __PRETTY_FUNCTION__); 
        return;       
    }
    NXLog("Save full path: %s", spath.string().c_str());

    GDALDriver * poDriver;
    char ** papszMetadata;
    poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
    if( poDriver == NULL )
    {
        NXLog("GTiff is not supported.\n");
        return;
    }
    char **papszOptions = NULL;
    NXLog("write width:%d, height:%d",src.width, src.height);
    GDALDataset *WriteDataSet = poDriver->Create(spath.string().c_str(), src.width,  src.height,1,GDT_Float32, papszOptions);
    int poBandMap[1] ={1};
    if(WriteDataSet->RasterIO(GF_Write,0,0,src.width,src.height,src.getBandPtr(0),src.width,src.height,GDT_Float32,1,poBandMap,0,0,0) != CE_None)
    {
        NXLog("write % failed.\n", spath.string().c_str());
        return;
    }

    NXLog("write ndvi done\n");
    GDALClose(WriteDataSet);

}

}