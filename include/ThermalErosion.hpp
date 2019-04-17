#pragma once

#include <opencv2/core/core.hpp>

#include "Kernel.hpp"

class ThermalErosion : public Kernel 
{
  // using Point = cv::Point2i;

  public:
    ThermalErosion();

    ThermalErosion(KernelType kernel_type);

    ~ThermalErosion();

    void operation(cv::Mat& img, Point center, std::vector<Point> neighbours);

  private:
    float talus_angle; // T
    float magnitude = 1; // c
    KernelType kernel_type;
};