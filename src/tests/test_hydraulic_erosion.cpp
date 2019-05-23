#include <opencv2/opencv.hpp>

#include "HydraulicErosion.hpp"
#include "DiamondSquare.hpp"
#include "Voronoi.hpp"
#include "Perturb.hpp"

using namespace cv;
using namespace std;
using namespace terrain;

float x, y;
bool clicked = false;

void imshow2(std::string windowName, cv::Mat img)
{
  namedWindow(windowName, cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow(windowName, 1000, 1000);
  cv::imshow(windowName, img);
}

int main(int argc, char** argv)
{
  int iterations = std::stoi(argv[1]);
  float k_rain = 0.01, k_solubility = 0.01, k_evaporation = 0.5, k_capacity = 0.01;
  if (argc >= 6)
  {
    k_rain = std::atof(argv[2]);
    k_solubility = std::atof(argv[3]);
    k_evaporation = std::atof(argv[4]);
    k_capacity = std::atof(argv[5]);
  }

 // Get coeff for Voronoi
  std::vector<float> coeffs{ -1, 1 };

  int n = pow(2, 9) + 1; // 513 x 513 image
  int rows, cols; rows = cols = n;
  float persistence = 0.5; // Higher = more jagged terrain
  
  DiamondSquare diamondSquare;
  auto heightmap_ds = diamondSquare.generate(n, persistence, 1337);

  int n_points = 20;
  Voronoi vrn = Voronoi(rows, cols, coeffs, n_points, 1337);
  cv::Mat heightmap_vrn = vrn.generate();

  // cv::Mat combined = combine({ heightmap_ds, heightmap_vrn }, { 1 / 3, 2 / 3 });
  cv::Mat combined;
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);
  cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);
  combined = Perturb::apply(combined, 0.1);
  HydraulicErosion hydraulic(MOORE, rows, cols, k_rain, k_solubility, k_evaporation, k_capacity);
  // HydraulicErosion hydraulic(MOORE, rows, cols);
  cv::Mat eroded;
  combined.copyTo(eroded);
  hydraulic.apply(eroded, iterations);
  std::cout << "Applied!!\n";
  cv::normalize(eroded, eroded, 1, 0, cv::NORM_MINMAX);

  cv::Mat watermap = hydraulic.getWatermap();
  cv::Mat sedimentmap = hydraulic.getSedimentmap();

  imshow2("Base", combined);
  imshow2("Eroded", eroded);
  //normalize(hydraulic.watermap*255, hydraulic.watermap, 1, 0, cv::NORM_MINMAX);
  //normalize(hydraulic.sedimentmap*255, hydraulic.sedimentmap, 1, 0, cv::NORM_MINMAX);
  imshow2("watermap", watermap);
  imshow2("sedmap", sedimentmap);
  // cout << hydraulic.watermap;
  // cout << hydraulic.sedimentmap;
 
  cv::waitKey(0);

  cv::Mat eroded_img;
  cv::Mat base_img;
  eroded *= 255;
  eroded.convertTo(eroded_img, CV_8UC1);
  combined *= 255;
  combined.convertTo(base_img, CV_8UC1);
  cv::imwrite("../../examples/hydraulic_erosion_base.png", base_img);
  cv::imwrite("../../examples/hydraulic_erosion_eroded.png", eroded_img);

  return 0;
}