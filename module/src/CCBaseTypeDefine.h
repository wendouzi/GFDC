#ifndef CCBASETYPEDEFINE_H
#define CCBASETYPEDEFINE_H
#include <iostream>
typedef unsigned int AppId;
const AppId InvalidAppId = 0xffffffff;

#define FILE_EMPTY       std::string("")
#define TIFF_FILEDIALOG    int(1)
#define XML_FILEDIALOG     int(2)
#define RPB_FILEDIALOG     int(3)
#define SDIR_FILEDIALOG    int(4)
#define POI_FILEDIALOG     int(5)

#define TOTAL_PRODUCTS     int(7)
#define NDVI_ORDER         int(0)
#define NDWI_ORDER         int(1)
#define SVI_ORDER          int(2)
#define DISTANCE_ORDER     int(3)
#define KT_ORDER           int(4)
#define DENSITY_ORDER      int(5)
#define RISK_ORDER     int(6)


#define COLOR_MAX_INT      255
#define PREVIEW_WIDTH     int(360)
#define PREVIEW_HEIGHT    int(402)
//#define PREVIEW_WIDTH     int(440)
//#define PREVIEW_HEIGHT    int(360)
#define PREVIEW_TESE_CASE  "test"
#ifdef WIN32
#define DIR_BEGIN   8
#else
#define DIR_BEGIN   7
#endif
/*********************************************************************
*                               登录间隔 *单位:ms
***********************************************************************/
#define INTERVAL_LOGIN     int(10000)

/*********************************************************************
*                             最大登录次数
**********************************************************************/
#define NUM_LOGIN               5

/***************************记住密码标志位的有效值 ************/
#define VALID_FLAG  1


#define assert(exp) { if (!(exp)) { std::cout << "Assertion failed: " << #exp << "\n" << "line: " << __LINE__ << "\n" << "file: " << __FILE__ << "\n";exit(EXIT_FAILURE); } }
#endif // CCBASETYPEDEFINE_H

#ifdef WIN32
#define FILE_SEPERATOR "\\"
#else
#define FILE_SEPERATOR "/"
#endif

#ifdef WIN32
#define __PRETTY_FUNCTION__ ""
#endif

