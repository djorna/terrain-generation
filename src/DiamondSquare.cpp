#include "DiamondSquare.hpp"

#include <iostream>

DiamondSquare::DiamondSquare() {}

DiamondSquare::~DiamondSquare() {}

void DiamondSquare::applySquare(cv::Mat& heightmap, const int row, const int col, const int k, const float p) {
  int step = k / 2;
  std::vector<float> feature_points;
  std::vector<std::vector<int>> pts = { {row, col - step}, {row, col + step}, {row - step, col}, {row + step, col } };
  for (auto pt : pts)
    if (pointInRange(pt[0], pt[1], heightmap.rows, heightmap.cols))
      feature_points.push_back(heightmap.at<float>(pt[0], pt[1]));
    
  float result = average(feature_points);
  result += randf() * p; // Add random part
  // std::cout << "Result: " << result << '\n';
  heightmap.at<float>(row, col) = result;
}

void DiamondSquare::applyDiamond(cv::Mat& heightmap, int row, int col, int k, float p) {
  int step = k/2;
  float result = heightmap.at<float>(row - step, col - step)
         + heightmap.at<float>(row + step, col - step)
         + heightmap.at<float>(row - step, col + step)
         + heightmap.at<float>(row + step, col + step);
  result /= 4;
  result += randf() * p;
 // std::cout << "Result: " << result << '\n';
  heightmap.at<float>(row, col) = result;
}

void DiamondSquare::diamond(cv::Mat& heightmap, int k, float p) {
  int stride = k - 1;

  for (int i = k / 2; i < heightmap.rows; i += stride)
    for (int j = k / 2; j < heightmap.cols; j += stride)
      applyDiamond(heightmap, i, j, k, p);
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
  //namedWindow("debug", cv::WINDOW_NORMAL );// Create a window for display.
  //cv::resizeWindow("debug", 1000, 1000);
  for (int k = n; k > 2; k = (k + 1) / 2) {
    diamond(heightmap, k, p);
    //cv::imshow("debug", heightmap);
    //cv::waitKey(0);
    square(heightmap, k, p);
    //cv::imshow("debug", heightmap);
    //cv::waitKey(0);
    p *= decay;
    std::cout << k << ",";
  }
}

cv::Mat DiamondSquare::generate(const int n, const std::array<float, 4> corners, const float decay, bool normalized) {
  cv::Mat heightmap;
  heightmap = cv::Mat::zeros(cv::Size(n, n), CV_32FC1);

  // Initialize corners
  heightmap.at<float>(0, 0) = corners[0];
  heightmap.at<float>(0, n - 1) = corners[1];
  heightmap.at<float>(n - 1, 0) = corners[2];
  heightmap.at<float>(n - 1, n - 1) = corners[3];

  diamondSquare(heightmap, n, decay);
  if (normalized)
    cv::normalize(heightmap, heightmap, 1, 0, cv::NORM_MINMAX);
  return heightmap;
}

cv::Mat DiamondSquare::generate(const int n, const float decay, bool normalized) {
  return generate(n, {randf(), randf(), randf(), randf()}, decay);
}