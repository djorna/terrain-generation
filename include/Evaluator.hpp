#include <opencv2/opencv.hpp>

namespace terrain
{

class Evaluator
{
public:
  Evaluator(cv::Mat height_map, float building_thres, float unit_thres);

  static cv::Mat getSlope(cv::Mat heightmap);

  // Getters
  cv::Mat getSlopeMap();
  cv::Mat getFlatnessMap();
  cv::Mat getBuildingMap();
  cv::Mat getAccessibilityMap();
  cv::Mat getUnitMap();
  float getErosionScore();

private:
  void computeSlope();
  void computeAccessibilityMap();
  void computeUnitMap();
  void computeFlatnessMap();
  void computeBuildingMap();

  // Get mean
  // if mean == 0, erosion = infinite??
  // Get standard deviation
  // e = standard_deviation / mean
  void computeErosionScore();

  cv::Mat slope_map;
  cv::Mat height_map;
  cv::Mat building_map;
  cv::Mat accessibility_map;
  cv::Mat unit_map;
  cv::Mat flatness_map;

  float building_thres;
  float unit_thres;
  float erosion_score;
};

}