
#include <array>
#include <string>

// Random number generation
#include <time.h>
#include <cstdlib> 

// OpenCV includes
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

template <typename T>
class DiamondSquare 
{
typedef Corners std::array<int, 4>;

public:
  DiamondSquare(int n) : {n(n)};

  ~DiamondSquare();

  void generate(int n, std::array<int, 4> corners, float decay, bool display);

private:

  /** \brief Perform a complete "diamond" step.
   * 
   * \param k The current iteration number
   * \param p The current weight coefficient 
   */
  void diamond(int k, float p);

  /** \brief Perform a complete "square" step.
   * 
   * \param k The current iteration number
   * \param p The current weight coefficient 
   */
  void square(int k, float p);

  /** \brief Helper method to perform a single "square" operation.
   * 
   * \param row The row number of the center of the square.
   * \param col The column number of the center of the square.
   * \param k The current depth of the algorithm.
   * \param img The current elevation map.
   * \param p The current weight coefficient. 
   */
  void applySquare(int row, int col, int k, float p);


  /** \brief Helper method to perform a single "diamond" operation.
   * 
   * \param row The row number of the center of the square.
   * \param col The column number of the center of the square.
   * \param k The current depth of the algorithm.
   * \param img The current elevation map.
   * \param p The current weight coefficient. 
   */
  void applyDiamond(int row, int col, int k, float p);

  void show();


  cv::Mat elevation_map;
}