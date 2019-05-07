#include <iostream>
#include <opencv2/opencv.hpp>

#include "Voronoi.hpp"
#include "DiamondSquare.hpp"

// For debugging
#include <fstream>

using namespace std;
using namespace cv;

void MaskPlugin(Mat& masked_img, int n, int n_points, vector<float> vcoeffs, float persistence, std::string mask_file_str)
{
  int rows = n, cols = n;
  Voronoi vrn = Voronoi(rows, cols, vcoeffs, n_points);
  cv::Mat heightmap_vrn = vrn.generate();

  DiamondSquare ds;
  cv::Mat heightmap_ds = ds.generate(n, persistence);

  cv::Mat combined;
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);
  cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);
  
  // Apply binary mask
  cv::Mat mask = cv::imread(mask_file_str, cv::IMREAD_GRAYSCALE);
  cv::bitwise_and(combined, combined, masked_img, mask);
}

std::string convert(char* data, int len)
{
   return std::string(data, std::find(data, data + len, '\0'));
}

int main(int argc, char** argv)
{
  // Get mask file
  if (argc < 2)
  {
    cerr << "Not enough input arguments.";
    return -1;
  }
  string mask_file = argv[1];

  // Get coeff for Voronoi
  std::vector<float> coeffs;
  for (int i = 2; i < argc; ++i)
    coeffs.push_back(std::stoi(argv[i]));

  Mat img;
  Mat masked_img;

  // Get mask
  Mat mask = imread(mask_file, IMREAD_GRAYSCALE);

  // Generate terrain

  int n = pow(2, 9) + 1; // 513 x 513 image
  int rows, cols; rows = cols = n;
  float persistence = 0.5; // Higher = more jagged terrain
  
  DiamondSquare diamondSquare;
  auto heightmap_ds = diamondSquare.generate(n, persistence);

  int n_points = 50;
  Voronoi vrn = Voronoi(rows, cols, coeffs, n_points);
  auto heightmap_vrn = vrn.generate();

  // cv::Mat combined = combine({ heightmap_ds, heightmap_vrn }, { 1 / 3, 2 / 3 });
  cv::Mat combined;
  cv::addWeighted(heightmap_ds, 0.67, heightmap_vrn, 0.33, 0, combined);

  cv::normalize(combined, combined, 1, 0, cv::NORM_MINMAX);

  // Apply binary mask 
  bitwise_and(combined, combined, masked_img, mask);

  Mat masked_img2;
  MaskPlugin(masked_img2, rows, n_points, coeffs, persistence, mask_file);

  ofstream myfile;
  myfile.open("example.txt");
  myfile << mask_file;
  myfile.close();

  cout << "masked_img type: " << masked_img.type();
  cout << "masked_img pixel: " << masked_img.at<float>(0, 0);
  cout << "mask pixel: ";
  cout << mask.at<uchar>(0, 0);
  /*
  imshow("Masked", masked_img);
  imshow("Mask", mask);
  imshow("Masked2", masked_img2);
  waitKey(0);
  */

  return 0;
}