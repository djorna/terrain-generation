#include "FastErosion.hpp"

namespace terrain
{

//FastErosion::FastErosion(KernelType kt) : kernel_type(kt) {
FastErosion::FastErosion(KernelType kt)
{
  kernel_type = kt;
}

void FastErosion::moveMaterial(cv::Mat& img, Point move_from, Point move_to, float amount)
{
  img.at<float>(move_from) -= amount;
  img.at<float>(move_to) += amount;
}

void FastErosion::operation(cv::Mat& img, Point center, std::vector<Point> neighbours)
{
  float diff_max = 0;
  float center_height = img.at<float>(center);
  Point lowest_point;
  for (Point p : neighbours)
  {
    float diff = center_height - img.at<float>(p);
    if (diff > diff_max)
    {
      diff_max = diff;
      lowest_point = p;
    }
  }

  if (0 < diff_max && diff_max >= talus_angle)
    moveMaterial(img, center, lowest_point, diff_max / 2);
}

void FastErosion::apply(cv::Mat& img, int iterations)
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