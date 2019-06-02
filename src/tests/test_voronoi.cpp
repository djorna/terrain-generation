#include "Voronoi.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace std::chrono;
using namespace cv;
using namespace terrain;


int main(int argc, char** argv) {

  int rows = 512, cols = 512;
  // vector<float> coeffs{ -1, 1 };
  // vector<float> coeffs{0, 1 };

  // Read command line args
  vector<float> coeffs;
  for (int i = 1; i < argc - 1; ++i)
    coeffs.push_back(stoi(argv[i]));

  int n_points = stoi(argv[argc - 1]);

  int* ptr;
  if (ptr)
    cout << "Null!!\n";

  char key = 0;
  while (key != 0x71)
  {
    //vector<float> coeffs{ -1 };
    auto start = high_resolution_clock::now();
    Voronoi vrn(rows, cols, coeffs, n_points, 1337);
    auto constructed = high_resolution_clock::now();
    // vrn.binaryMask(1, 2411);
    auto vrn_img = vrn.generate();
    auto generated = high_resolution_clock::now();
    Mat points_img;
    points_img = Mat::zeros(rows, cols, CV_32FC1);
    cout << "Drawing points...\n";
    vrn.drawPoints(points_img);

    imshow("Points", points_img);

    cout << "Creating map...\n";
    float mean = 0.5;
    float stdev = 0.2;
    int shift_seed = 1231;
    // vrn.shiftHeightMask(mean, stdev, shift_seed);

    auto duration1 = duration_cast<milliseconds>(constructed - start);
    auto duration2 = duration_cast<milliseconds>(generated - constructed);
    std::cout << " Duration (get points): " << duration1.count() << " milliseconds\n";
    std::cout << " Duration (knn): " << duration2.count() << " milliseconds\n";

    cout << "Creating window" << endl;
    namedWindow("Voronoi", WINDOW_AUTOSIZE);
    imshow("Voronoi", vrn_img);

    key = waitKey(0);
}

  /*
  cv::Mat vrn_uint8;
  vrn_img *= 255;
  vrn_img.convertTo(vrn_uint8, CV_8UC1);
  cv::imwrite("voronoi.png", vrn_uint8);
  */

  return 0;
}