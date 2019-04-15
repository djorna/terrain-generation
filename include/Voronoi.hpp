#pragma once

// #include "common.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/flann/miniflann.hpp>
#include <opencv2/flann/kdtree_index.h>
#include <opencv2/highgui.hpp>
#include <random>
#include <iostream>
#include <opencv2/imgproc.hpp>

class Voronoi
{
  using PointList = std::vector<cv::Point2d>;

public:
  Voronoi(int rows, int cols, std::vector<float> coeffs, int n_points);

  /**
   * Initialize Voronoi diagram
   */
  Voronoi(int rows, int cols, std::vector<float> coeffs, PointList points);

  ~Voronoi();

  void generatePoints();

  void Voronoi::generatePoints(int n_points, int rows, int cols);

  // void setPoints(PointList points);

  // void setCoeffs(std::vector<float> new_coeffs);

  void drawPoints(cv::Mat& img);

  cv::Mat generate();

  cv::Mat heatmap;
private:
  PointList points;
  std::vector<float> coeffs;
  int n_coeffs;
  int rows;
  int cols;
};