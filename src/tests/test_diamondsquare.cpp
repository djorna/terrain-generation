#include "DiamondSquare.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

using namespace cv;
using namespace std;


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
  float persistence = 0.5; // Higher = more jagged terrain

  if (argc > 1){
    int t = std::stoi(argv[1]);
    if (x > 1 && x < 13) x = t;
    if (argc > 2)
      persistence = std::atof(argv[2]);
  }
  int n = pow(2, x) + 1;
  std::cout << "Creating map of size " << n << "\n";

  namedWindow("Generated terrain", cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow("Generated terrain", 1000, 1000);

  std::cout << "Init object...";
  
  DiamondSquare diamondSquare;

  cv::Mat heightmap = diamondSquare.generate(n, persistence, -1);
  cv::imshow("Generated terrain", heightmap);

  cv::waitKey(0);
  
  return 0;
}