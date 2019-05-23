#include <opencv2/opencv.hpp>

#include "ThermalErosion.hpp"
#include "DiamondSquare.hpp"
#include "Voronoi.hpp"
#include "Perturb.hpp"

using namespace std;
using namespace std::chrono;
using namespace terrain;

void imshow2(std::string windowName, cv::Mat img)
{
  namedWindow(windowName, cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow(windowName, 1000, 1000);
  cv::imshow(windowName, img);
}

int main(int argc, char** argv)
{
  int iterations = std::stoi(argv[1]);

  int n = pow(2, 9) + 1; // 513 x 513 image
  int rows, cols; rows = cols = n;
  float persistence = 0.5; // Higher = more jagged terrain
  
  DiamondSquare diamondSquare;
  auto heightmap_ds = diamondSquare.generate(n, persistence, 1337);

  std::vector<float> coeffs{ -1, 1 };
  int n_points = 20;
  Voronoi vrn(rows, cols, coeffs, n_points, 1337);
  auto heightmap_vrn = vrn.generate();

  // cv::Mat combined = combine({ heightmap_ds, heightmap_vrn }, { 1 / 3, 2 / 3 });
  cv::Mat combined;
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);
  cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

  combined = Perturb::apply(combined, 0.1);

  ThermalErosion thermal(MOORE);
  cv::Mat eroded;
  combined.copyTo(eroded);
  cout << "Starting...\n";
  auto start = high_resolution_clock::now(); 
  thermal.apply(eroded, iterations);
  auto end = high_resolution_clock::now(); 
  cv::normalize(eroded, eroded, 1, 0, cv::NORM_MINMAX);

  auto duration = duration_cast<milliseconds>(end - start); 
  cout << iterations << " iterations took " << duration.count() << " milliseconds.\n";

  imshow2("Base", combined);
  imshow2("Eroded", eroded);

  cv::waitKey(0);

  cv::Mat eroded_img;
  cv::Mat base_img;
  eroded *= 255;
  eroded.convertTo(eroded_img, CV_8UC1);
  combined *= 255;
  combined.convertTo(base_img, CV_8UC1);
  cv::imwrite("../../examples/thermal_erosion_base.png", base_img);
  cv::imwrite("../../examples/thermal_erosion_eroded.png", eroded_img);

  return 0;
}