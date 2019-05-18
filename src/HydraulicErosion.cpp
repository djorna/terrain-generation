#include "HydraulicErosion.hpp"

#include <iostream>

namespace terrain
{

HydraulicErosion::HydraulicErosion(KernelType kt, int rows, int cols,
  float k_rain, float k_solubility, float k_evaporation, float k_capacity)
  : rows(rows), cols(cols), k_rain(k_rain), k_solubility(k_solubility), k_evaporation(k_evaporation), k_capacity(k_capacity)
{
  kernel_type = kt;
  watermap = cv::Mat::zeros(rows, cols, CV_32FC1);
  sedimentmap = cv::Mat::zeros(rows, cols, CV_32FC1);
}

HydraulicErosion::~HydraulicErosion() {}

void HydraulicErosion::apply(cv::Mat& img, const int iterations)
{
  heightmap = img;
  std::cout << "starting...\n";
  for (int pass = 0; pass < iterations; ++pass)
  {
    rain();
    erosion();
    transfer();
    evaporate();
    std::cout << "Iteration " << pass << " finished.\n";
  }
}

/**
 * Step 1: Add new water.
 */
void HydraulicErosion::rain()
{
  watermap += k_rain;
}

/**
 * Step 2: Erode
 */
void HydraulicErosion::erosion()
{
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      heightmap.at<float>(i, j) -= k_solubility * watermap.at<float>(i, j);
      sedimentmap.at<float>(i, j) += k_solubility * watermap.at<float>(i, j);
    }
  }
}

/**
 * Step 3: Transfer materials 
 */
void HydraulicErosion::transfer()
{
  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      Point center = Point(j, i);
      float w = watermap.at<float>(center);
      float h = heightmap.at<float>(center);
      float m = sedimentmap.at<float>(center);
      float a = h + w;
      std::vector<float> dlist;
      std::vector<float> alist;
      auto nbs = neighbours(center, rows, cols);
      for (auto it = nbs.cbegin(); it != nbs.cend();)
      {
        float ai = watermap.at<float>(*it) + heightmap.at<float>(*it);
        if (ai < a)
        {
          dlist.push_back(a - ai);
          alist.push_back(ai);
          ++it;
        }
        else
        {
          it = nbs.erase(it);
        }
      }

      float d_total = sum(dlist);
      float delta_a = a - average(alist);
      float min_val = std::min(w, delta_a);

      // std::cout << "minval: " << min_val << ", d_total: " << d_total << ", delta_a: " << delta_a << '\n';

      for (int i = 0; i < nbs.size(); ++i)
      {
        // std::cout << dlist[i] << '\n';
        float delta_wi = min_val * dlist[i] / d_total;
        // std::cout << "wm before" << watermap.at<float>(center);
        moveMaterial(watermap, center, nbs[i], delta_wi);
        // std::cout << "wm after" << watermap.at<float>(center);
        float delta_mi = m * delta_wi / w;
        moveMaterial(sedimentmap, center, nbs[i], delta_mi);
      }
    }
  }
}

/**
 * Step 4: Evaporation
 */
void HydraulicErosion::evaporate()
{
  watermap *= (1 - k_evaporation);

  for (int i = 0; i < rows; ++i)
  {
    for (int j = 0; j < cols; ++j)
    {
      float max_sediment = k_capacity * watermap.at<float>(i, j);
      float delta_sediment = std::max((float)0, sedimentmap.at<float>(i, j) - max_sediment);
      sedimentmap.at<float>(i, j) -= delta_sediment;
      heightmap.at<float>(i, j) += delta_sediment;
    }
  }
}

/**
 * Convenience function to transfer 
 */
void HydraulicErosion::moveMaterial(cv::Mat& img, Point move_from, Point move_to, float amount)
{
  img.at<float>(move_from) -= amount;
  img.at<float>(move_to) += amount;
}

} // namespace terrain