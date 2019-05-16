#include <opencv2/opencv.hpp>

#include "Evaluator.hpp"
#include "DiamondSquare.hpp"

using namespace std;
using namespace cv;
using namespace terrain;

int main(int argc, char** argv)
{
  int x = 9;
  float persistence = 0.5; // Higher = more jagged terrain

  if (argc > 1){
    int t = std::stoi(argv[1]);
    if (x > 1 && x < 13) x = t;
    if (argc > 2)
      persistence = std::atof(argv[2]);
  }
  int n = pow(2, x) + 1;
  std::cout << "Creating map of size " << n << "\n";

  namedWindow("Generated terrain", cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow("Generated terrain", 1000, 1000);

  DiamondSquare diamondSquare;

  cv::Mat heightmap = diamondSquare.generate(n, persistence, -1);

  float building_thres = 2 / n;
  float unit_thres = 8 / n;
  Evaluator eval(heightmap, building_thres, unit_thres);

  cv::imshow("Generated terrain", heightmap);
  cv::imshow("Slope", eval.getSlopeMap());
  cv::imshow("Flatness", eval.getFlatnessMap());
  cv::imshow("Building", eval.getBuildingMap());
  cv::imshow("Accessibility", eval.getAccessibilityMap());
  cv::imshow("Unit Map", eval.getUnitMap());

  cv::waitKey(0);
}