#include "Voronoi.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


int main(int argc, char** argv) {

  int rows = 512, cols = 512;
  // vector<float> coeffs{ -1, 1 };
  // vector<float> coeffs{0, 1 };

  // Read command line args
  vector<float> coeffs;
  for (int i = 1; i < argc; ++i)
    coeffs.push_back(stoi(argv[i]));

  //vector<float> coeffs{ -1 };
  int n_points = 10;
  Voronoi vrn = Voronoi(rows, cols, coeffs, n_points);
  cout << "Generating points...\n";
  vrn.generatePoints(n_points, rows, cols);
  Mat img;
  img = Mat::zeros(rows, cols, CV_32FC1);
  cout << "Drawing points...\n";
  vrn.drawPoints(img);


  imshow("Points", img);

  cout << "Creating map...\n";
  auto vrn_img = vrn.generate();
  // cv::Mat uint_img(rows, cols, CV_8UC1);
  cv::Mat uint_img;
  // vrn_img *= 10;
  vrn_img.convertTo(uint_img, CV_8UC1);
  for (int i = 0; i < rows; ++i)
    for (int j = 0; j < cols; ++j)
      uint_img.at<uchar>(i, j) = vrn_img.at<float>(i, j) * 255;

  cout << "Creating window" << endl;
  namedWindow("Voronoi", WINDOW_AUTOSIZE);
  imshow("Voronoi", vrn_img);
  imshow("Voronoi2", uint_img);

  waitKey(0);

  return 0;
}