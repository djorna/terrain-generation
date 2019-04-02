#pragma once

#include <array>
#include <string>

// Random number generation
#include <time.h>
#include <cstdlib> 

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class DiamondSquare 
{

public:
  DiamondSquare();

  ~DiamondSquare();

  /** \brief Generate a heightmap with the given initial parameters
   * 
   * \param n The side length of the image. The side length must satisfy n = 2^x + 1
   * where x is a positive integer
   * \param decay The decay parameter. Higher values will result in greater variance in
   * height
   * \param corners The corner values to initialize the heightmap. Should be integer values
   * between 0 and 255.
   */
  cv::Mat generate(const int n, const std::array<int, 4> corners, const float decay=0.5);

  /** \brief Generate a heightmap with a random initialization.
   * 
   * \param n The side length of the image. The side length must satisfy n = 2^x + 1
   * where x is a positive integer
   * \param decay The decay parameter. Higher values will result in greater variance in
   * height
   */
  cv::Mat generate(int n, float decay=0.5);

private:

  int randInt(int min, int max);

  /** \brief Perform a complete "diamond" step.
   * 
   * \param k The current iteration number
   * \param p The current weight coefficient 
   */
  void diamond(cv::Mat& heightmap, int k, float p);

  /** \brief Perform a complete "square" step.
   * 
   * \param k The current iteration number
   * \param p The current weight coefficient 
   */
  void square(cv::Mat& heightmap, int k, float p);

  /** \brief Helper method to perform a single "square" operation.
   * 
   * \param row The row number of the center of the square.
   * \param col The column number of the center of the square.
   * \param k The current depth of the algorithm.
   * \param img The current elevation map.
   * \param p The current weight coefficient. 
   */
  void applySquare(cv::Mat& heightmap, const int row, const int col, const int k, const float p);


  /** \brief Helper method to perform a single "diamond" operation.
   * 
   * \param row The row number of the center of the square.
   * \param col The column number of the center of the square.
   * \param k The current depth of the algorithm.
   * \param img The current elevation map.
   * \param p The current weight coefficient. 
   */
  void applyDiamond(cv::Mat& heightmap, int row, int col, int k, float p);

  void diamondSquare(cv::Mat& heightmap, const int n, const float decay=0.5);
};