#pragma once

#include "common.hpp"

#include <opencv2/core/core.hpp>

enum KernelType { MOORE, VON_NEUMANN, VON_NEUMANN2 };
using Point = cv::Point2i;

class Kernel
{
  public:
    std::vector<Point> neighbours(Point point, int rows, int cols);

    virtual void apply(cv::Mat& img, int iterations) = 0;

  protected:
    KernelType kernel_type;
};