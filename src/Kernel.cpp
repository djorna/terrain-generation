#include "Kernel.hpp"

// Kernel::Kernel() {}

// Kernel::Kernel(KernelType kernel_type) : kernel_type(kernel_type) {}

std::vector<Point> Kernel::neighbours(Point point, int rows, int cols)
{
  int x = point.x;
  int y = point.y;
  std::vector<Point> points;
  
  switch(kernel_type)
  {
    case MOORE:
      points = {Point(x-1, y-1), Point(x, y-1),  Point(x+1, y-1),
                Point(x-1, y),   Point(x, y),   Point(x+1, y),
                Point(x-1, y+1), Point(x, y+1), Point(x+1, y+1)};
      break;

    case VON_NEUMANN:
      points = {               Point(x, y-1),
                Point(x-1, y), Point(x, y),   Point(x+1, y),
                               Point(x, y+1)};
      break;

    case VON_NEUMANN2:
       points = {Point(x-1, y-1),              Point(x+1, y-1),
                                  Point(x, y),  
                Point(x-1, y+1),               Point(x+1, y+1)};
        break;
  };

  // Cull outlier points
  for (auto it = points.begin(); it != points.end(); ) {
    if(!pointInRange(it->y, it->x, rows, cols))
      points.erase(it);
    else
      ++it;
  }

  return points;        
}

void Kernel::applyOnce(cv::Mat& img, cv::Point2i center)
{
  operation(img, center, neighbours(center, img.rows, img.cols));
}

void Kernel::apply(cv::Mat& img)
{
  for (int r = 0; r < img.rows; ++r)
    for (int c = 0; c < img.cols; ++c)
      applyOnce(img, cv::Point2i(c, r));
}