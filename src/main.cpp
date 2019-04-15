#include <iostream>
#include "DiamondSquare.hpp"

#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  
  int x = 2; // Defauts to 2 (size of 5)
  if (argc > 1){
    int t = std::stoi(argv[1]);
    if (x > 1 && x < 13) x = t;
  }
  int n = pow(2, x) + 1;
  srand(time(NULL));
  std::cout << "Creating map of size " << n << "\n";

  namedWindow("Generated terrain", cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow("Generated terrain", 1000, 1000);

  std::cout << "Init object...";
  
  float persistence = 0.5; // Higher = more jagged terrain
  DiamondSquare diamondSquare;

  auto heightmap = diamondSquare.generate(n, persistence);
  cv::imshow("Generated terrain", heightmap);
  
  while(true) {
    char c = cv::waitKey(0);
    if (c == 27) break; // Spin until esc pressed
  }
  
  std::ostringstream os;
  os << "./examples/" << n << "x" << n << ".png";
  imwrite(os.str(), heightmap);

  std::cout << "Saved image as " << os.str() << "\n";
  
  return 0;
}