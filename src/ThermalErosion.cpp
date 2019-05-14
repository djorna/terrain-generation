#include "ThermalErosion.hpp"

#include <iostream>

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
  std::vector<float> diffs;
  float center_height = img.at<float>(center);
  float diff_total = 0;
  float diff_max = 0;
  for (Point p : neighbours)
  {
    float diff = center_height - img.at<float>(p);
    if (diff > talus_angle)
    {
      diffs.push_back(diff);
      diff_max = std::max(diff_max, diff);
      diff_total += diff;
      float amount = magnitude * (diff_max - talus_angle) * diff / diff_total;
      moveMaterial(img, center, p, amount);
    }
    else
    {
      diffs.push_back(-1);
    }
    center_height = img.at<float>(center);
  }
  // diff_max = std::max_element(diffs.begin(), diffs.end());

// std::cout << "diff max : " << diff_max << '\n';

/*
for(int i = 0; i < neighbours.size(); ++i)
{
  Point p = neighbours[i];
  if (diffs[i] > talus_angle) {
    float amount = magnitude * (diff_max - talus_angle) * diffs[i] / diff_total;
    // std::cout << "diffs[i]: " << diffs[i];
    //std::cout << "Moving material " << amount << '\n';
    moveMaterial(img, center, p, amount);
  }
}
*/
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