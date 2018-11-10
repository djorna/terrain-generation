
#include <array>
#include <cstdlib> // For random numbers
#include <iostream>
#include <string>
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define MAX_VAL 255/2

int randInt(int min=0, int max=MAX_VAL) { return rand() % (max - min) + min; }

template <typename T>
void applySquare(int row, int col, int k, cv::Mat& img, float p) {
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
template <typename T>
void applyDiamond(int row, int col, int k, cv::Mat& img, float p) {
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

template <typename T>
void diamond(int k, cv::Mat& img, float p) {
  int stride = k - 1;
  //int nApplications = pow((img.rows() - 1) / (k - 1), 2);

  for (int i = k / 2; i < img.rows; i += stride) {
    for (int j = k / 2; j < img.cols; j += stride) {
      applyDiamond<T>(i, j, k, img, p);
    }
  }
}

template <typename T>
void square(int k, cv::Mat& img, float p) {
  int stride = k - 1;

  for (int i = k / 2; i < img.rows; i += stride)
    for (int j = 0; j < img.cols; j += stride)
      applySquare<T>(i, j, k, img, p);

  for (int i = 0; i < img.rows; i += stride)
    for (int j = k / 2; j < img.cols; j += stride)
      applySquare<T>(i, j, k, img, p);
}


template <typename T>
void diamondSquare(int n, cv::Mat& img, std::array<int, 4> corners, float decay) {
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