#ifndef __CCSETTING_H
#define __CCSETTING_H

#include <string>
#include <vector>
#include <locale>
#include <algorithm>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"

#include "tinyxml.h"
#include "CCBaseTypeDefine.h"
#include "NXLog.h"

namespace wendouzi {

static void Usage(const char* pszErrorMsg = NULL)
{
    NXLog("Usage: \n  cmdGF.exe [OPTION]... [Source FILE]\n"
    "Mandatory arguments to long options are mandatory for short options too.\n"
    "  -c, --config    [path]                          input the configure file in abosolute path, format in xml format, see the example.xml. all info could be writen in config.xml include the src file and the output dir\n"
    "  -r, --rect      [ [xmin, ymin, xmax, ymax] ]          input the crop area in the rectangle, this option save the calculation account greatly.\n"
    "  -h, --shp      [ shape path ]          input the crop area in the rectangle, this option save the calculation account greatly.\n"
    "  -p, --product   [ [ndvi, ndwi, distance, density, risk] ]     input the products you desire, the flowing products are default as output: density.\n"
    "  -s,  --srcfile  [path]                           intput the src tiff file \n"
    "  -d,  --destdir  [dir]                            input the dest directory, where productes will be saved.\n"
    "  That's all\n"
    )
    if( pszErrorMsg != NULL )
        NXLog("\nFAILURE: %s\n", pszErrorMsg);
    exit(1);
}

class __setting{
public:
    enum RangeRole{
        xmin = 0,
        ymin,
        xmax,
        ymax, 
        rangeTotalNum
    };
    enum ProductRole {
        ndvi = 0,
        ndwi,
        svi,
        distance,
        kt,
        density,
        risk,
        productTotalNum
    };

    std::vector<std::string> productNames = {
        "ndvi",
        "ndwi",
        "svi",
        "distance",
        "kt",
        "density",
        "risk"
    };
    std::vector<float> m_range;
    std::vector<bool> m_products;

    std::string srcfile;
    std::string destdir;
    std::string shpfile;
    __setting(){
        NXLog("%s\n", __PRETTY_FUNCTION__);
        // for_each(range.begin(), range.end(), [](float & r){ r = 0;});
        // for_each(products.begin(), products.end(), [](bool & p) { p = false;}); // error cause by return type bool &
        m_range.assign(int(rangeTotalNum), 0);
        m_products.assign(int(productTotalNum), false);
        srcfile = "";
        destdir = "";
        shpfile = "";
    }

    void config(int argc, char* argv[]){
        NXLog("%s, argc:%d\n", __PRETTY_FUNCTION__, argc);
        for(int idx = 1; idx < argc; ++idx) {
            NXLog("%s, idx:%d, argv:%s\n", __PRETTY_FUNCTION__, idx, argv[idx+1]);
            if (EQUAL(argv[idx], "-c") || EQUAL(argv[idx], "--config")){
                TiXmlDocument doc(argv[++idx]);
                if (!doc.LoadFile())
                {
                    Usage("could not load input config file, please check if file exist and format\n");
                }
                TiXmlElement* root = doc.RootElement();  
                if (root == NULL) {
                    Usage("could not read the input xml file\n");
                    return;
                }
                std::string temp;
                TiXmlNode * r = root->FirstChild("rect");
                if (r != NULL) {
                    temp = std::string((char *)r->ToElement()->GetText());
                    NXLog("rect:%s\n", temp.c_str());
                    float tempfloat[4] = {0.0f};
                    memset(tempfloat, 0, sizeof(float) * 4);
                    sscanf(temp.c_str(), "[%f, %f, %f, %f]", &tempfloat[0], &tempfloat[1], &tempfloat[2], &tempfloat[3]);
                    int tempidx = 0;
                    for_each(m_range.begin(), m_range.end(), [&tempfloat, &tempidx](float & r){ r = tempfloat[tempidx++];});
                }

                //! product info
                TiXmlNode * p = root->FirstChild("product");
                if (p != NULL) {
                    temp = std::string((char *)p->ToElement()->GetText());
                    for_each(temp.begin(), temp.end(), [](char & c) {c = tolower(c);});
                    std::size_t found = std::string::npos;
                    for (int proidx = 0; proidx < int(productTotalNum); ++proidx) {
                        if (std::string::npos != (found = temp.find(productNames[proidx]))){
                            m_products[proidx] = true;
                        }
                        found = std::string::npos;
                    }
                }


                //!srcfile
                TiXmlNode * s = root->FirstChild("srcfile");
                if (s != NULL) {
                    temp = std::string((char *)s->ToElement()->GetText());
                    if (boost::filesystem::exists(temp)) {
                        srcfile = temp;
                    }
                    else {
                        Usage("srcfile(input source file ) is not exists.\n");
                    }
                }

                TiXmlNode * d = root->FirstChild("destdir");
                if (d != NULL) {
                    temp = std::string((char *)d->ToElement()->GetText());
                    if (!boost::filesystem::exists(temp)) {
                        Usage("destdir(dest directory ) is not exists.\n");
                    }
                    if (!boost::filesystem::is_directory(temp)){
                        Usage("input destdir(dest directory ) is not directory.\n");
                    }
                    destdir = temp;
                }
                continue;
            }
            else if (EQUAL(argv[idx], "-r") || EQUAL(argv[idx], "--rect"))
            {

                std::string temp = std::string(argv[++idx]);
                float tempfloat[4];
                memset(tempfloat, 0, sizeof(float) * 4);
                sscanf(temp.c_str(), "[%f, %f, %f, %f]", &tempfloat[0], &tempfloat[1], &tempfloat[2], &tempfloat[3]);
                int tempidx = 0;
                for_each(m_range.begin(), m_range.end(), [&tempfloat, &tempidx](float & r){ r = tempfloat[tempidx++];});
                continue;
            }
            else if (EQUAL(argv[idx], "-p") || EQUAL(argv[idx], "--product"))
            {
                std::string str(argv[++idx]);
                for_each(str.begin(), str.end(), [](char & c) {c = tolower(c);});
                std::size_t found = std::string::npos;
                for (int proidx = 0; proidx < int(productTotalNum); ++proidx) {
                    if (std::string::npos != (found = str.find(productNames[proidx]))){
                        m_products[proidx] = true;
                    }
                    found = std::string::npos;
                }
                continue;
            }
            else if (EQUAL(argv[idx], "-s") || EQUAL(argv[idx], "--srcfile"))
            {
                if (boost::filesystem::exists(argv[++idx])) {
                    srcfile = std::string(argv[idx]);
                }
                else {
                    Usage("srcfile(input source file ) is not exists.\n");
                }
                continue;
            }   
            else if (EQUAL(argv[idx], "-d") || EQUAL(argv[idx], "--destdir"))
            {
                std::string dir = std::string(argv[++idx]);
                if (!boost::filesystem::exists(dir)) {
                    Usage("destdir(dest directory ) is not exists.\n");
                }
                if (!boost::filesystem::is_directory(dir)){
                    Usage("input destdir(dest directory ) is not directory.\n");
                }
                continue;          
            }
            else {
                // do nothing;
            }      
        }
        
    }
    void check() {
        NXLog("%s\n", __PRETTY_FUNCTION__);
        if (srcfile.compare("") == 0){
            Usage("please check input source file\n");
        }
        NXLog("%s, save dir:%s\n", __PRETTY_FUNCTION__, destdir.c_str());

    }

    void clear() {
        
        NXLog("%s\n", __PRETTY_FUNCTION__);
        m_range.assign(int(rangeTotalNum), 0);
        m_products.assign(int(productTotalNum), false);
        srcfile = "";
        destdir = "";
        shpfile = "";
    }
    virtual ~__setting(){
       NXLog("%s\n", __PRETTY_FUNCTION__);
    }
    
};

typedef __setting setting;
}

#endif
