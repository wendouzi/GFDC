#include "GFimg.h"

// int main(int argc, char *argv[]) {

//     GFimg *m_pGF = new GFimg(_tiff.toStdString(), 0, _xml.toStdString(),_rpb.toStdString());
//     m_pGF->getCorner();
//     m_pGF->init();
//     m_pGF->caldensity();
//     m_pGF->writeDen(sdir.toStdString());
//     delete m_pGF; m_pGF=NULL;
// }


#include <stdio.h>
#include <string>
#include "geoshape.h"
#include "GFimg.h"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
using namespace boost::filesystem;
std::vector<std::string> vector_files;
int main(int argc, char * argv[])
{
    if(argc<3)
    {
        printf("need input the directory\n");
        printf("Usage: \n  testdis <your_path> <save_path>\n <your_path> mean the directory where you save the Gao Fen image!\n <save_path> mean the directory where you want to save the result!\n");
        return 0;
    }
    if(argc>3)
    {
        printf("Do not input more than two dicectory\n");
        printf("Usage: \n  testdis <your_path> <save_path>\n <your_path> mean the directory where you save the Gao Fen image!\n <save_path> mean the directory where you want to save the result!\n");
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
    GFimg * gf = new GFimg(filename, 0, xmlfile,rpbfile);
    gf->getCorner();
    gf->init();
  // gf->print(); 
    gf->caldensity(withoutDist);
  //  gf->caldensity(withDist_1);
  //  gf->writeDen(saveimg);
  //  gf->extractValueByFile(geofile,savefile);
 //   gf->readGeoinfo(geofile);
    delete gf; gf=NULL;
}


