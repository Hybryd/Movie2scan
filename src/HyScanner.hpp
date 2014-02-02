#ifndef HYSCANNER_HPP
#define HYSCANNER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "/usr/include/opencv/cv_compatible.h"
#include "/usr/include/opencv/highgui.h"

#include "HyDataHandler.hpp"
#include "HyLayersMesher.hpp"
#include "HyNode.hpp"
#include "HyTriangle.hpp"


#define PI 3.141565358979323


/*

  HyScanner takes a video file, the calibration matrix and vector,
  and compute the 3D point cloud and a "naive" mesh.

*/

class HyScanner
{
protected:
  std::string                               videoFileName;    // video file name
  unsigned long int                       nbFrames;         // number of frames of the video
  int                                       Vcut;            // value for the manual thresholding
  cv::Mat                                   matM;             // transformation matrix
  cv::Mat                                   vecN;             // normal vector to the laser plane
  std::vector<HyNode *>                     cloud;             // the set of all measures
  std::vector< HyTriangle * >               layers;           // result of the triangulation
  double                                   angle;            // current angle for rotation
  double                                   stepAngle;       // rotation between two scans
  HyLayersMesher                            layMesher;       // layers mesher


public:
  HyScanner();
  HyScanner(std::string & videoFile);
  ~HyScanner();

  void computeNumberOfFrames();
  void readCalibrationParameters(std::string calParamFileName, std::string pNameMatrix=std::string("matM"), std::string pNameNormal=std::string("vecN"));
  cv::Mat makeRotationMatrix();
  void display();
  
  void process();
  
  void saveCloud(std::string fileName);
  void saveMesh(std::string fileName);
};

#endif

