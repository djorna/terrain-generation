#include "DiamondSquare.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;


void extractData(cv::Mat& mat, std::vector<uchar>& array)
{
  if (mat.isContinuous())
  {
    array.assign(mat.datastart, mat.dataend);
  }
  else
  {
    for (int i = 0; i < mat.rows; ++i)
      array.insert(array.end(), mat.ptr<uchar>(i), mat.ptr<uchar>(i) + mat.cols);
  }
}

void extractData(const cv::Mat& mat, float* data)
{
  std::vector<float> array;
  if (mat.isContinuous())
  {
    array.assign((float*)mat.datastart, (float*)mat.dataend);
  }
  else
  {
    for (int i = 0; i < mat.rows; ++i)
      array.insert(array.end(), mat.ptr<float>(i), mat.ptr<float>(i) + mat.cols);
  }
  std::memcpy(data, array.data(), array.size() * sizeof(float));
}

void extractData2(const cv::Mat& mat, uchar* data)
{
  std::vector<uchar> array;
  if (mat.isContinuous())
  {
    array.assign(mat.datastart, mat.dataend);
  }
  else
  {
    for (int i = 0; i < mat.rows; ++i)
      array.insert(array.end(), mat.ptr<uchar>(i), mat.ptr<uchar>(i) + mat.cols);
  }
  std::memcpy(data, array.data(), array.size() * sizeof(uchar));
}

int main(int argc, char** argv) {
  
  int x = 2; // Defauts to 2 (size of 5)
  float persistence = 0.2; // Higher = more jagged terrain

  if (argc > 1){
    int t = std::stoi(argv[1]);
    if (x > 1 && x < 13) x = t;
    if (argc > 2)
      persistence = std::atof(argv[2]);
  }
  int n = pow(2, x) + 1;
  srand(time(NULL));
  std::cout << "Creating map of size " << n << "\n";

  namedWindow("Generated terrain", cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow("Generated terrain", 1000, 1000);

  std::cout << "Init object...";
  
  DiamondSquare diamondSquare;

  auto heightmap = diamondSquare.generate(n, persistence);
  cv::imshow("Generated terrain", heightmap);

  // uchar* mat_data = heightmap.data;

  cv::Mat heightmap2;
  cv::Mat hm = heightmap * 255;
  hm.convertTo(heightmap2, CV_8UC1);
  namedWindow("Generated terrain 2", cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow("Generated terrain 2", 1000, 1000);
  cv::imshow("Generated terrain 2", heightmap2);

  std::cout << heightmap << std::endl;

  uchar* data;
  extractData2(heightmap2, data);
  float* data2;
  extractData(heightmap, data2);
  std::cout << "---------------- floats \n";
  std::cout << "data2:" << data2[0] << ", " << data2[1] << std::endl;
  std::cout << "heightmap:" <<  (float)heightmap.at<float>(0, 0) << (float)heightmap.at<float>(0, 1) << std::endl;
  /*
  std::cout << data[0] << data[1] << std::endl;
  std::cout << heightmap2.at<uchar>(0, 0) << heightmap2.at<uchar>(0, 1);
  */
  
  cv::waitKey(0);
  
  /*
  std::ostringstream os;
  os << "./examples/" << n << "x" << n << ".png";
  imwrite(os.str(), heightmap);

  std::cout << "Saved image as " << os.str() << "\n";
  */
  
  return 0;
}