
#include <stdio.h>
#include <string>
#include "geoshape.h"
#include "GFimg.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "CCBaseTypeDefine.h"
using namespace boost::filesystem;
std::vector<std::string> vector_files;
int main(int argc, char * argv[])
{
    if(argc<3)
    {
        printf("need input the directory\n");
        printf("Usage: \n  GFCD.exe <your_path> <save_path>\n <your_path> mean the directory where you save the Gao Fen image!\n <save_path> mean the directory where you want to save the result!\n");
        return 0;
    }
    if(argc>3)
    {
        printf("Do not input more than two dicectory\n");
        printf("Usage: \n  GFCD.exe <your_path> <save_path>\n <your_path> mean the directory where you save the Gao Fen image!\n <save_path> mean the directory where you want to save the result!\n");
    }
    std::string full_path = argv[1];
    std::string savepath = std::string(argv[2]);
    std::string saveimg = savepath + "saveimg.tiff";
    if(is_directory(full_path) && is_directory(argv[2]))
    {
        printf("begin.....\n");
    }else
    {
        printf("the fist or second input is not a directory");
    }
    directory_iterator end;
  for(directory_iterator pos(full_path);pos !=end; ++pos){
    boost::filesystem::path path_(*pos);
    if(is_regular(path_)){
        std::string s= path_.string();
      vector_files.push_back(s);
    }
  }
  std::string filename = " ";
  std::string xmlfile = " ";
  std::string rpbfile = " ";
    for(int i=0; i < vector_files.size(); i++)
    {
        if(vector_files[i].find(".tiff") != std::string::npos)
        {    
            filename = vector_files[i];
             printf("filename:%s\n", vector_files[i].c_str());
        }
            else if(vector_files[i].find(".rpb") != std::string::npos)
            {
                rpbfile = vector_files[i];
                printf("rpbfile:%s\n", vector_files[i].c_str());
            }
                else if(vector_files[i].find(".xml") != std::string::npos)
                {
                    xmlfile = vector_files[i];
                    printf("xmlfile:%s\n", vector_files[i].c_str());
                }
    }
    printf("filename:%s\n", filename.c_str());
    printf("rpbfile:%s\n", rpbfile.c_str());
    printf("xmlfile:%s\n", xmlfile.c_str());
   if(filename.compare(" ")==0 || xmlfile.compare(" ") ==0 || rpbfile.compare(" ")==0)
   {
    printf("There are not .tiff, or .rpb, or .xml file in your directory!\n");
    exit(1);
   }

/*
    std::string filename = "/home/yangxianping/workspace/gf/data/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473.tiff";
    std::string xmlfile = "/home/yangxianping/workspace/gf/data/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473.xml";
    std::string rpbfile = "/home/yangxianping/workspace/gf/data/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473.rpb";
    std::string geofile = "/home/yangxianping/workspace/gf/data/2015.txt";
    std::string savefile = "/home/yangxianping/workspace/gf/data/2015save.txt";
    std::string saveimg = "/home/yangxianping/workspace/gf/data/savedimg.tiff"; 
    */
 //    GFimg * gf = new GFimg(filename, 0, xmlfile,rpbfile);
 //    gf->getCorner();
 //    gf->init();
 //  // gf->print(); 
 //    gf->caldensity(withoutDist);
 //  //  gf->caldensity(withDist_1);
 //  //  gf->writeDen(saveimg);
 //  //  gf->extractValueByFile(geofile,savefile);
 // //   gf->readGeoinfo(geofile);
 //    delete gf; gf=NULL;


    printf("process");
    std::string sep = std::string(FILE_SEPERATOR);
    // QString sFilePath = _sdir+ sep+ QString("README.txt");
    // qDebug("test file path : %s", sFilePath.toStdString().c_str());
    // QFile file(sFilePath);
    // if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
    //     emit sig_to_schedule(RESULT_FAILED);
    //     qDebug("open README.txt failed ");
    //     return;
    // }
    std::string ALL_PRODUCTS[TOTAL_PRODUCTS] ={"ndvi","ndwi","svi","distance","KT","Density","Risk"};
    bool want[TOTAL_PRODUCTS] = {true, true, true, false, false, false, false};
    for(int i = 0; i < TOTAL_PRODUCTS; i++) {
        // want[i] = prods.at(i) == '1';
        printf("%s products want: %s\n", ALL_PRODUCTS[i].c_str(), want[i] ? "yes": "no");
    }

    // QDateTime time = QDateTime::currentDateTime();
    // QString str = time.toString("yyyy-MM-dd hh:mm:ss ddd");
    // QTextStream out(&file);
    // out<<QString("The GAOFEN image processed at : ")<< str.toStdString().c_str()<< QString("  by Disease Control Image Process System.")<<std::endl;
    // out<<QString("Copyright © 2016 Powered By Chinese Center For Disease Control and Prevention. All rights reserved. ") << std::endl;
    // out.flush();
    // if img exist, then return old pointer to old one
    // GFimg * m_pGF = IMGMANAGER->addImg(
    //  _tiff.toStdString(), _xml.toStdString(), _rpb.toStdString(), _sdir.toStdString());

    GFimg * m_pGF = new GFimg(filename, xmlfile,rpbfile, savepath);
    m_pGF->init();
    //GFimg * m_pGF = new GFimg(_tiff.toStdString(), _xml.toStdString(), _rpb.toStdString(), _sdir.toStdString());
    m_pGF->getCorner();
    //m_pGF->init();
    m_pGF->getUrban();
   // m_pGF->write(var_urban,"urban.tiff");
    Product pro;
    if(want[NDVI_ORDER]) {
        pro.ndvi = true;
        // if(cancelFlag::getInstance()->getCancelFlag()) {
        // }
        // else {
            std::cout<< "Begin to calculate NDVI " << std::endl;
            m_pGF->getNDVI();
            //m_pGF->print(print_ndvi);
            std::cout << "calculate NDVI done, begin to write to save directory"<< std::endl;
            m_pGF->write(var_ndvi, "ndvi.tiff");
            std::cout << ("write ndvi done") << std::endl;
        // }
    }
    if(want[NDWI_ORDER]) {
        pro.ndwi = true;
        // if(cancelFlag::getInstance()->getCancelFlag()) {
        // }
        // else {
            std::cout << ("Begin to calculate NDWI ") << std::endl;
            m_pGF->getNDWI();
            std::cout  << ("calculate NDWI done, begin to write to save directory")<< std::endl;
            m_pGF->write(var_ndwi, "ndwi.tiff");
            std::cout<< ("write ndwi done") << std::endl;
        // }
    }
    if(want[SVI_ORDER]) {
        pro.svi = true;
        // if(cancelFlag::getInstance()->getCancelFlag()) {
        // }
        // else {
            std::cout << ("Begin to calculate SVI ") << std::endl;
            m_pGF->getSVI();
            std::cout  << ("calculate SVI done, begin to write to save directory")<< std::endl;
            m_pGF->write(var_svi, "svi.tiff");
            std::cout << ("write svi done") << std::endl;
        // }
    }

    if(want[DISTANCE_ORDER]) {
        pro.distance = true;
        // if(cancelFlag::getInstance()->getCancelFlag()) {

        // }
        // else {
            std::string distancefile = savepath + sep + std::string("distance.tiff");
            if(boost::filesystem::exists(distancefile)) {
                // std::cout << QString("File \"") << distancefile<< QString("\" exist, Begin to load") << std::endl;
                m_pGF->loadData(distancefile, var_dist);
                std::cout  << ("load done")<< std::endl;

            }else {
                std::cout << ("Begin to calculate distance ") << std::endl;
                m_pGF->getDistance2water(Water2Pixel);
                std::cout  << ("calculate distance done, begin to write to save directory")<< std::endl;
                m_pGF->write(var_dist, "distance.tiff");
            }


            std::cout << ("write distance done") << std::endl;
        // }
    }
    if(want[KT_ORDER]) {
        pro.kt = true;
        // if(cancelFlag::getInstance()->getCancelFlag()) {

        // }
        // else {
            std::cout << ("Begin to calculate KT transform ") << std::endl;
            m_pGF->getKTtransform();
            std::cout  << ("calculate KT transform done, begin to write to save directory")<< std::endl;
            m_pGF->write(var_bright, "KT_bright.tiff");
            m_pGF->write(var_wet, "KT_wet.tiff");
            m_pGF->write(var_green,"KT_green.tiff");
            std::cout << ("write bright, wet, green done") << std::endl;
        // }
    }

    if(want[DENSITY_ORDER]) {
        pro.density = true;
        // if(cancelFlag::getInstance()->getCancelFlag()) {
        // }
        // else {
            std::cout << ("Begin to calculate DENSITY ") << std::endl;
            m_pGF->caldensity(SVI_DIST);
            std::cout << ("calculate DENSITY done, begin to write to save directory")<< std::endl;
            m_pGF->write(var_density, "svi_dist_density.tiff");
            std::cout << ("write density done") << std::endl;
        // }
    }
    if(want[RISK_ORDER]) {
        pro.risk = true;
        // if(cancelFlag::getInstance()->getCancelFlag()) {

        // }
        // else {
            std::cout << ("RISK function still not finished ") << std::endl;
            m_pGF->CalRisk();
            std::cout  << ("calculate RISK done, begin to write to save directory")<< std::endl;
            m_pGF->write(var_risk, "risk.tiff");
            std::cout<< ("write risk done") << std::endl;
        // }
    }
    // if(_poi.compare(FILE_EMPTY) != 0) {
    //     // if(cancelFlag::getInstance()->getCancelFlag()) {
    //     // }
    //     // else {
    //         std::cout << ("Begin to extract POIs ") << std::endl;
    //         // QString extractFilePath = _sdir+ sep+ QString("extractResult.txt");
    //         // QString urbanextract = _sdir+ sep+ QString("extractUrban.txt");
    //     //    m_pGF->generate_Lat_Long();
    //     //    m_pGF->readGeoinfo(_poi.toStdString());
    //     //    m_pGF->doextract(extractFilePath.toStdString());

    //         pro.distance = true;
    //         m_pGF->extractPOIs(pro,_poi,extractFilePath);
    //         m_pGF->extractUrban(pro,urbanextract);
    //     // }
    // }
    delete m_pGF; m_pGF=NULL;

}
