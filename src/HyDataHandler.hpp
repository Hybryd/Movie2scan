#ifndef DATAHANDLER_HPP
#define DATAHANDLER_HPP

/*!
*
* \file   HyDataHandler.hpp
* \brief  Header file of HyDataHandler class.
*
*/


#include "/usr/include/opencv/cv_compatible.h"
#include <fstream>
#include <iostream>
#include <string>

#include "HyTriangle.hpp"



/*!
*
* \class HyDataHandler
* \brief Contains a set of functions converting data types.
*
*/

class HyDataHandler
{
protected:

public:
  HyDataHandler();
  
//  void readXYZ(std::string fileName, PointList & cloud);
  
  std::vector < double >                  readVector3(std::string fileName, std::string varName);
  void                                    saveVector3(std::string fileName, std::string varName, std::vector < double > & vec);
  
  std::vector< std::vector < double > >   readMatrix3x3(std::string fileName, std::string varName);
  void                                    saveMatrix3x3(  std::string fileName, std::string varName,
                                                          std::vector< std::vector < double > > & mat);
  
  
  void                                    saveMatrixAndVector(  std::string fileName,
                                                                std::string nameMat, 
                                                                std::vector< std::vector < double > > & mat, 
                                                                std::string nameVec,
                                                                std::vector < double > & vec);
  
  void                                    saveAsXYZ(std::string fileName, std::vector< std::vector<double> > & vecMat);
  void                                    saveAsXYZ(std::string fileName, std::vector< HyNode * > & vec);
  void                                    saveAsXYZLayers(std::string fileName, std::vector< std::vector< cv::Mat > > & layers);
  
  void                                    saveMesh(std::string fileName, std::vector< HyTriangle > & m);

};

#endif
