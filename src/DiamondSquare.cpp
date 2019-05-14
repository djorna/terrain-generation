#include "DiamondSquare.hpp"
#include <algorithm>
#include <iostream>

namespace terrain
{

DiamondSquare::DiamondSquare() {}

DiamondSquare::~DiamondSquare() {}

void DiamondSquare::applySquare(cv::Mat& heightmap, const int row, const int col, const int k, const float offset) {
  int step = k / 2;
  std::vector<float> feature_points;
  std::vector<std::vector<int>> pts = { {row, col - step}, {row, col + step}, {row - step, col}, {row + step, col } };
  for (auto pt : pts)
    if (pointInRange(pt[0], pt[1], heightmap.rows, heightmap.cols))
      feature_points.push_back(heightmap.at<float>(pt[0], pt[1]));
    
  // float height = average(feature_points) + randf(0, p);
  float height = average(feature_points) + offset;
  heightmap.at<float>(row, col) = height;
}

void DiamondSquare::applyDiamond(cv::Mat& heightmap, int row, int col, int k, float offset) {
  int step = k/2;
  float height = heightmap.at<float>(row - step, col - step)
               + heightmap.at<float>(row + step, col - step)
               + heightmap.at<float>(row - step, col + step)
               + heightmap.at<float>(row + step, col + step);
  height /= 4;
  height += offset;
  heightmap.at<float>(row, col) = height;
}

void DiamondSquare::diamond(cv::Mat& heightmap, int k, float p) {
  int stride = k - 1;

  std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

  for (int i = k / 2; i < heightmap.rows; i += stride)
    for (int j = k / 2; j < heightmap.cols; j += stride)
      //applyDiamond(heightmap, i, j, k, p * random.randf());
      applyDiamond(heightmap, i, j, k, p * dis(pseudo_rand_engine));
}

void DiamondSquare::square(cv::Mat& heightmap, int k, float p) {
  int stride = k - 1;

  std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
  // Random random(0.0f, 1.0f, -1);

  for (int i = k / 2; i < heightmap.rows; i += stride)
    for (int j = 0; j < heightmap.cols; j += stride)
      // applySquare(heightmap, i, j, k, p * random.randf());
      applySquare(heightmap, i, j, k, p * dis(pseudo_rand_engine));

  for (int i = 0; i < heightmap.rows; i += stride)
    for (int j = k / 2; j < heightmap.cols; j += stride)
      // applySquare(heightmap, i, j, k, p * random.randf());
      applySquare(heightmap, i, j, k, p * dis(pseudo_rand_engine));
}

void DiamondSquare::diamondSquare(cv::Mat& heightmap, const int n, const float decay)
{
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
  }
}

cv::Mat DiamondSquare::generate(const int n, const std::array<float, 4> corners, const float decay, int seed, bool normalized)
{
  cv::Mat heightmap;
  heightmap = cv::Mat::zeros(cv::Size(n, n), CV_32FC1);

  // Initialize corners
  heightmap.at<float>(0, 0) = corners[0];
  heightmap.at<float>(0, n - 1) = corners[1];
  heightmap.at<float>(n - 1, 0) = corners[2];
  heightmap.at<float>(n - 1, n - 1) = corners[3];

  seed_rand(seed);
//   Random random(seed);
  // random.seed(seed);
  // random.set_dis(0.0f, 1.0f);
  diamondSquare(heightmap, n, decay);
  if (normalized)
    cv::normalize(heightmap, heightmap, 1, 0, cv::NORM_MINMAX);
  return heightmap;
}

cv::Mat DiamondSquare::generate(const int n, const float decay, int seed, bool normalized) {
  //return generate(n, {randf(), randf(), randf(), randf()}, decay);
  //std::uniform_real_distribution<float> dis(0.0f, 1.0f);
  std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
  // seed_rand(seed);
  // Random random(seed);
  // random.seed(seed);
  // random.set_dis<float>(0.0f, 1.0f);
  auto randf = [&]() { return dis(pseudo_rand_engine); };
  return generate(n, {randf(), randf(), randf(), randf()}, decay, seed, normalized);
  // return generate(n, { random.randf(), random.randf(), random.randf(), random.randf() }, decay, seed, normalized);
}

} // namespace terrain