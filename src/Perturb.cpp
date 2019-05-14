#include "Perturb.hpp"

#include <iostream>

namespace terrain
{

/**
 * \brief Apply the perturbation operation to the matrix. From "Texturing and Modeling: A Procedural Approach (Third Edition)"
 */
cv::Mat Perturb::apply(const cv::Mat& img, float mag, Offset offset1, Offset offset2)
{
  cv::Mat new_img(img.rows, img.cols, CV_32FC1);
  for (int i = 0; i < img.rows; ++i)
  {
    for (int j = 0; j < img.cols; ++j)
    {
      float val = img.at<float>(i, j);
      float s = static_cast<float>(i) / (float)img.rows;
      float t = static_cast<float>(j) / (float)img.cols;

      float ss = s + mag * SimplexNoise::noise(s + offset1.x, t + offset1.y, val + offset1.z);
      float tt = t + mag * SimplexNoise::noise(s + offset2.x, t + offset2.y, val+ offset2.z);

      float y2 = std::min(static_cast<float>(img.rows), std::max(0.0f, ss * static_cast<float>img.rows));
      float x2 = std::min(static_cast<float>(img.cols), std::max(0.0f, tt * static_cast<float>img.cols));

      new_img.at<float>(i, j) = getValue(img, x2, y2);
    }
  }
  return new_img;
}

/**
 * \brief Saturate a value between min_range and max_range.
 */
float Perturb::clamp(const float val, const float min_range, const float max_range)
{
  if (val < min_range)      return min_range;
  else if (val > max_range) return max_range;
  else                      return val;
}

/**
 * \brief Find Mat value at point (x,y) via bilinear interpolation.
 * https://en.wikipedia.org/wiki/Bilinear_interpolation
 */
float Perturb::interp(const cv::Mat& img, const float point_x, const float point_y)
{
  float x = clamp(point_x, 0, static_cast<float>(img.cols - 2));
  float y = clamp(point_y, 0, static_cast<float>(img.rows - 2));
  float xi[2] = { floor(x), floor(x + 1) };
  float yi[2] = { floor(y), floor(y + 1) };
  auto f = [&](int cx, int cy) {
    return img.at<float>(static_cast<int>(yi[cy]), static_cast<int>(xi[cx]));
  };
  float fxy0 = (xi[1] - x) * f(0, 0) + (x - xi[0]) * f(1, 0);
  float fxy1 = (xi[1] - x) * f(0, 1) + (x - xi[0]) * f(1, 1);
  float value = (yi[1] - y) * fxy1 + (y - yi[0]) * fxy1;
  return value;
}

/**
 * \brief Get the value of the matrix at (x,y), clamping the points if they are outside the image.
 */
float Perturb::getValue(const cv::Mat& img, const float point_x, const float point_y)
{
  float x = clamp(point_x, 0, static_cast<float>(img.cols - 2));
  float y = clamp(point_y, 0, static_cast<float>(img.rows - 2));
  return img.at<float>(static_cast<int>(y), static_cast<int>(x));
}

} // namespace terrain