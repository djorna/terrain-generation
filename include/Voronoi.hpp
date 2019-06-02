#pragma once

#include "rng.hpp"

#include <opencv2/core/core.hpp>
#include <random>

namespace terrain
{

class Voronoi
{
  using Point = cv::Point2i;
  using PointList = std::vector<Point>;

public:
  Voronoi(int rows, int cols, std::vector<float> coeffs, int n_points, int seed=-1, bool regularize=true);

  /**
   * Initialize Voronoi diagram
   */
  Voronoi(int rows, int cols, std::vector<float> coeffs, PointList points, int seed=-1, bool regularize=true);

  ~Voronoi();


  // void setPoints(PointList points);

  // void setCoeffs(std::vector<float> new_coeffs);

  void drawPoints(cv::Mat& img) const;

  cv::Mat generate(bool normalize=true);

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

  /**
   * \brief Widens gaps between peaks
   * \param img The original Voronoi diagram
   * \param magnitude The height to subtract from the diagram
   */
  void widenGaps(cv::Mat& img, float magnitude);

  PointList getPoints() const;

  void setPoints(int*x, int* y);

  void setWeights(float* weights);

  void getPoints(int* x, int* y) const;

private:
  void generatePoints(int n_points, int rows, int cols, int seed=-1, bool regularize=true);

  cv::Mat heatmap;
  PointList points;
  std::vector<float> points_norm; // Points normalized between 0 and 1
  std::vector<float> coeffs;
  std::vector<float> multipliers;
  // Random random;
  int rows;
  int cols;
  int seed;
  int point_seed;
  int mask_seed;
  int shift_seed;
};

} // namespace terrain