#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "/usr/include/opencv/highgui.h"
#include "/usr/include/opencv/cv_compatible.h"

#include "../src/HyScanner.hpp"






int main(int argc, char** argv)
{
  std::string movieFileName="";
  std::string calParamFileName="";
  std::string outputFileName="";

  if(argc>3)
  {
    movieFileName = std::string(argv[1]);
    calParamFileName = std::string(argv[2]);
    outputFileName = std::string(argv[3]);
  }
  else
  {
    std::cerr << "Usage: ./movie2scan movieFileName.webm calParamFileName.xml outputFileName" << std::endl;
    return -1;
  }

  std::cout << std::endl;
  HyScanner scan(movieFileName);
  scan.readCalibrationParameters(calParamFileName);
  scan.display();
  scan.process();
  scan.saveCloud(outputFileName);
  scan.saveMesh(outputFileName);
  std::cout << std::endl;

  return 0;
}





