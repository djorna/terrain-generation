#include "TerrainGenerator.hpp"

#define MAX_VAL 255/2

TerrainGenerator::TerrainGenerator()
{
  // Initialize corners randomly
  elevation_map.at<T>(0, 0) = randInt();
  elevation_map.at<T>(0, n - 1) = randInt();
  elevation_map.at<T>(n - 1, 0) = randInt();
  elevation_map.at<T>(n - 1, n - 1) = randInt();
}

TerrainGenerator::TerrainGenerator(Corners corners)
{
  // Initialize corners
  elevation_map.at<T>(0, 0) = corners[0];
  elevation_map.at<T>(0, n - 1) = corners[1];
  elevation_map.at<T>(n - 1, 0) = corners[2];
  elevation_map.at<T>(n - 1, n - 1) = corners[3];
}

TerrainGenerator::~TerrainGenerator() {}

int randInt(int min=0, int max=MAX_VAL) { return rand() % (max - min) + min; }

void TerrainGenerator::applySquare(int row, int col, int k, cv::Mat& img, float p) {
  int step = k/2;
  float result = 0;
  float i = 0;
  if (col - step > 0){
    result += img.at<T>(row, col - step);
    ++i;
  }
  if (col + step < img.cols) {
    result += img.at<T>(row, col + step);
    ++i;
  }
  if (row - step > 0) { 
    result += img.at<T>(row - step, col);
    ++i;
  }
  if (row + step < img.rows) {
    result += img.at<T>(row + step, col);
    ++i;
  }
  result /= i;
  result += randInt() * p;
  img.at<T>(row, col) = result;
}

void TerrainGenerator::applyDiamond(int row, int col, int k, cv::Mat& img, float p) {
  int step = k/2;
  float result = 0;
  result = img.at<T>(row - step, col - step)
         + img.at<T>(row + step, col - step)
         + img.at<T>(row - step, col + step)
         + img.at<T>(row + step, col + step);
  result /= 4;
  result += randInt() * p;
  img.at<T>(row, col) = result;
}

void TerrainGenerator::diamond(int k, cv::Mat& img, float p) {
  int stride = k - 1;
  //int nApplications = pow((img.rows() - 1) / (k - 1), 2);

  for (int i = k / 2; i < img.rows; i += stride) {
    for (int j = k / 2; j < img.cols; j += stride) {
      applyDiamond<T>(i, j, k, img, p);
    }
  }
}

void TerrainGenerator::square(int k, cv::Mat& img, float p) {
  int stride = k - 1;

  for (int i = k / 2; i < img.rows; i += stride)
    for (int j = 0; j < img.cols; j += stride)
      applySquare<T>(i, j, k, img, p);

  for (int i = 0; i < img.rows; i += stride)
    for (int j = k / 2; j < img.cols; j += stride)
      applySquare<T>(i, j, k, img, p);
}

void generate(int n, cv::Mat& img, std::array<int, 4> corners, float decay, bool display);

void TerrainGenerator::generate(int n, float decay=0.5) {
  // Initialize corners
  img.at<T>(0, 0) = corners[0];
  img.at<T>(0, n - 1) = corners[1];
  img.at<T>(n - 1, 0) = corners[2];
  img.at<T>(n - 1, n - 1) = corners[3];

  int k = n;
  float p = 1;
  for (int k = n; k > 2; k = (k + 1) / 2) {
    diamond<T>(k, img, p);
    cv::imshow("Generated terrain", img);
    cv::waitKey(0);
    square<T>(k, img, p);
    cv::imshow("Generated terrain", img);

    cv::waitKey(0);

    p *= decay;
  }
}