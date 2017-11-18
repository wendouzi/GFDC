#include "io.h"
#include "Image.h"
#include "NXLog.h"

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


}