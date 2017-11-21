#include <iostream>
#include <string>
//./gdalwarp -te 112.60 29.221 113.10 29.857 
// /home/yangxianping/data/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473.tiff 
// /home/yangxianping/data/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473/test4.tiff
int main() 
{
    std::string cmd;
    cmd = "gdalwarp";
    cmd += " ";
    cmd += "-te 112.60 29.221 113.10 29.857";
    cmd += " ";
    cmd += "/home/yangxianping/data/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473.tiff";
    cmd += " ";
    cmd += "/home/yangxianping/data/GF1_WFV1_E111.8_N29.7_20150521_L1A0000818473/test10.tiff";
    system(cmd.c_str());
    fflush(stdout);

    cmd.clear();
    cmd = "./cmdDC";
    cmd += " ";
    cmd += "-c ~/workspace/DiseaseControl_v4/module/src/config/example.xml ";
    system(cmd.c_str());
    fflush(stdout);
}
