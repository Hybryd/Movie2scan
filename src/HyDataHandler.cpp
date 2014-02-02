#include "HyDataHandler.hpp"

HyDataHandler::HyDataHandler()
{
}


//void HyDataHandler::readXYZ(std::string fileName, PointList & cloud)
//{
////  if (fileName.find(".xyz") == fileName.size()-4)
//  {
//    std::ifstream input(fileName.c_str());
//    if(!input.is_open())
//    {
//      std::cerr << "ERROR [ HyDataHandler::readXYZ ]: unable to open "<< fileName << std::endl;
//      exit(-1);
//    }
//    std::string line;
//    double x,y,z;
//    while(getline(input,line))
//    {
//      input >> x >> y >> z;
//      cloud.push_back(Point(x,y,z));
//    }
//    input.close();
//  }
////  else
////  {
////    std::cerr << "ERROR: "<< fileName << " must end with .pcd, .ply or .xyz" << std::endl;
////  }
//}

void HyDataHandler::saveVector3(std::string fileName, std::string varName, std::vector < double > & vec)
{
  std::ofstream out(fileName.c_str(),std::ios::out | std::ios::app);
  if(out)
  {
    out << "#" << varName << " ";
    for(int i=0;i<vec.size();++i)
      out << vec[i] << " ";
  }
  else
    std::cerr << "ERROR [ HyDataHandler::saveMatrix3x3 ]: unable to write in " << fileName << std::endl;
}


void HyDataHandler::saveMatrix3x3(std::string fileName, std::string varName, std::vector< std::vector < double > > & mat)
{
  std::ofstream out(fileName.c_str(), std::ios::out | std::ios::app);
  if(out)
  {
    out << "#" << varName << " ";
    for(int i=0;i<mat.size();++i)
      for(int j=0;j<mat[i].size();++j)
        out << mat[i][j] << " ";
  }
  else
    std::cerr << "ERROR [ HyDataHandler::saveMatrix3x3 ]: unable to write in " << fileName << std::endl;
}


std::vector < double > HyDataHandler::readVector3(std::string fileName, std::string varName)
{
  std::string line;
  std::string keyword;
  std::stringstream sline;
  std::fstream file(fileName.c_str(),std::ios::in);
  std::vector < double > res;
  if(file)
  {
    while(getline(file,line))
    {
      if(line != "") // avoids empty lines
      {
        sline.clear();
        sline.str("");
        sline << line;
        sline >> keyword;
        // Parser
        if(keyword[0] == '#')
        {
          if      (keyword == varName)
          {
            double x;
            for(int i=0;i<3;++i)
            {
              sline >> x;
              res.push_back(x);  
            }
          }
        }
      }
    }
  }
  else
    std::cerr << "ERROR [ HyDataHandler::readVector3 ]: unable to read " << fileName << std::endl;
  return res;
}



std::vector< std::vector < double > > HyDataHandler::readMatrix3x3(std::string fileName, std::string varName)
{
  std::string line;
  std::string keyword;
  std::stringstream sline;
  std::fstream file(fileName.c_str(),std::ios::in);
  std::vector< std::vector < double > > res;
  if(file)
  {
    while(getline(file,line))
    {
      if(line != "") // avoids empty lines
      {
        sline.clear();
        sline.str("");
        sline << line;
        sline >> keyword;
        // Parser
        if(keyword[0] == '#')
        {
          if      (keyword == varName)
          {
            double x;
            for(int i=0;i<3;++i)
            {
              std::vector < double > raw;
              for(int j=0;j<3;++j)
              {
                sline >> x;
                raw.push_back(x);  
              }
              res.push_back(raw);
            }
          }
        }
      }
    }
  }
  else
    std::cerr << "ERROR [ HyDataHandler::readMatrix3x3 ]: unable to read " << fileName << std::endl;
  return res;
}

void HyDataHandler::saveMatrixAndVector(std::string fileName, std::string nameMat, std::vector< std::vector < double > > & mat, std::string nameVec, std::vector < double > & vec)
{
  cv::Mat M;
  M.create(3,4,cv::DataType<double>::type);
  cv::Mat N;
  N.create(4,1,cv::DataType<double>::type);
  
  std::cerr << "IN saveMatrixAndVector :" << mat.size() << " " << vec.size() << std::endl;
  
  for(int i=0;i<mat.size();++i)
  {
    N.at<double>(i,0) = vec[i];
    for(int j=0;j<mat[i].size();++j)
      M.at<double>(i,j) = mat[i][j] ;
  }
  
  
  cv::FileStorage fs(fileName, cv::FileStorage::WRITE);
  if(fs.isOpened())
  {
    fs << nameMat << M;
    fs << nameVec << N;
  }
  else
    std::cerr << "ERROR [ HyDataHandler::saveMatrixAndVector ]: unable to write in " << fileName << std::endl;

}

void HyDataHandler::saveAsXYZ(std::string fileName, std::vector< std::vector<double> > & vecMat)
{
  std::ofstream out(fileName.c_str());
  if(out)
  {
    for(unsigned long int i=0; i< vecMat.size(); ++i)
    {
      for(int j=0; j< vecMat[i].size(); ++j)
      {
        out << vecMat[i][j] << " ";
      }
      out << std::endl;
    }
    out.close();
  }
  else
    std::cerr << "ERROR [ HyDataHandler::saveAsXYZ ]: unable to write in " << fileName << std::endl;
}

void HyDataHandler::saveAsXYZ(std::string fileName, std::vector< HyNode * > & vec)
{
  std::ofstream out(fileName.c_str());
  if(out)
  {
    for(unsigned long int i=0; i< vec.size(); ++i)
      out << *vec[i] << std::endl;
    out.close();
  }
  else
    std::cerr << "ERROR [ HyDataHandler::saveAsXYZ ]: unable to write in " << fileName << std::endl;


}


void HyDataHandler::saveAsXYZLayers(std::string fileName, std::vector< std::vector< cv::Mat > > & layers)
{
  std::ofstream out(fileName.c_str());
  if(out)
  {
    for(unsigned long int i=0; i< layers.size(); ++i)
    {
      for(unsigned long int j=0; j< layers[i].size(); ++j)
      {
        for(int k=0; k< layers[i][j].rows; ++k)
        {
          out << layers[i][j].at<double>(k,0) << " ";
        }
        out << std::endl;
      }
      out << "#" << std::endl;
    }
    out.close();
  }
  else
    std::cerr << "ERROR [ HyDataHandler::saveAsXYZLayers ]: unable to write in " << fileName << std::endl;
}




void HyDataHandler::saveMesh(std::string fileName, std::vector< HyTriangle > & m)
{
  std::ofstream out(fileName.c_str());
  if(out)
  {
    for(unsigned long int i=0; i< m.size(); ++i)
    {
      out << m[i] << std::endl << std::endl;
    }
    out.close();
  }
  else
    std::cerr << "ERROR [ HyDataHandler::saveMesh ]: unable to write in " << fileName << std::endl;
}


