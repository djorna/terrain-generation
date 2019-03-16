#include <iostream>

#include "TerrainGenerator.hpp"

int main(int argc, char** argv) {
  
  // Create empty image
  int x = 2; // Defauts to 2 (size of 5)
  if (argc > 1){
    int t = std::stoi(argv[1]);
    if (x > 1 && x < 13) x = t;
  }
  int n = pow(2, x) + 1;
  srand(time(NULL));
  std::cout << "Creating map of size " << n << "\n";
  cv::Mat heightMap(cv::Size(n, n), CV_8U, cv::Scalar(0));
  std::array<int, 4> corners = {randInt(), randInt(), randInt(), randInt()};
  namedWindow("Generated terrain", cv::WINDOW_NORMAL );// Create a window for display.
  cv::resizeWindow("Generated terrain", 1000, 1000);

  float persistence = 0.5; // Higher = more jagged terrain
  diamondSquare = DiamondSquare(n);
  diamondSquare.generate();
  diamondSquare<uchar>(n, heightMap, corners, persistence);
  imshow("Generated terrain", heightMap);

  while(true) {
    char c = cv::waitKey(0);
    if (c == 27) break; // Spin until esc pressed
  }

  std::ostringstream os;

  os << "./examples/" << n << "x" << n << ".png";

  cv::imwrite(os.str(), heightMap);

  std::cout << "Saved image as " << os.str() << "\n";

  return 0;
}