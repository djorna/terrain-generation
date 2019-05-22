#pragma once

#include <opencv2/core/core.hpp>

#include "Kernel.hpp"

namespace terrain
{

class HydraulicErosion : public Kernel
{
public:
   HydraulicErosion(KernelType kernel_type, int rows, int col,
    float k_rain=0.01, float k_solubility=0.01, float k_evaporation=0.5, float k_capacity=0.01);

  ~HydraulicErosion();

  void apply(cv::Mat& img, int iterations=1);

  void moveMaterial(cv::Mat& img, Point move_from, Point move_to, float amount);

  /**
   * \brief Get normalized watermap for viewing
   */
  cv::Mat getWatermap() const;

  /**
   * \brief Get normalized sedimentmap for viewing
   */
  cv::Mat getSedimentmap() const;

private:

  void HydraulicErosion::rain();

  void HydraulicErosion::erosion(cv::Mat& heightmap);
  
  void HydraulicErosion::transfer(cv::Mat& heightmap);

  void HydraulicErosion::evaporate(cv::Mat& heightmap);

  cv::Mat watermap;
  cv::Mat sedimentmap;
  float k_rain;
  float k_solubility;
  float k_evaporation;
  float k_capacity;
  int rows;
  int cols;
};

} // namespace terrain