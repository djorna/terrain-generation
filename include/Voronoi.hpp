#pragma once

#include "common.hpp"
#include "Random.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/flann/miniflann.hpp>
#include <opencv2/flann/kdtree_index.h>
#include <opencv2/highgui.hpp>
#include <random>
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <unordered_map>

namespace terrain
{

class Voronoi
{
  using Point = cv::Point2i;
  using PointList = std::vector<Point>;

public:
  Voronoi(int rows, int cols, std::vector<float> coeffs, int n_points, int seed=-1);

  /**
   * Initialize Voronoi diagram
   */
  Voronoi(int rows, int cols, std::vector<float> coeffs, PointList points, int seed=-1);

  ~Voronoi();


  // void setPoints(PointList points);

  // void setCoeffs(std::vector<float> new_coeffs);

  void drawPoints(cv::Mat& img);

  cv::Mat generate();

  /**
  * \brief Uniformly cull a percentage
  * \param keep Fraction of regions to keep 
  * \param seed The seed of the uniform distribution
  */
  void binaryMask(float keep, int seed);

  /**
  * \brief Vary height of regions based on a normal distribution
  * \param mean The mean of the normal distribution
  * \param stdev The standard deviation of the normal distribution
  */
  void shiftHeightMask(float mean, float stdev, int seed);

private:
  void generatePoints(int n_points, int rows, int cols);

  cv::Mat heatmap;
  PointList points;
  std::vector<float> coeffs;
  std::vector<float> multipliers;
  // Random random;
  int n_coeffs;
  int n_points;
  int rows;
  int cols;
  int seed;
  int point_seed;
  int mask_seed;
  int shift_seed;
  // std::unordered_map<int, std::vector<Point>> region;
};

} // namespace terrain