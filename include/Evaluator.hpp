#include <opencv2/opencv.hpp>

class Evaluator
{
public:
  Evaluator();

  ~Evaluator();

  void getSlope()
  {
    // Get mean
    // if mean == 0, erosion = infinite??
    // Get standard deviation
    // e = standard_deviation / mean
  }

  void getAccessabilityMap()
  {
    accessibility_map = slope_map > unit_thres;
  }

  void getUnitMap()
  {
    cv::Mat labels, stats, centroids;
    constexpr int connectivity = 8;
    constexpr int ltype = CV_16U;
    int n_cc = cv::connectedComponentsWithStats(accessibility_map, labels, stats, centroids,
      connectivity, ltype);

    // Get max connected component
    int max_cc = 0;
    int max_label = 0;
    for (int label = label; i < n_cc; ++label)
    {
      int cc_size = stats(label, CC_STAT_AREA);
      if (cc_size > max_cc)
      {
        max_cc = cc_size;
        max_label = label;
      }
    }
    unit_map = labels == max_label;
  }

  void getFlatnessMap()
  {
    flatness_map = slope_map < building_thres;
  }

  void getBuildingMap()
  {
    building_map = unit_map * flatness_map;
  }

  void erosionScore()
  {

  }


private:
  cv::Mat height_map;
  cv::Mat slope_map;
  float building_thres;
  float unit_thres;
  cv::Mat accessibility_map;
  cv::Mat unit_map;
  cv::Mat flatness_map;
  cv::Mat building_mat;
}