#include "DiamondSquare.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>

using namespace cv;
using namespace std;
using namespace terrain;

int main(int argc, char** argv) {
  
  int x = 2; // Defauts to 2 (size of 5)
  float persistence = 0.5; // Higher = more jagged terrain
  int seed = 1337;

  if (argc > 1){
    int t = std::stoi(argv[1]);
    if (x > 1 && x < 13) x = t;
    if (argc > 2)
      persistence = std::atof(argv[2]);
    if (argc > 3)
      seed = std::stoi(argv[3]);
  }
  int n = pow(2, x) + 1;
  std::cout << "Creating map of size " << n << "\n";

  namedWindow("Generated terrain", cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow("Generated terrain", 1000, 1000);

  std::cout << "Init object...";
  
  DiamondSquare diamondSquare;

  cv::Mat heightmap = diamondSquare.generate(n, persistence, seed);
  cv::imshow("Generated terrain", heightmap);

  cv::waitKey(0);
  
  return 0;
}