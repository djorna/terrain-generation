#include "DiamondSquare.hpp"

#include <iostream>

DiamondSquare::DiamondSquare() {}

DiamondSquare::~DiamondSquare() {}

inline int DiamondSquare::randInt(int min=0, const int max=255/2) { return rand() % (max - min) + min; }

void DiamondSquare::applySquare(cv::Mat& heightmap, const int row, const int col, const int k, const float p) {
  int step = k/2;
  float result = 0;
  float i = 0;

  if (col - step > 0) {
    result += heightmap.at<uchar>(row, col - step);
    ++i;
  }
  if (col + step < heightmap.cols) {
    result += heightmap.at<uchar>(row, col + step);
    ++i;
  }
  if (row - step > 0) { 
    result += heightmap.at<uchar>(row - step, col);
    ++i;
  }
  if (row + step < heightmap.rows) {
    result += heightmap.at<uchar>(row + step, col);
    ++i;
  }
  result /= i;
  result += randInt() * p;
  heightmap.at<uchar>(row, col) = result;
}

void DiamondSquare::applyDiamond(cv::Mat& heightmap, int row, int col, int k, float p) {
  int step = k/2;
  float result = 0;
  result = heightmap.at<uchar>(row - step, col - step)
         + heightmap.at<uchar>(row + step, col - step)
         + heightmap.at<uchar>(row - step, col + step)
         + heightmap.at<uchar>(row + step, col + step);
  result /= 4;
  result += randInt() * p;
  heightmap.at<uchar>(row, col) = result;
}

void DiamondSquare::diamond(cv::Mat& heightmap, int k, float p) {
  int stride = k - 1;
  //int nApplications = pow((heightmap.rows() - 1) / (k - 1), 2);

  for (int i = k / 2; i < heightmap.rows; i += stride) {
    for (int j = k / 2; j < heightmap.cols; j += stride) {
      applyDiamond(heightmap, i, j, k, p);
    }
  }
}

void DiamondSquare::square(cv::Mat& heightmap, int k, float p) {
  int stride = k - 1;

  for (int i = k / 2; i < heightmap.rows; i += stride)
    for (int j = 0; j < heightmap.cols; j += stride)
      applySquare(heightmap, i, j, k, p);

  for (int i = 0; i < heightmap.rows; i += stride)
    for (int j = k / 2; j < heightmap.cols; j += stride)
      applySquare(heightmap, i, j, k, p);
}

void DiamondSquare::diamondSquare(cv::Mat& heightmap, const int n, const float decay) {
  float p = 1;
  for (int k = n; k > 2; k = (k + 1) / 2) {
    diamond(heightmap, k, p);
    square(heightmap, k, p);
    p *= decay;
  }
}

cv::Mat DiamondSquare::generate(const int n, const std::array<int, 4> corners, const float decay) {
  cv::Mat heightmap;
  heightmap = cv::Mat::zeros(cv::Size(n, n), CV_8UC1);

  std::cout << "Initializing corners..." << std::endl;

  // Initialize corners
  heightmap.at<uchar>(0, 0) = corners[0];
  heightmap.at<uchar>(0, n - 1) = corners[1];
  heightmap.at<uchar>(n - 1, 0) = corners[2];
  heightmap.at<uchar>(n - 1, n - 1) = corners[3];

  std::cout << "Starting diamond-square..." << std::endl;

  diamondSquare(heightmap, n, decay);
  cv::imshow("Generated terrain", heightmap);
  cv::waitKey(0);
  return heightmap;
}

cv::Mat DiamondSquare::generate(const int n, const float decay) {
  return generate(n, {randInt(), randInt(), randInt(), randInt()}, decay);
}