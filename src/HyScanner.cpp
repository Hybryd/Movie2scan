#include "HyScanner.hpp"


HyScanner::HyScanner()
{
  videoFileName             = "video.webm";
  computeNumberOfFrames();
  Vcut                      = 200;
  angle                     = 0;
  stepAngle                 = 360.0/nbFrames;
  matM.create(3,4,cv::DataType<double>::type);
  vecN.create(4,1,cv::DataType<double>::type);
}


HyScanner::HyScanner(std::string & videoFile): videoFileName(videoFile)
{
  computeNumberOfFrames();
  Vcut                      = 200;
  angle                     = 0;
  stepAngle                 = 360.0/nbFrames;
  matM.create(3,4,cv::DataType<double>::type);
  vecN.create(4,1,cv::DataType<double>::type);
}

HyScanner::~HyScanner()
{
  for(unsigned long int i=0;i<cloud.size();++i)
    if(cloud[i] != NULL)
      delete cloud[i];
}

/*
  Count the number of frames (CV_CAP_PROP_FRAME_COUNT is not recognized)
*/
void HyScanner::computeNumberOfFrames()
{
  cv::VideoCapture cap0(videoFileName);
  if ( !cap0.isOpened() )  // if not success, exit program
    std::cerr << "ERROR [HyScanner::computeNumberOfFrames]: Cannot open the video file " << videoFileName << std::endl;
  
  unsigned long int cpt=0;
  cv::Mat frame;
//  std::cerr << "Determining number of frames..." << std::endl;
  while(cap0.read(frame))
    ++cpt;
//  std::cout << "Number of frames : " << N << std::endl;
  nbFrames = cpt;
  cap0.release();
}



/*
  Read calibration parameters for the scan in an XML file: the transformation matrix and the normal vector
*/

void HyScanner::readCalibrationParameters(std::string calParamFileName, std::string pNameMatrix, std::string pNameNormal)
{
  cv::FileStorage fs(calParamFileName, cv::FileStorage::READ); // parameters file
  if(fs.isOpened())
  {
    fs[pNameMatrix] >> matM;
    fs[pNameNormal] >> vecN;
  }
  else
    std::cerr << "ERROR [HyScanner::readCalibrationParameters]: can't open file "<< calParamFileName << std::endl;
}



/*
  Construct the rotation matrix around Z axis
*/

cv::Mat HyScanner::makeRotationMatrix()
{
  cv::Mat rot(3,3,cv::DataType<double>::type);
  // rotation around Z axis
  rot.at<double>(0,0)=cos(angle) ; rot.at<double>(0,1)=-sin(angle) ; rot.at<double>(0,2)=0 ; 
  rot.at<double>(1,0)=sin(angle) ; rot.at<double>(1,1)=cos(angle)  ; rot.at<double>(1,2)=0 ;
  rot.at<double>(2,0)=0          ; rot.at<double>(2,1)=0           ; rot.at<double>(2,2)=1 ;
  return rot;
}


/*
  Display the parameters
*/
void HyScanner::display()
{
  std::cout << "Video file            : " <<  videoFileName << std::endl;
  std::cout << "Number of frames      : " <<  nbFrames << std::endl;
  std::cout << "Brightness threshold  : " <<  Vcut << std::endl;
  std::cout << "Transformation matrix : " << std::endl <<  matM << std::endl;
  std::cout << "Normal vector         : " <<  vecN << std::endl;
  std::cout << "Angular step          : " <<  stepAngle << "°" << std::endl << std::endl;
}

/*
  Read the movie frame by frame, detect the laser ray, compute the corresponding 3D points and triangulate on the fly
*/

void HyScanner::process()
{
  cv::VideoCapture cap(videoFileName);

  if ( !cap.isOpened() )  // if not success, exit program
    std::cerr << "ERROR [HyScanner::process]: Cannot open the video file" << videoFileName << std::endl;

  cv::Mat HSV_image;
  cv::Mat HSV_gray_image;
  cv::Mat finalVec(3,1,cv::DataType<double>::type);
  cv::Mat finalMat(3,3,cv::DataType<double>::type);
  cv::Mat rot=makeRotationMatrix();  
  cv::Mat image;
  
  unsigned long int  cpt=0;
  
  while(cap.read(image))
  {
    std::stringstream s;
    cv::cvtColor(image, HSV_image, CV_BGR2HSV);
    
    // Threshold HSV_image with V_cut
    for(int i=0; i<HSV_image.rows; i++)
    {
      for(int j=0; j<HSV_image.cols; j++)
      {
        if(HSV_image.at<cv::Vec3b>(i,j)[2] < Vcut)
        {
          HSV_image.at<cv::Vec3b>(i,j)[0] = 0;
          HSV_image.at<cv::Vec3b>(i,j)[1] = 0;
          HSV_image.at<cv::Vec3b>(i,j)[2] = 0;
        }
        else
        {
          HSV_image.at<cv::Vec3b>(i,j) = HSV_image.at<cv::Vec3b>(i,j);
        }
      }
    }
    cv::blur( HSV_image, HSV_image, cv::Size(3,3) );
    cvtColor( HSV_image, HSV_gray_image, CV_BGR2GRAY );

    // Find the middle of the laser line and compute the corresponding 3D point
    std::vector<int> row;
    std::vector< HyNode * > layNode;
    
    for(int v=0; v<HSV_gray_image.rows; v++)
    {
      // If there are serveral highlighted points in a row, put them together
      for(int u=0; u<HSV_gray_image.cols; u++)
      {
        if(HSV_gray_image.at<unsigned char>(v,u) != 0)
          row.push_back(u);
      }
      if(row.size()!=0)
      {
        // Horizontal middle of the laser ray
        int u=row[row.size()/2]; 
        
        finalMat.at<double>(0,0) = matM.at<double>(0,0) - matM.at<double>(2,0)*u;
        finalMat.at<double>(0,1) = matM.at<double>(0,1) - matM.at<double>(2,1)*u;
        finalMat.at<double>(0,2) = matM.at<double>(0,2) - matM.at<double>(2,2)*u;

        finalMat.at<double>(1,0) = matM.at<double>(1,0) - matM.at<double>(2,0)*v;
        finalMat.at<double>(1,1) = matM.at<double>(1,1) - matM.at<double>(2,1)*v;
        finalMat.at<double>(1,2) = matM.at<double>(1,2) - matM.at<double>(2,2)*v;
        
        finalMat.at<double>(2,0) = vecN.at<double>(0,0);
        finalMat.at<double>(2,1) = vecN.at<double>(0,1);
        finalMat.at<double>(2,2) = vecN.at<double>(0,2);
        
        finalVec.at<double>(0,0) = matM.at<double>(2,3)*u - matM.at<double>(0,3);
        finalVec.at<double>(0,1) = matM.at<double>(2,3)*v - matM.at<double>(1,3);
        finalVec.at<double>(0,2) = -vecN.at<double>(0,3);

        finalMat=finalMat.inv();
        
        cv::Mat p = rot*finalMat*finalVec;
        HyNode * n = new HyNode(p.at<double>(0,0), p.at<double>(1,0), p.at<double>(2,0));
        
        // Store the node twice
        layNode.push_back(n);
        cloud.push_back(n);
      }
      row.clear();
    }
    if(cpt==0)
      layMesher.addFirstLayer(layNode);
    else
      layMesher.addLayer(layNode);

    rot=makeRotationMatrix();
    angle += stepAngle*PI/180.;
    ++cpt;
    std::cerr << "Progress: " << angle*180./PI << "° (" << (double)(cpt)/nbFrames*100. << " %)                      \r";
  }
  std::cerr << "                                                                            \r";
}


void HyScanner::saveCloud(std::string fileName)
{
  HyDataHandler dh;
  std::cerr << "Saving data in " << fileName+".xyz ..." << std::endl;
  dh.saveAsXYZ(fileName+".xyz",cloud);
}



void HyScanner::saveMesh(std::string fileName)
{
  HyDataHandler dh;
  std::cerr << "Saving mesh in " << fileName+".mesh ..." << std::endl;
  dh.saveMesh(fileName+".mesh",layMesher.m());
}










