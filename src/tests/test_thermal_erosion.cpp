#include <opencv2/opencv.hpp>

#include "ThermalErosion.hpp"
#include "DiamondSquare.hpp"
#include "Voronoi.hpp"

void imshow2(std::string windowName, cv::Mat img)
{
  namedWindow(windowName, cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow(windowName, 1000, 1000);
  cv::imshow(windowName, img);
}

int main(int argc, char** argv)
{
 // Get coeff for Voronoi
  std::vector<float> coeffs;
  for (int i = 1; i < argc; ++i)
    coeffs.push_back(std::stoi(argv[i]));

  int n = pow(2, 9) + 1; // 513 x 513 image
  int rows, cols; rows = cols = n;
  float persistence = 0.5; // Higher = more jagged terrain
  
  DiamondSquare diamondSquare;
  auto heightmap_ds = diamondSquare.generate(n, persistence);

  int n_points = 50;
  Voronoi vrn(rows, cols, coeffs, n_points);
  auto heightmap_vrn = vrn.generate();

  // cv::Mat combined = combine({ heightmap_ds, heightmap_vrn }, { 1 / 3, 2 / 3 });
  cv::Mat combined;
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);
  cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

  ThermalErosion thermal(MOORE);
  cv::Mat eroded;
  combined.copyTo(eroded);
  thermal.apply(eroded);
  cv::normalize(eroded, eroded, 1, 0, cv::NORM_MINMAX);

  imshow2("Base", combined);
  imshow2("Eroded", eroded);

  /*
  for (int i = 0; i < 513; ++i) {
    for (int j = 0; j < 513; ++j) {
      if (combined.at<float>(i, j) != eroded.at<float>(i, j))
        std::cout << "Wow!" << combined.at<float>(i, j) - eroded.at<float>(i, j) << std::endl;
    }
  }
  */

  cv::waitKey(0);

  return 0;
}