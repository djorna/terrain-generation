#include "DiamondSquare.hpp"
#include "Voronoi.hpp"
#include "SimplexNoise.hpp"
#include "Perturb.hpp"

#include <opencv2/opencv.hpp>
#include <cmath>
#include <thread>

using namespace terrain;

int main(int argc, char** argv)
{
  float mag = std::atof(argv[1]);

  // Get Combined
  int n = pow(2, 9) + 1; // 513 x 513 image
  int rows, cols; rows = cols = n;
  float persistence = 0.5; // Higher = more jagged terrain

  cv::Mat heightmap_ds;
  std::thread ds_thread([&] {
    DiamondSquare diamondSquare;
    heightmap_ds = diamondSquare.generate(n, persistence, 1337);
  });
  // auto heightmap_ds = diamondSquare.generate(n, persistence);

  int n_points = 20;

  cv::Mat heightmap_vrn;
  std::vector<float> coeffs = { -1, 1 };
  std::thread vrn_thread([&] {
    Voronoi vrn(rows, cols, coeffs, n_points, 1337);
    heightmap_vrn = vrn.generate();
  });

  // cv::Mat combined = combine({ heightmap_ds, heightmap_vrn }, { 1 / 3, 2 / 3 });
  cv::Mat combined;
  ds_thread.join();
  vrn_thread.join();
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);

  cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);
  /*
  (float frequency = 1.0f,
    float amplitude = 1.0f,
    float lacunarity = 2.0f,
    float persistence = 0.5f) :
  int count = 100;
  cv::createTrackbar("frequency", "trackbars", int* value, count);
    cv::createTrackbar("amplitude", "trackbars", int* value, count);
    cv::createTrackbar("lacunarity", "trackbars", int* value, count);
    cv::createTrackbar("persistence", "trackbars", int* value, count);
    */
  //cv::Mat terrain_pt = perturb(combined, mag);
  cv::Mat terrain_pt = Perturb::apply(combined, mag);

  cv::imshow("Terrain", combined);
  cv::imshow("Perturbed terrain", terrain_pt);

  cv::waitKey(0);

  cv::Mat pt_img;
  terrain_pt *= 255;
  terrain_pt.convertTo(pt_img, CV_8UC1);
  cv::imwrite("../../examples/perturb.png", pt_img);
  
}