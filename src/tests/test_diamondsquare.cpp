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

  cv::VideoWriter writer;
  int codec = cv::VideoWriter::fourcc('M','J','P','G');
  std::string filename = "./diamond_square_demo.avi";             // name of the output video file
  double fps = 3;
  writer.open(filename, codec, fps, cv::Size(n, n), false);

  cout << "Recording video...";
  cv::Mat heightmap  = diamondSquare.generate(n, persistence, seed);
  cv::imshow("Generated terrain", heightmap);

  cv::Mat ds_img;
  heightmap *= 255;
  heightmap.convertTo(ds_img, CV_8UC1);
  cv::imwrite("../../examples/diamond_square.png", ds_img);

  /*
  for (persistence = 0.1; persistence < 1; persistence += 0.05)
  {
    heightmap = diamondSquare.generate(n, persistence, ++seed);
    heightmap *= 255;
    heightmap.convertTo(heightmap, CV_8UC1);
    stringstream ss;
    ss << setprecision(3) << "Persistence = " << persistence;
    cv::putText(heightmap, ss.str(),
      { 20, 20 }, cv::FONT_HERSHEY_SIMPLEX, 0.5, 255, 1, cv::LINE_AA);
    // cv::imshow("Frame", heightmap);
    // cv::waitKey(1);
    writer.write(heightmap);
  }
  cout << "done.\n";
  */

  cv::waitKey(0);
  
  return 0;
}