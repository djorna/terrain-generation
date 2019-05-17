#include "ThermalErosion.hpp"

#include <iostream>
#include <queue>

namespace terrain
{

ThermalErosion::ThermalErosion() {}

//ThermalErosion::ThermalErosion(KernelType kt) : kernel_type(kt) {
ThermalErosion::ThermalErosion(KernelType kt)
{
  kernel_type = kt;
}

ThermalErosion::~ThermalErosion() {}


void ThermalErosion::moveMaterial(cv::Mat& img, Point move_from, Point move_to, float amount)
{
  img.at<float>(move_from) -= amount;
  img.at<float>(move_to) += amount;
}

void ThermalErosion::operation(cv::Mat& img, Point center, std::vector<Point> neighbours)
{
  float diff_total = 0;
  float diff_max = 0;
  float center_height = img.at<float>(center);
  std::vector<Point> lower_neighbours;
  std::queue<float> diffs;
  for (Point p : neighbours)
  {
    float diff = center_height - img.at<float>(p);
    if (diff > talus_angle)  
    {
      if (diff > diff_max) diff_max = diff;
      diff_total += diff;
      lower_neighbours.push_back(p);
      diffs.push(diff);
    }
  }

  for (Point p : lower_neighbours)
  {
    float diff = diffs.front();
    diffs.pop();
    float amount = magnitude * (diff_max - talus_angle) * diff / diff_total;
    moveMaterial(img, center, p, amount);
  }
}

void ThermalErosion::apply(cv::Mat& img, int iterations)
{
  for (int pass; pass < iterations; ++pass)
  {
    for (int i = 0; i < img.rows; ++i)
    {
      for (int j = 0; j < img.cols; ++j)
      {
        Point center = Point(i, j);
        operation(img, center, neighbours(center, img.rows, img.cols));
      }
    }
  }
}

} // namespace terrain