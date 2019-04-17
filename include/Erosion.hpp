#pragma once

#include <opencv2/core/core.hpp>

#include "Kernel.hpp"

class Erosion
{
  // using Point = cv::Point2i;

  public:
    Erosion();

    Erosion(KernelType kernel_type);

    virtual ~Erosion();

    virtual void apply(cv::Mat& img);

    virtual void operation(cv::Mat& img, Point center, std::vector<Point> neighbours);

  private:
    Kernel kernel;
};