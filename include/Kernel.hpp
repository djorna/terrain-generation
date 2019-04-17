#pragma once

#include "common.hpp"

#include <opencv2/core/core.hpp>

enum KernelType { MOORE, VON_NEUMANN, VON_NEUMANN2 };
using Point = cv::Point2i;

class Kernel
{

  public:
    void apply(cv::Mat& img);

    void applyOnce(cv::Mat& img, Point center);

    virtual void operation(cv::Mat& img, Point center, std::vector<Point> neighbours) = 0;

  private:
    std::vector<Point> neighbours(Point point, int rows, int cols);
    KernelType kernel_type;
};