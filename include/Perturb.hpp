#pragma once

#include "SimplexNoise.hpp"

#include <opencv2/opencv.hpp>
#include <cmath>

namespace terrain
{

struct Offset {
  float x, y, z;
  Offset(float x, float y, float z) : x(x), y(y), z(z) { }
};

class Perturb
{
public:
  static cv::Mat apply(const cv::Mat& img, float mag=0.25, Offset offset1={0, 0, 0}, Offset offset2={ 1.5, 6.7, 3.4});

private:
  static float clamp(const float val, const float min_range, const float max_range);

  static float interp(const cv::Mat& img, float x, float y);

  static float getValue(const cv::Mat& img, float x, float y);
};

} // namespace terrain