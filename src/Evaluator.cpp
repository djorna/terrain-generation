#include "Evaluator.hpp"
#include "Kernel.hpp"
#include <cmath>

namespace terrain
{

Evaluator::Evaluator(cv::Mat hm, float bt, float ut)
  : height_map(hm), building_thres(bt), unit_thres(ut)
{
  computeSlope();
  computeAccessibilityMap();
  computeUnitMap();
  computeFlatnessMap();
  computeBuildingMap();
  computeErosionScore();
}

cv::Mat Evaluator::getSlope(cv::Mat heightmap)
{
  cv::Mat slope = cv::Mat(heightmap.rows, heightmap.cols, CV_32FC1);
  // Kernel kernel(VON_NEUMANN2);
  for (int i = 0; i < slope.rows; ++i)
  {
    for (int j = 0; j < slope.cols; ++j)
    {
      Point pt(j, i);
      float height = heightmap.at<float>(i, j);
      auto neighbours = Kernel::neighbours(pt, heightmap.rows, heightmap.cols, VON_NEUMANN2);
      std::vector<float> delta_heights;
      for (Point nb : neighbours)
        delta_heights.push_back(std::abs(height - heightmap.at<float>(nb)));
      
      slope.at<float>(i, j) = *std::max_element(delta_heights.begin(), delta_heights.end());

      //slope.at<float>(i, j) = std::max({
        //height - heightmap.at<float>(i - 1, j), heightmap.at<float>(i + 1, j),
        //height - heightmap.at<float>(i, j - 1), heightmap.at<float>(i, j + 1)});
    }
  }
  return slope;
}

void Evaluator::computeSlope()
{
  slope_map = cv::Mat(height_map.rows, height_map.cols, CV_32FC1);
  for (int i = 0; i < slope_map.rows; ++i)
  {
    for (int j = 0; j < slope_map.cols; ++j)
    {
      Point pt(j, i);
      float height = height_map.at<float>(i, j);
      auto neighbours = Kernel::neighbours(pt, height_map.rows, height_map.cols, VON_NEUMANN2);
      std::vector<float> delta_heights;
      for (Point nb : neighbours)
        delta_heights.push_back(std::abs(height - height_map.at<float>(nb)));
      
      slope_map.at<float>(i, j) = *std::max_element(delta_heights.begin(), delta_heights.end());
    }
  }
  cv::imshow("Debug Heightmap", height_map);
  cv::Mat slope_debug;
  cv::normalize(slope_map, slope_debug, 1, 0, cv::NORM_MINMAX);
  cv::imshow("Debug Slope", slope_debug);
  cv::waitKey(0);
}

void Evaluator::computeAccessibilityMap()
{
  cv::threshold(slope_map, accessibility_map, unit_thres, 1, cv::THRESH_BINARY);
  cv::Mat access_debug;
  cv::normalize(accessibility_map, access_debug, 1, 0, cv::NORM_MINMAX);
  cv::imshow("Debug Access", access_debug);
  cv::waitKey(0);
}

void Evaluator::computeUnitMap()
{
  // Problems!!
  cv::Mat labels, stats, centroids;
  constexpr int connectivity = 8;
  constexpr int ltype = CV_16U;
  int n_cc = cv::connectedComponentsWithStats(accessibility_map, labels, stats, centroids,
    connectivity, ltype);

  // Get max connected component
  int max_cc = 0;
  int max_label = 0;
  for (int label = label; label < n_cc; ++label)
  {
    int cc_size = stats.at<int>(label, cv::CC_STAT_AREA);
    if (cc_size > max_cc)
    {
      max_cc = cc_size;
      max_label = label;
    }
  }
  unit_map = labels == max_label;
}

void Evaluator::computeFlatnessMap()
{
  flatness_map = slope_map < building_thres;
}

void Evaluator::computeBuildingMap()
{
  building_map = unit_map * flatness_map;
}

void Evaluator::computeErosionScore()
{
  float slope_mean = 0;
  for (int i = 0; i < slope_map.rows; ++i)
    for (int j = 0; j < slope_map.cols; ++j)
      slope_mean += slope_map.at<float>(i, j);

  float denom = slope_map.rows * slope_map.cols;
  slope_mean /= denom;

  float slope_stdev;
  if (slope_mean != 0)
  {
    for (int i = 0; i < slope_map.rows; ++i)
      for (int j = 0; j < slope_map.cols; ++j)
        slope_stdev += pow(slope_map.at<float>(i, j) - slope_mean, 2);
    slope_stdev /= denom;
    slope_stdev = sqrt(slope_stdev);
    erosion_score = slope_stdev / slope_mean;
  }
  else
  {
    erosion_score = -1;
  }
}

cv::Mat Evaluator::getSlopeMap() { return slope_map;  }
cv::Mat Evaluator::getFlatnessMap() { return flatness_map; }
cv::Mat Evaluator::getBuildingMap() { return building_map; }
cv::Mat Evaluator::getAccessibilityMap() { return accessibility_map;  }
cv::Mat Evaluator::getUnitMap() { return unit_map;  }
float Evaluator::getErosionScore() { return erosion_score; };

}