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

bool IO::RasterWrite(const Image_f & src, const std::string & dir)
{
    if (dir.empty()){
        NXLog("%s, dir is empty\n", dir.c_str());        
    }
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

}

}