#include "ThermalErosion.hpp"

ThermalErosion::ThermalErosion() {}

ThermalErosion::ThermalErosion(KernelType kernel_type) : kernel_type(kernel_type) {}

ThermalErosion::~ThermalErosion() {}

void ThermalErosion::operation(cv::Mat& img, Point center, std::vector<Point> neighbours)
{
  std::vector<float> diffs;
  float center_height = img.at<float>(center);
  for (Point p : neighbours)
  {
    float diff = center_height - img.at<float>(p);
    if (diff > talus_angle)
      img.at<float>(p) = img.at<float>(p) + magnitude * (diff - talus_angle);
  }
}