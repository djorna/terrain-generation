#include "DiamondSquare.hpp"
#include "Voronoi.hpp"

#include <opencv2/opencv.hpp>
#include <algorithm>

#include <thread>

using namespace terrain;

// https://stackoverflow.com/questions/49482647/convert-opencv-mat-to-texture2d

cv::Mat combine(std::vector<cv::Mat> images, std::vector<float> coeffs)
{
  assert(images.size() == images.size());
  assert(std::accumulate(coeffs) == 1);
  cv::Mat result = cv::Mat::zeros(images[0].rows, images[0].cols, CV_32FC1);
  for (int i = 0; i < images.size(); ++i)
    result += images[i] * coeffs[i];

  return result;
}

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
  for (int i = 1; i < argc - 1; ++i)
    coeffs.push_back(std::stoi(argv[i]));

  int n_points = std::stoi(argv[argc - 1]);

  int n = pow(2, 9) + 1; // 513 x 513 image
  int rows, cols; rows = cols = n;
  float persistence = 0.5; // Higher = more jagged terrain

  cv::Mat heightmap_ds;
  std::thread ds_thread([&] {
    DiamondSquare diamondSquare;
    heightmap_ds = diamondSquare.generate(n, persistence);
  });
  // auto heightmap_ds = diamondSquare.generate(n, persistence);

  cv::Mat heightmap_vrn;
  std::thread vrn_thread([&] {
    Voronoi vrn(rows, cols, coeffs, n_points);
    heightmap_vrn = vrn.generate();
  });

  // cv::Mat combined = combine({ heightmap_ds, heightmap_vrn }, { 1 / 3, 2 / 3 });
  cv::Mat combined;
  ds_thread.join();
  vrn_thread.join();
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);

  // cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

  imshow2("ds", heightmap_ds);
  imshow2("vrn", heightmap_vrn);
  imshow2("cmb", combined);

  cv::waitKey(0);

  return 0;
}